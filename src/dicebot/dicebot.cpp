#include "./dicebot.h"

#include <memory>
#include <regex>
#include <sstream>

#include "./utils/number.h"
#include "./utils/utils.h"
#include "./data/database_manager.h"
#include "./data/manual_dice_control.h"
#include "./data/nick_manager.h"
#include "./data/poker_manager.h"
#include "./data/profile_manager.h"
#include "./dice_roller.h"
#include "./entry/entry_base.h"
#include "./entry/entry_manager.h"
#include "./entry/entry_manual_dice.h"
#include "./entry/entry_nickname.h"
#include "./entry/entry_poker.h"
#include "./entry/entry_profile.h"
#include "./entry/entry_roll_dice.h"
#include "./entry/entry_specialized_dice.h"
#include "./random/random_provider.h"
#include "./utils/logger.h"
#include "./utils/string_utils.h"

using namespace dicebot;

std::unique_ptr<entry_manager> dice_ptcs;
profile::profile_manager* pf_ctrl;

void dicebot::initialize(const char* app_dir) {
    database::database_manager::create_instance(app_dir);
    nickname::nickname_manager::create_instance();
    manual::manual_dice_control::create_instance();
    profile::profile_manager::create_instance();
    poker::poker_manager::create_instance();

    random::initialize();
    dice_ptcs = std::make_unique<entry_manager>();
    dice_ptcs->register_dice(std::make_shared<entry::entry_roll_dice>());
    dice_ptcs->register_dice(std::make_shared<entry::entry_coc_dice>());
    dice_ptcs->register_dice(std::make_shared<entry::entry_manual_dice>());
    dice_ptcs->register_dice(std::make_shared<entry::entry_nickname>());
    dice_ptcs->register_dice(std::make_shared<entry::entry_wod_dice>());
    dice_ptcs->register_dice(std::make_shared<entry::entry_fate_dice>());
    dice_ptcs->register_dice(std::make_shared<entry::entry_set_roll>());
    dice_ptcs->register_dice(std::make_shared<entry::entry_list>());
    dice_ptcs->register_dice(std::make_shared<entry::entry_delete>());
    dice_ptcs->register_dice(std::make_shared<entry::entry_poker>());
    dice_ptcs->finish_initialization();

    poker::initialize_basic_cards();    
}

void dicebot::salvage() {
    if (dice_ptcs) dice_ptcs = nullptr;
    profile::profile_manager::destroy_instance();
    nickname::nickname_manager::destroy_instance();
    manual::manual_dice_control::destroy_instance();
    database::database_manager::destroy_instance();
    poker::poker_manager::destroy_instance();
}

void dicebot::set_logger(std::function<void(std::string, std::string)> varlog) { logger::_log = varlog; }

// bool dicebot::try_fill_nickname(event_info& event) {
//     return nickname::nickname_manager::instance->get_nickname(event, event.nickname);
// }

bool dicebot::message_pipeline(std::string const& source, event_info& event, nick_fill_call fill_nick, std::string& output) {
    auto lines = utils::split_line_corn(source);

    std::ostringstream ot;

    bool ignore_stand_alone = false;
    bool is_output_available = false;

    ::std::list<::std::string> responses;

    ::std::string extern_nick;

    for (auto& item : lines) {
        if (!utils::jump_to_front_of_point(item)) continue;
        if (!utils::trim(item)) continue;

        std::smatch match_cmd;
        std::regex_search(item.begin(), item.end(), match_cmd, dice_ptcs->get_regex_command());
        if (!match_cmd[1].matched) continue;

        if(event.nickname.empty()){
            if(!nickname::nickname_manager::instance->get_nickname(event, event.nickname)){
                if(extern_nick.empty()){
                    extern_nick = event.nickname = fill_nick();
                    if(extern_nick.empty()) extern_nick = event.nickname = ::std::to_string(event.user_id);
                }
                else event.nickname = extern_nick;
            }
        }

        auto target_entry = dice_ptcs->get_entry(match_cmd[1]);

        utils::string_view suffix = match_cmd.suffix();
        if (target_entry->is_stand_alone) {
            if (ignore_stand_alone) continue;

            if(target_entry->resolve_request(suffix, event, output)) {
                return is_output_available = true;
            }
        }else {
            std::string response;
            if (target_entry->resolve_request(suffix, event, response)) {
                if (is_output_available) ot << std::endl;
                ot << response;
                is_output_available = true;
                ignore_stand_alone = true;
                continue;
            }
        }
    }

    if (is_output_available) {
        output.assign(ot.str());
    }
    return is_output_available;
}
#include "./entry_poker.h"

#include <functional>
#include <regex>

#include "../data/poker_manager.h"
#include "../data/profile_manager.h"
#include "./output_constructor.h"

using namespace dicebot;
using namespace entry;

using func_type =
    std::function<bool(const std::string &, const event_info &, std::string &)>;

using func_map_t = std::unordered_map<std::string, func_type>;

static bool init_poker (const std::string &suffix, const event_info &event,
                            std::string &response) noexcept{
    if (suffix.empty())
        return false;

    auto& deck =
        poker::poker_manager::get_instance()->get_deck(event.group_id);

    deck.init(suffix);

    output_constructor oc(event.nickname);
    oc << u8"已初始化牌库，总计" << deck.size() << u8"张牌";
    response = oc;
    return true;
}

static bool draw_poker(const std::string &suffix, const event_info &event,
                       std::string &response) noexcept {
    auto& deck =
        poker::poker_manager::get_instance()->get_deck(event.group_id);

    auto show_drawer = [&deck](output_constructor &oc, bool detailed) {
        if (detailed) {
            oc << u8" | 牌堆剩余" << deck.size() << u8"张，已经抽出了: ";
            bool is_first = true;
            for (const auto &item : deck.drawer) {
                if (is_first) {
                    is_first = false;
                    oc << deck.render_name(item);
                } else {
                    oc << std::string(", ") + deck.render_name(item);
                }
            }
        } else {
            oc << u8" | 牌堆剩余" << deck.size() << u8"张";
        }
    };

    unsigned long draw_count = 1;
    size_t tail_start;
    try {
        if(suffix.size() > 0){
            draw_count = std::stoul(suffix, &tail_start);
            if (draw_count == 0)
                return false;
            if (draw_count > deck.size())
                draw_count = deck.size();
        }
    } catch (std::invalid_argument &) {
    } catch (std::out_of_range &) {
        draw_count = deck.size();
    }

    auto pfm = profile::profile_manager::get_instance();
    profile::var_pair var =
        pfm->get_profile(event.user_id)->sys_vars[profile::sys_var_type::rs_on];
    bool detailed_roll = static_cast<bool>(var.second);

    if (deck.draw(draw_count)) {
        output_constructor oc(event.nickname);
        oc << u8"抽出了 ";
        bool is_first = true;
        for (size_t i = deck.drawer.size() - draw_count;
             i < deck.drawer.size();
             i++) {
            if (is_first)
                is_first = false;
            else
                oc << ", ";
            oc << deck.render_name(deck.drawer.at(i));
        }
        show_drawer(oc, detailed_roll);
        response = oc;
        return true;
    } else {
        output_constructor oc(event.nickname);
        oc << u8"无牌可抽";
        show_drawer(oc, detailed_roll);
        response = oc;
        return true;
    }
    return false;
}

static auto shuffle_poker = [](const std::string &suffix,
                               const event_info &event,
                               std::string &response) noexcept -> bool {
    auto& deck =
        poker::poker_manager::get_instance()->get_deck(event.group_id);
    deck.shuffle();

    output_constructor oc(event.nickname);
    oc << u8"已将牌堆重新切洗";
    response = oc;
    return true;
};

static const func_map_t func_map = {{"init", init_poker},
                                    {"draw", draw_poker},
                                    {"d", draw_poker},
                                    {"shuffle", shuffle_poker}

};

static const std::regex filter_command("^(init|draw|d|shuffle) *",
                                       std::regex_constants::icase);
entry_poker::entry_poker() noexcept {
    this->is_stand_alone = false;
    this->identifier_regex = "p(?:oker)?";
    this->identifier_list = {"poker", "p"};
    this->help_message =
        u8"扑克牌指令（.poker或者.p）\n"
        u8"指令.poker init standard 初始化一副共54张的扑克牌，并洗牌\n"
        u8"指令.poker draw 抽一张牌\n"
        u8"指令.poker draw 4 抽四张牌\n"
        u8"指令.poker shuffle 重新洗牌\n"
        u8"指令.poker init 8j,8diamonda "
        u8"初始化一副由8张joker和8张方片A组成的牌堆，并洗牌\n"
        u8"指令.poker init 战,法,牧 "
        u8"初始化一副由战,法,牧3张牌组成的牌堆，并洗牌\n"
        u8"牌组最大数量为200，使用rson/rsoff来开关抽牌记录显示";
}

bool entry_poker::resolve_request(std::string const &message, event_info &event,
                                  std::string &response) noexcept {
    std::smatch detail_command_match;
    std::regex_search(message, detail_command_match, filter_command);
    if (detail_command_match.empty())
        return false;

    auto iter = func_map.find(detail_command_match[1]);
    if (iter == func_map.end())
        return false;
    iter->second(detail_command_match.suffix().str(), event, response);
    return true;
}
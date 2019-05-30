#include "./entry_poker.h"

#include <functional>
#include <regex>

#include "../data/poker_manager.h"
#include "./output_constructor.h"
using namespace dicebot;
using namespace entry;

using func_type = std::function<bool(const std::string &, const event_info &, std::string &)>;

using func_map_t = std::unordered_map<std::string, func_type>;

static auto init_poker = [](const std::string &suffix, const event_info &event, std::string &response) noexcept -> bool {
    if (suffix.empty()) return false;

    auto p_deck = poker::poker_manager::get_instance()->get_deck(event.group_id);

    p_deck->init(suffix);

    output_constructor oc(event.nickname);
    oc << u8"已初始化牌库，总计" << p_deck->size() << u8"张牌";
    response = oc;
    return true;
};

static auto draw_poker = [](const std::string &suffix, const event_info &event, std::string &response) noexcept -> bool {
    poker::card_item card;
    auto p_deck = poker::poker_manager::get_instance()->get_deck(event.group_id);

    auto show_drawer = [p_deck](output_constructor &oc) {
        oc << u8" | 牌堆剩余" << p_deck->size() << "张，已经抽出了: ";
        bool is_first = true;
        for (const auto &item : p_deck->drawer) {
            if (is_first) {
                is_first = false;
                oc << p_deck->render_name(item);
            } else {
                oc << std::string(", ") + p_deck->render_name(item);
            }
        }
    };

    if (p_deck->draw(card)) {
        output_constructor oc(event.nickname);
        oc << u8"抽出了" << p_deck->render_name(card);
        show_drawer(oc);
        response = oc;
        return true;
    } else {
        output_constructor oc(event.nickname);
        oc << u8"无牌可抽";
        show_drawer(oc);
        response = oc;
        return true;
    }
    return false;
};

static auto shuffle_poker = [](const std::string &suffix, const event_info &event, std::string &response) noexcept -> bool {
    auto p_deck = poker::poker_manager::get_instance()->get_deck(event.group_id);
    p_deck->shuffle();

    output_constructor oc(event.nickname);
    oc << u8"已将牌堆重新切洗";
    response = oc;
    return true;
};

static const func_map_t func_map = {{"init", init_poker}, {"draw", draw_poker}, {"d", draw_poker}, {"shuffle", shuffle_poker}

};

static const std::regex filter_command("^(init|draw|d|shuffle) *", std::regex_constants::icase);
entry_poker::entry_poker() noexcept {
    this->is_stand_alone = false;
    this->identifier_regex = "p(?:oker)?";
    this->identifier_list = {"poker", "p"};
    this->help_message =
        u8"扑克牌指令（.poker或者.p）\n"
        u8"指令.poker init standard 初始化一副共54张的扑克牌，并洗牌\n"
        u8"指令.poker draw 抽一张牌\n"
        u8"指令.poker shuffle 重新洗牌\n"
        u8"指令.poker init 8j,8diamonda "
        u8"初始化一副由8张joker和8张方片A组成的牌堆，并洗牌\n"
        u8"指令.poker init 战,法,牧 "
        u8"初始化一副由战,法,牧3张牌组成的牌堆，并洗牌";
}

bool entry_poker::resolve_request(std::string const &message, event_info &event, std::string &response) noexcept {
    std::smatch detail_command_match;
    std::regex_search(message, detail_command_match, filter_command);
    if (detail_command_match.empty()) return false;

    auto iter = func_map.find(detail_command_match[1]);
    if (iter == func_map.end()) return false;
    iter->second(detail_command_match.suffix().str(), event, response);
    return true;
}
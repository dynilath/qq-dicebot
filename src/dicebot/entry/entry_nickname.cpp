#include "./entry_nickname.h"

#include <regex>

#include "../data/nick_manager.h"
#include "./output_constructor.h"

using namespace dicebot;
using namespace dicebot::entry;

entry_nickname::entry_nickname() noexcept {
    this->is_stand_alone = false;
    this->identifier_regex = "n(?:ame)?";
    this->identifier_list = {"name", "n"};

    this->help_message =
        u8"昵称(.name或.n)\n"
        u8"指令.name [名字]：更改昵称为[名字]\n"
        u8"指令.n [名字]：上述命令的简写形式\n"
        u8"指令.name silence [名字]：更改昵称为[名字]，但机器人不会回复\n"
        u8"指令.ns [名字]：上述命令的简写形式";
}

static std::regex filter_command("^(s)(?:ilence)? *",
                                 std::regex_constants::icase);

bool entry_nickname::resolve_request(std::string const& message,
                                     event_info& event,
                                     std::string& response) noexcept {
    bool is_silence = false;
    std::smatch match_list_silence;
    std::regex_search(message, match_list_silence, filter_command);

    if (match_list_silence.empty()) {
        std::string message_cp;
        size_t pos_t = message.find_first_not_of(" \t");
        if (pos_t != std::string ::npos)
            message_cp.assign(message.substr(pos_t));
        else
            message_cp.assign(message);

        std::string str_origin_name = event.nickname;

        if (message_cp.length() > 0) {
            event.nickname = message_cp;
            (nickname::nickname_manager::instance)->set_nickname(event);

            output_constructor oc(str_origin_name);
            oc << u8"的新名字是 " << event.nickname;
            response = oc;
            return true;
        } else {
            output_constructor oc(str_origin_name);
            oc << u8"的名字是 " << event.nickname;
            response = oc;
            return true;
        }
    } else {
        std::string s_message = match_list_silence.suffix().str();
        size_t pos_t = s_message.find_first_not_of(" \t");
        if (pos_t != std::string::npos) s_message = s_message.substr(pos_t);

        if (s_message.length() > 0) {
            event.nickname = s_message;
            (nickname::nickname_manager::instance)->set_nickname(event);
        }
    }
    return false;
}
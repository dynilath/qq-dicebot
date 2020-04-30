#include "./cqtool.h"

#include <regex>
#include <unordered_map>

namespace cqlog = cq::logging;
namespace cqmsg = cq::message;

bool get_group_nickname(event_info& ei, uint64_t const group_id,
                        uint64_t const user_id) {
    try {
        ei.nickname = cq::get_group_member_info(group_id, user_id).nickname;
        return true;
    } catch (...) {
#ifdef _DEBUG
        cqlog::debug(u8"DICE", u8"无法获取昵称：" + std::to_string(user_id));
#endif
        return false;
    }
}

bool get_nickname(event_info& ei, uint64_t const user_id) {
    try {
        ei.nickname = cq::get_stranger_info(user_id).nickname;
        return true;
    } catch (...) {
#ifdef _DEBUG
        cqlog::debug(u8"DICE", u8"无法获取昵称：" + std::to_string(user_id));
#endif
        return false;
    }
}
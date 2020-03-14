#include "./cqtool.h"

#include <unordered_map>
#include <regex>


namespace cqlog = cq::logging;
namespace cqmsg = cq::message;

std::unordered_map<std::string, std::function<void(std::string,cqmsg::Message&)>>
    seg_map = {
        {
            "image", 
            [](std::string v,cqmsg::Message& msg){ 
                auto st_pos = v.find("file=") + 5;
                auto end_pos = v.find_first_of("]", st_pos);
                msg.emplace_back(cqmsg::MessageSegment::image(v.substr(st_pos, end_pos - st_pos)));
            } 
        },
        {
            "face", 
            [](std::string v,cqmsg::Message& msg){ 
                auto st_pos = v.find("id=") + 3;
                auto end_pos = v.find_first_of("]", st_pos);
                msg.emplace_back(cqmsg::MessageSegment::face(std::stoi(v.substr(st_pos, end_pos - st_pos))));
            } 
        },
        {
            "emoji", 
            [](std::string v,cqmsg::Message& msg){ 
                auto st_pos = v.find("id=") + 3;
                auto end_pos = v.find_first_of("]", st_pos);
                msg.emplace_back(cqmsg::MessageSegment::emoji(std::stoi(v.substr(st_pos, end_pos - st_pos))));
            } 
        },
    };

cqmsg::Message seg_cq_code(std::string str){
    cqmsg::Message msg;
    std::regex const reg_cq_code("\\[CQ:([a-z]+),[^[]+\\]");
    std::smatch m;
    std::regex_search(str, m, reg_cq_code);
    while (!m.empty()) {
        if(m.prefix().length() > 0)
            msg.push_back(cqmsg::MessageSegment::text(m.prefix()));
        if (m[1].matched)
            seg_map[m[1]](m[0],msg);
        str = m.suffix();
        std::regex_search(str, m, reg_cq_code);
    }
    msg.push_back(cqmsg::MessageSegment::text(str));
    return msg;
}

bool get_group_nickname(event_info &ei, uint64_t const group_id, uint64_t const user_id) {
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

bool get_nickname(event_info &ei, uint64_t const user_id) {
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
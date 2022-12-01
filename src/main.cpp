#include "oicq_func.hpp"
#include "./dicebot/dicebot.h"

template<typename Sender>
void resolve_cap(event_info &ei, std::string const &raw_source, dicebot::nick_fill_call filler , Sender sender) noexcept {
    try {
        std::string temp;
        if (!dicebot::message_pipeline(raw_source, ei, filler, temp)) return;
        sender(temp);
    } catch (const std::exception &err) {
        log_warn("dicebot encounter error :" + std::string(err.what()) + "\n invoking message：" + raw_source);
    }
}

void on_enable(const char * app_dir){
    dicebot::initialize(app_dir);
    log_info(u8"dicebot started.");
}

void on_disable(){
    dicebot::salvage();
}
void on_private_message(PrivateMessageEventData data){
    ::event_info ei(data.user_id);
    resolve_cap(ei, data.raw_msg, 
    [&](){return data.user_nickname;},
    [&](const std::string& ret) {
        send_private_message(data.user_id,ret);
    });
}

void on_group_message(GroupMessageEventData data){
    ::event_info ei(data.user_id,data.group_id,event_type::group);
    resolve_cap(ei, data.raw_msg, 
    [&](){return data.user_nickname;},
    [&](const std::string& ret) {
        send_group_message(data.group_id,ret);
    });
}

void on_discuss_message(DiscussMessageEventData data){
    ::event_info ei(data.user_id,data.discuss_id,event_type::discuss);
    resolve_cap(ei, data.raw_msg, 
    [&](){return data.user_nickname;},
    [&](const std::string& ret) {
        send_group_message(data.discuss_id,ret);
    });
}

#include "./config.h"
#include "../extern/corncppsdk/sdk/sdk.h"
#include "dicebot/dicebot.h"
#include <cstdint>

auto my_logger = [](const std::string&v1,const std::string&v2){
            api->OutputLog(v1+" : "+v2);};

template<typename Sender>
void resolve_cap(event_info &ei, std::string const &raw_source, Sender sender) noexcept {
    try {
        std::string temp;
        if (!dicebot::message_pipeline(raw_source, ei, temp)) return;
        sender(temp);
    } catch (const std::exception &err) {
        my_logger("DICE","exception：" + std::string(err.what()) + ",message：" + raw_source);
    }
}

EventProcessEnum OnPrivateMessage(PrivateMessageData data)
{
    //if (!dicebot::utils::basic_event_filter(content)) return EventProcess::Ignore;

    //my_logger("dicebot", "private msg from(" + std::to_string(sender_qq) + "): " + content);

    ::event_info ei(data.SenderQQ);
    if (!dicebot::try_fill_nickname(ei)) {
        ei.nickname = api->GetNameForce(data.ThisQQ, data.SenderQQ);
    }

    resolve_cap(ei, data.MessageContent, [&](const std::string& ret) {
        if(data.MessageType == MessageTypeEnum::FriendUsualMessage)
            api->SendFriendMessage(data.ThisQQ, data.SenderQQ, ret);
        else if(data.MessageType == MessageTypeEnum::Temporary)
            api->SendGroupTemporaryMessage(data.ThisQQ, data.MessageGroupQQ, data.SenderQQ, ret);
    });

    return EventProcessEnum::Ignore;
}

EventProcessEnum OnGroupMessage(GroupMessageData data){

    //if (!dicebot::utils::basic_event_filter(content)) return EventProcess::Ignore;

    ::event_info ei(data.SenderQQ, data.MessageGroupQQ, ::event_type::group);
    if (!dicebot::try_fill_nickname(ei)) {
        ei.nickname = api->GetGroupNickname(data.ThisQQ, data.MessageGroupQQ, data.SenderQQ);
    }

    resolve_cap(ei, data.MessageContent, [&](const std::string& ret) {
        api->SendGroupMessage(data.ThisQQ, data.MessageGroupQQ, ret);
    });

    return EventProcessEnum::Ignore;    
}

// 插件卸载事件（未知参数）
EventProcessEnum OnUninstall(){
    delete api;  // 清除app对象避免内存泄漏
    return EventProcessEnum::Ignore;
}

// 插件设置事件（未知参数），这里可以弹出对话框
EventProcessEnum OnSettings(){
    return EventProcessEnum::Ignore;
}


// 插件被启用事件（未知参数）
EventProcessEnum OnEnabled(){
    std::string dir = api->GetPluginDataDirectory();
    dicebot::initialize(dir.c_str());
    dicebot::set_logger(my_logger);
    my_logger(u8"dicebot", u8"插件已启动");

    return EventProcessEnum::Ignore;
}

// 插件被禁用事件（未知参数）
EventProcessEnum OnDisabled(){
    dicebot::salvage(); 
    return EventProcessEnum::Ignore;
}

// 事件消息
EventProcessEnum OnEvent(EventData data){
    return EventProcessEnum::Ignore;
}

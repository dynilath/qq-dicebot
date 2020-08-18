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

EventProcess OnPrivateMessage(volatile PrivateMessageData* data)
{
    auto this_qq = data->ThisQQ;
    auto sender_qq = data->SenderQQ;
    auto content = GBKtoUTF8(data->MessageContent);

    //if (!dicebot::utils::basic_event_filter(content)) return EventProcess::Ignore;

    //my_logger("dicebot", "private msg from(" + std::to_string(sender_qq) + "): " + content);

    ::event_info ei(sender_qq);
    if (!dicebot::try_fill_nickname(ei)) {
        // ei.nickname = std::to_string(sender_qq);
        // ei.nickname = api->GetNameForce(sender_qq);
    }

    resolve_cap(ei, content, [=](const std::string& ret) {
        //my_logger("dicebot", "private msg to(" + std::to_string(sender_qq) + "): " + ret);
        api->SendFriendMessage(this_qq, sender_qq, ret);
    });

    return EventProcess::Ignore;
}

EventProcess OnGroupMessage(volatile GroupMessageData* data){
    auto this_qq = data->ThisQQ;
    auto group_qq = data->MessageGroupQQ;
    auto sender_qq = data->SenderQQ;
    auto content = GBKtoUTF8(data->MessageContent);

    //if (!dicebot::utils::basic_event_filter(content)) return EventProcess::Ignore;

    ::event_info ei(data->SenderQQ, data->MessageGroupQQ, ::event_type::group);
    if (!dicebot::try_fill_nickname(ei)) {
        //ei.nickname = api->GetGroupNickname(this_qq, group_qq, sender_qq);
    }

    resolve_cap(ei, content, [=](const std::string& ret) {
        api->SendGroupMessage(this_qq, group_qq, ret);
    });

    return EventProcess::Ignore;    
}

// 插件卸载事件（未知参数）
EventProcess OnUninstall(void*){
    delete api;  // 清除app对象避免内存泄漏
    return EventProcess::Ignore;
}

// 插件设置事件（未知参数），这里可以弹出对话框
EventProcess OnSettings(void*){
    return EventProcess::Ignore;
}


// 插件被启用事件（未知参数）
EventProcess OnEnabled(void*){
    std::string dir = api->GetPluginDataDirectory();
    dicebot::initialize(dir.c_str());
    dicebot::set_logger(my_logger);
    my_logger(u8"dicebot", u8"插件已启动");

    return EventProcess::Ignore;
}

// 插件被禁用事件（未知参数）
EventProcess OnDisabled(void*){
    dicebot::salvage(); 
    return EventProcess::Ignore;
}

// 事件消息
EventProcess OnEvent(volatile EventData* data){
    return EventProcess::Ignore;
}

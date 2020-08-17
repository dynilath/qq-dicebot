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

    if (!dicebot::utils::basic_event_filter(content)) return EventProcess::Ignore;

    ::event_info ei(sender_qq);
    if (!dicebot::try_fill_nickname(ei)) {
        //ei.nickname = api->GetNameForce(sender_qq);
    }

    resolve_cap(ei, content, [=](const std::string& ret) {
        api->SendFriendMessage(this_qq, sender_qq, ret);
    });

    return EventProcess::Ignore;
}

EventProcess OnGroupMessage(volatile GroupMessageData* data){
    auto this_qq = data->ThisQQ;
    auto group_qq = data->MessageGroupQQ;
    auto sender_qq = data->SenderQQ;
    auto content = GBKtoUTF8(data->MessageContent);

    if (!dicebot::utils::basic_event_filter(content)) return EventProcess::Ignore;

    ::event_info ei(data->SenderQQ, data->MessageGroupQQ, ::event_type::group);
    if (!dicebot::try_fill_nickname(ei)) {
        auto nick = api->GetGroupNickname(this_qq, group_qq, sender_qq);
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
    if (data->SourceGroupQQ == 0)  // 非群事件
    {
        switch (data->EventType)
        {
        // 好友事件_被好友删除
        case EventType::Friend_Removed:
            break;
        // 好友事件_签名变更
        case EventType::Friend_SignatureChanged:
            break;
        // 好友事件_昵称改变
        case EventType::Friend_NameChanged:
            break;
        // 好友事件_某人撤回事件
        case EventType::Friend_UserUndid:
            break;
        // 好友事件_有新好友
        case EventType::Friend_NewFriend:
            break;
        // 好友事件_好友请求
        case EventType::Friend_FriendRequest:
            break;
        // 好友事件_对方同意了您的好友请求
        case EventType::Friend_FriendRequestAccepted:
            break;
        // 好友事件_对方拒绝了您的好友请求
        case EventType::Friend_FriendRequestRefused:
            break;
        // 好友事件_资料卡点赞
        case EventType::Friend_InformationLiked:
            break;
        // 好友事件_签名点赞
        case EventType::Friend_SignatureLiked:
            break;
        // 好友事件_签名回复
        case EventType::Friend_SignatureReplied:
            break;
        // 好友事件_个性标签点赞
        case EventType::Friend_TagLiked:
            break;
        // 好友事件_随心贴回复
        case EventType::Friend_StickerLiked:
            break;
        // 好友事件_随心贴增添
        case EventType::Friend_StickerAdded:
            break;
        // 空间事件_与我相关
        case EventType::QZone_Related:
            break;
        // 框架事件_登录成功
        case EventType::This_SignInSuccess:
            break;
        }   
    }
    else  // 群事件
    {
        switch (data->EventType)
        {
        // 群事件_我被邀请加入群
        case EventType::Group_Invited:
            break;
        // 群事件_某人加入了群
        case EventType::Group_MemberJoined:
            break;
        // 群事件_某人申请加群
        case EventType::Group_MemberVerifying:
            break;
        // 群事件_群被解散
        case EventType::Group_GroupDissolved:
            break;
        // 群事件_某人退出了群
        case EventType::Group_MemberQuit:
            break;
        // 群事件_某人被踢出群
        case EventType::Group_MemberKicked:
            break;
        // 群事件_某人被禁言
        case EventType::Group_MemberShutUp:
            break;
        // 群事件_某人撤回事件
        case EventType::Group_MemberUndid:
            break;
        // 群事件_某人被取消管理
        case EventType::Group_AdministratorTook:
            break;
        // 群事件_某人被赋予管理
        case EventType::Group_AdministratorGave:
            break;
        // 群事件_开启全员禁言
        case EventType::Group_EnableAllShutUp:
            break;
        // 群事件_关闭全员禁言
        case EventType::Group_DisableAllShutUp:
            break;
        // 群事件_开启匿名聊天
        case EventType::Group_EnableAnonymous:
            break;
        // 群事件_关闭匿名聊天
        case EventType::Group_DisableAnonymous:
            break;
        // 群事件_开启坦白说
        case EventType::Group_EnableChatFrankly:
            break;
        // 群事件_关闭坦白说
        case EventType::Group_DisableChatFrankly:
            break;
        // 群事件_允许群临时会话
        case EventType::Group_AllowGroupTemporary:
            break;
        // 群事件_禁止群临时会话
        case EventType::Group_ForbidGroupTemporary:
            break;
        // 群事件_允许发起新的群聊
        case EventType::Group_AllowCreateGroup:
            break;
        // 群事件_禁止发起新的群聊
        case EventType::Group_ForbidCreateGroup:
            break;
        // 群事件_允许上传群文件
        case EventType::Group_AllowUploadFile:
            break;
        // 群事件_禁止上传群文件
        case EventType::Group_ForbidUploadFile:
            break;
        // 群事件_允许上传相册
        case EventType::Group_AllowUploadPicture:
            break;
        // 群事件_禁止上传相册
        case EventType::Group_ForbidUploadPicture:
            break;
        // 群事件_某人被邀请入群
        case EventType::Group_MemberInvited:
            break;
        // 群事件_展示成员群头衔
        case EventType::Group_ShowMemberTitle:
            break;
        // 群事件_隐藏成员群头衔
        case EventType::Group_HideMemberTitle:
            break;
        // 群事件_某人被解除禁言
        case EventType::Group_MemberNotShutUp:
            break;
        }
    }
    return EventProcess::Ignore;
}

/*
Cornerstone SDK v0.2.0
-- 面向现代 C++ 的 Corn SDK
兼容 Corn SDK v2.6.5
https://github.com/Sc-Softs/CornerstoneSDK

使用 MIT License 进行许可
SPDX-License-Identifier: MIT
Copyright © 2020 Contributors of Cornerstone SDK

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef CORNERSTONE_HEADER_TYPES_H_
#define CORNERSTONE_HEADER_TYPES_H_

#include "../sdk.h"
#include "utils-inl.h"

#include <string>
#include <unordered_map>

// 易语言类型
using ebyte = std::uint8_t;        // 易语言字节型
using eshort = std::int16_t;       // 易语言短整数型
using eint = std::int32_t;         // 易语言整数型
using elong = std::int64_t;        // 易语言长整数型
using efloat = float;              // 易语言小数型
using edouble = double;            // 易语言双精度小数型
using edate = std::uint64_t;       // 易语言日期时间型
using ebool = std::int32_t;        // 易语言逻辑型
using etext = const char *;        // 易语言文本型(GBK)
using ebin = const std::uint8_t *; // 易语言字节集
using esubptr = std::uintptr_t;    // 易语言子程序指针

class earray // 易语言数组
{
public:
    earray() noexcept;
    ~earray() noexcept;
    size_t GetDimension() const noexcept;

    void *data;

private:
    HANDLE heap;
};

template <class EType>
class earray1D // 易语言一维数组
    : public earray
{
public:
    size_t GetSize() const noexcept
    {
        return ((eint *)this->data)[1];
    }

    size_t Unpack(std::vector<EType> &array) const noexcept
    {
        if (this->GetDimension() != 1)
        {
            return -1;
        }
        auto size = this->GetSize();
        if constexpr (std::is_compound_v<EType>)
        {
            volatile EType **data = (volatile EType **)(((eint *)this->data) + 2);
            array.clear();
            for (auto i = 0; i < size; i++)
            {
                array.push_back(*(const_cast<EType *>(*data)));
                data++;
            }
        }
        else
        {
            EType *data = (EType *)(((eint *)this->data) + 2);
            array.clear();
            for (auto i = 0; i < size; i++)
            {
                array.push_back(*data);
                data++;
            }
        }
        return size;
    }
};

// 易语言常量
constexpr ebool etrue = 1;
constexpr ebool efalse = 0;

// 易语言类型转换
constexpr ebool bool2ebool(const bool &b)
{
    return b ? etrue : efalse;
}

constexpr bool ebool2bool(const ebool &e)
{
    return e == etrue;
}

constexpr ebool b2e(const bool &b)
{
    return bool2ebool(b);
}

constexpr bool e2b(const ebool &e)
{
    return ebool2bool(e);
}

std::string GBKtoUTF8(const char *gbk);
std::string UTF8toGBK(const std::string &utf8);
#define e2s_s(src_str) GBKtoUTF8(src_str)
#define s2e_s(src_str) UTF8toGBK(src_str)
#define e2s(src_str) e2s_s(src_str).c_str()
#define s2e(src_str) s2e_s(src_str).c_str()

// 枚举常量

// 事件处理
enum class EventProcess : eint
{
    // 阻止其他插件继续处理此事件
    Block = 1,
    // 允许其他插件继续处理此事件
    Ignore = 0
};

// 消息类型
enum class MessageType : eint
{
    // 临时会话
    Temporary = 141,
    // 临时会话_群
    Temporary_Group = 0,
    // 临时会话_公众号
    Temporary_PublicAccount = 129,
    // 临时会话_网页QQ咨询
    Temporary_WebQQConsultation = 201,
    // 好友通常消息
    FriendUsualMessage = 166,
    // 好友文件
    FriendFile = 529,
    // 好友语音
    FriendAudio = 208,
    // 群红包
    GroupRedEnvelope = 78,
    // 群聊通常消息
    GroupUsualMessage = 134
};

// 事件类型
enum class EventType : eint
{
    // 好友事件_被好友删除
    Friend_Removed = 15,
    // 好友事件_签名变更
    Friend_SignatureChanged = 16,
    // 好友事件_昵称改变
    Friend_NameChanged = 17,
    // 好友事件_某人撤回事件
    Friend_UserUndid = 18,
    // 好友事件_有新好友
    Friend_NewFriend = 19,
    // 好友事件_好友请求
    Friend_FriendRequest = 20,
    // 好友事件_对方同意了您的好友请求
    Friend_FriendRequestAccepted = 21,
    // 好友事件_对方拒绝了您的好友请求
    Friend_FriendRequestRefused = 22,
    // 好友事件_资料卡点赞
    Friend_InformationLiked = 23,
    // 好友事件_签名点赞
    Friend_SignatureLiked = 24,
    // 好友事件_签名回复
    Friend_SignatureReplied = 25,
    // 好友事件_个性标签点赞
    Friend_TagLiked = 26,
    // 好友事件_随心贴回复
    Friend_StickerLiked = 27,
    // 好友事件_随心贴增添
    Friend_StickerAdded = 28,

    // 空间事件_与我相关
    QZone_Related = 30,

    // 框架事件_登录成功
    This_SignInSuccess = 32,

    // 群事件_我被邀请加入群
    Group_Invited = 1,
    // 群事件_某人加入了群
    Group_MemberJoined = 2,
    // 群事件_某人申请加群
    Group_MemberVerifying = 3,
    // 群事件_群被解散
    Group_GroupDissolved = 4,
    // 群事件_某人退出了群
    Group_MemberQuit = 5,
    // 群事件_某人被踢出群
    Group_MemberKicked = 6,
    // 群事件_某人被禁言
    Group_MemberShutUp = 7,
    // 群事件_某人撤回事件
    Group_MemberUndid = 8,
    // 群事件_某人被取消管理
    Group_AdministratorTook = 9,
    // 群事件_某人被赋予管理
    Group_AdministratorGave = 10,
    // 群事件_开启全员禁言
    Group_EnableAllShutUp = 11,
    // 群事件_关闭全员禁言
    Group_DisableAllShutUp = 12,
    // 群事件_开启匿名聊天
    Group_EnableAnonymous = 13,
    // 群事件_关闭匿名聊天
    Group_DisableAnonymous = 14,
    // 群事件_开启坦白说
    Group_EnableChatFrankly = 15,
    // 群事件_关闭坦白说
    Group_DisableChatFrankly = 16,
    // 群事件_允许群临时会话
    Group_AllowGroupTemporary = 17,
    // 群事件_禁止群临时会话
    Group_ForbidGroupTemporary = 18,
    // 群事件_允许发起新的群聊
    Group_AllowCreateGroup = 19,
    // 群事件_禁止发起新的群聊
    Group_ForbidCreateGroup = 20,
    // 群事件_允许上传群文件
    Group_AllowUploadFile = 21,
    // 群事件_禁止上传群文件
    Group_ForbidUploadFile = 22,
    // 群事件_允许上传相册
    Group_AllowUploadPicture = 23,
    // 群事件_禁止上传相册
    Group_ForbidUploadPicture = 24,
    // 群事件_某人被邀请入群
    Group_MemberInvited = 25,
    // 群事件_展示成员群头衔
    Group_ShowMemberTitle = 27,
    // 群事件_隐藏成员群头衔
    Group_HideMemberTitle = 28,
    // 群事件_某人被解除禁言
    Group_MemberNotShutUp = 29
};

// 某些API中可能会用
// 权限（有感叹号的是危险权限！）
enum class Permission : eint
{
    // 输出日志
    OutputLog = 0,
    // 发送好友消息
    SendFriendMessage = 1,
    // 发送群消息
    SendGroupMessage = 2,
    // 发送群临时消息
    SendGroupTemporaryMessage = 3,
    // 添加好友
    AddFriend = 4,
    // 添加群
    AddGroup = 5,
    // 删除好友！
    RemoveFriend = 6,
    // 置屏蔽好友！
    SetBlockFriend = 7,
    // 置特别关心好友
    SetSpecialFriend = 8,
    // 发送好友json消息
    SendFriendJSONMessage = 11,
    // 发送群json消息
    SendGroupJSONMessage = 12,
    // 上传好友图片
    UploadFriendPicture = 13,
    // 上传群图片
    UploadGroupPicture = 14,
    // 上传好友语音
    UploadFriendAudio = 15,
    // 上传群语音
    UploadGroupAudio = 16,
    // 上传头像！
    UploadAvatar = 17,
    // 设置群名片
    SetGroupMemberNickname = 18,
    // 取昵称_从缓存
    GetNameFromCache = 19,
    // 强制取昵称
    GetNameForce = 20,
    // 获取skey！
    GetSKey = 21,
    // 获取pskey！
    GetPSKey = 22,
    // 获取clientkey！
    GetClientKey = 23,
    // 取框架QQ
    GetThisQQ = 24,
    // 取好友列表
    GetFriendList = 25,
    // 取群列表
    GetGroupList = 26,
    // 取群成员列表
    GetGroupMemberList = 27,
    // 设置管理员
    SetAdministrator = 28,
    // 取管理层列表
    GetAdministratorList = 29,
    // 取群名片
    GetGroupMemberNickname = 30,
    // 取个性签名
    GetSignature = 31,
    // 修改昵称！
    SetName = 32,
    // 修改个性签名！
    SetSignature = 33,
    // 删除群成员
    KickGroupMember = 34,
    // 禁言群成员
    BanGroupMember = 35,
    // 退群！
    QuitGroup = 36,
    // 解散群！
    DissolveGroup = 37,
    // 上传群头像
    UploadGroupAvatar = 38,
    // 全员禁言
    BanAll = 39,
    // 群权限_发起新的群聊
    Group_Create = 40,
    // 群权限_发起临时会话
    Group_CreateTemporary = 41,
    // 群权限_上传文件
    Group_UploadFile = 42,
    // 群权限_上传相册
    Group_UploadPicture = 43,
    // 群权限_邀请好友加群
    Group_InviteFriend = 44,
    // 群权限_匿名聊天
    Group_Anonymous = 45,
    // 群权限_坦白说
    Group_ChatFrankly = 46,
    // 群权限_新成员查看历史消息
    Group_NewMemberReadChatHistory = 47,
    // 群权限_邀请方式设置
    Group_SetInviteMethod = 48,
    // 撤回消息_群聊
    Undo_Group = 49,
    // 撤回消息_私聊本身
    Undo_Private = 50,
    // 设置位置共享
    SetLocationShare = 51,
    // 上报当前位置
    ReportCurrentLocation = 52,
    // 是否被禁言
    IsShutUp = 53,
    // 处理好友验证事件
    ProcessFriendVerification = 54,
    // 处理群验证事件
    ProcessGroupVerification = 55,
    // 查看转发聊天记录内容
    ReadForwardedChatHistory = 56,
    // 上传群文件
    UploadGroupFile = 57,
    // 创建群文件夹
    CreateGroupFolder = 58,
    // 设置在线状态
    SetStatus = 59,
    // QQ点赞！
    QQLike = 60,
    // 取图片下载地址
    GetImageDownloadLink = 61,
    // 查询好友信息
    QueryFriendInformation = 63,
    // 查询群信息
    QueryGroupInformation = 64,
    // 框架重启！
    Reboot = 65,
    // 群文件转发至群
    GroupFileForwardToGroup = 66,
    // 群文件转发至好友
    GroupFileForwardToFriend = 67,
    // 好友文件转发至好友
    FriendFileForwardToFriend = 68,
    // 置群消息接收
    SetGroupMessageReceive = 69,
    // 取群名称_从缓存
    GetGroupNameFromCache = 70,
    // 发送免费礼物
    SendFreeGift = 71,
    // 取好友在线状态
    GetFriendStatus = 72,
    // 取QQ钱包个人信息！
    GetQQWalletPersonalInformation = 73,
    // 获取订单详情
    GetOrderDetail = 74,
    // 提交支付验证码
    SubmitPaymentCaptcha = 75,
    // 分享音乐
    ShareMusic = 77,
    // 更改群聊消息内容！
    ModifyGroupMessageContent = 78,
    // 更改私聊消息内容！
    ModifyPrivateMessageContent = 79,
    // 群聊口令红包
    GroupPasswordRedEnvelope = 80,
    // 群聊拼手气红包
    GroupRandomRedEnvelope = 81,
    // 群聊普通红包
    GroupNormalRedEnvelope = 82,
    // 群聊画图红包
    GroupDrawRedEnvelope = 83,
    // 群聊语音红包
    GroupAudioRedEnvelope = 84,
    // 群聊接龙红包
    GroupFollowRedEnvelope = 85,
    // 群聊专属红包
    GroupExclusiveRedEnvelope = 86,
    // 好友口令红包
    FriendPasswordRedEnvelope = 87,
    // 好友普通红包
    FriendNormalRedEnvelope = 88,
    // 好友画图红包
    FriendDrawRedEnvelope = 89,
    // 好友语音红包
    FriendAudioRedEnvelope = 90,
    // 好友接龙红包
    FriendFollowRedEnvelope = 91,
};

const std::unordered_map<Permission, std::string> PermissionMap =
    {{Permission::OutputLog, "输出日志"},
     {Permission::SendFriendMessage, "发送好友消息"},
     {Permission::SendGroupMessage, "发送群消息"},
     {Permission::SendGroupTemporaryMessage, "发送群临时消息"},
     {Permission::AddFriend, "添加好友"},
     {Permission::AddGroup, "添加群"},
     {Permission::RemoveFriend, "删除好友"},
     {Permission::SetBlockFriend, "置屏蔽好友"},
     {Permission::SetSpecialFriend, "置特别关心好友"},
     {Permission::SendFriendJSONMessage, "发送好友json消息"},
     {Permission::SendGroupJSONMessage, "发送群json消息"},
     {Permission::UploadFriendPicture, "上传好友图片"},
     {Permission::UploadGroupPicture, "上传群图片"},
     {Permission::UploadFriendAudio, "上传好友语音"},
     {Permission::UploadGroupAudio, "上传群语音"},
     {Permission::UploadAvatar, "上传头像"},
     {Permission::SetGroupMemberNickname, "设置群名片"},
     {Permission::GetNameFromCache, "取昵称_从缓存"},
     {Permission::GetNameForce, "强制取昵称"},
     {Permission::GetSKey, "获取skey"},
     {Permission::GetPSKey, "获取pskey"},
     {Permission::GetClientKey, "获取clientkey"},
     {Permission::GetThisQQ, "取框架QQ"},
     {Permission::GetFriendList, "取好友列表"},
     {Permission::GetGroupList, "取群列表"},
     {Permission::GetGroupMemberList, "取群成员列表"},
     {Permission::SetAdministrator, "设置管理员"},
     {Permission::GetAdministratorList, "取管理层列表"},
     {Permission::GetGroupMemberNickname, "取群名片"},
     {Permission::GetSignature, "取个性签名"},
     {Permission::SetName, "修改昵称"},
     {Permission::SetSignature, "修改个性签名"},
     {Permission::KickGroupMember, "删除群成员"},
     {Permission::BanGroupMember, "禁言群成员"},
     {Permission::QuitGroup, "退群"},
     {Permission::DissolveGroup, "解散群"},
     {Permission::UploadGroupAvatar, "上传群头像"},
     {Permission::BanAll, "全员禁言"},
     {Permission::Group_Create, "群权限_发起新的群聊"},
     {Permission::Group_CreateTemporary, "群权限_发起临时会话"},
     {Permission::Group_UploadFile, "群权限_上传文件"},
     {Permission::Group_UploadPicture, "群权限_上传相册"},
     {Permission::Group_InviteFriend, "群权限_邀请好友加群"},
     {Permission::Group_Anonymous, "群权限_匿名聊天"},
     {Permission::Group_ChatFrankly, "群权限_坦白说"},
     {Permission::Group_NewMemberReadChatHistory, "群权限_新成员查看历史消息"},
     {Permission::Group_SetInviteMethod, "群权限_邀请方式设置"},
     {Permission::Undo_Group, "撤回消息_群聊"},
     {Permission::Undo_Private, "撤回消息_私聊本身"},
     {Permission::SetLocationShare, "设置位置共享"},
     {Permission::ReportCurrentLocation, "上报当前位置"},
     {Permission::IsShutUp, "是否被禁言"},
     {Permission::ProcessFriendVerification, "处理好友验证事件"},
     {Permission::ProcessGroupVerification, "处理群验证事件"},
     {Permission::ReadForwardedChatHistory, "查看转发聊天记录内容"},
     {Permission::UploadGroupFile, "上传群文件"},
     {Permission::CreateGroupFolder, "创建群文件夹"},
     {Permission::SetStatus, "设置在线状态"},
     {Permission::QQLike, "QQ点赞"},
     {Permission::GetImageDownloadLink, "取图片下载地址"},
     {Permission::QueryFriendInformation, "查询好友信息"},
     {Permission::QueryGroupInformation, "查询群信息"},
     {Permission::Reboot, "框架重启"},
     {Permission::GroupFileForwardToGroup, "群文件转发至群"},
     {Permission::GroupFileForwardToFriend, "群文件转发至好友"},
     {Permission::FriendFileForwardToFriend, "好友文件转发至好友"},
     {Permission::SetGroupMessageReceive, "置群消息接收"},
     {Permission::GetGroupNameFromCache, "取群名称_从缓存"},
     {Permission::SendFreeGift, "发送免费礼物"},
     {Permission::GetFriendStatus, "取好友在线状态"},
     {Permission::GetQQWalletPersonalInformation, "取QQ钱包个人信息"},
     {Permission::GetOrderDetail, "获取订单详情"},
     {Permission::SubmitPaymentCaptcha, "提交支付验证码"},
     {Permission::ShareMusic, "分享音乐"},
     {Permission::ModifyGroupMessageContent, "更改群聊消息内容"},
     {Permission::ModifyPrivateMessageContent, "更改私聊消息内容"},
     {Permission::GroupPasswordRedEnvelope, "群聊口令红包"},
     {Permission::GroupRandomRedEnvelope, "群聊拼手气红包"},
     {Permission::GroupNormalRedEnvelope, "群聊普通红包"},
     {Permission::GroupDrawRedEnvelope, "群聊画图红包"},
     {Permission::GroupAudioRedEnvelope, "群聊语音红包"},
     {Permission::GroupFollowRedEnvelope, "群聊接龙红包"},
     {Permission::GroupExclusiveRedEnvelope, "群聊专属红包"},
     {Permission::FriendPasswordRedEnvelope, "好友口令红包"},
     {Permission::FriendNormalRedEnvelope, "好友普通红包"},
     {Permission::FriendDrawRedEnvelope, "好友画图红包"},
     {Permission::FriendAudioRedEnvelope, "好友语音红包"},
     {Permission::FriendFollowRedEnvelope, "好友接龙红包"}};

#pragma pack(4)
// 数据结构

// 私聊消息数据
struct PrivateMessageData
{
    // 发送人QQ
    elong SenderQQ;
    // 框架QQ
    elong ThisQQ;
    // 消息Req
    eint MessageReq;
    // 消息Seq
    elong MessageSeq;
    // 消息接收时间
    eint MessageReceiveTime;
    // 消息群号 当为群临时会话时可取
    elong MessageGroupQQ;
    // 消息发送时间
    eint MessageSendTime;
    // 消息Random
    elong MessageRandom;
    // 消息分片序列
    eint MessageClip;
    // 消息分片数量
    eint MessageClipCount;
    // 消息分片标识
    elong MessageClipID;
    // 消息内容
    etext MessageContent;
    // 发送人气泡ID
    eint BubbleID;
    // 消息类型
    MessageType MessageType;
    // 消息子类型
    eint MessageSubType;
    // 消息子临时类型 0: 群, 1: 讨论组, 129: 腾讯公众号, 201: QQ咨询
    eint MessageSubTemporaryType;
    // 红包类型 2: 已转入余额, 4: 点击收款, 10: 红包
    eint RedEnvelopeType;
    // 会话Token
    ebin SessionToken;
    // 来源事件QQ
    elong SourceEventQQ;
    // 来源事件QQ昵称
    etext SourceEventQQName;
    // 文件ID
    etext FileID;
    // 文件Md5
    ebin FileMD5;
    // 文件名
    etext FileName;
    // 文件大小
    elong FileSize;
};

// 服务信息
struct ServiceInformation
{
    // 服务代号 1: SVIP, 4: 视频会员, 6: 音乐包, 105: star, 102: 黄钻, 103: 绿钻, 101: 红钻, 104:yellowlove, 107: SVIP&视频会员, 109: SVIP&绿钻, 110: SVIP&音乐包
    eint ServiceCodename;
    // 服务等级
    eint ServiceLevel;
};

// 好友信息
struct FriendInformation
{
    // 邮箱
    etext Email;
    // 账号
    elong QQNumber;
    // 昵称
    etext Name;
    // 备注
    etext Note;
    // 在线状态 只能使用[取好友列表]获取
    etext Status;
    // 赞数量 只能使用[查询好友信息]获取
    eint Likes;
    // 签名 只能使用[查询好友信息]获取
    etext Signature;
    // 性别 255: 隐藏, 0: 男, 1: 女
    eint Gender;
    // Q等级 只能使用[查询好友信息]获取
    eint Level;
    // 年龄 只能使用[查询好友信息]获取
    eint Age;
    // 国家 只能使用[查询好友信息]获取
    etext Nation;
    // 省份 只能使用[查询好友信息]获取
    etext Province;
    // 城市 只能使用[查询好友信息]获取
    etext City;
    // FIXME: 易语言列表需要解包而不是一个简单的指针
    // 服务列表 只能使用[查询好友信息]获取
    ServiceInformation *ServiceList;
    // 连续在线天数 只能使用[查询好友信息]获取
    eint ContinuousOnlineTime;
    // QQ达人 只能使用[查询好友信息]获取
    etext QQTalent;
    // 今日已赞 只能使用[查询好友信息]获取
    eint LikesToday;
    // 今日可赞数 只能使用[查询好友信息]获取
    eint LikesAvailableToday;
};

// 群信息
struct GroupInformation
{
    // 群ID
    elong GroupID;
    // 群号
    elong GroupQQ;
    // cFlag
    elong CFlag;
    // dwGroupInfoSeq
    elong GroupInfoSeq;
    // dwGroupFlagExt
    elong GroupFlagExt;
    // dwGroupRankSeq
    elong GroupRankSeq;
    // dwCertificationType
    elong CertificationType;
    // 禁言时间戳
    elong ShutUpTimestamp;
    // 框架QQ禁言时间戳
    elong ThisShutUpTimestamp;
    // dwCmdUinUinFlag
    elong CmdUinUinFlag;
    // dwAdditionalFlag
    elong AdditionalFlag;
    // dwGroupTypeFlag
    elong GroupTypeFlag;
    // dwGroupSecType
    elong GroupSecType;
    // dwGroupSecTypeInfo
    elong GroupSecTypeInfo;
    // dwGroupClassExt
    elong GroupClassExt;
    // dwAppPrivilegeFlag
    elong AppPrivilegeFlag;
    // dwSubscriptionUin
    elong SubscriptionUin;
    // 群成员数量
    elong GroupMemberCount;
    // dwMemberNumSeq
    elong MemberNumSeq;
    // dwMemberCardSeq
    elong MemberCardSeq;
    // dwGroupFlagExt3
    elong GroupFlagExt3;
    // dwGroupOwnerUin
    elong GroupOwnerUin;
    // cIsConfGroup
    elong IsConfGroup;
    // cIsModifyConfGroupFace
    elong IsModifyConfGroupFace;
    // cIsModifyConfGroupName
    elong IsModifyConfGroupName;
    // dwCmduinJoinTime
    elong CmduinJoinTime;
    // 群名称
    etext GroupName;
    // strGroupMemo
    etext GroupMemo;
};

// 群成员信息
struct GroupMemberInformation
{
    // 账号
    etext QQNumber;
    // 年龄
    eint Age;
    // 性别
    eint Gender;
    // 昵称
    etext Name;
    // 邮箱
    etext Email;
    // 名片
    etext Nickname;
    // 备注
    etext Note;
    // 头衔
    etext Title;
    // 手机号
    etext Phone;
    // 头衔到期时间
    elong TitleTimeout;
    // 禁言时间戳
    elong ShutUpTimestamp;
    // 加群时间
    elong JoinTime;
    // 发言时间
    elong ChatTime;
    // 群等级
    elong Level;
};

// 群消息数据
struct GroupMessageData
{
    // 发送人QQ
    elong SenderQQ;
    // 框架QQ
    elong ThisQQ;
    // 消息Req
    eint MessageReq;
    // 消息接收时间
    eint MessageReceiveTime;
    // 消息群号
    elong MessageGroupQQ;
    // 消息来源群名（貌似失效了）
    etext SourceGroupName;
    // 发送人群名片 没有名片则为空白
    etext SenderNickname;
    // 消息发送时间
    eint MessageSendTime;
    // 消息Random
    elong MessageRandom;
    // 消息分片序列
    eint MessageClip;
    // 消息分片数量
    eint MessageClipCount;
    // 消息分片标识
    elong MessageClipID;
    // 消息类型
    MessageType MessageType;
    // 发送人群头衔
    etext SenderTitle;
    // 消息内容
    etext MessageContent;
    // 回复对象消息内容 如果是回复消息
    etext ReplyMessageContent;
    // 发送者气泡ID
    eint BubbleID;
    // 发送人位置经度
    eint SenderLongitude;
    // 发送人位置纬度
    eint SenderLatitude;
    // 文件Id
    etext FileID;
    // 文件Md5
    ebin FileMD5;
    // 文件名
    etext FileName;
    // 文件大小
    elong FileSize;
    // 消息AppID
    eint MessageAppID;
};

// 事件数据
struct EventData
{
    // 框架QQ
    elong ThisQQ;
    // 来源群号
    elong SourceGroupQQ;
    // 操作QQ
    elong OperateQQ;
    // 触发QQ
    elong TriggerQQ;
    // 消息Seq
    elong MessageSeq;
    // 消息时间戳
    eint MessageTimestamp;
    // 来源群名
    etext SourceGroupName;
    // 操作QQ昵称
    etext OperateQQName;
    // 触发QQ昵称
    etext TriggerQQName;
    // 事件内容
    etext MessageContent;
    // 事件类型
    EventType EventType;
    // 事件子类型
    eint EventSubType;
};

// 群卡片信息
struct GroupCardInformation
{
    // 群名称
    etext GroupName;
    // 群地点
    etext GroupLocation;
    // 群分类
    etext GroupClassification;
    // 群标签 以|分割
    etext GroupTags;
    // 群介绍
    etext GroupDescription;
};

// 银行卡信息
struct CardInformation
{
    // 序列
    eint Serial;
    // 尾号
    etext TailNumber;
    // 银行
    etext Bank;
    // 绑定手机
    etext BindPhone;
    // bind_serial
    etext BindSerial;
    // bank_type
    etext BankType;
};

// QQ钱包信息
struct QQWalletInformation
{
    // 余额
    etext Balance;
    // 身份证号
    etext ID;
    // 实名
    etext RealName;
    // 银行卡列表
    CardInformation *CardList;
};

// 订单详情
struct OrderDetail
{
    // 订单时间
    etext OrderTime;
    // 订单说明
    etext OrderDescription;
    // 订单类名
    etext OrderClassification;
    // 订单类型
    etext OrderType;
    // 订单手续费
    etext OrderCommission;
    // 操作人QQ
    etext OperatorQQ;
    // 操作人昵称
    etext OperatorName;
    // 接收人QQ
    etext ReceiverQQ;
    // 接收人昵称
    etext ReceiverName;
    // 操作金额
    etext OperateAmount;
};

// 验证码信息
struct CaptchaInformation
{
    // token_id
    etext TokenID;
    // skey
    etext SKey;
    // bank_type
    etext BankType;
    // mobile
    etext Mobile;
    // business_type
    etext BusinessType;
    // random
    eint Random;
    // transaction_id
    etext TransactionID;
    // purchaser_id
    etext PurchaserID;
    // token
    etext Token;
    // auth_params
    etext AuthParams;
};

#pragma pack()
#endif // CORNERSTONE_HEADER_TYPES_H_
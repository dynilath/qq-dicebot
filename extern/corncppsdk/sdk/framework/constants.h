/*
Cornerstone SDK v1.0.1
-- 面向现代 C++ 的 Corn SDK
兼容小栗子框架 v2.7.1-v2.7.2 和 Corn SDK v2.7.1
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

#pragma once

#include "../eport/etypes.h"

//region 枚举常量
//=============

// 事件处理
enum class EventProcessEnum : eint
{
    // 阻止其他插件继续处理此事件
    Block = 1,
    // 允许其他插件继续处理此事件
    Ignore = 0
};

// 消息类型
enum class MessageTypeEnum : eint
{
    // 临时会话
    Temporary = 141,
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

// 消息子类型
enum class MessageSubTypeEnum : eint
{
    // 临时会话_群
    Temporary_Group = 0,
    // 临时会话_公众号
    Temporary_PublicAccount = 129,
    // 临时会话_网页QQ咨询
    Temporary_WebQQConsultation = 201,
};

// 事件类型
enum class EventTypeEnum : eint
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

// 语音类型
enum class AudioTypeEnum : eint
{
    // 普通语音
    Normal = 0,
    // 变声语音
    Change = 1,
    // 文字语音
    Text = 2,
    // (红包)匹配语音
    Match = 3,
};

// 群验证信息操作类型
enum class GroupVerificationOperateEnum : eint
{
    Agree = 11,
    Deny = 12,
    Ignore = 14
};

// 好友验证信息操作类型
enum class FriendVerificationOperateEnum : eint
{
    Agree = 1,
    Deny = 2
};

// 主要在线状态
enum class StatusTypeEnum : eint
{
    // 在线
    Online = 11,
    // 离开
    Away = 31,
    // 隐身
    Invisible = 41,
    // 忙碌
    Busy = 50,
    // Q我吧
    TalkToMe = 60,
    // 请勿打扰
    DoNotDisturb = 70
};

// 详细在线状态
enum class StatusOnlineTypeEnum : eint
{
    // 普通在线
    Normal = 0,
    // 我的电量
    Battery = 1000,
    // 信号弱
    WeakSignal = 1011,
    // 睡觉中
    Sleeping = 1016,
    // 游戏中
    Gaming = 1017,
    // 学习中
    Studying = 1018,
    // 吃饭中
    Eating = 1019,
    // 煲剧中
    WatchingTVSeries = 1021,
    // 度假中
    OnVacation = 1022,
    // 在线学习
    OnlineStudying = 1024,
    // 在家旅游
    TravelAtHome = 1025,
    // TiMi中
    TiMiing = 1027,
    // 我在听歌
    ListeningToMusic = 1028,
    // 熬夜中
    StayingUpLate = 1032,
    // 打球中
    PlayingBall = 1050,
    // 恋爱中
    FallInLove = 1051,
    // 我没事(实际上有事)
    ImOK = 1052
};

// TODO: 优化使用体验
// 免费礼物
enum class FreeGiftEnum : eint
{
    // 牵你的手
    Gift_280 = 280,
    // 可爱猫咪
    Gift_281 = 281,
    // 神秘面具
    Gift_284 = 284,
    // 甜wink
    Gift_285 = 285,
    // 我超忙的
    Gift_286 = 286,
    // 快乐肥宅水
    Gift_289 = 289,
    // 幸运手链
    Gift_290 = 290,
    // 卡布奇诺
    Gift_299 = 299,
    // 猫咪手表
    Gift_302 = 302,
    // 绒绒手套
    Gift_307 = 307,
    // 彩虹糖果
    Gift_308 = 308,
    // 爱心口罩
    Gift_312 = 312,
    // 坚强
    Gift_313 = 313,
    // 告白话筒
    Gift_367 = 367
};

//const std::unordered_map<std::string, eint> FreeGiftMap =
//        {
// 367: 告白话筒, 299: 卡布奇诺, 302: 猫咪手表, 280: 牵你的手, 281: 可爱猫咪, 284: 神秘面具, 285: 甜wink, 286: 我超忙的, 289: 快乐肥宅水, 290: 幸运手链, 313: 坚强, 307: 绒绒手套, 312: 爱心口罩, 308: 彩虹糖果
//        };

// 某些API中可能会用
// 权限（有感叹号的是危险权限！）
enum class PermissionEnum : eint
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
    // 重命名群文件夹
    RenameGroupDirectory = 92,
    // 删除群文件夹
    RemoveGroupDirectory = 93,
    // 删除群文件
    RemoveGroupFile = 94,
    // 保存文件到微云
    SaveFileToWeiYun = 95,
    // 移动群文件
    MoveGroupFile = 96,
    // 取群文件列表
    GetGroupFileList = 97,
    // 设置专属头衔
    SetExclusiveTitle = 98
};
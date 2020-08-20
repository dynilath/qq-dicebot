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

#ifndef CORNERSTONE_HEADER_API_H_
#define CORNERSTONE_HEADER_API_H_

#include "../sdk.h"

class API
{
private:
    Json j;
    std::string key;
public:
    /**
     * @brief 载入插件关键字和系统API函数指针
     * @param api_data 系统API函数指针JSON
     * @param plugin_key 插件标识符
     */
    API(etext api_data, etext plugin_key);

    ~API();

    /**
     * @brief 输出日志
     * @param message 日志
     * @param text_color 文字颜色
     * @param background_color 背景颜色
     */
    std::string OutputLog(const std::string &message, std::int32_t text_color = 32768, std::int32_t background_color = 16777215);

    /**
     * @brief 发送好友消息
     * @param thisQQ 框架QQ
     * @param friendQQ 好友QQ
     * @param content 发送内容
     * @param random 撤回消息用（传出）
     * @param req 撤回消息用（传出）
     * @return 成功返回的time用于撤回消息
     */
    std::string SendFriendMessage(std::int64_t thisQQ, std::int64_t friendQQ, const std::string &content, std::int64_t &random, std::int32_t &req);

    /**
     * @brief 发送好友消息（若要撤回消息请使用另一重载）
     * @param thisQQ 框架QQ
     * @param friendQQ 好友QQ
     * @param content 发送内容
     * @return 成功返回的time用于撤回消息
     */
    std::string SendFriendMessage(std::int64_t thisQQ, std::int64_t friendQQ, const std::string &content);

    /**
     * @brief 发送群消息
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param content 发送内容
     * @param anonymous 匿名发送
     */
    std::string SendGroupMessage(std::int64_t thisQQ, std::int64_t groupQQ, const std::string &content, bool anonymous = false);

    /**
     * @brief 发送群临时消息
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param otherQQ 对方QQ
     * @param content 发送内容
     * @param random 撤回消息用
     * @param req 撤回消息用
     * @return 成功返回的time用于撤回消息
     */
    std::string SendGroupTemporaryMessage(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t otherQQ, const std::string &content, std::int64_t &random, std::int32_t &req);

    /**
     * @brief 发送群临时消息（若要撤回消息请使用另一重载）
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param otherQQ 对方QQ
     * @param content 发送内容
     * @return 成功返回的time用于撤回消息
     */
    std::string SendGroupTemporaryMessage(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t otherQQ, const std::string &content);

    /**
     * @brief 添加好友
     * @param thisQQ 框架QQ
     * @param otherQQ 对方QQ
     * @param verification 设置回答问题答案或是验证消息，多个问题答案用"|"分隔开
     */
    std::string AddFriend(std::int64_t thisQQ, std::int64_t otherQQ, const std::string &verification);

    /**
     * @brief 添加群（你在群内或需要付费入群也会直接发送验证消息）
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param verification 回答问题答案或是验证消息
     */
    std::string AddGroup(std::int64_t thisQQ, std::int64_t groupQQ, const std::string &verification);

    /**
     * @brief 删除好友
     * @param thisQQ 框架QQ
     * @param otherQQ 对方QQ
     */
    std::string RemoveFriend(std::int64_t thisQQ, std::int64_t otherQQ);

    /**
     * @brief 设置屏蔽好友
     * @param thisQQ 框架QQ
     * @param otherQQ 对方QQ
     * @param is_blocked 是否屏蔽
     */
    std::string SetBlockFriend(std::int64_t thisQQ, std::int64_t otherQQ, bool is_blocked);

    /**
     * @brief 设置特别关心好友
     * @param thisQQ 框架QQ
     * @param otherQQ 对方QQ
     * @param is_special 是否屏蔽
     */
    std::string SetSpecialFriend(std::int64_t thisQQ, std::int64_t otherQQ, bool is_special);

    /**
     * @brief 发送好友JSON消息
     * @param thisQQ 框架QQ
     * @param friendQQ 好友QQ
     * @param json_content json发送内容
     * @param random 撤回消息用
     * @param req 撤回消息用
     * @return 成功返回的time用于撤回消息
     */
    std::string SendFriendJSONMessage(std::int64_t thisQQ, std::int64_t friendQQ, const std::string &json_content, std::int64_t &random, std::int32_t &req);

    /**
     * @brief 发送好友JSON消息（若要撤回消息请使用另一重载）
     * @param thisQQ 框架QQ
     * @param friendQQ 好友QQ
     * @param json_content json发送内容
     * @return 成功返回的time用于撤回消息
     */
    std::string SendFriendJSONMessage(std::int64_t thisQQ, std::int64_t friendQQ, const std::string &json_content);

    /**
     * @brief 发送群JSON消息
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param json_content json发送内容
     * @param anonymous 匿名发送
     */
    std::string SendGroupJSONMessage(std::int64_t thisQQ, std::int64_t groupQQ, const std::string &json_content, bool anonymous = false);

    /**
     * @brief 上传好友图片
     * @param thisQQ 框架QQ
     * @param friendQQ 好友QQ
     * @param picture 图片数据
     * @param is_flash 是否闪照
     * @return 成功返回图片代码
     */
    std::string UploadFriendImage(std::int64_t thisQQ, std::int64_t friendQQ, const std::uint8_t *picture, size_t length, bool is_flash);

    /**
     * @brief 上传群图片
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param picture 图片数据
     * @param is_flash 是否闪照
     * @return 成功返回图片代码
     */
    std::string UploadGroupImage(std::int64_t thisQQ, std::int64_t groupQQ, const std::uint8_t *picture, size_t size, bool is_flash);

    /**
     * @brief 上传好友语音
     * @param thisQQ 框架QQ
     * @param friendQQ 好友QQ
     * @param audio 语音数据
     * @param audio_type 语音类型 0：普通语音，1：变声语音，2：文字语音，3：红包匹配语音
     * @param audio_text 语音文字 文字语音填附加文字(貌似会自动替换为语音对应的文本), 匹配语音填a、b、s、ss、sss，注意是小写
     * @return 成功返回语音代码
     */
    std::string UploadFriendAudio(std::int64_t thisQQ, std::int64_t friendQQ, const std::uint8_t *audio, size_t size, std::int32_t audio_type, const std::string &audio_text);

    /**
     * @brief 上传群语音
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param audio 语音数据
     * @param audio_type 语音类型 0：普通语音，1：变声语音，2：文字语音，3：红包匹配语音
     * @param audio_text 语音文字 文字语音填附加文字(貌似会自动替换为语音对应的文本), 匹配语音填a、b、s、ss、sss，注意是小写
     * @return 成功返回语音代码
     */
    std::string UploadGroupAudio(std::int64_t thisQQ, std::int64_t groupQQ, const std::uint8_t *audio, size_t size, std::int32_t audio_type = 0, const std::string &audio_text = "");

    /**
     * @brief 上传头像
     * @param thisQQ 框架QQ
     * @param picture 图片数据
     * @param size 图片大小
     */
    std::string UploadAvatar(std::int64_t thisQQ, const std::uint8_t *picture, size_t size);

    /**
     * @brief silk解码 无权限要求 尚未实现！
     * @param audio_file_path 音频文件路径 注意文件后缀必须和文件格式相对应
     */
    const std::uint8_t *SilkDecode(std::string audio_file_path);

    /**
     * @brief silk编码 无权限要求 尚未实现！
     * @param audio_file_path 音频文件路径 注意文件后缀必须和文件格式相对应
     */
    const std::uint8_t *SilkEncode(std::string audio_file_path);

    /**
     * @brief 设置群名片
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param otherQQ 群成员QQ
     * @param nickname 新名片
     */
    std::string SetGroupNickname(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t otherQQ, const std::string &nickname);

    /**
     * @brief 从缓存获取昵称
     * @param otherQQ 对方QQ
     * @return 成功返回昵称
     */
    std::string GetNameFromCache(std::int64_t otherQQ);

    /**
     * @brief 从缓存获取昵称
     * @param otherQQ 对方QQ
     * @return 成功返回昵称
     */
    std::string GetNameForce(std::int64_t otherQQ);

    /**
     * @brief 从缓存获取群名（如果是框架QQ没加的群，[查询群信息]后也会记录缓存）
     * @param groupQQ 群号
     * @return 成功返回群名称
     */
    std::string GetGroupNameFromCache(std::int64_t groupQQ);

    /**
         * @brief 获取SKey
         * @param thisQQ 框架QQ
         * @return 成功返回SKey
         */
    std::string GetSKey(std::int64_t thisQQ);

    /**
         * @brief 获取PSKey
         * @param thisQQ 框架QQ
         * @param domain 域: tenpay.com;openmobile.qq.com;docs.qq.com;connect.qq.com;qzone.qq.com;vip.qq.com;gamecenter.qq.com;qun.qq.com;game.qq.com;qqweb.qq.com;ti.qq.com;office.qq.com;mail.qq.com;mma.qq.com
         * @return 成功返回PSKey
         */
    std::string GetPSKey(std::int64_t thisQQ, const std::string &domain);

    /**
     * @brief 获取ClientKey
     * @param thisQQ 框架QQ
     * @return 成功返回ClientKey
     */
    std::string GetClientKey(std::int64_t thisQQ);

    /**
     * @brief 获取框架QQ
     */
    std::string GetThisQQ();

    /**
     * @brief 获取好友列表
     * @param thisQQ 框架QQ
     * @param friend_list 好友信息列表
     * @return 成功返回好友数量，失败或无权限返回0
     */
    size_t GetFriendList(std::int64_t thisQQ, std::vector<FriendInformation> &friend_list);

    /**
     * @brief 获取群列表
     * @param thisQQ 框架QQ
     * @param group_list 群信息列表
     * @return 成功返回群数量，失败或无权限返回0
     */
    size_t GetGroupList(std::int64_t thisQQ, std::vector<GroupInformation> &group_list);

    /**
     * @brief 获取群成员列表
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param group_member_list 群成员信息列表
     * @return 失败或无权限返回数量0
     */
    std::int32_t GetGroupMemberList(std::int64_t thisQQ, std::int64_t groupQQ, std::vector<GroupMemberInformation> &group_member_list);

    /**
     * @brief 设置管理员
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param otherQQ 群成员QQ
     * @param is_administrator 取消或设置管理
     * @return 失败或无权限返回false
     */
    bool SetAdministrator(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t otherQQ, bool is_administrator);

    /**
     * @brief 获取管理层列表
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @return 包括群主
     */
    std::string GetAdministratorList(std::int64_t thisQQ, std::int64_t groupQQ);

    /**
     * @brief 获取群名片
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param otherQQ 群成员QQ
     * @return 成功返回群名片，注意，如果群成员的群名片未设置或群名片为空白，返回结果均为空
     */
    std::string GetGroupNickname(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t otherQQ);

    /**
     * @brief 获取个性签名
     * @param thisQQ 框架QQ
     * @param otherQQ 对方QQ 可以填框架QQ本身
     * @return 成功返回个性签名
     */
    std::string GetSignature(std::int64_t thisQQ, std::int64_t otherQQ);

    /**
     * @brief 修改昵称
     * @param thisQQ 框架QQ
     * @param name 昵称
     * @return 失败或无权限返回false
     */
    bool SetName(std::int64_t thisQQ, std::string name);

    /**
     * @brief 修改个性签名
     * @param thisQQ 框架QQ
     * @param signature 签名
     * @return 失败或无权限返回false
     */
    bool SetSignature(std::int64_t thisQQ, std::string signature);

    /**
     * @brief 删除群成员
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param otherQQ 群成员QQ
     * @param is_verification_refused 拒绝加群申请
     * @return 失败或无权限返回false
     */
    bool RemoveGroupMember(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t otherQQ, bool is_verification_refused);

    /**
     * @brief 禁言群成员
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param otherQQ 群成员QQ
     * @param time 禁言时长 单位：秒
     * @return 失败或无权限返回false
     */
    bool ShutUpGroupMember(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t otherQQ, std::int32_t time);

    /**
     * @brief 退群
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @return 失败或无权限返回false
     */
    bool QuitGroup(std::int64_t thisQQ, std::int64_t groupQQ);

    /**
     * @brief 解散群
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @return 失败或无权限返回false
     */
    bool DissolveGroup(std::int64_t thisQQ, std::int64_t groupQQ);

    /**
     * @brief 上传群头像
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param picture pic
     * @return 失败或无权限返回false
     */
    bool UploadGroupAvatar(std::int64_t thisQQ, std::int64_t groupQQ, const std::uint8_t *picture);

    /**
     * @brief 全员禁言
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param is_shut_up_all 是否开启
     * @return 失败或无权限返回false
     */
    bool ShutUpAll(std::int64_t thisQQ, std::int64_t groupQQ, bool is_shut_up_all);

    /**
     * @brief 群权限_发起新的群聊
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param is_allowed 是否允许
     * @return 失败或无权限返回false
     */
    bool GroupPermission_CreateGroup(std::int64_t thisQQ, std::int64_t groupQQ, bool is_allowed);

    /**
     * @brief 群权限_发起临时会话
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param is_allowed 是否允许
     * @return 失败或无权限返回false
     */
    bool GroupPermission_CreateTemporaryConversation(std::int64_t thisQQ, std::int64_t groupQQ, bool is_allowed);

    /**
     * @brief 群权限_上传文件
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param is_allowed 是否允许
     * @return 失败或无权限返回false
     */
    bool GroupPermission_UploadFile(std::int64_t thisQQ, std::int64_t groupQQ, bool is_allowed);

    /**
     * @brief 群权限_上传相册
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param is_allowed 是否允许
     * @return 失败或无权限返回false
     */
    bool GroupPermission_UploadPicture(std::int64_t thisQQ, std::int64_t groupQQ, bool is_allowed);

    /**
     * @brief 群权限_邀请好友加群
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param is_allowed 是否允许
     * @return 失败或无权限返回false
     */
    bool GroupPermission_InviteFriend(std::int64_t thisQQ, std::int64_t groupQQ, bool is_allowed);

    /**
     * @brief 群权限_匿名聊天
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param is_allowed 是否允许
     * @return 失败或无权限返回false
     */
    bool GroupPermission_Anonymous(std::int64_t thisQQ, std::int64_t groupQQ, bool is_allowed);

    /**
     * @brief 群权限_坦白说
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param is_allowed 是否允许
     * @return 失败或无权限返回false
     */
    bool GroupPermission_ChatFrankly(std::int64_t thisQQ, std::int64_t groupQQ, bool is_allowed);

    /**
     * @brief 群权限_新成员查看历史消息
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param is_allowed 是否允许
     * @return 失败或无权限返回false
     */
    bool GroupPermission_NewMemberReadChatHistory(std::int64_t thisQQ, std::int64_t groupQQ, bool is_allowed);

    /**
     * @brief 群权限_邀请方式设置
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param method 方式 1 无需审核;2 需要管理员审核;3 100人以内无需审核
     * @return 失败或无权限返回false
     */
    bool GroupPermission_SetInviteMethod(std::int64_t thisQQ, std::int64_t groupQQ, std::int32_t method);

    /**
     * @brief 撤回消息_群聊 在群消息事件中使用，能收到并撤回自己发的消息
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param message_random 消息Random
     * @param message_req 消息Req
     * @return 失败或无权限返回false
     */
    bool Undo_Group(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t message_random, std::int32_t message_req);

    /**
     * @brief 撤回消息_私聊本身 用于撤回自己发的消息，其他设备的个人消息通知也可以撤回
     * @param thisQQ 框架QQ
     * @param otherQQ 对方QQ
     * @param message_random 消息Random
     * @param message_req 消息Req
     * @param time 消息接收时间
     * @return 失败或无权限返回false
     */
    bool Undo_Private(std::int64_t thisQQ, std::int64_t otherQQ, std::int64_t from_random, std::int32_t from_req, std::int32_t time);

    /**
     * @brief 设置位置共享（开启后需要上报位置，大约3s上报一次，否则会自动关闭）
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param Longitude 经度 如：121.711540
     * @param Latitude 纬度 如：31.403343
     * @param is_enabled 是否开启
     * @return 失败或无权限返回false
     */
    bool SetLocationShare(std::int64_t thisQQ, std::int64_t groupQQ, double Longitude, double Latitude, bool is_enabled);

    /**
     * @brief 上报当前位置（大约3s上报一次，不得过快）
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param Longitude 经度 如：121.711540
     * @param Latitude 纬度 如：31.403343
     * @return 失败或无权限返回false
     */
    bool ReportCurrent(std::int64_t thisQQ, std::int64_t groupQQ, double Longitude, double Latitude);

    /**
     * @brief 是否被禁言
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @return 返回禁言时长，单位秒，失败、无权限或未被禁言返回0
     */
    std::int64_t IsShuttedUp(std::int64_t thisQQ, std::int64_t groupQQ);

    /**
     * @brief 处理群验证事件 在群验证事件下使用，无权限时不执行
     * @param thisQQ 框架QQ
     * @param source_groupQQ 来源群号
     * @param triggerQQ 触发QQ
     * @param message_seq 消息Seq
     * @param operate_type 操作类型 11: 同意, 12: 拒绝, 14: 忽略
     * @param event_type 事件类型 群事件_某人申请加群(Group_MemberVerifying)或群事件_我被邀请加入群(Group_Invited);
     */
    void ProcessGroupVerificationEvent(std::int64_t thisQQ, std::int64_t source_groupQQ, std::int64_t triggerQQ, std::int64_t message_seq, std::int32_t operate_type, std::int32_t event_type);

    /**
     * @brief 处理好友验证事件 在好友验证事件下使用，无权限时不执行
     * @param thisQQ 框架QQ
     * @param triggerQQ 触发QQ
     * @param message_seq 消息Seq
     * @param operate_type 操作类型 1: 同意, 2: 拒绝
     */
    void ProcessFriendVerificationEvent(std::int64_t thisQQ, std::int64_t triggerQQ, std::int64_t message_seq, std::int32_t operate_type);

    /**
     * @brief 查看转发聊天记录内容 私聊消息也可以使用此命令解析，无权限时不执行
     * @param thisQQ 框架QQ
     * @param resID resID 可在xml消息代码中获取到
     * @param message_content 消息内容 私聊消息也可从该结构获取信息
     */
    void ReadForwardedChatHistory(std::int64_t thisQQ, std::string resID, std::vector<GroupMessageData> &message_content);

    /**
     * @brief 上传群文件 本命令为耗时操作，请另开线程执行，本命令不支持上百mb的文件，无权限时不执行
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param path 文件路径 本地文件路径
     * @param folder 文件夹 该空保留，暂时无效
     */
    void UploadGroupFile(std::int64_t thisQQ, std::int64_t groupQQ, std::string path, std::string folder = "");

    /**
     * @brief 创建群文件夹
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param folder 文件夹名
     * @return 失败或无权限返回false
     */
    bool CreateGroupFolder(std::int64_t thisQQ, std::int64_t groupQQ, std::string folder);

    /**
    * @brief 设置在线状态
    * @param thisQQ 框架QQ
    * @param main main 11: 在线, 31: 离开, 41: 隐身, 50: 忙碌, 60: Q我吧, 70: 请勿打扰
    * @param sun sun 当main=11时，可进一步设置 0: 普通在线, 1000: 我的电量, 1011: 信号弱, 1016: 睡觉中, 1017: 游戏中, 1018: 学习中, 1019: 吃饭中, 1021: 煲剧中, 1022: 度假中, 1024: 在线学习, 1025: 在家旅游, 1027: TiMi中, 1028: 我在听歌, 1032: 熬夜中, 1050: 打球中, 1051: 恋爱中, 1052: 我没事
    * @param battery 电量 sun=1000时，可以设置上报电量，取值1到100
    * @return 失败或无权限返回false
    */
    bool SetStatus(std::int64_t thisQQ, std::int32_t main, std::int32_t sun = 0, std::int32_t battery = 0);

    /**
     * @brief 判断某api是否有权限
     * @param permission 权限
     */
    bool CheckPermission(Permission permission);

    /**
     * @brief 判断某api是否有权限
     * @param permission 权限
     */
    bool CheckPermission(std::string permission);

    /**
     * @brief 重载自身 [暂时无效，请勿调用]没有权限限制，请勿将新的插件文件下载至plugin文件夹，请确保新旧文件appname一致
     * @param new_file_path 新文件路径 若要更新插件，可将插件文件下载后在此填写新文件路径
    */
    void ReloadPlugin(std::string new_file_path = "");

    /**
     * @brief 获取插件数据目录 没有权限限制，建议将设置文件之类的都写这里面
     * @return 插件数据目录 结果结尾带\
     */
    std::string GetPluginDataDirectory();

    /**
     * @brief QQ点赞 注意，非好友情况下进行点赞时返回成功，但不一定真正点上了
     * @param thisQQ 框架QQ
     * @param otherQQ 对方QQ
     */
    std::string QQLike(std::int64_t thisQQ, std::int64_t otherQQ);

    /**
     * @brief 获取图片下载地址
     * @param image_code 图片代码 支持群聊、私聊的图片、闪照代码，注意是图片代码
     * @param thisQQ 框架QQ 群聊图片必填，私聊图片必空
     * @param groupQQ 群号 群聊图片必填，私聊图片必空
     */
    std::string GetImageDownloadLink(std::string image_code, std::int64_t thisQQ = 0, std::int64_t groupQQ = 0);

    /**
     * @brief 查询好友信息
     * @param thisQQ 框架QQ
     * @param otherQQ 对方QQ
     * @param data 数据
     * @return 支持陌生人查询
     */
    bool GetFriendInformation(std::int64_t thisQQ, std::int64_t otherQQ, FriendInformation &data);

    /**
     * @brief 查询群信息
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param data 数据
     */
    bool GetGroupInformation(std::int64_t thisQQ, std::int64_t groupQQ, GroupCardInformation &data);

    /**
     * @brief 框架重启
     * @return 有权限限制，建议使用前检查是否具有权限
     */
    void Reboot();

    /**
     * @brief 转发群文件至群
     * @param thisQQ 框架QQ
     * @param source_groupQQ 来源群号
     * @param target_groupQQ 目标群号
     * @param fileID FileId
     * @return 失败或无权限返回false
     */
    bool ForwardGroupFileToGroup(std::int64_t thisQQ, std::int64_t source_groupQQ, std::int64_t target_groupQQ, std::string fileID);

    /**
     * @brief 转发群文件至好友
     * @param thisQQ 框架QQ
     * @param souce_groupQQ 来源群号
     * @param otherQQ 目标QQ
     * @param fileID FileId
     * @param file_name Filename
     * @param req Req 撤回消息用
     * @param random Random 撤回消息用
     * @param time time 撤回消息用
     * @return 失败或无权限返回false
     */
    bool ForwardGroupFileToFriend(std::int64_t thisQQ, std::int64_t souce_groupQQ, std::int64_t otherQQ, std::string fileID, std::string file_name, std::int32_t &req, std::int64_t &random, std::int32_t &time);

    /**
     * @brief 转发群文件至好友（若要撤回消息请使用另一重载）
     * @param thisQQ 框架QQ
     * @param souce_groupQQ 来源群号
     * @param otherQQ 目标QQ
     * @param fileID FileId
     * @param file_name Filename
     * @return 失败或无权限返回false
     */
    bool ForwardGroupFileToFriend(std::int64_t thisQQ, std::int64_t souce_groupQQ, std::int64_t otherQQ, std::string fileID, std::string file_name);

    /**
     * @brief 转发好友文件至好友
     * @param thisQQ 框架QQ
     * @param sourceQQ 来源QQ
     * @param targetQQ 目标QQ
     * @param fileID FileId
     * @param file_name Filename
     * @param req Req 撤回消息用
     * @param random Random 撤回消息用
     * @param time time 撤回消息用
     * @return 失败或无权限返回false
     */
    bool ForwardFriendFileToFriend(std::int64_t thisQQ, std::int64_t sourceQQ, std::int64_t targetQQ, std::string fileID, std::string file_name, std::int32_t &req, std::int32_t &random, std::int32_t &time);

    /**
     * @brief 转发好友文件至好友（若要撤回消息请使用另一重载）
     * @param thisQQ 框架QQ
     * @param sourceQQ 来源QQ
     * @param targetQQ 目标QQ
     * @param fileID FileId
     * @param file_name Filename
     * @return 失败或无权限返回false
     */
    bool ForwardFriendFileToFriend(std::int64_t thisQQ, std::int64_t sourceQQ, std::int64_t targetQQ, std::string fileID, std::string file_name);

    /**
     * @brief 设置群消息接收
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param set_type 设置类型 1: 接收并提醒, 2: 收进群助手, 3: 屏蔽群消息, 4: 接收不提醒
     * @return 失败或无权限返回false，此API未对返回结果进行分析，返回true不一定成功
     */
    bool SetGroupMessageReceive(std::int64_t thisQQ, std::int64_t groupQQ, std::int32_t set_type);

    /**
     * @brief 发送免费礼物（绕过广告）
     * @param thisQQ 框架QQ
     * @param groupQQ 群号
     * @param otherQQ 对方QQ
     * @param packageID packageID 299: 卡布奇诺, 302: 猫咪手表, 280: 牵你的手, 281: 可爱猫咪, 284: 神秘面具, 285: 甜wink, 286: 我超忙的, 289: 快乐肥宅水, 290: 幸运手链, 313: 坚强, 307: 绒绒手套, 312: 爱心口罩, 308: 彩虹糖果
     */
    std::string SendFreeGift(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t otherQQ, std::int32_t packageID);

    /**
     * @brief 获取好友在线状态
     * @param thisQQ 框架QQ
     * @param otherQQ 对方QQ
     * @return 失败或无权限返回空，支持查询陌生人
     */
    std::string GetFriendStatus(std::int64_t thisQQ, std::int64_t otherQQ);

    /**
     * @brief 获取QQ钱包个人信息
     * @param thisQQ 框架QQ
     * @param data 数据 获取银行卡信息时注意不要数组越界
     * @return 包括余额、名字、银行卡等
     */
    std::string GetQQWalletPersonalInformation(std::int64_t thisQQ, QQWalletInformation &data);

    /**
     * @brief 获取订单详情
     * @param thisQQ 框架QQ
     * @param orderID 订单号或者称之为listid
     * @param data 数据
     * @return 可以查订单，比如别人给你转账，你可以查询转账的详情
     */
    std::string GetOrderDetail(std::int64_t thisQQ, std::string orderID, OrderDetail &data);

    /**
     * @brief 提交支付验证码
     * @param thisQQ 框架QQ
     * @param captcha_information 验证码信息 银行卡发红包时传回的
     * @param captcha 验证码 手机收到的短信验证码
     * @param payment_password 支付密码 用于验证并支付
     * @return 用银行卡支付时需要验证，只需要验证一次
     */
    std::string SubmitPaymentCaptcha(std::int64_t thisQQ, volatile CaptchaInformation *captcha_information, std::string captcha, std::string payment_password);

    /**
     * @brief 分享音乐
     * @param thisQQ 框架QQ
     * @param otherQQ 分享对象 分享的群或分享的好友QQ
     * @param music_name 歌曲名
     * @param artist_name 歌手名
     * @param redirect_link 跳转地址 点击音乐json后跳转的地址
     * @param cover_link 封面地址 音乐的封面图片地址
     * @param file_path 文件地址 音乐源文件地址，如https://xxx.com/xxx.mp3
     * @param application_type 应用类型 0QQ音乐 1虾米音乐 2酷我音乐 3酷狗音乐 4网易云音乐  默认0
     * @param share_type 分享类型 0私聊 1群聊  默认0
     * @return 失败或无权限返回false
     */
    bool ShareMusic(std::int64_t thisQQ, std::int64_t otherQQ, std::string music_name, std::string artist_name, std::string redirect_link, std::string cover_link, std::string file_path, std::int32_t app_type = 0, std::int32_t share_type = 0);

    /**
     * @brief 更改群聊消息内容（使用此命令可以更改当前群聊消息内容，并使更改后的内容投递给之后的插件）
     * @param data_pointer 数据指针
     * @param new_message_content 新消息内容
     * @return 无权限返回false
     */
    bool ModifyGroupMessageContent(std::int32_t data_pointer, std::string new_message_content);

    /**
     * @brief 更改私聊消息内容（使用此命令可以更改当前私聊消息内容，并使更改后的内容投递给之后的插件）
     * @param data_pointer 数据指针
     * @param new_message_content 新消息内容
     * @return 无权限返回false
     */
    bool ModifyPrivateMessageContent(std::int32_t data_pointer, std::string new_message_content);

    /**
     * @brief 群聊口令红包
     * @param thisQQ 框架QQ
     * @param total_number 总数量
     * @param total_amount 总金额 单位分
     * @param groupQQ 群号
     * @param password 口令
     * @param payment_password 支付密码
     * @param card_serial 银行卡序列 大于0时使用银行卡支付
     */
    std::string GroupPasswordRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t groupQQ, std::string password, std::string payment_password, std::int32_t card_serial = 0);

    /**
     * @brief 群聊拼手气红包
     * @param thisQQ 框架QQ
     * @param total_number 总数量
     * @param total_amount 总金额 单位分
     * @param groupQQ 群号
     * @param blessing 祝福语
     * @param payment_password 支付密码
     * @param card_serial 银行卡序列 大于0时使用银行卡支付
     * @param skinID 红包皮肤Id 1522光与夜之恋
     */
    std::string GroupRandomRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t groupQQ, std::string blessing, std::string payment_password, std::int32_t card_serial = 0, std::int32_t skinID = 0);

    /**
     * @brief 群聊普通红包
     * @param thisQQ 框架QQ
     * @param total_number 总数量
     * @param total_amount 总金额 单位分
     * @param groupQQ 群号
     * @param blessing 祝福语
     * @param payment_password 支付密码
     * @param card_serial 银行卡序列 大于0时使用银行卡支付
     * @param skinID 红包皮肤Id 1522: 光与夜之恋, 1527: 代号：三国（打了一辈子仗）, 1525: 街霸：对决, 1518: 代号：三国（俺送红包来了）, 1476: 天涯明月刀, 1512: 一人之下，其他皮肤id自己找
     */
    std::string GroupNormalRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t groupQQ, std::string blessing, std::string payment_password, std::int32_t card_serial = 0, std::int32_t skinID = 0);

    /**
     * @brief 群聊画图红包
     * @param thisQQ 框架QQ
     * @param total_number 总数量
     * @param total_amount 总金额 单位分
     * @param groupQQ 群号
     * @param question 题目名 只能填手Q有的，如：庄周
     * @param payment_password 支付密码
     * @param card_serial 银行卡序列 大于0时使用银行卡支付
     */
    std::string GroupDrawRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t groupQQ, std::string question, std::string payment_password, std::int32_t card_serial = 0);

    /**
     * @brief 群聊语音红包
     * @param thisQQ 框架QQ
     * @param total_number 总数量
     * @param total_amount 总金额 单位分
     * @param groupQQ 群号
     * @param audio_password 语音口令
     * @param payment_password 支付密码
     * @param card_serial 银行卡序列 大于0时使用银行卡支付
     */
    std::string GroupAudioRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t groupQQ, std::string audio_password, std::string payment_password, std::int32_t card_serial = 0);

    /**
     * @brief 群聊接龙红包
     * @param thisQQ 框架QQ
     * @param total_number 总数量
     * @param total_amount 总金额 单位分
     * @param groupQQ 群号
     * @param follow_content 接龙内容
     * @param payment_password 支付密码
     * @param card_serial 银行卡序列 大于0时使用银行卡支付
     */
    std::string GroupFollowRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t groupQQ, std::string follow_content, std::string payment_password, std::int32_t card_serial = 0);

    /**
     * @brief 群聊专属红包
     * @param thisQQ 框架QQ
     * @param total_number 总数量
     * @param total_amount 总金额 单位分
     * @param groupQQ 群号
     * @param otherQQ 领取人 多个领取人QQ用|分隔
     * @param blessing 祝福语
     * @param payment_password 支付密码
     * @param card_serial 银行卡序列 大于0时使用银行卡支付
     * @param is_equal 是否均分 默认不均分（拼手气）
     */
    std::string GroupExclusiveRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t groupQQ, std::string otherQQ, std::string blessing, std::string payment_password = "", std::int32_t card_serial = 0, bool is_equal = false);

    /**
     * @brief 好友口令红包 不支持非好友
     * @param thisQQ 框架QQ
     * @param total_number 总数量
     * @param total_amount 总金额 单位分
     * @param otherQQ 对方QQ
     * @param password 口令
     * @param payment_password 支付密码
     * @param card_serial 银行卡序列 大于0时使用银行卡支付
     */
    std::string FriendPasswordRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t otherQQ, std::string password, std::string payment_password, std::int32_t card_serial = 0);
    /**
     * @brief 好友普通红包 不支持非好友
     * @param thisQQ 框架QQ
     * @param total_number 总数量
     * @param total_amount 总金额 单位分
     * @param otherQQ 对方QQ
     * @param blessing 祝福语
     * @param payment_password 支付密码
     * @param card_serial 银行卡序列 大于0时使用银行卡支付
     * @param skinID 红包皮肤Id 1522: 光与夜之恋, 1527: 代号：三国（打了一辈子仗）, 1525: 街霸：对决, 1518: 代号：三国（俺送红包来了）, 1476: 天涯明月刀, 1512: 一人之下，其他皮肤id自己找
     */
    std::string FriendNormalRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t otherQQ, std::string blessing, std::string payment_password, std::int32_t card_serial = 0, std::int32_t skinID = 0);

    /**
     * @brief 好友画图红包 不支持非好友
     * @param thisQQ 框架QQ
     * @param total_number 总数量
     * @param total_amount 总金额 单位分
     * @param otherQQ 对方QQ
     * @param question 题目名 只能填手Q有的，如：庄周
     * @param payment_password 支付密码
     * @param card_serial 银行卡序列 大于0时使用银行卡支付
     */
    std::string FriendDrawRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t otherQQ, std::string question, std::string payment_password, std::int32_t card_serial = 0);

    /**
     * @brief 好友语音红包 不支持非好友
     * @param thisQQ 框架QQ
     * @param total_number 总数量
     * @param total_amount 总金额 单位分
     * @param otherQQ 对方QQ
     * @param audio_password 语音口令
     * @param payment_password 支付密码
     * @param card_serial 银行卡序列 大于0时使用银行卡支付
     */
    std::string FriendAudioRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t otherQQ, std::string audio_password, std::string payment_password, std::int32_t card_serial = 0);

    /**
     * @brief 好友接龙红包 不支持非好友
     * @param thisQQ 框架QQ
     * @param total_number 总数量
     * @param total_amount 总金额 单位分
     * @param otherQQ 对方QQ
     * @param follow_content 接龙内容
     * @param payment_password 支付密码
     * @param card_serial 银行卡序列 大于0时使用银行卡支付
     */
    std::string FriendFollowRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t otherQQ, std::string follow_content, std::string payment_password, std::int32_t card_serial = 0);

};

#endif // CORNERSTONE_HEADER_API_H_

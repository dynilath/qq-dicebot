/*
Cornerstone SDK
-- Corn SDK for Modern C++

Licensed under the MIT License
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

#include "api.h"
/**
 * @brief 载入插件关键字和系统回调函数指针
 * @param api_data 系统回调函数指针JSON
 * @param plugin_key 插件标识符
 */
API::API(etext api_data, etext plugin_key)
    : j(Json::parse(GBKtoUTF8(api_data)))
{
    size_t size = std::strlen(plugin_key);
    this->key = new char[size + 1];
    std::strcpy(this->key, plugin_key);
    this->key[size] = '\0';
}

API::~API()
{
    delete[] this->key;
}

/**
 * @brief 输出日志
 * @param message 日志
 * @param text_color 文字颜色
 * @param background_color 背景颜色
 */
string API::OutputLog(const string &message, int32_t text_color,
                      int32_t background_color)
{
    return e2s(_f<etext(etext, etext, eint, eint)>
        (this->j, "输出日志")
        (this->key, s2e(message), text_color, background_color));
}

/**
 * @brief 发送好友消息
 * @param thisQQ 框架QQ
 * @param friendQQ 好友QQ
 * @param content 发送内容
 * @param random 撤回消息用
 * @param req 撤回消息用
 * @return 成功返回的time用于撤回消息
 */
string API::SendFriendMessage(int64_t thisQQ, int64_t friendQQ,
                              const string &content, int64_t *random, int32_t *req)
{
    auto a = random ? *random : 0;
    auto b = req ? *req : 0;
    return e2s(_f<etext(etext, elong, elong, etext, elong *, eint *)>
        (this->j, "发送好友消息")
        (this->key, thisQQ, friendQQ, s2e(content), &a, &b));
}

/**
 * @brief 发送群消息
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param content 发送内容
 * @param anonymous 匿名发送
 */
string API::SendGroupMessage(int64_t thisQQ, int64_t groupQQ,
                             const string &content, bool anonymous)
{
    return e2s(_f<etext(etext, elong, elong, etext, ebool)>
        (this->j, "发送群消息")
        (this->key, thisQQ, groupQQ, s2e(content), b2e(anonymous)));
}

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
string API::SendGroupTemporaryMessage(int64_t thisQQ, int64_t groupQQ,
                                      int64_t otherQQ, const string &content,
                                      int64_t *random, int32_t *req)
{
    auto a = random ? *random : 0;
    auto b = req ? *req : 0;
    return e2s(_f<etext(etext, elong, elong, elong, etext, elong *, eint *)>
        (this->j, "发送群临时消息")
        (this->key, thisQQ, groupQQ, otherQQ, s2e(content.c_str()), &a, &b));
}

/**
 * @brief 添加好友
 * @param thisQQ 框架QQ
 * @param otherQQ 对方QQ
 * @param verification 设置回答问题答案或是验证消息，多个问题答案用"|"分隔开
 */
string API::AddFriend(int64_t thisQQ, int64_t otherQQ,
                      const string &verification)
{
    return e2s(_f<etext(etext, elong, elong, etext)>
        (this->j, "添加好友")
        (this->key, thisQQ, otherQQ, s2e(verification.c_str())));
}

/**
 * @brief 添加群（你在群内或需要付费入群也会直接发送验证消息）
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param verification 回答问题答案或是验证消息
 */
string API::AddGroup(int64_t thisQQ, int64_t groupQQ,
                     const string &verification)
{
    return e2s(_f<etext(etext, elong, elong, etext)>
        (this->j, "添加群")
        (this->key, thisQQ, groupQQ, s2e(verification.c_str())));
}

/**
 * @brief 删除好友
 * @param thisQQ 框架QQ
 * @param otherQQ 对方QQ
 */
string API::RemoveFriend(int64_t thisQQ, int64_t otherQQ)
{
    return e2s(_f<etext(etext, elong, elong)>
        (this->j, "删除好友")
        (this->key, thisQQ, otherQQ));
}

/**
 * @brief 设置屏蔽好友
 * @param thisQQ 框架QQ
 * @param otherQQ 对方QQ
 * @param is_blocked 是否屏蔽
 */
string API::SetBlockFriend(int64_t thisQQ, int64_t otherQQ,
                           bool is_blocked)
{
    return e2s(_f<etext(etext, elong, elong, ebool)>
        (this->j, "置屏蔽好友")
        (this->key, thisQQ, otherQQ, b2e(is_blocked)));
}

/**
 * @brief 设置特别关心好友
 * @param thisQQ 框架QQ
 * @param otherQQ 对方QQ
 * @param is_special 是否屏蔽
 */
string API::SetSpecialFriend(int64_t thisQQ, int64_t otherQQ,
                             bool is_special)
{
    return e2s(_f<etext(etext, elong, elong, ebool)>
        (this->j, "置特别关心好友")
        (this->key, thisQQ, otherQQ, b2e(is_special)));
}

/**
 * @brief 发送好友JSON消息
 * @param thisQQ 框架QQ
 * @param friendQQ 好友QQ
 * @param json_content json发送内容
 * @param random 撤回消息用
 * @param req 撤回消息用
 * @return 成功返回的time用于撤回消息
 */
string API::SendFriendJSONMessage(int64_t thisQQ, int64_t friendQQ,
                                  const string &json_content,
                                  int64_t *random, int32_t *req)
{
    auto a = random ? *random : 0;
    auto b = req ? *req : 0;
    return e2s(_f<etext(etext, elong, elong, etext, elong*, eint*)>
        (this->j, "发送好友json消息")
        (this->key, thisQQ, friendQQ, s2e(json_content), &a, &b));
}

/**
 * @brief 发送群JSON消息
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param json_content json发送内容
 * @param anonymous 匿名发送
 */
string API::SendGroupJSONMessage(int64_t thisQQ, int64_t groupQQ,
                                 const string &json_content, bool anonymous)
{
    return e2s(_f<etext(etext, elong, elong, etext, bool)>
        (this->j, "发送群json消息")
        (this->key, thisQQ, groupQQ, s2e(json_content), anonymous));
}

/**
 * @brief 上传好友图片
 * @param thisQQ 框架QQ
 * @param friendQQ 好友QQ
 * @param picture 图片数据
 * @param is_flash 是否闪照
 * @return 成功返回图片代码
 */
string API::UploadFriendImage(int64_t thisQQ, int64_t friendQQ,
                              const uint8_t *picture, size_t length, bool is_flash)
{
    return e2s(_f<etext(etext, elong, elong, ebool, ebin, eint)>
        (this->j, "上传好友图片")
        (this->key, thisQQ, friendQQ, b2e(is_flash), picture, static_cast<eint>(length)));
}

/**
 * @brief 上传群图片
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param picture 图片数据
 * @param is_flash 是否闪照
 * @return 成功返回图片代码
 */
string API::UploadGroupImage(int64_t thisQQ, int64_t groupQQ,
                             const uint8_t *picture, size_t size, bool is_flash)
{
    return e2s(_f<etext(etext, elong, elong, ebool, ebin, eint)>
        (this->j, "上传群图片")
        (this->key, thisQQ, groupQQ, b2e(is_flash), picture, static_cast<eint>(size)));
}

/**
 * @brief 上传好友语音
 * @param thisQQ 框架QQ
 * @param friendQQ 好友QQ
 * @param audio 语音数据
 * @param audio_type 语音类型 0：普通语音，1：变声语音，2：文字语音，3：红包匹配语音
 * @param audio_text 语音文字 文字语音填附加文字(貌似会自动替换为语音对应的文本), 匹配语音填a、b、s、ss、sss，注意是小写
 * @return 成功返回语音代码
 */
string API::UploadFriendAudio(int64_t thisQQ, int64_t friendQQ,
                              const uint8_t *audio, size_t size,
                              int32_t audio_type, const string &audio_text)
{
    return e2s(_f<etext(etext, elong, elong, eint, etext, ebin, eint)>
        (this->j, "上传好友语音")
        (this->key, thisQQ, friendQQ, audio_type, s2e(audio_text), audio, static_cast<eint>(size)));
}

/**
 * @brief 上传群语音
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param audio 语音数据
 * @param audio_type 语音类型 0：普通语音，1：变声语音，2：文字语音，3：红包匹配语音
 * @param audio_text 语音文字 文字语音填附加文字(貌似会自动替换为语音对应的文本), 匹配语音填a、b、s、ss、sss，注意是小写
 * @return 成功返回语音代码
 */
string API::UploadGroupAudio(int64_t thisQQ, int64_t groupQQ,
                             const uint8_t *audio, size_t size,
                             int32_t audio_type, const string &audio_text)
{
    return e2s(_f<etext(etext, elong, elong, eint, etext, ebin, eint)>
        (this->j, "上传群语音")
        (this->key, thisQQ, groupQQ, audio_type, s2e(audio_text), audio, static_cast<eint>(size)));
}

/**
 * @brief 上传头像
 * @param thisQQ 框架QQ
 * @param picture 图片数据
 * @param size 图片大小
 */
string API::UploadAvatar(int64_t thisQQ, const uint8_t *picture, size_t size)
{
    return e2s(_f<etext(etext, elong, ebin, eint)>
        (this->j, "上传头像")
        (this->key, thisQQ, picture, static_cast<eint>(size)));
}

/**
 * @brief silk解码 无权限要求 尚未实现！
 * @param audio_file_path 音频文件路径 注意文件后缀必须和文件格式相对应
 */
const uint8_t* API::SilkDecode(string audio_file_path)
{
    return nullptr;
}

/**
 * @brief silk编码 无权限要求 尚未实现！
 * @param audio_file_path 音频文件路径 注意文件后缀必须和文件格式相对应
 */
const uint8_t* API::SilkEncode(string audio_file_path)
{
    return nullptr;
}

/**
 * @brief 设置群名片
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param otherQQ 群成员QQ
 * @param nickname 新名片
 */
string API::SetGroupNickname(int64_t thisQQ, int64_t groupQQ,
                             int64_t otherQQ, const string &nickname)
{
    return e2s(_f<etext(etext, elong, elong, elong, etext)>
        (this->j, "设置群名片")
        (this->key, thisQQ, groupQQ, otherQQ, s2e(nickname)));
}

/**
 * @brief 从缓存取昵称
 * @param otherQQ 对方QQ
 * @return 成功返回昵称
 */
string API::GetNameFromCache(int64_t otherQQ)
{
    return e2s(_f<etext(etext, elong)>
        (this->j, "取昵称_从缓存")
        (this->key, otherQQ));
}

/**
 * @brief 从缓存取昵称
 * @param otherQQ 对方QQ
 * @return 成功返回昵称
 */
string API::GetNameForce(int64_t otherQQ)
{
    return e2s(_f<etext(etext, elong)>
        (this->j, "强制取昵称")
        (this->key, otherQQ));
}

/**
 * @brief 从缓存取群名（如果是框架QQ没加的群，[查询群信息]后也会记录缓存）
 * @param groupQQ 群号
 * @return 成功返回群名称
 */
string API::GetGroupNameFromCache(int64_t groupQQ)
{
    return e2s(_f<etext(etext, elong)>
        (this->j, "取群名称_从缓存")
        (this->key, groupQQ));
}

/**
     * @brief 获取SKey
     * @param thisQQ 框架QQ
     * @return 成功返回SKey
     */
string API::GetSKey(int64_t thisQQ)
{
    return e2s(_f<etext(etext, elong)>
        (this->j, "获取skey")
        (this->key, thisQQ));
}

/**
     * @brief 获取PSKey
     * @param thisQQ 框架QQ
     * @param domain 域: tenpay.com;openmobile.qq.com;docs.qq.com;connect.qq.com;qzone.qq.com;vip.qq.com;gamecenter.qq.com;qun.qq.com;game.qq.com;qqweb.qq.com;ti.qq.com;office.qq.com;mail.qq.com;mma.qq.com
     * @return 成功返回PSKey
     */
string API::GetPSKey(int64_t thisQQ, const string &domain)
{
    return e2s(_f<etext(etext, elong)>
        (this->j, "获取pskey")
        (this->key, thisQQ));
}

/**
 * @brief 获取ClientKey
 * @param thisQQ 框架QQ
 * @return 成功返回ClientKey
 */
string API::GetClientKey(int64_t thisQQ)
{
    return e2s(_f<etext(etext, elong)>
        (this->j, "获取clientkey")
        (this->key, thisQQ));
}

/**
 * @brief 获取框架QQ
 */
string API::GetThisQQ()
{
    return e2s(_f<etext(etext)>
        (this->j, "取框架QQ")
        (this->key));
}

/**
 * @brief 获取好友列表
 * @param thisQQ 框架QQ
 * @return 成功返回好友数量，失败或无权限返回0
 */
size_t API::GetFriendList(int64_t thisQQ, vector<FriendInformation> &friendList)
{
    int number;
    earray* info;
    number = _f<eint(etext, elong, earray**)>
                (this->j, "取好友列表")
                (this->key, thisQQ, &info);
    earray1D::UnpackStruct<FriendInformation>(info, friendList);
    delete info;
    return number;
}

// FIXME: 此类获取列表的API存在重大问题！！参照上面的获取好友列表进行修改
/**
 * @brief 获取群列表
 * @param thisQQ 框架QQ
 * @param friendList 群信息列表
 * @return 成功返回群数量，失败或无权限返回0
 */
size_t API::GetGroupList(int64_t thisQQ, vector<GroupInformation> &groupList)
{
    int size;
    earray* info;
    size = _f<eint(etext, elong, earray**)>
                (this->j, "取群列表")
                (this->key, thisQQ, &info);
    earray1D::UnpackStruct<GroupInformation>(info, groupList);
    delete info;
    return size;
}

/**
 * @brief 取群成员列表
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param group_member_list 数据
 * @return 失败或无权限返回数量0
 */
int32_t API::GetGroupMemberList(int64_t thisQQ, int64_t groupQQ, vector<GroupMemberInformation>& group_member_list)
{
    int number;
    earray* info;
    number = _f<eint(etext, elong, elong, earray**)>
                (this->j, "取群成员列表")
                (this->key, thisQQ, groupQQ, &info);
    earray1D::UnpackStruct<GroupMemberInformation>(info, group_member_list);
    delete info;
    return 0;
}

/**
 * @brief 设置管理员
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param otherQQ 群成员QQ
 * @param is_administrator 取消管理 取消或设置
 * @return 失败或无权限返回假
 */
bool API::SetAdministrator(int64_t thisQQ, int64_t groupQQ, int64_t otherQQ, bool is_administrator)
{
    return e2b(_f<ebool(etext, elong, elong, elong, ebool)>
        (this->j, "设置管理员")
        (this->key, thisQQ, groupQQ, otherQQ, b2e(is_administrator)));
}

/**
 * @brief 获取管理层列表
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @return 包括群主
 */
string API::GetAdministratorList(int64_t thisQQ, int64_t groupQQ)
{
    return e2s(_f<etext(etext, elong, elong)>
        (this->j, "取管理层列表")
        (this->key, thisQQ, groupQQ));
}

/**
 * @brief 取群名片
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param otherQQ 群成员QQ
 * @return 成功返回群名片，注意，如果群成员的群名片未设置或群名片为空白，返回结果均为空
 */
string API::GetGroupNickname(int64_t thisQQ, int64_t groupQQ, int64_t otherQQ)
{
    return e2s(_f<etext(etext, elong, elong, elong)>
        (this->j, "取群名片")
        (this->key, thisQQ, groupQQ, otherQQ));
}

/**
 * @brief 取个性签名
 * @param thisQQ 框架QQ
 * @param otherQQ 对方QQ 可以填框架QQ本身
 * @return 成功返回个性签名
 */
string API::GetSignature(int64_t thisQQ, int64_t otherQQ)
{
    return e2s(_f<etext(etext, elong, elong)>
        (this->j, "取个性签名")
        (this->key, thisQQ, otherQQ));
}

/**
 * @brief 修改昵称
 * @param thisQQ 框架QQ
 * @param name 昵称
 * @return 失败或无权限返回假
 */
bool API::SetName(int64_t thisQQ, string name)
{
    return e2b(_f<ebool(etext, elong, etext)>
        (this->j, "修改昵称")
        (this->key, thisQQ, s2e(name)));
}

/**
 * @brief 修改个性签名
 * @param thisQQ 框架QQ
 * @param signature 签名
 * @return 失败或无权限返回假
 */
bool API::SetSignature(int64_t thisQQ, string signature)
{
    return e2b(_f<ebool(etext, elong, etext)>
        (this->j, "修改个性签名")
        (this->key, thisQQ, s2e(signature)));
}

 
/**
 * @brief 删除群成员
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param otherQQ 群成员QQ
 * @param is_verification_refused 拒绝加群申请
 * @return 失败或无权限返回假
 */
bool API::RemoveGroupMember(int64_t thisQQ, int64_t groupQQ, int64_t otherQQ, bool is_verification_refused)
{
    return e2b(_f<ebool(etext, elong, elong, elong, ebool)>
        (this->j, "删除群成员")
        (this->key, thisQQ, groupQQ, otherQQ, b2e(is_verification_refused)));
}

/**
 * @brief 禁言群成员
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param otherQQ 群成员QQ
 * @param time 禁言时长 单位：秒
 * @return 失败或无权限返回假
 */
bool API::ShutUpGroupMember(int64_t thisQQ, int64_t groupQQ, int64_t otherQQ, int32_t time)
{
    return e2b(_f<ebool(etext, elong, elong, elong, eint)>
        (this->j, "禁言群成员")
        (this->key, thisQQ, groupQQ, otherQQ, time));
}

/**
 * @brief 退群
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @return 失败或无权限返回假
 */
bool API::QuitGroup(int64_t thisQQ, int64_t groupQQ)
{
    return e2b(_f<ebool(etext, elong, elong)>
        (this->j, "退群")
        (this->key, thisQQ, groupQQ));
}

/**
 * @brief 解散群
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @return 失败或无权限返回假
 */
bool API::DissolveGroup(int64_t thisQQ, int64_t groupQQ)
{
    return e2b(_f<ebool(etext, elong, elong)>
        (this->j, "解散群")
        (this->key, thisQQ, groupQQ));
}

/**
 * @brief 上传群头像
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param picture pic
 * @return 失败或无权限返回假
 */
bool API::UploadGroupAvatar(int64_t thisQQ, int64_t groupQQ, const uint8_t *picture)
{
    return e2b(_f<ebool(etext, elong, elong, ebin)>
        (this->j, "上传群头像")
        (this->key, thisQQ, groupQQ, picture));
}

/**
 * @brief 全员禁言
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param is_shut_up_all 是否开启
 * @return 失败或无权限返回假
 */
bool API::ShutUpAll(int64_t thisQQ, int64_t groupQQ, bool is_shut_up_all)
{
    return e2b(_f<ebool(etext, elong, elong, ebool)>
        (this->j, "全员禁言")
        (this->key, thisQQ, groupQQ, is_shut_up_all));
}

/**
 * @brief 群权限_发起新的群聊
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param is_allowed 是否允许
 * @return 失败或无权限返回假
 */
bool API::GroupPermission_CreateGroup(int64_t thisQQ, int64_t groupQQ, bool is_allowed)
{
    return e2b(_f<ebool(etext, elong, elong, ebool)>
        (this->j, "群权限_发起新的群聊")
        (this->key, thisQQ, groupQQ, b2e(is_allowed)));
}

/**
 * @brief 群权限_发起临时会话
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param is_allowed 是否允许
 * @return 失败或无权限返回假
 */
bool API::GroupPermission_CreateTemporaryConversation(int64_t thisQQ, int64_t groupQQ, bool is_allowed)
{
    return e2b(_f<ebool(etext, elong, elong, ebool)>
            (this->j, "群权限_发起临时会话")
            (this->key, thisQQ, groupQQ, b2e(is_allowed)));
}

/**
 * @brief 群权限_上传文件
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param is_allowed 是否允许
 * @return 失败或无权限返回假
 */
bool API::GroupPermission_UploadFile(int64_t thisQQ, int64_t groupQQ, bool is_allowed)
{
    return e2b(_f<ebool(etext, elong, elong, ebool)>
                (this->j, "群权限_上传文件")
                (this->key, thisQQ, groupQQ, b2e(is_allowed)));
}


/**
 * @brief 群权限_上传相册
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param is_allowed 是否允许
 * @return 失败或无权限返回假
 */
bool API::GroupPermission_UploadPicture(int64_t thisQQ, int64_t groupQQ, bool is_allowed)
{
return e2b(_f<ebool(etext, elong, elong, ebool)>
                (this->j, "群权限_上传相册")
                (this->key, thisQQ, groupQQ, b2e(is_allowed)));
                    
}

/**
 * @brief 群权限_邀请好友加群
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param is_allowed 是否允许
 * @return 失败或无权限返回假
 */
bool API::GroupPermission_InviteFriend(int64_t thisQQ, int64_t groupQQ, bool is_allowed)
{
    return e2b(_f<ebool(etext, elong, elong, ebool)>
                (this->j, "群权限_邀请好友加群")
                (this->key, thisQQ, groupQQ, b2e(is_allowed)));
}

/**
 * @brief 群权限_匿名聊天
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param is_allowed 是否允许
 * @return 失败或无权限返回假
 */
bool API::GroupPermission_Anonymous(int64_t thisQQ, int64_t groupQQ, bool is_allowed)
{

    return e2b(_f<ebool(etext, elong, elong, ebool)>
                (this->j, "群权限_匿名聊天")
                (this->key, thisQQ, groupQQ, b2e(is_allowed)));
}

/**
 * @brief 群权限_坦白说
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param is_allowed 是否允许
 * @return 失败或无权限返回假
 */
bool API::GroupPermission_ChatFrankly(int64_t thisQQ, int64_t groupQQ, bool is_allowed)
{
    return e2b(_f<ebool(etext, elong, elong, ebool)>
                (this->j, "群权限_坦白说")
                (this->key, thisQQ, groupQQ, b2e(is_allowed)));
}

/**
 * @brief 群权限_新成员查看历史消息
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param is_allowed 是否允许
 * @return 失败或无权限返回假
 */
bool API::GroupPermission_NewMemberReadChatHistory(int64_t thisQQ, int64_t groupQQ, bool is_allowed)
{
    return e2b(_f<ebool(etext, elong, elong, ebool)>
                (this->j, "群权限_新成员查看历史消息")
                (this->key, thisQQ, groupQQ, b2e(is_allowed)));
}

/**
 * @brief 群权限_邀请方式设置
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param method 方式 1 无需审核;2 需要管理员审核;3 100人以内无需审核
 * @return 失败或无权限返回假
 */
bool API::GroupPermission_SetInviteMethod(int64_t thisQQ, int64_t groupQQ, int32_t method)
{
    return e2b(_f<ebool(etext, elong, elong, eint)>
                (this->j, "群权限_邀请方式设置")
                (this->key, thisQQ, groupQQ, method));
}

/**
 * @brief 撤回消息_群聊
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param fromRandom fromRandom
 * @param fromReq fromReq
 * @return 在群消息事件中使用，能收到并撤回自己发的消息，失败或无权限返回假
 */
bool API::Undid_Group(int64_t thisQQ, int64_t groupQQ, int64_t from_random, int32_t from_req)
{
    return e2b(_f<ebool(etext, elong, elong, elong, eint)>
                (this->j, "撤回消息_群聊")
                (this->key, thisQQ, groupQQ, from_random, from_req));
}

/**
 * @brief 撤回消息_私聊本身
 * @param thisQQ 框架QQ
 * @param otherQQ 对方QQ
 * @param from_random fromRandom
 * @param from_req fromReq
 * @param time time
 * @return 用于撤回自己发的消息，其他设备的个人消息通知也可以撤回
 */
bool API::Undid_Private(int64_t thisQQ, int64_t otherQQ, int64_t from_random, int32_t from_req, int32_t time)
{
    return e2b(_f<ebool(etext, elong, elong, elong, eint, eint)>
                (this->j, "撤回消息_私聊本身")
                (this->key, thisQQ, otherQQ, from_random, from_req, time));
}

/**
 * @brief 设置位置共享（开启后需要上报位置，大约3s上报一次，否则会自动关闭）
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param Longitude 经度 如：121.711540
 * @param Latitude 纬度 如：31.403343
 * @param is_enabled 是否开启
 * @return 失败或无权限返回假
 */ 
bool API::SetLocationShare(int64_t thisQQ, int64_t groupQQ, double Longitude, double Latitude, bool is_enabled)
{
    return e2b(_f<ebool(etext, elong, elong, edouble, edouble, ebool)>
                (this->j, "设置位置共享")
                (this->key, thisQQ, groupQQ, Longitude, Latitude, b2e(is_enabled)));
}

/**
 * @brief 上报当前位置（大约3s上报一次，不得过快）
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param Longitude 经度 如：121.711540
 * @param Latitude 纬度 如：31.403343
 * @return 失败或无权限返回假
 */
bool API::ReportCurrent(int64_t thisQQ, int64_t groupQQ, double Longitude, double Latitude)
{
    return e2b(_f<ebool(etext, elong, elong, edouble, edouble)>
                (this->j, "上报当前位置")
                (this->key, thisQQ, groupQQ, Longitude, Latitude));
}

/**
 * @brief 是否被禁言
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @return 返回禁言时长，单位秒，失败、无权限或未被禁言返回0
 */
int64_t API::IsShuttedUp(int64_t thisQQ, int64_t groupQQ)
{
    return _f<elong(etext, elong, elong)>
            (this->j, "是否被禁言")
            (this->key, thisQQ, groupQQ);
}


 /**
 * @brief 处理群验证事件 在群验证事件下使用，无权限时不执行
 * @param thisQQ 框架QQ
 * @param source_groupQQ 来源群号
 * @param triggerQQ 触发QQ
 * @param message_seq 消息Seq
 * @param operate_type 操作类型 11: 同意, 12: 拒绝, 14: 忽略
 * @param event_type 事件类型 群事件_某人申请加群(Group_MemberVerifying)或群事件_我被邀请加入群(Group_Invited)
 */
void API::ProcessGroupVerificationEvent(int64_t thisQQ, int64_t source_groupQQ, int64_t triggerQQ, int64_t message_seq, int32_t operate_type, int32_t event_type)
{
    return _f<void(etext, elong, elong, elong, elong, eint, eint)>
            (this->j, "处理群验证事件")
            (this->key, thisQQ, source_groupQQ, triggerQQ, message_seq, operate_type, event_type);
}

/**
 * @brief 处理好友验证事件 在好友验证事件下使用，无权限时不执行
 * @param thisQQ 框架QQ
 * @param triggerQQ 触发QQ
 * @param message_seq 消息Seq
 * @param operate_type 操作类型 1: 同意, 2: 拒绝
 */
void API::ProcessFriendVerificationEvent(int64_t thisQQ, int64_t triggerQQ, int64_t message_seq, int32_t operate_type)
{
    return _f<void(etext, elong, elong, elong, eint)>
            (this->j, "处理好友验证事件")
            (this->key, thisQQ, triggerQQ, message_seq, operate_type);
}


 /**
  * @brief 查看转发聊天记录内容 私聊消息也可以使用此命令解析，无权限时不执行
  * @param thisQQ 框架QQ
  * @param resID resID 可在xml消息代码中取到
  * @param message_content 消息内容 私聊消息也可从该结构取信息
  */
void API::ReadForwardedChatHistory(int64_t thisQQ, string resID, vector<GroupMessageData>& message_content)
{
    earray* info;
    _f<void(etext, elong, etext, earray**)>
                (this->j, "查看转发聊天记录内容")
                (this->key, thisQQ, s2e(resID), &info);
    earray1D::UnpackStruct<GroupMessageData>(info, message_content);
    delete info;
}

/**
 * @brief 上传群文件 本命令为耗时操作，请另开线程执行，本命令不支持上百mb的文件，无权限时不执行
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param path 文件路径 本地文件路径
 * @param folder 文件夹 该空保留，暂时无效
 */
void API::UploadGroupFile(int64_t thisQQ, int64_t groupQQ, string path, string folder)
{
    return _f<void(etext, elong, elong, etext, etext)>
            (this->j, "上传群文件")
            (this->key, thisQQ, groupQQ, s2e(path), s2e(folder));
}

/**
 * @brief 创建群文件夹
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param folder 文件夹名
 * @return 失败或无权限返回假
 */
bool API::CreateGroupFolder(int64_t thisQQ, int64_t groupQQ, string folder)
{
    return e2b(_f<ebool(etext, elong, elong, etext)>
                (this->j, "创建群文件夹")
                (this->key, thisQQ, groupQQ, s2e(folder)));
}

/**
* @brief 设置在线状态
* @param thisQQ 框架QQ
* @param main main 11: 在线, 31: 离开, 41: 隐身, 50: 忙碌, 60: Q我吧, 70: 请勿打扰
* @param sun sun 当main=11时，可进一步设置 0: 普通在线, 1000: 我的电量, 1011: 信号弱, 1016: 睡觉中, 1017: 游戏中, 1018: 学习中, 1019: 吃饭中, 1021: 煲剧中, 1022: 度假中, 1024: 在线学习, 1025: 在家旅游, 1027: TiMi中, 1028: 我在听歌, 1032: 熬夜中, 1050: 打球中, 1051: 恋爱中, 1052: 我没事
* @param battery 电量 sun=1000时，可以设置上报电量，取值1到100
* @return 失败或无权限返回假
*/
bool API::SetStatus(int64_t thisQQ, int32_t main, int32_t sun, int32_t battery)
{
    return e2b(_f<ebool(etext, elong, eint, eint, eint)>
                (this->j, "设置在线状态")
                (this->key, thisQQ, main, sun, battery));
}

/**
 * @brief api是否有权限
 * @param permission 权限 #权限_
 * @return 判断某api是否有权限
 */
bool API::CheckPermission(int32_t permission)
{
    return e2b(_f<ebool(etext, eint)>
                (this->j, "api是否有权限")
                (this->key, permission));
}

/**
 * @brief 重载自身 没有权限限制，请勿将新的插件文件下载至plugin文件夹，请确保新旧文件appname一致
 * @param new_file_path 新文件路径 若要更新插件，可将插件文件下载后在此填写新文件路径
*/
void API::ReloadPlugin(string new_file_path)
{
    return _f<void(etext, etext)>
            (this->j, "重载自身")
            (this->key, s2e(new_file_path));
}

/**
 * @brief 取插件数据目录 没有权限限制，建议将设置文件之类的都写这里面
 * @return 插件数据目录 结果结尾带\
 */
string API::GetPluginDataDirectory()
{
    return e2s(_f<etext(etext)>
                (this->j, "取插件数据目录")
                (this->key));
}

/**
 * @brief QQ点赞 注意，非好友情况下进行点赞时返回成功，但不一定真正点上了
 * @param thisQQ 框架QQ
 * @param otherQQ 对方QQ
 */
string API::QQLike(int64_t thisQQ, int64_t otherQQ)
{
    return e2s(_f<etext(etext, elong, elong)>
                (this->j, "QQ点赞")
                (this->key, thisQQ, otherQQ));
}

/**
 * @brief 取图片下载地址
 * @param image_code 图片代码 支持群聊、私聊的图片、闪照代码，注意是图片代码
 * @param thisQQ 框架QQ 群聊图片必填，私聊图片必空
 * @param groupQQ 群号 群聊图片必填，私聊图片必空
 */
string API::GetImageDownloadLink(string image_code, int64_t thisQQ, int64_t groupQQ)
{
    return e2s(_f<etext(etext, etext, elong, elong)>
                (this->j, "取图片下载地址")
                (this->key, s2e(image_code), thisQQ, groupQQ));
}

// FIXME: 需要给易语言API传入struct**然后返回struct
/**
 * @brief 查询好友信息
 * @param thisQQ 框架QQ
 * @param otherQQ 对方QQ
 * @param data 数据
 * @return 支持陌生人查询
 */
bool API::QueryFriendInformation(int64_t thisQQ, int64_t otherQQ, volatile FriendInformation* data)
{
    return e2b(_f<ebool(etext, elong, elong, volatile FriendInformation*)>
                (this->j, "查询好友信息")
                (this->key, thisQQ, otherQQ, data));
}

/**
 * @brief 查询群信息
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param data 数据
 */
bool API::QueryGroupInformation(int64_t thisQQ, int64_t groupQQ, volatile GroupCardInformation* data)
{
    return e2b(_f<ebool(etext, elong, elong, volatile GroupCardInformation*)>
                (this->j, "查询群信息")
                (this->key, thisQQ, groupQQ, data));
}

/**
 * @brief 框架重启
 * @return 有权限限制，建议使用前检查是否具有权限
 */
void API::Reboot()
{
    return _f<void(etext)>
            (this->j, "框架重启")
            (this->key);
}

/**
 * @brief 群文件转发至群
 * @param thisQQ 框架QQ
 * @param source_groupQQ 来源群号
 * @param target_groupQQ 目标群号
 * @param fileID FileId
 * @return 失败或无权限返回假
 */
bool API::GroupFileForwardToGroup(int64_t thisQQ, int64_t source_groupQQ, int64_t target_groupQQ, string fileID)
{
    return e2b(_f<ebool(etext, elong, elong, elong, etext)>
                (this->j, "群文件转发至群")
                (this->key, thisQQ, source_groupQQ, target_groupQQ, s2e(fileID)));
}

/**
 * @brief 群文件转发至好友
 * @param thisQQ 框架QQ
 * @param souce_groupQQ 来源群号
 * @param otherQQ 目标QQ
 * @param fileID FileId
 * @param file_name Filename
 * @param req Req 撤回消息用
 * @param random Random 撤回消息用
 * @param time time 撤回消息用
 * @return 失败或无权限返回假
 */
bool API::GroupFileForwardToFriend(int64_t thisQQ, int64_t souce_groupQQ, int64_t otherQQ, string fileID, string file_name,  int32_t *req, int64_t *random, int32_t *time)
{
    return e2b(_f<ebool(etext, elong, elong, elong, etext, etext, eint*, elong*, eint*)>
                (this->j, "群文件转发至好友")
                (this->key, thisQQ, souce_groupQQ, otherQQ, s2e(fileID), s2e(file_name), req, random, time));
}

/**
 * @brief 好友文件转发至好友
 * @param thisQQ 框架QQ
 * @param sourceQQ 来源QQ
 * @param targetQQ 目标QQ
 * @param fileID FileId
 * @param file_name Filename
 * @param req Req 撤回消息用
 * @param random Random 撤回消息用
 * @param time time 撤回消息用
 * @return 失败或无权限返回假
 */
bool API::FriendFileForwardToFriend(int64_t thisQQ, int64_t sourceQQ, int64_t targetQQ, string fileID, string file_name, int32_t *req, int32_t *random, int32_t *time)
{
    return e2b(_f<ebool(etext, elong, elong, elong, etext, etext, eint*, eint*, eint*)>
                (this->j, "好友文件转发至好友")
                (this->key, thisQQ, sourceQQ, targetQQ, s2e(fileID), s2e(file_name), req, random, time));
}

/**
 * @brief 设置群消息接收
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param set_type 设置类型 1接收并提醒 2收进群助手 3屏蔽群消息 4接收不提醒
 * @return 失败或无权限返回假，此API未对返回结果进行分析，返回真不一定成功
 */
bool API::SetGroupMessageReceive(int64_t thisQQ, int64_t groupQQ, int32_t set_type)
{
    return e2b(_f<ebool(etext, elong, elong, eint)>
                (this->j, "置群消息接收")
                (this->key, thisQQ, groupQQ, set_type));
}

/**
 * @brief 发送免费礼物
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param otherQQ 对方QQ
 * @param packageID packageID 299卡布奇诺;302猫咪手表;280牵你的手;281可爱猫咪;284神秘面具;285甜wink;286我超忙的;289快乐肥宅水;290幸运手链;313坚强;307绒绒手套; 312爱心口罩;308彩虹糖果
 * @return 绕过广告发送免费礼物
 */
string API::SendFreeGift(int64_t thisQQ, int64_t groupQQ, int64_t otherQQ, int32_t packageID)
{
    return e2s(_f<etext(etext, elong, elong, elong, eint)>
                (this->j, "发送免费礼物")
                (this->key, thisQQ, groupQQ, otherQQ, packageID));
}

/**
 * @brief 获取好友在线状态
 * @param thisQQ 框架QQ
 * @param otherQQ 对方QQ
 * @return 失败或无权限返回空，支持查询陌生人
 */
string API::GetFriendStatus(int64_t thisQQ, int64_t otherQQ)
{
    return e2s(_f<etext(etext, elong, elong)>
                (this->j, "取好友在线状态")
                (this->key, thisQQ, otherQQ));
}

/**
 * @brief 获取QQ钱包个人信息
 * @param thisQQ 框架QQ
 * @param data 数据 取银行卡信息时注意不要数组越界
 * @return 包括余额、名字、银行卡等
 */
string API::GetQQWalletPersonalInformation(int64_t thisQQ, volatile QQWalletInformation* data)
{
    return e2s(_f<etext(etext, elong, volatile QQWalletInformation*)>
                (this->j, "取QQ钱包个人信息")
                (this->key, thisQQ, data));
}


/**
 * @brief 获取订单详情
 * @param thisQQ 框架QQ
 * @param orderID 订单号 或者称之为listid
 * @param data 数据
 * @return 可以查订单，比如别人给你转账，你可以查询转账的详情
 */
string API::GetOrderDetail(int64_t thisQQ, string orderID, volatile OrderDetail* data)
{
    return e2s(_f<etext(etext, elong, etext, volatile OrderDetail* )>
                (this->j, "获取订单详情")
                (this->key, thisQQ, s2e(orderID), data));
}

/**
 * @brief 提交支付验证码
 * @param thisQQ 框架QQ
 * @param captcha_information 验证码信息 银行卡发红包时传回的
 * @param captcha 验证码 手机收到的短信验证码
 * @param payment_password 支付密码 用于验证并支付
 * @return 用银行卡支付时需要验证，只需要验证一次
 */
string API::SubmitPaymentCaptcha(int64_t thisQQ, volatile CaptchaInformation* captcha_information, string captcha, string payment_password)
{
    return e2s(_f<etext(etext, elong, volatile CaptchaInformation*, etext, etext)>
                (this->j, "提交支付验证码")
                (this->key, thisQQ, captcha_information, s2e(captcha), s2e(payment_password)));
}

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
 * @return 失败或无权限返回假
 */
bool API::ShareMusic(int64_t thisQQ, int64_t otherQQ, string music_name, string artist_name, string redirect_link, string cover_link, string file_path,  int32_t app_type, int32_t share_type)
{
    return e2b(_f<ebool(etext, elong, elong, etext, etext, etext, etext, etext, eint, eint)>
                    (this->j, "分享音乐")
                    (this->key, thisQQ, otherQQ, s2e(music_name), s2e(artist_name), s2e(redirect_link), s2e(cover_link), s2e(file_path), app_type, share_type));
}

/**
 * @brief 更改群聊消息内容
 * @param data_pointer 数据指针
 * @param new_message_content 新消息内容
 * @return 使用此命令可以更改当前群聊消息内容，并使更改后的内容投递给之后的插件，无权限返回假
 */
bool API::ModifyGroupMessageContent(int32_t data_pointer, string new_message_content)
{
    return e2b(_f<ebool(etext, eint, etext)>
                (this->j, "更改群聊消息内容")
                (this->key, data_pointer, s2e(new_message_content)));
}

/**
 * @brief 更改私聊消息内容
 * @param data_pointer 数据指针
 * @param new_message_content 新消息内容
 * @return 使用此命令可以更改当前私聊消息内容，并使更改后的内容投递给之后的插件，无权限返回假
 */
bool API::ModifyPrivateMessageContent(int32_t data_pointer, string new_message_content)
{
    return e2b(_f<ebool(etext, eint, etext)>
                (this->j, "更改私聊消息内容")
                (this->key, data_pointer, s2e(new_message_content)));
}

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
string API::GroupPasswordRedEnvelope(int64_t thisQQ, int32_t total_number, int32_t total_amount, int64_t groupQQ, string password, string payment_password,  int32_t card_serial)
{
    return e2s(_f<etext(etext, elong, eint, eint, elong, etext, etext, eint)>
                (this->j, "群聊口令红包")
                (this->key, thisQQ, total_number, total_amount, groupQQ, s2e(password), s2e(payment_password), card_serial));
}

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
string API::GroupRandomRedEnvelope(int64_t thisQQ, int32_t total_number, int32_t total_amount, int64_t groupQQ, string blessing, string payment_password, int32_t card_serial, int32_t skinID)
{
    return e2s(_f<etext(etext, elong, eint, eint, elong, etext, eint, etext, eint)>
                (this->j, "群聊拼手气红包")
                (this->key, thisQQ, total_number, total_amount, groupQQ, s2e(blessing), skinID, s2e(payment_password), card_serial));
}

/**
 * @brief 群聊普通红包
 * @param thisQQ 框架QQ
 * @param total_number 总数量
 * @param total_amount 总金额 单位分
 * @param groupQQ 群号
 * @param blessing 祝福语
 * @param payment_password 支付密码
 * @param card_serial 银行卡序列 大于0时使用银行卡支付
 * @param skinID 红包皮肤Id 1522光与夜之恋,1527代号:三国(打了一辈子仗),1525街霸:对决,1518代号:三国(俺送红包来了),1476天涯明月刀,1512一人之下。其他皮肤id自己找
 */
string API::GroupNormalRedEnvelope(int64_t thisQQ, int32_t total_number, int32_t total_amount, int64_t groupQQ, string blessing, string payment_password, int32_t card_serial, int32_t skinID)
{
    return e2s(_f<etext(etext, elong, eint, eint, elong, etext, eint, etext, eint)>
                (this->j, "群聊普通红包")
                (this->key, thisQQ, total_number, total_amount, groupQQ, s2e(blessing), skinID, s2e(payment_password), card_serial));
}

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
string API::GroupDrawRedEnvelope(int64_t thisQQ, int32_t total_number, int32_t total_amount, int64_t groupQQ, string question, string payment_password, int32_t card_serial)
{
    return e2s(_f<etext(etext, elong, eint, eint, elong, etext, etext, eint)>
                (this->j, "群聊画图红包")
                (this->key, thisQQ, total_number, total_amount, groupQQ, s2e(question), s2e(payment_password), card_serial));
}

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
string API::GroupAudioRedEnvelope(int64_t thisQQ, int32_t total_number, int32_t total_amount, int64_t groupQQ, string audio_password, string payment_password,  int32_t card_serial)
{
    return e2s(_f<etext(etext, elong, eint, eint, elong, etext, etext, eint)>
                (this->j, "群聊语音红包")
                (this->key, thisQQ, total_number, total_amount, groupQQ, s2e(audio_password), s2e(payment_password), card_serial));
}

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
string API::GroupFollowRedEnvelope(int64_t thisQQ, int32_t total_number, int32_t total_amount, int64_t groupQQ, string follow_content, string payment_password,  int32_t card_serial)
{
    return e2s(_f<etext(etext, elong, eint, eint, elong, etext, etext, eint)>
                (this->j, "群聊接龙红包")
                (this->key, thisQQ, total_number, total_amount, groupQQ, s2e(follow_content), s2e(payment_password), card_serial));
}

/**
 * @brief 群聊专属红包
 * @param thisQQ 框架QQ
 * @param total_number 总数量
 * @param total_amount 总金额 单位分
 * @param groupQQ 群号
 * @param otherQQ 领取人 多个领取人QQ用|分隔
 * @param blessing 祝福语
 * @param is_divided 是否均分 默认不均分(拼手气)
 * @param payment_password 支付密码
 * @param card_serial 银行卡序列 大于0时使用银行卡支付
 */
string API::GroupExclusiveRedEnvelope(int64_t thisQQ, int32_t total_number, int32_t total_amount, int64_t groupQQ, string otherQQ, string blessing,  bool is_divided, string payment_password,  int32_t card_serial)
{
    return  e2s(_f<etext(etext, elong, eint, eint, elong, etext, etext, ebool, etext, eint)>
                (this->j, "群聊专属红包")
                (this->key, thisQQ, total_number, total_amount, groupQQ, s2e(otherQQ), s2e(blessing), b2e(is_divided), s2e(payment_password), card_serial));
}

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
string API::FriendPasswordRedEnvelope(int64_t thisQQ, int32_t total_number, int32_t total_amount, int64_t otherQQ, string password, string payment_password,  int32_t card_serial)
{
    return e2s(_f<etext(etext, elong, eint, eint, elong, etext, etext, eint)>
                (this->j, "好友口令红包")
                (this->key, thisQQ, total_number, total_amount, otherQQ, s2e(password), s2e(payment_password), card_serial));
}
/**
 * @brief 好友普通红包
 * @param thisQQ 框架QQ
 * @param total_number 总数量
 * @param total_amount 总金额 单位分
 * @param otherQQ 对方QQ
 * @param blessing 祝福语
 * @param payment_password 支付密码
 * @param card_serial 银行卡序列 大于0时使用银行卡支付
 * @param skinID 红包皮肤Id 1522光与夜之恋
 * @return 不支持非好友！
 */
string API::FriendNormalRedEnvelope(int64_t thisQQ, int32_t total_number, int32_t total_amount, int64_t otherQQ, string blessing, string payment_password, int32_t card_serial, int32_t skinID)
{
    return e2s(_f<etext(etext, elong, eint, eint, elong, etext, eint, etext, eint)>
                (this->j, "好友普通红包")
                (this->key, thisQQ, total_number, total_amount, otherQQ, s2e(blessing), skinID, s2e(payment_password), card_serial));
}

/**
 * @brief 好友画图红包
 * @param thisQQ 框架QQ
 * @param total_number 总数量
 * @param total_amount 总金额 单位分
 * @param otherQQ 对方QQ
 * @param question 题目名 只能填手Q有的，如：庄周
 * @param payment_password 支付密码
 * @param card_serial 银行卡序列 大于0时使用银行卡支付
 * @return 不支持非好友！
 */
string API::FriendDrawRedEnvelope(int64_t thisQQ, int32_t total_number, int32_t total_amount, int64_t otherQQ, string question, string payment_password, int32_t card_serial)
{
    return e2s(_f<etext(etext, elong, eint, eint, elong, etext, etext, eint)>
                (this->j, "好友画图红包")
                (this->key, thisQQ, total_number, total_amount, otherQQ, s2e(question), s2e(payment_password), card_serial));
}

/**
 * @brief 好友语音红包
 * @param thisQQ 框架QQ
 * @param total_number 总数量
 * @param total_amount 总金额 单位分
 * @param otherQQ 对方QQ
 * @param audio_password 语音口令
 * @param payment_password 支付密码
 * @param card_serial 银行卡序列 大于0时使用银行卡支付
 * @return 不支持非好友！
 */
string API::FriendAudioRedEnvelope(int64_t thisQQ, int32_t total_number, int32_t total_amount, int64_t otherQQ, string audio_password, string payment_password, int32_t card_serial)
{
    return e2s(_f<etext(etext, elong, eint, eint, elong, etext, etext, eint)>
                (this->j, "好友语音红包")
                (this->key, thisQQ, total_number, total_amount, otherQQ, s2e(audio_password), s2e(payment_password), card_serial));
}

/**
 * @brief 好友接龙红包
 * @param thisQQ 框架QQ
 * @param total_number 总数量
 * @param total_amount 总金额 单位分
 * @param otherQQ 对方QQ
 * @param follow_content 接龙内容
 * @param payment_password 支付密码
 * @param card_serial 银行卡序列 大于0时使用银行卡支付
 * @return 不支持非好友！
 */
string API::FriendFollowRedEnvelope(int64_t thisQQ, int32_t total_number, int32_t total_amount, int64_t otherQQ, string follow_content, string payment_password, int32_t card_serial)
{
    return e2s(_f<etext(etext, elong, eint, eint, elong, etext, etext, eint)>
                (this->j, "好友接龙红包")
                (this->key, thisQQ, total_number, total_amount, otherQQ, s2e(follow_content), s2e(payment_password), card_serial));
}

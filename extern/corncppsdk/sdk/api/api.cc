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

#include "api.h"


/**
 * @brief 载入插件关键字和系统API函数指针
 * @param api_data 系统API函数指针JSON
 * @param plugin_key 插件标识符
 */
API::API(etext api_data, etext plugin_key)
    : j(Json::parse(GBKtoUTF8(api_data))), key(plugin_key)
{
}

API::~API()
{
}

/**
 * @brief 输出日志
 * @param message 日志
 * @param text_color 文字颜色
 * @param background_color 背景颜色
 */
std::string API::OutputLog(const std::string &message, std::int32_t text_color, std::int32_t background_color)
{
    return e2s(_f<etext(etext, etext, eint, eint)>(this->j, "输出日志")(this->key.c_str(), s2e(message), text_color, background_color));
}

/**
 * @brief 发送好友消息
 * @param thisQQ 框架QQ
 * @param friendQQ 好友QQ
 * @param content 发送内容
 * @param random 撤回消息用（传出）
 * @param req 撤回消息用（传出）
 * @return 成功返回的time用于撤回消息
 */
std::string API::SendFriendMessage(std::int64_t thisQQ, std::int64_t friendQQ, const std::string &content, std::int64_t &random, std::int32_t &req)
{
    elong *random_p;
    eint *req_p;
    auto ret = e2s_s(_f<etext(etext, elong, elong, etext, elong **, eint **)>(this->j, "发送好友消息")(this->key.c_str(), thisQQ, friendQQ, s2e(content), &random_p, &req_p));
    random = *random_p;
    req = *req_p;
    return ret;
}

/**
 * @brief 发送好友消息（若要撤回消息请使用另一重载）
 * @param thisQQ 框架QQ
 * @param friendQQ 好友QQ
 * @param content 发送内容
 * @return 成功返回的time用于撤回消息
 */
std::string API::SendFriendMessage(std::int64_t thisQQ, std::int64_t friendQQ, const std::string &content)
{
    elong *random_p;
    eint *req_p;
    return e2s_s(_f<etext(etext, elong, elong, etext, elong **, eint **)>(this->j, "发送好友消息")(this->key.c_str(), thisQQ, friendQQ, s2e(content), &random_p, &req_p));
}

/**
 * @brief 发送群消息
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param content 发送内容
 * @param anonymous 匿名发送
 */
std::string API::SendGroupMessage(std::int64_t thisQQ, std::int64_t groupQQ, const std::string &content, bool anonymous)
{
    return e2s_s(_f<etext(etext, elong, elong, etext, ebool)>(this->j, "发送群消息")(this->key.c_str(), thisQQ, groupQQ, s2e(content), b2e(anonymous)));
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
std::string API::SendGroupTemporaryMessage(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t otherQQ, const std::string &content, std::int64_t &random, std::int32_t &req)
{
    elong *random_p;
    eint *req_p;
    auto ret = e2s_s(_f<etext(etext, elong, elong, elong, etext, elong **, eint **)>(this->j, "发送群临时消息")(this->key.c_str(), thisQQ, groupQQ, otherQQ, s2e(content.c_str()), &random_p, &req_p));
    random = *random_p;
    req = *req_p;
    return ret;
}

/**
 * @brief 发送群临时消息（若要撤回消息请使用另一重载）
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param otherQQ 对方QQ
 * @param content 发送内容
 * @return 成功返回的time用于撤回消息
 */
std::string API::SendGroupTemporaryMessage(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t otherQQ, const std::string &content)
{
    elong *random_p;
    eint *req_p;
    return e2s_s(_f<etext(etext, elong, elong, elong, etext, elong **, eint **)>(this->j, "发送群临时消息")(this->key.c_str(), thisQQ, groupQQ, otherQQ, s2e(content.c_str()), &random_p, &req_p));
}

/**
 * @brief 添加好友
 * @param thisQQ 框架QQ
 * @param otherQQ 对方QQ
 * @param verification 设置回答问题答案或是验证消息，多个问题答案用"|"分隔开
 */
std::string API::AddFriend(std::int64_t thisQQ, std::int64_t otherQQ, const std::string &verification)
{
    return e2s_s(_f<etext(etext, elong, elong, etext)>(this->j, "添加好友")(this->key.c_str(), thisQQ, otherQQ, s2e(verification.c_str())));
}

/**
 * @brief 添加群（你在群内或需要付费入群也会直接发送验证消息）
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param verification 回答问题答案或是验证消息
 */
std::string API::AddGroup(std::int64_t thisQQ, std::int64_t groupQQ, const std::string &verification)
{
    return e2s_s(_f<etext(etext, elong, elong, etext)>(this->j, "添加群")(this->key.c_str(), thisQQ, groupQQ, s2e(verification.c_str())));
}

/**
 * @brief 删除好友
 * @param thisQQ 框架QQ
 * @param otherQQ 对方QQ
 */
std::string API::RemoveFriend(std::int64_t thisQQ, std::int64_t otherQQ)
{
    return e2s_s(_f<etext(etext, elong, elong)>(this->j, "删除好友")(this->key.c_str(), thisQQ, otherQQ));
}

/**
 * @brief 设置屏蔽好友
 * @param thisQQ 框架QQ
 * @param otherQQ 对方QQ
 * @param is_blocked 是否屏蔽
 */
std::string API::SetBlockFriend(std::int64_t thisQQ, std::int64_t otherQQ, bool is_blocked)
{
    return e2s_s(_f<etext(etext, elong, elong, ebool)>(this->j, "置屏蔽好友")(this->key.c_str(), thisQQ, otherQQ, b2e(is_blocked)));
}

/**
 * @brief 设置特别关心好友
 * @param thisQQ 框架QQ
 * @param otherQQ 对方QQ
 * @param is_special 是否屏蔽
 */
std::string API::SetSpecialFriend(std::int64_t thisQQ, std::int64_t otherQQ, bool is_special)
{
    return e2s_s(_f<etext(etext, elong, elong, ebool)>(this->j, "置特别关心好友")(this->key.c_str(), thisQQ, otherQQ, b2e(is_special)));
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
std::string API::SendFriendJSONMessage(std::int64_t thisQQ, std::int64_t friendQQ, const std::string &json_content, std::int64_t &random, std::int32_t &req)
{
    elong *random_p;
    eint *req_p;
    auto ret = e2s_s(_f<etext(etext, elong, elong, etext, elong **, eint **)>(this->j, "发送好友json消息")(this->key.c_str(), thisQQ, friendQQ, s2e(json_content), &random_p, &req_p));
    random = *random_p;
    req = *req_p;
    return ret;
}

/**
 * @brief 发送好友JSON消息（若要撤回消息请使用另一重载）
 * @param thisQQ 框架QQ
 * @param friendQQ 好友QQ
 * @param json_content json发送内容
 * @return 成功返回的time用于撤回消息
 */
std::string API::SendFriendJSONMessage(std::int64_t thisQQ, std::int64_t friendQQ, const std::string &json_content)
{
    elong *random_p;
    eint *req_p;
    return e2s_s(_f<etext(etext, elong, elong, etext, elong **, eint **)>(this->j, "发送好友json消息")(this->key.c_str(), thisQQ, friendQQ, s2e(json_content), &random_p, &req_p));
}

/**
 * @brief 发送群JSON消息
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param json_content json发送内容
 * @param anonymous 匿名发送
 */
std::string API::SendGroupJSONMessage(std::int64_t thisQQ, std::int64_t groupQQ, const std::string &json_content, bool anonymous)
{
    return e2s_s(_f<etext(etext, elong, elong, etext, bool)>(this->j, "发送群json消息")(this->key.c_str(), thisQQ, groupQQ, s2e(json_content), anonymous));
}

/**
 * @brief 上传好友图片
 * @param thisQQ 框架QQ
 * @param friendQQ 好友QQ
 * @param picture 图片数据
 * @param is_flash 是否闪照
 * @return 成功返回图片代码
 */
std::string API::UploadFriendImage(std::int64_t thisQQ, std::int64_t friendQQ, const std::uint8_t *picture, size_t length, bool is_flash)
{
    return e2s(_f<etext(etext, elong, elong, ebool, ebin, eint)>(this->j, "上传好友图片")(this->key.c_str(), thisQQ, friendQQ, b2e(is_flash), picture, static_cast<eint>(length)));
}

/**
 * @brief 上传群图片
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param picture 图片数据
 * @param is_flash 是否闪照
 * @return 成功返回图片代码
 */
std::string API::UploadGroupImage(std::int64_t thisQQ, std::int64_t groupQQ, const std::uint8_t *picture, size_t size, bool is_flash)
{
    return e2s(_f<etext(etext, elong, elong, ebool, ebin, eint)>(this->j, "上传群图片")(this->key.c_str(), thisQQ, groupQQ, b2e(is_flash), picture, static_cast<eint>(size)));
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
std::string API::UploadFriendAudio(std::int64_t thisQQ, std::int64_t friendQQ, const std::uint8_t *audio, size_t size, std::int32_t audio_type, const std::string &audio_text)
{
    return e2s(_f<etext(etext, elong, elong, eint, etext, ebin, eint)>(this->j, "上传好友语音")(this->key.c_str(), thisQQ, friendQQ, audio_type, s2e(audio_text), audio, static_cast<eint>(size)));
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
std::string API::UploadGroupAudio(std::int64_t thisQQ, std::int64_t groupQQ, const std::uint8_t *audio, size_t size, std::int32_t audio_type, const std::string &audio_text)
{
    return e2s(_f<etext(etext, elong, elong, eint, etext, ebin, eint)>(this->j, "上传群语音")(this->key.c_str(), thisQQ, groupQQ, audio_type, s2e(audio_text), audio, static_cast<eint>(size)));
}

/**
 * @brief 上传头像
 * @param thisQQ 框架QQ
 * @param picture 图片数据
 * @param size 图片大小
 */
std::string API::UploadAvatar(std::int64_t thisQQ, const std::uint8_t *picture, size_t size)
{
    return e2s(_f<etext(etext, elong, ebin, eint)>(this->j, "上传头像")(this->key.c_str(), thisQQ, picture, static_cast<eint>(size)));
}

/**
 * @brief silk解码 无权限要求 尚未实现！
 * @param audio_file_path 音频文件路径 注意文件后缀必须和文件格式相对应
 */
const std::uint8_t *API::SilkDecode(std::string audio_file_path)
{
    return nullptr;
}

/**
 * @brief silk编码 无权限要求 尚未实现！
 * @param audio_file_path 音频文件路径 注意文件后缀必须和文件格式相对应
 */
const std::uint8_t *API::SilkEncode(std::string audio_file_path)
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
std::string API::SetGroupNickname(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t otherQQ, const std::string &nickname)
{
    return e2s(_f<etext(etext, elong, elong, elong, etext)>(this->j, "设置群名片")(this->key.c_str(), thisQQ, groupQQ, otherQQ, s2e(nickname)));
}

/**
 * @brief 从缓存获取昵称
 * @param otherQQ 对方QQ
 * @return 成功返回昵称
 */
std::string API::GetNameFromCache(std::int64_t otherQQ)
{
    return e2s(_f<etext(etext, elong)>(this->j, "取昵称_从缓存")(this->key.c_str(), otherQQ));
}

/**
 * @brief 从缓存获取昵称
 * @param otherQQ 对方QQ
 * @return 成功返回昵称
 */
std::string API::GetNameForce(std::int64_t otherQQ)
{
    return e2s(_f<etext(etext, elong)>(this->j, "强制取昵称")(this->key.c_str(), otherQQ));
}

/**
 * @brief 从缓存获取群名（如果是框架QQ没加的群，[查询群信息]后也会记录缓存）
 * @param groupQQ 群号
 * @return 成功返回群名称
 */
std::string API::GetGroupNameFromCache(std::int64_t groupQQ)
{
    return e2s(_f<etext(etext, elong)>(this->j, "取群名称_从缓存")(this->key.c_str(), groupQQ));
}

/**
     * @brief 获取SKey
     * @param thisQQ 框架QQ
     * @return 成功返回SKey
     */
std::string API::GetSKey(std::int64_t thisQQ)
{
    return e2s(_f<etext(etext, elong)>(this->j, "获取skey")(this->key.c_str(), thisQQ));
}

/**
     * @brief 获取PSKey
     * @param thisQQ 框架QQ
     * @param domain 域: tenpay.com;openmobile.qq.com;docs.qq.com;connect.qq.com;qzone.qq.com;vip.qq.com;gamecenter.qq.com;qun.qq.com;game.qq.com;qqweb.qq.com;ti.qq.com;office.qq.com;mail.qq.com;mma.qq.com
     * @return 成功返回PSKey
     */
std::string API::GetPSKey(std::int64_t thisQQ, const std::string &domain)
{
    return e2s(_f<etext(etext, elong)>(this->j, "获取pskey")(this->key.c_str(), thisQQ));
}

/**
 * @brief 获取ClientKey
 * @param thisQQ 框架QQ
 * @return 成功返回ClientKey
 */
std::string API::GetClientKey(std::int64_t thisQQ)
{
    return e2s(_f<etext(etext, elong)>(this->j, "获取clientkey")(this->key.c_str(), thisQQ));
}

/**
 * @brief 获取框架QQ
 */
std::string API::GetThisQQ()
{
    return e2s(_f<etext(etext)>(this->j, "取框架QQ")(this->key.c_str()));
}

// FIXME: 易语言数组有未知bug
/**
 * @brief 获取好友列表
 * @param thisQQ 框架QQ
 * @param friend_list 好友信息列表
 * @return 成功返回好友数量，失败或无权限返回0
 */
size_t API::GetFriendList(std::int64_t thisQQ, std::vector<FriendInformation> &friend_list)
{
    earray1D<FriendInformation> array;
    eint size = _f<eint(etext, elong, void **)>(this->j, "取好友列表")(this->key.c_str(), thisQQ, &array.data);
    return size == 0 ? 0 : array.Unpack(friend_list);
}

/**
 * @brief 获取群列表
 * @param thisQQ 框架QQ
 * @param group_list 群信息列表
 * @return 成功返回群数量，失败或无权限返回0
 */
size_t API::GetGroupList(std::int64_t thisQQ, std::vector<GroupInformation> &group_list)
{
    earray1D<GroupInformation> array;
    eint size = _f<eint(etext, elong, void **)>(this->j, "取群列表")(this->key.c_str(), thisQQ, &array.data);
    return size == 0 ? 0 : array.Unpack(group_list);
}

/**
 * @brief 获取群成员列表
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param group_member_list 群成员信息列表
 * @return 失败或无权限返回数量0
 */
int32_t API::GetGroupMemberList(std::int64_t thisQQ, std::int64_t groupQQ, std::vector<GroupMemberInformation> &group_member_list)
{
    earray1D<GroupMemberInformation> array;
    eint size = _f<eint(etext, elong, elong, void **)>(this->j, "取群成员列表")(this->key.c_str(), thisQQ, groupQQ, &array.data);
    return size == 0 ? 0 : array.Unpack(group_member_list);
}

/**
 * @brief 设置管理员
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param otherQQ 群成员QQ
 * @param is_administrator 取消或设置管理
 * @return 失败或无权限返回false
 */
bool API::SetAdministrator(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t otherQQ, bool is_administrator)
{
    return e2b(_f<ebool(etext, elong, elong, elong, ebool)>(this->j, "设置管理员")(this->key.c_str(), thisQQ, groupQQ, otherQQ, b2e(is_administrator)));
}

/**
 * @brief 获取管理层列表
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @return 包括群主
 */
std::string API::GetAdministratorList(std::int64_t thisQQ, std::int64_t groupQQ)
{
    return e2s(_f<etext(etext, elong, elong)>(this->j, "取管理层列表")(this->key.c_str(), thisQQ, groupQQ));
}

/**
 * @brief 获取群名片
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param otherQQ 群成员QQ
 * @return 成功返回群名片，注意，如果群成员的群名片未设置或群名片为空白，返回结果均为空
 */
std::string API::GetGroupNickname(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t otherQQ)
{
    return e2s(_f<etext(etext, elong, elong, elong)>(this->j, "取群名片")(this->key.c_str(), thisQQ, groupQQ, otherQQ));
}

/**
 * @brief 获取个性签名
 * @param thisQQ 框架QQ
 * @param otherQQ 对方QQ 可以填框架QQ本身
 * @return 成功返回个性签名
 */
std::string API::GetSignature(std::int64_t thisQQ, std::int64_t otherQQ)
{
    return e2s(_f<etext(etext, elong, elong)>(this->j, "取个性签名")(this->key.c_str(), thisQQ, otherQQ));
}

/**
 * @brief 修改昵称
 * @param thisQQ 框架QQ
 * @param name 昵称
 * @return 失败或无权限返回false
 */
bool API::SetName(std::int64_t thisQQ, std::string name)
{
    return e2b(_f<ebool(etext, elong, etext)>(this->j, "修改昵称")(this->key.c_str(), thisQQ, s2e(name)));
}

/**
 * @brief 修改个性签名
 * @param thisQQ 框架QQ
 * @param signature 签名
 * @return 失败或无权限返回false
 */
bool API::SetSignature(std::int64_t thisQQ, std::string signature)
{
    return e2b(_f<ebool(etext, elong, etext)>(this->j, "修改个性签名")(this->key.c_str(), thisQQ, s2e(signature)));
}

/**
 * @brief 删除群成员
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param otherQQ 群成员QQ
 * @param is_verification_refused 拒绝加群申请
 * @return 失败或无权限返回false
 */
bool API::RemoveGroupMember(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t otherQQ, bool is_verification_refused)
{
    return e2b(_f<ebool(etext, elong, elong, elong, ebool)>(this->j, "删除群成员")(this->key.c_str(), thisQQ, groupQQ, otherQQ, b2e(is_verification_refused)));
}

/**
 * @brief 禁言群成员
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param otherQQ 群成员QQ
 * @param time 禁言时长 单位：秒
 * @return 失败或无权限返回false
 */
bool API::ShutUpGroupMember(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t otherQQ, std::int32_t time)
{
    return e2b(_f<ebool(etext, elong, elong, elong, eint)>(this->j, "禁言群成员")(this->key.c_str(), thisQQ, groupQQ, otherQQ, time));
}

/**
 * @brief 退群
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @return 失败或无权限返回false
 */
bool API::QuitGroup(std::int64_t thisQQ, std::int64_t groupQQ)
{
    return e2b(_f<ebool(etext, elong, elong)>(this->j, "退群")(this->key.c_str(), thisQQ, groupQQ));
}

/**
 * @brief 解散群
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @return 失败或无权限返回false
 */
bool API::DissolveGroup(std::int64_t thisQQ, std::int64_t groupQQ)
{
    return e2b(_f<ebool(etext, elong, elong)>(this->j, "解散群")(this->key.c_str(), thisQQ, groupQQ));
}

/**
 * @brief 上传群头像
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param picture pic
 * @return 失败或无权限返回false
 */
bool API::UploadGroupAvatar(std::int64_t thisQQ, std::int64_t groupQQ, const std::uint8_t *picture)
{
    return e2b(_f<ebool(etext, elong, elong, ebin)>(this->j, "上传群头像")(this->key.c_str(), thisQQ, groupQQ, picture));
}

/**
 * @brief 全员禁言
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param is_shut_up_all 是否开启
 * @return 失败或无权限返回false
 */
bool API::ShutUpAll(std::int64_t thisQQ, std::int64_t groupQQ, bool is_shut_up_all)
{
    return e2b(_f<ebool(etext, elong, elong, ebool)>(this->j, "全员禁言")(this->key.c_str(), thisQQ, groupQQ, is_shut_up_all));
}

/**
 * @brief 群权限_发起新的群聊
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param is_allowed 是否允许
 * @return 失败或无权限返回false
 */
bool API::GroupPermission_CreateGroup(std::int64_t thisQQ, std::int64_t groupQQ, bool is_allowed)
{
    return e2b(_f<ebool(etext, elong, elong, ebool)>(this->j, "群权限_发起新的群聊")(this->key.c_str(), thisQQ, groupQQ, b2e(is_allowed)));
}

/**
 * @brief 群权限_发起临时会话
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param is_allowed 是否允许
 * @return 失败或无权限返回false
 */
bool API::GroupPermission_CreateTemporaryConversation(std::int64_t thisQQ, std::int64_t groupQQ, bool is_allowed)
{
    return e2b(_f<ebool(etext, elong, elong, ebool)>(this->j, "群权限_发起临时会话")(this->key.c_str(), thisQQ, groupQQ, b2e(is_allowed)));
}

/**
 * @brief 群权限_上传文件
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param is_allowed 是否允许
 * @return 失败或无权限返回false
 */
bool API::GroupPermission_UploadFile(std::int64_t thisQQ, std::int64_t groupQQ, bool is_allowed)
{
    return e2b(_f<ebool(etext, elong, elong, ebool)>(this->j, "群权限_上传文件")(this->key.c_str(), thisQQ, groupQQ, b2e(is_allowed)));
}

/**
 * @brief 群权限_上传相册
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param is_allowed 是否允许
 * @return 失败或无权限返回false
 */
bool API::GroupPermission_UploadPicture(std::int64_t thisQQ, std::int64_t groupQQ, bool is_allowed)
{
    return e2b(_f<ebool(etext, elong, elong, ebool)>(this->j, "群权限_上传相册")(this->key.c_str(), thisQQ, groupQQ, b2e(is_allowed)));
}

/**
 * @brief 群权限_邀请好友加群
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param is_allowed 是否允许
 * @return 失败或无权限返回false
 */
bool API::GroupPermission_InviteFriend(std::int64_t thisQQ, std::int64_t groupQQ, bool is_allowed)
{
    return e2b(_f<ebool(etext, elong, elong, ebool)>(this->j, "群权限_邀请好友加群")(this->key.c_str(), thisQQ, groupQQ, b2e(is_allowed)));
}

/**
 * @brief 群权限_匿名聊天
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param is_allowed 是否允许
 * @return 失败或无权限返回false
 */
bool API::GroupPermission_Anonymous(std::int64_t thisQQ, std::int64_t groupQQ, bool is_allowed)
{

    return e2b(_f<ebool(etext, elong, elong, ebool)>(this->j, "群权限_匿名聊天")(this->key.c_str(), thisQQ, groupQQ, b2e(is_allowed)));
}

/**
 * @brief 群权限_坦白说
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param is_allowed 是否允许
 * @return 失败或无权限返回false
 */
bool API::GroupPermission_ChatFrankly(std::int64_t thisQQ, std::int64_t groupQQ, bool is_allowed)
{
    return e2b(_f<ebool(etext, elong, elong, ebool)>(this->j, "群权限_坦白说")(this->key.c_str(), thisQQ, groupQQ, b2e(is_allowed)));
}

/**
 * @brief 群权限_新成员查看历史消息
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param is_allowed 是否允许
 * @return 失败或无权限返回false
 */
bool API::GroupPermission_NewMemberReadChatHistory(std::int64_t thisQQ, std::int64_t groupQQ, bool is_allowed)
{
    return e2b(_f<ebool(etext, elong, elong, ebool)>(this->j, "群权限_新成员查看历史消息")(this->key.c_str(), thisQQ, groupQQ, b2e(is_allowed)));
}

/**
 * @brief 群权限_邀请方式设置
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param method 方式 1 无需审核;2 需要管理员审核;3 100人以内无需审核
 * @return 失败或无权限返回false
 */
bool API::GroupPermission_SetInviteMethod(std::int64_t thisQQ, std::int64_t groupQQ, std::int32_t method)
{
    return e2b(_f<ebool(etext, elong, elong, eint)>(this->j, "群权限_邀请方式设置")(this->key.c_str(), thisQQ, groupQQ, method));
}

/**
 * @brief 撤回消息_群聊 在群消息事件中使用，能收到并撤回自己发的消息
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param message_random 消息Random
 * @param message_req 消息Req
 * @return 失败或无权限返回false
 */
bool API::Undo_Group(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t message_random, std::int32_t message_req)
{
    return e2b(_f<ebool(etext, elong, elong, elong, eint)>(this->j, "撤回消息_群聊")(this->key.c_str(), thisQQ, groupQQ, message_random, message_req));
}

/**
 * @brief 撤回消息_私聊本身 用于撤回自己发的消息，其他设备的个人消息通知也可以撤回
 * @param thisQQ 框架QQ
 * @param otherQQ 对方QQ
 * @param message_random 消息Random
 * @param message_req 消息Req
 * @param time 消息接收时间
 * @return 失败或无权限返回false
 */
bool API::Undo_Private(std::int64_t thisQQ, std::int64_t otherQQ, std::int64_t message_random, std::int32_t message_req, std::int32_t time)
{
    return e2b(_f<ebool(etext, elong, elong, elong, eint, eint)>(this->j, "撤回消息_私聊本身")(this->key.c_str(), thisQQ, otherQQ, message_random, message_req, time));
}

/**
 * @brief 设置位置共享（开启后需要上报位置，大约3s上报一次，否则会自动关闭）
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param Longitude 经度 如：121.711540
 * @param Latitude 纬度 如：31.403343
 * @param is_enabled 是否开启
 * @return 失败或无权限返回false
 */
bool API::SetLocationShare(std::int64_t thisQQ, std::int64_t groupQQ, double Longitude, double Latitude, bool is_enabled)
{
    return e2b(_f<ebool(etext, elong, elong, edouble, edouble, ebool)>(this->j, "设置位置共享")(this->key.c_str(), thisQQ, groupQQ, Longitude, Latitude, b2e(is_enabled)));
}

/**
 * @brief 上报当前位置（大约3s上报一次，不得过快）
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param Longitude 经度 如：121.711540
 * @param Latitude 纬度 如：31.403343
 * @return 失败或无权限返回false
 */
bool API::ReportCurrent(std::int64_t thisQQ, std::int64_t groupQQ, double Longitude, double Latitude)
{
    return e2b(_f<ebool(etext, elong, elong, edouble, edouble)>(this->j, "上报当前位置")(this->key.c_str(), thisQQ, groupQQ, Longitude, Latitude));
}

/**
 * @brief 是否被禁言
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @return 返回禁言时长，单位秒，失败、无权限或未被禁言返回0
 */
int64_t API::IsShuttedUp(std::int64_t thisQQ, std::int64_t groupQQ)
{
    return _f<elong(etext, elong, elong)>(this->j, "是否被禁言")(this->key.c_str(), thisQQ, groupQQ);
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
void API::ProcessGroupVerificationEvent(std::int64_t thisQQ, std::int64_t source_groupQQ, std::int64_t triggerQQ, std::int64_t message_seq, std::int32_t operate_type, std::int32_t event_type)
{
    return _f<void(etext, elong, elong, elong, elong, eint, eint)>(this->j, "处理群验证事件")(this->key.c_str(), thisQQ, source_groupQQ, triggerQQ, message_seq, operate_type, event_type);
}

/**
 * @brief 处理好友验证事件 在好友验证事件下使用，无权限时不执行
 * @param thisQQ 框架QQ
 * @param triggerQQ 触发QQ
 * @param message_seq 消息Seq
 * @param operate_type 操作类型 1: 同意, 2: 拒绝
 */
void API::ProcessFriendVerificationEvent(std::int64_t thisQQ, std::int64_t triggerQQ, std::int64_t message_seq, std::int32_t operate_type)
{
    return _f<void(etext, elong, elong, elong, eint)>(this->j, "处理好友验证事件")(this->key.c_str(), thisQQ, triggerQQ, message_seq, operate_type);
}

/**
  * @brief 查看转发聊天记录内容 私聊消息也可以使用此命令解析，无权限时不执行
  * @param thisQQ 框架QQ
  * @param resID resID 可在xml消息代码中获取到
  * @param message_content 消息内容 私聊消息也可从该结构获取信息
  */
void API::ReadForwardedChatHistory(std::int64_t thisQQ, std::string resID, std::vector<GroupMessageData> &message_content)
{
    earray1D<GroupMessageData> array;
    _f<void(etext, elong, etext, void **)>(this->j, "查看转发聊天记录内容")(this->key.c_str(), thisQQ, s2e(resID), &array.data);
    array.Unpack(message_content);
}

/**
 * @brief 上传群文件 本命令为耗时操作，请另开线程执行，本命令不支持上百mb的文件，无权限时不执行
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param path 文件路径 本地文件路径
 * @param folder 文件夹 该空保留，暂时无效
 */
void API::UploadGroupFile(std::int64_t thisQQ, std::int64_t groupQQ, std::string path, std::string folder)
{
    return _f<void(etext, elong, elong, etext, etext)>(this->j, "上传群文件")(this->key.c_str(), thisQQ, groupQQ, s2e(path), s2e(folder));
}

/**
 * @brief 创建群文件夹
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param folder 文件夹名
 * @return 失败或无权限返回false
 */
bool API::CreateGroupFolder(std::int64_t thisQQ, std::int64_t groupQQ, std::string folder)
{
    return e2b(_f<ebool(etext, elong, elong, etext)>(this->j, "创建群文件夹")(this->key.c_str(), thisQQ, groupQQ, s2e(folder)));
}

/**
* @brief 设置在线状态
* @param thisQQ 框架QQ
* @param main main 11: 在线, 31: 离开, 41: 隐身, 50: 忙碌, 60: Q我吧, 70: 请勿打扰
* @param sun sun 当main=11时，可进一步设置 0: 普通在线, 1000: 我的电量, 1011: 信号弱, 1016: 睡觉中, 1017: 游戏中, 1018: 学习中, 1019: 吃饭中, 1021: 煲剧中, 1022: 度假中, 1024: 在线学习, 1025: 在家旅游, 1027: TiMi中, 1028: 我在听歌, 1032: 熬夜中, 1050: 打球中, 1051: 恋爱中, 1052: 我没事
* @param battery 电量 sun=1000时，可以设置上报电量，取值1到100
* @return 失败或无权限返回false
*/
bool API::SetStatus(std::int64_t thisQQ, std::int32_t main, std::int32_t sun, std::int32_t battery)
{
    return e2b(_f<ebool(etext, elong, eint, eint, eint)>(this->j, "设置在线状态")(this->key.c_str(), thisQQ, main, sun, battery));
}

/**
 * @brief 判断某api是否有权限
 * @param permission 权限
 */
bool API::CheckPermission(Permission permission)
{
    return e2b(_f<ebool(etext, etext)>(this->j, "api是否有权限")(this->key.c_str(), s2e(PermissionMap.at(permission))));
}

/**
 * @brief 判断某api是否有权限
 * @param permission 权限
 */
bool API::CheckPermission(std::string permission)
{
    for (auto it : PermissionMap)
    {
        if (it.second == permission)
        {
            return e2b(_f<ebool(etext, etext)>(this->j, "api是否有权限")(this->key.c_str(), s2e(permission)));
        }
    }
    return false;
}

/**
 * @brief 重载自身 [暂时无效，请勿调用]没有权限限制，请勿将新的插件文件下载至plugin文件夹，请确保新旧文件appname一致
 * @param new_file_path 新文件路径 若要更新插件，可将插件文件下载后在此填写新文件路径
*/
void API::ReloadPlugin(std::string new_file_path)
{
    return _f<void(etext, etext)>(this->j, "重载自身")(this->key.c_str(), s2e(new_file_path));
}

/**
 * @brief 获取插件数据目录 没有权限限制，建议将设置文件之类的都写这里面
 * @return 插件数据目录 结果结尾带\
 */
std::string API::GetPluginDataDirectory()
{
    return e2s(_f<etext(etext)>(this->j, "取插件数据目录")(this->key.c_str()));
}

/**
 * @brief QQ点赞 注意，非好友情况下进行点赞时返回成功，但不一定真正点上了
 * @param thisQQ 框架QQ
 * @param otherQQ 对方QQ
 */
std::string API::QQLike(std::int64_t thisQQ, std::int64_t otherQQ)
{
    return e2s(_f<etext(etext, elong, elong)>(this->j, "QQ点赞")(this->key.c_str(), thisQQ, otherQQ));
}

/**
 * @brief 获取图片下载地址
 * @param image_code 图片代码 支持群聊、私聊的图片、闪照代码，注意是图片代码
 * @param thisQQ 框架QQ 群聊图片必填，私聊图片必空
 * @param groupQQ 群号 群聊图片必填，私聊图片必空
 */
std::string API::GetImageDownloadLink(std::string image_code, std::int64_t thisQQ, std::int64_t groupQQ)
{
    return e2s(_f<etext(etext, etext, elong, elong)>(this->j, "取图片下载地址")(this->key.c_str(), s2e(image_code), thisQQ, groupQQ));
}

/**
 * @brief 查询好友信息
 * @param thisQQ 框架QQ
 * @param otherQQ 对方QQ
 * @param data 数据
 * @return 支持陌生人查询
 */
bool API::GetFriendInformation(std::int64_t thisQQ, std::int64_t otherQQ, FriendInformation &data)
{
    volatile FriendInformation *info;
    auto ret = e2b(_f<ebool(etext, elong, elong, volatile FriendInformation **)>(this->j, "查询好友信息")(this->key.c_str(), thisQQ, otherQQ, &info));
    data = *(const_cast<FriendInformation *>(info));
    return ret;
}

/**
 * @brief 查询群信息
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param data 数据
 */
bool API::GetGroupInformation(std::int64_t thisQQ, std::int64_t groupQQ, GroupCardInformation &data)
{
    volatile GroupCardInformation *info;
    auto ret = e2b(_f<ebool(etext, elong, elong, volatile GroupCardInformation **)>(this->j, "查询群信息")(this->key.c_str(), thisQQ, groupQQ, &info));
    data = *(const_cast<GroupCardInformation *>(info));
    return ret;
}

/**
 * @brief 框架重启
 * @return 有权限限制，建议使用前检查是否具有权限
 */
void API::Reboot()
{
    return _f<void(etext)>(this->j, "框架重启")(this->key.c_str());
}

/**
 * @brief 转发群文件至群
 * @param thisQQ 框架QQ
 * @param source_groupQQ 来源群号
 * @param target_groupQQ 目标群号
 * @param fileID FileId
 * @return 失败或无权限返回false
 */
bool API::ForwardGroupFileToGroup(std::int64_t thisQQ, std::int64_t source_groupQQ, std::int64_t target_groupQQ, std::string fileID)
{
    return e2b(_f<ebool(etext, elong, elong, elong, etext)>(this->j, "群文件转发至群")(this->key.c_str(), thisQQ, source_groupQQ, target_groupQQ, s2e(fileID)));
}

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
bool API::ForwardGroupFileToFriend(std::int64_t thisQQ, std::int64_t souce_groupQQ, std::int64_t otherQQ, std::string fileID, std::string file_name, std::int32_t &req, std::int64_t &random, std::int32_t &time)
{
    eint *req_p;
    elong *random_p;
    eint *time_p;
    auto ret = e2b(_f<ebool(etext, elong, elong, elong, etext, etext, eint **, elong **, eint **)>(this->j, "群文件转发至好友")(this->key.c_str(), thisQQ, souce_groupQQ, otherQQ, s2e(fileID), s2e(file_name), &req_p, &random_p, &time_p));
    req = *req_p;
    random = *random_p;
    time = *time_p;
    return ret;
}

/**
 * @brief 转发群文件至好友（若要撤回消息请使用另一重载）
 * @param thisQQ 框架QQ
 * @param souce_groupQQ 来源群号
 * @param otherQQ 目标QQ
 * @param fileID FileId
 * @param file_name Filename
 * @return 失败或无权限返回false
 */
bool API::ForwardGroupFileToFriend(std::int64_t thisQQ, std::int64_t souce_groupQQ, std::int64_t otherQQ, std::string fileID, std::string file_name)
{
    eint *req_p;
    elong *random_p;
    eint *time_p;
    return e2b(_f<ebool(etext, elong, elong, elong, etext, etext, eint **, elong **, eint **)>(this->j, "群文件转发至好友")(this->key.c_str(), thisQQ, souce_groupQQ, otherQQ, s2e(fileID), s2e(file_name), &req_p, &random_p, &time_p));
}

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
bool API::ForwardFriendFileToFriend(std::int64_t thisQQ, std::int64_t sourceQQ, std::int64_t targetQQ, std::string fileID, std::string file_name, std::int32_t &req, std::int32_t &random, std::int32_t &time)
{
    eint *req_p;
    elong *random_p;
    eint *time_p;
    auto ret = e2b(_f<ebool(etext, elong, elong, elong, etext, etext, eint **, elong **, eint **)>(this->j, "好友文件转发至好友")(this->key.c_str(), thisQQ, sourceQQ, targetQQ, s2e(fileID), s2e(file_name), &req_p, &random_p, &time_p));
    req = *req_p;
    random = *random_p;
    time = *time_p;
    return ret;
}

/**
 * @brief 转发好友文件至好友（若要撤回消息请使用另一重载）
 * @param thisQQ 框架QQ
 * @param sourceQQ 来源QQ
 * @param targetQQ 目标QQ
 * @param fileID FileId
 * @param file_name Filename
 * @return 失败或无权限返回false
 */
bool API::ForwardFriendFileToFriend(std::int64_t thisQQ, std::int64_t sourceQQ, std::int64_t targetQQ, std::string fileID, std::string file_name)
{
    eint *req_p;
    elong *random_p;
    eint *time_p;
    return e2b(_f<ebool(etext, elong, elong, elong, etext, etext, eint **, elong **, eint **)>(this->j, "好友文件转发至好友")(this->key.c_str(), thisQQ, sourceQQ, targetQQ, s2e(fileID), s2e(file_name), &req_p, &random_p, &time_p));
}

/**
 * @brief 设置群消息接收
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param set_type 设置类型 1: 接收并提醒, 2: 收进群助手, 3: 屏蔽群消息, 4: 接收不提醒
 * @return 失败或无权限返回false，此API未对返回结果进行分析，返回true不一定成功
 */
bool API::SetGroupMessageReceive(std::int64_t thisQQ, std::int64_t groupQQ, std::int32_t set_type)
{
    return e2b(_f<ebool(etext, elong, elong, eint)>(this->j, "置群消息接收")(this->key.c_str(), thisQQ, groupQQ, set_type));
}

/**
 * @brief 发送免费礼物（绕过广告）
 * @param thisQQ 框架QQ
 * @param groupQQ 群号
 * @param otherQQ 对方QQ
 * @param packageID packageID 299: 卡布奇诺, 302: 猫咪手表, 280: 牵你的手, 281: 可爱猫咪, 284: 神秘面具, 285: 甜wink, 286: 我超忙的, 289: 快乐肥宅水, 290: 幸运手链, 313: 坚强, 307: 绒绒手套, 312: 爱心口罩, 308: 彩虹糖果
 */
std::string API::SendFreeGift(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t otherQQ, std::int32_t packageID)
{
    return e2s(_f<etext(etext, elong, elong, elong, eint)>(this->j, "发送免费礼物")(this->key.c_str(), thisQQ, groupQQ, otherQQ, packageID));
}

/**
 * @brief 获取好友在线状态
 * @param thisQQ 框架QQ
 * @param otherQQ 对方QQ
 * @return 失败或无权限返回空，支持查询陌生人
 */
std::string API::GetFriendStatus(std::int64_t thisQQ, std::int64_t otherQQ)
{
    return e2s(_f<etext(etext, elong, elong)>(this->j, "取好友在线状态")(this->key.c_str(), thisQQ, otherQQ));
}

/**
 * @brief 获取QQ钱包个人信息
 * @param thisQQ 框架QQ
 * @param data 数据 获取银行卡信息时注意不要数组越界
 * @return 包括余额、名字、银行卡等
 */
std::string API::GetQQWalletPersonalInformation(std::int64_t thisQQ, QQWalletInformation &data)
{
    volatile QQWalletInformation *info;
    auto ret = e2s(_f<etext(etext, elong, volatile QQWalletInformation **)>(this->j, "取QQ钱包个人信息")(this->key.c_str(), thisQQ, &info));
    data = *(const_cast<QQWalletInformation *>(info));
    return ret;
}

/**
 * @brief 获取订单详情
 * @param thisQQ 框架QQ
 * @param orderID 订单号或者称之为listid
 * @param data 数据
 * @return 可以查订单，比如别人给你转账，你可以查询转账的详情
 */
std::string API::GetOrderDetail(std::int64_t thisQQ, std::string orderID, OrderDetail &data)
{
    volatile OrderDetail *info;
    auto ret = e2s(_f<etext(etext, elong, etext, volatile OrderDetail **)>(this->j, "获取订单详情")(this->key.c_str(), thisQQ, s2e(orderID), &info));
    data = *(const_cast<OrderDetail *>(info));
    return ret;
}

/**
 * @brief 提交支付验证码
 * @param thisQQ 框架QQ
 * @param captcha_information 验证码信息 银行卡发红包时传回的
 * @param captcha 验证码 手机收到的短信验证码
 * @param payment_password 支付密码 用于验证并支付
 * @return 用银行卡支付时需要验证，只需要验证一次
 */
std::string API::SubmitPaymentCaptcha(std::int64_t thisQQ, volatile CaptchaInformation *captcha_information, std::string captcha, std::string payment_password)
{
    return e2s(_f<etext(etext, elong, volatile CaptchaInformation *, etext, etext)>(this->j, "提交支付验证码")(this->key.c_str(), thisQQ, captcha_information, s2e(captcha), s2e(payment_password)));
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
 * @return 失败或无权限返回false
 */
bool API::ShareMusic(std::int64_t thisQQ, std::int64_t otherQQ, std::string music_name, std::string artist_name, std::string redirect_link, std::string cover_link, std::string file_path, std::int32_t app_type, std::int32_t share_type)
{
    return e2b(_f<ebool(etext, elong, elong, etext, etext, etext, etext, etext, eint, eint)>(this->j, "分享音乐")(this->key.c_str(), thisQQ, otherQQ, s2e(music_name), s2e(artist_name), s2e(redirect_link), s2e(cover_link), s2e(file_path), app_type, share_type));
}

/**
 * @brief 更改群聊消息内容（使用此命令可以更改当前群聊消息内容，并使更改后的内容投递给之后的插件）
 * @param data_pointer 数据指针
 * @param new_message_content 新消息内容
 * @return 无权限返回false
 */
bool API::ModifyGroupMessageContent(std::int32_t data_pointer, std::string new_message_content)
{
    return e2b(_f<ebool(etext, eint, etext)>(this->j, "更改群聊消息内容")(this->key.c_str(), data_pointer, s2e(new_message_content)));
}

/**
 * @brief 更改私聊消息内容（使用此命令可以更改当前私聊消息内容，并使更改后的内容投递给之后的插件）
 * @param data_pointer 数据指针
 * @param new_message_content 新消息内容
 * @return 无权限返回false
 */
bool API::ModifyPrivateMessageContent(std::int32_t data_pointer, std::string new_message_content)
{
    return e2b(_f<ebool(etext, eint, etext)>(this->j, "更改私聊消息内容")(this->key.c_str(), data_pointer, s2e(new_message_content)));
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
std::string API::GroupPasswordRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t groupQQ, std::string password, std::string payment_password, std::int32_t card_serial)
{
    return e2s(_f<etext(etext, elong, eint, eint, elong, etext, etext, eint)>(this->j, "群聊口令红包")(this->key.c_str(), thisQQ, total_number, total_amount, groupQQ, s2e(password), s2e(payment_password), card_serial));
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
std::string API::GroupRandomRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t groupQQ, std::string blessing, std::string payment_password, std::int32_t card_serial, std::int32_t skinID)
{
    return e2s(_f<etext(etext, elong, eint, eint, elong, etext, eint, etext, eint)>(this->j, "群聊拼手气红包")(this->key.c_str(), thisQQ, total_number, total_amount, groupQQ, s2e(blessing), skinID, s2e(payment_password), card_serial));
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
 * @param skinID 红包皮肤Id 1522: 光与夜之恋, 1527: 代号：三国（打了一辈子仗）, 1525: 街霸：对决, 1518: 代号：三国（俺送红包来了）, 1476: 天涯明月刀, 1512: 一人之下，其他皮肤id自己找
 */
std::string API::GroupNormalRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t groupQQ, std::string blessing, std::string payment_password, std::int32_t card_serial, std::int32_t skinID)
{
    return e2s(_f<etext(etext, elong, eint, eint, elong, etext, eint, etext, eint)>(this->j, "群聊普通红包")(this->key.c_str(), thisQQ, total_number, total_amount, groupQQ, s2e(blessing), skinID, s2e(payment_password), card_serial));
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
std::string API::GroupDrawRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t groupQQ, std::string question, std::string payment_password, std::int32_t card_serial)
{
    return e2s(_f<etext(etext, elong, eint, eint, elong, etext, etext, eint)>(this->j, "群聊画图红包")(this->key.c_str(), thisQQ, total_number, total_amount, groupQQ, s2e(question), s2e(payment_password), card_serial));
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
std::string API::GroupAudioRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t groupQQ, std::string audio_password, std::string payment_password, std::int32_t card_serial)
{
    return e2s(_f<etext(etext, elong, eint, eint, elong, etext, etext, eint)>(this->j, "群聊语音红包")(this->key.c_str(), thisQQ, total_number, total_amount, groupQQ, s2e(audio_password), s2e(payment_password), card_serial));
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
std::string API::GroupFollowRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t groupQQ, std::string follow_content, std::string payment_password, std::int32_t card_serial)
{
    return e2s(_f<etext(etext, elong, eint, eint, elong, etext, etext, eint)>(this->j, "群聊接龙红包")(this->key.c_str(), thisQQ, total_number, total_amount, groupQQ, s2e(follow_content), s2e(payment_password), card_serial));
}

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
std::string API::GroupExclusiveRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t groupQQ, std::string otherQQ, std::string blessing, std::string payment_password, std::int32_t card_serial, bool is_equal)
{
    return e2s(_f<etext(etext, elong, eint, eint, elong, etext, etext, ebool, etext, eint)>(this->j, "群聊专属红包")(this->key.c_str(), thisQQ, total_number, total_amount, groupQQ, s2e(otherQQ), s2e(blessing), b2e(is_equal), s2e(payment_password), card_serial));
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
std::string API::FriendPasswordRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t otherQQ, std::string password, std::string payment_password, std::int32_t card_serial)
{
    return e2s(_f<etext(etext, elong, eint, eint, elong, etext, etext, eint)>(this->j, "好友口令红包")(this->key.c_str(), thisQQ, total_number, total_amount, otherQQ, s2e(password), s2e(payment_password), card_serial));
}
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
std::string API::FriendNormalRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t otherQQ, std::string blessing, std::string payment_password, std::int32_t card_serial, std::int32_t skinID)
{
    return e2s(_f<etext(etext, elong, eint, eint, elong, etext, eint, etext, eint)>(this->j, "好友普通红包")(this->key.c_str(), thisQQ, total_number, total_amount, otherQQ, s2e(blessing), skinID, s2e(payment_password), card_serial));
}

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
std::string API::FriendDrawRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t otherQQ, std::string question, std::string payment_password, std::int32_t card_serial)
{
    return e2s(_f<etext(etext, elong, eint, eint, elong, etext, etext, eint)>(this->j, "好友画图红包")(this->key.c_str(), thisQQ, total_number, total_amount, otherQQ, s2e(question), s2e(payment_password), card_serial));
}

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
std::string API::FriendAudioRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t otherQQ, std::string audio_password, std::string payment_password, std::int32_t card_serial)
{
    return e2s(_f<etext(etext, elong, eint, eint, elong, etext, etext, eint)>(this->j, "好友语音红包")(this->key.c_str(), thisQQ, total_number, total_amount, otherQQ, s2e(audio_password), s2e(payment_password), card_serial));
}

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
std::string API::FriendFollowRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t otherQQ, std::string follow_content, std::string payment_password, std::int32_t card_serial)
{
    return e2s(_f<etext(etext, elong, eint, eint, elong, etext, etext, eint)>(this->j, "好友接龙红包")(this->key.c_str(), thisQQ, total_number, total_amount, otherQQ, s2e(follow_content), s2e(payment_password), card_serial));
}

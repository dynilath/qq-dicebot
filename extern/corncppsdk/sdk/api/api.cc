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

#include "../sdk.h"

#include "../eport/earray.h"
#include <windows.h>
// 匿名命名空间，只允许内部链接
namespace
{
// 从api.inc获得相关参数，声明一系列名为_API_func_##_Func_name的函数指针
#define Func(_Func_name, _Arg_type, _Json_name) \
    stdcall_cast<_Arg_type> _API_func_##_Func_name;
#include ".\api.inc"
#undef Func
} // namespace

API::API(etext api_data, etext plugin_key)
    : j(Json::parse(e2s_s(api_data))), key(plugin_key)
{
//从api.inc获得相关参数，为_API_func_##_Func_name这些函数指针赋值
#define Func(_Func_name, _Arg_type, _Json_name)                         \
    _API_func_##_Func_name = reinterpret_cast<stdcall_cast<_Arg_type>>( \
        static_cast<uintptr_t>(this->j[_Json_name]));
#include ".\api.inc"
#undef Func
}

std::string API::OutputLog(const std::string &message, std::int32_t text_color, std::int32_t background_color)
{
    return e2s_s(::_API_func_OutputLog(this->key.c_str(), s2e(message), text_color, background_color));
}

std::string API::SendFriendMessage(std::int64_t thisQQ, std::int64_t friendQQ, const std::string &content, std::int64_t &random, std::int32_t &req)
{
    elong *random_p = nullptr;
    eint *req_p = nullptr;
    auto ret = e2s_s(::_API_func_SendFriendMessage(this->key.c_str(), thisQQ, friendQQ, s2e(content), &random_p, &req_p));
    random = pointer_or_zero(random_p);
    req = pointer_or_zero(req_p);
    return ret;
}

std::string API::SendFriendMessage(std::int64_t thisQQ, std::int64_t friendQQ, const std::string &content)
{
    elong *random_p = nullptr;
    eint *req_p = nullptr;
    return e2s_s(::_API_func_SendFriendMessage(this->key.c_str(), thisQQ, friendQQ, s2e(content), &random_p, &req_p));
}

std::string API::SendGroupMessage(std::int64_t thisQQ, std::int64_t groupQQ, const std::string &content, bool anonymous)
{
    return e2s_s(::_API_func_SendGroupMessage(this->key.c_str(), thisQQ, groupQQ, s2e(content), b2e(anonymous)));
}

std::string API::SendGroupTemporaryMessage(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t otherQQ, const std::string &content, std::int64_t &random, std::int32_t &req)
{
    elong *random_p = nullptr;
    eint *req_p = nullptr;
    auto ret = e2s_s(::_API_func_SendGroupTemporaryMessage(this->key.c_str(), thisQQ, groupQQ, otherQQ, s2e(content), &random_p, &req_p));
    random = pointer_or_zero(random_p);
    req = pointer_or_zero(req_p);
    return ret;
}

std::string API::SendGroupTemporaryMessage(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t otherQQ, const std::string &content)
{
    elong *random_p = nullptr;
    eint *req_p = nullptr;
    return e2s_s(::_API_func_SendGroupTemporaryMessage(this->key.c_str(), thisQQ, groupQQ, otherQQ, s2e(content), &random_p, &req_p));
}

std::string API::AddFriend(std::int64_t thisQQ, std::int64_t otherQQ, const std::string &verification)
{
    return e2s_s(::_API_func_AddFriend(this->key.c_str(), thisQQ, otherQQ, s2e(verification)));
}

std::string API::AddGroup(std::int64_t thisQQ, std::int64_t groupQQ, const std::string &verification)
{
    return e2s_s(::_API_func_AddGroup(this->key.c_str(), thisQQ, groupQQ, s2e(verification)));
}

std::string API::RemoveFriend(std::int64_t thisQQ, std::int64_t otherQQ)
{
    return e2s_s(::_API_func_RemoveFriend(this->key.c_str(), thisQQ, otherQQ));
}

std::string API::SetBlockFriend(std::int64_t thisQQ, std::int64_t otherQQ, bool is_blocked)
{
    return e2s_s(::_API_func_SetBlockFriend(this->key.c_str(), thisQQ, otherQQ, b2e(is_blocked)));
}

std::string API::SetSpecialFriend(std::int64_t thisQQ, std::int64_t otherQQ, bool is_special)
{
    return e2s_s(::_API_func_SetSpecialFriend(this->key.c_str(), thisQQ, otherQQ, b2e(is_special)));
}

std::string API::SendFriendJSONMessage(std::int64_t thisQQ, std::int64_t friendQQ, const std::string &json_content, std::int64_t &random, std::int32_t &req)
{
    elong *random_p = nullptr;
    eint *req_p = nullptr;
    auto ret = e2s_s(::_API_func_SendFriendJSONMessage(this->key.c_str(), thisQQ, friendQQ, s2e(json_content), &random_p, &req_p));
    random = pointer_or_zero(random_p);
    req = pointer_or_zero(req_p);
    return ret;
}

std::string API::SendFriendJSONMessage(std::int64_t thisQQ, std::int64_t friendQQ, const std::string &json_content)
{
    elong *random_p = nullptr;
    eint *req_p = nullptr;
    return e2s_s(::_API_func_SendFriendJSONMessage(this->key.c_str(), thisQQ, friendQQ, s2e(json_content), &random_p, &req_p));
}

std::string API::SendGroupJSONMessage(std::int64_t thisQQ, std::int64_t groupQQ, const std::string &json_content, bool anonymous)
{
    return e2s_s(::_API_func_SendGroupJSONMessage(this->key.c_str(), thisQQ, groupQQ, s2e(json_content), anonymous));
}

std::string API::UploadFriendImage(std::int64_t thisQQ, std::int64_t friendQQ, const std::uint8_t *picture, size_t length, bool is_flash)
{
    return e2s_s(::_API_func_UploadFriendImage(this->key.c_str(), thisQQ, friendQQ, b2e(is_flash), picture, static_cast<eint>(length)));
}

std::string API::UploadGroupImage(std::int64_t thisQQ, std::int64_t groupQQ, const std::uint8_t *picture, size_t size, bool is_flash)
{
    return e2s_s(::_API_func_UploadGroupImage(this->key.c_str(), thisQQ, groupQQ, b2e(is_flash), picture, static_cast<eint>(size)));
}

std::string API::UploadFriendAudio(std::int64_t thisQQ, std::int64_t friendQQ, const std::uint8_t *audio, size_t size, AudioTypeEnum audio_type, const std::string &audio_text)
{
    return e2s_s(::_API_func_UploadFriendAudio(this->key.c_str(), thisQQ, friendQQ, static_cast<eint>(audio_type), s2e(audio_text), audio, static_cast<eint>(size)));
}

std::string API::UploadGroupAudio(std::int64_t thisQQ, std::int64_t groupQQ, const std::uint8_t *audio, size_t size, AudioTypeEnum audio_type, const std::string &audio_text)
{
    return e2s_s(::_API_func_UploadGroupAudio(this->key.c_str(), thisQQ, groupQQ, static_cast<eint>(audio_type), s2e(audio_text), audio, static_cast<eint>(size)));
}

std::string API::UploadAvatar(std::int64_t thisQQ, const std::uint8_t *picture, size_t size)
{
    return e2s_s(::_API_func_UploadAvatar(this->key.c_str(), thisQQ, picture, static_cast<eint>(size)));
}

const std::uint8_t *API::SilkDecode(const std::string &audio_file_path)
{
    // TODO: 实现该方法
    return nullptr;
}

const std::uint8_t *API::SilkEncode(const std::string &audio_file_path)
{
    // TODO: 实现该方法
    return nullptr;
}

std::string API::SetGroupNickname(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t otherQQ, const std::string &nickname)
{
    return e2s_s(::_API_func_SetGroupNickname(this->key.c_str(), thisQQ, groupQQ, otherQQ, s2e(nickname)));
}

std::string API::GetNameFromCache(std::int64_t otherQQ)
{
    return e2s_s(::_API_func_GetNameFromCache(this->key.c_str(), ::std::to_string(otherQQ).c_str()));
}

std::string API::GetNameForce(std::int64_t thisQQ, std::int64_t otherQQ)
{
    return e2s_s(::_API_func_GetNameForce(this->key.c_str(), thisQQ, ::std::to_string(otherQQ).c_str()));
}

std::string API::GetGroupNameFromCache(std::int64_t groupQQ)
{
    return e2s_s(::_API_func_GetGroupNameFromCache(this->key.c_str(), groupQQ));
}

std::string API::GetSKey(std::int64_t thisQQ)
{
    return e2s_s(::_API_func_GetSKey(this->key.c_str(), thisQQ));
}

std::string API::GetPSKey(std::int64_t thisQQ, const std::string &domain)
{
    return e2s_s(::_API_func_GetPSKey(this->key.c_str(), thisQQ, domain.c_str()));
}

std::string API::GetClientKey(std::int64_t thisQQ)
{
    return e2s_s(::_API_func_GetClientKey(this->key.c_str(), thisQQ));
}

std::string API::GetThisQQ()
{
    return e2s_s(::_API_func_GetThisQQ(this->key.c_str()));
}

size_t API::GetFriendList(std::int64_t thisQQ, std::vector<FriendInformation> &friend_list)
{
    earray_head earr;
    eint size = ::_API_func_GetFriendList(this->key.c_str(), thisQQ, earr);
    return size == 0 ? 0 : earray1d2vector<_EType_FriendInformation, FriendInformation>(earr, friend_list);
}

size_t API::GetGroupList(std::int64_t thisQQ, std::vector<GroupInformation> &group_list)
{
    earray_head earr;
    eint size = ::_API_func_GetGroupList(this->key.c_str(), thisQQ, earr);
    return size == 0 ? 0 : earray1d2vector<_EType_GroupInformation, GroupInformation>(earr, group_list);
}

int32_t API::GetGroupMemberList(std::int64_t thisQQ, std::int64_t groupQQ, std::vector<GroupMemberInformation> &group_member_list)
{
    earray_head earr;
    eint size = ::_API_func_GetGroupMemberList(this->key.c_str(), thisQQ, groupQQ, earr);
    return size == 0 ? 0 : earray1d2vector<_EType_GroupMemberInformation, GroupMemberInformation>(earr, group_member_list);
}

bool API::SetAdministrator(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t otherQQ, bool is_administrator)
{
    return e2b(::_API_func_SetAdministrator(this->key.c_str(), thisQQ, groupQQ, otherQQ, b2e(is_administrator)));
}

std::string API::GetAdministratorList(std::int64_t thisQQ, std::int64_t groupQQ)
{
    return e2s_s(::_API_func_GetAdministratorList(this->key.c_str(), thisQQ, groupQQ));
}

std::string API::GetGroupNickname(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t otherQQ)
{
    return e2s_s(::_API_func_GetGroupNickname(this->key.c_str(), thisQQ, groupQQ, otherQQ));
}

std::string API::GetSignature(std::int64_t thisQQ, std::int64_t otherQQ)
{
    return e2s_s(::_API_func_GetSignature(this->key.c_str(), thisQQ, otherQQ));
}

bool API::SetName(std::int64_t thisQQ, const std::string &name)
{
    return e2b(::_API_func_SetName(this->key.c_str(), thisQQ, s2e(name)));
}

bool API::SetSignature(std::int64_t thisQQ, const std::string &signature, const std::string &location)
{
    return e2b(::_API_func_SetSignature(this->key.c_str(), thisQQ, s2e(signature), s2e(location)));
}

bool API::RemoveGroupMember(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t otherQQ, bool is_verification_refused)
{
    return e2b(::_API_func_RemoveGroupMember(this->key.c_str(), thisQQ, groupQQ, otherQQ, b2e(is_verification_refused)));
}

bool API::ShutUpGroupMember(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t otherQQ, std::int32_t time)
{
    return e2b(::_API_func_ShutUpGroupMember(this->key.c_str(), thisQQ, groupQQ, otherQQ, time));
}

bool API::QuitGroup(std::int64_t thisQQ, std::int64_t groupQQ)
{
    return e2b(::_API_func_QuitGroup(this->key.c_str(), thisQQ, groupQQ));
}

bool API::DissolveGroup(std::int64_t thisQQ, std::int64_t groupQQ)
{
    return e2b(::_API_func_DissolveGroup(this->key.c_str(), thisQQ, groupQQ));
}

bool API::UploadGroupAvatar(std::int64_t thisQQ, std::int64_t groupQQ, const std::uint8_t *picture)
{
    return e2b(::_API_func_UploadGroupAvatar(this->key.c_str(), thisQQ, groupQQ, picture));
}

bool API::ShutUpAll(std::int64_t thisQQ, std::int64_t groupQQ, bool is_shut_up_all)
{
    return e2b(::_API_func_ShutUpAll(this->key.c_str(), thisQQ, groupQQ, is_shut_up_all));
}

bool API::GroupPermission_CreateGroup(std::int64_t thisQQ, std::int64_t groupQQ, bool is_allowed)
{
    return e2b(::_API_func_GroupPermission_CreateGroup(this->key.c_str(), thisQQ, groupQQ, b2e(is_allowed)));
}

bool API::GroupPermission_CreateTemporaryConversation(std::int64_t thisQQ, std::int64_t groupQQ, bool is_allowed)
{
    return e2b(::_API_func_GroupPermission_CreateTemporaryConversation(this->key.c_str(), thisQQ, groupQQ, b2e(is_allowed)));
}

bool API::GroupPermission_UploadFile(std::int64_t thisQQ, std::int64_t groupQQ, bool is_allowed)
{
    return e2b(::_API_func_GroupPermission_UploadFile(this->key.c_str(), thisQQ, groupQQ, b2e(is_allowed)));
}

bool API::GroupPermission_UploadPicture(std::int64_t thisQQ, std::int64_t groupQQ, bool is_allowed)
{
    return e2b(::_API_func_GroupPermission_UploadPicture(this->key.c_str(), thisQQ, groupQQ, b2e(is_allowed)));
}

bool API::GroupPermission_InviteFriend(std::int64_t thisQQ, std::int64_t groupQQ, bool is_allowed)
{
    return e2b(::_API_func_GroupPermission_InviteFriend(this->key.c_str(), thisQQ, groupQQ, b2e(is_allowed)));
}

bool API::GroupPermission_Anonymous(std::int64_t thisQQ, std::int64_t groupQQ, bool is_allowed)
{

    return e2b(::_API_func_GroupPermission_Anonymous(this->key.c_str(), thisQQ, groupQQ, b2e(is_allowed)));
}

bool API::GroupPermission_ChatFrankly(std::int64_t thisQQ, std::int64_t groupQQ, bool is_allowed)
{
    return e2b(::_API_func_GroupPermission_ChatFrankly(this->key.c_str(), thisQQ, groupQQ, b2e(is_allowed)));
}

bool API::GroupPermission_NewMemberReadChatHistory(std::int64_t thisQQ, std::int64_t groupQQ, bool is_allowed)
{
    return e2b(::_API_func_GroupPermission_NewMemberReadChatHistory(this->key.c_str(), thisQQ, groupQQ, b2e(is_allowed)));
}

bool API::GroupPermission_SetInviteMethod(std::int64_t thisQQ, std::int64_t groupQQ, std::int32_t method)
{
    return e2b(::_API_func_GroupPermission_SetInviteMethod(this->key.c_str(), thisQQ, groupQQ, method));
}

bool API::Undo_Group(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t message_random, std::int32_t message_req)
{
    return e2b(::_API_func_Undo_Group(this->key.c_str(), thisQQ, groupQQ, message_random, message_req));
}

bool API::Undo_Private(std::int64_t thisQQ, std::int64_t otherQQ, std::int64_t message_random, std::int32_t message_req, std::int32_t time)
{
    return e2b(::_API_func_Undo_Private(this->key.c_str(), thisQQ, otherQQ, message_random, message_req, time));
}

bool API::SetLocationShare(std::int64_t thisQQ, std::int64_t groupQQ, double Longitude, double Latitude, bool is_enabled)
{
    return e2b(::_API_func_SetLocationShare(this->key.c_str(), thisQQ, groupQQ, Longitude, Latitude, b2e(is_enabled)));
}

bool API::ReportCurrent(std::int64_t thisQQ, std::int64_t groupQQ, double Longitude, double Latitude)
{
    return e2b(::_API_func_ReportCurrent(this->key.c_str(), thisQQ, groupQQ, Longitude, Latitude));
}

int64_t API::IsShuttedUp(std::int64_t thisQQ, std::int64_t groupQQ)
{
    return ::_API_func_IsShuttedUp(this->key.c_str(), thisQQ, groupQQ);
}

void API::ProcessGroupVerificationEvent(std::int64_t thisQQ, std::int64_t source_groupQQ, std::int64_t triggerQQ, std::int64_t message_seq, GroupVerificationOperateEnum operate_type, std::int32_t event_type, const std::string &refuse_reason)
{
    return ::_API_func_ProcessGroupVerificationEvent(this->key.c_str(), thisQQ, source_groupQQ, triggerQQ, message_seq, (eint)operate_type, event_type, s2e(refuse_reason));
}

void API::ProcessFriendVerificationEvent(std::int64_t thisQQ, std::int64_t triggerQQ, std::int64_t message_seq, FriendVerificationOperateEnum operate_type)
{
    return ::_API_func_ProcessFriendVerificationEvent(this->key.c_str(), thisQQ, triggerQQ, message_seq, (eint)operate_type);
}

void API::ReadForwardedChatHistory(std::int64_t thisQQ, const std::string &resID, std::vector<GroupMessageData> &message_content)
{
    earray_head earr;
    ::_API_func_ReadForwardedChatHistory(this->key.c_str(), thisQQ, s2e(resID), earr);
    earray1d2vector<_EType_GroupMessageData, GroupMessageData>(earr, message_content);
}

std::string API::UploadGroupFile(std::int64_t thisQQ, std::int64_t groupQQ, const std::string &path, const std::string &folder)
{
    return e2s(::_API_func_UploadGroupFile(this->key.c_str(), thisQQ, groupQQ, s2e(path), s2e(folder)));
}

std::string API::CreateGroupFolder(std::int64_t thisQQ, std::int64_t groupQQ, const std::string &folder)
{
    return e2s(::_API_func_CreateGroupFolder(this->key.c_str(), thisQQ, groupQQ, s2e(folder)));
}

std::string API::RenameGroupFolder(std::int64_t thisQQ, std::int64_t groupQQ, const std::string &old_folder, const std::string &new_folder)
{
    return e2s(::_API_func_RenameGroupFolder(this->key.c_str(), thisQQ, groupQQ, s2e(old_folder), s2e(new_folder)));
}

std::string API::DeleteGroupFolder(std::int64_t thisQQ, std::int64_t groupQQ, const std::string &folder)
{
    return e2s(::_API_func_DeleteGroupFolder(this->key.c_str(), thisQQ, groupQQ, s2e(folder)));
}

std::string API::DeleteGroupFile(std::int64_t thisQQ, std::int64_t groupQQ, const std::string &file_id, const std::string &folder)
{
    return e2s(::_API_func_DeleteGroupFile(this->key.c_str(), thisQQ, groupQQ, s2e(file_id), s2e(folder)));
}

std::string API::MoveGroupFile(std::int64_t thisQQ, std::int64_t groupQQ, const std::string &file_id, const std::string &old_folder, const std::string &new_folder)
{
    return e2s(::_API_func_MoveGroupFile(this->key.c_str(), thisQQ, groupQQ, s2e(file_id), s2e(old_folder), s2e(new_folder)));
}

std::string API::GetGroupFileList(std::int64_t thisQQ, std::int64_t groupQQ, const std::string &folder, std::vector<GroupFileInformation> &group_file_list)
{
    earray_head earr;
    std::string ret = e2s(::_API_func_GetGroupFileList(this->key.c_str(), thisQQ, groupQQ, s2e(folder), earr));
    earray1d2vector<_EType_GroupFileInformation, GroupFileInformation>(earr, group_file_list);
    return ret;
}

std::string API::SaveFileToWeiYun(std::int64_t thisQQ, std::int64_t groupQQ, const std::string &file_id, const std::string &folder)
{
    return e2s(::_API_func_SaveFileToWeiYun(this->key.c_str(), thisQQ, groupQQ, s2e(file_id), s2e(folder)));
}

bool API::SetStatus(std::int64_t thisQQ, StatusTypeEnum main, StatusOnlineTypeEnum sun, std::int32_t battery)
{
    return e2b(::_API_func_SetStatus(this->key.c_str(), thisQQ, static_cast<eint>(main), static_cast<eint>(sun), battery));
}

bool API::CheckPermission(PermissionEnum permission)
{
    return e2b(::_API_func_CheckPermission(this->key.c_str(), s2e(PermissionMap.at(permission))));
}

bool API::CheckPermission(const std::string &permission)
{
    // 确保存在这个权限
    for (auto it : PermissionMap)
    {
        if (it.second == permission)
        {
            return this->CheckPermission(it.first);
        }
    }
    return false;
}

std::string API::GetPluginDataDirectory()
{
    return e2s_s(::_API_func_GetPluginDataDirectory(this->key.c_str()));
}

std::string API::QQLike(std::int64_t thisQQ, std::int64_t otherQQ)
{
    return e2s_s(::_API_func_QQLike(this->key.c_str(), thisQQ, otherQQ));
}

std::string API::GetImageDownloadLink(const std::string &image_code, std::int64_t thisQQ, std::int64_t groupQQ)
{
    return e2s_s(::_API_func_GetImageDownloadLink(this->key.c_str(), s2e(image_code), thisQQ, groupQQ));
}

bool API::GetFriendInformation(std::int64_t thisQQ, std::int64_t otherQQ, FriendInformation &data)
{
    _EType_FriendInformation eInfo;
    auto pi = &eInfo;
    auto ret = e2b(::_API_func_GetFriendInformation(this->key.c_str(), thisQQ, otherQQ, &pi));
    data = eInfo;
    return ret;
}

bool API::GetGroupInformation(std::int64_t thisQQ, std::int64_t groupQQ, GroupCardInformation &data)
{
    _EType_GroupCardInformation eInfo;
    auto pi = &eInfo;
    auto ret = e2b(::_API_func_GetGroupInformation(this->key.c_str(), thisQQ, groupQQ, &pi));
    data = eInfo;
    return ret;
}

void API::Reboot()
{
    return ::_API_func_Reboot(this->key.c_str());
}

bool API::ForwardGroupFileToGroup(std::int64_t thisQQ, std::int64_t source_groupQQ, std::int64_t target_groupQQ, const std::string &fileID)
{
    return e2b(::_API_func_ForwardGroupFileToGroup(this->key.c_str(), thisQQ, source_groupQQ, target_groupQQ, s2e(fileID)));
}

bool API::ForwardGroupFileToFriend(std::int64_t thisQQ, std::int64_t source_groupQQ, std::int64_t otherQQ, const std::string &fileID, const std::string &file_name, std::int64_t file_size, std::int32_t &req, std::int64_t &random, std::int32_t &time)
{
    eint *req_p = nullptr;
    elong *random_p = nullptr;
    eint *time_p = nullptr;
    auto ret = e2b(::_API_func_ForwardGroupFileToFriend(this->key.c_str(), thisQQ, source_groupQQ, otherQQ, s2e(fileID), s2e(file_name), file_size, &req_p, &random_p, &time_p));
    req = pointer_or_zero(req_p);
    random = pointer_or_zero(random_p);
    time = pointer_or_zero(time_p);
    return ret;
}

bool API::ForwardGroupFileToFriend(std::int64_t thisQQ, std::int64_t source_groupQQ, std::int64_t otherQQ, const std::string &fileID, const std::string &file_name, std::int64_t file_size)
{
    std::int32_t req;
    std::int64_t random;
    std::int32_t time;
    return this->ForwardGroupFileToFriend(thisQQ, source_groupQQ, otherQQ, fileID, file_name, file_size, req, random, time);
}

bool API::ForwardFriendFileToFriend(std::int64_t thisQQ, std::int64_t sourceQQ, std::int64_t targetQQ, const std::string &fileID, const std::string &file_name, std::int64_t file_size, std::int32_t &req, std::int64_t &random, std::int32_t &time)
{
    eint *req_p = nullptr;
    elong *random_p = nullptr;
    eint *time_p = nullptr;
    auto ret = e2b(::_API_func_ForwardFriendFileToFriend(this->key.c_str(), thisQQ, sourceQQ, targetQQ, s2e(fileID), s2e(file_name), file_size, &req_p, &random_p, &time_p));
    req = pointer_or_zero(req_p);
    random = pointer_or_zero(random_p);
    time = pointer_or_zero(time_p);
    return ret;
}

bool API::ForwardFriendFileToFriend(std::int64_t thisQQ, std::int64_t sourceQQ, std::int64_t targetQQ, const std::string &fileID, const std::string &file_name, std::int64_t file_size)
{
    std::int32_t req;
    std::int64_t random;
    std::int32_t time;
    return this->ForwardFriendFileToFriend(thisQQ, sourceQQ, targetQQ, fileID, file_name, file_size, req, random, time);
}

bool API::SetGroupMessageReceive(std::int64_t thisQQ, std::int64_t groupQQ, std::int32_t set_type)
{
    return e2b(::_API_func_SetGroupMessageReceive(this->key.c_str(), thisQQ, groupQQ, set_type));
}

std::string API::SendFreeGift(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t otherQQ, FreeGiftEnum gift)
{
    return e2s_s(::_API_func_SendFreeGift(this->key.c_str(), thisQQ, groupQQ, otherQQ, static_cast<eint>(gift)));
}

std::string API::GetFriendStatus(std::int64_t thisQQ, std::int64_t otherQQ)
{
    return e2s_s(::_API_func_GetFriendStatus(this->key.c_str(), thisQQ, otherQQ));
}

std::string API::GetQQWalletPersonalInformation(std::int64_t thisQQ, QQWalletInformation &data)
{
    volatile _EType_QQWalletInformation *eInfo;
    auto ret = e2s_s(::_API_func_GetQQWalletPersonalInformation(this->key.c_str(), thisQQ, &eInfo));
    data = *(const_cast<_EType_QQWalletInformation *>(eInfo));
    return ret;
}

std::string API::GetOrderDetail(std::int64_t thisQQ, const std::string &orderID, OrderDetail &data)
{
    volatile _EType_OrderDetail *eInfo;
    auto ret = e2s_s(::_API_func_GetOrderDetail(this->key.c_str(), thisQQ, s2e(orderID), &eInfo));
    data = *(const_cast<_EType_OrderDetail *>(eInfo));
    return ret;
}

std::string API::SubmitPaymentCaptcha(std::int64_t thisQQ, CaptchaInformation *captcha_information, const std::string &captcha, const std::string &payment_password)
{
    volatile _EType_CaptchaInformation eInfo = *captcha_information;
    return e2s_s(::_API_func_SubmitPaymentCaptcha(this->key.c_str(), thisQQ, &eInfo, s2e(captcha), s2e(payment_password)));
}

bool API::ShareMusic(std::int64_t thisQQ, std::int64_t otherQQ, const std::string &music_name, const std::string &artist_name, const std::string &redirect_link, const std::string &cover_link, const std::string &file_path, std::int32_t app_type, std::int32_t share_type)
{
    return e2b(::_API_func_ShareMusic(this->key.c_str(), thisQQ, otherQQ, s2e(music_name), s2e(artist_name), s2e(redirect_link), s2e(cover_link), s2e(file_path), app_type, share_type));
}

bool API::ModifyGroupMessageContent(std::int32_t data_pointer, const std::string &new_message_content)
{
    return e2b(::_API_func_ModifyGroupMessageContent(this->key.c_str(), data_pointer, s2e(new_message_content)));
}

bool API::ModifyPrivateMessageContent(std::int32_t data_pointer, const std::string &new_message_content)
{
    return e2b(::_API_func_ModifyPrivateMessageContent(this->key.c_str(), data_pointer, s2e(new_message_content)));
}

std::string API::GroupPasswordRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t groupQQ, const std::string &password, const std::string &payment_password, std::int32_t card_serial)
{
    return e2s_s(::_API_func_GroupPasswordRedEnvelope(this->key.c_str(), thisQQ, total_number, total_amount, groupQQ, s2e(password), s2e(payment_password), card_serial));
}

std::string API::GroupRandomRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t groupQQ, const std::string &blessing, const std::string &payment_password, std::int32_t card_serial, std::int32_t skinID)
{
    return e2s_s(::_API_func_GroupRandomRedEnvelope(this->key.c_str(), thisQQ, total_number, total_amount, groupQQ, s2e(blessing), skinID, s2e(payment_password), card_serial));
}

std::string API::GroupNormalRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t groupQQ, const std::string &blessing, const std::string &payment_password, std::int32_t card_serial, std::int32_t skinID)
{
    return e2s_s(::_API_func_GroupNormalRedEnvelope(this->key.c_str(), thisQQ, total_number, total_amount, groupQQ, s2e(blessing), skinID, s2e(payment_password), card_serial));
}

std::string API::GroupDrawRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t groupQQ, const std::string &question, const std::string &payment_password, std::int32_t card_serial)
{
    return e2s_s(::_API_func_GroupDrawRedEnvelope(this->key.c_str(), thisQQ, total_number, total_amount, groupQQ, s2e(question), s2e(payment_password), card_serial));
}

std::string API::GroupAudioRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t groupQQ, const std::string &audio_password, const std::string &payment_password, std::int32_t card_serial)
{
    return e2s_s(::_API_func_GroupAudioRedEnvelope(this->key.c_str(), thisQQ, total_number, total_amount, groupQQ, s2e(audio_password), s2e(payment_password), card_serial));
}

std::string API::GroupFollowRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t groupQQ, const std::string &follow_content, const std::string &payment_password, std::int32_t card_serial)
{
    return e2s_s(::_API_func_GroupFollowRedEnvelope(this->key.c_str(), thisQQ, total_number, total_amount, groupQQ, s2e(follow_content), s2e(payment_password), card_serial));
}

std::string API::GroupExclusiveRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t groupQQ, const std::string &otherQQ, const std::string &blessing, const std::string &payment_password, std::int32_t card_serial, bool is_equal)
{
    return e2s_s(::_API_func_GroupExclusiveRedEnvelope(this->key.c_str(), thisQQ, total_number, total_amount, groupQQ, s2e(otherQQ), s2e(blessing), b2e(is_equal), s2e(payment_password), card_serial));
}

std::string API::FriendPasswordRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t otherQQ, const std::string &password, const std::string &payment_password, std::int32_t card_serial)
{
    return e2s_s(::_API_func_FriendPasswordRedEnvelope(this->key.c_str(), thisQQ, total_number, total_amount, otherQQ, s2e(password), s2e(payment_password), card_serial));
}

std::string API::FriendNormalRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t otherQQ, const std::string &blessing, const std::string &payment_password, std::int32_t card_serial, std::int32_t skinID)
{
    return e2s_s(::_API_func_FriendNormalRedEnvelope(this->key.c_str(), thisQQ, total_number, total_amount, otherQQ, s2e(blessing), skinID, s2e(payment_password), card_serial));
}

std::string API::FriendDrawRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t otherQQ, const std::string &question, const std::string &payment_password, std::int32_t card_serial)
{
    return e2s_s(::_API_func_FriendDrawRedEnvelope(this->key.c_str(), thisQQ, total_number, total_amount, otherQQ, s2e(question), s2e(payment_password), card_serial));
}

std::string API::FriendAudioRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t otherQQ, const std::string &audio_password, const std::string &payment_password, std::int32_t card_serial)
{
    return e2s_s(::_API_func_FriendAudioRedEnvelope(this->key.c_str(), thisQQ, total_number, total_amount, otherQQ, s2e(audio_password), s2e(payment_password), card_serial));
}

std::string API::FriendFollowRedEnvelope(std::int64_t thisQQ, std::int32_t total_number, std::int32_t total_amount, std::int64_t otherQQ, const std::string &follow_content, const std::string &payment_password, std::int32_t card_serial)
{
    return e2s_s(::_API_func_FriendFollowRedEnvelope(this->key.c_str(), thisQQ, total_number, total_amount, otherQQ, s2e(follow_content), s2e(payment_password), card_serial));
}

bool API::SetExclusiveTitle(std::int64_t thisQQ, std::int64_t groupQQ, std::int64_t otherQQ, std::string title)
{
    return e2b(::_API_func_SetExclusiveTitle(this->key.c_str(), thisQQ, groupQQ, otherQQ, s2e(title)));
}

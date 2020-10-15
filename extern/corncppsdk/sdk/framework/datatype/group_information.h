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

#include <string>
#include <vector>
#include <algorithm>

#include "../../eport/etypes.h"
#include "../constants.h"
#include "./etext2string_mem.h"

#pragma pack(4)
// Note: _EType_开头的是内部转换用的类型，请使用普通的GroupInformation
struct _EType_GroupInformation
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
    etext GroupName = nullptr;
    // strGroupMemo
    etext GroupMemo = nullptr;
};
#pragma pack()

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
    ::std::string GroupName;
    // strGroupMemo
    ::std::string GroupMemo;

    GroupInformation(const _EType_GroupInformation &info) : GroupID{info.GroupID},
                                                            GroupQQ{info.GroupQQ},
                                                            CFlag{info.CFlag},
                                                            GroupInfoSeq{info.GroupInfoSeq},
                                                            GroupFlagExt{info.GroupFlagExt},
                                                            GroupRankSeq{info.GroupRankSeq},
                                                            CertificationType{info.CertificationType},
                                                            ShutUpTimestamp{info.ShutUpTimestamp},
                                                            ThisShutUpTimestamp{info.ThisShutUpTimestamp},
                                                            CmdUinUinFlag{info.CmdUinUinFlag},
                                                            AdditionalFlag{info.AdditionalFlag},
                                                            GroupTypeFlag{info.GroupTypeFlag},
                                                            GroupSecType{info.GroupSecType},
                                                            GroupSecTypeInfo{info.GroupSecTypeInfo},
                                                            GroupClassExt{info.GroupClassExt},
                                                            AppPrivilegeFlag{info.AppPrivilegeFlag},
                                                            SubscriptionUin{info.SubscriptionUin},
                                                            GroupMemberCount{info.GroupMemberCount},
                                                            MemberNumSeq{info.MemberNumSeq},
                                                            MemberCardSeq{info.MemberCardSeq},
                                                            GroupFlagExt3{info.GroupFlagExt3},
                                                            GroupOwnerUin{info.GroupOwnerUin},
                                                            IsConfGroup{info.IsConfGroup},
                                                            IsModifyConfGroupFace{info.IsModifyConfGroupFace},
                                                            IsModifyConfGroupName{info.IsModifyConfGroupName},
                                                            CmduinJoinTime{info.CmduinJoinTime}
    {
        string_e2std(this->GroupName, info.GroupName);
        string_e2std(this->GroupMemo, info.GroupMemo);
    }
};

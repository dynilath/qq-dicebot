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

#include "../../eport/etypes.h"
#include "../constants.h"
#include "./etext2string_mem.h"

#pragma pack(4)
// Note: _EType_开头的是内部转换用的类型，请使用普通的GroupMemberInformation
struct _EType_GroupMemberInformation
{
    // 账号
    etext QQNumber = nullptr;
    // 年龄
    eint Age;
    // 性别
    eint Gender;
    // 昵称
    etext Name = nullptr;
    // 邮箱
    etext Email = nullptr;
    // 名片
    etext Nickname = nullptr;
    // 备注
    etext Note = nullptr;
    // 头衔
    etext Title = nullptr;
    // 手机号
    etext Phone = nullptr;
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
#pragma pack()

// 群成员信息
struct GroupMemberInformation
{
    // 账号
    ::std::string QQNumber;
    // 年龄
    eint Age;
    // 性别
    eint Gender;
    // 昵称
    ::std::string Name;
    // 邮箱
    ::std::string Email;
    // 名片
    ::std::string Nickname;
    // 备注
    ::std::string Note;
    // 头衔
    ::std::string Title;
    // 手机号
    ::std::string Phone;
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

    GroupMemberInformation(const _EType_GroupMemberInformation &info) : Age{info.Age},
                                                                        Gender{info.Gender},
                                                                        TitleTimeout{info.TitleTimeout},
                                                                        ShutUpTimestamp{info.ShutUpTimestamp},
                                                                        JoinTime{info.JoinTime},
                                                                        ChatTime{info.ChatTime},
                                                                        Level{info.Level}
    {
        string_e2std(this->QQNumber, info.QQNumber);
        string_e2std(this->Name, info.Name);
        string_e2std(this->Email, info.Email);
        string_e2std(this->Nickname, info.Nickname);
        string_e2std(this->Note, info.Note);
        string_e2std(this->Title, info.Title);
        string_e2std(this->Phone, info.Phone);
    }
};

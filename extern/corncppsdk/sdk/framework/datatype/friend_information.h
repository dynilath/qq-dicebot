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
#include "./service_information.h"
#include "./etext2string_mem.h"

#pragma pack(4)
// Note: _EType_开头的是内部转换用的类型，请使用普通的FriendInformation
struct _EType_FriendInformation
{
    // 邮箱
    etext Email = nullptr;
    // 账号
    elong QQNumber;
    // 昵称
    etext Name = nullptr;
    // 备注
    etext Note = nullptr;
    // 在线状态 只能使用[取好友列表]获取
    etext Status = nullptr;
    // 赞数量 只能使用[查询好友信息]获取
    eint Likes;
    // 签名 只能使用[查询好友信息]获取
    etext Signature = nullptr;
    // 性别 255: 隐藏, 0: 男, 1: 女
    eint Gender;
    // Q等级 只能使用[查询好友信息]获取
    eint Level;
    // 年龄 只能使用[查询好友信息]获取
    eint Age;
    // 国家 只能使用[查询好友信息]获取
    etext Nation = nullptr;
    // 省份 只能使用[查询好友信息]获取
    etext Province = nullptr;
    // 城市 只能使用[查询好友信息]获取
    etext City = nullptr;
    // 服务列表 只能使用[查询好友信息]获取
    _EType_ServiceInformation *ServiceList = nullptr;
    // 连续在线天数 只能使用[查询好友信息]获取
    eint ContinuousOnlineTime;
    // QQ达人 只能使用[查询好友信息]获取
    etext QQTalent = nullptr;
    // 今日已赞 只能使用[查询好友信息]获取
    eint LikesToday;
    // 今日可赞数 只能使用[查询好友信息]获取
    eint LikesAvailableToday;
};
#pragma pack()

// 好友信息
struct FriendInformation
{
    // 邮箱
    ::std::string Email;
    // 账号
    elong QQNumber;
    // 昵称
    ::std::string Name;
    // 备注
    ::std::string Note;
    // 在线状态 只能使用[取好友列表]获取
    ::std::string Status;
    // 赞数量 只能使用[查询好友信息]获取
    eint Likes;
    // 签名 只能使用[查询好友信息]获取
    ::std::string Signature;
    // 性别 255: 隐藏, 0: 男, 1: 女
    enum GenderType : eint
    {
        UNKNOW = 255,
        Male = 0,
        Female = 1
    } Gender;
    // Q等级 只能使用[查询好友信息]获取
    eint Level;
    // 年龄 只能使用[查询好友信息]获取
    eint Age;
    // 国家 只能使用[查询好友信息]获取
    ::std::string Nation;
    // 省份 只能使用[查询好友信息]获取
    ::std::string Province;
    // 城市 只能使用[查询好友信息]获取
    ::std::string City;
    // 服务列表 只能使用[查询好友信息]获取
    ::std::vector<ServiceInformation> ServiceList;
    // 连续在线天数 只能使用[查询好友信息]获取
    eint ContinuousOnlineTime;
    // QQ达人 只能使用[查询好友信息]获取
    ::std::string QQTalent;
    // 今日已赞 只能使用[查询好友信息]获取
    eint LikesToday;
    // 今日可赞数 只能使用[查询好友信息]获取
    eint LikesAvailableToday;

    friend _EType_FriendInformation;

    FriendInformation() = default;

    FriendInformation(const _EType_FriendInformation &info) : QQNumber{info.QQNumber},
                                                              Gender{info.Gender},
                                                              Level{info.Level},
                                                              Age{info.Age},
                                                              ContinuousOnlineTime{info.ContinuousOnlineTime},
                                                              LikesToday{info.LikesToday},
                                                              LikesAvailableToday{info.LikesAvailableToday}
    {
        string_e2std(this->Email, info.Email);
        string_e2std(this->Name, info.Name);
        string_e2std(this->Note, info.Note);
        string_e2std(this->Status, info.Status);
        string_e2std(this->Signature, info.Signature);
        string_e2std(this->Nation, info.Nation);
        string_e2std(this->Province, info.Province);
        string_e2std(this->City, info.City);
        string_e2std(this->QQTalent, info.QQTalent);

        if (info.ServiceList != nullptr)
        {
            auto size = reinterpret_cast<eint *>(info.ServiceList)[1];
            auto pptr = reinterpret_cast<_EType_ServiceInformation **>(
                reinterpret_cast<eint *>(info.ServiceList) + 2);
            ::std::for_each(pptr, pptr + size, [this](const auto ptr) {
                this->ServiceList.push_back(*ptr);
            });
        }
    }
};

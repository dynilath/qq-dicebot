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
#include "./card_information.h"

#pragma pack(4)
// Note: _EType_开头的是内部转换用的类型，请使用普通的QQWalletInformation
struct _EType_QQWalletInformation
{
    // 余额
    etext Balance = nullptr;
    // 身份证号
    etext ID = nullptr;
    // 实名
    etext RealName = nullptr;
    // 银行卡列表
    _EType_CardInformation *CardList = nullptr;
};
#pragma pack()

// QQ钱包信息
struct QQWalletInformation
{
    // 余额
    ::std::string Balance;
    // 身份证号
    ::std::string ID;
    // 实名
    ::std::string RealName;
    // 银行卡列表
    ::std::vector<CardInformation> CardList;

    QQWalletInformation(const _EType_QQWalletInformation &info)
    {
        string_e2std(this->Balance, info.Balance);
        string_e2std(this->ID, info.ID);
        string_e2std(this->RealName, info.RealName);

        if (info.CardList != nullptr)
        {
            auto size = reinterpret_cast<eint *>(info.CardList)[1];
            auto pptr = reinterpret_cast<_EType_CardInformation **>(
                reinterpret_cast<eint *>(info.CardList) + 2);
            ::std::for_each(pptr, pptr + size, [this](const auto ptr) {
                this->CardList.push_back(*ptr);
            });
        }
    }
};

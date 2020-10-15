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
// Note: _EType_开头的是内部转换用的类型，请使用普通的CardInformation
struct _EType_CardInformation
{
    // 序列
    eint Serial;
    // 尾号
    etext TailNumber = nullptr;
    // 银行
    etext Bank = nullptr;
    // 绑定手机
    etext BindPhone = nullptr;
    // bind_serial
    etext BindSerial = nullptr;
    // bank_type
    etext BankType = nullptr;
};
#pragma pack()

// 银行卡信息
struct CardInformation
{
    // 序列
    eint Serial;
    // 尾号
    ::std::string TailNumber;
    // 银行
    ::std::string Bank;
    // 绑定手机
    ::std::string BindPhone;
    // bind_serial
    ::std::string BindSerial;
    // bank_type
    ::std::string BankType;

    CardInformation(const _EType_CardInformation &info) : Serial{info.Serial}
    {
        string_e2std(this->TailNumber, info.TailNumber);
        string_e2std(this->Bank, info.Bank);
        string_e2std(this->BindPhone, info.BindPhone);
        string_e2std(this->BindSerial, info.BindSerial);
        string_e2std(this->BankType, info.BankType);
    }
};

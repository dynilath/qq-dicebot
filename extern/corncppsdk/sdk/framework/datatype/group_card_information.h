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
// Note: _EType_开头的是内部转换用的类型，请使用普通的GroupCardInformation
struct _EType_GroupCardInformation
{
    // 群名称
    etext GroupName = nullptr;
    // 群地点
    etext GroupLocation = nullptr;
    // 群分类
    etext GroupClassification = nullptr;
    // 群标签 以|分割
    etext GroupTags = nullptr;
    // 群介绍
    etext GroupDescription = nullptr;
};
#pragma pack()

// 群卡片信息
struct GroupCardInformation
{
    // 群名称
    ::std::string GroupName;
    // 群地点
    ::std::string GroupLocation;
    // 群分类
    ::std::string GroupClassification;
    // 群标签 以|分割
    ::std::string GroupTags;
    // 群介绍
    ::std::string GroupDescription;

    GroupCardInformation() = default;

    GroupCardInformation(const _EType_GroupCardInformation &info)
    {
        string_e2std(this->GroupName, info.GroupName);
        string_e2std(this->GroupLocation, info.GroupLocation);
        string_e2std(this->GroupClassification, info.GroupClassification);
        string_e2std(this->GroupTags, info.GroupTags);
        string_e2std(this->GroupDescription, info.GroupDescription);
    }
};

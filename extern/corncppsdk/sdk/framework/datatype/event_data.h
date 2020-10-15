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
// Note: _EType_开头的是内部转换用的类型，请使用普通的EventData
struct _EType_EventData
{
    // 框架QQ
    elong ThisQQ;
    // 来源群号
    elong SourceGroupQQ;
    // 操作QQ
    elong OperateQQ;
    // 触发QQ
    elong TriggerQQ;
    // 消息Seq
    elong MessageSeq;
    // 消息时间戳
    eint MessageTimestamp;
    // 来源群名
    etext SourceGroupName = nullptr;
    // 操作QQ昵称
    etext OperateQQName = nullptr;
    // 触发QQ昵称
    etext TriggerQQName = nullptr;
    // 事件内容
    etext MessageContent = nullptr;
    // 事件类型
    EventTypeEnum EventType;
    // 事件子类型
    eint EventSubType;
};
#pragma pack()

// 事件数据
struct EventData
{
    // 框架QQ
    elong ThisQQ;
    // 来源群号
    elong SourceGroupQQ;
    // 操作QQ
    elong OperateQQ;
    // 触发QQ
    elong TriggerQQ;
    // 消息Seq
    elong MessageSeq;
    // 消息时间戳
    eint MessageTimestamp;
    // 来源群名
    ::std::string SourceGroupName;
    // 操作QQ昵称
    ::std::string OperateQQName;
    // 触发QQ昵称
    ::std::string TriggerQQName;
    // 事件内容
    ::std::string MessageContent;
    // 事件类型
    EventTypeEnum EventType;
    // 事件子类型
    eint EventSubType;

    EventData(const _EType_EventData &info) : ThisQQ{info.ThisQQ},
                                              SourceGroupQQ{info.SourceGroupQQ},
                                              OperateQQ{info.OperateQQ},
                                              TriggerQQ{info.TriggerQQ},
                                              MessageSeq{info.MessageSeq},
                                              MessageTimestamp{info.MessageTimestamp},
                                              EventType{info.EventType},
                                              EventSubType{info.EventSubType}
    {
        string_e2std(this->SourceGroupName, info.SourceGroupName);
        string_e2std(this->OperateQQName, info.OperateQQName);
        string_e2std(this->TriggerQQName, info.TriggerQQName);
        string_e2std(this->MessageContent, info.MessageContent);
    }
};

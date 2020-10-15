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
#include <algorithm>

#include "../../eport/etypes.h"
#include "../constants.h"
#include "./etext2string_mem.h"

#pragma pack(4)
// Note: _EType_开头的是内部转换用的类型，请使用普通的PrivateMessageData
struct _EType_PrivateMessageData
{
    // 发送人QQ
    elong SenderQQ;
    // 框架QQ
    elong ThisQQ;
    // 消息Req
    eint MessageReq;
    // 消息Seq
    elong MessageSeq;
    // 消息接收时间
    eint MessageReceiveTime;
    // 消息群号 当为群临时会话时可取
    elong MessageGroupQQ;
    // 消息发送时间
    eint MessageSendTime;
    // 消息Random
    elong MessageRandom;
    // 消息分片序列
    eint MessageClip;
    // 消息分片数量
    eint MessageClipCount;
    // 消息分片标识
    elong MessageClipID;
    // 消息内容
    etext MessageContent = nullptr;
    // 发送人气泡ID
    eint BubbleID;
    // 消息类型
    MessageTypeEnum MessageType;
    // 消息子类型
    MessageSubTypeEnum MessageSubType;
    // 消息子临时类型 0: 群, 1: 讨论组, 129: 腾讯公众号, 201: QQ咨询
    MessageSubTypeEnum MessageSubTemporaryType;
    // 红包类型 0: 非红包的普通消息, 2: 已转入余额, 4: 点击收款, 10: 红包
    eint RedEnvelopeType;
    // 会话Token
    ebin SessionToken = nullptr;
    // 来源事件QQ
    elong SourceEventQQ;
    // 来源事件QQ昵称
    etext SourceEventQQName = nullptr;
    // 文件ID
    etext FileID = nullptr;
    // 文件Md5
    ebin FileMD5 = nullptr;
    // 文件名
    etext FileName = nullptr;
    // 文件大小
    elong FileSize;
};
#pragma pack()

// 私聊消息数据
struct PrivateMessageData
{
    // 发送人QQ
    elong SenderQQ;
    // 框架QQ
    elong ThisQQ;
    // 消息Req
    eint MessageReq;
    // 消息Seq
    elong MessageSeq;
    // 消息接收时间
    eint MessageReceiveTime;
    // 消息群号 当为群临时会话时可取
    elong MessageGroupQQ;
    // 消息发送时间
    eint MessageSendTime;
    // 消息Random
    elong MessageRandom;
    // 消息分片序列
    eint MessageClip;
    // 消息分片数量
    eint MessageClipCount;
    // 消息分片标识
    elong MessageClipID;
    // 消息内容
    ::std::string MessageContent;
    // 发送人气泡ID
    eint BubbleID;
    // 消息类型
    MessageTypeEnum MessageType;
    // 消息子类型
    MessageSubTypeEnum MessageSubType;
    // 消息子临时类型 0: 群, 1: 讨论组, 129: 腾讯公众号, 201: QQ咨询
    MessageSubTypeEnum MessageSubTemporaryType;
    // 红包类型 0: 非红包的普通消息, 2: 已转入余额, 4: 点击收款, 10: 红包
    eint RedEnvelopeType;
    // 会话Token
    ebin SessionToken;
    // 来源事件QQ
    elong SourceEventQQ;
    // 来源事件QQ昵称
    ::std::string SourceEventQQName;
    // 文件ID
    ::std::string FileID;
    // 文件Md5
    ebin FileMD5 = nullptr;
    // 文件名
    ::std::string FileName;
    // 文件大小
    elong FileSize;

    PrivateMessageData(const _EType_PrivateMessageData &info) : SenderQQ{info.SenderQQ},
                                                                ThisQQ{info.ThisQQ},
                                                                MessageReq{info.MessageReq},
                                                                MessageSeq{info.MessageSeq},
                                                                MessageReceiveTime{info.MessageReceiveTime},
                                                                MessageGroupQQ{info.MessageGroupQQ},
                                                                MessageSendTime{info.MessageSendTime},
                                                                MessageRandom{info.MessageRandom},
                                                                MessageClip{info.MessageClip},
                                                                MessageClipCount{info.MessageClipCount},
                                                                MessageClipID{info.MessageClipID},
                                                                BubbleID{info.BubbleID},
                                                                MessageType{info.MessageType},
                                                                MessageSubType{info.MessageSubType},
                                                                MessageSubTemporaryType{info.MessageSubTemporaryType},
                                                                RedEnvelopeType{info.RedEnvelopeType},
                                                                SourceEventQQ{info.SourceEventQQ},
                                                                FileSize{info.FileSize}
    {
        string_e2std(this->MessageContent, info.MessageContent);
        string_e2std(this->SourceEventQQName, info.SourceEventQQName);
        string_e2std(this->FileID, info.FileID);
        string_e2std(this->FileName, info.FileName);
    }
};

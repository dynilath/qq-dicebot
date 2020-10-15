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
// Note: _EType_开头的是内部转换用的类型，请使用普通的GroupMessageData
struct _EType_GroupMessageData
{
    // 发送人QQ
    elong SenderQQ;
    // 框架QQ
    elong ThisQQ;
    // 消息Req
    eint MessageReq;
    // 消息接收时间
    eint MessageReceiveTime;
    // 消息群号
    elong MessageGroupQQ;
    // 消息来源群名（貌似失效了）
    etext SourceGroupName = nullptr;
    // 发送人群名片 没有名片则为空白
    etext SenderNickname = nullptr;
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
    // 消息类型
    MessageTypeEnum MessageType;
    // 发送人群头衔
    etext SenderTitle = nullptr;
    // 消息内容
    etext MessageContent = nullptr;
    // 回复对象消息内容 如果是回复消息
    etext ReplyMessageContent = nullptr;
    // 发送者气泡ID
    eint BubbleID;
    // 框架QQ匿名Id，用于区分别人和自己(当天从未使用过匿名则为0)
    eint ThisQQAnonymousID;
    // 保留参数，请勿使用
    eint reserved_;
    // 文件Id
    etext FileID = nullptr;
    // 文件Md5
    ebin FileMD5 = nullptr;
    // 文件名
    etext FileName = nullptr;
    // 文件大小
    elong FileSize;
    // 消息AppID
    eint MessageAppID;
};
#pragma pack()

// 群消息数据
struct GroupMessageData
{
    // 发送人QQ
    elong SenderQQ;
    // 框架QQ
    elong ThisQQ;
    // 消息Req
    eint MessageReq;
    // 消息接收时间
    eint MessageReceiveTime;
    // 消息群号
    elong MessageGroupQQ;
    // 消息来源群名（貌似失效了）
    ::std::string SourceGroupName;
    // 发送人群名片 没有名片则为空白
    ::std::string SenderNickname;
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
    // 消息类型
    MessageTypeEnum MessageType;
    // 发送人群头衔
    ::std::string SenderTitle;
    // 消息内容
    ::std::string MessageContent;
    // 回复对象消息内容 如果是回复消息
    ::std::string ReplyMessageContent;
    // 发送者气泡ID
    eint BubbleID;
    // 框架QQ匿名Id，用于区分别人和自己(当天从未使用过匿名则为0)
    eint ThisQQAnonymousID;
    // 保留参数，请勿使用
    eint reserved_;
    // 文件Id
    ::std::string FileID;
    // 文件Md5
    ebin FileMD5 = nullptr;
    // 文件名
    ::std::string FileName;
    // 文件大小
    elong FileSize;
    // 消息AppID
    eint MessageAppID;
    GroupMessageData(const _EType_GroupMessageData &info) : SenderQQ{info.SenderQQ},
                                                            ThisQQ{info.ThisQQ},
                                                            MessageReq{info.MessageReq},
                                                            MessageReceiveTime{info.MessageReceiveTime},
                                                            MessageGroupQQ{info.MessageGroupQQ},
                                                            MessageSendTime{info.MessageSendTime},
                                                            MessageRandom{info.MessageRandom},
                                                            MessageClip{info.MessageClip},
                                                            MessageClipCount{info.MessageClipCount},
                                                            MessageClipID{info.MessageClipID},
                                                            MessageType{info.MessageType},
                                                            BubbleID{info.BubbleID},
                                                            ThisQQAnonymousID{info.ThisQQAnonymousID},
                                                            FileSize{info.FileSize},
                                                            MessageAppID{info.MessageAppID}
    {
        string_e2std(this->SourceGroupName, info.SourceGroupName);
        string_e2std(this->SenderNickname, info.SenderNickname);
        string_e2std(this->SenderTitle, info.SenderTitle);
        string_e2std(this->MessageContent, info.MessageContent);
        string_e2std(this->ReplyMessageContent, info.ReplyMessageContent);
        string_e2std(this->FileID, info.FileID);
        string_e2std(this->FileName, info.FileName);
    }
};

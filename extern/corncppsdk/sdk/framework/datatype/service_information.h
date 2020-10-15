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

#include "../../eport/etypes.h"
#include "../constants.h"

#pragma pack(4)
// Note: _EType_开头的是内部转换用的类型，请使用普通的ServiceInformation
struct _EType_ServiceInformation
{
    // 服务代号 1: SVIP, 4: 视频会员, 6: 音乐包, 105: star, 102: 黄钻, 103: 绿钻, 101: 红钻, 104:yellowlove, 107: SVIP&视频会员, 109: SVIP&绿钻, 110: SVIP&音乐包
    eint ServiceCodename;
    // 服务等级
    eint ServiceLevel;
};
#pragma pack()

// 服务信息
struct ServiceInformation
{
    enum ServiceCode : eint
    {
        SVIP = 1,              //SVIP
        VIDEO_VIP = 4,         //视频会员
        MUSIC_PACK = 6,        //音乐包
        STAR = 105,            //star
        YELLOW_DIAMOND = 102,  //黄钻
        GREEN_DIAMOND = 103,   //绿钻
        RED_DIAMOND = 101,     //红钻
        YELLOWLOVE = 104,      //yellowlove
        SVIP_WITH_VIDEO = 107, //SVIP&视频会员
        SVIP_WITH_GREEN = 109, //SVIP&绿钻
        SVIP_WITH_MUSIC = 110  //SVIP&音乐包
    } ServiceCodename;
    eint ServiceLevel;

    ServiceInformation(const _EType_ServiceInformation &info) : ServiceCodename{info.ServiceCodename}, ServiceLevel{info.ServiceLevel}
    {
    }
};

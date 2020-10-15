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
// Note: _EType_开头的是内部转换用的类型，请使用普通的GroupFileInformation
struct _EType_GroupFileInformation
{
    // 文件夹fileid或者文件fileid
    etext FileID = nullptr;
    // 文件夹名或文件名
    etext FileName = nullptr;
    // 文件大小，文件夹时表示有多少个文件
    elong FileSize;
    // 文件md5，文件夹时为空，部分文件类型也可能是空
    ebin FileMd5 = nullptr;
    // 创建文件夹或上传文件的QQ
    elong FileFromUin;
    // 创建文件夹或上传文件的QQ
    etext FileFromNick = nullptr;
    // 文件类型  1: 文件, 2: 文件夹
    eint FileType;
};
#pragma pack()

// 群文件信息
struct GroupFileInformation
{
    // 文件夹fileid或者文件fileid
    ::std::string FileID;
    // 文件夹名或文件名
    ::std::string FileName;
    // 文件大小，文件夹时表示有多少个文件
    elong FileSize;
    // 文件md5，文件夹时为空，部分文件类型也可能是空
    ebin FileMd5;
    // 创建文件夹或上传文件的QQ
    elong FileFromUin;
    // 创建文件夹或上传文件的QQ
    ::std::string FileFromNick;
    // 文件类型  1: 文件, 2: 文件夹
    eint FileType;

    GroupFileInformation(const _EType_GroupFileInformation &info) : FileSize{info.FileSize},
                                                                    FileFromUin{info.FileFromUin},
                                                                    FileType{info.FileType}
    {
        string_e2std(this->FileID, info.FileID);
        string_e2std(this->FileName, info.FileName);
        string_e2std(this->FileFromNick, info.FileFromNick);
    }
};

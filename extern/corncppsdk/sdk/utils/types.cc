/*
Cornerstone SDK v0.2.0
-- 面向现代 C++ 的 Corn SDK
兼容 Corn SDK v2.6.5
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

#include "../sdk.h"

#include <cstring>
#include <type_traits>

std::string GBKtoUTF8(const char *src_str)
{
    auto len = MultiByteToWideChar(CP_ACP, 0, src_str, -1, nullptr, 0);
    std::vector<wchar_t> wstr(len);
    MultiByteToWideChar(CP_ACP, 0, src_str, -1, wstr.data(), len);
    len = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), -1, nullptr, 0, nullptr, nullptr);
    std::string utf8(len-1,' ');
    WideCharToMultiByte(CP_UTF8, 0, wstr.data(), -1, utf8.data(), len, nullptr, nullptr);
    return utf8;
}

std::string UTF8toGBK(const std::string &src_str)
{
    auto len = MultiByteToWideChar(CP_UTF8, 0, src_str.c_str(), -1, nullptr, 0);
    std::vector<wchar_t> wszGBK(len);
    MultiByteToWideChar(CP_UTF8, 0, src_str.c_str(), -1, wszGBK.data(), len);
    len = WideCharToMultiByte(CP_ACP, 0, wszGBK.data(), -1, nullptr, 0, nullptr, nullptr);
    std::string szGBK(len-1,' ');
    WideCharToMultiByte(CP_ACP, 0, wszGBK.data(), -1, szGBK.data(), len, nullptr, nullptr);
    return szGBK;
}

earray::earray() noexcept
{
    // 分配一个空间专门给框架释放着玩
    this->heap = GetProcessHeap();
    this->data = HeapAlloc(heap, HEAP_ZERO_MEMORY, 1);
    *((std::uint8_t *)this->data) = 1;
}

earray::~earray() noexcept
{
    HeapFree(this->heap, 0, this->data);
}

size_t earray::GetDimension() const noexcept
{
    return ((eint *)this->data)[0];
}

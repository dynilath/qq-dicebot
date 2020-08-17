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

#ifndef CORNERSTONE_HEADER_UNTILS_INL_H_
#define CORNERSTONE_HEADER_UNTILS_INL_H_

#include "../sdk.h"

#include <string>

template<typename>
struct FuncRestruct;

template<typename RetT,typename ...ArgT>
struct FuncRestruct<RetT(ArgT...)> {
    using stdcall_t = RetT(__stdcall*)(ArgT...);
};

template <class FuncT>
inline auto _f(Json api, const char *name)
{
    using stdcall_t = FuncRestruct<FuncT>::stdcall_t;
    auto value = static_cast<uintptr_t>(api[name]);
    return reinterpret_cast<stdcall_t>(value);
}

/**
 * @brief 获取API返回的JSON文本中的返回码
 * @param retstr API返回的JSON文本
 * @return 返回码
 */
inline std::int32_t get_retcode(std::string retstr)
{
    return (std::int32_t)(Json::parse(retstr)["retcode"]);
}

inline std::string to_string(const char *value)
{
    return std::string(value);
}

inline std::string to_string(const std::string &value)
{
    return value;
}

template <class T>
inline std::string to_string(T value)
{
    return std::to_string(value);
}

/**
 * @brief 依次连接所有参数
 * @return 所有参数连接后的字符串
 */
template <class... Types>
inline std::string sum_string(Types... args)
{
    return (... + to_string(args));
}

constexpr std::uint32_t make_color(std::uint8_t r, std::uint8_t g, std::uint8_t b)
{
    return (r) | (g << 8) | (b << 16);
}

constexpr void read_color(std::uint32_t color, std::uint8_t &r, std::uint8_t &g, std::uint8_t &b)
{
    b = color >> 16;
    g = color & 0x0000ff00 >> 8;
    r = color & 0x000000ff;
}

// 调试
#ifdef DEBUG
#define debug() MessageBoxA(nullptr, sum_string(__FUNCTION__, ":", __LINE__).c_str(), "DEBUG at", MB_OK | MB_ICONINFORMATION)
#define debugEx(x) MessageBoxA(nullptr, sum_string(__FUNCTION__, ":", __LINE__, "\r\n", x).c_str(), "DEBUG at", MB_OK | MB_ICONINFORMATION)
#else
#define debug()
#define debugEx(x)
#endif

#endif // CORNERSTONE_HEADER_UNTILS_INL_H_

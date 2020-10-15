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

#include "../sdk.h"

#include <string>

template <class>
struct stdcall_cast_s;

template <class RetT, class... ArgT>
struct stdcall_cast_s<RetT(ArgT...)>
{
    using type = RetT(__stdcall *)(ArgT...);
};

template <class FuncT>
using stdcall_cast = typename stdcall_cast_s<FuncT>::type;

#define pointer_or_zero(pointer) (((pointer) == nullptr) ? 0 : *(pointer))

/**
 * @brief 获取 API 返回的 JSON 文本中的返回码
 * @param retstr API 返回的 JSON 文本
 * @return 返回码
 */
inline std::int32_t get_retcode(std::string retstr)
{
    return (std::int32_t)(Json::parse(retstr)["retcode"]);
}

inline std::string to_string(const char value)
{
    return std::string(1, value);
}

inline std::string to_string(const char *value)
{
    return std::string(value);
}

inline std::string to_string(const std::string &value)
{
    return value;
}

inline std::string to_string(const wchar_t value)
{
    return WideCharToUTF8(std::wstring(1, value));
}

inline std::string to_string(const wchar_t *value)
{
    return WideCharToUTF8(value);
}

inline std::string to_string(const std::wstring &value)
{
    return WideCharToUTF8(value);
}

template <class T>
inline std::string to_string(T value)
{
    return std::to_string(value);
}

/**
 * @brief 将所有参数转换为字符串后依次连接
 * @return 连接后的字符串
 */
template <class... Types>
inline std::string sum_string(Types... args)
{
    return (... + to_string(args));
}

/**
 * @brief 将 RGB 颜色分量转换为易语言的颜色值
 * @param r R 颜色分量
 * @param g G 颜色分量
 * @param b B 颜色分量
 * @return 转换后的颜色值
 */
constexpr std::uint32_t make_color(std::uint8_t r, std::uint8_t g, std::uint8_t b)
{
    return r + (g << 8) + (b << 16);
}

/**
 * @brief 将易语言的颜色值转换为 RGB 颜色分量
 * @param color 待转换的颜色值
 * @param r R 颜色分量（传出）
 * @param g G 颜色分量（传出）
 * @param b B 颜色分量（传出）
 */
constexpr void read_color(std::uint32_t color, std::uint8_t &r, std::uint8_t &g, std::uint8_t &b)
{
    b = color >> 16;
    g = color & 0x0000ff00 >> 8;
    r = color & 0x000000ff;
}

// 报错
#define ErrorMessageBox(message, e) MessageBoxW(                                 \
    nullptr,                                                                     \
    UTF8ToWideChar(sum_string(message, "\r\n错误信息：\r\n", e.what())).c_str(), \
    UTF8ToWideChar("Cornerstone SDK 错误").c_str(),                              \
    MB_OK | MB_ICONERROR)

// 调试
#ifdef DEBUG
#define debug() MessageBoxW(                                               \
    nullptr,                                                               \
    L"",                                                                   \
    UTF8ToWideChar(sum_string("debug@", __func__, ":", __LINE__)).c_str(), \
    MB_OK | MB_ICONINFORMATION)
#define debugEx(x) MessageBoxW(                                              \
    nullptr,                                                                 \
    UTF8ToWideChar(sum_string(#x, " == ", (x))).c_str(),                     \
    UTF8ToWideChar(sum_string("debugEx@", __func__, ":", __LINE__)).c_str(), \
    MB_OK | MB_ICONINFORMATION)
#else
#define debug()
#define debugEx(x)
#endif

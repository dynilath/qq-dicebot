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

/**
 * @brief 将 UTF8 编码的字符串转换为宽字符串
 * @param utf8 待转换编码的 UTF8 编码的字符串
 * @return 转换后的宽字符串
 */
std::wstring UTF8ToWideChar(const std::string &utf8);

/**
 * @brief 将宽字符串转换为 UTF8 编码的字符串
 * @param wstr 待转换编码的宽字符串
 * @return 转换后的 UTF8 编码的字符串
 */
std::string WideCharToUTF8(const std::wstring &wstr);

/**
 * @brief 将 ANSI 编码的字符串转换为宽字符串
 * @param ansi 待转换编码的 ANSI 编码的字符串
 * @return 转换后的宽字符串
 */
std::wstring ANSIToWideChar(const std::string &ansi);

/**
 * @brief 将宽字符串转换为 ANSI 编码的字符串
 * @param wstr 待转换编码的宽字符串
 * @return 转换后的 ANSI 编码的字符串
 */
std::string WideCharToANSI(const std::wstring &wstr);

/**
 * @brief 将 ANSI 编码混杂 UCS2 编码的字符串转换为宽字符串
 * @param ansi_with_ucs2 待转换编码的 ANSI 编码混杂 UCS2 编码的字符串
 * @param not_unescape_wide_chars 不进行反转义的宽字符集
 * @return 转换后的宽字符串
 */
std::wstring ANSIWithUCS2ToWideChar(const std::string &ansi_with_ucs2, const std::wstring &not_unescape_wide_chars = L"[\\]");

/**
 * @brief 将宽字符串转换为 ANSI 编码混杂 UCS2 编码的字符串
 * @param wstr 待转换编码的宽字符串
 * @param force_escape_wchars 要强制转义的宽字符集
 * @return 转换后的 ANSI 编码混杂 UCS2 编码的字符串
 */
std::string WideCharToANSIWithUCS2(const std::wstring &wstr, const std::wstring &force_escape_wide_chars = L"");

/**
 * @brief 转义宽字符串
 * @param wstr 包含待转义的宽字符的宽字符串
 * @param escape_wchars 待转义的宽字符集
 * @return 转义后的宽字符串
 */
std::wstring EscapeWideChar(const std::wstring &wstr, const std::wstring &escape_wide_chars = L"[\\]");

/**
 * @brief 反转义宽字符串
 * @param wstr 待反转义的宽字符串
 * @param not_unescape_wide_chars 不进行反转义的宽字符集
 * @return 反转义后的宽字符串
 */
std::wstring UnescapeWideChar(const std::wstring &wstr_with_ucs2, const std::wstring &not_unescape_wide_chars = L"");

/**
 * @brief 转义 UTF8 编码的字符串
 * @param wstr 包含待转义的 UTF8 编码的字符串
 * @param escape_wchars 待转义的 UTF8 编码的字符集
 * @return 转义后的 UTF8 编码的字符串
 */
std::string EscapeUTF8(const std::string &utf8, const std::string &escape_utf8_chars = "[\\]");

/**
 * @brief 反转义 UTF8 编码的字符串
 * @param wstr 待反转义的 UTF8 编码的字符串
 * @param not_unescape_utf8_chars 不进行反转义的 UTF8 编码的字符集
 * @return 反转义后的 UTF8 编码的字符串
 */
std::string UnescapeUTF8(const std::string &utf8_with_ucs2, const std::string &not_unescape_utf8_chars = "");

#define e2s_s(ansi_with_ucs2) WideCharToUTF8(ANSIWithUCS2ToWideChar(ansi_with_ucs2))
#define s2e_s(utf8) WideCharToANSIWithUCS2(UTF8ToWideChar(utf8))
#define e2s(ansi_with_ucs2) e2s_s(ansi_with_ucs2).c_str()
#define s2e(utf8) s2e_s(utf8).c_str()

#define e2w_s(ansi) ANSIToWideChar(ansi_with_ucs2)
#define w2e_s(wstr) WideCharToANSI(wstr)
#define e2w(ansi) e2s_w_s(ansi).c_str()
#define w2e(wstr) s2e_w_s(wstr).c_str()

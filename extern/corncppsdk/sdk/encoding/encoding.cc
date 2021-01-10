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

#include "../sdk.h"

#include <string>
#include <cstring>

std::wstring ToWideChar(std::uint32_t code_page, const std::string &src_str)
{
    if (src_str.empty())
    {
        return L"";
    }
    auto len = MultiByteToWideChar(code_page, 0, src_str.c_str(), -1, nullptr, 0);
    if (!len)
    {
        return L"";
    }
    auto wstr_c = new wchar_t[len + 1];
    if (!wstr_c)
    {
        return L"";
    }
    memset(wstr_c, 0, len + 1);
    MultiByteToWideChar(code_page, 0, src_str.c_str(), -1, wstr_c, len);
    std::wstring wstr(wstr_c);
    delete[] wstr_c;
    return wstr;
}

std::string FromWideChar(std::uint32_t code_page, const std::wstring &src_wstr)
{
    if (src_wstr.empty())
    {
        return "";
    }
    auto len = WideCharToMultiByte(code_page, 0, src_wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (!len)
    {
        return "";
    }
    auto str_c = new char[len + 1];
    if (!str_c)
    {
        return "";
    }
    memset(str_c, 0, len + 1);
    WideCharToMultiByte(code_page, 0, src_wstr.c_str(), -1, str_c, len, nullptr, nullptr);
    std::string str(str_c);
    delete[] str_c;
    return str;
}

std::wstring UTF8ToWideChar(const std::string &utf8)
{
    return ToWideChar(CP_UTF8, utf8);
}

std::string WideCharToUTF8(const std::wstring &wstr)
{
    return FromWideChar(CP_UTF8, wstr);
}

std::wstring ANSIToWideChar(const std::string &ansi)
{
    return ToWideChar(CP_ACP, ansi);
}

std::string WideCharToANSI(const std::wstring &wstr)
{
    return FromWideChar(CP_ACP, wstr);
}

inline bool IsWideCharHex(wchar_t wch)
{
    return (wch >= L'0' && wch <= L'9') || (wch >= L'a' && wch <= L'f') || (wch >= L'A' && wch <= L'F');
}

std::wstring ANSIWithUCS2ToWideChar(const std::string &ansi_with_ucs2, const std::wstring &not_unescape_wide_chars)
{
    return UnescapeWideChar(ANSIToWideChar(ansi_with_ucs2), not_unescape_wide_chars);
}

inline std::uint8_t WideCharToHex(wchar_t wch)
{
    if (wch >= L'0' && wch <= L'9')
    {
        return wch - L'0';
    }
    else if (wch >= L'a' && wch <= L'f')
    {
        return wch - L'a' + 10;
    }
    else // wch >= L'A' && wch <= L'F'
    {
        return wch - L'A' + 10;
    }
}

inline char HexToChar(uint8_t hex)
{
    if (hex <= 9)
    {
        return '0' + hex;
    }
    else // hex >= 10 && hex <= 15
    {
        return 'a' + hex - 10;
    }
}

inline std::string WideCharToUCS2(wchar_t wch)
{
    auto wch_value = static_cast<uint16_t>(wch);
    std::string ucs2;
    ucs2.reserve(6);
    ucs2 += "\\u";
    ucs2 += HexToChar(wch_value >> 12);
    ucs2 += HexToChar((wch_value >> 8) & 0xf);
    ucs2 += HexToChar((wch_value >> 4) & 0xf);
    ucs2 += HexToChar(wch_value & 0xf);
    return ucs2;
}

std::string WideCharToANSIWithUCS2(const std::wstring &wstr, const std::wstring &force_escape_wide_chars)
{
    if (wstr.empty())
    {
        return "";
    }
    BOOL need_escape = FALSE;
    // 测试是否有需要强制转义的字符
    if (!force_escape_wide_chars.empty())
    {
        for (auto wch : wstr)
        {
            if (force_escape_wide_chars.find(wch) != std::wstring::npos)
            {
                need_escape = TRUE;
                break;
            }
        }
    }
    int len;
    // 测试是否能被ANSI编码
    if (need_escape == FALSE)
    {
        need_escape = FALSE;
        len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, &need_escape);
        if (!len)
        {
            return "";
        }
    }
    // 如果不需要转义
    if (!need_escape)
    {
        auto ansi_c = new char[len + 1];
        if (!ansi_c)
        {
            return "";
        }
        memset(ansi_c, 0, len + 1);
        WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, ansi_c, len, nullptr, nullptr);
        std::string ansi(ansi_c);
        delete[] ansi_c;
        return ansi;
    }
    else
    {
        auto wstr_c_tmp = new wchar_t[2];
        if (!wstr_c_tmp)
        {
            return "";
        }
        wstr_c_tmp[1] = L'\0';
        auto ansi_c_tmp = new char[8]; // 3应该够用了但是保险点还是分配个8
        if (!ansi_c_tmp)
        {
            delete[] wstr_c_tmp;
            return "";
        }
        std::string ansi_with_ucs2;
        // 遍历字符，选择不同的方案
        for (auto it : wstr)
        {
            // 如果字符不需要强制转义
            if (force_escape_wide_chars.find(it) == std::wstring::npos)
            {
                // 如果字符是ASCII字符
                if (static_cast<uint16_t>(it) <= 0x7f)
                {
                    ansi_with_ucs2 += static_cast<char>(it);
                    continue;
                }
                // 如果字符不是代理（在基础平面上）
                else if ((uint16_t)(it) >> 11 != 0b11011)
                {
                    need_escape = FALSE;
                    wstr_c_tmp[0] = it;
                    auto len = WideCharToMultiByte(CP_ACP, 0, wstr_c_tmp, -1, nullptr, 0, nullptr, &need_escape);
                    if (!len)
                    {
                        delete[] wstr_c_tmp;
                        delete[] ansi_c_tmp;
                        return "";
                    }
                    // 如果能够用ANSI编码
                    if (!need_escape)
                    {
                        memset(ansi_c_tmp, 0, len + 1);
                        WideCharToMultiByte(CP_ACP, 0, wstr_c_tmp, -1, ansi_c_tmp, len, nullptr, nullptr);
                        ansi_with_ucs2 += ansi_c_tmp;
                        continue;
                    }
                }
            }
            // 否则字符需要转义
            ansi_with_ucs2 += WideCharToUCS2(it);
        }
        delete[] wstr_c_tmp;
        delete[] ansi_c_tmp;
        return ansi_with_ucs2;
    }
}

inline char HexToWideChar(uint8_t hex)
{
    if (hex <= 9)
    {
        return L'0' + hex;
    }
    else // hex >= 10 && hex <= 15
    {
        return L'a' + hex - 10;
    }
}

inline std::wstring WideCharToWideCharUCS2(wchar_t wch)
{
    auto wch_value = static_cast<uint16_t>(wch);
    std::wstring ucs2;
    ucs2.reserve(6);
    ucs2 += L"\\u";
    ucs2 += HexToWideChar(wch_value >> 12);
    ucs2 += HexToWideChar((wch_value >> 8) & 0xf);
    ucs2 += HexToWideChar((wch_value >> 4) & 0xf);
    ucs2 += HexToWideChar(wch_value & 0xf);
    return ucs2;
}

std::wstring EscapeWideChar(const std::wstring &wstr, const std::wstring &escape_wide_chars)
{
    if (wstr.empty())
    {
        return L"";
    }
    if (escape_wide_chars.empty())
    {
        return wstr;
    }
    std::wstring wstr_with_ucs2;
    for (wchar_t wch : wstr)
    {
        if (escape_wide_chars.find(wch) == std::wstring::npos)
        {
            wstr_with_ucs2 += wch;
        }
        else
        {
            wstr_with_ucs2 += WideCharToWideCharUCS2(wch);
        }
    }
    return wstr_with_ucs2;
}

wchar_t UCS2ToWideChar(const std::wstring &ucs2)
{
    auto it = ucs2.cbegin();
    it += 2; // L'\\u'
    uint16_t wch_value = 0;
    for (auto i = 0; i < 4; i++)
    {
        wch_value <<= 4;
        wch_value += WideCharToHex(*it);
        it++;
    }
    auto wch = static_cast<wchar_t>(wch_value);
    return wch;
}

std::wstring UnescapeWideChar(const std::wstring &wstr_with_ucs2, const std::wstring &not_unescape_wide_chars)
{
    if (wstr_with_ucs2.empty())
    {
        return L"";
    }
    std::wstring wstr;
    wstr.reserve(wstr_with_ucs2.size());
    std::wstring ucs2_tmp;
    ucs2_tmp.reserve(6);
    for (auto it = wstr_with_ucs2.cbegin(), cend = wstr_with_ucs2.cend(); it != cend; it++)
    {
        if (*it == L'\\')
        {
            ucs2_tmp = L"\\";
            it++;
            if (it != cend && *it == L'u')
            {
                ucs2_tmp += L'u';
                it++;
                if (it != cend && IsWideCharHex(*it))
                {
                    ucs2_tmp += *it;
                    it++;
                    if (it != cend && IsWideCharHex(*it))
                    {
                        ucs2_tmp += *it;
                        it++;
                        if (it != cend && IsWideCharHex(*it))
                        {
                            ucs2_tmp += *it;
                            it++;
                            if (it != cend && IsWideCharHex(*it))
                            {
                                ucs2_tmp += *it;
                                auto wch = UCS2ToWideChar(ucs2_tmp);
                                if (not_unescape_wide_chars.find(wch) == std::wstring::npos)
                                {
                                    wstr += wch;
                                    continue;
                                }
                                it++;
                            }
                        }
                    }
                }
            }
            wstr += ucs2_tmp;
            if (it == cend)
            {
                break;
            }
        }
        wstr += *it;
    }
    return wstr;
}

std::string EscapeUTF8(const std::string &utf8, const std::string &escape_utf8_chars)
{
    return WideCharToUTF8(EscapeWideChar(UTF8ToWideChar(utf8), UTF8ToWideChar(escape_utf8_chars)));
}

std::string UnescapeUTF8(const std::string &utf8_with_ucs2, const std::string &not_unescape_utf8_chars)
{
    return WideCharToUTF8(UnescapeWideChar(UTF8ToWideChar(utf8_with_ucs2), UTF8ToWideChar(not_unescape_utf8_chars)));
}

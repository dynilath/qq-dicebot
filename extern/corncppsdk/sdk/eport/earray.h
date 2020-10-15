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

#include <algorithm>
#include <vector>
#include <type_traits>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "./etypes.h"

struct earray_head
{
    void *data;
    HANDLE heap;

    earray_head() noexcept
    {
        this->heap = GetProcessHeap();
        this->data = HeapAlloc(heap, HEAP_ZERO_MEMORY, 1);
        reinterpret_cast<std::uint8_t *>(this->data)[0] = 1;
    }

    ~earray_head() noexcept
    {
        HeapFree(this->heap, 0, this->data);
    }

    operator void **()
    {
        return &this->data;
    }
};

template <typename EType, typename CPPType>
size_t earray1d2vector(const earray_head &earr, ::std::vector<CPPType> &out)
{
    auto srcptr = reinterpret_cast<eint *>(earr.data);
    auto dim = srcptr[0];
    if (dim != 1)
        return static_cast<size_t>(-1);
    auto size = srcptr[1];
    if constexpr (::std::is_compound<CPPType>::value)
    {
        auto pptr = reinterpret_cast<EType **>(srcptr + 2);
        out.clear();
        out.reserve(size);
        ::std::for_each(pptr, pptr + size, [&](auto ptr) {
            out.emplace_back(*ptr);
        });
    }
    else
    {
        auto ptr = reinterpret_cast<EType *>(srcptr + 2);
        out.clear();
        out.reserve(size);
        ::std::for_each(ptr, ptr + size, [&](auto val) {
            out.emplace_back(val);
        });
    }
    return size;
}

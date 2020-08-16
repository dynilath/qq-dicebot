/*
Cornerstone SDK
-- Corn SDK for Modern C++

Licensed under the MIT License
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

class Unpacker
{
public:
    Unpacker(const void *data){
        this->data = (uint8_t *)data;
    }

    template <class RetT>
    RetT& get(){
        auto& ret = *((RetT*)data);
        data += sizeof(RetT);
        return ret;
    }
private:
    uint8_t *data;
};

class earray1D
{
public:
    /**
     * @brief 解包易语言数组（成员为简单类型）
     */
    template <class ESimpleT>
    static size_t Unpack(const void* data, vector<ESimpleT>& array)
    {
        auto unpacker = Unpacker(data);
        auto dimension = unpacker.get<eint>();  // 数组维度 
        if (dimension != 1)  // 只考虑1维的情况
        {
            return -1;
        }
        auto size = unpacker.get<eint>();
        array.clear();
        for (auto i = 0; i < size; i++)
        {
            array.push_back(unpacker.get<ESimpleT>());
        }
        return size;
    }

    /**
     * @brief 解包易语言数组（成员为自定义结构）
     */
    template <class EStructT>
    static size_t UnpackStruct(const void* data, vector<EStructT>& array)
    {
        auto unpacker = Unpacker(data);
        auto dimension = unpacker.get<eint>();  // 数组维度 
        if (dimension != 1)  // 只考虑1维的情况
        {
            return -1;
        }
        auto size = unpacker.get<eint>();
        array.clear();
        for (auto i = 0; i < size; i++)
        {
            volatile EStructT* tmp = unpacker.get<volatile EStructT*>();
            EStructT tmp_ = EStructT((const EStructT &)*tmp);
            array.push_back(tmp_);
        }
        return size;
    }
};

template <class FuncT>
inline FuncT* _f(Json api, const char* name)
{
    return (FuncT*)((uintptr_t)api[name]);
}

#endif  // CORNERSTONE_HEADER_UNTILS_INL_H_

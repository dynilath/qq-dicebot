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

#include "sdk.h"

#include "../config.h"
#include <unordered_set>

API *api;

std::string plugin_info;

#pragma comment(linker,"/EXPORT:apprun=_apprun@8")
extern "C" etext __stdcall apprun(etext apidata, etext pluginkey)
{
    // 创建全局API对象
    api = new API(apidata, pluginkey);
    try
    {
        // 解析插件配置
        auto config = Json::parse(Configuration);
        Json json_info =
            {{"appname", config["插件名称"]},
             {"author", config["插件作者"]},
             {"appv", config["插件版本"]},
             {"describe", config["插件说明"]},
             {"sdkv", "2.6.5"},
             {"friendmsaddres", (uintptr_t)&OnPrivateMessage},
             {"groupmsaddres", (uintptr_t)&OnGroupMessage},
             {"unitproaddres", (uintptr_t)&OnUninstall},
             {"setproaddres", (uintptr_t)&OnSettings},
             {"useproaddres", (uintptr_t)&OnEnabled},
             {"banproaddres", (uintptr_t)&OnDisabled},
             {"eventmsaddres", (uintptr_t)&OnEvent}};
        const std::unordered_set<std::string> dangerous_api =
            {
                "QQ点赞",
                "获取clientkey",
                "获取pskey",
                "获取skey",
                "解散群",
                "删除好友",
                "退群",
                "置屏蔽好友",
                "修改个性签名",
                "修改昵称",
                "上传头像",
                "框架重启",
                "取QQ钱包个人信息",
                "更改群聊消息内容",
                "更改私聊消息内容"};
        for (auto &it : config["所需权限"].items())
        {
            auto is_safe = "1";                    // 是否是安全的权限
            if (dangerous_api.count(it.key()) == 1) // 如果 == 1 就算找到
            {
                is_safe = "0";
            }

            json_info["data"]["needapilist"][it.key()] =
                Json({{"state", is_safe},
                      {"safe", is_safe},
                      {"desc", it.value()}});
        }

        // 将插件信息提交给框架
        auto info_utf8 = json_info.dump();
        plugin_info = s2e_s(info_utf8);
        return plugin_info.c_str();
    }
    catch (Json::exception e)
    {
        MessageBoxA(nullptr,
                    s2e(sum_string("插件信息解析失败，请检查config.h\r\n错误信息：\r\n", e.what())),
                    s2e("Cornerstone SDK 错误"),
                    MB_OK | MB_ICONERROR);
        return "{}";
    }
}

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

#include "sdk.h"

#include <cstring>
#include <unordered_set>
// #include <type_traits>
#include <exception>

API *api;

template <typename VData_t>
inline auto deref_and_remove_volatile(VData_t *data)
{
    using data_t = typename ::std::remove_volatile_t<VData_t>;
    using p_data_t = typename ::std::add_pointer_t<data_t>;
    return *const_cast<p_data_t>(data);
}

// 私聊消息事件回调包装
EventProcessEnum ECallBack_OnPrivateMessage(volatile _EType_PrivateMessageData *eData)
{
    try
    {
        return OnPrivateMessage(deref_and_remove_volatile(eData));
    }
    catch (std::exception e)
    {
        ErrorMessageBox("处理私聊消息事件时出现异常", e);
        return EventProcessEnum::Ignore;
    }
}

// 群消息事件回调包装
EventProcessEnum ECallBack_OnGroupMessage(volatile _EType_GroupMessageData *eData)
{
    try
    {
        return OnGroupMessage(deref_and_remove_volatile(eData));
    }
    catch (std::exception e)
    {
        ErrorMessageBox("处理群消息事件时出现异常", e);
        return EventProcessEnum::Ignore;
    }
}

// 插件卸载事件回调包装（未知参数）
EventProcessEnum ECallBack_OnUninstall(void *)
{
    try
    {
        auto ret = OnUninstall();
        // 释放全局api对象避免内存泄漏
        delete api;
        return ret;
    }
    catch (std::exception e)
    {
        ErrorMessageBox("处理插件卸载事件时出现异常", e);
        return EventProcessEnum::Ignore;
    }
}

// 插件设置事件回调包装（未知参数）
EventProcessEnum ECallBack_OnSettings(void *)
{
    try
    {
        return OnSettings();
    }
    catch (std::exception e)
    {
        ErrorMessageBox("处理插件设置事件时出现异常", e);
        return EventProcessEnum::Ignore;
    }
}

// 插件被启用事件回调包装（未知参数）
EventProcessEnum ECallBack_OnEnabled(void *)
{
    try
    {
        return OnEnabled();
    }
    catch (std::exception e)
    {
        ErrorMessageBox("处理插件被启用事件时出现异常", e);
        return EventProcessEnum::Ignore;
    }
}

// 插件被禁用事件回调包装（未知参数）
EventProcessEnum ECallBack_OnDisabled(void *)
{
    try
    {
        return OnDisabled();
    }
    catch (std::exception e)
    {
        ErrorMessageBox("处理插件被禁用事件时出现异常", e);
        return EventProcessEnum::Ignore;
    }
}

// 其他事件回调包装
EventProcessEnum ECallBack_OnEvent(volatile _EType_EventData *eData)
{
    try
    {
        return OnEvent(deref_and_remove_volatile(eData));
    }
    catch (std::exception e)
    {
        ErrorMessageBox("处理其他事件时出现异常", e);
        return EventProcessEnum::Ignore;
    }
}

const extern char *Configuration;

std::string plugin_info;

extern "C" etext __stdcall apprun(etext apidata, etext pluginkey)
{
    try
    {
        // 创建全局API对象
        api = new API(apidata, pluginkey);
        // 解析插件配置
        auto config = Json::parse(Configuration);
        Json json_info =
            {{"appname", config["插件名称"]},
             {"author", config["插件作者"]},
             {"appv", config["插件版本"]},
             {"describe", config["插件说明"]},
             {"sdkv", "2.7.1"},
             {"friendmsaddres", (uintptr_t)&ECallBack_OnPrivateMessage},
             {"groupmsaddres", (uintptr_t)&ECallBack_OnGroupMessage},
             {"unitproaddres", (uintptr_t)&ECallBack_OnUninstall},
             {"setproaddres", (uintptr_t)&ECallBack_OnSettings},
             {"useproaddres", (uintptr_t)&ECallBack_OnEnabled},
             {"banproaddres", (uintptr_t)&ECallBack_OnDisabled},
             {"eventmsaddres", (uintptr_t)&ECallBack_OnEvent}};
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
            auto is_safe = "1";                     // 是否是安全的权限
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
        // auto info = s2e_s(json_info.dump());
        // auto size = info.size();
        // auto cstr = new char[size + 1]; // 如果直接返回c_str()，有时会崩溃
        // std::memcpy(cstr, info.c_str(), size + 1);
        // cstr[size] = '\0'; // 确保没事
        // return cstr;       // 不知道易语言会不会回收这个内存？
    }
    catch (Json::exception e)
    {
        ErrorMessageBox("解析插件信息时出现异常，请检查 config.h", e);
        return "{}";
    }
    catch (std::exception e)
    {
        ErrorMessageBox("加载插件时出现异常", e);
        return "{}";
    }
}

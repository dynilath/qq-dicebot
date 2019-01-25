#include "cqsdk/cqsdk.h"
#include "cqsdk/dir.h"

#include "dicebot/dicebot.h"

namespace cqapp = cq::app; // 插件本身的生命周期事件和管理
namespace cqevent = cq::event; // 用于注册 QQ 相关的事件处理函数
namespace api = cq::api; // 用于调用酷 Q 提供的接口
namespace cqlogging = cq::logging; // 用于日志
namespace cqmessage = cq::message; // 提供封装了的 Message 等类

// 初始化 App ID
CQ_INITIALIZE("com.dynilath.coolqdicebot");

std::string get_group_nickname(uint64_t const group_id, uint64_t const user_id);
std::string get_nickname(uint64_t const user_id);

// 插件入口，在静态成员初始化之后，app::on_initialize 事件发生之前被执行，用于配置 SDK 和注册事件回调
CQ_MAIN {
    cq::config.convert_unicode_emoji = true; // 配置 SDK 自动转换 Emoji 到 Unicode（默认就是 true）

    cqapp::on_enable = [] {
        // cqlogging、api、dir 等命名空间下的函数只能在事件回调函数内部调用，而不能直接在 CQ_MAIN 中调用
        std::string dir = cq::dir::app();
        dicebot::initialize(dir);
        dicebot::set_logger(cqlogging::debug);
        cqlogging::debug(u8"启用", u8"插件已启动");
    };

    cqapp::on_disable = []{
        dicebot::salvage();
    };

    cqevent::on_private_msg = [](const cq::PrivateMessageEvent &e) {
        try{
            dicebot::event_info ei(e.user_id);
            if(!dicebot::try_fill_nickname(ei)){
                ei.nickname = get_nickname(e.user_id);
            }

            std::string output;
            if(dicebot::message_pipeline(e.raw_message,ei,output)){
                cq::Message message;
                message.push_back(cqmessage::MessageSegment::text(output));
                message.send(e.target);

            }else{
                cqlogging::debug(u8"DICE", u8"调用失败，无法产生结果");
            }
        }
        catch (const cq::exception::ApiError &err){
            cqlogging::debug(u8"DICE", u8"调用失败，错误码：" + std::to_string(err.code));
        }
        catch(const std::exception &err){
            cqlogging::warning(u8"DICE", u8"发生std exception：" + std::to_string(err.what()) + u8"\n发送消息为：" + e.raw_message);
        }
        catch (...){
            cqlogging::warning(u8"DICE", u8"发生无法捕捉的异常，发送消息为：" + e.raw_message);
        }
        e.block(); 
    }; 

    cqevent::on_group_msg = [](const cq::GroupMessageEvent &e ) { 
        try {
            dicebot::event_info ei(e.user_id,e.group_id,dicebot::event_type::group);
            if(!dicebot::try_fill_nickname(ei)){
                ei.nickname = get_group_nickname(e.group_id,e.user_id);
            }

            std::string output;
            if(dicebot::message_pipeline(e.raw_message, ei, output)){
                cq::Message message;
                message.push_back(cqmessage::MessageSegment::text(output));
                message.send(e.target);
            }else{
                cqlogging::debug(u8"DICE", u8"调用失败，无法产生结果");
            }
        } catch (const cq::exception::ApiError &err) {
            cqlogging::debug(u8"DICE", u8"调用失败，错误码：" + std::to_string(err.code));
        }
        catch(const std::exception &err){
            cqlogging::warning(u8"DICE", u8"发生std exception：" + std::to_string(err.what()) + u8"\n发送消息为：" + e.raw_message);
        }
        catch (...){
            cqlogging::warning(u8"DICE", u8"发生无法捕捉的异常，发送消息为：" + e.raw_message);
        }
        e.block();
    };

    cqevent::on_discuss_msg = [](const cq::DiscussMessageEvent &e ) {
        try {
            dicebot::event_info ei(e.user_id,e.discuss_id,dicebot::event_type::discuss);
            if(!dicebot::try_fill_nickname(ei)){
                ei.nickname = get_nickname(e.user_id);
            }

            std::string output;
            if(dicebot::message_pipeline(e.raw_message, ei, output)){
                cq::Message message;
                message.push_back(cqmessage::MessageSegment::text(output));
                message.send(e.target);

            }else{
                cqlogging::debug(u8"DICE", u8"调用失败，无法产生结果");
            }
        } catch (const cq::exception::ApiError &err) {
            cqlogging::debug(u8"DICE", u8"调用失败，错误码：" + std::to_string(err.code));
        }
        catch(const std::exception &err){
            cqlogging::warning(u8"DICE", u8"发生std exception：" + std::to_string(err.what()) + u8"\n发送消息为：" + e.raw_message);
        }
        catch (...){
            cqlogging::warning(u8"DICE", u8"发生无法捕捉的异常，发送消息为：" + e.raw_message);
        }
        e.block();
    };
}

std::string get_group_nickname(uint64_t const group_id, uint64_t const user_id){
    cq::GroupMember member = cq::api::get_group_member_info(group_id,user_id);
    return member.nickname;
}

std::string get_nickname(uint64_t const user_id){
    cq::User user = cq::api::get_stranger_info(user_id);
    return user.nickname;
}
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
        try {
            std::string output;
            if(dicebot::group_message_pipeline(e.raw_message, 1000,e.user_id,false,output)){
                cq::Message message;
                message.push_back(cqmessage::MessageSegment::text(output));
                message.send(e.target);

            }else{
                cqlogging::debug(u8"API", u8"调用失败，无法产生结果");
            }
            //api::send_private_msg(e.user_id, message); // echo 回去

            //api::send_msg(e.target, e.message); // 使用 e.target 指定发送目标

            // MessageSegment 类提供一些静态成员函数以快速构造消息段
            //cq::Message msg = cq::MessageSegment::contact(cq::MessageSegment::ContactType::GROUP, 201865589);
            //msg.send(e.target); // 使用 Message 类的 send 成员函数
        } catch (const cq::exception::ApiError &err) {
            // API 调用失败
            cqlogging::debug(u8"API", u8"调用失败，错误码：" + std::to_string(err.code));
        }

        e.block(); // 阻止事件继续传递给其它插件
    }; 

    cqevent::on_group_msg = [](const cq::GroupMessageEvent &e ) { 
        try {
            std::string output;
            if(dicebot::group_message_pipeline(e.raw_message, e.group_id,e.user_id,false,output)){
                cq::Message message;
                message.push_back(cqmessage::MessageSegment::text(output));
                message.send(e.target);

            }else{
                cqlogging::debug(u8"API", u8"调用失败，无法产生结果");
            }
        } catch (const cq::exception::ApiError &err) {
            // API 调用失败
            cqlogging::debug(u8"API", u8"调用失败，错误码：" + std::to_string(err.code));
        }
    };

}

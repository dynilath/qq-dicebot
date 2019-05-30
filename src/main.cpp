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

bool get_group_nickname(dicebot::event_info &ei, uint64_t const group_id, uint64_t const user_id);
bool get_nickname(dicebot::event_info &ei, uint64_t const user_id);
void resolve_cap(dicebot::event_info &ei, std::string const &raw_source, cq::Target target) noexcept;

// 插件入口，在静态成员初始化之后，app::on_initialize 事件发生之前被执行，用于配置 SDK 和注册事件回调
CQ_MAIN {
    cq::config.convert_unicode_emoji = true; // 配置 SDK 自动转换 Emoji 到 Unicode（默认就是 true）

    cqapp::on_enable = [] {
        // cqlogging、api、dir 等命名空间下的函数只能在事件回调函数内部调用，而不能直接在 CQ_MAIN 中调用
        std::string dir = cq::dir::app();
        dicebot::initialize(dir.c_str());
        dicebot::set_logger(cqlogging::debug);
        cqlogging::debug(u8"启用", u8"插件已启动");
    };

    cqapp::on_disable = [] { dicebot::salvage(); };

    cqevent::on_private_msg = [](const cq::PrivateMessageEvent &e) {
        dicebot::event_info ei(e.user_id);
        if (!dicebot::try_fill_nickname(ei)) {
            if (!get_nickname(ei, e.user_id)) return;
        }
        resolve_cap(ei, e.raw_message, e.target);
        e.block();
    };

    cqevent::on_group_msg = [](const cq::GroupMessageEvent &e) {
        dicebot::event_info ei(e.user_id, e.group_id, dicebot::event_type::group);
        if (!dicebot::try_fill_nickname(ei)) {
            if (!get_group_nickname(ei, e.group_id, e.user_id)) return;
        }
        resolve_cap(ei, e.raw_message, e.target);
        e.block();
    };

    cqevent::on_discuss_msg = [](const cq::DiscussMessageEvent &e) {
        dicebot::event_info ei(e.user_id, e.discuss_id, dicebot::event_type::discuss);
        if (!dicebot::try_fill_nickname(ei)) {
            if (!get_nickname(ei, e.user_id)) return;
        }
        resolve_cap(ei, e.raw_message, e.target);
        e.block();
    };
}

bool get_group_nickname(dicebot::event_info &ei, uint64_t const group_id, uint64_t const user_id) {
    try {
        ei.nickname = cq::api::get_group_member_info(group_id, user_id).nickname;
        return true;
    } catch (...) {
#ifdef _DEBUG
        cqlogging::debug(u8"DICE", u8"无法获取昵称：" + std::to_string(user_id));
#endif
        return false;
    }
}

bool get_nickname(dicebot::event_info &ei, uint64_t const user_id) {
    try {
        ei.nickname = cq::api::get_stranger_info(user_id).nickname;
        return true;
    } catch (...) {
#ifdef _DEBUG
        cqlogging::debug(u8"DICE", u8"无法获取昵称：" + std::to_string(user_id));
#endif
        return false;
    }
}

void resolve_cap(dicebot::event_info &ei, std::string const &raw_source, cq::Target target) noexcept {
    try {
        cq::Message msg;

        std::string temp;
        if (!dicebot::message_pipeline(raw_source, ei, temp)) return;

        std::regex const reg_cq_code("\\[CQ:image,file=([A-Z,0-9]{32}\\.(?:jpg|png|gif))\\]|\\[CQ:face,id=([0-9]+)\\]");
        std::smatch m;
        std::regex_search(temp, m, reg_cq_code);
        while (!m.empty()) {
            msg.push_back(cqmessage::MessageSegment::text(m.prefix()));
            if (m[1].matched)
                msg.push_back(cqmessage::MessageSegment::image(m[1]));
            else if (m[2].matched)
                msg.push_back(cqmessage::MessageSegment::face(std::stoi(m[2])));
            temp = m.suffix();
            std::regex_search(temp, m, reg_cq_code);
        }
        msg.push_back(cqmessage::MessageSegment::text(temp));
        msg.send(target);
    } catch (const cq::exception::ApiError &err) {
#ifdef _DEBUG
        cqlogging::debug(u8"DICE", u8"调用失败，错误码：" + std::to_string(err.code));
#endif
    } catch (const std::exception &err) {
        cqlogging::warning(u8"DICE", u8"发生std exception：" + std::to_string(err.what()) + u8"\n发送消息为：" + raw_source);
    }
}
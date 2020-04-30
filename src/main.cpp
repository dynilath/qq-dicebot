#include "./cqtool.h"
#include "cqcppsdk/cqcppsdk.h"
#include "dicebot/dicebot.h"

namespace cqlogging = cq::logging; // 用于日志
namespace cqmessage = cq::message; // 提供封装了的 Message 等类

void resolve_cap(event_info &ei, std::string const &raw_source,
                 cq::Target target) noexcept;

// 插件入口，在静态成员初始化之后，app::on_initialize
// 事件发生之前被执行，用于配置 SDK 和注册事件回调
CQ_INIT {
    cq::on_enable([] {
        std::string dir = cq::dir::app();
        dicebot::initialize(dir.c_str());
        dicebot::set_logger(cqlogging::debug);
        cqlogging::debug(u8"启用", u8"插件已启动");
    });

    cq::on_disable([] { dicebot::salvage(); });

    cq::on_private_message([](const cq::MessageEvent &e) {
        if (!dicebot::utils::basic_event_filter(e.message)) return;
        ::event_info ei(e.user_id);
        if (!dicebot::try_fill_nickname(ei)) {
            if (!get_nickname(ei, e.user_id)) return;
        }
        resolve_cap(ei, e.message, e.target);
    });

    cq::on_group_message([](const cq::GroupMessageEvent &e) {
        if (!dicebot::utils::basic_event_filter(e.message)) return;
        ::event_info ei(e.user_id, e.group_id, ::event_type::group);
        if (!dicebot::try_fill_nickname(ei)) {
            if (!get_group_nickname(ei, e.group_id, e.user_id)) return;
        }
        resolve_cap(ei, e.message, e.target);
    });

    cq::on_discuss_message([](const cq::DiscussMessageEvent &e) {
        cq::message::Message msg = e.message;
        for (auto &seg : msg) {
            if (seg.type == "at") {
                int64_t uid = ::std::stoll(seg.data["qq"]);
            }
        }

        if (!dicebot::utils::basic_event_filter(e.message)) return;
        event_info ei(e.user_id, e.discuss_id, event_type::group);
        if (!dicebot::try_fill_nickname(ei)) {
            if (!get_nickname(ei, e.user_id)) return;
        }
        resolve_cap(ei, e.message, e.target);
    });
}

void resolve_cap(event_info &ei, std::string const &raw_source,
                 cq::Target target) noexcept {
    try {
        std::string temp;
        if (!dicebot::message_pipeline(raw_source, ei, temp)) return;
        cq::send_message(target, temp);
#ifdef _DEBUG
    } catch (const cq::ApiError &err) {
        cqlogging::debug(u8"DICE",
                         u8"调用失败，错误码：" + std::to_string(err.code));
#else
    } catch (const cq::ApiError &) {
#endif
    } catch (const std::exception &err) {
        cqlogging::warning(u8"DICE",
                           u8"发生std exception：" + std::string(err.what())
                               + u8"\n发送消息为：" + raw_source);
    }
}
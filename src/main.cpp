#include <cqcppsdk/cqcppsdk.h>

#include "dicebot/dicebot.h"

namespace cqlogging = cq::logging; // 用于日志
namespace cqmessage = cq::message; // 提供封装了的 Message 等类

bool get_group_nickname(dicebot::event_info &ei, uint64_t const group_id, uint64_t const user_id);
bool get_nickname(dicebot::event_info &ei, uint64_t const user_id);
void resolve_cap(dicebot::event_info &ei, std::string const &raw_source, cq::Target target) noexcept;

// 插件入口，在静态成员初始化之后，app::on_initialize 事件发生之前被执行，用于配置 SDK 和注册事件回调
CQ_INIT {
    cq::on_enable([] {
        std::string dir = cq::dir::app();
        dicebot::initialize(dir.c_str());
        dicebot::set_logger(cqlogging::debug);
        cqlogging::debug(u8"启用", u8"插件已启动");
    });

    cq::on_disable([] { dicebot::salvage(); });

    cq::on_private_message([](const cq::MessageEvent &e) {
        dicebot::event_info ei(e.user_id);
        if (!dicebot::try_fill_nickname(ei)) {
            if (!get_nickname(ei, e.user_id)) return;
        }
        resolve_cap(ei, e.message, e.target);
    });

    cq::on_group_message([](const cq::GroupMessageEvent &e) {
        dicebot::event_info ei(e.user_id, e.group_id, dicebot::event_type::group);
        if (!dicebot::try_fill_nickname(ei)) {
            if (!get_group_nickname(ei, e.group_id, e.user_id)) return;
        }
        resolve_cap(ei, e.message, e.target);
    });
}

bool get_group_nickname(dicebot::event_info &ei, uint64_t const group_id, uint64_t const user_id) {
    try {
        ei.nickname = cq::get_group_member_info(group_id, user_id).nickname;
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
        ei.nickname = cq::get_stranger_info(user_id).nickname;
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
        cq::message::Message msg;

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
    } catch (const cq::ApiError &err) {
#ifdef _DEBUG
        cqlogging::debug(u8"DICE", u8"调用失败，错误码：" + std::to_string(err.code));
#endif
    } catch (const std::exception &err) {
        cqlogging::warning(u8"DICE", u8"发生std exception：" + std::to_string(err.what()) + u8"\n发送消息为：" + raw_source);
    }
}
#pragma once
#include <cstdint>
#include "./constants.h"

namespace dicebot {
    enum class event_type { group, discuss, pmessage };

    class event_info {
    public:
        int64_t user_id;
        int64_t group_id;
        event_type type;
        std::string nickname;

        event_info(int64_t id, int64_t group, event_type ev) {
            this->user_id = id;
            this->group_id = group;
            this->type = ev;
        }

        event_info(int64_t id) {
            this->user_id = id;
            this->group_id = 0;
            this->type = event_type::pmessage;
        }

        const auto pair() const {
            return std::pair<int64_t, int64_t>(user_id, group_id);
        };
    };
} // namespace dicebot
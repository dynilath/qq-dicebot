#pragma once

#include <unordered_map>

#include <memory>

#include "../event_info.h"
#include "../utils/pair_hash.h"
namespace dicebot::nickname {
    class nickname_manager {
        using nick_map_t =
            std::unordered_map<utils::pair_t, std::string, utils::pair_hash>;
        using nick_pair_t = nick_map_t::value_type;
        using nick_key_t = nick_map_t::key_type;

        nick_map_t nick_map;

    public:
        static std::unique_ptr<nickname_manager> instance;
        static nickname_manager* create_instance();
        static void destroy_instance();

        bool get_nickname(event_info const& event, std::string& nickname);
        void set_nickname(event_info const& event);
    };

} // namespace dicebot::nickname

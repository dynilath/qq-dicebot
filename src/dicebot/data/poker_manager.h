#pragma once
#include <memory>
#include <unordered_map>

#include "../event_info.h"
#include "../entity/poker.h"
#include "../utils/pair_hash.h"

namespace dicebot::poker {
    class poker_manager {
    private:
        using poker_map_t = std::unordered_map<int64_t, poker_deck>;
        static std::unique_ptr<poker_manager> instance;
        poker_map_t private_poker_map;
        poker_map_t group_poker_map;

    public:
        static poker_manager* get_instance() noexcept;
        static poker_manager* create_instance() noexcept;
        static void destroy_instance() noexcept;

        poker_deck& get_deck(const event_info&);

        void sync_database(const event_info&) const;
        void sync_database_draw(const event_info&) const;
    };
} // namespace dicebot::poker

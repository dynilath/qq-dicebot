#pragma once
#include <memory>
#include <unordered_map>
#include "../entity/poker.h"

namespace dicebot::poker {
    class poker_manager {
        static std::unique_ptr<poker_manager> instance;
        std::unordered_map<int64_t, poker_deck> poker_map;

    public:
        static poker_manager* get_instance() noexcept;
        static poker_manager* create_instance() noexcept;
        static void destroy_instance() noexcept;

        poker_deck& get_deck(const int64_t);
    };
} // namespace dicebot::poker

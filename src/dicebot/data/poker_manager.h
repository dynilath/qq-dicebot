#pragma once
#include <memory>
#include <unordered_map>
#include "../entity/poker.h"
#include "../utils/pair_hash.h"

namespace dicebot::poker {
    class poker_manager {
    private:
        using poker_map_t = std::unordered_map<utils::pair_t, poker_deck, utils::pair_hash>;
        static std::unique_ptr<poker_manager> instance;
        poker_map_t poker_map;

    public:
        static poker_manager* get_instance() noexcept;
        static poker_manager* create_instance() noexcept;
        static void destroy_instance() noexcept;

        poker_deck& get_deck(const utils::pair_t&);

        void sync_database(const utils::pair_t&) const;
        void sync_database_draw(const utils::pair_t&) const;
    };
} // namespace dicebot::poker

#pragma once

#include <memory>
#include "../entity/manual_dice.h"
#include "../event_info.h"
#include "../utils/pair_hash.h"

namespace dicebot::manual {
    class manual_dice_control {
    private:
        using md_map_t =
            std::unordered_map<utils::pair_t, manual_dice, utils::pair_hash>;

        static std::unique_ptr<manual_dice_control> instance;
        md_map_t manual_dice_map;

    public:
        static manual_dice_control *get_instance();
        static manual_dice_control *create_instance();
        static void destroy_instance();
        md_map_t::iterator find_manual_dice(const event_info &);
        void sync_database(const md_map_t::iterator) const;
    };
} // namespace dicebot::manual

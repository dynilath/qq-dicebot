#pragma once

#include <stdint.h>
#include <deque>
#include <list>
#include <string>
#include <vector>

namespace dicebot::poker {

    enum poker_type : uint8_t {
        heart_2,
        heart_3,
        heart_4,
        heart_5,
        heart_6,
        heart_7,
        heart_8,
        heart_9,
        heart_10,
        heart_J,
        heart_Q,
        heart_K,
        heart_A,

        spade_2,
        spade_3,
        spade_4,
        spade_5,
        spade_6,
        spade_7,
        spade_8,
        spade_9,
        spade_10,
        spade_J,
        spade_Q,
        spade_K,
        spade_A,

        diamond_2,
        diamond_3,
        diamond_4,
        diamond_5,
        diamond_6,
        diamond_7,
        diamond_8,
        diamond_9,
        diamond_10,
        diamond_J,
        diamond_Q,
        diamond_K,
        diamond_A,

        club_2,
        club_3,
        club_4,
        club_5,
        club_6,
        club_7,
        club_8,
        club_9,
        club_10,
        club_J,
        club_Q,
        club_K,
        club_A,

        joker_color,
        joker_bw,
        custom
    };

    // struct card_item {
    //     poker_type type;
    //     size_t source_index;
    // };

    using card_item = std::pair<poker_type, size_t>;

    class poker_deck {
        std::deque<card_item> deck;
        std::vector<std::string> card_sources;

    public:
        std::deque<card_item> drawer;
        void init(const std::string &) noexcept;
        void add(const std::string &) noexcept;
        void shuffle() noexcept;
        bool peek(unsigned, std::deque<card_item> &) const noexcept;
        bool draw(unsigned) noexcept;
        void clear() noexcept;
        size_t size() const noexcept;
        std::string render_name(const card_item &) const noexcept;
    };
} // namespace dicebot::poker
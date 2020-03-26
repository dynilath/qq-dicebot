#pragma once

#include <stdint.h>
#include <deque>
#include <list>
#include <string>
#include <vector>

namespace dicebot::poker {

    void initialize_basic_cards() noexcept;

    class poker_deck {
        using card_item_t = typename ::std::add_pointer<::std::string>::type;
        std::deque<card_item_t> deck;
        std::vector<std::string> card_sources;

    public:
        std::deque<card_item_t> drawer;
        void init(const std::string &) noexcept;
        void add(const std::string &) noexcept;
        void shuffle() noexcept;
        bool peek(unsigned, std::deque<card_item_t> &) const noexcept;
        bool draw(unsigned) noexcept;
        void clear() noexcept;
        size_t size() const noexcept;
        std::string render_name(const card_item_t &) const noexcept;
    };
} // namespace dicebot::poker
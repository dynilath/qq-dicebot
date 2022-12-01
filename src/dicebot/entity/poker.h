#pragma once

#include <cstdint>
#include <deque>
#include <list>
#include <string>
#include <vector>
#include <variant>

namespace dicebot::poker {

    void initialize_basic_cards() noexcept;
    
    struct card_t{
        size_t value;
    };

    struct poker_card_t : public card_t{};

    struct major_arcana_card_t : public card_t{};

    struct minor_arcana_card_t : public card_t{};

    struct user_defined_card_t : public card_t{};

    using card_item_t = std::variant<poker_card_t,major_arcana_card_t,minor_arcana_card_t,user_defined_card_t>;

    class poker_deck {
        
    public:
        //using card_item_t = typename ::std::add_pointer<::std::string>::type;
        using deck_t = std::deque<card_item_t>;
        using sources_t = std::vector<std::string>;

    private:
        deck_t deck;
        sources_t card_sources;

    public:
        deck_t drawer;
        void init(const std::string &) noexcept;
        void add(const std::string &) noexcept;
        void shuffle() noexcept;
        bool peek(unsigned, std::deque<card_item_t> &) const noexcept;
        bool draw(unsigned) noexcept;
        void clear() noexcept;
        size_t size() const noexcept;
        std::string render_name(const card_item_t &) const noexcept;
        std::string pack_definition() const noexcept;
        std::string pack_drawn() const noexcept;
        std::string pack_deck() const noexcept;
        void load_pack(const std::string &,const std::string &,const std::string &) noexcept;
    };
} // namespace dicebot::poker
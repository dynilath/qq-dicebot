#include "./poker.h"

#include <array>
#include <deque>
#include <regex>
#include <unordered_map>
#include <vector>

#include <sstream>

#include "../../utils/base64.hpp"
#include "../random/random_provider.h"
#include "../utils/string_utils.h"
#include "../utils/utils.h"
#include "../utils/integer_utf8.h"

using namespace dicebot;
using namespace poker;

using namespace boost::beast::detail;

constexpr size_t MAX_DECK_SIZE = 200;

namespace {
    constexpr char poker_core_deck_name[] = "core";
    constexpr char poker_standard_deck_name[] = "standard";

    constexpr char poker_name_ascii[][14] = {
        "Heart 2",   "Heart 3",   "Heart 4",   "Heart 5",    "Heart 6",
        "Heart 7",   "Heart 8",   "Heart 9",   "Heart 10",   "Heart J",
        "Heart Q",   "Heart K",   "Heart A",

        "Spade 2",   "Spade 3",   "Spade 4",   "Spade 5",    "Spade 6",
        "Spade 7",   "Spade 8",   "Spade 9",   "Spade 10",   "Spade J",
        "Spade Q",   "Spade K",   "Spade A",

        "Diamond 2", "Diamond 3", "Diamond 4", "Diamond 5",  "Diamond 6",
        "Diamond 7", "Diamond 8", "Diamond 9", "Diamond 10", "Diamond J",
        "Diamond Q", "Diamond K", "Diamond A",

        "Club 2",    "Club 3",    "Club 4",    "Club 5",     "Club 6",
        "Club 7",    "Club 8",    "Club 9",    "Club 10",    "Club J",
        "Club Q",    "Club K",    "Club A",

        "Joker",     "Joker(bw)"};

    #define heart "♡"
    #define spade "♤"
    #define diamd "♢"
    #define club "♧"

    constexpr char poker_name_unicode_lite[][14] = {
        heart "2", heart "3",  heart "4", heart "5", heart "6", heart "7", heart "8",
        heart "9", heart "10", heart "J", heart "Q", heart "K", heart "A",

        spade "2", spade "3",  spade "4", spade "5", spade "6", spade "7", spade "8",
        spade "9", spade "10", spade "J", spade "Q", spade "K", spade "A",
        
        diamd "2", diamd "3",  diamd "4", diamd "5", diamd "6", diamd "7", diamd "8",
        diamd "9", diamd "10", diamd "J", diamd "Q", diamd "K", diamd "A",
        
        club "2", club "3",  club "4", club "5", club "6", club "7", club "8",
        club "9", club "10", club "J", club "Q", club "K", club "A",

        "Joker", "Joker(bw)"};

    #undef heart
    #undef spade
    #undef diamd
    #undef club


    constexpr char poker_name_unicode_pretty[][14] = {
        "🂲", "🂳",   "🂴", "🂵", "🂶", "🂷", "🂸", "🂹", "🂺", "🂻", "🂽", "🂾", "🂱",

        "🂢", "🂣",   "🂤", "🂥", "🂦", "🂧", "🂨", "🂩", "🂪", "🂫", "🂭", "🂮", "🂡",

        "🃂", "🃃",   "🃄", "🃅", "🃆", "🃇", "🃈", "🃉", "🃊", "🃋", "🃍", "🃎", "🃁",

        "🃒", "🃓",   "🃔", "🃕", "🃖", "🃗", "🃘", "🃙", "🃚", "🃛", "🃝", "🃞", "🃑",
        "🃏", "🂿"};

    constexpr char poker_alter_names_set_1[][4] = {
        "h2", "h3",  "h4", "h5", "h6", "h7", "h8",
        "h9", "h10", "hj", "hq", "hk", "ha",

        "s2", "s3",  "s4", "s5", "s6", "s7", "s8",
        "s9", "s10", "sj", "sq", "sk", "sa",

        "d2", "d3",  "d4", "d5", "d6", "d7", "d8",
        "d9", "d10", "dj", "dq", "dk", "da",

        "c2", "c3",  "c4", "c5", "c6", "c7", "c8",
        "c9", "c10", "cj", "cq", "ck", "ca",

        "jc", "jb"};

    constexpr char poker_alter_names_set_2[][16] = {
        "heart2",   "heart3",   "heart4",   "heart5",    "heart6",
        "heart7",   "heart8",   "heart9",   "heart10",   "heartj",
        "heartq",   "heartk",   "hearta",

        "spade2",   "spade3",   "spade4",   "spade5",    "spade6",
        "spade7",   "spade8",   "spade9",   "spade10",   "spadej",
        "spadeq",   "spadek",   "spadea",

        "diamond2", "diamond3", "diamond4", "diamond5",  "diamond6",
        "diamond7", "diamond8", "diamond9", "diamond10", "diamondj",
        "diamondq", "diamondk", "diamonda",

        "club2",    "club3",    "club4",    "club5",     "club6",
        "club7",    "club8",    "club9",    "club10",    "clubj",
        "clubq",    "clubk",    "cluba"};

    constexpr char tarot_deck_name[] = "tarot";
    constexpr char tarot_major_deck_name[] = "major arcana";
    constexpr char tarot_minor_deck_name[] = "minor arcana";
    constexpr char major_arcana_card_names[][30] = {
        "1 The Magician",   "2 The High Priestess",
        "3 The Empress",    "4 The Emperor",
        "5 The Hierophant", "6 The Lovers",
        "7 The Chariot",    "8 Strength",
        "9 The Hermit",     "10 Wheel of Fortune",
        "11 Justice",       "12 The Hanged Man",
        "13 Death",         "14 Temperance",
        "15 The Devil",     "16 The Tower",
        "17 The Star",      "18 The Moon",
        "19 The Sun",       "20 Judgement",
        "21 The World",     "0 The Foul"};

    constexpr char minor_arcana_card_names[][30] = {
        "Ace of Wands",    "2 of Wands",     "3 of Wands",
        "4 of Wands",      "5 of Wands",     "6 of Wands",
        "7 of Wands",      "8 of Wands",     "9 of Wands",
        "10 of Wands",     "Page of Wands",  "Knight of Wands",
        "Queen of Wands",  "King of Wands",

        "Ace of Coins",    "2 of Coins",     "3 of Coins",
        "4 of Coins",      "5 of Coins",     "6 of Coins",
        "7 of Coins",      "8 of Coins",     "9 of Coins",
        "10 of Coins",     "Page of Coins",  "Knight of Coins",
        "Queen of Coins",  "King of Coins",

        "Ace of Cups",     "2 of Cups",      "3 of Cups",
        "4 of Cups",       "5 of Cups",      "6 of Cups",
        "7 of Cups",       "8 of Cups",      "9 of Cups",
        "10 of Cups",      "Page of Cups",   "Knight of Cups",
        "Queen of Cups",   "King of Cups",

        "Ace of Swords",   "2 of Swords",    "3 of Swords",
        "4 of Swords",     "5 of Swords",    "6 of Swords",
        "7 of Swords",     "8 of Swords",    "9 of Swords",
        "10 of Swords",    "Page of Swords", "Knight of Swords",
        "Queen of Swords", "King of Swords",
    };

    template <class T>
    struct c_array_trait {};

    template <class val_t, size_t len>
    struct c_array_trait<val_t[len]> {
        static constexpr size_t length = len;
        using value_type = val_t;
    };

    auto& poker_name = poker_name_unicode_lite;
    constexpr auto poker_full_len = c_array_trait<std::remove_reference<decltype(poker_name)>::type>::length;
    constexpr auto poker_std_len = poker_full_len - 2;
    constexpr auto major_len = c_array_trait<decltype(major_arcana_card_names)>::length;
    constexpr auto minor_len = c_array_trait<decltype(minor_arcana_card_names)>::length;

    ::std::array<::std::string, 2> joker;
    ::std::array<::std::string, major_len> major_arcana;
    ::std::array<::std::string, minor_len> minor_arcana;

    //using p_type = typename ::std::add_pointer<::std::string>::type;

    using nick_2_card_map = ::std::unordered_map<std::string, card_item_t>;

    nick_2_card_map card_nicks;

    ::std::unordered_map<::std::string, ::std::vector<card_item_t>> preset_deck_map =
        {{poker_core_deck_name, {}},
         {poker_standard_deck_name, {}},
         {tarot_deck_name, {}},
         {tarot_major_deck_name, {}},
         {tarot_minor_deck_name, {}}};

    void creat_card_name_map() noexcept {
        
        // init poker decks
        auto& poker_core_vec = preset_deck_map[poker_core_deck_name];
        auto& poker_standard_vec = preset_deck_map[poker_standard_deck_name];

        poker_core_vec.resize(poker_std_len);
        poker_standard_vec.resize(poker_full_len);
        for (size_t i = 0; i < 52; i++) {
            poker_core_vec[i] = poker_standard_vec[i] = poker_card_t{i};
        }

        constexpr size_t joker0_offset = poker_full_len - 2;
        constexpr size_t joker1_offset = poker_full_len - 1;
        joker[0] = poker_name[joker0_offset];
        joker[1] = poker_name[joker1_offset];
        poker_standard_vec[joker0_offset] = poker_card_t{joker0_offset};
        poker_standard_vec[joker1_offset] = poker_card_t{joker1_offset};

        // init arcana decks
        auto& tarot_vec = preset_deck_map[tarot_deck_name];
        auto& major_arcana_vec = preset_deck_map[tarot_major_deck_name];
        auto& minor_arcana_vec = preset_deck_map[tarot_minor_deck_name];


        major_arcana_vec.reserve(major_len);
        minor_arcana_vec.reserve(minor_len);
        tarot_vec.reserve(major_len + minor_len);
        for (size_t i = 0; i < major_len; i++) {
            major_arcana[i] = major_arcana_card_names[i];
            tarot_vec.push_back(major_arcana_card_t{i});
            major_arcana_vec.push_back(major_arcana_card_t{i});
        }
        for (size_t i = 0; i < minor_len; i++) {
            minor_arcana[i] = minor_arcana_card_names[i];
            tarot_vec.push_back(minor_arcana_card_t{i});
            minor_arcana_vec.push_back(minor_arcana_card_t{i});
        }

        // init poker nicks
        for (size_t i = 0;
             i < c_array_trait<decltype(poker_alter_names_set_1)>::length;
             i++) {
            card_nicks[poker_alter_names_set_1[i]] = poker_standard_vec[i];
        }

        for (size_t i = 0;
             i < c_array_trait<decltype(poker_alter_names_set_2)>::length;
             i++) {
            card_nicks[poker_alter_names_set_2[i]] = poker_standard_vec[i];
        }
        card_nicks["j"] = poker_card_t{joker0_offset};
        card_nicks["joker"] = poker_card_t{joker1_offset};
    }

} // namespace

void poker::initialize_basic_cards() noexcept { creat_card_name_map(); }

static size_t split_string_ignore_spaces(
    const std::string& source, char splitter,
    std::deque<std::pair<size_t, size_t>>& container) {
    constexpr char ignores[] = " \t";
    size_t o_pos_found = 0;
    size_t pos_found;
    for (; o_pos_found < source.size();) {
        pos_found = source.find_first_of(splitter, o_pos_found);
        if (pos_found != std::string::npos) {
            size_t endpoint = source.find_last_not_of(ignores, pos_found - 1);
            if (endpoint != std::string::npos && endpoint >= o_pos_found)
                container.push_back({o_pos_found, endpoint});
        } else {
            size_t endpoint = source.find_last_not_of(ignores, pos_found - 1);
            if (endpoint != std::string::npos && endpoint >= o_pos_found)
                container.push_back({o_pos_found, endpoint});
            break;
        }
        o_pos_found = source.find_first_not_of(ignores, pos_found + 1);
    }
    return container.size();
};

static std::pair<uint32_t, std::string> split_into_number_and_name(
    ::utils::string_view source) {
    auto number_end =
        ::std::find_if_not(source.begin(), source.end(), [](auto c) {
            return '0' <= c && c <= '9';
        });

    if (number_end == source.begin()) {
        if (::utils::trim(source))
            return {1, source};
        else
            return {0, ""};
    }

    ::utils::string_view sv_num = {source.begin(), number_end};
    ::utils::string_view sv_name = {number_end, source.end()};
    if (::utils::trim(sv_name)) {
        size_t counts_of_item = std::stoi(sv_num);
        return {counts_of_item, sv_name};
    } else {
        ::utils::trim(sv_num);
        return {1, sv_num};
    }
};

auto split_with_first_separator(const std::string& source)
    -> std::deque<::utils::string_view> {
    std::deque<::utils::string_view> ret_val;

    ::std::deque<::utils::string_view> unbracketed;
    auto start_point = source.begin();
    while (start_point != source.end()) {
        auto lbracket = ::std::find(start_point, source.end(), '[');
        if (lbracket == source.end()) {
            unbracketed.push_back({start_point, source.end()});
            break;
        }
        auto rbracket = ::std::find(lbracket, source.end(), ']');
        if (rbracket == source.end()) {
            unbracketed.push_back({start_point, source.end()});
            break;
        }
        if (lbracket != start_point)
            unbracketed.push_back({start_point, lbracket});
        start_point = rbracket + 1;
    }

    std::regex reg_separator(u8"(,|\\/|&|;|，|。|、|；)");
    ::utils::string_view sv_sep;
    auto where = unbracketed.begin();
    for (; where != unbracketed.end(); where++) {
        std::smatch match_separator;
        std::regex_search(
            where->begin(), where->end(), match_separator, reg_separator);
        if (match_separator.empty())
            continue;
        else {
            sv_sep = match_separator[0];
            break;
        }
    }

    auto trim_push = [&ret_val](::utils::string_view::iterator left,
                                ::utils::string_view::iterator right) {
        ::utils::string_view sv = {left, right};
        if (::utils::trim(sv)) {
            ret_val.push_back(std::move(sv));
        }
    };

    if (where == unbracketed.end()) {
        trim_push(source.begin(), source.end());
    } else {
        auto source_begin_point = source.begin();
        trim_push(source_begin_point, sv_sep.begin());
        source_begin_point = sv_sep.end();

        auto start_point = sv_sep.end();
        auto end_point = where->end();
        while (true) {
            auto found = ::std::search(
                start_point, end_point, sv_sep.begin(), sv_sep.end());
            if (found == end_point) {
                where++;
                if (where == unbracketed.end()) break;
                start_point = where->begin();
                end_point = where->end();
                continue;
            } else {
                trim_push(source_begin_point, found);
                start_point = source_begin_point = found + sv_sep.size();
            }
        }
        trim_push(source_begin_point, source.end());
    }

    return ret_val;
}

void poker_deck::init(const std::string& params) noexcept {
    this->clear();
    auto split_container = split_with_first_separator(params);

    if (split_container.size() > MAX_DECK_SIZE)
        this->card_sources.reserve(MAX_DECK_SIZE);
    else
        this->card_sources.reserve(split_container.size());

    for (auto& item : split_container) {
        std::string item_string = item;

        auto preset_d = preset_deck_map.find(item_string);
        if (preset_d != preset_deck_map.end()) {
            for (auto& item : preset_d->second) {
                this->deck.push_back(item);
            }
            continue;
        }

        auto [count, name] = split_into_number_and_name(item);

        if (count > MAX_DECK_SIZE) count = MAX_DECK_SIZE;
        if (count + this->deck.size() > MAX_DECK_SIZE) {
            count = MAX_DECK_SIZE - this->deck.size();
        }

        if (count > 0) {
            std::string lc_content = utils::lower_case_copy(name);
            auto iter = card_nicks.find(lc_content);

            if (iter != card_nicks.end()) {
                utils::repeat(count, [this, iter](size_t) {
                    this->deck.push_back(iter->second);
                });
            } else {
                size_t source_idx = this->card_sources.size();
                this->card_sources.push_back(name);
                utils::repeat(count, [this, iter, source_idx](size_t) {
                    this->deck.push_back(user_defined_card_t{source_idx});
                });
            }
        }

        if (this->deck.size() >= MAX_DECK_SIZE) break;
    }
    this->shuffle();
}

void poker_deck::shuffle() noexcept {
    for (auto& item : this->drawer) {
        this->deck.push_back(item);
    }
    this->drawer.clear();
    decltype(this->deck) sequencer;
    utils::repeat(this->deck.size(), [&sequencer, this](size_t a) {
        size_t target = random::rand_int(0, a);
        sequencer.push_back(this->deck[target]);
        this->deck.erase(this->deck.begin() + target);
    });
    this->deck = std::move(sequencer);
}

bool poker_deck::draw(unsigned draw_count) noexcept {
    if (this->deck.empty()) return false;
    utils::repeat(draw_count, [this](size_t) {
        this->drawer.push_back(this->deck.front());
        this->deck.pop_front();
    });
    return true;
}

bool poker_deck::peek(unsigned peek_count,
                      std::deque<card_item_t>& container) const noexcept {
    if (this->deck.empty()) return false;
    if (this->deck.size() < peek_count) peek_count = this->deck.size();
    utils::repeat(peek_count, [&container, this](size_t a) {
        container.push_back(this->deck.at(a));
    });
    return true;
}

size_t poker_deck::size() const noexcept { return this->deck.size(); }

void poker_deck::clear() noexcept {
    this->card_sources.clear();
    this->deck.clear();
    this->drawer.clear();
}

size_t get_card_value(const card_item_t& item) noexcept {
    if(std::holds_alternative<poker_card_t>(item)){
        return std::get<poker_card_t>(item).value;
    }
    else if (std::holds_alternative<major_arcana_card_t>(item)){
        return std::get<major_arcana_card_t>(item).value;
    }
    else if (std::holds_alternative<minor_arcana_card_t>(item)){
        return std::get<minor_arcana_card_t>(item).value;
    }
    else if (std::holds_alternative<user_defined_card_t>(item)){
        return std::get<user_defined_card_t>(item).value;
    }
    return (size_t)-1;
}

card_item_t card_from_value(size_t id, size_t value) {
    switch (id)
    {
    case 0:
        return decltype(std::get<0>(card_item_t{})){value};
    case 1:
        return decltype(std::get<1>(card_item_t{})){value};
    case 2:
        return decltype(std::get<2>(card_item_t{})){value};
    case 3:
        return decltype(std::get<3>(card_item_t{})){value};
    default:
        throw std::bad_variant_access();
    }
}

std::string poker_deck::render_name(const card_item_t& item) const noexcept {
    if(std::holds_alternative<poker_card_t>(item)){
        auto v = std::get<poker_card_t>(item).value;
        if(v < poker_full_len)
            return poker_name[v];
    }
    else if (std::holds_alternative<major_arcana_card_t>(item)){
        auto v = std::get<major_arcana_card_t>(item).value;
        if(v < major_len)
            return major_arcana_card_names[v];
    }
    else if (std::holds_alternative<minor_arcana_card_t>(item)){
        auto v = std::get<minor_arcana_card_t>(item).value;
        if(v < minor_len)
            return minor_arcana_card_names[v];
    }
    else if (std::holds_alternative<user_defined_card_t>(item)){
        auto v = std::get<user_defined_card_t>(item).value;
        if(v < this->card_sources.size())
            return this->card_sources[v];
    }
    return "";
}

std::string poker_deck::pack_definition() const noexcept {
    if(this->card_sources.empty()) return "";
    std::ostringstream out;

    std::deque<unsigned int> out_buff;
    out_buff.push_back(this->card_sources.size());
    for (const auto & i : this->card_sources)
    {
        out_buff.push_back(i.length());
        for(const auto c : i){
            out_buff.push_back(static_cast<unsigned char>(c));
        }
    }
    auto outs = utils::integers_2_utf8_bytes(out_buff.begin(),out_buff.end());
    return base64_encode(outs.data(), outs.size());
}

auto unpack_definitions(const std::string& in){
    poker_deck::sources_t ret;
    if(in.empty()) return ret;
    auto ins = base64_decode(in);
    auto in_ints = utils::utf8_bytes_2_integers(ins.begin(),ins.end());
    auto iter = in_ints.begin();
    size_t full_len = *(iter++);
    
    while(full_len--){
        size_t this_len = *(iter++);
        std::string v(this_len,' ');
        auto pt = reinterpret_cast<unsigned char*>(v.data());
        while(this_len--){
            if(iter == in_ints.end()) return poker_deck::sources_t();
            *pt = static_cast<unsigned char>(*iter);
            pt++; iter++;
        }
        ret.push_back(std::move(v));
    }
    return ret;
}

std::string pack_deck_t(const poker_deck::deck_t& deck) noexcept{
    if(deck.empty()) return "";
    std::ostringstream oss;
    
    std::deque<unsigned int> out_buff;
    out_buff.push_back(deck.size());
    for (const auto & i : deck)
    {
        out_buff.push_back(i.index());
        out_buff.push_back(get_card_value(i));
    }
    auto outs = utils::integers_2_utf8_bytes(out_buff.begin(),out_buff.end());
    return base64_encode(outs.data(), outs.size());
}

auto unpack_deck_t(const std::string& in) noexcept{
    poker_deck::deck_t ret;
    if(in.empty()) return ret;

    auto ins = base64_decode(in);
    auto in_ints = utils::utf8_bytes_2_integers(ins.begin(),ins.end());
    if(in_ints.empty()) return ret;

    auto iter = in_ints.begin();
    size_t full_len = *(iter++);
    while(full_len--){
        size_t id = *(iter++);
        size_t value = *(iter++);
        try{
            card_item_t c = card_from_value(id, value);
            ret.push_back(std::move(c));
        }
        catch(std::bad_variant_access&){}
    }
    return ret;
}

std::string poker_deck::pack_drawn() const noexcept {
    return pack_deck_t(this->drawer);
}

std::string poker_deck::pack_deck() const noexcept {
    return pack_deck_t(this->deck);
}

void poker_deck::load_pack(const std::string & definition,const std::string & drawn,const std::string & deck) noexcept {
    this->card_sources = unpack_definitions(definition);
    this->drawer = unpack_deck_t(drawn);
    this->deck = unpack_deck_t(deck);
}
#include "./poker.h"
#include <array>
#include <regex>
#include <unordered_map>
#include <vector>
#include "../random/random_provider.h"
#include "../utils/utils.h"
using namespace dicebot;
using namespace poker;

constexpr size_t MAX_DECK_SIZE = 200;

namespace {
    constexpr char core_deck_name[] = "core";
    constexpr char standard_deck_name[] = "standard";

    constexpr char poker_name_ascii[][14] = {
        "heart 2",   "Heart 3",   "Heart 4",   "Heart 5",    "Heart 6",
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

    constexpr char poker_name_unicode_lite[][14] = {
        "♡2",    "♡3",       "♡4", "♡5", "♡6", "♡7", "♡8",
        "♡9",    "♡10",      "♡J", "♡Q", "♡K", "♡A",

        "♠2",    "♠3",       "♠4", "♠5", "♠6", "♠7", "♠8",
        "♠9",    "♠10",      "♠J", "♠Q", "♠K", "♠A",

        "♢2",    "♢3",       "♢4", "♢5", "♢6", "♢7", "♢8",
        "♢9",    "♢10",      "♢J", "♢Q", "♢K", "♢A",

        "♣2",    "♣3",       "♣4", "♣5", "♣6", "♣7", "♣8",
        "♣9",    "♣10",      "♣J", "♣Q", "♣K", "♣A",

        "Joker", "Joker(bw)"};

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

    constexpr char major_akana_name[][30] = {
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

    template <class T>
    struct c_array_trait {};

    template <class val_t, size_t len>
    struct c_array_trait<val_t[len]> {
        static constexpr size_t length = len;
        using value_type = val_t;
    };

    auto& poker_name = poker_name_unicode_lite;

    constexpr size_t house_len = 13;
    using house_container_t = ::std::array<::std::string, house_len>;

    house_container_t heart;
    house_container_t spade;
    house_container_t diamond;
    house_container_t club;
    ::std::array<::std::string, 2> joker;
    ::std::array<::std::string, 22> major_akana;

    using p_type = typename ::std::add_pointer<::std::string>::type;

    ::std::vector<p_type> poker_core_vec(52);
    ::std::vector<p_type> poker_standard_vec(54);

    using nick_2_card_map = ::std::unordered_map<std::string, p_type>;
    nick_2_card_map card_nicks;

    void creat_card_name_map() noexcept{
        constexpr size_t arr_len = heart.size();

        using p_2_containers_t =
            typename ::std::add_pointer<decltype(heart)>::type;
        ::std::vector<p_2_containers_t> poker_standard = {
            &heart, &spade, &diamond, &club};

        for (size_t i = 0; i < arr_len * 4; i++) {
            auto& p_poker = *(poker_standard[i / arr_len]);
            p_poker[i % arr_len] = poker_name[i];
            poker_core_vec[i] = poker_standard_vec[i] = &p_poker[i % arr_len];
        }

        joker[0] = poker_name[52];
        joker[1] = poker_name[53];

        poker_standard_vec[52] = &joker[0];
        poker_standard_vec[53] = &joker[1];

        for (size_t i = 0; i < 22; i++) {
            major_akana[i] = major_akana_name[i];
        }

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
        card_nicks["j"] = &joker[0];
        card_nicks["joker"] = &joker[0];
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
    const std::string& source, const std::pair<size_t, size_t>& item) {
    constexpr char numbers[] = "0123456789";
    int counts_of_item = 1;
    size_t item_number_part = source.find_first_not_of(numbers, item.first);

    size_t name_start = item.first;

    if (item_number_part > item.second)
        return {0, ""};
    else if (item_number_part > item.first) {
        counts_of_item =
            std::stoi(source.substr(item.first, item_number_part - item.first));
        name_start = source.find_first_not_of(numbers, item_number_part);
    }
    return {counts_of_item,
            source.substr(name_start, item.second + 1 - name_start)};
};

auto split_with_first_separator(const std::string& source)
    -> std::deque<std::pair<size_t, size_t>> {
    std::deque<std::pair<size_t, size_t>> ret_val;
    std::smatch match_separator;
    std::regex reg_separator(u8"(,|\\/|&|;|，|。|、|；)");
    constexpr char ignores[] = " \t";
    std::regex_search(source, match_separator, reg_separator);
    if (match_separator.empty()) {
        auto start_point = source.find_first_not_of(ignores);
        if (start_point != std::string::npos) {
            auto end_point = source.find_last_not_of(ignores);
            ret_val.push_back({start_point, end_point});
            return ret_val;
        } else
            return ret_val;
    } else {
        std::string separator = match_separator[0];
        size_t start_point = 0;
        size_t end_point = std::string::npos;
        while (true) {
            auto sep_point = source.find(separator, start_point);
            start_point = source.find_first_not_of(ignores, start_point);
            end_point = source.find_last_not_of(ignores, sep_point - 1);
            if (end_point != std::string::npos && end_point >= start_point)
                ret_val.push_back({start_point, end_point});
            if (sep_point == std::string::npos) break;
            start_point = sep_point + separator.size();
            if (start_point >= source.size()) break;
        }
        return ret_val;
    }
}

void poker_deck::init(const std::string& params) noexcept {
    this->clear();
    auto split_container = split_with_first_separator(params);

    if(split_container.size() > MAX_DECK_SIZE)
        this->card_sources.reserve(MAX_DECK_SIZE);
    else
        this->card_sources.reserve(split_container.size());

    for (auto& item : split_container) {
        size_t item_len = item.second + 1 - item.first;

        std::string item_string = params.substr(item.first, item_len);

        if (item_string == core_deck_name) {
            for (auto& item : poker_core_vec) {
                this->deck.push_back(item);
            }
            continue;
        } else if (item_string == standard_deck_name) {
            for (auto& item : poker_standard_vec) {
                this->deck.push_back(item);
            }
            continue;
        }

        auto [count, name] = split_into_number_and_name(params, item);

        if(count > MAX_DECK_SIZE)
            count = MAX_DECK_SIZE;
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
                    this->deck.push_back(&this->card_sources[source_idx]);
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

std::string poker_deck::render_name(const card_item_t& item) const noexcept {
    return *item;
}
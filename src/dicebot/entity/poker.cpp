#include "./poker.h"
#include <unordered_map>
#include <regex>
#include <vector>
#include "../random/random_provider.h"
#include "../utils/utils.h"
using namespace dicebot;
using namespace poker;

constexpr size_t MAX_DECK_SIZE = 200;

constexpr char core_deck_name[] = "core";
constexpr char standard_deck_name[] = "standard";

constexpr poker::poker_type core_deck[] = {
    heart_2,   heart_3,    heart_4,   heart_5,   heart_6,   heart_7,   heart_8,   heart_9,   heart_10,  heart_J,   heart_Q,
    heart_K,   heart_A,    spade_2,   spade_3,   spade_4,   spade_5,   spade_6,   spade_7,   spade_8,   spade_9,   spade_10,
    spade_J,   spade_Q,    spade_K,   spade_A,   diamond_2, diamond_3, diamond_4, diamond_5, diamond_6, diamond_7, diamond_8,
    diamond_9, diamond_10, diamond_J, diamond_Q, diamond_K, diamond_A, club_2,    club_3,    club_4,    club_5,    club_6,
    club_7,    club_8,     club_9,    club_10,   club_J,    club_Q,    club_K,    club_A};

constexpr poker::poker_type standard_deck[] = {
    heart_2,   heart_3,    heart_4,   heart_5,   heart_6,   heart_7,   heart_8,   heart_9,   heart_10,    heart_J,   heart_Q,
    heart_K,   heart_A,    spade_2,   spade_3,   spade_4,   spade_5,   spade_6,   spade_7,   spade_8,     spade_9,   spade_10,
    spade_J,   spade_Q,    spade_K,   spade_A,   diamond_2, diamond_3, diamond_4, diamond_5, diamond_6,   diamond_7, diamond_8,
    diamond_9, diamond_10, diamond_J, diamond_Q, diamond_K, diamond_A, club_2,    club_3,    club_4,      club_5,    club_6,
    club_7,    club_8,     club_9,    club_10,   club_J,    club_Q,    club_K,    club_A,    joker_color, joker_bw};

constexpr char poker_name_ascii[][14] = {
    "heart 2",   "Heart 3",   "Heart 4",   "Heart 5",   "Heart 6",   "Heart 7",   "Heart 8",   "Heart 9",    "Heart 10",
    "Heart J",   "Heart Q",   "Heart K",   "Heart A",   "Spade 2",   "Spade 3",   "Spade 4",   "Spade 5",    "Spade 6",
    "Spade 7",   "Spade 8",   "Spade 9",   "Spade 10",  "Spade J",   "Spade Q",   "Spade K",   "Spade A",    "Diamond 2",
    "Diamond 3", "Diamond 4", "Diamond 5", "Diamond 6", "Diamond 7", "Diamond 8", "Diamond 9", "Diamond 10", "Diamond J",
    "Diamond Q", "Diamond K", "Diamond A", "Club 2",    "Club 3",    "Club 4",    "Club 5",    "Club 6",     "Club 7",
    "Club 8",    "Club 9",    "Club 10",   "Club J",    "Club Q",    "Club K",    "Club A",    "Joker",      "Joker(bw)"};

constexpr char poker_name_unicode_lite[][14] = {
    "♡2", "♡3", "♡4", "♡5",  "♡6", "♡7", "♡8", "♡9", "♡10", "♡J", "♡Q", "♡K",  "♡A", "♠2", "♠3", "♠4", "♠5",    "♠6",
    "♠7", "♠8", "♠9", "♠10", "♠J", "♠Q", "♠K", "♠A", "♢2",  "♢3", "♢4", "♢5",  "♢6", "♢7", "♢8", "♢9", "♢10",   "♢J",
    "♢Q", "♢K", "♢A", "♣2",  "♣3", "♣4", "♣5", "♣6", "♣7",  "♣8", "♣9", "♣10", "♣J", "♣Q", "♣K", "♣A", "Joker", "Joker(bw)"};

constexpr char poker_name_unicode_pretty[][14] = {"🂲", "🂳", "🂴", "🂵", "🂶", "🂷", "🂸", "🂹", "🂺", "🂻", "🂽", "🂾",   "🂱", "🂢",
                                                  "🂣", "🂤", "🂥", "🂦", "🂧", "🂨", "🂩", "🂪", "🂫", "🂭", "🂮", "🂡",   "🃂", "🃃",
                                                  "🃄", "🃅", "🃆", "🃇", "🃈", "🃉", "🃊", "🃋", "🃍", "🃎", "🃁", "🃒",   "🃓", "🃔",
                                                  "🃕", "🃖", "🃗", "🃘", "🃙", "🃚", "🃛", "🃝", "🃞", "🃑", "🃏", "🂿"};

const std::unordered_map<std::string, poker::poker_type> card_names = {{"h2", heart_2},
                                                                       {"h3", heart_3},
                                                                       {"h4", heart_4},
                                                                       {"h5", heart_5},
                                                                       {"h6", heart_6},
                                                                       {"h7", heart_7},
                                                                       {"h8", heart_8},
                                                                       {"h9", heart_9},
                                                                       {"h10", heart_10},
                                                                       {"hj", heart_J},
                                                                       {"hq", heart_Q},
                                                                       {"hk", heart_K},
                                                                       {"ha", heart_A},
                                                                       {"s2", spade_2},
                                                                       {"s3", spade_3},
                                                                       {"s4", spade_4},
                                                                       {"s5", spade_5},
                                                                       {"s6", spade_6},
                                                                       {"s7", spade_7},
                                                                       {"s8", spade_8},
                                                                       {"s9", spade_9},
                                                                       {"s10", spade_10},
                                                                       {"sj", spade_J},
                                                                       {"sq", spade_Q},
                                                                       {"sk", spade_K},
                                                                       {"sa", spade_A},
                                                                       {"d2", diamond_2},
                                                                       {"d3", diamond_3},
                                                                       {"d4", diamond_4},
                                                                       {"d5", diamond_5},
                                                                       {"d6", diamond_6},
                                                                       {"d7", diamond_7},
                                                                       {"d8", diamond_8},
                                                                       {"d9", diamond_9},
                                                                       {"d10", diamond_10},
                                                                       {"dj", diamond_J},
                                                                       {"dq", diamond_Q},
                                                                       {"dk", diamond_K},
                                                                       {"da", diamond_A},
                                                                       {"c2", club_2},
                                                                       {"c3", club_3},
                                                                       {"c4", club_4},
                                                                       {"c5", club_5},
                                                                       {"c6", club_6},
                                                                       {"c7", club_7},
                                                                       {"c8", club_8},
                                                                       {"c9", club_9},
                                                                       {"c10", club_10},
                                                                       {"cj", club_J},
                                                                       {"cq", club_Q},
                                                                       {"ck", club_K},
                                                                       {"ca", club_A},
                                                                       {"jc", joker_color},
                                                                       {"jb", joker_bw},
                                                                       {"heart2", heart_2},
                                                                       {"heart3", heart_3},
                                                                       {"heart4", heart_4},
                                                                       {"heart5", heart_5},
                                                                       {"heart6", heart_6},
                                                                       {"heart7", heart_7},
                                                                       {"heart8", heart_8},
                                                                       {"heart9", heart_9},
                                                                       {"heart10", heart_10},
                                                                       {"heartj", heart_J},
                                                                       {"heartq", heart_Q},
                                                                       {"heartk", heart_K},
                                                                       {"hearta", heart_A},
                                                                       {"spade2", spade_2},
                                                                       {"spade3", spade_3},
                                                                       {"spade4", spade_4},
                                                                       {"spade5", spade_5},
                                                                       {"spade6", spade_6},
                                                                       {"spade7", spade_7},
                                                                       {"spade8", spade_8},
                                                                       {"spade9", spade_9},
                                                                       {"spade10", spade_10},
                                                                       {"spadej", spade_J},
                                                                       {"spadeq", spade_Q},
                                                                       {"spadek", spade_K},
                                                                       {"spadea", spade_A},
                                                                       {"diamond2", diamond_2},
                                                                       {"diamond3", diamond_3},
                                                                       {"diamond4", diamond_4},
                                                                       {"diamond5", diamond_5},
                                                                       {"diamond6", diamond_6},
                                                                       {"diamond7", diamond_7},
                                                                       {"diamond8", diamond_8},
                                                                       {"diamond9", diamond_9},
                                                                       {"diamond10", diamond_10},
                                                                       {"diamondj", diamond_J},
                                                                       {"diamondq", diamond_Q},
                                                                       {"diamondk", diamond_K},
                                                                       {"diamonda", diamond_A},
                                                                       {"club2", club_2},
                                                                       {"club3", club_3},
                                                                       {"club4", club_4},
                                                                       {"club5", club_5},
                                                                       {"club6", club_6},
                                                                       {"club7", club_7},
                                                                       {"club8", club_8},
                                                                       {"club9", club_9},
                                                                       {"club10", club_10},
                                                                       {"clubj", club_J},
                                                                       {"clubq", club_Q},
                                                                       {"clubk", club_K},
                                                                       {"cluba", club_A},
                                                                       {"jc", joker_color},
                                                                       {"jb", joker_bw},
                                                                       {"j", joker_color},
                                                                       {"joker", joker_color}};

static size_t split_string_ignore_spaces(const std::string& source, char splitter,
                                         std::deque<std::pair<size_t, size_t>>& container) {
    constexpr char ignores[] = " \t";
    size_t o_pos_found = 0;
    size_t pos_found;
    for (; o_pos_found < source.size();) {
        pos_found = source.find_first_of(splitter, o_pos_found);
        if (pos_found != std::string::npos) {
            size_t endpoint = source.find_last_not_of(ignores, pos_found - 1);
            if (endpoint != std::string::npos && endpoint >= o_pos_found) container.push_back({o_pos_found, endpoint});
        } else {
            size_t endpoint = source.find_last_not_of(ignores, pos_found - 1);
            if (endpoint != std::string::npos && endpoint >= o_pos_found) container.push_back({o_pos_found, endpoint});
            break;
        }
        o_pos_found = source.find_first_not_of(ignores, pos_found + 1);
    }
    return container.size();
};

static std::pair<uint32_t, std::string> split_into_number_and_name(const std::string& source,
                                                                   const std::pair<size_t, size_t>& item) {
    constexpr char numbers[] = "0123456789";
    int counts_of_item = 1;
    size_t item_number_part = source.find_first_not_of(numbers, item.first);

    size_t name_start = item.first;

    if (item_number_part > item.second)
        return {0, ""};
    else if (item_number_part > item.first) {
        counts_of_item = std::stoi(source.substr(item.first, item_number_part - item.first));
        name_start = source.find_first_not_of(numbers, item_number_part);
    }
    return {counts_of_item, source.substr(name_start, item.second + 1 - name_start)};
};

auto split_with_first_separator(const std::string& source) -> std::deque<std::pair<size_t, size_t>>{
    std::deque<std::pair<size_t, size_t>> ret_val;
    std::smatch match_separator;
    std::regex reg_separator(u8"[,\\/&;，。、；]");
    constexpr char ignores[] = " \t";
    std::regex_search(source,match_separator,reg_separator);
    if(match_separator.empty()){
        auto start_point = source.find_first_not_of(ignores);
        if(start_point!=std::string::npos){
            auto end_point = source.find_last_not_of(ignores);
            ret_val.push_back({start_point,end_point});
            return ret_val;
        }
        else return ret_val;
    }else{
        std::string separator = match_separator[0];
        size_t start_point = 0;
        size_t end_point = std::string::npos;
        while(true){
            auto sep_point = source.find(separator,start_point);
            start_point = source.find_first_not_of(ignores,start_point);
            end_point = source.find_last_not_of(ignores,sep_point-1);
            if (end_point != std::string::npos && end_point >= start_point) 
                ret_val.push_back({start_point, end_point});
            if(sep_point == std::string::npos) break;
            start_point = sep_point+separator.size();
            if(start_point >= source.size()) break;
        }
        return ret_val;
    }
}

void poker_deck::init(const std::string& params) noexcept {
    this->clear();
    this->card_sources.reserve(MAX_DECK_SIZE);
    auto split_container = split_with_first_separator(params);
    for (auto& item : split_container) {
        size_t item_len = item.second + 1 - item.first;

        std::string item_string = params.substr(item.first, item_len);

        if (item_string == core_deck_name) {
            for (auto& item : core_deck) {
                this->deck.push_back({item, 0});
            }
            continue;
        } else if (item_string == standard_deck_name) {
            for (auto& item : standard_deck) {
                this->deck.push_back({item, 0});
            }
            continue;
        }

        auto [count, name] = split_into_number_and_name(params, item);

        if (count > 0) {
            std::string lc_content = utils::lower_case_copy(name);
            auto iter = card_names.find(lc_content);

            if (count + this->deck.size() > MAX_DECK_SIZE) {
                count = MAX_DECK_SIZE - this->deck.size();
            }

            if (iter != card_names.end()) {
                utils::repeat(count, [this, iter](size_t) { this->deck.push_back({iter->second, 0}); });
            } else {
                size_t source_idx = this->card_sources.size();
                utils::repeat(count, [this, iter, source_idx](size_t) { this->deck.push_back({custom, source_idx}); });
                this->card_sources.push_back(name);
            }
        }

        if (this->deck.size() >= MAX_DECK_SIZE) break;
    }
    this->card_sources.shrink_to_fit();
    this->shuffle();
}

void poker_deck::shuffle() noexcept {
    for (auto& item : this->drawer) {
        this->deck.push_back(item);
    }
    this->drawer.clear();
    std::deque<card_item> sequencer;
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

bool poker_deck::peek(unsigned peek_count, std::deque<card_item>& container) const noexcept {
    if (this->deck.empty()) return false;
    if (this->deck.size() < peek_count) peek_count = this->deck.size();
    utils::repeat(peek_count, [&container, this](size_t a) { container.push_back(this->deck.at(a)); });
    return true;
}

size_t poker_deck::size() const noexcept { return this->deck.size(); }

void poker_deck::clear() noexcept {
    this->card_sources.clear();
    this->deck.clear();
    this->drawer.clear();
}

std::string poker_deck::render_name(const card_item& item) const noexcept {
    if (item.first == custom) {
        return this->card_sources.at(item.second);
    } else {
        return poker_name_unicode_lite[item.first];
    }
}
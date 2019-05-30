#include "./tokenizer.h"

#include <algorithm>
#include <list>
#include "../constants.h"
#include "../utils/utils.h"

using namespace dicebot;
using namespace diceparser;

struct keyword_container {
    char name[3];
    token_index kw;
} static constexpr keyword_arr[]{{"d", token_index::keyword_d},
                                 {"D", token_index::keyword_d},
                                 {"kl", token_index::keyword_kl},
                                 {"kL", token_index::keyword_kl},
                                 {"Kl", token_index::keyword_kl},
                                 {"KL", token_index::keyword_kl},
                                 {"k", token_index::keyword_k},
                                 {"K", token_index::keyword_k}};

struct punct_container {
    char name;
    token_index pc;
} static constexpr punct_arr[]{{'+', token_index::punct_add},
                               {'-', token_index::punct_sub},
                               {'*', token_index::punct_mul},
                               {'/', token_index::punct_dvi},
                               {'{', token_index::punct_lbrace},
                               {'}', token_index::punct_rbrace},
                               {',', token_index::punct_comma},
                               {'(', token_index::punct_lparenthesis},
                               {')', token_index::punct_rparenthesis},
                               {'#', token_index::punct_sharp}};

/**
 * just ignore useless parenthesis/braces/commas
 */
static size_t regulate_parenthesis(std::string const& str, bool parse_brace) {
    using parenthesis_pair = std::pair<char, size_t>;
    std::deque<parenthesis_pair> chaser;
    bool terminate = false;
    int good_for_comma = 0;
    size_t i = 0;
    for (; i < str.size(); i++) {
        const char& c_here = str[i];
        switch (c_here) {
        case '{':
            if (!parse_brace) {
                terminate = true;
                break;
            }
            good_for_comma++;
        case '(':
            chaser.push_back(parenthesis_pair(c_here, i));
            break;
        case '}':
            if (!parse_brace) {
                terminate = true;
                break;
            }
            good_for_comma--;
            if (chaser.empty()) {
                terminate = true;
                break;
            }
            if (chaser.back().first == '{') {
                chaser.pop_back();
            } else
                terminate = true;
            break;
        case ')':
            if (chaser.empty()) {
                terminate = true;
                break;
            }
            if (chaser.back().first == '(') {
                chaser.pop_back();
            } else
                terminate = true;
            break;
        case ',':
            if (!parse_brace) {
                terminate = true;
                break;
            }
            if (good_for_comma == 0) {
                terminate = true;
            }
            break;
        case '\n':
            terminate = true;
            break;
        default:
            break;
        }
        if (terminate) break;
    }

    if (!chaser.empty())
        return chaser.front().second;
    else
        return i;
}

tokenizer::tokenizer(std::deque<token_t>& tokens, tokenizer_flag const& flag, std::string const& source,
                     macro_map_t const* macros) noexcept
    : token_container(tokens), macro_map(macros) {
    size_t restrict_len = regulate_parenthesis(source, flag.parse_dicelet);
    this->source = source.substr(0, restrict_len);
    this->sources = std::make_unique<sources_container_t>();
    this->sources->emplace_back(&this->source);

    size_t tail_start = source.find_first_not_of(' ', restrict_len);
    if (tail_start != npos) this->rtail = source.substr(tail_start);

    this->sources_sites = std::make_unique<macro_marker_container_t>();
    this->sources_sites->push_back({0, 0});

    this->token_container.push_back({token_index::index_begin, 0, 0, 0});

    if (flag.parse_macros) {
        this->do_parse_identifier = true;
        for (auto const& pair : *(this->macro_map)) {
            std::string key = pair.first;
            if (key.size() > 3) continue;
            utils::lower_case(key);
            if (key == "d") this->ambi_flag.ambiguity_d = true;
            if (key == "k") this->ambi_flag.ambiguity_k = true;
            if (key == "kl") this->ambi_flag.ambiguity_kl = true;
        }
    } else
        this->do_parse_identifier = false;
}

std::string tokenizer::token_string(token_t const& target) const {
    if (target.pos_cur == npos) return "";
    std::string const& src = *(this->sources->at(target.source_index));
    return src.substr(target.pos_cur, target.pos_next - target.pos_cur);
}

std::string tokenizer::tail(token_t const& target) const {
    const std::string& src = *(this->sources->front());
    if (target.source_index > 0) {
        size_t next_begin = this->sources_sites->at(target.source_index).macro_end;
        if (next_begin == npos) return rtail;
        while (src[next_begin] == ' ' && src[next_begin]) next_begin++;
        return src.substr(next_begin) + rtail;
    } else if (target.pos_next == npos)
        return rtail;
    else {
        size_t tail_start = target.pos_next;
        while (src[tail_start] == ' ' && src[tail_start]) tail_start++;
        return src.substr(tail_start) + rtail;
    }
}

token_t* tokenizer::cur_token() const { return &(this->token_container.back()); }

token_t* tokenizer::next_token() const {
    const token_t& tback = this->token_container.back();
    token_t temp_token = this->token_container.back();

    if (tback.id == token_index::index_begin)
        this->token_container.pop_back();
    else if (tback.id == token_index::index_stop)
        return &(this->token_container.back());

    temp_token.pos_next = this->sources->at(temp_token.source_index)->find_first_not_of(' ', temp_token.pos_next);

    if (temp_token.pos_next == npos || temp_token.pos_next >= this->sources->at(temp_token.source_index)->size()) {
        if (temp_token.source_index == 0) {
            temp_token = {token_index::index_stop, 0, npos, npos};
            this->token_container.emplace_back(std::move(temp_token));
            return &(this->token_container.back());
        } else {
            temp_token = this->token_container.back();
            temp_token.pos_next = (*this->sources_sites)[temp_token.source_index].macro_end;
            temp_token.source_index = 0;
            temp_token.pos_next = this->sources->at(temp_token.source_index)->find_first_not_of(' ', temp_token.pos_next);
            if (temp_token.pos_next == npos) {
                temp_token = {token_index::index_stop, 0, npos, npos};
                this->token_container.emplace_back(std::move(temp_token));
                return &(this->token_container.back());
            }
        }
    }

    temp_token.pos_cur = temp_token.pos_next;

    bool has_got = this->get_punctuator(temp_token) || this->get_number(temp_token);

    if (has_got) {
        this->token_container.emplace_back(std::move(temp_token));
        return &(this->token_container.back());
    }

    if (this->do_parse_identifier // parse macro only when parse identifier is
                                  // enabled
        && temp_token.source_index == 0 // parse macro only for origin source,
                                        // macros cannot contains macros
        && (this->sources_sites->back().macro_end == 0 // only the first one (for origin source) have a macro_end
                                                       // of 0
            || this->sources_sites->back().macro_end < temp_token.pos_next)) // last one is not a macro
    {
        has_got = this->resolve_identifier(temp_token);

        if (has_got && temp_token.id == token_index::index_macro) {
            this->sources_sites->push_back({temp_token.pos_cur, temp_token.pos_next});
            const std::string* expand_macro = &macro_map->find(this->token_string(temp_token))->second;
            this->sources->push_back(expand_macro);
            temp_token = {token_index::index_begin, this->sources->size() - 1, 0, 0};
            this->token_container.emplace_back(std::move(temp_token));
            return this->next_token();
        }
    }

    has_got = this->get_keyword(temp_token);

    if (!has_got) temp_token = {token_index::index_stop, 0, npos, npos};
    this->token_container.emplace_back(std::move(temp_token));
    return &(this->token_container.back());
}

bool tokenizer::get_punctuator(token_t& target) const {
    char t = this->sources->at(target.source_index)->at(target.pos_next);
    for (const punct_container& punc : punct_arr) {
        if (punc.name == t) {
            target.id = punc.pc;
            this->move_next_cursor(target, 1);
            return true;
        }
    }
    return false;
}

bool tokenizer::get_number(token_t& target) const {
    std::string const& src = *(this->sources->at(target.source_index));

    bool dotted = false;

    size_t start = target.pos_next;
    if (src[start] == '.') {
        dotted = true;
        start += 1;
    }

    size_t i = start;
    for (; i < src.size(); i++) {
        char a = src[i];
        if (a <= '9' && a >= '0')
            continue;
        else if (!dotted && a == '.') {
            dotted = true;
            continue;
        } else if (a == '%') {
            i++;
            break;
        } else
            break;
    }
    size_t src_len = i - target.pos_next;
    if (src_len) {
        target.id = token_index::index_number;
        this->move_next_cursor(target, src_len);
        return true;
    }
    return false;
}

bool tokenizer::get_keyword(token_t& target) const {
    std::string const& src = *(this->sources->at(target.source_index));
    size_t max_len = src.size() - target.pos_next;
    for (const keyword_container& keyw : keyword_arr) {
        size_t i = 0;
        for (; keyw.name[i] && i < max_len; i++) {
            if (keyw.name[i] == src[target.pos_cur + i])
                continue;
            else
                break;
        }
        if (keyw.name[i])
            continue;
        else {
            target.id = keyw.kw;
            this->move_next_cursor(target, i);
            return true;
        };
    }
    return false;
}

void tokenizer::move_next_cursor(token_t& src_token, size_t length) const {
    src_token.pos_cur = src_token.pos_next;
    src_token.pos_next = src_token.pos_next + length;
    std::string const& src = *(this->sources->at(src_token.source_index));

    if (src_token.pos_next >= src.size()) {
        src_token.pos_next = npos;
    }
}

std::pair<size_t, bool> greed_map_find(std::string const& source, size_t start_pos, tokenizer::macro_map_t const& map) {
    std::vector<char> source_cp;
    source_cp.reserve(source.size() + 1);

    size_t maxlen = source.find_first_of(illegal_identifier, start_pos);

    std::string::const_iterator end_point;
    if (maxlen == 0)
        return {0, false};
    else if (maxlen == npos)
        end_point = source.cend();
    else
        end_point = source.begin() + maxlen;
    std::copy(source.cbegin() + start_pos, end_point, std::back_inserter(source_cp));
    source_cp.push_back('\0');

    for (auto ri = source_cp.rbegin(); ri != source_cp.rend(); ri++) {
        auto target = map.find(source_cp.data());
        if (target != map.cend()) {
            return {target->first.size(), target->second.front() == '('};
        }
        *ri = '\0';
    }
    return {0, false};
}

bool tokenizer::resolve_identifier(token_t& target) const {
    // no macros inside macro
    if (target.source_index != 0) return 0;

    std::string const& src = *this->sources->at(target.source_index);
    bool terminate = false;

    // match macro at maximum length
    // thus if there is two macro such as "abc" and "abcde"
    // input "abcdef" would match "abcde"
    // "abcd" still match "abc"
    auto find_result = greed_map_find(src, target.pos_next, *this->macro_map);

    if (find_result.first == 0) return false;

    if (find_result.second) {
        // the macro is a random or dicelet expression
        target.id = token_index::index_macro;
        this->move_next_cursor(target, find_result.first);
        return true;
    } else if (find_result.first <= 2) {
        // the macro is a number, and it appears as a keyword
        // begin ambiguity deduction
        std::string suppose_macro = src.substr(target.pos_next, find_result.first);

        utils::lower_case(suppose_macro);

        do {
            if ((this->ambi_flag.ambiguity_d) && suppose_macro == "d" || (this->ambi_flag.ambiguity_k) && suppose_macro == "k"
                || (this->ambi_flag.ambiguity_kl) && suppose_macro == "kl")
                ;
            else
                break;

            token_t suppose_target = target;
            suppose_target.pos_next += find_result.first;
            if (suppose_target.pos_next >= this->sources->at(suppose_target.source_index)->size()) suppose_target.pos_next = npos;

            // if "d" "k" "kl" is a macro, it would work just
            // like const_unit/var_unit/dicelet_unit.
            // there is no way a number, ")", or "}" infront of
            // them. also this is sufficient to decide whether a
            // k/kl is allowed here, for k/kl is always following
            // a unit.
            if (!(this->token_container.empty())
                && (this->token_container.back().id == token_index::index_number
                    || this->token_container.back().id == token_index::punct_rparenthesis
                    || this->token_container.back().id == token_index::punct_rbrace)) {
                return false;
            }

            // The situation of multiple "d"s is a little more
            // complex. "d d" indicates the last "d" is a macro.
            // "d d d" indicates that the first and last "d" is
            // a maro.
            // In addition, "d d 'number'" indicates that the first "d" is a
            // macro
            if ((this->ambi_flag.ambiguity_d) && suppose_macro == "d") {
                if (!(this->token_container.empty()) && this->token_container.back().id == token_index::keyword_d) break;
                token_t tn = peek_next(suppose_target);
                if (tn.id == token_index::punct_lbrace || tn.id == token_index::punct_lparenthesis
                    || tn.id == token_index::index_number || tn.id == token_index::ambiguity_indentifier)
                    return false;

                if (tn.id == token_index::index_stop || tn.id == token_index::punct_rbrace
                    || tn.id == token_index::punct_rparenthesis || tn.id == token_index::punct_comma)
                    break;

                if (tn.id == token_index::ambiguity_macro_d) {
                    token_t tnn = peek_next(tn);
                    if (tnn.id == token_index::ambiguity_macro_d || tnn.id == token_index::index_number)
                        break;
                    else
                        return false;
                }
            }
        } while (false);
    }
    target.id = token_index::index_macro;
    this->move_next_cursor(target, find_result.first);
    return true;
}

token_t tokenizer::peek_next(token_t const& origin) const {
    token_t target = origin;
    if (target.pos_next == npos) {
        target = {token_index::index_stop, 0, npos, npos};
        return target;
    }
    bool has_got = get_punctuator(target) || get_number(target);
    if (has_got) return target;

    has_got = peek_identifier_d(target);
    if (!has_got) target.id = token_index::index_stop;
    return target;
}

bool tokenizer::peek_identifier_d(token_t& target) const {
    if (target.source_index != 0) return false;
    std::string const& src = *this->sources->at(target.source_index);
    target.pos_next = src.find_first_not_of(' ', target.pos_next);
    if (target.pos_next == npos) {
        target.id = token_index::index_stop;
        return false;
    }

    auto find_result = greed_map_find(src, target.pos_next, *this->macro_map);

    if (find_result.first >= 2 || find_result.first == 0) return false;

    if (find_result.second) {
        target.id = token_index::index_macro;
        this->move_next_cursor(target, find_result.first);
        return true;
    }

    std::string suppose_macro = src.substr(target.pos_next, find_result.first);
    utils::lower_case(suppose_macro);

    if (this->ambi_flag.ambiguity_d && suppose_macro == "d") {
        target.id = token_index::ambiguity_macro_d;
    } else
        target.id = token_index::ambiguity_indentifier;
    this->move_next_cursor(target, find_result.first);
    return true;
}

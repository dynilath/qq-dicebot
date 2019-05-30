#pragma once

#include <cstdint>
#include <deque>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "./token.h"

namespace dicebot::diceparser {

    struct macro_marker {
        size_t macro_begin;
        size_t macro_end;
    };

    using macro_marker_container_t = std::vector<macro_marker>;

    struct ambiguity_flag {
        bool ambiguity_d : 1;
        bool ambiguity_k : 1;
        bool ambiguity_kl : 1;
    };

    class tokenizer {
    public:
        using macro_map_t = std::unordered_map<std::string, std::string>;
        using macro_map_pair_t = typename macro_map_t::value_type;
        using sources_container_t = std::deque<const std::string *>;

    private:
        // string source, sources[0] is original source, the others are expanded macros, ordered by the sequence where it presents
        // in the original input
        std::unique_ptr<sources_container_t> sources;
        // tail string separated from input by parenthesis check
        std::string rtail;

        // the real source, with tail cutted by regulate_parenthesis
        std::string source;

        // map of macros, if do_parse_identifier = false, it is nullptr
        macro_map_t const *macro_map;
        // a vector that contains informations of where each macro begins an ends, ordered just like sources
        std::unique_ptr<macro_marker_container_t> sources_sites;

        ambiguity_flag ambi_flag;
        // whether or not the identifiers are parsed
        bool do_parse_identifier;

        bool get_punctuator(token_t &) const;
        bool get_keyword(token_t &) const;
        bool get_number(token_t &) const;
        bool resolve_identifier(token_t &) const;

        void move_next_cursor(token_t &, size_t length) const;

        token_t peek_next(token_t const &) const;
        bool peek_identifier_d(token_t &) const;

    public:
        using token_container_t = std::deque<token_t>;
        token_container_t &token_container;
        tokenizer(token_container_t &, tokenizer_flag const &, std::string const &, macro_map_t const * = nullptr) noexcept;

        token_t *cur_token() const;
        token_t *next_token() const;
        std::string cur_token_string() const;

        std::string token_string(token_t const &) const;
        std::string tail(token_t const &) const;
    };
} // namespace dicebot::diceparser
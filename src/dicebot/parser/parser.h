#pragma once

#include "../utils/number.h"
#include "./syntax.h"
#include "./token.h"
#include "./tokenizer.h"

namespace dicebot::diceparser {

    class parser {
        using macro_map_t = std::map<std::string, std::string>;
        // macro_map_t const *p_macro_map;
        tokenizer const& tknzer;
        // std::unique_ptr<std::vector<uint8_t>> vec_status;
        std::unique_ptr<std::vector<p_syntax_item>> vec_symbols;
        // std::unique_ptr<std::list<token_t>> tokens;

    public:
        std::string tail;
        std::string source;
        parser(tokenizer const&) noexcept;
        p_syntax_item parse();
        p_syntax_item resolve(uint8_t) const;
        p_syntax_item stack(token_t, std::string&&) const;
    };
} // namespace dicebot::diceparser
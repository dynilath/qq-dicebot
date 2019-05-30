#pragma once

#include <cstdint>
#include <memory>
#include "./token.h"

namespace dicebot::diceparser {
    enum terminals : uint8_t {
        terminal_number = 0,
        terminal_lparenthesis = 1,
        terminal_rparenthesis = 2,
        terminal_mul_dvi = 3,
        terminal_minus = 4,
        terminal_plus = 5,
        terminal_roll_d = 6,
        terminal_roll_k = 7,
        terminal_roll_kl = 8,
        terminal_sharp = 9,
        terminal_lbrace = 10,
        terminal_rbrace = 11,
        terminal_comma = 12,
        terminal_end = 13
    };
    enum nterminals : uint8_t {
        nterminal_S = 0,
        nterminal_acc = 1,
        nterminal_const_expr = 2,
        nterminal_rand_expr = 3,
        nterminal_dicelet_expr = 4,
        nterminal_const_unit = 5,
        nterminal_const_mul = 6,
        nterminal_rand_unit = 7,
        nterminal_rand_mul = 8,
        nterminal_dicelet_u = 9,
        nterminal_dicelet_ct = 10,
        nterminal_dicelet_mul = 11,
    };

    enum class production_type : uint8_t {
        S_acc,
        acc_const_expr,
        acc_rand_expr,
        acc_dicelet_expr,
        const_unit_number,
        const_unit_lparenthesis_const_expr_rparenthesis,
        const_mul_const_unit,
        const_mul_const_mul_muldvi_const_unit,
        const_expr_const_mul,
        const_expr_minus_const_mul,
        const_expr_const_expr_plus_const_mul,
        const_expr_const_expr_minus_const_mul,
        rand_unit_d_const_unit,
        rand_unit_const_unit_d_const_unit,
        rand_unit_const_unit_d_const_unit_k_const_unit,
        rand_unit_const_unit_d_const_unit_kl_const_unit,
        rand_unit_lparenthesis_rand_expr_rparenthesis,
        rand_mul_rand_unit,
        rand_mul_const_mul_muldvi_rand_unit,
        rand_mul_rand_mul_muldvi_const_unit,
        rand_mul_rand_mul_muldvi_rand_unit,
        rand_expr_rand_mul,
        rand_expr_minus_rand_mul,
        rand_expr_const_expr_plus_rand_mul,
        rand_expr_const_expr_minus_rand_mul,
        rand_expr_rand_expr_plus_const_mul,
        rand_expr_rand_expr_minus_const_mul,
        rand_expr_rand_expr_plus_rand_mul,
        rand_expr_rand_expr_minus_rand_mul,
        dicelet_u_const_unit_sharp_const_unit,
        dicelet_u_const_unit_sharp_rand_unit,
        dicelet_u_lbrace_dicelet_ct_rbrace,
        dicelet_u_lparenthesis_dicelet_expr_rparenthesis,
        dicelet_ct_const_expr,
        dicelet_ct_rand_expr,
        dicelet_ct_dicelet_ct_comma_const_expr,
        dicelet_ct_dicelet_ct_comma_rand_expr,
        dicelet_mul_dicelet_u,
        dicelet_mul_const_mul_muldvi_dicelet_u,
        dicelet_mul_rand_mul_muldvi_dicelet_u,
        dicelet_mul_dicelet_mul_muldvi_const_unit,
        dicelet_mul_dicelet_mul_muldvi_rand_unit,
        dicelet_mul_dicelet_mul_muldvi_dicelet_u,
        dicelet_expr_dicelet_mul,
        dicelet_expr_minus_dicelet_mul,
        dicelet_expr_const_expr_plus_dicelet_mul,
        dicelet_expr_const_expr_minus_dicelet_mul,
        dicelet_expr_rand_expr_plus_dicelet_mul,
        dicelet_expr_rand_expr_minus_dicelet_mul,
        dicelet_expr_dicelet_expr_plus_const_mul,
        dicelet_expr_dicelet_expr_minus_const_mul,
        dicelet_expr_dicelet_expr_plus_rand_mul,
        dicelet_expr_dicelet_expr_minus_rand_mul,
        dicelet_expr_dicelet_expr_plus_dicelet_mul,
        dicelet_expr_dicelet_expr_minus_dicelet_mul
    };

    class syntax_item {
    public:
        uint8_t type;
        token_t source_token;
        virtual bool is_terminal() const { return false; }
    };

    using p_syntax_item = std::shared_ptr<syntax_item>;

    class syntax_terminal : public syntax_item {
    public:
        std::string source;

        syntax_terminal(token_t t, uint8_t type, std::string&& src) {
            this->type = type;
            this->source_token = t;
            this->source = std::move(src);
        }

        bool is_terminal() const override { return true; }
    };

    class syntax_nterminal : public syntax_item {
    public:
        size_t source_index;
        size_t end_pos;
        production_type prod_type;
        p_syntax_item items[5];

        syntax_nterminal(token_t end_token, uint8_t type,
                         production_type prod_type) {
            this->type = type;
            this->prod_type = prod_type;
            this->source_token = end_token;
        }
        bool is_terminal() const override { return false; }
    };
} // namespace dicebot::diceparser
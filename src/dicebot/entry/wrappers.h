#pragma once

#include <string>
#include "../dice_excepts.h"
#include "../event_info.h"
#include "../parser/dicenalyzer.h"
#include "../parser/parser.h"
#include "../parser/tokenizer.h"
#include "./output_constructor.h"

namespace dicebot {

    template <class work_t>
    static bool shoot_exceptions(work_t fun, std::string const& message, event_info& event, std::string& response) {
        try {
            return fun(message, event, response);
        } catch (dicebot::zero_divider_exception const&) {
            output_constructor oc(event.nickname);
            oc << u8"指令中以0为除数";
            response = oc;
            return true;
        } catch (dicebot::dice_exceed const&) {
            output_constructor oc(event.nickname);
            oc << u8"投掷过多骰子，最大为" << MAX_DICE_NUM;
            response = oc;
            return true;
        } catch (dicebot::face_exceed const&) {
            output_constructor oc(event.nickname);
            oc << u8"投掷骰子面数过多，最大为" << MAX_DICE_FACE;
            response = oc;
            return true;
        } catch (dicebot::unit_exceed const&) {
            output_constructor oc(event.nickname);
            oc << u8"过多重复投掷骰子，最大为" << MAX_DICE_UNIT_COUNT;
            response = oc;
            return true;
        } catch (dicebot::invalid_dice const&) {
            output_constructor oc(event.nickname);
            oc << u8"骰子的数量、面数、重复次数必须为正整数";
            response = oc;
            return true;
        } catch (dicebot::invalid_macro const&) {
            output_constructor oc(event.nickname);
            oc << u8"骰子指令名称中包含不可用字符";
            response = oc;
            return true;
        } catch (std::out_of_range const&) {
            output_constructor oc(event.nickname);
            oc << u8"数值超出计算范围";
            response = oc;
            return true;
        }
    }

    template <class container_t, class excutable_t, class item_t = typename container_t::value_type>
    inline std::string result_builder(const char* prefix, const container_t& src, excutable_t strconv, const char* separater,
                                      const char* suffix) {
        std::string ret;
        ret.assign(prefix);
        bool is_first = true;
        for (const auto& item : src) {
            if (is_first)
                is_first = false;
            else
                ret.append(separater);
            ret.append(strconv(item));
        }
        ret.append(suffix);
        return ret;
    };

    namespace diceparser {
        template <class _run_when_const, class _run_when_rand, class _run_when_dicelet>
        inline bool result_brancher(const std::string& src, std::string& cmd_out, std::string& tail_out,
                                    tokenizer::macro_map_t const* map, _run_when_const run_const_expr,
                                    _run_when_rand run_rand_expr, _run_when_dicelet run_dicelet_expr) {
            tokenizer::token_container_t tk_cont;
            tokenizer tknz(tk_cont, {true, true}, src, map);
            parser parser(tknz);
            auto p_syntax = parser.parse();
            if (!p_syntax) return false;
            auto p_dice = build_component_from_syntax(p_syntax.get());
            if (!p_dice) return false;

            {
                str_container strs_command;
                p_dice->print(strs_command);
                cmd_out.append(strs_command.str());
            }

            tail_out = std::move(parser.tail);

            auto p_const = std::dynamic_pointer_cast<comp_number>(p_dice);
            if (p_const) {
                run_const_expr(p_const);
                return true;
            }

            auto p_dicelet = std::dynamic_pointer_cast<dicelet>(p_dice);
            if (p_dicelet) {
                run_dicelet_expr(p_dicelet);
                return true;
            }

            run_rand_expr(p_dice);
            return true;
        }
    } // namespace diceparser

} // namespace dicebot

#pragma once

#include "./common.h"

namespace dicebot::roll{

    #define CHECK_LIMITS(_Num, _Face) \
    ((_Face < MAX_DICE_FACE && _Num < MAX_DICE_NUM) &&\
    (_Face > 1 && _Num >= 1))
    
    typedef std::pair<int32_t,bool> dice_pair;

    class dice_roll{
    private:
        dice_roll(roll_status const & status) noexcept;
    public:
        int32_t summary;
        roll_status status;
        std::vector<dice_pair> results;
        std::string detail();
        std::string dice_roll::detail_coc();
        dice_roll() noexcept;
        size_t add_result(int32_t const result);
        size_t add_ignored_result(int32_t const result);
        size_t update_result(int32_t const result, size_t const target);
        size_t set_ignore(size_t const target);
        size_t set_good(size_t const target);
        size_t clear();
        roll_status finish_roll() noexcept;
        roll_status finish_coc() noexcept;
        roll_status finish_wod(int const i_d, bool const failing) noexcept;
        roll_status dice_exceed() noexcept;
        roll_status general_err() noexcept;
        explicit operator bool() const noexcept;
    };
    
    typedef std::shared_ptr<dice_roll> p_dice_roll;

    roll_status roll_base(dice_roll & dice, int const i_num_of_dice, int const i_num_of_face) noexcept;

    roll_status roll_rdk(dice_roll & dice, int const i_num_of_dice, int const i_num_of_face, int const i_keep) noexcept;
    roll_status roll_rdk(dice_roll & dice, std::string const & str_dice_command) noexcept;

    roll_status roll_coc(dice_roll & dice, int const i_bp) noexcept;
    roll_status roll_coc(dice_roll & dice, std::string const & str_dice_command) noexcept;

    roll_status roll_wod(dice_roll & dice, int const i_val, int const i_d, int const i_bonus, bool failing) noexcept;
    roll_status roll_nwod(dice_roll & dice, std::string const & str_dice_command) noexcept;
    roll_status roll_owod(dice_roll & dice, std::string const & str_dice_command) noexcept;

    roll_status roll_fate(dice_roll & dice) noexcept;

    void random_initialize();
}
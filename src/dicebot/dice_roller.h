#pragma once

#include "./common.h"

namespace dicebot::roll{
	class dice_roll{
	private:
		dice_roll(roll_status const & status) noexcept;
	public:
		int result;
		std::string detail;
		roll_status status;
		dice_roll(int const & i_result, std::string const & str_detail, roll_status const & stat) noexcept;
		explicit operator bool() const noexcept;
		static dice_roll ERR_ROLL_GENERAL;
		static dice_roll ERR_ROLL_EXCEED;
	};

	dice_roll roll_base(int const i_num_of_dice, int const i_num_of_face) noexcept;
	dice_roll roll_rdk(int const i_num_of_dice, int const i_num_of_face, int const i_keep) noexcept;
	dice_roll roll_rdk(std::string & const str_dice_command) noexcept;
	dice_roll roll_coc(int const i_bp) noexcept;
	dice_roll roll_coc(std::string & const str_dice_command) noexcept;


	void random_initialize();
}
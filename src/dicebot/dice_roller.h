#pragma once

#include "./common.h"

namespace dicebot::roll{

	class dice_roller
	{
	private:
		static bool is_using_pseudo_random;
		static unsigned long ulong_prand_seed;
		static unsigned long ulong_prand_stage;
	public:
		roll_status status;
		dice_roller() noexcept;
		~dice_roller();
		std::string * pstr_detail_result = nullptr;
		int i_sum_result = 0;
		dice_roller(int val1_i_num_of_dice, int val2_num_of_face, int start_value = 1);
		dice_roller(int num_of_dice, int num_of_face, int keep, bool is_keeping_high, int start_value = 1);
		dice_roller(std::string & str_dice_command, roll_mode mode = roll_mode::DND_DK);
		static void random_initialize();
	};
}
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
		dice_roller(int val1_i_num_of_dice, int val2_num_of_face);
		dice_roller(int num_of_dice, int num_of_face, int keep, bool is_keeping_high);
		dice_roller(int val1_bp);
		std::string * pstr_detail_result = nullptr;
		int i_sum_result = 0;
		static std::shared_ptr<dice_roller> roll_base(int i_num_of_dice, int i_num_of_face) noexcept;
		static std::shared_ptr<dice_roller> roll_rd(std::string & str_dice_command) noexcept;
		static std::shared_ptr<dice_roller> roll_coc(std::string & str_dice_command) noexcept;
		static std::shared_ptr<dice_roller> roll_wod(std::string & str_dice_command) noexcept;
		static void random_initialize();
	};

	typedef std::shared_ptr<dice_roller> p_dice_roller;
}
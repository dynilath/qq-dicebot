#pragma once

class dice_roller
{
private:
	static bool is_using_pseudo_random;
	static unsigned long ulong_prand_seed;
	static unsigned long ulong_prand_stage;
public:
	static const int ROLL_STATUS_FINISHED = 0;
	static const int ROLL_STATUS_GENERAL_ERR = 1;
	static const int ROLL_STATUS_DICE_NOT_AVAILABLE = 2;
	static const int ROLL_STATUS_UNINITIALIZED = -1;
	static const int ROLL_STATUS_TOO_MANY_DICE = 3;
	static const int ROLL_MODE_DND_DK = 1;
	static const int ROLL_MODE_COC_PB = 2;

	int status;
	dice_roller() noexcept;
	~dice_roller();
	std::string * pstr_detail_result = nullptr;
	int i_sum_result = 0;
	dice_roller(int val1_i_num_of_dice, int val2_num_of_face, int start_value = 1);
	dice_roller(int num_of_dice, int num_of_face, int keep, bool is_keeping_high, int start_value = 1);
	dice_roller(std::string & str_dice_command, int mode = ROLL_MODE_DND_DK);
	static void random_initialize();
};


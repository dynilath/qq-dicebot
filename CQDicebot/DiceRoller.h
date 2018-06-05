#pragma once

#define ROLL_STATUS_FINISHED 0
#define ROLL_STATUS_GENERAL_ERR 1
#define ROLL_STATUS_DICE_NOT_AVAILABLE 2
#define ROLL_STATUS_UNINITIALIZED -1
#define ROLL_STATUS_TOO_MANY_DICE 3

#define ROLL_MODE_DND_DK 1
#define ROLL_MODE_COC_PB 2
class DiceRoller
{
private:
	static bool is_using_pseudo_random;
	static unsigned long ulong_prand_seed;
	static unsigned long ulong_prand_stage;
public:
	int status;
	DiceRoller() noexcept;
	~DiceRoller();
	std::string * pstr_detail_result = nullptr;
	int i_sum_result = 0;
	DiceRoller(int val1_i_num_of_dice, int val2_num_of_face, int start_value = 1);
	DiceRoller(int num_of_dice, int num_of_face, int keep, bool is_keeping_high, int start_value = 1);
	DiceRoller(std::string & str_dice_command, int mode = ROLL_MODE_DND_DK);
	static void random_initialize();
};


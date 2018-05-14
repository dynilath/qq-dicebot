#pragma once

class DiceRoller
{
private:
	static bool is_using_pseudo_random;
	static unsigned long ulong_prand_seed;
	static unsigned long ulong_prand_stage;
public:
	DiceRoller() noexcept;
	~DiceRoller();
	std::string * str_detail_result = nullptr;
	int i_sum_result = 0;
	DiceRoller(int val1_i_num_of_dice, int val2_num_of_face);
	DiceRoller(int num_of_dice, int num_of_face, int keep, bool is_keeping_high);
	DiceRoller(std::string & str_dice_command);
	static void random_initialize();
};


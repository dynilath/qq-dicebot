#pragma once

class DiceRoller
{
public:
	DiceRoller();
	~DiceRoller();
	std::string  str_detail_result;
	int i_sum_result;
	DiceRoller(int val1_i_num_of_dice, int val2_num_of_face);
	DiceRoller(int num_of_dice, int num_of_face, int keep, bool is_keeping_high);
};


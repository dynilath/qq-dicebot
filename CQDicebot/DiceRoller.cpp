#include "stdafx.h"
#include "DiceRoller.h"
#include "utility.h"
#include <random>
#include <vector>
DiceRoller::DiceRoller() noexcept
{
}


DiceRoller::~DiceRoller()
{
}

DiceRoller::DiceRoller(int val1_i_num_of_dice, int val2_num_of_face) {
	std::random_device rd;
	std::mt19937 generator(rd());
	int i_result_sum = 0;
	std::uniform_int_distribution<> dice(1, val2_num_of_face);
	std::ostringstream ostrs_dice_stream(std::ostringstream::ate);
	while (val1_i_num_of_dice > 0)
	{
		int i_step_result = dice(generator);
		i_result_sum += i_step_result;
		ostrs_dice_stream << i_step_result;
		if ((--val1_i_num_of_dice) > 0) ostrs_dice_stream << " + ";
	}
	this->str_detail_result = ostrs_dice_stream.str();
	this->i_sum_result = i_result_sum;
}

DiceRoller::DiceRoller(int num_of_dice, int num_of_face, int keep, bool is_keeping_high) {
	if (keep >= num_of_dice) {
		DiceRoller dice(num_of_dice, num_of_face);
		this->str_detail_result = dice.str_detail_result;
		this->i_sum_result = dice.i_sum_result;
	}
	else {

		std::random_device rd;
		std::mt19937 generator(rd());
		int i_result_sum = 0;
		std::uniform_int_distribution<> dice(1, num_of_face);
		std::ostringstream ostrs_dice_stream(std::ostringstream::ate);

		std::vector<int> resultList;
		std::vector<int> sortList;
		std::vector<int> pivotList;
		std::vector<int> flagList;

		for (int i_count = 0; i_count < num_of_dice; i_count++) {
			int i_temp_result = dice(generator);
			resultList.push_back(i_temp_result);
			sortList.push_back(i_temp_result);
			pivotList.push_back(i_count);
			flagList.push_back(0);
		}
		quickSort(sortList.data(), pivotList.data(), 0, sortList.size() - 1);

		if (is_keeping_high) {
			for (int i_iter = 0; i_iter < keep; i_iter++) {
				flagList[pivotList[i_iter]] = 1;
			}
		}
		else {
			for (int i_iter = 1; i_iter <= keep; i_iter++) {
				flagList[pivotList[pivotList.size() - i_iter]] = 1;
			}
		}

		for (int i_iter = 0; i_iter < num_of_dice; i_iter++) {
			if (flagList[i_iter] == 1) {
				i_result_sum += resultList[i_iter];
				ostrs_dice_stream << resultList[i_iter];
			}
			else {
				ostrs_dice_stream << "(" << resultList[i_iter] << ")";
			}
			if (i_iter + 1 < num_of_dice) ostrs_dice_stream << " + ";
		}
		this->str_detail_result = ostrs_dice_stream.str();
		this->i_sum_result = i_result_sum;
	}
}

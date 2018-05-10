#include "stdafx.h"
#include "DiceRoller.h"
#include "utility.h"
#include <random>
#include <vector>
#include <chrono>

#define FUNCTION_STOI_STRING_PART(_Str_target,_Int_begin,_Int_end) std::stoi(_Str_target.substr(_Int_begin, _Int_end - (_Int_begin)))

#define FUNCTION_PARSE_DICE(_Str_target,_Pos_Start,_Pos_D,_Pos_K,_Pos_L) \
int i_num_of_die = i_pos_of_d > 0 ? FUNCTION_STOI_STRING_PART(_Str_target, _Pos_Start, _Pos_D) : 1;\
int i_face_of_die = FUNCTION_STOI_STRING_PART(_Str_target, _Pos_D + 1, _Pos_K);\
int i_num_of_keep = FUNCTION_STOI_STRING_PART(_Str_target, _Pos_L + 1, _Str_target.length())

#define CHECK_DICE_LIMITS() if (i_face_of_die > MAX_DICE_FACE || i_num_of_keep > MAX_DICE_FACE || i_num_of_die > MAX_DICE_NUM) break

bool DiceRoller::is_using_pseudo_random = false;
unsigned long DiceRoller::ulong_prand_seed = 0;
unsigned long DiceRoller::ulong_prand_stage = 0;

DiceRoller::DiceRoller() noexcept
{
	this->str_detail_result = nullptr;
}


DiceRoller::~DiceRoller()
{
	if (str_detail_result != nullptr)
		free(str_detail_result);
}

DiceRoller::DiceRoller(int val1_i_num_of_dice, int val2_num_of_face) {
	str_detail_result = nullptr;
	std::mt19937 mt_generator(ulong_prand_seed);
	mt_generator.discard(ulong_prand_stage);
	std::random_device rd_generator;

	int i_result_sum = 0;
	std::uniform_int_distribution<> dice(1, val2_num_of_face);
	std::ostringstream ostrs_dice_stream(std::ostringstream::ate);
	while (val1_i_num_of_dice > 0)
	{
		int i_step_result = 0;
		if (DiceRoller::is_using_pseudo_random) {
			i_step_result = dice(mt_generator);
			ulong_prand_stage++;
		}
		else i_step_result = dice(rd_generator);
		i_result_sum += i_step_result;
		ostrs_dice_stream << i_step_result;
		if ((--val1_i_num_of_dice) > 0) ostrs_dice_stream << " + ";
	}
	this->str_detail_result = new std::string(ostrs_dice_stream.str());
	this->i_sum_result = i_result_sum;
}

DiceRoller::DiceRoller(int num_of_dice, int num_of_face, int keep, bool is_keeping_high) {
	str_detail_result = nullptr;
	std::mt19937 mt_generator(ulong_prand_seed);
	mt_generator.discard(ulong_prand_stage);
	std::random_device rd_generator;

	if (keep >= num_of_dice) {
		DiceRoller dice(num_of_dice, num_of_face);
		this->str_detail_result = new std::string(*dice.str_detail_result);
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
			int i_temp_result = 0;
			if (DiceRoller::is_using_pseudo_random) {
				i_temp_result = dice(mt_generator);
				ulong_prand_stage++;
			}
			else i_temp_result = dice(rd_generator);
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
		this->str_detail_result = new std::string(ostrs_dice_stream.str());
		this->i_sum_result = i_result_sum;
	}
}

//inputs regex (\\+|\\-)?(\\d*d\\d+((k|kl)\\d+)?)|(\\d+)
DiceRoller::DiceRoller(std::string & str_single_dice)
{
	this->str_detail_result = nullptr;
	bool is_first_signed = (str_single_dice[0] == '+' || str_single_dice[0] == '-');
	std::ostringstream ostrs_dice_stream(std::ostringstream::ate);
	
	int i_dice_summary = 0;

	int i_single_dice_sign = 1;

	int i_start_pos = is_first_signed ? 1 : 0;

	if (is_first_signed) {
		ostrs_dice_stream << " " << str_single_dice[0] << " ";
		if (str_single_dice[0] == '-') i_single_dice_sign = -1;
		else i_single_dice_sign = 1;
	}

	do {
		int i_pos_of_d = str_single_dice.find('d');
		if (i_pos_of_d == std::string::npos) {
			int result = std::stoi(str_single_dice.substr(i_start_pos));
			this->i_sum_result = result * i_single_dice_sign;
			ostrs_dice_stream << result;
			this->str_detail_result = new std::string(ostrs_dice_stream.str());
		}
		else {
			int i_pos_of_k = str_single_dice.find('k');
			if (i_pos_of_k == std::string::npos) {
				int i_num_of_die = i_pos_of_d > 0 ? std::stoi(str_single_dice.substr(i_start_pos, i_pos_of_d)) : 1;
				int i_face_of_die = std::stoi(str_single_dice.substr(i_pos_of_d + 1, str_single_dice.length() - i_pos_of_d - 1));
				int i_num_of_keep = i_num_of_die;
				CHECK_DICE_LIMITS();
				DiceRoller dr_diceRoll(i_num_of_die, i_face_of_die);
				ostrs_dice_stream << "(" << *(dr_diceRoll.str_detail_result) << ")";
				this->i_sum_result = dr_diceRoll.i_sum_result * i_single_dice_sign;
				this->str_detail_result = new std::string(ostrs_dice_stream.str());
			}
			else {
				int i_pos_of_l = str_single_dice.find('l');
				if (i_pos_of_l == std::string::npos) {
					i_pos_of_l = i_pos_of_k;
					FUNCTION_PARSE_DICE(str_single_dice, i_start_pos,i_pos_of_d, i_pos_of_k, i_pos_of_l);
					CHECK_DICE_LIMITS();
					DiceRoller dr_diceRoll(i_num_of_die, i_face_of_die, i_num_of_keep, false);
					ostrs_dice_stream << "(" << *(dr_diceRoll.str_detail_result) << ")";
					this->i_sum_result = dr_diceRoll.i_sum_result * i_single_dice_sign;
					this->str_detail_result = new std::string(ostrs_dice_stream.str());
				}
				else {
					FUNCTION_PARSE_DICE(str_single_dice, i_start_pos,i_pos_of_d, i_pos_of_k, i_pos_of_l);
					CHECK_DICE_LIMITS();
					DiceRoller dr_diceRoll(i_num_of_die, i_face_of_die, i_num_of_keep, true);
					ostrs_dice_stream << "(" << *(dr_diceRoll.str_detail_result) << ")";
					this->i_sum_result = dr_diceRoll.i_sum_result * i_single_dice_sign;
					this->str_detail_result = new std::string(ostrs_dice_stream.str());
				}
			}
		}
	} while (false);
}

void DiceRoller::random_initialize()
{
	std::random_device rd;
	if (rd.entropy() == 0.0) {
		DiceRoller::is_using_pseudo_random = true;
	}
}

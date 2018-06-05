#include "stdafx.h"
#include "diceSpliter.h"
#include <regex>
#include "diceroller.h"

std::regex regex_single_dice("(\\+|\\-)?((\\d*d\\d+((k|kl)\\d+)?)|(\\d+))");

#define PCAL_WITH_IS_DOUBLE_CHECK(_D_Mode,_I_Mode,_OPERATOR,_FACTOR) \
if (is_using_double) _D_Mode _OPERATOR _FACTOR;\
else _I_Mode _OPERATOR _FACTOR

#define SWITCH_IS_DOUBLE_MODE { d_dice_buffer_level_1 = i_dice_buffer_level_1;\
d_dice_summary = i_dice_summary;\
is_using_double = true;}

#define PCAL_TYPE_ADD 0
#define PCAL_TYPE_MUL 1
#define PCAL_TYPE_DVI 2
#define PCAL_TYPE_INIT -1

bool baseSplitDice(const std::string & str_input, std::string & str_output) {
	std::string str_input_copy(str_input);
	std::smatch matchList_single_dice;

	std::ostringstream ostrs_dice_stream(std::ostringstream::ate);
	ostrs_dice_stream << str_input << " = ";

	INT32 i_dice_summary = 0;
	double d_dice_summary = 0.0;

	bool is_this_line_output = true;

	int i_cal_level = 0;
	bool is_using_double = false;

	INT32 i_dice_buffer_level_1 = 0;
	double d_dice_buffer_level_1 = 0.0;

	INT32 i_dice_result = 0;

	int i_dice_cal_type = PCAL_TYPE_INIT;

	int i_max_unit_alert = 0;
	for (; i_max_unit_alert < MAX_DICE_UNIT_COUNT; i_max_unit_alert++) {


		if (!std::regex_search(str_input_copy, matchList_single_dice, regex_single_dice)) { break; }
		std::string str_single_dice = matchList_single_dice.str();
		DiceRoller dr_single_dice(str_single_dice);
		if (dr_single_dice.status != ROLL_STATUS_FINISHED) { is_this_line_output = false; break; }
		i_dice_result = dr_single_dice.i_sum_result;


		ostrs_dice_stream << *(dr_single_dice.pstr_detail_result);
		str_input_copy = matchList_single_dice.suffix().str();

		if (i_dice_cal_type == PCAL_TYPE_MUL) {
			if ((!is_using_double) && INT32_MAX / i_dice_result < i_dice_buffer_level_1)
				SWITCH_IS_DOUBLE_MODE;
			PCAL_WITH_IS_DOUBLE_CHECK(d_dice_buffer_level_1, i_dice_buffer_level_1, *=, i_dice_result);
		}
		else if (i_dice_cal_type == PCAL_TYPE_DVI) {
			if (i_dice_result == 0) {
				is_this_line_output = false;
				break;
			}
			if ((!is_using_double) && i_dice_buffer_level_1 % i_dice_result != 0)
				SWITCH_IS_DOUBLE_MODE;
			PCAL_WITH_IS_DOUBLE_CHECK(d_dice_buffer_level_1, i_dice_buffer_level_1, /=, i_dice_result);
		}

		if (str_input_copy.length() > 0 && (str_input_copy[0] == '*' || str_input_copy[0] == '/')) {
			ostrs_dice_stream << ' ' << str_input_copy[0] << ' ';
			if (i_cal_level == 0) {
				PCAL_WITH_IS_DOUBLE_CHECK(d_dice_buffer_level_1, i_dice_buffer_level_1, =, i_dice_result);
				i_cal_level = 1;
			}
			if (str_input_copy[0] == '*') {
				i_dice_cal_type = PCAL_TYPE_MUL;
			}
			else if (str_input_copy[0] == '/') {
				i_dice_cal_type = PCAL_TYPE_DVI;
			}
		}
		else {

			if (i_cal_level > 0) {
				i_cal_level = 0;
				if (is_using_double) d_dice_summary += d_dice_buffer_level_1;
				else i_dice_summary += i_dice_buffer_level_1;
			}
			else PCAL_WITH_IS_DOUBLE_CHECK(d_dice_summary, i_dice_summary, +=, i_dice_result);
			if (str_input_copy.length() == 0) break;
			else i_dice_cal_type = PCAL_TYPE_ADD;
		}
	}

	if (i_max_unit_alert == MAX_DICE_UNIT_COUNT) is_this_line_output = false;

	if (is_this_line_output) {
		ostrs_dice_stream << " = ";
		if (is_using_double) {
			ostrs_dice_stream << d_dice_summary;
		}
		else {
			ostrs_dice_stream << i_dice_summary;
		}
		str_output.assign(ostrs_dice_stream.str());
	}
	return is_this_line_output;
}

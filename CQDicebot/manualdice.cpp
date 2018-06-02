#include "stdafx.h"
#include "manualdice.h"
#include <regex>
#include "diceroller.h"

std::regex regex_filter_manual_dice("^((\\+)?\\d*d\\d+)");

manualDice::manualDice()
{
	pintlist_dice = new TYPE_LIST_DICE();
	i_sum_result = 0;
}


manualDice::~manualDice()
{
}

manualDice::manualDice(const std::string & source)
{
	this->status = ROLL_STATUS_UNINITIALIZED;
	try {
		std::string str_source_copy(source);
		std::smatch smatch_single_manual_dice;
		while (str_source_copy.length() > 2) {
			std::regex_search(str_source_copy, regex_filter_manual_dice);
			if (smatch_single_manual_dice.begin() != smatch_single_manual_dice.end()) {
				std::string command = smatch_single_manual_dice.str();
				int i_pos_of_d = command.find('d');
				if (i_pos_of_d != std::string::npos) {
					int i_num_of_die = i_pos_of_d > 0 ? std::stoi(command.substr(0, i_pos_of_d)) : 1;
					int i_face_of_die = std::stoi(command.substr(i_pos_of_d + 1));
					for (int i_iter = 0; i_iter < i_num_of_die; i_iter++) {
						DiceRoller dice(1, i_face_of_die);
						if (dice.status != ROLL_STATUS_FINISHED) {
							this->status = dice.status;
							return;
						}
						this->pintlist_dice->push_back(TYPE_PAIR_DICE(i_face_of_die, dice.i_sum_result));
					}
				}
				str_source_copy = smatch_single_manual_dice.suffix().str();
			}
		}
	}
	catch (const std::invalid_argument& ia) {
		this->status = ROLL_STATUS_DICE_NOT_AVAILABLE;
	}
	if (this->status == ROLL_STATUS_UNINITIALIZED) this->status = ROLL_STATUS_FINISHED;
}

void manualDice::roll(const std::string & source)
{
	this->status = ROLL_STATUS_UNINITIALIZED;
	try {
		unsigned int target = std::stoi(source);
		if (target >= this->pintlist_dice->size()) return;
		else if (target < 0) return;
		TYPE_LIST_DICE::iterator iter_list = this->pintlist_dice->begin() + target;
		int i_face_of_die = (*iter_list).first;
		i_sum_result -= (*iter_list).second;
		DiceRoller dice(1, i_face_of_die);
		if (dice.status != ROLL_STATUS_FINISHED){
			this->status = dice.status;
			return;
		}
		(*iter_list).second = dice.i_sum_result;
		i_sum_result += dice.i_sum_result;
		if (this->status == ROLL_STATUS_UNINITIALIZED) this->status = ROLL_STATUS_FINISHED;
	}
	catch (const std::invalid_argument& ia) {
		this->status = ROLL_STATUS_DICE_NOT_AVAILABLE;
	}
}

void manualDice::kill(const std::string & source)
{
	this->status = ROLL_STATUS_UNINITIALIZED;
	try{
		unsigned int target = std::stoi(source);
		if (target >= this->pintlist_dice->size()) return;
		else if (target < 0) return;
		TYPE_LIST_DICE::iterator iter_list = (this->pintlist_dice->begin()) + target;
		i_sum_result -= (*iter_list).second;
		this->pintlist_dice->erase((iter_list));
		if (this->status == ROLL_STATUS_UNINITIALIZED) this->status = ROLL_STATUS_FINISHED;
	}
	catch (const std::invalid_argument& ia) {
		this->status = ROLL_STATUS_DICE_NOT_AVAILABLE;
	}
}

std::string manualDice::endcode()
{
	return std::string();
}

std::string manualDice::ToString()
{
	std::ostringstream ostrs_result(std::ostringstream::ate);
	int i_sum_result = 0;
	for (TYPE_LIST_DICE::iterator iter_list = this->pintlist_dice->begin(); iter_list != this->pintlist_dice->end(); iter_list++) {
		if (iter_list != this->pintlist_dice->begin()) {
			ostrs_result << " + ";
		}
		ostrs_result << (*iter_list).second << "(" << (*iter_list).first << ")";
		i_sum_result += (*iter_list).second;
	}
	return ostrs_result.str();
}

void manualDice::decode(std::string & source)
{
}

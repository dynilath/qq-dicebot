#include "stdafx.h"
#include "manualdice.h"
#include <regex>
#include "diceroller.h"
#include "encoder.h"

std::regex regex_filter_manual_dice_part("^((\\+)?\\d*d\\d+)");

manual_dice::manual_dice()
{
	pintlist_dice = new TYPE_LIST_DICE();
	i_sum_result = 0; 
}


manual_dice::~manual_dice()
{
}

manual_dice::manual_dice(const std::string & source)
{
	this->pintlist_dice = new TYPE_LIST_DICE();
	i_sum_result = 0;
	this->status = dice_roller::ROLL_STATUS_UNINITIALIZED;
	try {
		std::string str_source_copy(source);
		std::smatch smatch_single_manual_dice;
		int i_max_unit_alert = 0;
		for (; i_max_unit_alert < MAX_DICE_UNIT_COUNT; i_max_unit_alert++) {
			std::regex_search(str_source_copy, smatch_single_manual_dice, regex_filter_manual_dice_part);
			if (smatch_single_manual_dice.begin() != smatch_single_manual_dice.end()) {
				std::string command = smatch_single_manual_dice.str();
				str_source_copy = smatch_single_manual_dice.suffix().str();
				int i_pos_of_d = command.find('d');
				if (i_pos_of_d != std::string::npos) {
					int i_num_of_die = i_pos_of_d > 0 ? std::stoi(command.substr(0, i_pos_of_d)) : 1;
					int i_face_of_die = std::stoi(command.substr(i_pos_of_d + 1));
					
					for (int i_iter = 0; i_iter < i_num_of_die; i_iter++) {
						dice_roller dice(1, i_face_of_die);
						if (dice.status != dice_roller::ROLL_STATUS_FINISHED) {
							this->status = dice.status;
							return;
						}
						else {
							int result = dice.i_sum_result;
							this->pintlist_dice->push_back(TYPE_PAIR_DICE(i_face_of_die, result));
							this->i_sum_result += result;
						}
					}
					
				}
			}
			else break;
		}
		if (i_max_unit_alert == MAX_DICE_UNIT_COUNT) {
			this->status = dice_roller::ROLL_STATUS_TOO_MANY_DICE;
		}
	}
	catch (const std::invalid_argument& ia) {
		this->status = dice_roller::ROLL_STATUS_DICE_NOT_AVAILABLE;
	}
	if (this->status == dice_roller::ROLL_STATUS_UNINITIALIZED) this->status = dice_roller::ROLL_STATUS_FINISHED;
}

void manual_dice::roll(const std::string & source)
{
	this->status = dice_roller::ROLL_STATUS_UNINITIALIZED;
	try {
		unsigned int target = std::stoi(source) - 1;
		if (target > this->pintlist_dice->size()) { 
			this->status = dice_roller::ROLL_STATUS_FINISHED;
			return; 
		}
		else if (target < 0) return;
		TYPE_LIST_DICE::iterator iter_list = this->pintlist_dice->begin() + target;
		int i_face_of_die = (*iter_list).first;
		i_sum_result -= (*iter_list).second;
		dice_roller dice(1, i_face_of_die);
		if (dice.status != dice_roller::ROLL_STATUS_FINISHED){
			this->status = dice.status;
			return;
		}
		(*iter_list).second = dice.i_sum_result;
		i_sum_result += dice.i_sum_result;
		if (this->status == dice_roller::ROLL_STATUS_UNINITIALIZED) this->status = dice_roller::ROLL_STATUS_FINISHED;
	}
	catch (const std::invalid_argument& ia) {
		this->status = dice_roller::ROLL_STATUS_DICE_NOT_AVAILABLE;
	}
}

void manual_dice::kill(const std::string & source)
{
	this->status = dice_roller::ROLL_STATUS_UNINITIALIZED;
	try{
		unsigned int target = std::stoi(source) -1;
		if (target >= this->pintlist_dice->size()) {
			this->status = dice_roller::ROLL_STATUS_FINISHED;
			return;
		}
		else if (target < 0) return;
		TYPE_LIST_DICE::iterator iter_list = (this->pintlist_dice->begin()) + target;
		i_sum_result -= (*iter_list).second;
		this->pintlist_dice->erase((iter_list));
		if (this->status == dice_roller::ROLL_STATUS_UNINITIALIZED) this->status = dice_roller::ROLL_STATUS_FINISHED;
	}
	catch (const std::invalid_argument& ia) {
		this->status = dice_roller::ROLL_STATUS_DICE_NOT_AVAILABLE;
	}
}

void manual_dice::add(const std::string & source)
{
	try {
		std::string str_source_copy(source);
		std::smatch smatch_single_manual_dice;
		int i_max_unit_alert = this->pintlist_dice->size();
		for (; i_max_unit_alert < MAX_DICE_UNIT_COUNT; i_max_unit_alert++) {
			std::regex_search(str_source_copy, smatch_single_manual_dice, regex_filter_manual_dice_part);
			if (smatch_single_manual_dice.begin() != smatch_single_manual_dice.end()) {
				std::string command = smatch_single_manual_dice.str();
				str_source_copy = smatch_single_manual_dice.suffix().str();
				int i_pos_of_d = command.find('d');
				if (i_pos_of_d != std::string::npos) {
					int i_num_of_die = i_pos_of_d > 0 ? std::stoi(command.substr(0, i_pos_of_d)) : 1;
					int i_face_of_die = std::stoi(command.substr(i_pos_of_d + 1));

					for (int i_iter = 0; i_iter < i_num_of_die; i_iter++) {
						dice_roller dice(1, i_face_of_die);
						if (dice.status != dice_roller::ROLL_STATUS_FINISHED) {
							this->status = dice.status;
							return;
						}
						else {
							int result = dice.i_sum_result;
							this->pintlist_dice->push_back(TYPE_PAIR_DICE(i_face_of_die, result));
							this->i_sum_result += result;
						}
					}

				}
			}
			else break;
		}
		if (i_max_unit_alert == MAX_DICE_UNIT_COUNT) {
			this->status = dice_roller::ROLL_STATUS_TOO_MANY_DICE;
		}
	}
	catch (const std::invalid_argument& ia) {
		this->status = dice_roller::ROLL_STATUS_DICE_NOT_AVAILABLE;
	}
}

void manual_dice::killall()
{
	this->status = dice_roller::ROLL_STATUS_UNINITIALIZED;
	try {
		this->pintlist_dice->clear();
		this->i_sum_result = 0;
		if (this->status == dice_roller::ROLL_STATUS_UNINITIALIZED) this->status = dice_roller::ROLL_STATUS_FINISHED;
	}
	catch (const std::invalid_argument& ia) {
		this->status = dice_roller::ROLL_STATUS_DICE_NOT_AVAILABLE;
	}
}

std::string manual_dice::endcode()
{
	encoder enc_encoder;

	enc_encoder.addInt(this->pintlist_dice->size());
	TYPE_LIST_DICE::iterator iter_list = this->pintlist_dice->begin();
	for (; iter_list != this->pintlist_dice->end(); iter_list++) {
		enc_encoder.addInt((*iter_list).first);
		enc_encoder.addInt((*iter_list).second);
	}
	return std::string(enc_encoder.getBase64());
}

void manual_dice::decode(std::string & source)
{
	std::string source_copy(source);
	if (this->pintlist_dice != nullptr) delete(this->pintlist_dice);
	this->pintlist_dice = new TYPE_LIST_DICE();
	this->i_sum_result = 0;
	encoder enc_encoder;
	enc_encoder.setBase64(source_copy.c_str(),source_copy.length());
	//enc_encoder.setChar(source_copy.c_str(), source_copy.length());
	int len = enc_encoder.getInt();
	for (int i_iter = 0; i_iter < len; i_iter++) {
		int first = enc_encoder.getInt();
		int second = enc_encoder.getInt();
		this->i_sum_result += second;
		this->pintlist_dice->push_back(TYPE_PAIR_DICE(first, second));
	}
}

std::string manual_dice::ToString()
{
	std::ostringstream ostrs_result(std::ostringstream::ate);
	int i_sum_result = 0;
	TYPE_LIST_DICE::iterator iter_list = this->pintlist_dice->begin();

	bool hasDice = iter_list != this->pintlist_dice->end();
	for (; iter_list != this->pintlist_dice->end(); iter_list++) {
		if (iter_list != this->pintlist_dice->begin()) {
			ostrs_result << " + ";
		}
		ostrs_result << (*iter_list).second << "(" << (*iter_list).first << ")";
		i_sum_result += (*iter_list).second;
	}
	if (!hasDice) ostrs_result << "没有骰子了";
	else ostrs_result << " = " << i_sum_result;
	return ostrs_result.str();
}

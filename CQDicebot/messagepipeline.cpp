#include "stdafx.h"
#include "messagepipeline.h"
#include "utility.h"
#include "diceroller.h"

#include "NickNameControl.h"
#include "cqp.h"
#include "QTool.h"
#include <regex>
#include <iostream>

CQTool CQT_instance_A;

std::regex regex_filter_command_identifier("^ *. *(r|ns|n) *");
std::regex regex_filter_rename("^ *. *n *");
std::regex regex_filter_rename_silence("^ *. *ns *");
std::regex regex_filter_full_dice("^ *. *r *(\\+|\\-)?((\\d*d\\d+((k|kl)\\d+)?)|\\d+)((\\+|\\-)((\\d*d\\d+((k|kl)\\d+)?)|\\d+))* *");
void message_pipeline(const int32_t i_AuthCode,const char * msg, const int64_t uint64_fromGroupOrDiscuss, const int64_t uint64_fromQQ,bool isfromGroup)
{

	std::string source(msg);
	std::vector<std::string> source_splits;

	splitLine(source, source_splits);

	std::vector<std::string>::iterator iterator_sources = source_splits.begin();

	std::ostringstream ostrs_output_stream(std::ostringstream::ate);

	bool is_output_available = false;
	bool does_last_line_have_output = false;

	for (; iterator_sources != source_splits.end(); iterator_sources++) {

		std::smatch matchList_command_identifier_match;
		std::regex_search((*iterator_sources), matchList_command_identifier_match, regex_filter_command_identifier);
		if (matchList_command_identifier_match.begin() == matchList_command_identifier_match.end()) continue;


		std::smatch matchList_command_rename_silence_match;
		std::regex_search((*iterator_sources), matchList_command_rename_silence_match, regex_filter_rename_silence);
		if (matchList_command_rename_silence_match.begin() != matchList_command_rename_silence_match.end()) {
			std::string str_new_name = matchList_command_rename_silence_match.suffix().str();
			if (str_new_name.length() > 0) {
				std::string str_origin_name;
				CQT_instance_A.getDefaultName(i_AuthCode, uint64_fromGroupOrDiscuss, uint64_fromQQ, str_origin_name, isfromGroup);
				(NickNameControl::instance)->setNickName(i_AuthCode, uint64_fromGroupOrDiscuss, uint64_fromQQ, str_new_name,isfromGroup);
			}
			continue;
		}

		std::smatch matchList_command_rename_match;
		std::regex_search((*iterator_sources), matchList_command_rename_match, regex_filter_rename);
		if (matchList_command_rename_match.begin() != matchList_command_rename_match.end()){
			std::string str_new_name = matchList_command_rename_match.suffix().str();
			if (str_new_name.length() > 0) {
				std::string str_origin_name; 
				CQT_instance_A.getDefaultName(i_AuthCode, uint64_fromGroupOrDiscuss, uint64_fromQQ, str_origin_name,isfromGroup);
				(NickNameControl::instance)->setNickName(i_AuthCode, uint64_fromGroupOrDiscuss, uint64_fromQQ, str_new_name, isfromGroup);

				if (does_last_line_have_output) {
					does_last_line_have_output = false;
					ostrs_output_stream << std::endl;
				}

				ostrs_output_stream << " * " << str_origin_name;
				ostrs_output_stream << " 的新名字是 " << str_new_name;
				is_output_available = true;
				does_last_line_have_output = true;
			}
			continue;
		}

		std::smatch matchList_command_full_dice_roll_match;
		std::regex_search((*iterator_sources), matchList_command_full_dice_roll_match, regex_filter_full_dice);
		if (matchList_command_full_dice_roll_match.begin() != matchList_command_full_dice_roll_match.end()){
			std::string str_roll_message = matchList_command_full_dice_roll_match.suffix().str();
			std::string str_roll_source = matchList_command_full_dice_roll_match.str();

			removeSpaceAndTab(str_roll_source);
			if (str_roll_source.length() > 2) {
				if (str_roll_source[0] == '.' && str_roll_source[1] == 'r') {
					std::regex regex_single_dice("(\\d*d\\d+((k|kl)\\d+)?)|(\\d+)");
					std::regex regex_single_dice_sign("(\\+|\\-)");

					std::smatch matchList_single_dice;
					std::smatch matchList_single_dice_sign;

					std::ostringstream ostrs_dice_stream(std::ostringstream::ate);
					ostrs_dice_stream.str(str_roll_source.substr(2, str_roll_source.length() - 2));
					ostrs_dice_stream << " = ";

					int i_dice_summary = 0;

					bool is_first_signed = (str_roll_source[0] == '+' || str_roll_source[0] == '-');
					bool is_not_first_dice = false;

					bool is_this_line_output = true;

					int i_max_unit_altert = 0;
					for (; i_max_unit_altert < MAX_DICE_UNIT_COUNT; i_max_unit_altert++) {

						int i_single_dice_sign = 1;
						if (str_roll_source.length() < 1) break;
						if (is_first_signed || is_not_first_dice) {
							ostrs_dice_stream << " " << str_roll_source[0] << " ";
							if (str_roll_source[0] == '-') i_single_dice_sign = -1;
							else i_single_dice_sign = 1;
						}

						if (!std::regex_search(str_roll_source, matchList_single_dice, regex_single_dice)) break;
						std::string str_single_dice = matchList_single_dice.str();
						str_roll_source = matchList_single_dice.suffix().str();

						is_not_first_dice = true;

						int i_pos_of_d = str_single_dice.find('d');
						if (i_pos_of_d == std::string::npos) {
							int result = std::stoi(str_single_dice);
							i_dice_summary += result * i_single_dice_sign;
							ostrs_dice_stream << result;
						}
						else {
							int i_pos_of_k = str_single_dice.find('k');
							if (i_pos_of_k == std::string::npos) {
								int i_num_of_die = i_pos_of_d > 0 ? std::stoi(str_single_dice.substr(0, i_pos_of_d)) : 1;
								int i_face_of_die = std::stoi(str_single_dice.substr(i_pos_of_d + 1, str_single_dice.length() - i_pos_of_d - 1));
								if (i_num_of_die > MAX_DICE_NUM || i_face_of_die > MAX_DICE_FACE) {
									break;
									is_this_line_output = false;
								}
								DiceRoller dr_diceRoll(i_num_of_die, i_face_of_die);
								ostrs_dice_stream << "(";
								ostrs_dice_stream << dr_diceRoll.str_detail_result;
								ostrs_dice_stream << ")";
								i_dice_summary += dr_diceRoll.i_sum_result;
							}
							else {
								int i_pos_of_l = str_single_dice.find('l');
								if (i_pos_of_l == std::string::npos) {
									int i_num_of_die = i_pos_of_d > 0 ? std::stoi(str_single_dice.substr(0, i_pos_of_d)) : 1;
									int i_face_of_die = std::stoi(str_single_dice.substr(i_pos_of_d + 1, i_pos_of_k - i_pos_of_d - 1));
									int i_num_of_keep = std::stoi(str_single_dice.substr(i_pos_of_k + 1, str_single_dice.length() - i_pos_of_k - 1));
									if (i_num_of_die > MAX_DICE_NUM || i_face_of_die > MAX_DICE_FACE) {
										break;
										is_this_line_output = false;
									}
									DiceRoller dr_diceRoll(i_num_of_die, i_face_of_die, i_num_of_keep, false);
									ostrs_dice_stream << "(";
									ostrs_dice_stream << dr_diceRoll.str_detail_result;
									ostrs_dice_stream << ")";
									i_dice_summary += dr_diceRoll.i_sum_result;
								}
								else {
									int i_num_of_die = i_pos_of_d > 0 ? std::stoi(str_single_dice.substr(0, i_pos_of_d)) : 1;
									int i_face_of_die = std::stoi(str_single_dice.substr(i_pos_of_d + 1, i_pos_of_k - i_pos_of_d - 1));
									int i_num_of_keep = std::stoi(str_single_dice.substr(i_pos_of_l + 1, str_single_dice.length() - i_pos_of_l - 1));
									if (i_num_of_die > MAX_DICE_NUM || i_face_of_die > MAX_DICE_FACE) {
										break;
										is_this_line_output = false;
									}
									DiceRoller dr_diceRoll(i_num_of_die, i_face_of_die, i_num_of_keep, true);
									ostrs_dice_stream << "(";
									ostrs_dice_stream << dr_diceRoll.str_detail_result;
									ostrs_dice_stream << ")";
									i_dice_summary += dr_diceRoll.i_sum_result;
								}
							}
						}
					}
					if (i_max_unit_altert == MAX_DICE_UNIT_COUNT) is_this_line_output = false;

					if (is_this_line_output) {
						ostrs_dice_stream << " = ";
						ostrs_dice_stream << i_dice_summary;

						if (does_last_line_have_output) {
							does_last_line_have_output = false;
							ostrs_output_stream << std::endl;
						}

						std::string nickname;
						(NickNameControl::instance)->getNickName(i_AuthCode, uint64_fromGroupOrDiscuss, uint64_fromQQ, nickname, isfromGroup);
						ostrs_output_stream << " * " << nickname;
						ostrs_output_stream << " " << str_roll_message << " 掷骰: ";

						ostrs_output_stream << ostrs_dice_stream.str();
						is_output_available = true;
						does_last_line_have_output = true;
					}
				}
			}
			continue;
		}
	}

	if (is_output_available) {
		if(isfromGroup)	CQ_sendGroupMsg(i_AuthCode, uint64_fromGroupOrDiscuss, ostrs_output_stream.str().c_str());
		else CQ_sendDiscussMsg(i_AuthCode, uint64_fromGroupOrDiscuss, ostrs_output_stream.str().c_str());
	}
}



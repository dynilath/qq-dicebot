#include "stdafx.h"
#include "messagepipeline.h"
#include "utility.h"
#include "diceroller.h"

#include "NickNameControl.h"
#include "cqp.h"
#include "QTool.h"
#include <regex>
#include <iostream>

#define MAX_DICE_UNIT_COUNT 10
#define MAX_DICE_NUM 50
#define MAX_DICE_FACE 500


CQTool CQT_instance_A;

std::regex regex_filter_command_identifier("^ *. *r *\\d*d\\d+ *");
std::regex regex_filter_full_dice("^ *. *r *(\\+|\\-)?((\\d*d\\d+((k|kl)\\d+)?)|\\d+)((\\+|\\-)((\\d*d\\d+((k|kl)\\d+)?)|\\d+))* *");
void message_pipeline(const int32_t i_AuthCode,const char * msg, const int64_t fromGroup, const int64_t fromQQ)
{

	std::string source(msg);
	std::vector<std::string> source_splits;

	if (source.length() > 3 && source[0] == '.' && source[1] == 'n') {
		std::string str_new_name = source.substr(2, source.length() - 2);
		std::string str_origin_name;
		CQ_Type_GroupMember groupMember;
		if (CQT_instance_A.GetGroupMemberInfo(i_AuthCode, fromGroup, fromQQ, groupMember)) {
			if (groupMember.nick.length() == 0) str_origin_name = groupMember.username;
			else str_origin_name = groupMember.nick;
		}
		(NickNameControl::instance)->setNickName(i_AuthCode, fromGroup, fromQQ, str_new_name);
		std::ostringstream ostrs_ret_stream(std::ostringstream::ate);
		ostrs_ret_stream << " * " << str_origin_name;
		ostrs_ret_stream << " 的新名字是 " << str_new_name;
		CQ_sendGroupMsg(i_AuthCode, fromGroup, ostrs_ret_stream.str().c_str());
		return;
	}

	splitLine(source, source_splits);

	std::vector<std::string>::iterator iterator_sources = source_splits.begin();

	std::ostringstream ostrs_output_stream(std::ostringstream::ate);

	bool is_output_available = false;
	bool does_last_line_have_output = false;

	for (; iterator_sources != source_splits.end(); iterator_sources++) {
		bool is_this_line_output = false;
		/*
		std::regex regex_base_filter("^ *. *[rc] *([\\+\\-]?(((\\d*d\\d+)((k|kl)\\d+)?)|(\\d+\\.?\\d*)))((\\+|\\-|\\*|/)(((\\d*d\\d+)((k|kl)\\d+)?)|(\\d+\\.?\\d*)))*");
		//std::regex regex_base_filter("^[^\\f\\n\\r\\t\\v]*");

		std::smatch matchList_base_match;
		std::regex_search((*iterator_sources), matchList_base_match, regex_base_filter);
		if (matchList_base_match.begin() != matchList_base_match.end()) {
		std::string str_roll_command_raw_strip = (*matchList_base_match.begin()).str();
		std::string str_roll_comment = (*iterator_sources).substr(str_roll_command_raw_strip.length(), (*iterator_sources).length() - str_roll_command_raw_strip.length() - 1);
		removeSpaceAndTab(str_roll_command_raw_strip);
		//std::regex regex_base_filter("(\\d*d\\d+)|(\\d)");
		//CQ_sendGroupMsg(i_AuthCode, fromGroup, str_roll_comment.c_str());
		//CQ_sendGroupMsg(i_AuthCode, fromGroup, str_roll_command_raw_strip.c_str());
		}else continue;
		*/

		std::string str_roll_message = "";

		std::smatch matchList_command_identifier_match;
		std::regex_search((*iterator_sources), matchList_command_identifier_match, regex_filter_full_dice);
		std::smatch::iterator iter_roll_str = matchList_command_identifier_match.begin();
		if (iter_roll_str != matchList_command_identifier_match.end()) {

			int len = (*iter_roll_str).length();
			str_roll_message = (*iterator_sources).substr(len, (*iterator_sources).length() - len);
		}
		else continue; 

		std::string str_roll_source = (*iterator_sources).substr(0, (*iterator_sources).length() - str_roll_message.length());

		removeSpaceAndTab(str_roll_source);
		if (str_roll_source.length() > 2) {
			if (str_roll_source[0] == '.' && str_roll_source[1] == 'r') {
				//(m)d(x) for classic dice roll, may add (m)d(x)k(y) / (m)d(x)kl(y), and (m)d(x)[k/kl(y)]+(m)d(x)[k/kl(y)] for further plan 

				//^((((\([\d+\-*/]+\))|([\d]*))d((\([\d+\-*/]+\))|([\d]+))(((k|kl)((\([\d+\-*/]+\))|([\d]+)))?))|((\([\d+\-*/]+\))|([\d]+)))(([+\-]((((\([\d+\-*/]+\))|([\d]*))d((\([\d+\-*/]+\))|([\d]+))(((k|kl)((\([\d+\-*/]+\))|([\d]+)))?))|((\([\d+\-*/]+\))|([\d]+))))*)
				//regex for [cal]d(cal)[k|kl(cal)]|(cal) (+/- [cal]d(cal)[k|kl(cal)]|(cal)) * n

				//((((\([\d+\-*/]+\))|(\d*))d((\([\d+\-*/]+\))|([\d+))(((k|kl)((\([\d+\-*/]+\))|(\d+)))?))|((\([\d+\-*/]+\))|(\d+)))
				//regex for [cal]d(cal)[k|kl(cal)]|(cal)

				//(\([.\d+\-*/]+\))|(\d*)
				//regex for cal (a+b*c-d/e)|a
				std::regex regex_DICE("\\d*d\\d+"); //[m]d(x)
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
					(NickNameControl::instance)->getNickName(i_AuthCode, fromGroup, fromQQ, nickname);
					ostrs_output_stream << " * " << nickname;
					ostrs_output_stream << " " << str_roll_message << " 掷骰: ";

					ostrs_output_stream << ostrs_dice_stream.str();
					is_output_available = true;
					does_last_line_have_output = true;
				}
				/*
				if (matchList.size() > 0) {
					std::ostringstream ostrs_dice_stream(std::ostringstream::ate);

					CQ_Type_GroupMember groupMember;
					if (CQT_instance_A.GetGroupMemberInfo(i_AuthCode, fromGroup, fromQQ, groupMember)) {
						ostrs_output_stream << " * ";
						if (groupMember.nick.length() == 0) ostrs_output_stream << groupMember.username;
						else ostrs_output_stream << groupMember.nick;
						ostrs_output_stream << " : " << str_roll_message << " 投掷";
					}

					std::string nickname;
					(NickNameControl::instance)->getNickName(i_AuthCode, fromGroup, fromQQ, nickname);
					ostrs_output_stream << " * " << nickname;
					ostrs_output_stream << " : " << str_roll_message << " 掷骰 ";

					std::string adb = *(matchList.begin());
					int i_pos_of_d = adb.find('d');
					int i_num_of_die = i_pos_of_d > 0 ? std::stoi(adb.substr(0, i_pos_of_d)) : 1;
					int i_face_of_die = std::stoi(adb.substr(i_pos_of_d + 1, adb.length() - i_pos_of_d - 1));




					if (does_last_line_have_output && is_this_line_output) {
						does_last_line_have_output = false;
						ostrs_output_stream << std::endl;
					}

					ostrs_dice_stream.str(adb);
					ostrs_dice_stream << " = ";
					if (i_num_of_die < 100 && i_face_of_die < 1000 && i_face_of_die >= 1 && i_num_of_die >= 1) {
						DiceRoller dr_diceRoll(i_num_of_die, i_face_of_die);
						ostrs_dice_stream << "(";
						ostrs_dice_stream << dr_diceRoll.str_detail_result;
						ostrs_dice_stream << ") = ";
						ostrs_dice_stream << dr_diceRoll.i_sum_result;
						ostrs_output_stream << ostrs_dice_stream.str();
						is_output_available = true;
						does_last_line_have_output = true;
					}
				}
				*/
			}
		}
	}

	if (is_output_available) {
		CQ_sendGroupMsg(i_AuthCode, fromGroup, ostrs_output_stream.str().c_str());
	}
}



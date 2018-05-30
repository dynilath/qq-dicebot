#include "stdafx.h"
#include "messagepipeline.h"
#include "utility.h"
#include "diceroller.h"

#include "nickManager.h"

#include "cqp.h"
#include "QTool.h"
#include "diceSpliter.h"

#include <regex>
#include <iostream>

#define CHECK_LASTLINE_FOR_ENDL(_Stream,_Flag) if (_Flag) {	_Flag = false;	_Stream << std::endl;}

std::regex regex_filter_command_identifier("^ *. *(c|r|ns|n) *");
std::regex regex_filter_rename("^ *. *n *");
std::regex regex_filter_rename_silence("^ *. *ns *");
std::regex regex_filter_normaldice("^ *. *r *");
std::regex regex_filter_cocdice("^ *. *c *");
std::regex regex_filter_macro_dice("^ *. *m *");
std::regex regex_filter_use_macro("^ *. *rm *");
std::regex regex_filter_full_dice("^(\\+|\\-)?((\\d*d\\d+((k|kl)\\d+)?)|\\d+)((\\+|\\-|\\*|/)((\\d*d\\d+((k|kl)\\d+)?)|\\d+))* *");
std::regex regex_filter_coc_full_dice("^((p|b)\\d+)* *");

bool group_message_pipeline(const int32_t i_AuthCode,const char * msg, const int64_t uint64_fromGroupOrDiscuss, const int64_t uint64_fromQQ,bool isfromGroup)
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
				CQTool::getDefaultName(i_AuthCode, uint64_fromGroupOrDiscuss, uint64_fromQQ, str_origin_name, isfromGroup);
				(nickManager::instance)->setNickName(i_AuthCode, uint64_fromGroupOrDiscuss, uint64_fromQQ, str_new_name,isfromGroup);
			}
			continue;
		}

		std::smatch matchList_command_rename_match;
		std::regex_search((*iterator_sources), matchList_command_rename_match, regex_filter_rename);
		if (matchList_command_rename_match.begin() != matchList_command_rename_match.end()){
			std::string str_new_name = matchList_command_rename_match.suffix().str();
			if (str_new_name.length() > 0) {
				std::string str_origin_name; 
				CQTool::getDefaultName(i_AuthCode, uint64_fromGroupOrDiscuss, uint64_fromQQ, str_origin_name,isfromGroup);
				(nickManager::instance)->setNickName(i_AuthCode, uint64_fromGroupOrDiscuss, uint64_fromQQ, str_new_name, isfromGroup);
				CHECK_LASTLINE_FOR_ENDL(ostrs_output_stream, does_last_line_have_output);

				ostrs_output_stream << " * " << str_origin_name << " 的新名字是 " << str_new_name;
				is_output_available = true;
				does_last_line_have_output = true;
			}
			continue;
		}

		std::smatch matchList_command_normaldice_match;
		std::regex_search((*iterator_sources), matchList_command_normaldice_match, regex_filter_normaldice);
		if (matchList_command_normaldice_match.begin() != matchList_command_normaldice_match.end()) {
			std::string str_full_roll_command = matchList_command_normaldice_match.suffix().str();
			std::smatch matchList_command_full_dice_roll_match;
			std::regex_search(str_full_roll_command, matchList_command_full_dice_roll_match, regex_filter_full_dice);

			if (matchList_command_full_dice_roll_match.begin() != matchList_command_full_dice_roll_match.end()) {
				std::string str_roll_message = matchList_command_full_dice_roll_match.suffix().str();
				std::string str_roll_source = matchList_command_full_dice_roll_match.str();
				removeSpaceAndTab(str_roll_source);
				std::string str_roll_output;
				if (baseSplitDice(str_roll_source, str_roll_output)) {
					CHECK_LASTLINE_FOR_ENDL(ostrs_output_stream, does_last_line_have_output);

					std::string nickname;
					(nickManager::instance)->getNickName(i_AuthCode, uint64_fromGroupOrDiscuss, uint64_fromQQ, nickname, isfromGroup);
					ostrs_output_stream << " * " << nickname << " " << str_roll_message << " 掷骰: ";

					ostrs_output_stream << str_roll_output;
					is_output_available = true;
					does_last_line_have_output = true;
				}
				continue;
			}
		}

		std::smatch matchList_command_cocdice_match;
		std::regex_search((*iterator_sources), matchList_command_cocdice_match, regex_filter_cocdice);
		if (matchList_command_cocdice_match.begin() != matchList_command_cocdice_match.end()) {
			std::string str_full_roll_command = matchList_command_cocdice_match.suffix().str();
			std::smatch matchList_command_coc_dice_roll_match;
			std::regex_search(str_full_roll_command, matchList_command_coc_dice_roll_match, regex_filter_coc_full_dice);
			if (matchList_command_coc_dice_roll_match.begin() != matchList_command_coc_dice_roll_match.end()) {
				std::string str_roll_message = matchList_command_coc_dice_roll_match.suffix().str();
				std::string str_roll_source = matchList_command_coc_dice_roll_match.str();
				removeSpaceAndTab(str_roll_source);

				DiceRoller diceRoll(str_roll_source, ROLL_MODE_COC_PB);
				if (diceRoll.status == ROLL_STATUS_FINISHED) {
					CHECK_LASTLINE_FOR_ENDL(ostrs_output_stream, does_last_line_have_output);

					std::string nickname;
					(nickManager::instance)->getNickName(i_AuthCode, uint64_fromGroupOrDiscuss, uint64_fromQQ, nickname, isfromGroup);
					ostrs_output_stream << " * " << nickname << " " << str_roll_message << " 掷骰: ";

					ostrs_output_stream << *(diceRoll.str_detail_result) << " = " << diceRoll.i_sum_result;
					is_output_available = true;
					does_last_line_have_output = true;

				}
				continue;
			}
		}
		
	}
	if (is_output_available) {
		if(isfromGroup)	CQ_sendGroupMsg(i_AuthCode, uint64_fromGroupOrDiscuss, ostrs_output_stream.str().c_str());
		else CQ_sendDiscussMsg(i_AuthCode, uint64_fromGroupOrDiscuss, ostrs_output_stream.str().c_str());
	}
	return is_output_available;
}



#include "stdafx.h"
#include "messagepipeline.h"
#include "utility.h"
#include "diceroller.h"

#include "manualdice.h"
#include "manualDiceControl.h"

#include "nickManager.h"

#include "cqp.h"
#include "QTool.h"
#include "diceSpliter.h"

#include <regex>
#include <iostream>

#define CHECK_LASTLINE_FOR_ENDL(_Stream,_Flag) if (_Flag) {	_Flag = false;	_Stream << std::endl;}

std::regex regex_filter_command_identifier("^ *\\. *(hka|ha|hk|hr|h|c|r|ns|n) *");
//rename loudly
std::regex regex_filter_rename("^ *\\. *n *");
//rename silently
std::regex regex_filter_rename_silence("^ *\\. *ns *");
//regular dice
std::regex regex_filter_normaldice("^ *\\. *r *");
//coc7th customized dice
std::regex regex_filter_cocdice("^ *\\. *c *");

//TODO
//macro, use macro to save a specific regular diceroll, like .m 4d6+4 my_biggest_trick
//spaces are not allowed in macro names
std::regex regex_filter_macro_dice("^ *\\. *m *");
//use specific diceroll, like .rm my_biggest_trick
std::regex regex_filter_use_macro("^ *\\. *rm *");
//use specific diceroll, but the detail of rolling wont be displayed, only the result
std::regex regex_filter_use_macro_silent("^ *\\. *rms *");

//TODO
//hand use die, create some realdice and keep them alive, for example ".h 4d8+5d6"
//of course no plain numerical is allowed here 
std::regex regex_filter_manual_dice("^ *\\. *h *");
//roll specific one of hand use dice, .hr 4 will reroll the 4th of the 4 dice
std::regex regex_filter_manual_roll("^ *\\. *hr *");
//kill specific one of hand use dice, ".hk 1" will remove the 1st die
//".hk" will kill them all, also will ".hk 1" when there is only 1 die left
std::regex regex_filter_manual_kill("^ *\\. *hk *");
std::regex regex_filter_manual_add("^ *\\. *ha *");
std::regex regex_filter_manual_killall("^ *\\. *hka *");

//TODO
//define result, if you use ".def 1 i like it", any dice roll you get a result of 1 will be shown as "i like it"
std::regex regex_filter_def_result("^ *\\. *def *");
//macro specific define result, it only apply to specific macro
std::regex regex_filter_macro_def("^ *\\. *mdef *");


std::regex regex_filter_full_dice("^(\\+|\\-)?((\\d*d\\d+((k|kl)\\d+)?)|\\d+)((\\+|\\-|\\*|/)((\\d*d\\d+((k|kl)\\d+)?)|\\d+))* *");
std::regex regex_filter_coc_full_dice("^((p|b)\\d+)* *");
std::regex regex_filter_manual_full_dice("^((\\+)?\\d*d\\d+)(\\+\\d*d\\d+)* *");
std::regex regex_filter_integer_space("^\\d+ *");

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

					ostrs_output_stream << *(diceRoll.pstr_detail_result) << " = " << diceRoll.i_sum_result;
					is_output_available = true;
					does_last_line_have_output = true;

				}
				continue;
			}
		}

		{
			std::smatch matchlist_manualdice_add;
			std::regex_search((*iterator_sources), matchlist_manualdice_add, regex_filter_manual_add);
			if (matchlist_manualdice_add.begin() != matchlist_manualdice_add.end()) {
				std::string str_full_command = matchlist_manualdice_add.suffix().str();
				std::smatch matchList_roll_match;
				std::regex_search(str_full_command, matchList_roll_match, regex_filter_manual_full_dice);
				if (matchList_roll_match.begin() != matchList_roll_match.end()) {
					std::string str_command = matchList_roll_match.str();
					std::string str_roll_message = matchList_roll_match.suffix().str();
					removeSpaceAndTab(str_command);

					manualDice * md_manualdice = manualDiceManager::instance->addManualDice(uint64_fromQQ, uint64_fromGroupOrDiscuss, str_command);
					if ((*md_manualdice).status == ROLL_STATUS_FINISHED) {
						CHECK_LASTLINE_FOR_ENDL(ostrs_output_stream, does_last_line_have_output);

						std::string nickname;
						(nickManager::instance)->getNickName(i_AuthCode, uint64_fromGroupOrDiscuss, uint64_fromQQ, nickname, isfromGroup);
						ostrs_output_stream << " * " << nickname << " " << str_roll_message;
						ostrs_output_stream << " 在桌上增加了这些骰子: " << str_command;
						ostrs_output_stream << " 当前状态: " << md_manualdice->ToString();
						is_output_available = true;
						does_last_line_have_output = true;

					}
				}
				continue;
			}
		}

		std::smatch matchlist_manualdice_killall;
		std::regex_search((*iterator_sources), matchlist_manualdice_killall, regex_filter_manual_killall);
		if (matchlist_manualdice_killall.begin() != matchlist_manualdice_killall.end()) {

			std::string str_roll_message = matchlist_manualdice_killall.suffix().str();

			manualDice * md_manualdice = manualDiceManager::instance->killallManualDice(uint64_fromQQ, uint64_fromGroupOrDiscuss);
			if ((*md_manualdice).status == ROLL_STATUS_FINISHED) {
				CHECK_LASTLINE_FOR_ENDL(ostrs_output_stream, does_last_line_have_output);

				std::string nickname;
				(nickManager::instance)->getNickName(i_AuthCode, uint64_fromGroupOrDiscuss, uint64_fromQQ, nickname, isfromGroup);
				ostrs_output_stream << " * " << nickname << " " << str_roll_message;
				ostrs_output_stream << " 杀掉了所有的骰子 ";
				ostrs_output_stream << "当前状态: " << md_manualdice->ToString();
				is_output_available = true;
				does_last_line_have_output = true;

			}

			continue;
		}

		std::smatch matchlist_manualdice_kill;
		std::regex_search((*iterator_sources), matchlist_manualdice_kill, regex_filter_manual_kill);
		if (matchlist_manualdice_kill.begin() != matchlist_manualdice_kill.end()) {
			std::string str_full_command = matchlist_manualdice_kill.suffix().str();
			std::smatch matchList_roll_match;
			std::regex_search(str_full_command, matchList_roll_match, regex_filter_integer_space);
			if (matchList_roll_match.begin() != matchList_roll_match.end()) {
				std::string str_command = matchList_roll_match.str();
				std::string str_roll_message = matchList_roll_match.suffix().str();
				removeSpaceAndTab(str_command);
				
				manualDice * md_manualdice = manualDiceManager::instance->killManualDice(uint64_fromQQ, uint64_fromGroupOrDiscuss, str_command);
				if ((*md_manualdice).status == ROLL_STATUS_FINISHED) {
					CHECK_LASTLINE_FOR_ENDL(ostrs_output_stream, does_last_line_have_output);

					std::string nickname;
					(nickManager::instance)->getNickName(i_AuthCode, uint64_fromGroupOrDiscuss, uint64_fromQQ, nickname, isfromGroup);
					ostrs_output_stream << " * " << nickname << " " << str_roll_message;
					ostrs_output_stream << " 杀死桌上的第 " << str_command << " 个骰子 ";
					ostrs_output_stream << "当前状态: " << md_manualdice->ToString();
					is_output_available = true;
					does_last_line_have_output = true;
				}
			}
			continue;
		}

		std::smatch matchlist_manualdice_roll;
		std::regex_search((*iterator_sources), matchlist_manualdice_roll, regex_filter_manual_roll);
		if (matchlist_manualdice_roll.begin() != matchlist_manualdice_roll.end()) {
			std::string str_full_command = matchlist_manualdice_roll.suffix().str();
			std::smatch matchList_roll_match;
			std::regex_search(str_full_command, matchList_roll_match, regex_filter_integer_space);
			if (matchList_roll_match.begin() != matchList_roll_match.end()) {
				std::string str_command = matchList_roll_match.str();
				std::string str_roll_message = matchList_roll_match.suffix().str();
				removeSpaceAndTab(str_command);
				
				manualDice * md_manualdice = manualDiceManager::instance->rollManualDice(uint64_fromQQ, uint64_fromGroupOrDiscuss, str_command);
				if ((*md_manualdice).status == ROLL_STATUS_FINISHED) {
					CHECK_LASTLINE_FOR_ENDL(ostrs_output_stream, does_last_line_have_output);

					std::string nickname;
					(nickManager::instance)->getNickName(i_AuthCode, uint64_fromGroupOrDiscuss, uint64_fromQQ, nickname, isfromGroup);
					ostrs_output_stream << " * " << nickname << " " << str_roll_message;
					ostrs_output_stream << " 重骰桌上的第 " << str_command << " 个骰子 ";
					ostrs_output_stream << "当前状态: " << md_manualdice->ToString();
					is_output_available = true;
					does_last_line_have_output = true;

				}
			}
			continue;
		}

		std::smatch matchlist_manualdice_create;
		std::regex_search((*iterator_sources), matchlist_manualdice_create, regex_filter_manual_dice);
		if (matchlist_manualdice_create.begin() != matchlist_manualdice_create.end()) {
			std::string str_full_command = matchlist_manualdice_create.suffix().str();
			std::smatch matchList_roll_match;
			std::regex_search(str_full_command, matchList_roll_match, regex_filter_manual_full_dice);
			if (matchList_roll_match.begin() != matchList_roll_match.end()) {
				std::string str_command = matchList_roll_match.str();
				std::string str_roll_message = matchList_roll_match.suffix().str();
				removeSpaceAndTab(str_command);
				
				manualDice * md_manualdice = manualDiceManager::instance->createManualDice(uint64_fromQQ, uint64_fromGroupOrDiscuss, str_command);
				
				if (md_manualdice->status == ROLL_STATUS_FINISHED) {
					CHECK_LASTLINE_FOR_ENDL(ostrs_output_stream, does_last_line_have_output);

					std::string nickname;
					std::string md(md_manualdice->ToString());
					(nickManager::instance)->getNickName(i_AuthCode, uint64_fromGroupOrDiscuss, uint64_fromQQ, nickname, isfromGroup);
					ostrs_output_stream << " * " << nickname << " " << str_roll_message;
					ostrs_output_stream << " 在桌上放了这些骰子: " << str_command;
					ostrs_output_stream << " 当前状态: " << md_manualdice->ToString();
					is_output_available = true;
					does_last_line_have_output = true;

				}
			}
			continue;
		}
		
	}
	if (is_output_available) {
		if(isfromGroup)	CQ_sendGroupMsg(i_AuthCode, uint64_fromGroupOrDiscuss, ostrs_output_stream.str().c_str());
		else CQ_sendDiscussMsg(i_AuthCode, uint64_fromGroupOrDiscuss, ostrs_output_stream.str().c_str());
	}
	return is_output_available;
}



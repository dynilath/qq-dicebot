#include "stdafx.h"
#include "utility.h"
#include "nick_manager.h"

#include "manualdice.h"
#include "manual_dice_control.h"

#include "diceroller.h"

#include "dice_protocol.h"
#include "protocol_manual_dice.h"


protocol_manual_dice::protocol_manual_dice()
{
	this->identifier = new std::string("h");
	this->regex_filter_full_dice = new std::regex("^ *((\\+)?\\d*d\\d+)(\\+\\d*d\\d+)* *");
	this->regex_filter_integer_space = new std::regex("^ *\\d+ *");
	this->regex_detail_command = new std::regex("^(ka|a|k|r) *");
	this->method_map = new std::map<std::string, MANUAL_DICE_CALL_TYPE()>();
	this->method_map->insert(std::pair<std::string, MANUAL_DICE_CALL_TYPE()>("ka", &protocol_manual_dice::manualdice_killall));
	this->method_map->insert(std::pair<std::string, MANUAL_DICE_CALL_TYPE()>("k", &protocol_manual_dice::manualdice_kill));
	this->method_map->insert(std::pair<std::string, MANUAL_DICE_CALL_TYPE()>("a", &protocol_manual_dice::manualdice_add));
	this->method_map->insert(std::pair<std::string, MANUAL_DICE_CALL_TYPE()>("r", &protocol_manual_dice::manualdice_roll));
}


protocol_manual_dice::~protocol_manual_dice()
{
	delete this->identifier;
	delete this->regex_filter_full_dice;
	delete this->regex_filter_integer_space;
	delete this->method_map;
	delete this->regex_detail_command;
}

std::string protocol_manual_dice::resolve_request(
	std::string message,
	const int32_t i_AuthCode,
	const int64_t uint64_fromGroupOrDiscuss,
	const int64_t uint64_fromQQ,
	bool isfromGroup)
{
	std::string str_nickname;
	(nickname_manager::instance)->get_nickname(i_AuthCode, uint64_fromGroupOrDiscuss, uint64_fromQQ, str_nickname, isfromGroup);

	std::smatch match_list_command_identifier_match;
	std::regex_search(message, match_list_command_identifier_match, *this->regex_detail_command);
	if (match_list_command_identifier_match.begin() == match_list_command_identifier_match.end())
		return manualdice_create(message, str_nickname, uint64_fromGroupOrDiscuss, uint64_fromQQ);
	std::string str_match = match_list_command_identifier_match[1];
	std::map<std::string, MANUAL_DICE_CALL_TYPE()>::iterator iter = this->method_map->find(str_match);
	if (iter != method_map->end()) {
		MANUAL_DICE_CALL_TYPE(dice_call) = (*iter).second;
		return (this->*dice_call)(match_list_command_identifier_match.suffix().str(), str_nickname, uint64_fromGroupOrDiscuss, uint64_fromQQ);
	}
	return std::string();
}

std::string protocol_manual_dice::manualdice_add(std::string message, std::string nick_name, int64_t uint64_fromGroupOrDiscuss, int64_t uint64_fromQQ)
{
	std::smatch match_list_roll_match;
	std::regex_search(message, match_list_roll_match, *this->regex_filter_full_dice);
	if (match_list_roll_match.begin() != match_list_roll_match.end()) {
		std::string str_command = match_list_roll_match.str();
		std::string str_roll_message = match_list_roll_match.suffix().str();
		remove_space_and_tab(str_command);
		manual_dice * md_manualdice = manual_dice_manager::instance->add_manual_dice(uint64_fromQQ, uint64_fromGroupOrDiscuss, str_command);
		if ((*md_manualdice).status == ROLL_STATUS_FINISHED) {
			std::ostringstream ostrs_output_stream(std::ostringstream::ate);
			ostrs_output_stream << " * " << nick_name << " " << str_roll_message;
			ostrs_output_stream << " 在桌上增加了这些骰子: " << str_command;
			ostrs_output_stream << " 当前状态: " << md_manualdice->ToString();
			return ostrs_output_stream.str();
		}
	}
	return std::string();
}

std::string protocol_manual_dice::manualdice_killall(std::string message, std::string nick_name, int64_t uint64_fromGroupOrDiscuss, int64_t uint64_fromQQ)
{
	manual_dice * md_manualdice = manual_dice_manager::instance->killall_manual_dice(uint64_fromQQ, uint64_fromGroupOrDiscuss);
	if ((*md_manualdice).status == ROLL_STATUS_FINISHED) {
		std::ostringstream ostrs_output_stream(std::ostringstream::ate);
		ostrs_output_stream << " * " << nick_name << " " << message;
		ostrs_output_stream << " 杀掉了所有的骰子 ";
		ostrs_output_stream << "当前状态: " << md_manualdice->ToString();
		return ostrs_output_stream.str();
	}
	return std::string();
}

std::string protocol_manual_dice::manualdice_kill(std::string message, std::string nick_name, int64_t uint64_fromGroupOrDiscuss, int64_t uint64_fromQQ)
{
	std::smatch match_list_roll_match;
	std::regex_search(message, match_list_roll_match, *this->regex_filter_integer_space);
	if (match_list_roll_match.begin() != match_list_roll_match.end()) {
		std::string str_command = match_list_roll_match.str();
		std::string str_roll_message = match_list_roll_match.suffix().str();
		remove_space_and_tab(str_command);

		manual_dice * md_manualdice = manual_dice_manager::instance->kill_manual_dice(uint64_fromQQ, uint64_fromGroupOrDiscuss, str_command);
		if ((*md_manualdice).status == ROLL_STATUS_FINISHED) {
			std::ostringstream ostrs_output_stream(std::ostringstream::ate);
			ostrs_output_stream << " * " << nick_name << " " << str_roll_message;
			ostrs_output_stream << " 杀死桌上的第 " << str_command << " 个骰子 ";
			ostrs_output_stream << "当前状态: " << md_manualdice->ToString();
			return ostrs_output_stream.str();
		}
	}
	return std::string();
}

std::string protocol_manual_dice::manualdice_roll(std::string message, std::string nick_name, int64_t uint64_fromGroupOrDiscuss, int64_t uint64_fromQQ)
{
	std::smatch match_list_roll_match;
	std::regex_search(message, match_list_roll_match, *this->regex_filter_integer_space);
	if (match_list_roll_match.begin() != match_list_roll_match.end()) {
		std::string str_command = match_list_roll_match.str();
		std::string str_roll_message = match_list_roll_match.suffix().str();
		remove_space_and_tab(str_command);

		manual_dice * md_manualdice = manual_dice_manager::instance->roll_manual_dice(uint64_fromQQ, uint64_fromGroupOrDiscuss, str_command);
		if ((*md_manualdice).status == ROLL_STATUS_FINISHED) {
			std::ostringstream ostrs_output_stream(std::ostringstream::ate);
			ostrs_output_stream << " * " << nick_name << " " << str_roll_message;
			ostrs_output_stream << " 重骰桌上的第 " << str_command << " 个骰子 ";
			ostrs_output_stream << "当前状态: " << md_manualdice->ToString();
			return ostrs_output_stream.str();
		}
	}
	return std::string();
}

std::string protocol_manual_dice::manualdice_create(std::string message, std::string nick_name, int64_t uint64_fromGroupOrDiscuss, int64_t uint64_fromQQ)
{
	std::smatch match_list_roll_match;
	std::regex_search(message, match_list_roll_match, *this->regex_filter_full_dice);
	if (match_list_roll_match.begin() != match_list_roll_match.end()) {
		std::string str_command = match_list_roll_match.str();
		std::string str_roll_message = match_list_roll_match.suffix().str();
		remove_space_and_tab(str_command);

		manual_dice * md_manualdice = manual_dice_manager::instance->create_manual_dice(uint64_fromQQ, uint64_fromGroupOrDiscuss, str_command);
		if (md_manualdice->status == ROLL_STATUS_FINISHED) {
			std::ostringstream ostrs_output_stream(std::ostringstream::ate);
			ostrs_output_stream << " * " << nick_name << " " << str_roll_message;
			ostrs_output_stream << " 在桌上放了这些骰子: " << str_command;
			ostrs_output_stream << " 当前状态: " << md_manualdice->ToString();
			return ostrs_output_stream.str();
		}
	}
	return std::string();
}

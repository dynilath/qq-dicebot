#include "stdafx.h"
#include "utility.h"
#include "nick_manager.h"
#include "dice_spliter.h"
#include "dice_protocol.h"
#include "protocol_roll_dice.h"

//regular dice, with detailed info

protocol_roll_dice::protocol_roll_dice()
{
	this->regex_detail_command = new std::regex("^s *");
	this->identifier = new std::string("r");
	this->regex_filter_full_dice = new std::regex("^ *(\\+|\\-)?((\\d*d\\d+((k|kl)\\d+)?)|\\d+)((\\+|\\-|\\*|/)((\\d*d\\d+((k|kl)\\d+)?)|\\d+))* *");
}


protocol_roll_dice::~protocol_roll_dice()
{
	delete this->regex_detail_command;
	delete this->identifier;
	delete this->regex_filter_full_dice;
}

std::string protocol_roll_dice::resolve_request(
	std::string &message,
	const int32_t i_AuthCode,
	const int64_t uint64_fromGroupOrDiscuss,
	const int64_t uint64_fromQQ,
	bool isfromGroup)
{
	bool detailed_roll_message = false;
	std::smatch match_list_command_detail;
	std::regex_search(message, match_list_command_detail, *this->regex_detail_command);
	if (match_list_command_detail.begin() != match_list_command_detail.end()) {
		detailed_roll_message = true;
		message = match_list_command_detail.suffix().str();
	}

	std::string str_roll_command;
	std::string str_roll_detail;
	std::string str_result;
	std::string str_message;

	binary_tree_split_dice(message, str_roll_command, str_roll_detail, str_result, str_message);

	if (str_roll_command.size() > 0) {
		std::ostringstream ostrs_output_stream(std::ostringstream::ate);

		std::string str_nickname;
		(nickname_manager::instance)->get_nickname(i_AuthCode, uint64_fromGroupOrDiscuss, uint64_fromQQ, str_nickname, isfromGroup);

		ostrs_output_stream << " * " << str_nickname << " " << str_message << " ÖÀ÷»: ";
		ostrs_output_stream << str_roll_command << " = ";
		if (detailed_roll_message && str_roll_detail.size() > 0) ostrs_output_stream << str_roll_detail << " = ";
		ostrs_output_stream << str_result;

		return ostrs_output_stream.str();
	}
	return std::string();
}

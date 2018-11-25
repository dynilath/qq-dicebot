#include "stdafx.h"
#include "utility.h"
#include "nickManager.h"
#include "diceSpliter.h"
#include "dice_protocol.h"
#include "protocol_roll_dice.h"

//regular dice, with detailed info

protocol_roll_dice::protocol_roll_dice()
{
	this->identifier = new std::string("r");
	this->regex_filter_full_dice = new std::regex("^ *(\\+|\\-)?((\\d*d\\d+((k|kl)\\d+)?)|\\d+)((\\+|\\-|\\*|/)((\\d*d\\d+((k|kl)\\d+)?)|\\d+))* *");
}


protocol_roll_dice::~protocol_roll_dice()
{
	delete this->identifier;
	delete this->regex_filter_full_dice;
}

std::string protocol_roll_dice::resolve_request(
	std::string message,
	const int32_t i_AuthCode,
	const int64_t uint64_fromGroupOrDiscuss,
	const int64_t uint64_fromQQ,
	bool isfromGroup)
{
	std::smatch match_list_command_full_dice_roll_match;
	std::regex_search(message, match_list_command_full_dice_roll_match, *this->regex_filter_full_dice);
	if (match_list_command_full_dice_roll_match.begin() != match_list_command_full_dice_roll_match.end()) {
		std::string str_roll_message = match_list_command_full_dice_roll_match.suffix().str();
		std::string str_roll_source = match_list_command_full_dice_roll_match.str();
		removeSpaceAndTab(str_roll_source);
		std::string str_roll_output;
		if (baseSplitDice(str_roll_source, str_roll_output)) {
			std::ostringstream ostrs_output_stream(std::ostringstream::ate);

			std::string str_nickname;
			(nickManager::instance)->getNickName(i_AuthCode, uint64_fromGroupOrDiscuss, uint64_fromQQ, str_nickname, isfromGroup);

			ostrs_output_stream << " * " << str_nickname << " " << str_roll_message << " ÖÀ÷»: ";
			ostrs_output_stream << str_roll_output;
			return ostrs_output_stream.str();
		}
	}
	return std::string();
}

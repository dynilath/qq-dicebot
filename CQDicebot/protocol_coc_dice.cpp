#include "stdafx.h"
#include "utility.h"
#include "nick_manager.h"
#include "diceroller.h"
#include "dice_protocol.h"
#include "protocol_coc_dice.h"


//coc7th customized dice

protocol_coc_dice::protocol_coc_dice()
{
	this->identifier = new std::string("c");
	this->regex_filter_full_dice = new std::regex("^ *((p|b)\\d+ *)*");
}


protocol_coc_dice::~protocol_coc_dice()
{
	delete this->identifier;
	delete this->regex_filter_full_dice;
}

std::string protocol_coc_dice::resolve_request(
	std::string &message,
	const int32_t i_AuthCode,
	const int64_t uint64_fromGroupOrDiscuss,
	const int64_t uint64_fromQQ,
	bool isfromGroup)
{
	std::smatch match_list_command_coc_dice_roll_match;
	std::regex_search(message, match_list_command_coc_dice_roll_match, *this->regex_filter_full_dice);
	if (match_list_command_coc_dice_roll_match.begin() != match_list_command_coc_dice_roll_match.end()) {
		std::string str_roll_message = match_list_command_coc_dice_roll_match.suffix().str();
		std::string str_roll_source = match_list_command_coc_dice_roll_match.str();
		remove_space_and_tab(str_roll_source);

		dice_roller diceRoll(str_roll_source, dice_roller::ROLL_MODE_COC_PB);
		if (diceRoll.status == dice_roller::ROLL_STATUS_FINISHED) {
			std::ostringstream ostrs_output_stream(std::ostringstream::ate);

			std::string str_nickname;
			(nickname_manager::instance)->get_nickname(i_AuthCode, uint64_fromGroupOrDiscuss, uint64_fromQQ, str_nickname, isfromGroup);

			ostrs_output_stream << " * " << str_nickname << " " << str_roll_message << " ÖÀ÷»: ";
			ostrs_output_stream << *(diceRoll.pstr_detail_result) << " = " << diceRoll.i_sum_result;
			return ostrs_output_stream.str();
		}
	}
	return std::string();
}

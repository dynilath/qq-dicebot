#include "./protocol_coc_dice.h"
#include "./dice_utils.h"
#include "./nick_manager.h"
#include "./dice_roller.h"

//coc7th customized dice
namespace dicebot::protocol{
	protocol_coc_dice::protocol_coc_dice()
	{
		this->identifier = new std::string("c");
		this->regex_filter_full_dice = new std::regex("^ *([pPbB]\\d+ *)*");
	}


	protocol_coc_dice::~protocol_coc_dice()
	{
		delete this->identifier;
		delete this->regex_filter_full_dice;
	}

	std::string protocol_coc_dice::resolve_request(
			std::string &message,
			const int64_t group_id,
			const int64_t user_qq_id,
			bool isfromGroup)
	{
		std::smatch match_list_command_coc_dice_roll_match;
		std::regex_search(message, match_list_command_coc_dice_roll_match, *this->regex_filter_full_dice);
		if (match_list_command_coc_dice_roll_match.begin() != match_list_command_coc_dice_roll_match.end()) {
			std::string str_roll_message = match_list_command_coc_dice_roll_match.suffix().str();
			std::string str_roll_source = match_list_command_coc_dice_roll_match.str();
			dicebot::remove_space_and_tab(str_roll_source);

			//roll::dice_roller diceRoll(str_roll_source, roll::roll_mode::COC_PB);
			roll::p_dice_roller dice_roll = roll::dice_roller::roll_coc(str_roll_source);
			if (dice_roll->status == roll::roll_status::FINISHED) {
				std::ostringstream ostrs_output_stream(std::ostringstream::ate);

				std::string str_nickname;
				(nickname_manager::instance)->get_nickname(group_id, user_qq_id, str_nickname, isfromGroup);

				ostrs_output_stream << u8" * " << str_nickname << u8" " << str_roll_message << u8"  掷骰: ";
				ostrs_output_stream << *(dice_roll->pstr_detail_result) << u8" = " << dice_roll->i_sum_result;
				return ostrs_output_stream.str();
			}
		}
		return std::string();
	}
}
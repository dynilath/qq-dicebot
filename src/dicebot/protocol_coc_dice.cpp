#include "./protocol_coc_dice.h"
#include "./dice_utils.h"
#include "./nick_manager.h"
#include "./dice_roller.h"

//coc7th customized dice
namespace dicebot::protocol{
	protocol_coc_dice::protocol_coc_dice()
	{
		this->identifier = new std::string("c");
		this->regex_identifier = new std::string("[cC]");
		this->regex_filter_full_dice = new std::regex("^ *([pPbB]\\d+ *)*");
	}


	protocol_coc_dice::~protocol_coc_dice()
	{
		delete this->identifier;
		delete this->regex_filter_full_dice;
		delete this->regex_identifier;
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
			roll::dice_roll dr;
			roll::roll_coc(dr,str_roll_source);
			if (dr) {
				std::ostringstream ostr(std::ostringstream::ate);

				std::string str_nickname;
				(nickname_manager::instance)->get_nickname(group_id, user_qq_id, str_nickname, isfromGroup);

				ostr << u8" * " << str_nickname;
				if(str_roll_message.size() > 0) ostr << u8" " << str_roll_message;
				ostr << u8"  掷骰: d100" << str_roll_source ;
				std::string detail = dr.detail_coc();
				if(detail.size()>0) ostr << u8" = " << detail;
				ostr << u8" = " << dr.summary;
				return ostr.str();
			}
		}
		return std::string();
	}
}
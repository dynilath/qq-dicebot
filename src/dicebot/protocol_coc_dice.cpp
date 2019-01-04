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
		std::smatch roll_match;
		std::regex_search(message, roll_match, *this->regex_filter_full_dice);
		if (roll_match.size() > 0) {
			std::string str_roll_message = roll_match.suffix().str();
			std::string str_roll_source = roll_match.str();
			dicebot::remove_space_and_tab(str_roll_source);

			//roll::dice_roller diceRoll(str_roll_source, roll::roll_mode::COC_PB);
			roll::dice_roll dr;
			roll::roll_coc(dr,str_roll_source);
			if (dr) {
				std::string str_nickname;
				(nickname_manager::instance)->get_nickname(group_id, user_qq_id, str_nickname, isfromGroup);
				
				this->create_std_output(str_nickname);
				if(str_roll_message.size() > 0) this->append_message(str_roll_message);
				this->append_roll("d100" + str_roll_source, dr.detail_coc(), dr.summary);

				return this->str();
			}
		}
		return std::string();
	}
}
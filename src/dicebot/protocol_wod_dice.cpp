#include "./protocol_wod_dice.h"
#include "./dice_utils.h"
#include "./nick_manager.h"
#include "./dice_roller.h"

namespace dicebot::protocol{
    protocol_wod_dice::protocol_wod_dice(){
		this->identifier = new std::string("w");
		this->regex_identifier = new std::string("[wW]");
		this->regex_filter_full_dice = new std::regex("^ *(\\d+)[dD](\\d+)*");
	}


	protocol_wod_dice::~protocol_wod_dice(){
		delete this->regex_detail_command;
		delete this->identifier;
		delete this->regex_filter_full_dice;
	}

	std::string protocol_wod_dice::resolve_request(
		std::string &message,
		const int64_t group_id,
		const int64_t user_qq_id,
		bool isfromGroup){
		
		std::smatch command_match;
		std::regex_search(message, command_match, *this->regex_filter_full_dice);
		if (command_match.begin() != command_match.end()) {
			std::string str_roll_msg = command_match.suffix().str();
			std::string str_roll_source = command_match.str();
			dicebot::remove_space_and_tab(str_roll_source);

			//roll::dice_roller diceRoll(str_roll_source, roll::roll_mode::COC_PB);
			roll::dice_roll dr;
			roll::roll_wod(dr,str_roll_source);
			if (dr) {
				std::ostringstream ostr(std::ostringstream::ate);

				std::string str_nickname;
				(nickname_manager::instance)->get_nickname(group_id, user_qq_id, str_nickname, isfromGroup);

				ostr << u8" * " << str_nickname << u8" " << str_roll_msg << u8"  掷骰: WOD" << str_roll_source ;
				std::string detail = dr.detail();
				if(detail.size()>0) ostr << u8" = " << detail;
				ostr << u8" = " << dr.summary;
				return ostr.str();
			}
		}
		return std::string();
    }
}
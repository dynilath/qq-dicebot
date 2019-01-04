#include "./protocol_wod_dice.h"
#include "./dice_utils.h"
#include "./nick_manager.h"
#include "./dice_roller.h"

namespace dicebot::protocol{
    protocol_wod_dice::protocol_wod_dice(){
		this->identifier = new std::string("w");
		this->regex_identifier = new std::string("[wW]");
		this->regex_filter_full_dice = new std::regex("^(\\d+)(?:[dD](\\d+))?(?:[bB](\\d+))? *");
		this->regex_detail_command = new std::regex("^([nN]|[oO]) *");

		this->method_map = new std::map<std::string, WOD_DICE_CALL_TYPE_R>();
		this->method_map->insert(std::pair<std::string, WOD_DICE_CALL_TYPE_R>("n", &protocol_wod_dice::nwod));
		this->method_map->insert(std::pair<std::string, WOD_DICE_CALL_TYPE_R>("o", &protocol_wod_dice::owod));
	}


	protocol_wod_dice::~protocol_wod_dice(){
		delete this->identifier;
		delete this->regex_identifier;
		delete this->regex_filter_full_dice;
		delete this->regex_detail_command;

		delete this->method_map;
	}

	std::string protocol_wod_dice::resolve_request(
		std::string &message,
		const int64_t group_id,
		const int64_t user_qq_id,
		bool isfromGroup){
		
		std::string str_nickname;
		(nickname_manager::instance)->get_nickname(group_id, user_qq_id, str_nickname, isfromGroup);

		std::smatch command_match;
		std::regex_search(message, command_match, *this->regex_detail_command);
		if (command_match.size() == 0)
			return std::string();

		std::string str_match = command_match[1];
		std::transform(str_match.begin(),str_match.end(),str_match.begin(),tolower);

		std::map<std::string, WOD_DICE_CALL_TYPE_R>::iterator iter = this->method_map->find(str_match);
		if (iter != method_map->end()) {
			WOD_DICE_CALL_TYPE(dice_call) = (*iter).second;
			return (this->*dice_call)(command_match.suffix().str(), str_nickname);
		}

		return std::string();
    }

	std::string protocol_wod_dice::nwod(std::string message, std::string nick_name){
		std::smatch command_match;
		ostrs ostr(ostrs::ate);
		std::regex_search(message, command_match, *this->regex_filter_full_dice);
		if (command_match.size() > 0) {
			std::string str_roll_msg = command_match.suffix().str();
			std::string str_roll_source = command_match.str();
			dicebot::remove_space_and_tab(str_roll_source);

			roll::dice_roll dr;	
			roll::roll_nwod(dr,str_roll_source);
			if (dr) {
				ostr << u8" * " << nick_name ;
				if(str_roll_msg.size() > 0) ostr << u8" " << str_roll_msg ;
				ostr << u8" 掷骰: nWoD " << str_roll_source ;
				std::string detail = dr.detail();
				if(detail.size()>0) ostr << u8" = " << detail;
				ostr << u8" = " << dr.summary;
			}
		}
		return ostr.str();
	}

	std::string protocol_wod_dice::owod(std::string message, std::string nick_name){
		std::smatch command_match;
		ostrs ostr(ostrs::ate);
		std::regex_search(message, command_match, *this->regex_filter_full_dice);
		if (command_match.size() > 0) {
			std::string str_roll_msg = command_match.suffix().str();
			std::string str_roll_source = command_match.str();
			dicebot::remove_space_and_tab(str_roll_source);

			roll::dice_roll dr;	
			roll::roll_owod(dr,str_roll_source);
			if (dr) {
				ostr << u8" * " << nick_name ;
				if(str_roll_msg.size() > 0) ostr << u8" " << str_roll_msg ;
				ostr << u8" 掷骰: oWoD " << str_roll_source ;
				std::string detail = dr.detail();
				if(detail.size()>0) ostr << u8" = " << detail;
				ostr << u8" = " << dr.summary;
			}
		}
		return ostr.str();
	}
}
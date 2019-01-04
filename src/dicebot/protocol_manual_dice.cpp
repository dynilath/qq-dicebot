#include "./protocol_manual_dice.h"


#include "./dice_roller.h"
#include "./dice_utils.h"
#include "./nick_manager.h"
#include "./manual_dice.h"
#include "./manual_dice_control.h"



namespace dicebot::protocol{

	std::regex filter_manual_dice("^ *((\\+)?\\d*[dD]\\d+)(\\+\\d*[dD]\\d+)* *");
	std::regex filter_integer("^ *(\\d+) *");

	protocol_manual_dice::protocol_manual_dice()
	{
		this->identifier = new std::string("h");
		this->regex_identifier = new std::string("[hH]");
		this->regex_detail_command = new std::regex("^([kK][aA]|[aA]|[kK]|[rR]) *");
		this->method_map = new std::map<std::string, manual_dice_call>();

		this->method_map->insert(std::pair<std::string, manual_dice_call>("ka", &manualdice_killall));
		this->method_map->insert(std::pair<std::string, manual_dice_call>("k", &manualdice_kill));
		this->method_map->insert(std::pair<std::string, manual_dice_call>("a", &manualdice_add));
		this->method_map->insert(std::pair<std::string, manual_dice_call>("r", &manualdice_roll));
	}

	protocol_manual_dice::~protocol_manual_dice()
	{
		delete this->identifier;
		delete this->regex_identifier;
		delete this->method_map;
		delete this->regex_detail_command;
	}

	std::string protocol_manual_dice::resolve_request(
			std::string &message,
			const int64_t group_id,
			const int64_t user_qq_id,
			bool isfromGroup)
	{
		std::string str_nickname;
		(nickname_manager::instance)->get_nickname(group_id, user_qq_id, str_nickname, isfromGroup);

		std::smatch match_command;
		std::regex_search(message, match_command, *(this->regex_detail_command));
		if (match_command.size() == 0)
			return manualdice_create(message, str_nickname, group_id, user_qq_id);

		std::string str_match = match_command[1];
		std::transform(str_match.begin(),str_match.end(),str_match.begin(),tolower);

		auto iter = this->method_map->find(str_match);
		if (iter != method_map->end()) {
			manual_dice_call dice_call  = (*iter).second;
			return dice_call(match_command.suffix().str(), str_nickname, group_id, user_qq_id);
		}
		return std::string();
	}

	TMDICE_CALL(manualdice_add)
	{
		std::smatch roll_match;
		std::regex_search(message, roll_match, filter_manual_dice);
		if (roll_match.size() > 0) {
			std::string str_command = roll_match.str();
			std::string str_roll_message = roll_match.suffix().str();
			remove_space_and_tab(str_command);
			dicebot::p_manual md_manualdice = dicebot::manual_dice_control::instance->add_manual_dice(user_id, group_id, str_command);
			if (md_manualdice && (*md_manualdice)) {
				ostrs ostr(ostrs::ate);
				ostr << u8" * " << nick_name;
				if(str_roll_message.size() > 0) ostr << u8" " << str_roll_message;
				ostr << u8" 在桌上增加了这些骰子: " << str_command;
				ostr << u8" 当前状态: " << md_manualdice->str();
				return ostr.str();
			}
		}
		return std::string();
	}

	TMDICE_CALL(manualdice_killall)
	{
		dicebot::p_manual md_manualdice = dicebot::manual_dice_control::instance->killall_manual_dice(user_id, group_id);
		if (md_manualdice && (*md_manualdice)) {
			ostrs ostr(ostrs::ate);
			ostr << u8" * " << nick_name;
			if(message.size() > 0) ostr << u8" " << message;
			ostr << u8" 杀掉了所有的骰子 ";
			ostr << u8"当前状态: " << md_manualdice->str();
			return ostr.str();
		}
		return std::string();
	}

	TMDICE_CALL(manualdice_kill)
	{
		std::smatch roll_match;
		std::regex_search(message, roll_match, filter_integer);
		if (roll_match.size() > 0) {
			std::string str_command = roll_match.str();
			std::string str_roll_message = roll_match.suffix().str();
			remove_space_and_tab(str_command);

			dicebot::p_manual md_manualdice = dicebot::manual_dice_control::instance->kill_manual_dice(user_id, group_id, str_command);
			if (md_manualdice && (*md_manualdice)) {
				ostrs ostr(ostrs::ate);
				ostr << u8" * " << nick_name;
				if(str_roll_message.size() > 0) ostr << u8" " << str_roll_message;
				ostr << u8" 杀死桌上的第 " << str_command << u8" 个骰子 ";
				ostr << u8"当前状态: " << md_manualdice->str();
				return ostr.str();
			}
		}
		return std::string();
	}

	TMDICE_CALL(manualdice_roll)
	{
		std::smatch roll_match;
		std::regex_search(message, roll_match, filter_integer);
		if (roll_match.size() > 0) {
			std::string str_command = roll_match.str();
			std::string str_roll_message = roll_match.suffix().str();
			remove_space_and_tab(str_command);

			dicebot::p_manual md_manualdice = dicebot::manual_dice_control::instance->roll_manual_dice(user_id, group_id, str_command);
			if (md_manualdice && (*md_manualdice)) {
				ostrs ostr(ostrs::ate);
				ostr << u8" * " << nick_name;
				if(str_roll_message.size() > 0) ostr << u8" " << str_roll_message;
				ostr << u8" 重骰桌上的第 " << str_command << u8" 个骰子 ";
				ostr << u8"当前状态: " << md_manualdice->str();
				return ostr.str();
			}
		}
		return std::string();
	}

	TMDICE_CALL(manualdice_create){
		std::smatch roll_match;
		std::regex_search(message, roll_match, filter_manual_dice);
		if (roll_match.size() > 0) {
			std::string str_command = roll_match.str();
			std::string str_roll_message = roll_match.suffix().str();
			remove_space_and_tab(str_command);

			dicebot::p_manual md_manualdice = dicebot::manual_dice_control::instance->create_manual_dice(user_id, group_id, str_command);
			if (md_manualdice && (*md_manualdice)) {
				ostrs ostr(ostrs::ate);
				ostr << u8" * " << nick_name;
				if(str_roll_message.size() > 0) ostr << u8" " << str_roll_message;
				ostr << u8" 在桌上放了这些骰子: " << str_command;
				ostr << u8" 当前状态: " << md_manualdice->str();
				return ostr.str();
			}
		}
		return std::string();
	}
}
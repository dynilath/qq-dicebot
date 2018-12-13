#include "./protocol_manual_dice.h"


#include "./dice_roller.h"
#include "./dice_utils.h"
#include "./nick_manager.h"
#include "./manual_dice.h"
#include "./manual_dice_control.h"



namespace dicebot::protocol{

	protocol_manual_dice::protocol_manual_dice()
	{
		this->identifier = new std::string("h");
		this->regex_filter_full_dice = new std::regex("^ *((\\+)?\\d*d\\d+)(\\+\\d*d\\d+)* *");
		this->regex_filter_integer_space = new std::regex("^ *\\d+ *");
		this->regex_detail_command = new std::regex("^(ka|a|k|r) *");
		this->method_map = new std::map<std::string, MANUAL_DICE_CALL_TYPE_R>();
		this->method_map->insert(std::pair<std::string, MANUAL_DICE_CALL_TYPE_R>("ka", &protocol_manual_dice::manualdice_killall));
		this->method_map->insert(std::pair<std::string, MANUAL_DICE_CALL_TYPE_R>("k", &protocol_manual_dice::manualdice_kill));
		this->method_map->insert(std::pair<std::string, MANUAL_DICE_CALL_TYPE_R>("a", &protocol_manual_dice::manualdice_add));
		this->method_map->insert(std::pair<std::string, MANUAL_DICE_CALL_TYPE_R>("r", &protocol_manual_dice::manualdice_roll));
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
			std::string &message,
			const int64_t group_id,
			const int64_t user_qq_id,
			bool isfromGroup)
	{
		std::string str_nickname;
		(nickname_manager::instance)->get_nickname(group_id, user_qq_id, str_nickname, isfromGroup);

		std::smatch match_list_command_identifier_match;
		std::regex_search(message, match_list_command_identifier_match, *this->regex_detail_command);
		if (match_list_command_identifier_match.begin() == match_list_command_identifier_match.end())
			return manualdice_create(message, str_nickname, group_id, user_qq_id);
		std::string str_match = match_list_command_identifier_match[1];
		std::map<std::string, MANUAL_DICE_CALL_TYPE_R>::iterator iter = this->method_map->find(str_match);
		if (iter != method_map->end()) {
			MANUAL_DICE_CALL_TYPE(dice_call) = (*iter).second;
			return (this->*dice_call)(match_list_command_identifier_match.suffix().str(), str_nickname, group_id, user_qq_id);
		}
		return std::string();
	}

	std::string protocol_manual_dice::manualdice_add(std::string message, std::string nick_name, int64_t group_id, int64_t user_qq_id)
	{
		std::smatch match_list_roll_match;
		std::regex_search(message, match_list_roll_match, *this->regex_filter_full_dice);
		if (match_list_roll_match.begin() != match_list_roll_match.end()) {
			std::string str_command = match_list_roll_match.str();
			std::string str_roll_message = match_list_roll_match.suffix().str();
			remove_space_and_tab(str_command);
			dicebot::manual_dice * md_manualdice = dicebot::manual_dice_control::instance->add_manual_dice(user_qq_id, group_id, str_command);
			if ((*md_manualdice).status == dicebot::roll::roll_status::FINISHED) {
				std::ostringstream ostrs_output_stream(std::ostringstream::ate);
				ostrs_output_stream << u8" * " << nick_name << u8" " << str_roll_message;
				ostrs_output_stream << u8" 在桌上增加了这些骰子: " << str_command;
				ostrs_output_stream << u8" 当前状态: " << md_manualdice->str();
				return ostrs_output_stream.str();
			}
		}
		return std::string();
	}

	std::string protocol_manual_dice::manualdice_killall(std::string message, std::string nick_name, int64_t group_id, int64_t user_qq_id)
	{
		dicebot::manual_dice * md_manualdice = dicebot::manual_dice_control::instance->killall_manual_dice(user_qq_id, group_id);
		if ((*md_manualdice).status == dicebot::roll::roll_status::FINISHED) {
			std::ostringstream ostrs_output_stream(std::ostringstream::ate);
			ostrs_output_stream << u8" * " << nick_name << u8" " << message;
			ostrs_output_stream << u8" 杀掉了所有的骰子 ";
			ostrs_output_stream << u8"当前状态: " << md_manualdice->str();
			return ostrs_output_stream.str();
		}
		return std::string();
	}

	std::string protocol_manual_dice::manualdice_kill(std::string message, std::string nick_name, int64_t group_id, int64_t user_qq_id)
	{
		std::smatch match_list_roll_match;
		std::regex_search(message, match_list_roll_match, *this->regex_filter_integer_space);
		if (match_list_roll_match.begin() != match_list_roll_match.end()) {
			std::string str_command = match_list_roll_match.str();
			std::string str_roll_message = match_list_roll_match.suffix().str();
			remove_space_and_tab(str_command);

			dicebot::manual_dice * md_manualdice = dicebot::manual_dice_control::instance->kill_manual_dice(user_qq_id, group_id, str_command);
			if ((*md_manualdice).status == dicebot::roll::roll_status::FINISHED) {
				std::ostringstream ostrs_output_stream(std::ostringstream::ate);
				ostrs_output_stream << u8" * " << nick_name << u8" " << str_roll_message;
				ostrs_output_stream << u8" 杀死桌上的第 " << str_command << u8" 个骰子 ";
				ostrs_output_stream << u8"当前状态: " << md_manualdice->str();
				return ostrs_output_stream.str();
			}
		}
		return std::string();
	}

	std::string protocol_manual_dice::manualdice_roll(std::string message, std::string nick_name, int64_t group_id, int64_t user_qq_id)
	{
		std::smatch match_list_roll_match;
		std::regex_search(message, match_list_roll_match, *this->regex_filter_integer_space);
		if (match_list_roll_match.begin() != match_list_roll_match.end()) {
			std::string str_command = match_list_roll_match.str();
			std::string str_roll_message = match_list_roll_match.suffix().str();
			remove_space_and_tab(str_command);

			dicebot::manual_dice * md_manualdice = dicebot::manual_dice_control::instance->roll_manual_dice(user_qq_id, group_id, str_command);
			if ((*md_manualdice).status == dicebot::roll::roll_status::FINISHED) {
				std::ostringstream ostrs_output_stream(std::ostringstream::ate);
				ostrs_output_stream << u8" * " << nick_name << u8" " << str_roll_message;
				ostrs_output_stream << u8" 重骰桌上的第 " << str_command << u8" 个骰子 ";
				ostrs_output_stream << u8"当前状态: " << md_manualdice->str();
				return ostrs_output_stream.str();
			}
		}
		return std::string();
	}

	std::string protocol_manual_dice::manualdice_create(std::string message, std::string nick_name, int64_t group_id, int64_t user_qq_id)
	{
		std::smatch match_list_roll_match;
		std::regex_search(message, match_list_roll_match, *this->regex_filter_full_dice);
		if (match_list_roll_match.begin() != match_list_roll_match.end()) {
			std::string str_command = match_list_roll_match.str();
			std::string str_roll_message = match_list_roll_match.suffix().str();
			remove_space_and_tab(str_command);

			dicebot::manual_dice * md_manualdice = dicebot::manual_dice_control::instance->create_manual_dice(user_qq_id, group_id, str_command);
			if (md_manualdice->status == dicebot::roll::roll_status::FINISHED) {
				std::ostringstream ostrs_output_stream(std::ostringstream::ate);
				ostrs_output_stream << u8" * " << nick_name << u8" " << str_roll_message;
				ostrs_output_stream << u8" 在桌上放了这些骰子: " << str_command;
				ostrs_output_stream << u8" 当前状态: " << md_manualdice->str();
				return ostrs_output_stream.str();
			}
		}
		return std::string();
	}
}
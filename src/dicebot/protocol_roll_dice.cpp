#include "./protocol_roll_dice.h"

#include "./dice_utils.h"
#include "./nick_manager.h"
#include "./dice_spliter.h"

//regular dice, with detailed info

namespace dicebot::protocol{
	protocol_roll_dice::protocol_roll_dice()
	{
		this->identifier = new std::string("r");
		this->regex_identifier = new std::string("[rR]");
		this->regex_detail_command = new std::regex("^[sS] *");
	}


	protocol_roll_dice::~protocol_roll_dice()
	{
		delete this->regex_detail_command;
		delete this->identifier;
		delete this->regex_identifier;
	}

	std::string protocol_roll_dice::resolve_request(
		std::string &message,
		const int64_t group_id,
		const int64_t user_qq_id,
		bool isfromGroup){
		bool detailed_roll_message = false;
		std::smatch match_list_command_detail;
		std::regex_search(message, match_list_command_detail, *this->regex_detail_command);
		if (match_list_command_detail.size() > 0) {
			detailed_roll_message = true;
			message = match_list_command_detail.suffix().str();
		}

		std::string str_roll_command;
		std::string str_roll_detail;
		std::string str_result;
		std::string str_message;

		binary_tree_split_dice(message, str_roll_command, str_roll_detail, str_result, str_message);

		if (str_roll_command.size() > 0) {
			ostrs ostr(ostrs::ate);

			std::string str_nickname;
			(nickname_manager::instance)->get_nickname(group_id, user_qq_id, str_nickname, isfromGroup);

			ostr << u8" * " << str_nickname;
			if(str_message.size() > 0) ostr << u8" " << str_message;
			ostr << u8" 掷骰: " << str_roll_command << u8" = ";
			if (detailed_roll_message && str_roll_detail.size() > 0) ostr << str_roll_detail << u8" = ";
			ostr << str_result;

			return ostr.str();
		}
		return std::string();
	}
}
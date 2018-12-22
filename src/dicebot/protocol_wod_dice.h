#pragma once

#include "./common.h"
#include "./protocol_base.h"

namespace dicebot::protocol{

	#define WOD_DICE_CALL_TYPE_R std::string(protocol_wod_dice::* )(std::string, std::string)
	#define WOD_DICE_CALL_TYPE(_name) std::string(protocol_wod_dice::* _name )(std::string, std::string)

	class protocol_wod_dice : public protocol_base
	{
	private:
		std::regex * regex_detail_command;
		std::map<std::string, WOD_DICE_CALL_TYPE_R> * method_map;
	public:
		protocol_wod_dice();
		~protocol_wod_dice();
		std::string resolve_request(
			std::string &message,
			const int64_t group_id,
			const int64_t user_qq_id,
			bool isfromGroup);

		std::string nwod(std::string message, std::string nick_name);
		std::string owod(std::string message, std::string nick_name);
	};
}
#pragma once

#include "./common.h"
#include "./protocol_base.h"

namespace dicebot::protocol{
	#define MANUAL_DICE_CALL_TYPE_R std::string(protocol_manual_dice::* )(std::string, std::string, int64_t, int64_t)
	#define MANUAL_DICE_CALL_TYPE(_name) std::string(protocol_manual_dice::* _name )(std::string, std::string, int64_t, int64_t)

	class protocol_manual_dice : public protocol_base
	{
	private:
		std::regex * regex_detail_command;
		std::regex * regex_filter_integer_space;
		std::map<std::string, MANUAL_DICE_CALL_TYPE_R> * method_map;
	public:
		protocol_manual_dice();
		~protocol_manual_dice();
		std::string resolve_request(
			std::string &message,
			const int64_t group_id,
			const int64_t user_qq_id,
			bool isfromGroup);

		std::string manualdice_add(std::string message, std::string nick_name, int64_t uint64_fromGroupOrDiscuss, int64_t uint64_fromQQ);
		std::string manualdice_killall(std::string message, std::string nick_name, int64_t uint64_fromGroupOrDiscuss, int64_t uint64_fromQQ);
		std::string manualdice_kill(std::string message, std::string nick_name, int64_t uint64_fromGroupOrDiscuss, int64_t uint64_fromQQ);
		std::string manualdice_roll(std::string message, std::string nick_name, int64_t uint64_fromGroupOrDiscuss, int64_t uint64_fromQQ);
		std::string manualdice_create(std::string message, std::string nick_name, int64_t uint64_fromGroupOrDiscuss, int64_t uint64_fromQQ);
	};
}
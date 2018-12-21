#pragma once

#include "./common.h"
#include "./protocol_base.h"

namespace dicebot::protocol{
	class protocol_roll_dice : public protocol_base
	{
	private:
		std::regex * regex_detail_command;
	public:
		protocol_roll_dice();
		~protocol_roll_dice();
		std::string resolve_request(
			std::string &message,
			const int64_t group_id,
			const int64_t user_qq_id,
			bool isfromGroup);
	};
}

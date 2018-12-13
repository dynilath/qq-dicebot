#pragma once

#include "./common.h"

namespace dicebot::protocol{
	class protocol_nickname :public protocol_base
	{
	private:
		std::regex * regex_detail_command;
	public:
		protocol_nickname();
		~protocol_nickname(); 
		std::string resolve_request(
			std::string &message,
			const int64_t group_id,
			const int64_t user_qq_id,
			bool isfromGroup);
	};
}

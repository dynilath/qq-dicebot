#pragma once

#include "./common.h"
#include "./protocol_base.h"

namespace dicebot::protocol{
	class protocol_coc_dice : public protocol_base
	{
	public:
		protocol_coc_dice();
		~protocol_coc_dice();
		std::string resolve_request(
			std::string &message,
			const int64_t group_id,
			const int64_t user_qq_id,
			bool isfromGroup);
	};
}

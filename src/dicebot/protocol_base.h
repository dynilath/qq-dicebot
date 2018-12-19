#pragma once

#include "./common.h"

namespace dicebot::protocol{
	typedef std::string(*dice_request)(
		std::string message,
		const int32_t i_AuthCode,
		const int64_t uint64_fromGroupOrDiscuss,
		const int64_t uint64_fromQQ,
		bool isfromGroup);

	class protocol_base
	{
	protected:
		std::string * identifier;
		std::string * regex_identifier;
		std::regex * regex_filter_full_dice;
	public:
		int dynamic_protocol_id;
		std::string get_identifier() { return *(this->identifier); };
		std::string get_regex_identifier() { return *(this->regex_identifier); };
		virtual std::string resolve_request(
			std::string &message,
			const int64_t group_id,
			const int64_t user_qq_id,
			bool isfromGroup)
		{ return std::string(); };
	};
}

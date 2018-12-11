#pragma once
#include "stdafx.h"

typedef std::string(*dice_request)(
	std::string message,
	const int32_t i_AuthCode,
	const int64_t uint64_fromGroupOrDiscuss,
	const int64_t uint64_fromQQ,
	bool isfromGroup);

class dice_protocol
{
protected:
	std::string * identifier;
	std::regex * regex_filter_full_dice;
public:
	int dynamic_protocol_id;
	std::string get_identifier() { return *(this->identifier); };
	virtual std::string resolve_request(
		std::string &message,
		const int32_t i_AuthCode,
		const int64_t uint64_fromGroupOrDiscuss,
		const int64_t uint64_fromQQ,
		bool isfromGroup)
	{ return std::string(); };
};


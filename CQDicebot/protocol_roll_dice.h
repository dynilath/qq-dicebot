#pragma once


class protocol_roll_dice : public dice_protocol
{
private:
	std::regex * regex_detail_command;
public:
	protocol_roll_dice();
	~protocol_roll_dice();
	std::string resolve_request(
		std::string &message,
		const int32_t i_AuthCode,
		const int64_t uint64_fromGroupOrDiscuss,
		const int64_t uint64_fromQQ,
		bool isfromGroup);
};


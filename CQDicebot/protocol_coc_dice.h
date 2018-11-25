#pragma once
class protocol_coc_dice : public dice_protocol
{
public:
	protocol_coc_dice();
	~protocol_coc_dice();
	std::string resolve_request(
		std::string message,
		const int32_t i_AuthCode,
		const int64_t uint64_fromGroupOrDiscuss,
		const int64_t uint64_fromQQ,
		bool isfromGroup);
};


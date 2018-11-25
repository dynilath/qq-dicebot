#pragma once
class protocol_nickname :public dice_protocol
{
public:
	protocol_nickname();
	~protocol_nickname(); 
	std::string resolve_request(
		std::string message,
		const int32_t i_AuthCode,
		const int64_t uint64_fromGroupOrDiscuss,
		const int64_t uint64_fromQQ,
		bool isfromGroup);
};


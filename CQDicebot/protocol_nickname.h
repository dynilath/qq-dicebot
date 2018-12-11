#pragma once
class protocol_nickname :public dice_protocol
{
private:
	std::regex * regex_detail_command;
public:
	protocol_nickname();
	~protocol_nickname(); 
	std::string resolve_request(
		std::string &message,
		const int32_t i_AuthCode,
		const int64_t uint64_fromGroupOrDiscuss,
		const int64_t uint64_fromQQ,
		bool isfromGroup);
};


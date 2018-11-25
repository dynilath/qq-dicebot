#include "stdafx.h"
#include "dice_protocol.h"

#include "cqp.h"
#include "QTool.h"
#include "nick_manager.h"

#include "protocol_nickname.h"

std::regex regex_nickname_silence("^s *");

protocol_nickname::protocol_nickname()
{
	this->identifier = new std::string("n");
}


protocol_nickname::~protocol_nickname()
{
	delete this->identifier;
}

std::string protocol_nickname::resolve_request(std::string message, const int32_t i_AuthCode, const int64_t uint64_fromGroupOrDiscuss, const int64_t uint64_fromQQ, bool isfromGroup)
{
	bool is_silence = false;
	std::smatch match_list_silence;
	std::regex_search(message, match_list_silence, regex_nickname_silence);
	if (match_list_silence.begin() == match_list_silence.end() && message.length() > 0) {
		std::string str_origin_name;
		CQTool::getDefaultName(i_AuthCode, uint64_fromGroupOrDiscuss, uint64_fromQQ, str_origin_name, isfromGroup);
		(nickname_manager::instance)->set_nickname(i_AuthCode, uint64_fromGroupOrDiscuss, uint64_fromQQ, message, isfromGroup);

		std::ostringstream ostrs_output_stream(std::ostringstream::ate);
		ostrs_output_stream << " * " << str_origin_name << " 的新名字是 " << message;
		return ostrs_output_stream.str();
	}
	else {
		std::string s_message = match_list_silence.suffix().str();
		if(s_message.length() > 0)
			(nickname_manager::instance)->set_nickname(i_AuthCode, uint64_fromGroupOrDiscuss, uint64_fromQQ, s_message, isfromGroup);
	}
	return std::string();
}

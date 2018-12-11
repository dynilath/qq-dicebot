#include "stdafx.h"
#include "dice_protocol.h"

#include "cqp.h"
#include "QTool.h"
#include "nick_manager.h"

#include "protocol_nickname.h"

protocol_nickname::protocol_nickname()
{
	this->identifier = new std::string("n");
	this->regex_detail_command = new std::regex("^s *");
}


protocol_nickname::~protocol_nickname()
{
	delete this->identifier;
	delete this->regex_detail_command;
}

std::string protocol_nickname::resolve_request(std::string &message, const int32_t i_AuthCode, const int64_t uint64_fromGroupOrDiscuss, const int64_t uint64_fromQQ, bool isfromGroup)
{
	bool is_silence = false;
	std::smatch match_list_silence;
	std::regex_search(message, match_list_silence, *this->regex_detail_command);

	if (match_list_silence.begin() == match_list_silence.end()) {

		int pos_t = message.find_first_not_of(" \t");
		if(pos_t != std::string ::npos) message = message.substr(pos_t);

		if (message.length() > 0) {
			std::string str_origin_name;
			CQTool::getDefaultName(i_AuthCode, uint64_fromGroupOrDiscuss, uint64_fromQQ, str_origin_name, isfromGroup);
			(nickname_manager::instance)->set_nickname(i_AuthCode, uint64_fromGroupOrDiscuss, uint64_fromQQ, message, isfromGroup);

			std::ostringstream ostrs_output_stream(std::ostringstream::ate);
			ostrs_output_stream << " * " << str_origin_name << " 的新名字是 " << message;
			return ostrs_output_stream.str();
		}
	}
	else {
		std::string s_message = match_list_silence.suffix().str();
		int pos_t = s_message.find_first_not_of(" \t");
		if (pos_t != std::string::npos) s_message = s_message.substr(pos_t);

		if(s_message.length() > 0)
			(nickname_manager::instance)->set_nickname(i_AuthCode, uint64_fromGroupOrDiscuss, uint64_fromQQ, s_message, isfromGroup);
	}
	return std::string();
}

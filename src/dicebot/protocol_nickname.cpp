#include "./protocol_nickname.h"

#include "../cqsdk/api.h"


#include "./nick_manager.h"

namespace cqapi = cq::api;

namespace dicebot::protocol{
	protocol_nickname::protocol_nickname()
	{
		this->identifier = new std::string("n");
		this->regex_identifier = new std::string("[nN]");
		this->regex_detail_command = new std::regex("^[sS] *");
	}


	protocol_nickname::~protocol_nickname()
	{
		delete this->identifier;
		delete this->regex_detail_command;
		delete this->regex_identifier;
	}

	std::string protocol_nickname::resolve_request(
		std::string &message,
		const int64_t group_id,
		const int64_t user_qq_id,
		bool isfromGroup)
	{
		bool is_silence = false;
		std::smatch match_list_silence;
		std::regex_search(message, match_list_silence, *this->regex_detail_command);

		if (match_list_silence.begin() == match_list_silence.end()) {

			int pos_t = message.find_first_not_of(" \t");
			if(pos_t != std::string ::npos) message = message.substr(pos_t);

			if (message.length() > 0) {
				std::string str_origin_name;

				if(isfromGroup){
					cq::GroupMember group_member = cqapi::get_group_member_info(group_id, user_qq_id);
					str_origin_name.assign(group_member.nickname);
				}
				else{
					cq::User ano = cqapi::get_stranger_info( user_qq_id);
					str_origin_name.assign(ano.nickname);
				}

				std::ostringstream ostrs_output_stream(std::ostringstream::ate);
				ostrs_output_stream << u8" * " << str_origin_name << u8" 的新名字是 " << message;
				return ostrs_output_stream.str();
			}
		}
		else {
			std::string s_message = match_list_silence.suffix().str();
			int pos_t = s_message.find_first_not_of(" \t");
			if (pos_t != std::string::npos) s_message = s_message.substr(pos_t);

			if(s_message.length() > 0)
				(nickname_manager::instance)->set_nickname(group_id, user_qq_id, s_message, isfromGroup);
		}
		return std::string();
	}
}
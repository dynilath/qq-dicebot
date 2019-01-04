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
	private:
		ostrs protocol_ot;
	protected:
		std::string * identifier;
		std::string * regex_identifier;
		std::regex * regex_filter_full_dice;

		void create_std_output(std::string & nickname){
			protocol_ot.str("");
			protocol_ot << " * " << nickname;
		}
		void append_message(std::string & message){
			protocol_ot << " " << message;
		}
		void append_roll(std::string & roll_command, std::string & detail, std::string & roll_result){
			protocol_ot << " 掷骰： " << roll_command;
			if(detail.size() > 0) protocol_ot << " = " << detail;
			protocol_ot<<" = " << roll_result;
		}

		void append_roll(std::string & roll_command, std::string & detail, int roll_result){
			protocol_ot << " 掷骰： " << roll_command;
			if(detail.size() > 0) protocol_ot << " = " << detail;
			protocol_ot<<" = " << roll_result;
		}

		std::string str() noexcept{
			return protocol_ot.str();
		}

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

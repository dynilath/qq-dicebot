#pragma once

#include "./common.h"
#include "./protocol_base.h"

namespace dicebot{
	class protocol_manager
	{
	private:
		std::map<std::string, std::shared_ptr<protocol::protocol_base> > * dice_protocol_map;
		std::regex * regex_command;
	public:
		protocol_manager();
		~protocol_manager();
		void register_dice(std::shared_ptr<protocol::protocol_base> protocol);
		void create_command_regex();
		protocol::protocol_base * get_protocol(std::string command);
		std::regex * get_regex_command();
	};
}
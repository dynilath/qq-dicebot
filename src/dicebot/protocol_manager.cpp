#include "./protocol_manager.h"

#include <memory>

namespace dicebot{
	protocol_manager::protocol_manager()
	{
		this->dice_protocol_map = new std::map<std::string, std::shared_ptr<protocol::protocol_base> >();
	}


	protocol_manager::~protocol_manager()
	{
		delete this->dice_protocol_map;
		if (this->regex_command != nullptr) delete this->regex_command;
	}

	void protocol_manager::register_dice(std::shared_ptr<protocol::protocol_base> protocol)
	{
		std::string command = protocol->get_identifier();
		auto iter = this->dice_protocol_map->find(command);
		if (iter != dice_protocol_map->end()) return;
		dice_protocol_map->insert(std::pair<std::string, std::shared_ptr<protocol::protocol_base> >(command, protocol));
	}

	void protocol_manager::create_command_regex()
	{
		ostrs ostrs_stream(ostrs::ate);
		ostrs_stream << "^ *\\. *(";
		bool is_first = true;
		auto iter = this->dice_protocol_map->cbegin();
		for (; iter != this->dice_protocol_map->cend(); iter++) {
			if (is_first) {
				is_first = false;
			}
			else ostrs_stream << "|";
			ostrs_stream << (*iter).second->get_regex_identifier();
		}
		ostrs_stream << ")";
		this->regex_command = new std::regex(ostrs_stream.str());
	}

	protocol::protocol_base * protocol_manager::get_protocol(std::string command)
	{
		std::transform(command.begin(),command.end(),command.begin(),tolower);
		auto  iter = this->dice_protocol_map->find(command);
		if (iter == dice_protocol_map->end()) return nullptr;
		else return (*iter).second.get();
	}

	std::regex * protocol_manager::get_regex_command()
	{
		return this->regex_command;
	}
}
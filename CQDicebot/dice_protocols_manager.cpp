#include "stdafx.h"
#include "dice_protocol.h"


#include "dice_protocols_manager.h"



dice_protocols_manager::dice_protocols_manager()
{
	this->dice_protocol_map = new std::map<std::string, std::shared_ptr<dice_protocol> >();
}


dice_protocols_manager::~dice_protocols_manager()
{
	delete this->dice_protocol_map;
	if (this->regex_command != nullptr) delete this->regex_command;
}

void dice_protocols_manager::register_dice(std::shared_ptr<dice_protocol> protocol)
{
	std::string command = protocol->get_identifier();
	std::map<std::string, std::shared_ptr<dice_protocol> >::iterator  iter = this->dice_protocol_map->find(command);
	if (iter != dice_protocol_map->end()) return;
	dice_protocol_map->insert(std::pair<std::string, std::shared_ptr<dice_protocol> >(command, protocol));
}

void dice_protocols_manager::create_command_regex()
{
	std::ostringstream ostrs_stream(std::ostringstream::ate);
	ostrs_stream << "^ *\\. *(";
	bool is_first = true;
	std::map<std::string, std::shared_ptr<dice_protocol>>::iterator  iter = this->dice_protocol_map->begin();
	for (; iter != this->dice_protocol_map->end(); iter++) {
		if (is_first) {
			is_first = false;
		}
		else ostrs_stream << "|";
		ostrs_stream << (*iter).first;
	}
	ostrs_stream << ")";
	this->regex_command = new std::regex(ostrs_stream.str());
}

dice_protocol * dice_protocols_manager::get_protocol(std::string command)
{
	std::map<std::string, std::shared_ptr<dice_protocol> >::iterator  iter = this->dice_protocol_map->find(command);
	if (iter == dice_protocol_map->end()) return nullptr;
	else return (*iter).second.get();
}

std::regex * dice_protocols_manager::get_regex_command()
{
	return this->regex_command;
}

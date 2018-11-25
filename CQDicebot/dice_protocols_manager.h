#pragma once
class dice_protocols_manager
{
private:
	std::map<std::string, dice_protocol * > * dice_protocol_map;
	std::regex * regex_command;
public:
	dice_protocols_manager();
	~dice_protocols_manager();
	void register_dice(dice_protocol * protocol);
	void create_command_regex();
	dice_protocol * get_protocol(std::string command);
	std::regex * get_regex_command();
};


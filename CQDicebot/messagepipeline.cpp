#include "stdafx.h"
#include "dice_protocol.h"
#include "utility.h"
#include "diceroller.h"

#include "manualdice.h"
#include "manual_dice_control.h"

#include "nick_manager.h"
#include "dice_protocols_manager.h"

#include "cqp.h"
#include "QTool.h"
#include "dice_spliter.h"

#include <iostream>

#include "messagepipeline.h"


std::regex regex_filter_command_identifier("^ *\\. *(h|c|r|n)");

//TODO
//macro, use macro to save a specific regular diceroll, like .m 4d6+4 my_biggest_trick
//spaces are not allowed in macro names
std::regex regex_filter_macro_dice("^ *\\. *m *");
//use specific diceroll, like .rm my_biggest_trick
std::regex regex_filter_use_macro("^ *\\. *rm *");
//use specific diceroll, but the detail of rolling wont be displayed, only the result
std::regex regex_filter_use_macro_silent("^ *\\. *rms *");

//TODO
//define result, if you use ".def 1 i like it", any dice roll you get a result of 1 will be shown as "i like it"
std::regex regex_filter_def_result("^ *\\. *def *");
//macro specific define result, it only apply to specific macro
std::regex regex_filter_macro_def("^ *\\. *mdef *");


#define CHECK_LASTLINE_FOR_ENDL(_Stream,_Flag) if (_Flag) {	_Flag = false;	_Stream << std::endl;}

std::string str_help_message("DiceBot by dynilath v1.4.2\r\n访问项目主页http://github.com/dynilath/CoolQDicebot\r\n获得使用说明以及了解更多内容。");

bool group_message_pipeline(
	dice_protocols_manager * dice_protocols,
	const int32_t i_AuthCode,
	const char * msg, 
	const int64_t uint64_fromGroupOrDiscuss, 
	const int64_t uint64_fromQQ,
	bool isfromGroup)
{
	std::string source(msg);
	std::vector<std::string> source_splits;

	if (source == ".help") {
		if (isfromGroup)	CQ_sendGroupMsg(i_AuthCode, uint64_fromGroupOrDiscuss, str_help_message.c_str());
		else CQ_sendDiscussMsg(i_AuthCode, uint64_fromGroupOrDiscuss, str_help_message.c_str());
		return true;
	}

	split_line(source, source_splits);

	std::vector<std::string>::iterator iterator_sources = source_splits.begin();

	std::ostringstream ostrs_output_stream(std::ostringstream::ate);

	bool is_output_available = false;
	bool does_last_line_have_output = false;

	for (; iterator_sources != source_splits.end(); iterator_sources++) {
		std::smatch matchList_command_identifier_match;
		std::regex_search((*iterator_sources), matchList_command_identifier_match, *(dice_protocols->get_regex_command()));
		if (matchList_command_identifier_match.begin() == matchList_command_identifier_match.end()) continue;

		std::string command = matchList_command_identifier_match[1];
		dice_protocol * target_protocol = dice_protocols->get_protocol(command);

		std::string response = target_protocol->resolve_request(
			matchList_command_identifier_match.suffix().str(),
			i_AuthCode,
			uint64_fromGroupOrDiscuss,
			uint64_fromQQ,
			isfromGroup
		);
		if (response.size() > 0) {
			if (is_output_available) ostrs_output_stream << std::endl;
			ostrs_output_stream << response;
			is_output_available = true;
			continue;
		}

	}
	if (is_output_available) {
		if(isfromGroup)	CQ_sendGroupMsg(i_AuthCode, uint64_fromGroupOrDiscuss, ostrs_output_stream.str().c_str());
		else CQ_sendDiscussMsg(i_AuthCode, uint64_fromGroupOrDiscuss, ostrs_output_stream.str().c_str());
	}
	return is_output_available;
}



#include "stdafx.h"
#include "dice_protocol.h"
#include "utility.h"
#include "diceroller.h"

#include "manualdice.h"
#include "manualDiceControl.h"

#include "nickManager.h"

#include "cqp.h"
#include "QTool.h"
#include "diceSpliter.h"
#include <regex>
#include <iostream>

#include "messagepipeline.h"


std::regex regex_filter_command_identifier("^ *\\. *(h|c|r|n)");
//rename loudly
//std::regex regex_filter_rename("^ *\\. *n *");
//rename silently
//std::regex regex_filter_rename_silence("^ *\\. *ns *");

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

std::regex regex_filter_manual_full_dice("^((\\+)?\\d*d\\d+)(\\+\\d*d\\d+)* *");
std::regex regex_filter_integer_space("^\\d+ *");


#define CHECK_LASTLINE_FOR_ENDL(_Stream,_Flag) if (_Flag) {	_Flag = false;	_Stream << std::endl;}

bool group_message_pipeline(
	std::vector<dice_protocol * > * diceProtocols,
	const int32_t i_AuthCode,
	const char * msg, 
	const int64_t uint64_fromGroupOrDiscuss, 
	const int64_t uint64_fromQQ,
	bool isfromGroup)
{

	std::string source(msg);
	std::vector<std::string> source_splits;

	splitLine(source, source_splits);

	std::vector<std::string>::iterator iterator_sources = source_splits.begin();

	std::ostringstream ostrs_output_stream(std::ostringstream::ate);

	bool is_output_available = false;
	bool does_last_line_have_output = false;

	for (; iterator_sources != source_splits.end(); iterator_sources++) {

		std::vector<dice_protocol * >::iterator iter_protocals = diceProtocols->begin();
		std::smatch matchList_command_identifier_match;
		std::regex_search((*iterator_sources), matchList_command_identifier_match, regex_filter_command_identifier);
		if (matchList_command_identifier_match.begin() == matchList_command_identifier_match.end()) continue;

		bool go_next_line = false;
		for (; iter_protocals != diceProtocols->end(); iter_protocals++) {
			std::string command = matchList_command_identifier_match[1];
			if (!((*iter_protocals)->get_identifier().compare(command))) {
				std::string response = (*iter_protocals)->resolve_request(
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
				}
				break;
				go_next_line = true;
			}
		}
		if (go_next_line) continue;
	}
	if (is_output_available) {
		if(isfromGroup)	CQ_sendGroupMsg(i_AuthCode, uint64_fromGroupOrDiscuss, ostrs_output_stream.str().c_str());
		else CQ_sendDiscussMsg(i_AuthCode, uint64_fromGroupOrDiscuss, ostrs_output_stream.str().c_str());
	}
	return is_output_available;
}



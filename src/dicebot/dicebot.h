#pragma once

#include "./common.h"
#include "./dice_utils.h"
#include "./number.h"

#include <memory>

#include "./dice_roller.h"
#include "./protocol_manager.h"
#include "./protocol_base.h"
#include "./protocol_coc_dice.h"
#include "./protocol_nickname.h"
#include "./protocol_roll_dice.h"
#include "./protocol_manual_dice.h"
#include "./protocol_wod_dice.h"
#include "./nick_manager.h"
#include "./database_manager.h"
#include "./manual_dice_control.h"

#include "../cqsdk/utils/base64.h"

namespace base64 = cq::utils::base64;

namespace dicebot{
    protocol_manager * dice_protocols;
    nickname_manager * nick_ctrl;
    database::database_manager * db_ctrl;
    manual_dice_control * md_ctrl;

	std::string str_help_message("RGljZUJvdCBieSBkeW5pbGF0aCB2MS41LjAK6K6/6Zeu6aG555uu5Li76aG1aHR0cDovL2dpdGh1Yi5jb20vZHluaWxhdGgvY29vbHEtZGljZWJvdArojrflvpfkvb/nlKjor7TmmI7ku6Xlj4rkuobop6Pmm7TlpJrlhoXlrrnjgIIK5L2/55So5pa55byP566A5LuL77yaCi5yIDJkNiszIOaZrumAmumqsOWtkAouYyBjb2PpqrDlrZAK6K+m57uG5oyH5Luk5Y+C6ICD6K+35YmN5b6A6aG555uu5Li76aG144CCCuWmguaenOWcqOS9v+eUqOS4remBh+WIsOS6hmJ1Z++8jOaIluiAheacieS7gOS5iOWKn+iDveW7uuiuru+8jOasoui/juWcqOmhueebruS4u+mhtemAmui/h2lzc3Vl5Y+N6aaI77yM5oSf6LCi5oKo55qE5biu5Yqp44CC");
	
    void initialize(std::string app_dir){
        db_ctrl = new database::database_manager(app_dir);
        nick_ctrl = new nickname_manager();
        md_ctrl = new manual_dice_control();

        roll::random_initialize();
        dice_protocols = new protocol_manager();
        dice_protocols->register_dice(std::make_shared<protocol::protocol_roll_dice>());
        dice_protocols->register_dice(std::make_shared<protocol::protocol_coc_dice>());
        dice_protocols->register_dice(std::make_shared<protocol::protocol_manual_dice>());
        dice_protocols->register_dice(std::make_shared<protocol::protocol_nickname>());
        dice_protocols->register_dice(std::make_shared<protocol::protocol_wod_dice>());
	    dice_protocols->create_command_regex();
    }

	void salvage(){
		if(dice_protocols) delete dice_protocols;
		if(nick_ctrl) delete nick_ctrl;
		if(db_ctrl) delete db_ctrl;
		if(md_ctrl) delete md_ctrl;
	}

    void set_logger(std::function<void(std::string,std::string)> varlog){
        logger::log = varlog;
    }

	bool try_get_nickname(int64_t group_id, int64_t user_id, std::string &str_out_nick){
		str_out_nick.assign("");
		return true;
	}

    bool group_message_pipeline(
		std::string source,
		const int64_t fromGroup, 
		const int64_t fromQQ, 
		bool isfromGroup,
		std::string &output){

		std::vector<std::string> source_splits;

		if (source == ".help") {
			output.assign(base64::decode(str_help_message));
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
			protocol::protocol_base * target_protocol = dice_protocols->get_protocol(command);

			std::string response = target_protocol->resolve_request(
				matchList_command_identifier_match.suffix().str(),
				fromGroup,
				fromQQ,
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
			output.assign(ostrs_output_stream.str());
		}
		return is_output_available;
	}
}
#pragma once

#include "./common.h"
#include "./protocol_base.h"
#include <functional>

namespace dicebot::protocol{


	class protocol_manual_dice : public protocol_base
	{
		using manual_dice_call = std::function<std::string(std::string &, std::string &, int64_t, int64_t)>;
	private:
		std::regex * regex_detail_command;
		std::regex * regex_filter_integer_space;
		std::map<std::string, manual_dice_call> * method_map;
	public:
		protocol_manual_dice();
		~protocol_manual_dice();
		std::string resolve_request(
			std::string &message,
			const int64_t group_id,
			const int64_t user_qq_id,
			bool isfromGroup) override;
	};

	
	#define TMDICE_CALL(_Func_Name) std::string _Func_Name(std::string & message,std::string & nick_name,int64_t group_id,int64_t user_id)

	TMDICE_CALL(manualdice_add);
	TMDICE_CALL(manualdice_killall);
	TMDICE_CALL(manualdice_kill);
	TMDICE_CALL(manualdice_roll);
	TMDICE_CALL(manualdice_create);
}
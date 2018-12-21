#pragma once

#include "./common.h"

namespace dicebot{
	class manual_dice;

	typedef std::shared_ptr<manual_dice> p_manual;
	typedef std::pair<int64_t, int64_t> manual_kpair;
	typedef std::map<manual_kpair, p_manual> manual_map;
	typedef std::pair<manual_kpair, p_manual> manual_pair;

	class manual_dice_control:public std::map<manual_kpair, p_manual>
	{
	private:
		//manual_map * map;
		bool is_no_sql_mode;
	public:
		static manual_dice_control * instance;
		manual_dice_control();
		~manual_dice_control();
		p_manual create_manual_dice(const int64_t user_id, const int64_t group_id, const std::string & command);
		p_manual roll_manual_dice(const int64_t user_id, const int64_t group_id, const std::string & command);
		p_manual kill_manual_dice(const int64_t user_id, const int64_t group_id, const std::string & command);
		p_manual add_manual_dice(const int64_t user_id, const int64_t group_id, const std::string & command);
		p_manual killall_manual_dice(const int64_t user_id, const int64_t group_id);
		static int sqlite3_callback_query_manualdice(void * data, int argc, char ** argv, char ** azColName);
		bool update_database(manual_kpair manual_dice_key,p_manual manual_dice_target);
		bool insert_database(manual_kpair manual_dice_key,p_manual manual_dice_target);
		bool read_database(manual_kpair manual_dice_key,p_manual manual_dice_target);
		bool exist_database(manual_kpair manual_dice_key);
	};
}

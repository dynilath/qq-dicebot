#pragma once

#include "./common.h"

namespace dicebot{
	class nickname_manager
	{
	public:
		nickname_manager() noexcept;
		~nickname_manager();
		static nickname_manager * instance;
		void get_nickname(const int64_t fromGroupOrDiscuss, const int64_t fromQQ, std::string &nickname, bool isfromGroup);
		void set_nickname(const int64_t fromGroupOrDiscuss, const int64_t fromQQ, const std::string &nickname, bool isfromGroup);
	private:
		std::map<std::pair<int64_t, int64_t>, std::string> * map_nickname_cache;
		bool is_no_sql_mode = false;
		static int sqlite3_callback_query_name(void * data, int argc, char ** argv, char ** azColName);
		bool get_default_name(const int64_t fromGroupOrDiscuss, const int64_t fromQQ, std::string & nickname,bool isfromGroup);
	};
}

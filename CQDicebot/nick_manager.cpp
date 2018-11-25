#include "stdafx.h"
#include "nick_manager.h"
#include "database_manager.h"
#include "base64.h"
#include "QTool.h"
#include <iostream>


#define NICK_TABLE_NAME "nickname"
#define NICK_TABLE_DEFINE "create table " NICK_TABLE_NAME \
				"(qqid      int     NOT NULL," \
				"groupid    int     NOT NULL," \
				"name       text    NOT NULL," \
				"primary    key    (QQID,GROUPID));"

nickname_manager * nickname_manager::instance = nullptr;

nickname_manager::nickname_manager() noexcept
{
	database_manager * databaseControl = database_manager::get_instance();
	int i_ret_code = databaseControl->register_table(NICK_TABLE_NAME, NICK_TABLE_DEFINE);
	is_no_sql_mode = i_ret_code != SQLITE_OK;
	map_nickname_cache = new std::map<std::pair<int64_t, int64_t>, std::string>();
	if (nickname_manager::instance != nullptr) {
		delete(nickname_manager::instance);
	}
	else nickname_manager::instance = this;
}


nickname_manager::~nickname_manager()
{
	delete(this->map_nickname_cache);
	if (nickname_manager::instance == this) {
		nickname_manager::instance = nullptr;
	}
}

void nickname_manager::get_nickname(const int i_AuthCode, const int64_t fromGroupOrDiscuss, const int64_t fromQQ, std::string & nickname,bool isfromGroup)
{
	sqlite3 * database = database_manager::get_database();
	std::map<std::pair<int64_t, int64_t>, std::string>::iterator iter_map_nick =  map_nickname_cache->find(std::pair<int64_t, int64_t>(fromQQ, fromGroupOrDiscuss));
	if (iter_map_nick != map_nickname_cache->end()) {
		nickname = (*iter_map_nick).second;
	}
	else {
		if (is_no_sql_mode) {
			if (CQTool::getDefaultName(i_AuthCode, fromGroupOrDiscuss, fromQQ, nickname, isfromGroup)) {
				map_nickname_cache->insert(std::pair<std::pair<int64_t, int64_t>, std::string>(std::pair<int64_t, int64_t>(fromQQ, fromGroupOrDiscuss), nickname));
			}
		}
		else {
			std::ostringstream ostrs_sql_command(std::ostringstream::ate);
			ostrs_sql_command << "SELECT * FROM " NICK_TABLE_NAME " where qqid =" << fromQQ << " and groupid =" << fromGroupOrDiscuss;
			std::string str_nick_endcoded;
			char * pchar_err_message = nullptr;
			int ret_code = sqlite3_exec(database, ostrs_sql_command.str().c_str(), &sqlite3_callback_query_name, (void*)&str_nick_endcoded, &pchar_err_message);
			if (ret_code == SQLITE_OK) {
				if (str_nick_endcoded.length() > 0) {
					Base64::Decode(str_nick_endcoded, &nickname);
					map_nickname_cache->insert(std::pair<std::pair<int64_t, int64_t>, std::string>(std::pair<int64_t, int64_t>(fromQQ, fromGroupOrDiscuss), nickname));
				}
				else {
					CQTool::getDefaultName(i_AuthCode, fromGroupOrDiscuss, fromQQ, nickname, isfromGroup);
					set_nickname(i_AuthCode, fromGroupOrDiscuss, fromQQ, nickname, isfromGroup);
				}
			}
			else {
				is_no_sql_mode = true;
#ifdef _DEBUG
				std::cout << sqlite3_errmsg(database);
#endif
			}
		}
	}
}

void nickname_manager::set_nickname(const int i_AuthCode, const int64_t fromGroupOrDiscuss, const int64_t fromQQ, const std::string & nickname,bool isfromGroup)
{
	sqlite3 * database = database_manager::get_database();
	std::map<std::pair<int64_t, int64_t>, std::string>::iterator iter_map_nick = map_nickname_cache->find(std::pair<int64_t, int64_t>(fromQQ, fromGroupOrDiscuss));
	if (iter_map_nick != map_nickname_cache->end()) {
		(*iter_map_nick).second = nickname;
	}
	else {
		map_nickname_cache->insert(std::pair<std::pair<int64_t, int64_t>, std::string>(std::pair<int64_t, int64_t>(fromQQ, fromGroupOrDiscuss), nickname));
	}

	if (!is_no_sql_mode) {
		std::string str_encoded_nickname;
		Base64::Encode(nickname, &str_encoded_nickname);
		std::ostringstream ostrs_sql_command(std::ostringstream::ate);
		ostrs_sql_command<<"SELECT * FROM " NICK_TABLE_NAME " where qqid =" << fromQQ << " and groupid =" << fromGroupOrDiscuss;
		std::string str_nick_endcoded;
		char * pchar_err_message = nullptr;
		int ret_code = sqlite3_exec(database, ostrs_sql_command.str().c_str(), &sqlite3_callback_query_name, (void*)&str_nick_endcoded, &pchar_err_message);
		if (ret_code == SQLITE_OK) {
			if (str_nick_endcoded.length() > 0) {
				std::ostringstream ostrs_sql_command_2(std::ostringstream::ate);
				ostrs_sql_command_2.str("update " NICK_TABLE_NAME " set ");
				ostrs_sql_command_2 << " name ='" << str_encoded_nickname << "'";
				ostrs_sql_command_2 << " where qqid =" << fromQQ << " and groupid =" << fromGroupOrDiscuss;
				int ret_code_2 = sqlite3_exec(database, ostrs_sql_command_2.str().c_str(), &database_manager::sqlite3_callback, (void*)&i_data_database_update, &pchar_err_message);
#ifdef _DEBUG
				if (ret_code_2 != SQLITE_OK) {
					std::cout << sqlite3_errmsg(database);
				}
#endif
			}
			else {
				std::ostringstream ostrs_sql_command_2(std::ostringstream::ate);
				ostrs_sql_command_2.str("insert into " NICK_TABLE_NAME " values ( ");
				ostrs_sql_command_2 << fromQQ << ", " << fromGroupOrDiscuss << ", '" << str_encoded_nickname << "'" << ");";
				int ret_code_2 = sqlite3_exec(database, ostrs_sql_command_2.str().c_str(), &database_manager::sqlite3_callback, (void*)&i_data_database_update, &pchar_err_message);
#ifdef _DEBUG
				if (ret_code_2 != SQLITE_OK) {
					std::cout << sqlite3_errmsg(database);
				}
#endif
			}
		}
		else is_no_sql_mode = true;
	}
}

int nickname_manager::sqlite3_callback_query_name(void * data, int argc, char ** argv, char ** azColName)
{
	if (argc == 3) {
		std::string * pstr_ret = (std::string *) data;
		*pstr_ret = std::string(argv[2]);
	}
	return SQLITE_OK;
}
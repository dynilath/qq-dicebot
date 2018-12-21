#include "./database_manager.h"

#include "./nick_manager.h"

#include "../cqsdk/cqsdk.h"

#include "sqlite3.h"



namespace base64 = cq::utils::base64;
namespace cqapi = cq::api;

namespace dicebot{
	#define NICK_TABLE_NAME "nickname"
	#define NICK_TABLE_DEFINE "create table " NICK_TABLE_NAME \
					"(qqid      int     NOT NULL," \
					"groupid    int     NOT NULL," \
					"name       text    NOT NULL," \
					"primary    key    (QQID,GROUPID));"

	nickname_manager * nickname_manager::instance = nullptr;

	nickname_manager::nickname_manager() noexcept
	{
		database::database_manager * databaseControl = database::database_manager::get_instance();
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

	void nickname_manager::get_nickname(const int64_t fromGroupOrDiscuss, const int64_t fromQQ, std::string & nickname,bool is_from_group)
	{
		sqlite3 * database = database::database_manager::get_database();
		std::map<std::pair<int64_t, int64_t>, std::string>::iterator iter_map_nick =  map_nickname_cache->find(std::pair<int64_t, int64_t>(fromQQ, fromGroupOrDiscuss));
		if (iter_map_nick != map_nickname_cache->end()) {
			nickname = (*iter_map_nick).second;
		}
		else {
			if (is_no_sql_mode) {
				if (this->get_default_name(fromGroupOrDiscuss, fromQQ, nickname, is_from_group)) {
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
						nickname = base64::decode(str_nick_endcoded);
						map_nickname_cache->insert(std::pair<std::pair<int64_t, int64_t>, std::string>(std::pair<int64_t, int64_t>(fromQQ, fromGroupOrDiscuss), nickname));
					}
					else {
						this->get_default_name(fromGroupOrDiscuss, fromQQ, nickname, is_from_group);
						set_nickname(fromGroupOrDiscuss, fromQQ, nickname, is_from_group);
					}
				}
				else {
					is_no_sql_mode = true;
	#ifdef _DEBUG
					logger::log("dicebot nick_manager",std::string(sqlite3_errmsg(database)));
	#endif
				}
			}
		}
	}

	void nickname_manager::set_nickname(const int64_t fromGroupOrDiscuss, const int64_t fromQQ, const std::string & nickname,bool is_from_group)
	{
		sqlite3 * database = database::database_manager::get_database();
		std::map<std::pair<int64_t, int64_t>, std::string>::iterator iter_map_nick = map_nickname_cache->find(std::pair<int64_t, int64_t>(fromQQ, fromGroupOrDiscuss));
		if (iter_map_nick != map_nickname_cache->end()) {
			(*iter_map_nick).second = nickname;
		}
		else {
			map_nickname_cache->insert(std::pair<std::pair<int64_t, int64_t>, std::string>(std::pair<int64_t, int64_t>(fromQQ, fromGroupOrDiscuss), nickname));
		}

		if (!is_no_sql_mode) {
			std::string str_encoded_nickname;
			str_encoded_nickname = base64::encode((const unsigned char*)(nickname.c_str()), nickname.length());
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
					int ret_code_2 = sqlite3_exec(database, ostrs_sql_command_2.str().c_str(), &database::database_manager::sqlite3_callback, (void*)&i_data_database_update, &pchar_err_message);
	#ifdef _DEBUG
					if (ret_code_2 != SQLITE_OK) {
						logger::log("dicebot nick_manager",std::string(sqlite3_errmsg(database)));
					}
	#endif
				}
				else {
					std::ostringstream ostrs_sql_command_2(std::ostringstream::ate);
					ostrs_sql_command_2.str("insert into " NICK_TABLE_NAME " values ( ");
					ostrs_sql_command_2 << fromQQ << ", " << fromGroupOrDiscuss << ", '" << str_encoded_nickname << "'" << ");";
					int ret_code_2 = sqlite3_exec(database, ostrs_sql_command_2.str().c_str(), &database::database_manager::sqlite3_callback, (void*)&i_data_database_update, &pchar_err_message);
	#ifdef _DEBUG
					if (ret_code_2 != SQLITE_OK) {
						logger::log("dicebot nick_manager",std::string(sqlite3_errmsg(database)));
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

	bool nickname_manager::get_default_name(const int64_t fromGroupOrDiscuss, const int64_t fromQQ, std::string & nickname,bool is_from_group){
		if(is_from_group){
			cq::GroupMember member = cqapi::get_group_member_info(fromGroupOrDiscuss,fromQQ);
			nickname.assign(member.nickname);
			return true;
		}
		else{
			cq::User user = cqapi::get_stranger_info(fromQQ);
			nickname.assign(user.nickname);
			return true;
		}
		return false;
	}
}
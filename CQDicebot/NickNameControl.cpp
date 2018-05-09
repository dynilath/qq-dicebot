#include "stdafx.h"
#include "NickNameControl.h"
#include "base64.h"
#include "QTool.h"
#include <iostream>

const char * cstr_file_db = APP_DIR "com.dynilath.coolqdicebot.nickname.db";

CQTool CQT_instance_B;

//const int i_data_database_quarry = 1;
const int i_data_database_insert = 2;
const int i_data_database_create = 4;
const int i_data_database_update = 8;

NickNameControl * NickNameControl::instance = nullptr;

NickNameControl::NickNameControl()
{
	int i_ret_code = sqlite3_open(cstr_file_db, &database);
	if (i_ret_code == SQLITE_OK) {
		if (!is_my_table_exist(NICK_TABLE_NAME)) {
			char * sql_command = 
				"create table " NICK_TABLE_NAME \
				"(qqid      int     NOT NULL," \
				"groupid    int     NOT NULL," \
				"name       text    NOT NULL," \
				"primary    key    (QQID,GROUPID));";
			char * pchar_err_message = nullptr;
			int i_ret_code_2 = sqlite3_exec(database, sql_command, &sqlite3_callback, (void*)&i_data_database_create, &pchar_err_message);
			if (i_ret_code != SQLITE_OK) 
			{
				std::cout << pchar_err_message;
				is_no_sql_mode = true; 
			}
			else is_no_sql_mode = false;
		}
	}
	NickNameControl::instance = this;
}


NickNameControl::~NickNameControl()
{
	NickNameControl::instance = nullptr;
	sqlite3_close(database);
}

void NickNameControl::getNickName(const int i_AuthCode, const int64_t fromGroup, const int64_t fromQQ, std::string & nickname)
{
	std::map<std::pair<int64_t, int64_t>, std::string>::iterator iter_map_nick =  map_nickname_cache.find(std::pair<int64_t, int64_t>(fromQQ, fromGroup));
	if (iter_map_nick != map_nickname_cache.end()) {
		nickname = (*iter_map_nick).second;
	}
	else {
		if (is_no_sql_mode) {
			CQ_Type_GroupMember groupMember;
			if (CQT_instance_B.GetGroupMemberInfo(i_AuthCode, fromGroup, fromQQ, groupMember)) {
				if (groupMember.nick.length() == 0) nickname = std::string(groupMember.username);
				else nickname = std::string(groupMember.nick);
				map_nickname_cache.insert(std::pair<std::pair<int64_t, int64_t>, std::string>(std::pair<int64_t, int64_t>(fromQQ, fromGroup), nickname));
			}
		}
		else {
			std::ostringstream ostrs_sql_command(std::ostringstream::ate);
			ostrs_sql_command << "SELECT * FROM " NICK_TABLE_NAME " where qqid =" << fromQQ << " and groupid =" << fromGroup;
			std::string str_nick_endcoded;
			char * pchar_err_message = nullptr;
			int ret_code = sqlite3_exec(database, ostrs_sql_command.str().c_str(), &sqlite3_callback_quarry_name, (void*)&str_nick_endcoded, &pchar_err_message);
			if (ret_code == SQLITE_OK) {
				if (str_nick_endcoded.length() > 0) {
					Base64::Decode(str_nick_endcoded, &nickname);
					map_nickname_cache.insert(std::pair<std::pair<int64_t, int64_t>, std::string>(std::pair<int64_t, int64_t>(fromQQ, fromGroup), nickname));
				}
				else {
					CQ_Type_GroupMember groupMember;
					if (CQT_instance_B.GetGroupMemberInfo(i_AuthCode, fromGroup, fromQQ, groupMember)) {
						if (groupMember.nick.length() == 0) nickname = std::string(groupMember.username);
						else nickname = std::string(groupMember.nick);
					}
					setNickName(i_AuthCode, fromGroup, fromQQ, nickname);
					map_nickname_cache.insert(std::pair<std::pair<int64_t, int64_t>, std::string>(std::pair<int64_t, int64_t>(fromQQ, fromGroup), nickname));
				}
			}
			else is_no_sql_mode = true;
		}
	}
}

void NickNameControl::setNickName(const int i_AuthCode, const int64_t fromGroup, const int64_t fromQQ, const std::string & nickname)
{
	std::map<std::pair<int64_t, int64_t>, std::string>::iterator iter_map_nick = map_nickname_cache.find(std::pair<int64_t, int64_t>(fromQQ, fromGroup));
	if (iter_map_nick != map_nickname_cache.end()) {
		(*iter_map_nick).second = nickname;
	}
	else {
		map_nickname_cache.insert(std::pair<std::pair<int64_t, int64_t>, std::string>(std::pair<int64_t, int64_t>(fromQQ, fromGroup), nickname));
	}

	if (!is_no_sql_mode) {
		std::string str_encoded_nickname;
		Base64::Encode(nickname, &str_encoded_nickname);
		std::ostringstream ostrs_sql_command(std::ostringstream::ate);
		ostrs_sql_command<<"SELECT * FROM " NICK_TABLE_NAME " where qqid =" << fromQQ << " and groupid =" << fromGroup;
		std::string str_nick_endcoded;
		char * pchar_err_message = nullptr;
		int ret_code = sqlite3_exec(database, ostrs_sql_command.str().c_str(), &sqlite3_callback_quarry_name, (void*)&str_nick_endcoded, &pchar_err_message);
		if (ret_code == SQLITE_OK) {
			if (str_nick_endcoded.length() > 0) {
				std::ostringstream ostrs_sql_command_2(std::ostringstream::ate);
				ostrs_sql_command_2.str("UPDATE " NICK_TABLE_NAME " set ");
				ostrs_sql_command_2 << " name ='" << str_encoded_nickname << "'";
				ostrs_sql_command_2 << " where qqid =" << fromQQ << " and groupid =" << fromGroup;
				int ret_code_2 = sqlite3_exec(database, ostrs_sql_command_2.str().c_str(), &sqlite3_callback, (void*)&i_data_database_update, &pchar_err_message);
			}
			else {
				std::ostringstream ostrs_sql_command_2(std::ostringstream::ate);
				ostrs_sql_command_2.str("INSERT " NICK_TABLE_NAME " VALUES ( ");
				ostrs_sql_command_2 << fromQQ << ", " << fromGroup << ", '" << str_encoded_nickname << "'";
				ostrs_sql_command_2 << ");";
				int ret_code_2 = sqlite3_exec(database, ostrs_sql_command_2.str().c_str(), &sqlite3_callback, (void*)&i_data_database_update, &pchar_err_message);

			}
		}
		else is_no_sql_mode = true;
	}
}

bool NickNameControl::is_my_table_exist(const std::string & table_name)
{
	std::string sql_command = "SELECT COUNT(*) FROM sqlite_master where type ='table' and name ='" + table_name + "'";
	char * pchar_err_message = nullptr;
	int i_count_of_table = 0;
	int ret_code = sqlite3_exec(database, sql_command.c_str(), &sqlite3_callback, (void*)&i_count_of_table, &pchar_err_message);
	if (ret_code == SQLITE_OK) {
		return i_count_of_table > 0;
	}
	return false;
}

int NickNameControl::sqlite3_callback(void * data, int argc, char ** argv, char ** azColName)
{
	int i_database_op = *((int*)data);
	switch (i_database_op)
	{
	case i_data_database_create:
		return SQLITE_OK;
	case i_data_database_update:
		return SQLITE_OK;
	default:
		return SQLITE_OK;
	}
}

int NickNameControl::sqlite3_callback_check_table(void * data, int argc, char ** argv, char ** azColName)
{
	int * i_handle = (int*)data;
	if (argc == 1) {
		*i_handle = atoi(argv[0]);
	}
	return SQLITE_OK;
}

int NickNameControl::sqlite3_callback_quarry_name(void * data, int argc, char ** argv, char ** azColName)
{
	if (argc == 3) {
		std::string * pstr_ret = (std::string *) data;
		*pstr_ret = std::string(argv[2]);
	}
	return SQLITE_OK;
}

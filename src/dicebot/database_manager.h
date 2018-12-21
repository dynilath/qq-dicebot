#pragma once

#include "./common.h"
#include "sqlite3.h"

const int i_data_database_create = 4;
const int i_data_database_update = 8;
namespace dicebot::database{
	class database_manager
	{
	private:
		static database_manager * instance;
		bool isDatabaseReady;
		sqlite3 * database;
	public:
		static sqlite3 * get_database();
		static database_manager * get_instance(std::string str_app_dir);
		static database_manager * get_instance();
		database_manager(std::string str_app_dir);
		~database_manager();
		int register_table(std::string str_table_name, std::string str_table_sql);
		int is_table_exist(const std::string & table_name, bool &isExist);
		static int sqlite3_callback(void * data, int argc, char ** argv, char ** azColName);
		static int sqlite3_callback_is_table_exist(void * data, int argc, char ** argv, char ** azColName);
	};
}

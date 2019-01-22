#pragma once

#include "./common.h"
#include "sqlite3.h"

namespace dicebot::database{

    enum class operation{
        create,
        update
    };

    const operation oper_create = operation::create;
    const operation oper_update = operation::update;

    class database_manager{
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
    };

    int sqlite3_callback_general(void * data, int argc, char ** argv, char ** azColName);
    int sqlite3_callback_exist(void * data, int argc, char ** argv, char ** azColName);
}

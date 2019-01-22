#include "./database_manager.h"

#include "sqlite3.h"

using namespace dicebot;
using namespace dicebot::database;

const char * cstr_file_db = "com.dynilath.coolqdicebot.nickname.db";

database_manager * database_manager::instance = nullptr;

sqlite3 * database_manager::get_database(){
    return (database_manager::instance)->database;
}

database_manager * database_manager::get_instance(std::string str_app_dir){
    if(database_manager::instance == nullptr) 
        database_manager::instance = new database_manager(str_app_dir);
    return database_manager::instance;
}

database_manager * database_manager::get_instance(){
    return database_manager::instance;
}

database_manager::database_manager(std::string str_app_dir){
    std::string str_db_path = str_app_dir + cstr_file_db;
    isDatabaseReady = false;
    int i_ret_code = sqlite3_open(str_db_path.c_str(), &database);
    if(i_ret_code == SQLITE_OK){
        if(database_manager::instance != nullptr){
            delete(database_manager::instance);
            database_manager::instance = this;
        }
        database_manager::instance = this;
        isDatabaseReady = true;
    }
}

database_manager::~database_manager(){
    if(database_manager::instance == this) database_manager::instance = nullptr;
    sqlite3_close(this->database);
}

int database_manager::register_table(std::string str_table_name, std::string str_table_sql){
    bool isExist;
    int i_ret_code = is_table_exist(str_table_name, isExist);
    if(i_ret_code == SQLITE_OK && !isExist){
        const char * sql_command = str_table_sql.c_str();
        char * pchar_err_message = nullptr;
        int i_ret_code_2 = sqlite3_exec(this->database, sql_command, &sqlite3_callback_general, (void*)&database::oper_create, &pchar_err_message);
        if(i_ret_code_2 != SQLITE_OK)
        {
            #ifdef _DEBUG
            logger::log("dicebot db_manager",std::string(sqlite3_errmsg(database)));
            #endif // _DEBUG
            isDatabaseReady = true;
            return i_ret_code_2;
        }
        else{
            isDatabaseReady = false;
            return i_ret_code_2;
        }
    }
    else return i_ret_code;
}

inline int database_manager::is_table_exist(const std::string & table_name , bool & isExist){
    std::string sql_command = "select count(*) from sqlite_master where type ='table' and name ='" + table_name + "'";
    char * pchar_err_message = nullptr;
    int i_count_of_table = 0;
    int ret_code = sqlite3_exec(this->database, sql_command.c_str(), &sqlite3_callback_exist, (void*)&i_count_of_table, &pchar_err_message);
    if(ret_code == SQLITE_OK){
        isExist = i_count_of_table > 0;
        return ret_code;
    }
    #ifdef _DEBUG
    else{
        logger::log("dicebot db_manager",std::string(sqlite3_errmsg(database)));
    }
    #endif
    isExist = false;
    return ret_code;
}

int database::sqlite3_callback_general(void * data, int argc, char ** argv, char ** azColName){
    operation i_database_op = *((operation*)data);
    switch (i_database_op)
    {
    case operation::create:
        return SQLITE_OK;
    case operation::update:
        return SQLITE_OK;
    default:
        return SQLITE_ABORT;
    }
}

int database::sqlite3_callback_exist(void * data, int argc, char ** argv, char ** azColName){
    int * i_handle = (int*)data;
    if(argc == 1){
        *i_handle = atoi(argv[0]);
    }
    return SQLITE_OK;
}
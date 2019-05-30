#include "./database_manager.h"

#ifdef _DEBUG
#include "../utils/logger.h"
#endif

using namespace dicebot;
using namespace dicebot::database;

constexpr char cstr_file_db[] = "com.dynilath.coolqdicebot.db";

std::unique_ptr<database_manager> database_manager::instance;

bool database::is_no_sql_mode = false;

sqlite3* database_manager::get_database() { return instance->database; }

database_manager* database_manager::create_instance(const char* str_app_dir) noexcept {
    instance = std::make_unique<database_manager>(str_app_dir);
    return instance.get();
}

void database_manager::destroy_instance() noexcept {
    sqlite3_close(database_manager::instance->database);
    database_manager::instance = nullptr;
}
database_manager* database_manager::get_instance() noexcept { return instance.get(); }

database_manager::database_manager(const char* str_app_dir) {
    std::string str_db_path = str_app_dir;
    str_db_path.append(cstr_file_db);
    is_ready = false;
    int i_ret_code = sqlite3_open(str_db_path.c_str(), &database);
    if (i_ret_code != SQLITE_OK) {
        is_no_sql_mode = true;
    }
}

int database_manager::register_table(const char* str_table_name, const char* str_table_sql) {
    if (is_no_sql_mode) return SQLITE_ABORT;
    bool is_exist;
    int i_ret_code = is_table_exist(str_table_name, is_exist);
    if (i_ret_code == SQLITE_OK && !is_exist) {
        char* pchar_err_message = nullptr;
        int i_ret_code_2 = sqlite3_exec_noquery(this->database, str_table_sql);
        if (i_ret_code_2 != SQLITE_OK) {
#ifdef _DEBUG
            logger::log("dicebot db_manager", std::string(sqlite3_errmsg(database)));
#endif // _DEBUG
            is_ready = true;
            return i_ret_code_2;
        } else {
            is_ready = false;
            return i_ret_code_2;
        }
    } else
        return i_ret_code;
}

inline int database_manager::is_table_exist(const char* table_name, bool& is_exist) {
    is_exist = false;
    if (is_no_sql_mode) return SQLITE_ABORT;
    std::string sql_command = "select count(*) from sqlite_master where type ='table' and name ='";
    sql_command.append(table_name).append("'");
    char* pchar_err_message = nullptr;
    int i_count_of_table = 0;
    int ret_code = sqlite3_exec(this->database,
                                sql_command.c_str(),
                                [](void* data, int argc, char** argv, char** azColName) -> int {
                                    int* i_handle = (int*)data;
                                    if (argc == 1) {
                                        *i_handle = atoi(argv[0]);
                                    }
                                    return SQLITE_OK;
                                },
                                (void*)&i_count_of_table,
                                &pchar_err_message);
    if (ret_code == SQLITE_OK) {
        is_exist = i_count_of_table > 0;
        return ret_code;
    }
#ifdef _DEBUG
    else {
        logger::log("dicebot db_manager", std::string(sqlite3_errmsg(database)));
    }
#endif
    is_exist = false;
    return ret_code;
}

bool database_manager::exec_noquery(const char* sql) const noexcept {
    if (is_no_sql_mode) return false;
    char* pchar_err_message;
    int ret_code = sqlite3_exec(this->database,
                                sql,
                                [](void* data, int argc, char** argv, char** azColName) -> int { return SQLITE_OK; },
                                nullptr,
                                &pchar_err_message);
    if (ret_code != SQLITE_OK) {
#ifdef _DEBUG
        std::string err_msg = "sqlite3 errmsg: ";
        err_msg.append(sqlite3_errmsg(database));
        err_msg.append("\nreturned err msg from exec: ");
        err_msg.append(pchar_err_message);
        logger::log("exec_noquery", std::string());
#endif
        is_no_sql_mode = true;
        return false;
    } else
        return true;
}

bool database_manager::exec(const char* sql, void* data, int (*callback)(void*, int, char**, char**)) const noexcept {
    if (is_no_sql_mode) return false;
    char* pchar_err_message;
    int ret_code = sqlite3_exec(this->database, sql, callback, data, &pchar_err_message);
    if (ret_code != SQLITE_OK) {
#ifdef _DEBUG
        std::string err_msg = "sqlite3 errmsg: ";
        err_msg.append(sqlite3_errmsg(database));
        err_msg.append("\nreturned err msg from exec: ");
        err_msg.append(pchar_err_message);
        logger::log("exec_noquery", std::string());
#endif
        is_no_sql_mode = true;
        return false;
    } else
        return true;
}

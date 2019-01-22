#include "./profile_manager.h"

#include "sqlite3.h"

#include "./profile.h"
#include "./database_manager.h"

using namespace dicebot;
using namespace dicebot::profile;

#define PROFILE_TABLE_NAME "user_profile"
#define PROFILE_TABLE_DEFINE "create table " PROFILE_TABLE_NAME \
                "(qqid             int     NOT NULL," \
                "system_variables  text," \
                "user_variables    text," \
                "default_roll      text," \
                "macro_roll        text," \
                "primary    key    (qqid));"

using profile_pair = std::pair<int64_t,user_profile>;

profile_manager * profile_manager::instance = nullptr;

profile_manager::profile_manager() noexcept{
    database::database_manager * databaseControl = database::database_manager::get_instance();
    int i_ret_code = databaseControl->register_table(PROFILE_TABLE_NAME, PROFILE_TABLE_DEFINE);
    is_no_sql_mode = i_ret_code != SQLITE_OK;

    if(profile_manager::instance != nullptr){
        delete(profile_manager::instance);
    }
    
    profile_manager::instance = this;
}

profile_manager::~profile_manager(){
    if(profile_manager::instance == this){
        profile_manager::instance = nullptr;
    }
}

bool profile::profile_db::read_database(user_profile & profile, int64_t const user_id){
    sqlite3 * database = database::database_manager::get_database();
    ostrs ostrs_sql_command(ostrs::ate);
    ostrs_sql_command << "SELECT system_variables, user_variables, default_roll, macro_roll FROM " PROFILE_TABLE_NAME " where qqid =" << user_id;
    char * pchar_err_message = nullptr;
    int ret_code = sqlite3_exec(database, ostrs_sql_command.str().c_str(), &sqlite3_callback_query_profile, (void*)(&profile), &pchar_err_message);
    if(ret_code == SQLITE_OK) return true;
    else{
        #ifdef _DEBUG
        logger::log("dicebot read_database",std::string(sqlite3_errmsg(database)));
        #endif
        is_no_sql_mode = true;
        return false;
    }
}

bool profile::profile_db::write_database(user_profile const & profile, int64_t const user_id){
    bool existed = exist_database(user_id);
    if(is_no_sql_mode) return false;
    else if(existed){
        return update_database(profile,user_id);
    }
    else{
        return insert_database(profile,user_id);
    }
}

bool profile::profile_db::exist_database(int64_t const user_id){
    sqlite3 * database = database::database_manager::get_database();
    ostrs ostrs_sql_command(ostrs::ate);
    ostrs_sql_command << "SELECT count(*) FROM " PROFILE_TABLE_NAME " where qqid =" << user_id;
    std::string str_manualdice_read;
    char * pchar_err_message = nullptr;

    int count = 0;

    int ret_code = sqlite3_exec(database, ostrs_sql_command.str().c_str(), &database::sqlite3_callback_exist, (void*)&count, &pchar_err_message);
    
    if(ret_code != SQLITE_OK){
        #ifdef _DEBUG
        logger::log("dicebot exist_database",std::string(sqlite3_errmsg(database)));
        #endif
        is_no_sql_mode = true;
    }
    return count > 0;
}

bool profile::profile_db::insert_database(user_profile const & profile, int64_t const user_id){
    sqlite3 * database = database::database_manager::get_database();
    char * pchar_err_message = nullptr;

    std::vector<std::string> vecs;
    profile.encode(vecs);

    ostrs ostrs_sql_command(ostrs::ate);
    ostrs_sql_command.str("insert into " PROFILE_TABLE_NAME " values ( ");
    ostrs_sql_command << user_id;
    ostrs_sql_command << ", '" << vecs[0] << "'";
    ostrs_sql_command << ", '" << vecs[1] << "'";
    ostrs_sql_command << ", '" << vecs[2] << "'";
    ostrs_sql_command << ", '" << vecs[3] << "'";
    ostrs_sql_command << ");";

    int ret_code = sqlite3_exec(database, ostrs_sql_command.str().c_str(), &database::sqlite3_callback_general, (void*)&database::oper_update, &pchar_err_message);
    
    if(ret_code != SQLITE_OK){
        #ifdef _DEBUG
        logger::log("dicebot insert_database",std::string(sqlite3_errmsg(database)));
        #endif
        is_no_sql_mode = true;
    }
    return ret_code == SQLITE_OK;
}

bool profile::profile_db::update_database(user_profile const & profile, int64_t const user_id){
    sqlite3 * database = database::database_manager::get_database();
    char * pchar_err_message = nullptr;

    std::vector<std::string> vecs;
    profile.encode(vecs);

    ostrs ostrs_sql_command(ostrs::ate);
    ostrs_sql_command.str("update " PROFILE_TABLE_NAME " set ");
    ostrs_sql_command << " system_variables ='" << vecs[0] << "'";
    ostrs_sql_command << ", user_variables ='" << vecs[1] << "'";
    ostrs_sql_command << ", default_roll ='" << vecs[2] << "'";
    ostrs_sql_command << ", macro_roll ='" << vecs[3] << "'";
    ostrs_sql_command << " where qqid = " << user_id;

    int ret_code = sqlite3_exec(database, ostrs_sql_command.str().c_str(), &database::sqlite3_callback_general, (void*)&database::oper_update, &pchar_err_message);
    
    if(ret_code != SQLITE_OK){
        #ifdef _DEBUG
        logger::log("dicebot insert_database",std::string(sqlite3_errmsg(database)));
        #endif
        is_no_sql_mode = true;
    }
    return ret_code == SQLITE_OK;
}

int profile::profile_db::sqlite3_callback_query_profile(void * data, int argc, char ** argv, char ** azColName){
    user_profile * profile = (user_profile *) data;
    if(argc != 4) return SQLITE_ABORT;
    std::vector<std::string> codes;
    codes.push_back(std::string(argv[0]));
    codes.push_back(std::string(argv[1]));
    codes.push_back(std::string(argv[2]));
    codes.push_back(std::string(argv[3]));
    if(profile->decode(codes)) return SQLITE_OK;
    else return SQLITE_ABORT;
}
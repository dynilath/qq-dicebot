#include "./database_manager.h"

#include "./nick_manager.h"

#ifndef NO_COOLQ
#include "../cqsdk/cqsdk.h"
#else
#include "../cqsdk/utils/base64.h"
#endif

#include "sqlite3.h"


namespace base64 = cq::utils::base64;
#ifndef NO_COOLQ
namespace cqapi = cq::api;
#endif

namespace dicebot::nickname{
    #define NICK_TABLE_NAME "nickname"
    #define NICK_TABLE_DEFINE "create table " NICK_TABLE_NAME \
                    "(qqid      int     NOT NULL," \
                    "groupid    int     NOT NULL," \
                    "name       text    NOT NULL," \
                    "primary    key    (QQID,GROUPID));"

    nickname_manager * nickname_manager::instance = nullptr;

    nickname_manager::nickname_manager() noexcept{
        database::database_manager * databaseControl = database::database_manager::get_instance();
        int i_ret_code = databaseControl->register_table(NICK_TABLE_NAME, NICK_TABLE_DEFINE);
        is_no_sql_mode = i_ret_code != SQLITE_OK;
        if(nickname_manager::instance != nullptr){
            delete(nickname_manager::instance);
        }
        else nickname_manager::instance = this;
    }


    nickname_manager::~nickname_manager(){
        if(nickname_manager::instance == this){
            nickname_manager::instance = nullptr;
        }
    }

    bool nickname_manager::get_nickname(event_info & event){
        auto iter = nick_map.find(event.pair());
        if(iter != nick_map.end()){
            event.nickname = iter->second;
            return true;
        }
        else{
            if(exist_database(event)){
                return read_database(event);
            }
            return false;
        }
    }

    void nickname_manager::set_nickname(event_info const & event){
        auto iter = nick_map.find(event.pair());
        if(iter != nick_map.end()){
            iter->second = event.nickname;
            write_database(event);
        }
        else{
            auto t = nick_map.insert(nick_pair_t(event.pair(),event.nickname));
            if(t.second) write_database(event);
        }
    }

    bool read_database(event_info & event){
        sqlite3 * database = database::database_manager::get_database();
        ostrs ostrs_sql_command(ostrs::ate);
        ostrs_sql_command << "SELECT name FROM " NICK_TABLE_NAME;
        ostrs_sql_command << " where qqid =" << event.user_id;
        ostrs_sql_command << " and groupid =" << event.group_id;
        std::string str_nick_endcoded;
        char * pchar_err_message = nullptr;
        int ret_code = sqlite3_exec(database, ostrs_sql_command.str().c_str(), &sqlite3_callback_query_name, (void*)&str_nick_endcoded, &pchar_err_message);
        if(ret_code == SQLITE_OK){
            event.nickname = base64::decode(str_nick_endcoded);
            return true;
        }
        else{
            is_no_sql_mode = true;
            #ifdef _DEBUG
            logger::log("dicebot nick_manager",std::string(sqlite3_errmsg(database)));
            #endif
            return false;
        }
    }

    bool write_database(event_info const & event){
        if(exist_database(event)){
            return update_database(event);
        }
        else return insert_database(event);
    }

    bool exist_database(event_info const & event){
        sqlite3 * database = database::database_manager::get_database();
        ostrs ostrs_sql_command(ostrs::ate);
        ostrs_sql_command << "SELECT count(*) FROM " NICK_TABLE_NAME;
        ostrs_sql_command << " where qqid =" << event.user_id;
        ostrs_sql_command << " and groupid =" << event.group_id;
        char * pchar_err_message = nullptr;
        int count = 0;

        int ret_code = sqlite3_exec(
            database, 
            ostrs_sql_command.str().c_str(), 
            &database::sqlite3_callback_exist, 
            (void*)&count, 
            &pchar_err_message);
        
        if(ret_code != SQLITE_OK){
            #ifdef _DEBUG
            logger::log("dicebot nick_manager",std::string(sqlite3_errmsg(database)));
            #endif
            is_no_sql_mode = true;
            return false;
        }
        return count > 0;
    }

    bool insert_database(event_info const & event){
        sqlite3 * database = database::database_manager::get_database();

        std::string name = base64::encode((unsigned char *)(event.nickname.c_str()),event.nickname.size());

        ostrs ostrs_sql_command(ostrs::ate);
        ostrs_sql_command.str("insert into " NICK_TABLE_NAME " values ( ");
        ostrs_sql_command << event.user_id << ", ";
        ostrs_sql_command << event.group_id << ", ";
        ostrs_sql_command << "'" << name << "'" << ");";
        char * pchar_err_message = nullptr;
        int ret_code = sqlite3_exec(
            database, 
            ostrs_sql_command.str().c_str(), 
            &database::sqlite3_callback_general, 
            (void*)&database::oper_update, 
            &pchar_err_message);
        
        if(ret_code != SQLITE_OK){
            #ifdef _DEBUG
            logger::log("dicebot nick_manager",std::string(sqlite3_errmsg(database)));
            #endif
            is_no_sql_mode = true;
            return false;
        }
        return true;
    }

    bool update_database(event_info const & event){
        sqlite3 * database = database::database_manager::get_database();

        std::string name = base64::encode((unsigned char *)(event.nickname.c_str()),event.nickname.size());

        ostrs ostrs_sql_command(ostrs::ate);
        ostrs_sql_command.str("update " NICK_TABLE_NAME " set ");
        ostrs_sql_command << " name ='" << name << "'";
        ostrs_sql_command << " where qqid =" << event.user_id;
        ostrs_sql_command << " and groupid =" << event.group_id;
        char * pchar_err_message = nullptr;
        int ret_code = sqlite3_exec(
            database, 
            ostrs_sql_command.str().c_str(), 
            &database::sqlite3_callback_general, 
            (void*)&database::oper_update, 
            &pchar_err_message);
        
        if(ret_code != SQLITE_OK){
            #ifdef _DEBUG
            logger::log("dicebot nick_manager",std::string(sqlite3_errmsg(database)));
            #endif
            is_no_sql_mode = true;
            return false;
        }
        return true;
    }

    int sqlite3_callback_query_name(void * data, int argc, char ** argv, char ** azColName){
        if(argc == 1){
            std::string * pstr_ret = (std::string *) data;
            pstr_ret->assign(std::string(argv[0]));
            return SQLITE_OK;
        }
        else return SQLITE_ABORT;
    }
}
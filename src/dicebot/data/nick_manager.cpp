#include "./database_manager.h"

#include <sstream>

#include "../../cqsdk/utils/vendor/cpp-base64/base64.h"
#include "./nick_manager.h"

using namespace dicebot;
using namespace dicebot::nickname;
using namespace dicebot::database;
using db_manager = dicebot::database::database_manager;

#define NICK_TABLE_NAME "nickname"
#define NICK_TABLE_DEFINE           \
    "create table " NICK_TABLE_NAME \
    "(qqid      int     NOT NULL,"  \
    "groupid    int     NOT NULL,"  \
    "name       text    NOT NULL,"  \
    "primary    key    (QQID,GROUPID));"

static std::string nickname_encode(const std::string &nick) {
    return base64_encode(reinterpret_cast<const unsigned char *>(nick.c_str()), nick.size());
}

static std::string nickname_decode(const std::string &source) { return base64_decode(source); }

std::unique_ptr<nickname_manager> nickname_manager::instance = nullptr;

nickname_manager *nickname_manager::create_instance() {
    db_manager::get_instance()->register_table(NICK_TABLE_NAME, NICK_TABLE_DEFINE);
    nickname_manager::instance = std::make_unique<nickname_manager>();
    return nickname_manager::instance.get();
}

void nickname_manager::destroy_instance() { nickname_manager::instance = nullptr; }

static bool read_database(event_info const &event, std::string &nickname) {
    std::ostringstream ostrs_sql_command;
    ostrs_sql_command << "SELECT name FROM " NICK_TABLE_NAME << " where qqid =" << event.user_id
                      << " and groupid =" << event.group_id;

    return db_manager::get_instance()->exec(
        ostrs_sql_command.str().c_str(), &nickname, [](void *data, int argc, char **argv, char **azColName) -> int {
            if (argc == 1) {
                std::string *nick = reinterpret_cast<std::string *>(data);
                *nick = nickname_decode(argv[0]);
                return SQLITE_OK;
            } else
                return SQLITE_ABORT;
        });
}

static bool exist_database(event_info const &event) {
    std::ostringstream ostrs_sql_command;
    ostrs_sql_command << "SELECT count(*) FROM " NICK_TABLE_NAME << " where qqid =" << event.user_id
                      << " and groupid =" << event.group_id;
    bool ret = false;
    db_manager::get_instance()->exec(
        ostrs_sql_command.str().c_str(), &ret, [](void *data, int argc, char **argv, char **azColName) -> int {
            *(reinterpret_cast<bool *>(data)) = std::stoi(argv[0]) > 0;
            return SQLITE_OK;
        });
    return ret;
}

static bool insert_database(event_info const &event) {
    std::ostringstream ostrs_sql_command;
    ostrs_sql_command << "insert into " NICK_TABLE_NAME " values ( " << event.user_id << ", " << event.group_id << ", "
                      << "'" << nickname_encode(event.nickname) << "'"
                      << ");";
    return db_manager::get_instance()->exec_noquery(ostrs_sql_command.str().c_str());
}

static bool update_database(event_info const &event) {
    std::ostringstream ostrs_sql_command;
    ostrs_sql_command << "update " NICK_TABLE_NAME " set "
                      << " name ='" << nickname_encode(event.nickname) << "'"
                      << " where qqid =" << event.user_id << " and groupid =" << event.group_id;

    return db_manager::get_instance()->exec_noquery(ostrs_sql_command.str().c_str());
}

static bool write_database(event_info const &event) {
    if (exist_database(event)) {
        return update_database(event);
    } else
        return insert_database(event);
}

bool nickname_manager::get_nickname(event_info const &event, std::string &nickname) {
    auto iter = nick_map.find(event.pair());
    if (iter != nick_map.end()) {
        nickname = iter->second;
        return true;
    } else {
        if (exist_database(event)) {
            read_database(event, nickname);
            nick_map.insert(nick_pair_t(event.pair(), nickname));
            return true;
        }
        return false;
    }
}

void nickname_manager::set_nickname(event_info const &event) {
    auto iter = nick_map.find(event.pair());
    if (iter != nick_map.end()) {
        iter->second = event.nickname;
        write_database(event);
    } else {
        auto t = nick_map.insert(nick_pair_t(event.pair(), event.nickname));
        if (t.second) write_database(event);
    }
}
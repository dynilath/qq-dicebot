#include "./database_manager.h"

#include <sstream>

#include "../../cqcppsdk/src/utils/base64.hpp"
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

#define NICK_READ "SELECT name FROM " NICK_TABLE_NAME " where qqid = ?1 and groupid = ?2"

#define NICK_EXIST "SELECT count(*) FROM " NICK_TABLE_NAME " where qqid = ?1 and groupid = ?2"

#define NICK_INSERT "insert into " NICK_TABLE_NAME " values (?1, ?2, ?3)"

#define NICK_UPDATE "update " NICK_TABLE_NAME " set name =?3 where qqid =?1 and groupid =?2"

sqlstmt_wrapper sqlstmt_nickname_read;
sqlstmt_wrapper sqlstmt_nickname_exist;
sqlstmt_wrapper sqlstmt_nickname_insert;
sqlstmt_wrapper sqlstmt_nickname_update;

static std::string nickname_encode(const std::string &nick) {
    return cq::utils::base64_encode(reinterpret_cast<const unsigned char *>(nick.c_str()), nick.size());
}

static std::string nickname_decode(const std::string &source) { return cq::utils::base64_decode(source); }

std::unique_ptr<nickname_manager> nickname_manager::instance = nullptr;

nickname_manager *nickname_manager::create_instance() {
    db_manager::get_instance()->register_table(NICK_TABLE_NAME, NICK_TABLE_DEFINE);
    nickname_manager::instance = std::make_unique<nickname_manager>();

    auto db = db_manager::get_instance()->get_database();
    sqlstmt_nickname_exist = {db, NICK_EXIST};
    sqlstmt_nickname_read = {db, NICK_READ};
    sqlstmt_nickname_insert = {db, NICK_INSERT};
    sqlstmt_nickname_update = {db, NICK_UPDATE};

    return nickname_manager::instance.get();
}

void nickname_manager::destroy_instance() { nickname_manager::instance = nullptr; }

static bool read_database(event_info const &event, std::string &nickname) {
    auto binded = sqlstmt_nickname_read.bind(event.user_id, event.group_id);
    if (binded.step() != SQLITE_ROW) return false;
    std::string out;
    binded.column(out);
    nickname = nickname_decode(out);
    return true;
}

static bool exist_database(event_info const &event) {
    auto binded = sqlstmt_nickname_exist.bind(event.user_id, event.group_id);
    if (binded.step() != SQLITE_ROW) return false;
    int result;
    binded.column(result);
    return result > 0;
}

static bool insert_database(event_info const &event) {
    std::string nick = nickname_encode(event.nickname);
    auto binded = sqlstmt_nickname_insert.bind(event.user_id, event.group_id, nick);
    return binded.step() == SQLITE_DONE;
}

static bool update_database(event_info const &event) {
    std::string nick = nickname_encode(event.nickname);
    auto binded = sqlstmt_nickname_update.bind(event.user_id, event.group_id, nick);
    return binded.step() == SQLITE_DONE;
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
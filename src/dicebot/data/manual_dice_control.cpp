#include "./manual_dice_control.h"

#include <sstream>

#include "../entity/manual_dice.h"
#include "./database_manager.h"

using namespace dicebot;
using namespace dicebot::manual;
using namespace dicebot::database;
using db_manager = dicebot::database::database_manager;

#define MANUALDICE_TABLE_NAME "manualdice"
#define MANUALDICE_TABLE_DEFINE           \
    "create table " MANUALDICE_TABLE_NAME \
    "(qqid      int     NOT NULL,"        \
    "groupid    int     NOT NULL,"        \
    "source     text    NOT NULL,"        \
    "primary    key    (QQID,GROUPID));"

#define MANUALDICE_INSERT "insert into " MANUALDICE_TABLE_NAME "(qqid, groupid, source) values (?1,?2,?3)"

#define MANUALDICE_UPDATE "update " MANUALDICE_TABLE_NAME " set source=?3 where qqid=?1 and groupid=?2"

#define MANUALDICE_READ "SELECT source FROM " MANUALDICE_TABLE_NAME " where qqid=?1 and groupid=?2"

#define MANUALDICE_EXIST "SELECT count(*) FROM " MANUALDICE_TABLE_NAME " where qqid=?1 and groupid=?2"

sqlstmt_wrapper sqlstmt_manual_dice_insert;
sqlstmt_wrapper sqlstmt_manual_dice_update;
sqlstmt_wrapper sqlstmt_manual_dice_read;
sqlstmt_wrapper sqlstmt_manual_dice_exist;

static bool insert_database(const int64_t &qqid, const int64_t &groupid, const manual_dice &manual_dice_target) {
    std::string encoded_str = manual_dice_target.encode();
    auto binded = sqlstmt_manual_dice_insert.bind(qqid, groupid, encoded_str);
    return binded.step() == SQLITE_DONE;
}

static bool update_database(const int64_t &qqid, const int64_t &groupid, const manual_dice &manual_dice_target) {
    std::string encoded_str = manual_dice_target.encode();
    auto binded = sqlstmt_manual_dice_update.bind(qqid, groupid, encoded_str);
    return binded.step() == SQLITE_DONE;
}

static bool read_database(const int64_t &qqid, const int64_t &groupid, manual_dice &manual_dice_target) {
    auto binded = sqlstmt_manual_dice_read.bind(qqid, groupid);
    if (binded.step() != SQLITE_ROW) return false;
    std::string out;
    binded.column(out);
    manual_dice_target.decode(out);
    return true;
}

static bool exist_database(const int64_t &qqid, const int64_t &groupid) {
    auto binded = sqlstmt_manual_dice_exist.bind(qqid, groupid);
    if (binded.step() != SQLITE_ROW) return false;
    int out;
    binded.column(out);
    return out > 0;
}

std::unique_ptr<manual_dice_control> manual_dice_control::instance = nullptr;

std::shared_mutex manual_dice_mutex;

manual_dice_control *manual_dice_control::create_instance() {
    db_manager::get_instance()->register_table(MANUALDICE_TABLE_NAME, MANUALDICE_TABLE_DEFINE);
    manual_dice_control::instance = std::make_unique<manual_dice_control>();

    sqlstmt_manual_dice_insert = {db_manager::get_instance()->get_database(), MANUALDICE_INSERT};
    sqlstmt_manual_dice_update = {db_manager::get_instance()->get_database(), MANUALDICE_UPDATE};
    sqlstmt_manual_dice_read = {db_manager::get_instance()->get_database(), MANUALDICE_READ};
    sqlstmt_manual_dice_exist = {db_manager::get_instance()->get_database(), MANUALDICE_EXIST};

    return manual_dice_control::instance.get();
}

manual_dice_control *manual_dice_control::get_instance() { return instance.get(); }

void manual_dice_control::destroy_instance() { instance = nullptr; }

manual_dice_control::md_map_t::iterator manual_dice_control::find_manual_dice(const event_info &event) {
    std::unique_lock lock(manual_dice_mutex);
    auto iter = this->manual_dice_map.find({event.user_id, event.group_id});
    if (iter != this->manual_dice_map.end()) return iter;

    auto [target, is_inserted] = this->manual_dice_map.insert({{event.user_id, event.group_id}, manual_dice()});
    auto &[key_pair, manual_dice_target] = *target;
    auto &[user_id, group_id] = key_pair;

    if (!is_inserted) return this->manual_dice_map.end();
    if (exist_database(user_id, group_id)) {
        read_database(user_id, group_id, manual_dice_target);
        return target;
    } else {
        insert_database(user_id, group_id, manual_dice_target);
        return target;
    }
}

void manual_dice_control::sync_database(const md_map_t::iterator iter) const {
    std::shared_lock lock(manual_dice_mutex);
    if (iter != this->manual_dice_map.end()) {
        auto &[key_pair, manual_dice_target] = *iter;
        auto &[user_id, group_id] = key_pair;
        update_database(user_id, group_id, manual_dice_target);
    }
}
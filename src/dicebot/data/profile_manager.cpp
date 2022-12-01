#include "./profile_manager.h"

#include <sstream>
#include <memory>

#include "../entity/profile.h"
#include "./database_manager.h"

using namespace dicebot;
using namespace dicebot::profile;
using namespace dicebot::database;

using db_manager = dicebot::database::database_manager;

#define PROFILE_TABLE_NAME "user_profile"
#define PROFILE_TABLE_DEFINE              \
    "create table " PROFILE_TABLE_NAME    \
    "(qqid             int     NOT NULL," \
    "system_variables  text,"             \
    "user_variables    text,"             \
    "default_roll      text,"             \
    "macro_roll        text,"             \
    "primary    key    (qqid));"

#define PROFILE_READ "SELECT system_variables, default_roll, macro_roll FROM " PROFILE_TABLE_NAME " where qqid = ?1"

#define PROFILE_EXIST "SELECT count(*) FROM " PROFILE_TABLE_NAME " where qqid = ?1"

#define PROFILE_INSERT "insert into " PROFILE_TABLE_NAME "(qqid,system_variables,default_roll,macro_roll) values (?1, ?2, ?3, ?4)"

#define PROFILE_UPDATE "update " PROFILE_TABLE_NAME " set system_variables=?2, default_roll=?3, macro_roll=?4 where qqid =?1"

sqlstmt_wrapper sqlstmt_profile_read;
sqlstmt_wrapper sqlstmt_profile_exist;
sqlstmt_wrapper sqlstmt_profile_insert;
sqlstmt_wrapper sqlstmt_profile_update;

static bool read_database(user_profile &profile, int64_t const user_id) {
    auto binded = sqlstmt_profile_read.bind(user_id);
    if (binded.step() != SQLITE_ROW) return false;
    std::string sys_var, def_roll, mac_roll;
    binded.column(sys_var, def_roll, mac_roll);

    profile.sys_vars.decode(sys_var);
    profile.def_roll.decode(def_roll);
    profile.mac_rolls.decode(mac_roll);
    return true;
}

static bool exist_database(int64_t const user_id) {
    auto binded = sqlstmt_profile_exist.bind(user_id);
    if (binded.step() != SQLITE_ROW) return false;
    int result;
    binded.column(result);
    return result > 0;
}

static bool insert_database(user_profile const &profile, int64_t const user_id) {
    std::string sys_var = profile.sys_vars.encode();
    std::string def_roll = profile.def_roll.encode();
    std::string mac_roll = profile.mac_rolls.encode();

    auto binded = sqlstmt_profile_insert.bind(user_id, sys_var, def_roll, mac_roll);
    return binded.step() == SQLITE_DONE;
}

static bool update_database(user_profile const &profile, int64_t const user_id) {
    std::string sys_var = profile.sys_vars.encode();
    std::string def_roll = profile.def_roll.encode();
    std::string mac_roll = profile.mac_rolls.encode();

    auto binded = sqlstmt_profile_update.bind(user_id, sys_var, def_roll, mac_roll);
    return binded.step() == SQLITE_DONE;
}

static bool write_database(user_profile const &profile, int64_t const user_id) {
    if (is_no_sql_mode)
        return false;
    else if (exist_database(user_id)) {
        return update_database(profile, user_id);
    } else {
        return insert_database(profile, user_id);
    }
}

using profile_pair = std::pair<int64_t, user_profile>;

std::unique_ptr<profile_manager> profile_manager::instance = nullptr;
std::shared_mutex profile_mutex;

profile_manager *profile_manager::create_instance() noexcept {
    db_manager::get_instance()->register_table(PROFILE_TABLE_NAME, PROFILE_TABLE_DEFINE);
    profile_manager::instance = std::make_unique<profile_manager>();

    auto db = db_manager::get_instance()->get_database();
    sqlstmt_profile_exist = {db, PROFILE_EXIST};
    sqlstmt_profile_read = {db, PROFILE_READ};
    sqlstmt_profile_insert = {db, PROFILE_INSERT};
    sqlstmt_profile_update = {db, PROFILE_UPDATE};

    return profile_manager::instance.get();
}

profile_manager *profile_manager::get_instance() noexcept { return instance.get(); }

void profile_manager::destroy_instance() noexcept { profile_manager::instance = nullptr; }

bool profile_manager::force_update(int64_t const user_id) const {
    std::shared_lock lock(profile_mutex);
    if (database::is_no_sql_mode) return false;

    auto iter = this->profiles.find(user_id);
    if (iter == this->profiles.end())
        return false;
    else {
        return write_database(iter->second, user_id);
    }
    return false;
}

sptr_user_profile profile_manager::get_profile(int64_t const user_id) {
    std::unique_lock lock(profile_mutex);
    auto iter = this->profiles.find(user_id);
    if (iter != this->profiles.end()) {
        return &iter->second;
    } else {
        if (database::is_no_sql_mode) return nullptr;

        auto t = this->profiles.insert({user_id, user_profile()});

        if (!t.second) return nullptr;

        if (exist_database(user_id)) {
            read_database(t.first->second, user_id);
        } else {
            insert_database(t.first->second, user_id);
        }
        return &(t.first->second);
    }
}

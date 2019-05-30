#include "./profile_manager.h"

#include <sstream>

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

static bool read_database(user_profile &profile, int64_t const user_id) {
    std::ostringstream ostrs_sql_command;
    ostrs_sql_command << "SELECT system_variables, default_roll, macro_roll "
                         "FROM " PROFILE_TABLE_NAME " where qqid="
                      << user_id;

    return db_manager::get_instance()->exec(
        ostrs_sql_command.str().c_str(), &profile, [](void *data, int argc, char **argv, char **azColName) -> int {
            user_profile *profile = (user_profile *)data;
            bool good_query =
                profile->sys_vars.decode(argv[0]) && profile->def_roll.decode(argv[1]) && profile->mac_rolls.decode(argv[2]);
            if (good_query)
                return SQLITE_OK;
            else
                return SQLITE_ABORT;
        });
}

static bool exist_database(int64_t const user_id) {
    std::ostringstream ostrs_sql_command;
    ostrs_sql_command << "SELECT count(*) FROM " PROFILE_TABLE_NAME " where qqid=" << user_id;
    bool ret = false;
    db_manager::get_instance()->exec(
        ostrs_sql_command.str().c_str(), &ret, [](void *data, int argc, char **argv, char **azColName) -> int {
            *reinterpret_cast<bool *>(data) = std::stoi(argv[0]) > 0;
            return SQLITE_OK;
        });
    return ret;
}

static bool insert_database(user_profile const &profile, int64_t const user_id) {
    std::ostringstream ostrs_sql_command;
    ostrs_sql_command << "insert into " PROFILE_TABLE_NAME " (qqid,system_variables,default_roll,macro_roll) "
                      << "values ( " << user_id << " ,'" << profile.sys_vars.encode() << "'"
                      << ", '" << profile.def_roll.encode() << "'"
                      << ", '" << profile.mac_rolls.encode() << "'"
                      << ");";

    return db_manager::get_instance()->exec_noquery(ostrs_sql_command.str().c_str());
}

static bool update_database(user_profile const &profile, int64_t const user_id) {
    char *pchar_err_message = nullptr;

    std::ostringstream ostrs_sql_command;
    ostrs_sql_command << "update " PROFILE_TABLE_NAME " set "
                      << " system_variables ='" << profile.sys_vars.encode() << "'"
                      << ", default_roll='" << profile.def_roll.encode() << "'"
                      << ", macro_roll='" << profile.mac_rolls.encode() << "'"
                      << " where qqid= " << user_id;

    return db_manager::get_instance()->exec_noquery(ostrs_sql_command.str().c_str());
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

profile_manager *profile_manager::create_instance() noexcept {
    db_manager::get_instance()->register_table(PROFILE_TABLE_NAME, PROFILE_TABLE_DEFINE);
    profile_manager::instance = std::make_unique<profile_manager>();
    return profile_manager::instance.get();
}

profile_manager *profile_manager::get_instance() noexcept { return instance.get(); }

void profile_manager::destroy_instance() noexcept { profile_manager::instance = nullptr; }

bool profile_manager::force_update(int64_t const user_id) const {
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

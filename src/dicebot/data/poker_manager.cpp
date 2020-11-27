#include "./poker_manager.h"

#include <mutex>

#include "./database_manager.h"

using namespace dicebot;
using namespace poker;
using namespace dicebot::database;
using db_manager = dicebot::database::database_manager;

#define POKER_TABLE_NAME "poker"
#define POKER_TABLE_DEFINE           \
    "create table " POKER_TABLE_NAME \
    "(qqid      int     NOT NULL,"        \
    "groupid    int     NOT NULL,"        \
    "source     text    ,"        \
    "drawn      text    ,"        \
    "deck       text    ,"        \
    "primary    key    (QQID,GROUPID));"

#define POKER_INSERT "insert into " POKER_TABLE_NAME "(qqid, groupid, source, drawn, deck) values (?1,?2,?3,?4,?5)"

#define POKER_UPDATE_DRAWN "update " POKER_TABLE_NAME " set drawn=?3,deck=?4 where qqid=?1 and groupid=?2"

#define POKER_UPDATE_ALL "update " POKER_TABLE_NAME " set source=?3,drawn=?4,deck=?5 where qqid=?1 and groupid=?2"

#define POKER_READ "SELECT source,drawn,deck FROM " POKER_TABLE_NAME " where qqid=?1 and groupid=?2"

#define POKER_EXIST "SELECT count(*) FROM " POKER_TABLE_NAME " where qqid=?1 and groupid=?2"

sqlstmt_wrapper sqlstmt_poker_read;
sqlstmt_wrapper sqlstmt_poker_exist;
sqlstmt_wrapper sqlstmt_poker_insert;
sqlstmt_wrapper sqlstmt_poker_update_drawn;
sqlstmt_wrapper sqlstmt_poker_update_all;

static bool read_database(poker_deck &deck, int64_t const user_id, int64_t const group_id) {
    auto binded = sqlstmt_poker_read.bind(user_id,group_id);
    if (binded.step() != SQLITE_ROW) return false;
    std::string sdefinition, sdrawn, sdeck;
    binded.column(sdefinition, sdrawn, sdeck);
    deck.load_pack(sdefinition, sdrawn, sdeck);

    return true;
}

static bool exist_database(int64_t const user_id, int64_t const group_id) {
    auto binded = sqlstmt_poker_exist.bind(user_id,group_id);
    if (binded.step() != SQLITE_ROW) return false;
    int result;
    binded.column(result);
    return result > 0;
}

static bool insert_database(poker_deck const &deck, int64_t const user_id, int64_t const group_id) {
    std::string sdef = deck.pack_definition();
    std::string sdrawn = deck.pack_drawn();
    std::string sdeck = deck.pack_deck();
    auto binded = sqlstmt_poker_insert.bind(user_id, group_id, sdef, sdrawn, sdeck);
    return binded.step() == SQLITE_DONE;
}

static bool update_drawn(poker_deck const &deck, int64_t const user_id, int64_t const group_id) {
    std::string sdrawn = deck.pack_drawn();
    std::string sdeck = deck.pack_deck();
    auto binded = sqlstmt_poker_update_drawn.bind(user_id, group_id,sdrawn,sdeck);
    return binded.step() == SQLITE_DONE;
}

static bool update_all(poker_deck const &deck, int64_t const user_id, int64_t const group_id) {
    std::string sdef = deck.pack_definition();
    std::string sdrawn = deck.pack_drawn();
    std::string sdeck = deck.pack_deck();
    auto binded = sqlstmt_poker_update_all.bind(user_id, group_id, sdef, sdrawn, sdeck);
    return binded.step() == SQLITE_DONE;
}

static bool write_db_drawn(poker_deck const &deck, int64_t const user_id, int64_t const group_id) {
    if (is_no_sql_mode)
        return false;
    else if (exist_database(user_id, group_id)) {
        return update_drawn(deck, user_id, group_id);
    } else {
        return insert_database(deck, user_id, group_id);
    }
}

static bool write_db_all(poker_deck const &deck, int64_t const user_id, int64_t const group_id) {
    if (is_no_sql_mode)
        return false;
    else if (exist_database(user_id, group_id)) {
        return update_all(deck, user_id, group_id);
    } else {
        return insert_database(deck, user_id, group_id);
    }
}



std::unique_ptr<poker_manager> poker_manager::instance = nullptr;

std::shared_mutex poker_manager_mutex;

poker_manager* poker_manager::create_instance() noexcept {
    poker_manager::instance = std::make_unique<poker_manager>();

    db_manager::get_instance()->register_table(POKER_TABLE_NAME, POKER_TABLE_DEFINE);
    auto db = db_manager::get_instance()->get_database();
    sqlstmt_poker_exist = {db, POKER_EXIST};
    sqlstmt_poker_read = {db, POKER_READ};
    sqlstmt_poker_insert = {db, POKER_INSERT};
    sqlstmt_poker_update_all = {db, POKER_UPDATE_ALL};
    sqlstmt_poker_update_drawn = {db, POKER_UPDATE_DRAWN};

    return poker_manager::instance.get();
}

poker_manager* poker_manager::get_instance() noexcept { return instance.get(); }

void poker_manager::destroy_instance() noexcept {
    poker_manager::instance = nullptr;
}

poker_deck& poker_manager::get_deck(const utils::pair_t& event) {
    std::unique_lock pokerm_lock(poker_manager_mutex);
    auto iter = this->poker_map.find(event);
    if (iter != this->poker_map.end()) {
        return iter->second;
    } else {
        poker_deck temp;
        if (exist_database(event.first, event.second)) {
            read_database(temp, event.first, event.second);
        }
        auto iter_insert = this->poker_map.insert({event, temp});
        return iter_insert.first->second;
    }
}


void poker_manager::sync_database(const utils::pair_t& ev) const{
    std::shared_lock pokerm_lock(poker_manager_mutex);
    auto iter = this->poker_map.find(ev);
    if (iter == this->poker_map.end()) return;
    auto& [key,val] = *iter;
    write_db_all(val,key.first,key.second);
}

void poker_manager::sync_database_draw(const utils::pair_t& ev) const{
    std::shared_lock pokerm_lock(poker_manager_mutex);
    auto iter = this->poker_map.find(ev);
    if (iter == this->poker_map.end()) return;
    auto& [key,val] = *iter;
    write_db_drawn(val,key.first,key.second);
}
#pragma once

#include "./common.h"

namespace dicebot::nickname{
    class nickname_manager{
        using nick_map_t = std::map<std::pair<int64_t, int64_t>, std::string>;
        using nick_pair_t = nick_map_t::value_type;
        using nick_key_t = nick_map_t::key_type;
    public:
        nickname_manager() noexcept;
        ~nickname_manager();
        static nickname_manager * instance;
        bool get_nickname(event_info & event);
        void set_nickname(event_info const & event);
    private:
        nick_map_t nick_map;
    };

    bool read_database(event_info & event);
    bool write_database(event_info const & event);
    bool exist_database(event_info const & event);
    bool insert_database(event_info const & event);
    bool update_database(event_info const & event);

    int sqlite3_callback_query_name(void * data, int argc, char ** argv, char ** azColName);
}

#pragma once

#include "./common.h"

namespace dicebot::manual{
    class manual_dice;

    using p_manual = std::shared_ptr<manual_dice>;
    using manual_kpair = std::pair<int64_t, int64_t>;
    using manual_map = std::map<manual_kpair, p_manual>;
    using manual_pair = std::pair<manual_kpair, p_manual>;
    
    class manual_dice_control:public std::map<manual_kpair, p_manual>{
    public:
        static manual_dice_control * instance;
        manual_dice_control();
        ~manual_dice_control();
        p_manual create_manual_dice(const int64_t user_id, const int64_t group_id, const std::string & command);
        p_manual roll_manual_dice(const int64_t user_id, const int64_t group_id, const std::string & command);
        p_manual kill_manual_dice(const int64_t user_id, const int64_t group_id, const std::string & command);
        p_manual add_manual_dice(const int64_t user_id, const int64_t group_id, const std::string & command);
        p_manual killall_manual_dice(const int64_t user_id, const int64_t group_id);
        static int sqlite3_callback_query_manualdice(void * data, int argc, char ** argv, char ** azColName);
        bool update_database(manual_kpair manual_dice_key,p_manual manual_dice_target) const;
        bool insert_database(manual_kpair manual_dice_key,p_manual manual_dice_target) const;
        bool read_database(manual_kpair manual_dice_key,p_manual manual_dice_target);
        bool exist_database(manual_kpair manual_dice_key) const;
    };
}

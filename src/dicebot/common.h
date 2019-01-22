#pragma once

#include <map>
#include <string>
#include <list>
#include <vector>
#include <stack>
#include <regex>
#include <sstream>
#include <memory>

#include <functional>

namespace dicebot{
    using ostrs = std::ostringstream;
    
    namespace roll{
        enum class roll_status{
            FINISHED = 0,
            GENERAL_ERR = 1,
            DICE_NOT_AVAILABLE = 2,
            UNINITIALIZED = -1,
            TOO_MANY_DICE = 3
        };

        enum class roll_mode{
            DND_DK = 1,
            COC_PB = 2,
            WOD_WA = 3
        };
    }
    
    namespace profile{
        enum class profile_status{
            finished,
            not_exist,
            exceed
        };

        enum class sys_var_type{
            rs_on
        };

        enum class def_roll_type{
            def_roll
        };
    }

    extern bool is_no_sql_mode;
    
    namespace logger{
        extern std::function<void(std::string const & scope,std::string const & content)> _log;
        extern void log(std::string const & scope,std::string const & content);
    }

    enum class event_type{
        group,
        discuss,
        pmessage
    };

    class event_info{
    public:
        int64_t user_id;
        int64_t group_id;
        event_type type;
        std::string nickname;

        event_info(int64_t id,int64_t group, event_type ev){
            this->user_id = id;
            this->group_id = group;
            this->type = ev;
        }

        event_info(int64_t id){
            this->user_id = id;
            this->group_id = 0;
            this->type = event_type::pmessage;
        }

        const auto pair() const{
            return std::pair<int64_t,int64_t>(user_id,group_id);
        };
    };
    
    #define MAX_DICE_UNIT_COUNT 10
    #define MAX_DICE_NUM 50
    #define MAX_DICE_FACE 1000
}
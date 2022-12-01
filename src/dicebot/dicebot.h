#pragma once

#include "./utils/number.h"
#include "./utils/utils.h"

#include <memory>
#include <regex>
#include <sstream>

#include "./data/database_manager.h"
#include "./data/manual_dice_control.h"
#include "./data/nick_manager.h"
#include "./data/poker_manager.h"
#include "./data/profile_manager.h"
#include "./dice_roller.h"
#include "./entry/entry_base.h"
#include "./entry/entry_manager.h"
#include "./entry/entry_manual_dice.h"
#include "./entry/entry_nickname.h"
#include "./entry/entry_poker.h"
#include "./entry/entry_profile.h"
#include "./entry/entry_roll_dice.h"
#include "./entry/entry_specialized_dice.h"
#include "./random/random_provider.h"
#include "./utils/logger.h"
#include "./utils/string_utils.h"

namespace dicebot {
    using nick_fill_call = ::std::function<::std::string()>;
    void initialize(const char*);
    void salvage();
    void set_logger(std::function<void(std::string, std::string)>);
    // bool try_fill_nickname(event_info&);
    bool message_pipeline(std::string const&, event_info&, nick_fill_call, std::string&);
} // namespace dicebot
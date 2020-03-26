#pragma once
#include <cqcppsdk/cqcppsdk.hpp>
#include "./dicebot/event_info.h"

cq::message::Message seg_cq_code(std::string str);
bool get_group_nickname(event_info &, uint64_t const , uint64_t const );
bool get_nickname(event_info &, uint64_t const );
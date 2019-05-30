#pragma once

#include <functional>
#include "./entry_base.h"

namespace dicebot::entry {
    class entry_manual_dice : public entry_base {
    public:
        entry_manual_dice() noexcept;
        bool resolve_request(std::string const &message, event_info &event, std::string &response) noexcept override;
    };
} // namespace dicebot::entry
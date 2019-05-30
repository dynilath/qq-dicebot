#pragma once

#include <regex>
#include <unordered_map>

#include "./entry_base.h"

namespace dicebot::entry {
    class entry_wod_dice : public entry_base {
    public:
        entry_wod_dice() noexcept;
        bool resolve_request(std::string const &message, event_info &event, std::string &response) noexcept override;
    };

    class entry_coc_dice : public entry_base {
    public:
        entry_coc_dice() noexcept;
        bool resolve_request(std::string const &message, event_info &event, std::string &response) noexcept override;
    };

    class entry_fate_dice : public entry_base {
    public:
        entry_fate_dice() noexcept;
        bool resolve_request(std::string const &message, event_info &event, std::string &response) noexcept override;
    };
} // namespace dicebot::entry
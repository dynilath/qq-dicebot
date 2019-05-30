#pragma once

#include "../entity/profile.h"
#include "./entry_base.h"
namespace dicebot::entry {
    class entry_set_roll : public entry_base {
    public:
        entry_set_roll() noexcept;
        bool resolve_request(std::string const &message, event_info &event, std::string &response) noexcept override;
    };

    class entry_list : public entry_base {
    public:
        entry_list() noexcept;
        bool resolve_request(std::string const &message, event_info &event, std::string &response) noexcept override;
    };

    class entry_delete : public entry_base {
    public:
        entry_delete() noexcept;
        bool resolve_request(std::string const &message, event_info &event, std::string &response) noexcept override;
    };
} // namespace dicebot::entry
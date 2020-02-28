#pragma once

#include <sstream>

namespace dicebot {
    class output_constructor {
    private:
        std::ostringstream _ot;

    public:
        output_constructor(std::string const &nickname) {
            _ot.clear();
            _ot << " * " << nickname << " ";
        }
        template <class any_t>
        output_constructor(std::string const &nickname, any_t &&message) {
            _ot.clear();
            _ot << " * " << nickname << " ";
            if (message.length() > 0) _ot << std::forward<decltype(message)>(message) << " ";
        }

        template <class any_t>
        void append_message(any_t &&message, bool pre_space = false, bool suf_space = true) {
            if (message.length() > 0) {
                if(pre_space) _ot << " ";
                _ot << std::forward<decltype(message)>(message);
                if(suf_space) _ot << " ";
            }
        }

        template <class any_t>
        void append_roll(std::string const &roll_command, std::string const &detail, any_t &&roll_result) {
            _ot << u8"掷骰: " << roll_command;
            if (!detail.empty()) _ot << " = " << detail;
            _ot << " = " << std::forward<decltype(roll_result)>(roll_result);
        }

        operator std::string() const noexcept { return _ot.str(); }

        template <class any_t>
        output_constructor &operator<<(any_t &&message) {
            _ot << std::forward<decltype(message)>(message);
            return *this;
        }
    };
} // namespace dicebot

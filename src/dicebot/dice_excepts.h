#pragma once
#include <exception>

namespace dicebot {
    class zero_divider_exception : public std::exception {
        const char* what() const noexcept override { return "divided by zero or near zero value."; }
    };

    class face_exceed : public std::exception {
        const char* what() const noexcept override { return "too many faces for a die."; }
    };
    class dice_exceed : public std::exception {
        const char* what() const noexcept override { return "too many dice for a roll."; }
    };
    class unit_exceed : public std::exception {
        const char* what() const noexcept override { return "too many times for repeating roll."; }
    };
    class invalid_dice : public std::exception {
        const char* what() const noexcept override { return "a positive integer is required."; }
    };
    class invalid_macro : public std::exception {
        const char* what() const noexcept override { return "invalid char in macro name."; }
    };
} // namespace dicebot

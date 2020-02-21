#pragma once

#include <algorithm>
#include <numeric>
#include <string>
#include <vector>

namespace dicebot::utils {

    void remove_blank(std::string& source);

    template <class reference_cont>
    std::vector<size_t> pilot_sort(const reference_cont& ref) {
        std::vector<size_t> pilot(ref.size());
        std::iota(pilot.begin(), pilot.end(), (size_t)0);
        std::stable_sort(pilot.begin(), pilot.end(), [&ref](size_t a, size_t b) -> bool { return ref[a] < ref[b]; });
        return pilot;
    }

    bool basic_event_filter(const std::string& message) noexcept;

    void lower_case(std::string& target);
    std::string lower_case_copy(const std::string& target);

    template <class _Function>
    inline void repeat(size_t times, _Function func) {
        while (times--) func(times);
    }
} // namespace dicebot::utils
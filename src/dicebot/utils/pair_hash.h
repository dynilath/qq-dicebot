#pragma once
#include <unordered_map>
namespace dicebot::utils {
    using pair_t = std::pair<int64_t, int64_t>;
    constexpr size_t offset_basis = 2166136261;
    constexpr size_t FNV_prime = 16777619;
    struct pair_hash {
        size_t operator()(const pair_t& p) const noexcept {
            size_t ret = offset_basis;
            const uint8_t* pp = reinterpret_cast<const uint8_t*>(&p);
            for (size_t i = 0; i < sizeof(p); i++) {
                ret ^= *(pp + i);
                ret *= FNV_prime;
            }
            return ret;
        }
    };
} // namespace dicebot::utils

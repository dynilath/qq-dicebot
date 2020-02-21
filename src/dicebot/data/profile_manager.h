#pragma once

#include <unordered_map>
#include <memory>
#include "../entity/profile.h"

namespace dicebot::profile {
    using sptr_user_profile = user_profile*;
    using profile_map = std::unordered_map<int64_t, user_profile>;
    using profile_pair = profile_map::value_type;

    class profile_manager {
        static std::unique_ptr<profile_manager> instance;
        profile_map profiles;

    public:
        static profile_manager* create_instance() noexcept;
        static profile_manager* get_instance() noexcept;
        static void destroy_instance() noexcept;

        bool force_update(int64_t const user_id) const;

        user_profile* get_profile(int64_t const user_id);
    };
} // namespace dicebot::profile
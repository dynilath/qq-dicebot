#ifdef _WIN32
#ifdef _DEBUG
#pragma comment(lib, "gtestd.lib")
#else
#pragma comment(lib, "gtest.lib")
#endif
#endif

#include <experimental/filesystem>

#include "../dicebot/dicebot.h"
#include "gtest/gtest.h"

namespace dicebot::test {
#pragma region tokenizer test
    TEST(database, manual_dice) {
        dicebot::event_info ei(123456);
        ei.nickname = "dynilath";

        std::deque<int> result;

        std::string test_db = "./build/test_db_1/";

        dicebot::initialize(test_db.c_str());
        auto md = manual::manual_dice_control::get_instance()->find_manual_dice(ei);
        md->second.killall();
        const int test_face = 6;
        md->second.add({test_face, test_face, test_face, test_face});
        for (auto& item : md->second.mdice) {
            result.push_back(item.second);
            ASSERT_EQ(item.first, 6);
        }
        manual::manual_dice_control::get_instance()->sync_database(md);
        dicebot::salvage();

        dicebot::initialize(test_db.c_str());
        auto md2 = manual::manual_dice_control::get_instance()->find_manual_dice(ei);
        for (auto& item : md2->second.mdice) {
            ASSERT_EQ(item.second, result.front());
            result.pop_front();
        }
        dicebot::salvage();
    }

    TEST(database, profile) {
        dicebot::event_info ei(123456);
        ei.nickname = "dynilath";

        std::deque<int> result;

        std::string test_db = "./build/test_db_2/";

        dicebot::initialize(test_db.c_str());
        auto pf = profile::profile_manager::get_instance()->get_profile(ei.user_id);
        pf->sys_vars[profile::sys_var_type::rs_on] = profile::var_rs_on;
        pf->sys_vars[profile::sys_var_type::rs_on] = profile::var_rs_off;

        std::string def_roll = "(4d6)";
        pf->def_roll[profile::def_roll_type::def_roll] = def_roll;

        std::string test_roll = "(4d6k3)";
        std::string test_roll_name = "strength";
        pf->mac_rolls[test_roll_name] = test_roll;
        profile::profile_manager::get_instance()->force_update(ei.user_id);
        dicebot::salvage();

        dicebot::initialize(test_db.c_str());
        auto pf2 = profile::profile_manager::get_instance()->get_profile(ei.user_id);
        ASSERT_EQ(pf2->sys_vars[profile::sys_var_type::rs_on], profile::var_rs_off);
        ASSERT_EQ(pf2->def_roll[profile::def_roll_type::def_roll], def_roll);
        ASSERT_EQ(pf2->mac_rolls[test_roll_name], test_roll);
        dicebot::salvage();
    }
#pragma endregion
} // namespace dicebot::test

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
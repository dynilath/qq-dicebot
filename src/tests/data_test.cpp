#include <filesystem>

#include "../dicebot/dicebot.h"
#include "gtest/gtest.h"

#ifndef DB_FOLDER_1
#define DB_FOLDER_1 ""
#endif

#ifndef DB_FOLDER_2
#define DB_FOLDER_2 ""
#endif

#ifndef DB_FOLDER_3
#define DB_FOLDER_3 ""
#endif

#ifndef DB_FOLDER_4
#define DB_FOLDER_4 ""
#endif


namespace dicebot::test {
#pragma region tokenizer test
    TEST(database, manual_dice) {
        ::event_info ei(123456);
        ei.nickname = "dynilath";

        std::deque<int> result;

        std::string test_db = DB_FOLDER_1;

        dicebot::initialize(test_db.c_str());
        auto md =
            manual::manual_dice_control::get_instance()->find_manual_dice(ei);
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
        auto md2 =
            manual::manual_dice_control::get_instance()->find_manual_dice(ei);
        for (auto& item : md2->second.mdice) {
            ASSERT_EQ(item.second, result.front());
            result.pop_front();
        }
        dicebot::salvage();
    }

    TEST(database, profile) {
        ::event_info ei(123456);
        ei.nickname = "dynilath";

        std::deque<int> result;

        std::string test_db = DB_FOLDER_2;

        dicebot::initialize(test_db.c_str());
        auto pf =
            profile::profile_manager::get_instance()->get_profile(ei.user_id);
        pf->sys_vars[profile::sys_var_type::rs_on] = profile::var_rs_on;
        pf->sys_vars[profile::sys_var_type::rs_on] = profile::var_rs_off;

        std::string def_roll = "(4d6)";
        pf->def_roll[profile::def_roll_type::def_roll] = def_roll;

        std::string test_roll = "(4d6k3)";
        std::string test_roll_name = "strength";
        pf->mac_rolls[test_roll_name] = test_roll;
        ASSERT_TRUE(
            profile::profile_manager::get_instance()->force_update(ei.user_id));
        dicebot::salvage();

        dicebot::initialize(test_db.c_str());
        auto pf2 =
            profile::profile_manager::get_instance()->get_profile(ei.user_id);
        ASSERT_EQ(pf2->sys_vars[profile::sys_var_type::rs_on],
                  profile::var_rs_off);
        ASSERT_EQ(pf2->def_roll[profile::def_roll_type::def_roll], def_roll);
        ASSERT_EQ(pf2->mac_rolls[test_roll_name], test_roll);
        dicebot::salvage();
    }

    TEST(database, nickname) {
        ::event_info ei(123456);
        ei.nickname = "dynilath";

        std::string test_db = DB_FOLDER_3;

        std::string name;

        dicebot::initialize(test_db.c_str());
        nickname::nickname_manager::instance->set_nickname(ei);
        ASSERT_TRUE(nickname::nickname_manager::instance->get_nickname(ei.user_id, name));
        ASSERT_TRUE(name == ei.nickname);
        dicebot::salvage();

        dicebot::initialize(test_db.c_str());
        ASSERT_TRUE(nickname::nickname_manager::instance->get_nickname(ei.user_id, name));
        ASSERT_TRUE(name == ei.nickname);
        dicebot::salvage();

        ::event_info ei2(123457);
        ei2.nickname = "dynilath2";

        dicebot::initialize(test_db.c_str());
        nickname::nickname_manager::instance->set_nickname(ei2);
        ASSERT_TRUE(nickname::nickname_manager::instance->get_nickname(ei2.user_id, name));
        ASSERT_TRUE(name == ei2.nickname);
        dicebot::salvage();

        dicebot::initialize(test_db.c_str());
        ASSERT_TRUE(nickname::nickname_manager::instance->get_nickname(ei2.user_id, name));
        ASSERT_TRUE(name == ei2.nickname);
        dicebot::salvage();

        dicebot::initialize(test_db.c_str());
        ASSERT_TRUE(nickname::nickname_manager::instance->get_nickname(ei.user_id, name));
        ASSERT_TRUE(name == ei.nickname);
        dicebot::salvage();
    }    
    
    TEST(database, poker) {
        ::event_info ei(123456);
        ei.nickname = "dynilath";

        std::string poker_command = "a,b,c,d,e,f,g";

        std::string test_db = DB_FOLDER_4;
        
        std::string name;

        dicebot::initialize(test_db.c_str());
        auto& deck1 = poker::poker_manager::get_instance()->get_deck(ei.pair());
        deck1.init(poker_command);
        ASSERT_TRUE(deck1.size() == 7);
        ASSERT_TRUE(deck1.drawer.size() == 0);

        deck1.draw(2);
        ASSERT_TRUE(deck1.size() == 5);
        ASSERT_TRUE(deck1.drawer.size() == 2);
        poker::poker_manager::get_instance()->sync_database(ei.pair());
        dicebot::salvage();

        dicebot::initialize(test_db.c_str());
        auto deck2 = poker::poker_manager::get_instance()->get_deck(ei.pair());
        ASSERT_TRUE(deck2.size() == 5);
        ASSERT_TRUE(deck2.drawer.size() == 2);
        dicebot::salvage();
    }
#pragma endregion
} // namespace dicebot::test

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#include <climits>
#include <cmath>
#include <list>
#include <regex>

#include "gtest/gtest.h"

#include "dicebot/dicebot.h"

#ifndef DB_FOLDER
#define DB_FOLDER ""
#endif

class entry_test : public ::testing::Test {
protected:
    entry_test() { dicebot::initialize(DB_FOLDER); }
    ~entry_test() { dicebot::salvage(); }

public:
    bool test_call(::event_info &ei, const std::string &source, const std::regex &reg_test) {
        std::string output;
        dicebot::try_fill_nickname(ei);
        dicebot::message_pipeline(source, ei, output);
        return std::regex_search(output, reg_test);
    }

    void base_call(::event_info &ei, const std::string &source) {
        std::string output;
        dicebot::try_fill_nickname(ei);
        dicebot::message_pipeline(source, ei, output);
    }
};

TEST_F(entry_test, roll_2d20plus4) {
    ::event_info ei(123456, 10000, ::event_type::group);
    ei.nickname = "dynilath";

    std::string source_1 = ".r2d20+4";
    std::string source_2 = ".r 2d20+4";
    std::string source_3 = ".roll2d20+4";
    std::string source_4 = ".roll 2d20+4";

    this->base_call(ei, ".ndice");

    std::regex result_reg(
        u8"^ \\* dice 掷骰: 2d20 \\+ 4 = \\[\\d{1,2} \\+ \\d{1,2}\\] \\+ 4 = "
        u8"\\d{1,2}$");
    std::regex result_reg_s(u8"^ \\* dice 掷骰: 2d20 \\+ 4 = \\d{1,2}$");
    std::regex result_reg_s_msg(u8"^ \\* dice test 掷骰: 2d20 \\+ 4 = \\d{1,2}$");

    std::regex roll_source_on(u8"^ \\* dice 启用骰子详细输出$");
    ASSERT_TRUE(this->test_call(ei, ".rson", roll_source_on));
    ASSERT_TRUE(this->test_call(ei, ".r s on", roll_source_on));
    ASSERT_TRUE(this->test_call(ei, ".r s on", roll_source_on));
    ASSERT_TRUE(this->test_call(ei, ".roll source on", roll_source_on));
    ASSERT_TRUE(this->test_call(ei, source_1, result_reg));
    ASSERT_TRUE(this->test_call(ei, source_2, result_reg));
    ASSERT_TRUE(this->test_call(ei, source_3, result_reg));
    ASSERT_TRUE(this->test_call(ei, source_4, result_reg));

    std::regex roll_source_off(u8"^ \\* dice 关闭骰子详细输出$");
    ASSERT_TRUE(this->test_call(ei, ".rsoff", roll_source_off));
    ASSERT_TRUE(this->test_call(ei, ".r s off", roll_source_off));
    ASSERT_TRUE(this->test_call(ei, ".r s off", roll_source_off));
    ASSERT_TRUE(this->test_call(ei, ".roll source off", roll_source_off));
    ASSERT_TRUE(this->test_call(ei, source_1, result_reg_s));
    ASSERT_TRUE(this->test_call(ei, source_2, result_reg_s));
    ASSERT_TRUE(this->test_call(ei, source_3, result_reg_s));
    ASSERT_TRUE(this->test_call(ei, source_4, result_reg_s));
    ASSERT_TRUE(this->test_call(ei, ".rs2d20+4", result_reg));
    ASSERT_TRUE(this->test_call(ei, ".r s 2d20+4", result_reg));
    ASSERT_TRUE(this->test_call(ei, ".roll s 2d20+4", result_reg));
    ASSERT_TRUE(this->test_call(ei, ".roll source 2d20+4", result_reg));

    this->base_call(ei, ".rsoff");
    ASSERT_TRUE(this->test_call(ei, ".r 2d20+4", result_reg_s));
    ASSERT_TRUE(this->test_call(ei, ".r 2d20+4 ", result_reg_s));
    ASSERT_TRUE(this->test_call(ei, " . r 2d20+4 ", result_reg_s));
    ASSERT_TRUE(this->test_call(ei, ".r 2d20+4test", result_reg_s_msg));
    ASSERT_TRUE(this->test_call(ei, ".r 2d20+4 test", result_reg_s_msg));
    ASSERT_TRUE(this->test_call(ei, ".r 2d20+4 test", result_reg_s_msg));
}

TEST_F(entry_test, roll_sharp) {
    ::event_info ei(123456, 10000, ::event_type::group);
    ei.nickname = "dynilath";

    std::string source = ".r6#4d6kl3";

    this->base_call(ei, ".ndice");
    this->base_call(ei, ".rson");

    std::regex result_reg(
        u8"^ \\* dice 掷骰: 6\\#4d6kl3 = \\{(\\[(\\d\\*? \\+ ){3}\\d\\*?\\], ){5}(\\[(\\d\\*? \\+ ){3}\\d\\*?\\])\\} = "
        u8"\\{(\\d{1,2}, ){5}\\d{1,2}\\}$");
    ASSERT_TRUE(this->test_call(ei, source, result_reg));

    ASSERT_TRUE(this->test_call(ei, ".rsoff", std::regex(u8"^ \\* dice 关闭骰子详细输出$")));
    std::regex result_reg_s(
        u8"^ \\* dice 掷骰: 6\\#4d6kl3 = \\{(\\d{1,2}, "
        u8"){5}\\d{1,2}\\}$");
    ASSERT_TRUE(this->test_call(ei, source, result_reg_s));
    ASSERT_TRUE(this->test_call(ei, ".rs6#4d6kl3", result_reg));
}

TEST_F(entry_test, roll_brace_calculus) {
    ::event_info ei(123456, 10000, ::event_type::group);
    ei.nickname = "dynilath";

    std::string source = ".r2#d20-{1,10}";

    this->base_call(ei, ".ndice");
    this->base_call(ei, ".rson");

    std::regex result(
        u8"^ \\* dice 掷骰: 2\\#d20 - \\{1, 10\\} = "
        u8"\\{\\[\\d{1,2}\\], \\[\\d{1,2}\\]\\} - \\{1, 10\\} = "
        u8"\\{\\d{1,2}, -?\\d{1,2}\\}");

    ASSERT_TRUE(this->test_call(ei, source, result));
}

TEST_F(entry_test, name) {
    ::event_info ei(123456, 10000, ::event_type::group);
    ei.nickname = "dynilath";

    ei.group_id = 10001;
    ASSERT_TRUE(this->test_call(ei, ".ndice1", std::regex(u8"^ \\* .* 的新名字是 dice1$")));
    ASSERT_TRUE(this->test_call(ei, ".namedice1", std::regex(u8"^ \\* dice1 的新名字是 dice1$")));
    ASSERT_TRUE(this->test_call(ei, ".n   dice1", std::regex(u8"^ \\* dice1 的新名字是 dice1$")));
    ASSERT_TRUE(this->test_call(ei, ".name   dice1", std::regex(u8"^ \\* dice1 的新名字是 dice1$")));

    ei.group_id = 10002;
    ASSERT_TRUE(this->test_call(ei, ".ndice2", std::regex(u8"^ \\* .* 的新名字是 dice2$")));
    ASSERT_TRUE(this->test_call(ei, ".namedice2", std::regex(u8"^ \\* dice2 的新名字是 dice2$")));
    ASSERT_TRUE(this->test_call(ei, ".n   dice2", std::regex(u8"^ \\* dice2 的新名字是 dice2$")));
    ASSERT_TRUE(this->test_call(ei, ".name   dice2", std::regex(u8"^ \\* dice2 的新名字是 dice2$")));

    std::regex result_reg_r1(
        u8"^ \\* dice1 掷骰: 2d20 \\+ 4 = \\[\\d{1,2} \\+ \\d{1,2}\\] \\+ 4 = "
        u8"\\d{1,2}$");
    std::regex result_reg_r2(
        u8"^ \\* dice2 掷骰: 2d20 \\+ 4 = \\[\\d{1,2} \\+ \\d{1,2}\\] \\+ 4 = "
        u8"\\d{1,2}$");

    ei.group_id = 10001;
    ASSERT_TRUE(this->test_call(ei, ".rs2d20+4", result_reg_r1));

    ei.group_id = 10002;
    ASSERT_TRUE(this->test_call(ei, ".rs2d20+4", result_reg_r2));

    ei.group_id = 10001;
    ASSERT_TRUE(this->test_call(ei, ".ndice", std::regex(u8"^ \\* dice1 的新名字是 dice$")));

    ei.group_id = 10002;
    ASSERT_TRUE(this->test_call(ei, ".ndice", std::regex(u8"^ \\* dice2 的新名字是 dice$")));
}

TEST_F(entry_test, macro_recall) {
    ::event_info ei(123456, 10000, ::event_type::group);
    ei.nickname = "dynilath";

    this->base_call(ei, ".ndice");
    ASSERT_TRUE(this->test_call(ei, ".s 4d6", std::regex(u8"^ \\* dice 设置默认骰子指令: \\(4d6\\)")));
    ASSERT_TRUE(this->test_call(ei, ".set 4d6", std::regex(u8"^ \\* dice 设置默认骰子指令: \\(4d6\\)")));

    std::regex result_reg(
        u8"^ \\* dice 掷骰: \\(4d6\\) = \\(\\[\\d \\+ \\d \\+ \\d \\+ "
        u8"\\d\\]\\) = \\d{1,2}$");
    std::regex result_reg_s(u8"^ \\* dice 掷骰: \\(4d6\\) = \\d{1,2}$");

    this->base_call(ei, ".rsoff");
    ASSERT_TRUE(this->test_call(ei, ".rs", result_reg));
    ASSERT_TRUE(this->test_call(ei, ".r", result_reg_s));

    ASSERT_TRUE(this->test_call(ei, ".s 4d6 test", std::regex(u8"^ \\* dice 设置指令: \\(4d6\\) 为 test")));
    ASSERT_TRUE(this->test_call(ei, ".set 4d6 test", std::regex(u8"^ \\* dice 设置指令: \\(4d6\\) 为 test")));

    this->base_call(ei, ".rsoff");
    ASSERT_TRUE(this->test_call(ei, ".rs test", result_reg));
    ASSERT_TRUE(this->test_call(ei, ".r test", result_reg_s));

    ASSERT_TRUE(this->test_call(ei, ".s 4 test", std::regex(u8"^ \\* dice 设置指令: 4 为 test")));

    std::regex result2_reg(
        u8"^ \\* dice 掷骰: 4d6 = \\[\\d \\+ \\d \\+ \\d \\+ \\d\\] = "
        u8"\\d{1,2}$");
    std::regex result2_reg_s(u8"^ \\* dice 掷骰: 4d6 = \\d{1,2}$");

    this->base_call(ei, ".rsoff");
    ASSERT_TRUE(this->test_call(ei, ".rs testd6", result2_reg));
    ASSERT_TRUE(this->test_call(ei, ".rs test d 6 ", result2_reg));
    ASSERT_TRUE(this->test_call(ei, ".r testd6", result2_reg_s));
    ASSERT_TRUE(this->test_call(ei, ".r test d 6", result2_reg_s));

    this->base_call(ei, ".s 4d6k3");
    ASSERT_TRUE(this->test_call(ei, ".l", std::regex(u8"\\n\\* 默认 : \\(4d6k3\\)")));
    this->base_call(ei, ".s 4d6kl3");
    ASSERT_TRUE(this->test_call(ei, ".l", std::regex(u8"\\n\\* 默认 : \\(4d6kl3\\)")));
    this->base_call(ei, ".s 4d6k3 str");
    ASSERT_TRUE(this->test_call(ei, ".l", std::regex(u8"\\n> str : \\(4d6k3\\)")));
    this->base_call(ei, ".s 4d6kl3 str");
    ASSERT_TRUE(this->test_call(ei, ".l", std::regex(u8"\\n> str : \\(4d6kl3\\)")));

    ASSERT_TRUE(this->test_call(ei, ".d str", std::regex(u8"^ \\* dice 已删除骰子指令: str")));
    ASSERT_TRUE(this->test_call(ei, ".d", std::regex(u8"^ \\* dice 已删除所有骰子指令")));

    this->base_call(ei, ".s 4d6k3 test1");
    this->base_call(ei, ".s 4d6k3 test2");
    this->base_call(ei, ".s 4d6k3 test3");
    this->base_call(ei, ".s 4d6k3 tess");
    ASSERT_TRUE(this->test_call(ei, ".l test", std::regex(u8"(\\r\\n> test\\d : \\(4d6k3\\)){3}$")));
}

TEST_F(entry_test, roll_coc) {
    ::event_info ei(123456, 10000, ::event_type::group);
    ei.nickname = "dynilath";

    this->base_call(ei, ".ndice");
    ASSERT_TRUE(this->test_call(ei, ".c", std::regex(u8"^ \\* dice 掷骰: CoC = \\d{1,3}")));
    ASSERT_TRUE(this->test_call(ei, ".coc", std::regex(u8"^ \\* dice 掷骰: CoC = \\d{1,3}")));
    ASSERT_TRUE(this->test_call(ei, ".c test", std::regex(u8"^ \\* dice test 掷骰: CoC = \\d{1,3}")));
    ASSERT_TRUE(this->test_call(ei, ".coc test", std::regex(u8"^ \\* dice test 掷骰: CoC = \\d{1,3}")));

    std::string regex_prefix = u8" \\* dice 掷骰: CoC";
    std::string regex_suffix = u8" = \\[\\d{1,2}\\*? \\+ \\d{1,2}\\*?\\] \\[\\d\\] = \\d{1,3}";

    ASSERT_TRUE(this->test_call(ei, ".cb1", std::regex(regex_prefix + " b1" + regex_suffix)));
    ASSERT_TRUE(this->test_call(ei, ".cp1", std::regex(regex_prefix + " p1" + regex_suffix)));

    ASSERT_TRUE(this->test_call(ei, ".cb2p1", std::regex(regex_prefix + " b1" + regex_suffix)));
    ASSERT_TRUE(this->test_call(ei, ".cp2b1", std::regex(regex_prefix + " p1" + regex_suffix)));

    std::string regex_prefix_2 = u8" \\* dice test 掷骰: CoC";
    std::regex reg_coc_full_test(regex_prefix_2 + " \\(55\\) b1" + regex_suffix
                                 + " (crit success|extreme success|hard success|success|fail|crit fail)");
    ASSERT_TRUE(this->test_call(ei, ".c55b2p1test", reg_coc_full_test));
    ASSERT_TRUE(this->test_call(ei, ".c55b2p1 test", reg_coc_full_test));
    ASSERT_TRUE(this->test_call(ei, ".cb2 55p1test", reg_coc_full_test));
    ASSERT_TRUE(this->test_call(ei, ".cb2 55p1 test", reg_coc_full_test));
    ASSERT_TRUE(this->test_call(ei, ".cb2 55p1 test", reg_coc_full_test));
    ASSERT_TRUE(this->test_call(ei, ".cb2p1 55test", reg_coc_full_test));
    ASSERT_TRUE(this->test_call(ei, ".cb2p1 55 test", reg_coc_full_test));
}

TEST_F(entry_test, roll_wod) {
    ::event_info ei(123456, 10000, ::event_type::group);
    ei.nickname = "dynilath";

    std::regex reg_wodo6(
        u8"^ \\* dice 掷骰: oWoD = \\[((?:\\d{1,2}|(?:\\d\\*){1,2}) \\+ ){5}(?:\\d{1,2}|(?:\\d\\*){1,2})\\] = "
        u8"\\d");
    std::regex reg_wodn6(
        u8"^ \\* dice 掷骰: nWoD = \\[((?:\\d{1,2}|(?:\\d\\*){1,2}) \\+ ){5,}(?:\\d{1,2}|(?:\\d\\*){1,2})\\] = "
        u8"\\d");

    std::regex reg_wodo6_msg(
        u8"^ \\* dice test 掷骰: oWoD = \\[((?:\\d{1,2}|(?:\\d\\*){1,2}) \\+ "
        u8"){5}(?:\\d{1,2}|(?:\\d\\*){1,2})\\] = \\d");
    std::regex reg_wodn6_msg(
        u8"^ \\* dice test 掷骰: nWoD = \\[((?:\\d{1,2}|(?:\\d\\*){1,2}) \\+ "
        u8"){5,}(?:\\d{1,2}|(?:\\d\\*){1,2})\\] = \\d");
    this->base_call(ei, ".ndice");
    ASSERT_TRUE(this->test_call(ei, ".wodo6", reg_wodo6));
    ASSERT_TRUE(this->test_call(ei, ".wodn6", reg_wodn6));
    ASSERT_TRUE(this->test_call(ei, ".wo6", reg_wodo6));
    ASSERT_TRUE(this->test_call(ei, ".wn6", reg_wodn6));
    ASSERT_TRUE(this->test_call(ei, ".wodo6 test", reg_wodo6_msg));
    ASSERT_TRUE(this->test_call(ei, ".wodn6test", reg_wodn6_msg));
    ASSERT_TRUE(this->test_call(ei, ".wo6 test", reg_wodo6_msg));
    ASSERT_TRUE(this->test_call(ei, ".wn6test", reg_wodn6_msg));
}

TEST_F(entry_test, roll_fate) {
    ::event_info ei(123456, 10000, ::event_type::group);
    ei.nickname = "dynilath";

    std::regex reg_fate(u8"^ \\* dice 掷骰: FATE = \\[([o+\\-] ){3}[o+\\-]\\] = -?\\d");
    std::regex reg_fate1(u8"^ \\* dice 掷骰: FATE \\+1 = \\[([o+\\-] ){3}[o+\\-]\\] \\+ 1 = -?\\d");
    std::regex reg_fate_msg(u8"^ \\* dice test 掷骰: FATE = \\[([o+\\-] ){3}[o+\\-]\\] = -?\\d");
    std::regex reg_fate1_msg(
        u8"^ \\* dice test 掷骰: FATE \\+1 = \\[([o+\\-] ){3}[o+\\-]\\] \\+ 1 = "
        u8"-?\\d");

    this->base_call(ei, ".ndice");
    ASSERT_TRUE(this->test_call(ei, ".f", reg_fate));
    ASSERT_TRUE(this->test_call(ei, ".f+1", reg_fate1));
    ASSERT_TRUE(this->test_call(ei, ".fate", reg_fate));
    ASSERT_TRUE(this->test_call(ei, ".fate+1", reg_fate1));
    ASSERT_TRUE(this->test_call(ei, ".ftest", reg_fate_msg));
    ASSERT_TRUE(this->test_call(ei, ".f+1test", reg_fate1_msg));
    ASSERT_TRUE(this->test_call(ei, ".fate test", reg_fate_msg));
    ASSERT_TRUE(this->test_call(ei, ".fate+1 test", reg_fate1_msg));
}

TEST_F(entry_test, manual_dice) {
    ::event_info ei(123456, 10000, ::event_type::group);
    ei.nickname = "dynilath";

    std::string cur_state_4 = "\\| 当前状态: (\\d\\(6\\) \\+ ){3}\\d\\(6\\) = \\d{1,2}";
    std::string cur_state_3 = "\\| 当前状态: (\\d\\(6\\) \\+ ){2}\\d\\(6\\) = \\d{1,2}";

    std::regex reg_init(std::string(u8"^ \\* dice 在桌上放了这些骰子: 4d6 ") + cur_state_4);
    std::regex reg_r4(std::string(u8"^ \\* dice 重骰桌上的第4个骰子 ") + cur_state_4);
    std::regex reg_k4(std::string(u8"^ \\* dice 杀死桌上的第4个骰子 ") + cur_state_3);
    std::regex reg_ka(" \\* dice 杀掉了所有的骰子 \\| 当前状态: 没有骰子了");

    this->base_call(ei, ".ndice");
    ASSERT_TRUE(this->test_call(ei, ".m 4d6", reg_init));
    ASSERT_TRUE(this->test_call(ei, ".mr4", reg_r4));
    ASSERT_TRUE(this->test_call(ei, ".mr4", reg_r4));
    ASSERT_TRUE(this->test_call(ei, ".mk4", reg_k4));
    ASSERT_TRUE(this->test_call(ei, ".mka", reg_ka));
}

TEST_F(entry_test, poker) {
    ::event_info ei(123456, 10000, ::event_type::group);
    ei.nickname = "dynilath";

    this->base_call(ei, ".ndice");
    std::regex re_start(" \\* dice 已初始化牌库，总计4张牌");
    ASSERT_TRUE(this->test_call(ei, ".p init a,b,c,d", re_start));

    std::string prefix = "^ \\* dice 抽出了 ([abcd])";
    std::regex re_draw_off(prefix + " \\| 牌堆剩余\\d张");
    std::regex re_draw_two("^ \\* dice 抽出了 ([abcd], [abcd]) \\| 牌堆剩余\\d张");
    std::regex re_draw_three("^ \\* dice 抽出了 ([abcd], [abcd], [abcd]) \\| 牌堆剩余\\d张");
    std::regex re_draw1(prefix + " \\| 牌堆剩余3张，已经抽出了: \\1");
    std::regex re_draw2(prefix + " \\| 牌堆剩余2张，已经抽出了: [abcd], \\1");
    std::regex re_draw3(prefix + " \\| 牌堆剩余1张，已经抽出了: ([abcd], ){2}\\1");
    std::regex re_draw4(prefix + " \\| 牌堆剩余0张，已经抽出了: ([abcd], ){3}\\1");
    std::regex re_draw_out(" \\* dice 无牌可抽 \\| 牌堆剩余0张，已经抽出了: ([abcd], ){3}[abcd]");

    this->base_call(ei, ".rson");

    ASSERT_TRUE(this->test_call(ei, ".pd", re_draw1));
    ASSERT_TRUE(this->test_call(ei, ".poker draw", re_draw2));
    ASSERT_TRUE(this->test_call(ei, ".p draw", re_draw3));
    ASSERT_TRUE(this->test_call(ei, ".pdraw", re_draw4));
    ASSERT_TRUE(this->test_call(ei, ".p d", re_draw_out));

    ASSERT_TRUE(this->test_call(ei, ".pshuffle", std::regex(" \\* dice 已将牌堆重新切洗")));
    ASSERT_TRUE(this->test_call(ei, ".pd", re_draw1));
    ASSERT_TRUE(this->test_call(ei, ".poker draw", re_draw2));
    ASSERT_TRUE(this->test_call(ei, ".p draw", re_draw3));
    ASSERT_TRUE(this->test_call(ei, ".pdraw", re_draw4));

    this->base_call(ei, ".rsoff");
    ASSERT_TRUE(this->test_call(ei, ".pshuffle", std::regex(" \\* dice 已将牌堆重新切洗")));
    ASSERT_TRUE(this->test_call(ei, ".pd", re_draw_off));
    ASSERT_TRUE(this->test_call(ei, ".p d", re_draw_off));
    ASSERT_TRUE(this->test_call(ei, ".pdraw", re_draw_off));
    ASSERT_TRUE(this->test_call(ei, ".p draw", re_draw_off));

    ASSERT_TRUE(this->test_call(ei, ".pshuffle", std::regex(" \\* dice 已将牌堆重新切洗")));
    ASSERT_TRUE(this->test_call(ei, ".pd2", re_draw_two));

    ASSERT_TRUE(this->test_call(ei, ".pshuffle", std::regex(" \\* dice 已将牌堆重新切洗")));
    ASSERT_TRUE(this->test_call(ei, ".pd3", re_draw_three));
    ASSERT_TRUE(this->test_call(ei, ".pd3", re_draw_off));
}

TEST_F(entry_test, multiline_case_fullcmd) {
    ::event_info ei(123456, 10000, ::event_type::group);
    ei.nickname = "dynilath";

    this->base_call(ei, ".ndice");

    std::stringstream builder;
    builder << " \\* .+ 的新名字是 dice\\n"
            << " \\* dice 关闭骰子详细输出\\n"
            << " \\* dice test 掷骰: 2d20 \\+ 4 = \\d{1,2}\\n"
            << " \\* dice test 掷骰: CoC = \\d{1,3}\\n"
            << " \\* dice test 掷骰: FATE = \\[([o+\\-] ){3}[o+\\-]\\] = -?\\d";

    std::regex reg_out(builder.str());

    ASSERT_TRUE(this->test_call(ei, ".ndice\r\n.rsoff\r\n.r2d20+4test\r\n.ctest\r\n.ftest", reg_out));
    ASSERT_TRUE(this->test_call(ei, ".n  dice\r\n  . r  s off  \r\n . r 2d20 + 4 test \r\n . c test \r\n . f test", reg_out));
    ASSERT_TRUE(this->test_call(
        ei, ".name  dice\r\n  . roll source off  \r\n . roll 2d20 + 4 test \r\n . coc test \r\n . fate test", reg_out));
    ASSERT_TRUE(this->test_call(
        ei, ".name dice \r\n  . ROLL source off  \r\n . roLL 2d20 + 4 test \r\n . cOc test \r\n . FATE test", reg_out));
}

TEST_F(entry_test, range_exceed) {
    ::event_info ei(123456, 10000, ::event_type::group);
    ei.nickname = "dynilath";

    this->base_call(ei, ".ndice");

    ASSERT_TRUE(this->test_call(ei, ".r100d6", std::regex(" \\* dice 投掷过多骰子，最大为50")));
    ASSERT_TRUE(this->test_call(ei, ".r1d1001", std::regex(" \\* dice 投掷骰子面数过多，最大为1000")));
    ASSERT_TRUE(this->test_call(ei, ".r(-1)d10", std::regex(" \\* dice 骰子的数量、面数、重复次数必须为正整数")));
    ASSERT_TRUE(this->test_call(ei, ".r1d(3.6)", std::regex(" \\* dice 骰子的数量、面数、重复次数必须为正整数")));
    ASSERT_TRUE(this->test_call(ei, ".r1d10k(-2)", std::regex(" \\* dice 骰子的数量、面数、重复次数必须为正整数")));
    ASSERT_TRUE(
        this->test_call(ei, ".r12345678901234567890123456789012345678901234567890d6", std::regex(" \\* dice 数值超出计算范围")));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#ifdef _WIN32
#ifdef _DEBUG
#pragma comment(lib, "gtestd.lib")
#else
#pragma comment(lib, "gtest.lib")
#endif
#endif

#include <climits>
#include <cmath>
#include <functional>
#include <vector>

#include "gtest/gtest.h"

#include "./dicebot/dice_excepts.h"
#include "./dicebot/dice_roller.h"
#include "./dicebot/entity/manual_dice.h"

namespace dicebot::test {

#pragma region direct roll test

    double fun_chi_square(std::vector<int> sample, std::vector<int> expect) {
        if (sample.size() != expect.size()) return INFINITY;
        double sum_sample = 0.0;
        for (size_t i = 0; i < sample.size(); i++) {
            sum_sample += sample[i];
        }
        double sum_expect = 0.0;
        for (size_t i = 0; i < expect.size(); i++) {
            sum_expect += expect[i];
        }

        double chi_square = 0.0;

        int re_type = sample.size() - 1;
        while (re_type >= 0) {
            double val_expect = expect[re_type] * sum_sample / sum_expect;
            double fi_expect = sample[re_type] - val_expect;
            chi_square += ((fi_expect * fi_expect) / val_expect);
            re_type--;
        }
        return chi_square;
    }

    void generate_roll_result(std::vector<int>& result_container, int sample_size, int max_val, int min_val,
                              std::function<void(dicebot::roll::dice_roll&)> func) {
        result_container.assign(max_val - min_val + 1, 0);
        int repeat = sample_size;
        while (repeat--) {
            dicebot::roll::dice_roll dr;
            func(dr);
            result_container[dr.summary - min_val]++;
            ASSERT_GE(dr.summary, min_val);
            ASSERT_LE(dr.summary, max_val);
        }
    }

    class dice_result_structor {
    public:
        std::vector<uint16_t> d_vals;
        uint16_t max;
        dice_result_structor(uint16_t num, uint16_t max) {
            d_vals.assign(num, 1);
            this->max = max;
        }

        bool increase() {
            bool ret = false;
            for (uint16_t i = 0; i < d_vals.size(); i++) {
                if (d_vals[i] < max) {
                    d_vals[i]++;
                    ret = true;
                    break;
                } else {
                    d_vals[i] = 1;
                }
            }
            return ret;
        }
    };

    TEST(RollTest, Base_01_d100) {
        int sample_sum = (4000);

        int min_val = 1;
        int max_val = 100;

        std::vector<int> result;
        generate_roll_result(
            result, sample_sum, max_val, min_val, [](dicebot::roll::dice_roll& dr) { dicebot::roll::roll_base(dr, 1, 100); });

        std::vector<int> compare;
        compare.assign(result.size(), 1);
        double chi_square = fun_chi_square(result, compare);

        // chi-square check with 0.1% of coherence
        double chi_01_percent = 148.2303592;
        ASSERT_LT(chi_square, chi_01_percent);
    }

    TEST(RollTest, Base_03_2d6) {
        int sample_sum = (1200);

        int min_val = 2;
        int max_val = 12;

        std::vector<int> result;
        generate_roll_result(
            result, sample_sum, max_val, min_val, [](dicebot::roll::dice_roll& dr) { dicebot::roll::roll_base(dr, 2, 6); });

        std::vector<int> compare;
        compare.assign(result.size(), 0);
        for (int i = 1; i <= 6; i++) {
            for (int j = 1; j <= 6; j++) {
                compare[i + j - min_val]++;
            }
        }

        double chi_square = fun_chi_square(result, compare);
        // chi-square check with 0.1% of coherence
        double chi_01_percent = 31.26413362;
        ASSERT_LT(chi_square, chi_01_percent);
    }

    TEST(RollTest, RDK_01_4d6k3) {
        int sample_sum = (1200);

        int min_val = 3;
        int max_val = 18;

        std::vector<int> result;
        generate_roll_result(
            result, sample_sum, max_val, min_val, [](dicebot::roll::dice_roll& dr) { dicebot::roll::roll_rdk(dr, 4, 6, 3); });

        std::vector<int> compare;
        compare.assign(result.size(), 0);
        for (int i = 1; i <= 6; i++) {
            for (int j = 1; j <= 6; j++) {
                for (int k = 1; k <= 6; k++) {
                    for (int l = 1; l <= 6; l++) {
                        int min = i;
                        min = min > j ? j : min;
                        min = min > k ? k : min;
                        min = min > l ? l : min;
                        compare[i + j + k + l - min - min_val]++;
                    }
                }
            }
        }

        double chi_square = fun_chi_square(result, compare);
        // chi-square check with 0.1% of coherence
        double chi_01_percent = 40.79021671;
        ASSERT_LT(chi_square, chi_01_percent);
    }

    TEST(RollTest, RDK_02_4d6kl3) {
        int sample_sum = (1200);

        int min_val = 3;
        int max_val = 18;

        std::vector<int> result;
        generate_roll_result(
            result, sample_sum, max_val, min_val, [](dicebot::roll::dice_roll& dr) { dicebot::roll::roll_rdk(dr, 4, 6, -3); });

        std::vector<int> compare;
        compare.assign(result.size(), 0);
        for (int i = 1; i <= 6; i++) {
            for (int j = 1; j <= 6; j++) {
                for (int k = 1; k <= 6; k++) {
                    for (int l = 1; l <= 6; l++) {
                        int max = i;
                        max = max < j ? j : max;
                        max = max < k ? k : max;
                        max = max < l ? l : max;
                        compare[i + j + k + l - max - min_val]++;
                    }
                }
            }
        }

        double chi_square = fun_chi_square(result, compare);
        // chi-square check with 0.1% of coherence
        double chi_01_percent = 40.79021671;
        ASSERT_LT(chi_square, chi_01_percent);
    }

    TEST(RollTest, COC_01_C) {
        int sample_sum = (4000);

        int min_val = 1;
        int max_val = 100;

        std::vector<int> result;
        generate_roll_result(
            result, sample_sum, max_val, min_val, [](dicebot::roll::dice_roll& dr) { dicebot::roll::roll_coc(dr, 0); });

        std::vector<int> compare;
        compare.assign(max_val - min_val + 1, 1);
        double chi_square = fun_chi_square(result, compare);
        // chi-square check with 0.1% of coherence
        double chi_01_percent = 148.2303592;
        ASSERT_LT(chi_square, chi_01_percent);
    }

    TEST(RollTest, COC_02_CP2) {
        int sample_sum = (4000);

        int max_val = 100;
        int min_val = 1;

        std::vector<int> result;
        generate_roll_result(
            result, sample_sum, max_val, min_val, [](dicebot::roll::dice_roll& dr) { dicebot::roll::roll_coc(dr, 2); });

        std::vector<int> compare;
        compare.assign(max_val - min_val + 1, 0);
        for (int i = 1; i <= 10; i++) {
            for (int j = 1; j <= 10; j++) {
                for (int k = 1; k <= 10; k++) {
                    for (int l = 1; l <= 10; l++) {
                        int pj = j;
                        int pk = k;
                        int pl = l;
                        if (i != 0) {
                            if (j == 10) pj = 0;
                            if (k == 10) pk = 0;
                            if (l == 10) pl = 0;
                        }
                        int min = pj;
                        min = min > pk ? pk : min;
                        min = min > pl ? pl : min;
                        compare[i + min * 10 - min_val]++;
                    }
                }
            }
        }

        double chi_square = fun_chi_square(result, compare);

        // chi-square check with 0.1% of coherence
        double chi_01_percent = 148.2303592;
        ASSERT_LT(chi_square, chi_01_percent);
    }

    template <class exception_t, class func_t, class... Args>
    bool limit_test(func_t func, Args... args...) {
        try {
            func(args...);
            return false;
        } catch (exception_t e) {
            return true;
        } catch (...) {
            return false;
        }
    };

    TEST(RollTest, DICE_LIMIT) {
        dicebot::roll::dice_roll dr;

        ASSERT_FALSE(limit_test<std::exception>(dicebot::roll::roll_base, dr, 1, 6));

        ASSERT_TRUE(limit_test<dice_exceed>(dicebot::roll::roll_base, dr, MAX_DICE_NUM + 1, 2));

        ASSERT_TRUE(limit_test<face_exceed>(dicebot::roll::roll_base, dr, 1, MAX_DICE_FACE + 1));

        /*
        ASSERT_TRUE(limit_test<dice_exceed>(
            dicebot::roll::roll_rdk, dr, MAX_DICE_NUM + 1, 2, 1));

        ASSERT_TRUE(limit_test<face_exceed>(
            dicebot::roll::roll_rdk, dr, 1, MAX_DICE_FACE + 1, 1));

        ASSERT_TRUE(limit_test<dice_exceed>(
            dicebot::roll::roll_rdk, dr, 1, 6, MAX_DICE_NUM + 1));

        ASSERT_TRUE(limit_test<dice_exceed>(
            dicebot::roll::roll_coc, dr, -MAX_DICE_NUM - 1));

        ASSERT_TRUE(limit_test<dice_exceed>(
            dicebot::roll::roll_coc, dr, MAX_DICE_NUM + 1));*/
    }

    TEST(RollTest, WOD_01_O_5D8) {
        int sample_sum = (1000);

        int min_val = 0;
        int max_val = 5;

        std::vector<int> result;
        generate_roll_result(result, sample_sum, max_val, min_val, [](dicebot::roll::dice_roll& dr) {
            dicebot::roll::roll_wod(dr, 5, 8, 11, true);
        });

        std::vector<int> compare;
        compare.assign(max_val - min_val + 1, 0);
        dice_result_structor drs(max_val, 10);
        while (drs.increase()) {
            int result = 0;
            for (uint16_t i = 0; i < drs.d_vals.size(); i++) {
                int t_r = drs.d_vals[i] >= 8 ? 1 : (drs.d_vals[i] == 1 ? -1 : 0);
                result += t_r;
            }
            if (result < 0) result = 0;
            compare[result]++;
        }

        double chi_square = fun_chi_square(result, compare);
        // chi-square check with 0.1% of coherence
        double chi_01_percent = 20.51500565;
        ASSERT_LT(chi_square, chi_01_percent);
    }

    TEST(RollTest, FATE_01) {
        int sample_sum = (1000);

        int min_val = -4;
        int max_val = 4;

        std::vector<int> result;
        result.assign(9, 0);
        int t = sample_sum;
        while (t--) {
            roll::dice_roll dr;
            roll::roll_fate(dr, 0);
            result[dr.summary + 4]++;
        }

        std::vector<int> compare;
        compare.assign(9, 0);
        dice_result_structor drs(4, 3);
        do {
            int result = 0;
            for (uint16_t i = 0; i < drs.d_vals.size(); i++) {
                result += drs.d_vals[i];
            }
            result -= 4;
            compare[result]++;
        } while (drs.increase());

        double chi_square = fun_chi_square(result, compare);
        // chi-square check with 0.1% of coherence
        double chi_01_percent = 16.2662361962381;
        ASSERT_LT(chi_square, chi_01_percent);
    }

    TEST(ManualDice, TEST_01_4d6_1d8) {
        int sample_sum = (400);

        int max_val = 18;
        int min_val = 3;

        std::vector<int> result;
        result.assign(max_val - min_val + 1, 0);

        int repeat = sample_sum;
        while (repeat--) {
            manual::manual_dice md;
            md.add({6, 6, 6, 6, 8});
            int result = 0;
            result = md.i_sum_result;
            ASSERT_GE(result, 5);
            ASSERT_LE(result, 32);

            md.kill(1);
            result = md.i_sum_result;
            ASSERT_GE(result, 4);
            ASSERT_LE(result, 26);

            md.kill(4);
            result = md.i_sum_result;
            ASSERT_GE(result, 3);
            ASSERT_LE(result, 18);

            md.add({6, 6});
            result = md.i_sum_result;
            ASSERT_GE(result, 5);
            ASSERT_LE(result, 30);

            md.killall();
            result = md.i_sum_result;
            ASSERT_EQ(result, 0);
        }
    }

    TEST(ManualDice, TEST_01_4D6_1D8) {
        int sample_sum = (400);

        int max_val = 18;
        int min_val = 3;

        std::vector<int> result;
        result.assign(max_val - min_val + 1, 0);

        int repeat = sample_sum;
        while (repeat--) {
            manual::manual_dice md;
            md.add({6, 6, 6, 6, 8});
            int result = 0;
            result = md.i_sum_result;
            ASSERT_GE(result, 5);
            ASSERT_LE(result, 32);

            md.kill(1);
            result = md.i_sum_result;
            ASSERT_GE(result, 4);
            ASSERT_LE(result, 26);

            md.kill(4);
            result = md.i_sum_result;
            ASSERT_GE(result, 3);
            ASSERT_LE(result, 18);

            md.add({6, 6});
            result = md.i_sum_result;
            ASSERT_GE(result, 5);
            ASSERT_LE(result, 30);

            md.killall();
            result = md.i_sum_result;
            ASSERT_EQ(result, 0);
        }
    }
#pragma endregion
} // namespace dicebot::test

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
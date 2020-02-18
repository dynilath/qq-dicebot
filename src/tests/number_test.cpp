#include <climits>
#include <cmath>

#include "../dicebot/utils/number.h"
#include "gtest/gtest.h"

namespace dicebot::test {

#pragma region number test
    template <class T>
    struct add {
        inline T operator()(T a, T b) { return a + b; }
    };

    template <class T>
    struct sub {
        inline T operator()(T a, T b) { return a - b; }
    };

    template <class T>
    struct mul {
        inline T operator()(T a, T b) { return a * b; }
    };

    template <class T>
    struct dvi {
        inline T operator()(T a, T b) { return a / b; }
    };

    template <template <class> class oper, class v1_t, class v2_t>
    void ceil_check(v1_t val1, v2_t val2) {
        dicebot::number ret = oper<dicebot::number>()(val1, val2);

        oper<double> double_oper;

        std::string compare = std::to_string(double_oper((double)val1, (double)val2));
        int pos = compare.find_last_not_of('0');
        if (pos != std::string::npos)
            compare.assign(compare.substr(0, pos + 1));
        else
            compare = std::to_string(double_oper((double)val1, (double)val2));
        ASSERT_EQ(std::string(ret), compare);
        ASSERT_EQ(ret.is_using_int, false);
    }

    template <template <class T> class oper, class v1_t, class v2_t>
    void case_check(v1_t val1, v2_t val2, bool is_int) {
        dicebot::number num1 = val1;
        dicebot::number num2 = val2;
        auto ret = oper<dicebot::number>()(num1, num2);
        std::string compare;

        oper<double> double_oper;

        if (is_int)
            compare = std::to_string(std::lround(double_oper((double)val1, (double)val2)));
        else {
            compare = std::to_string(double_oper((double)val1, (double)val2));
            int pos = compare.find_last_not_of('0');
            if (pos != std::string::npos) compare.assign(compare.substr(0, pos + 1));
        }
        ASSERT_EQ(std::string(ret), compare);
        ASSERT_EQ(ret.is_using_int, is_int);
    }

} // namespace dicebot::test

TEST(number_test, add_test) {
    dicebot::test::case_check<dicebot::test::add>(12, 1234, true);
    dicebot::test::case_check<dicebot::test::add>(12, 123.4f, false);
    dicebot::test::case_check<dicebot::test::add>(123.4f, 12, false);
    dicebot::test::case_check<dicebot::test::add>(123.4f, 12.6f, true);
    dicebot::test::ceil_check<dicebot::test::add>(INT_MAX, 1);
    dicebot::test::ceil_check<dicebot::test::add>(-INT_MAX, -11);
}

TEST(number_test, sub_test) {
    dicebot::test::case_check<dicebot::test::sub>(1234, 12, true);
    dicebot::test::case_check<dicebot::test::sub>(1234, 12.3f, false);
    dicebot::test::case_check<dicebot::test::sub>(123.4f, 12, false);
    dicebot::test::case_check<dicebot::test::sub>(123.4f, 12.4f, true);
    dicebot::test::ceil_check<dicebot::test::sub>(INT_MAX, -5);
    dicebot::test::ceil_check<dicebot::test::sub>(-INT_MAX, 11);
}

TEST(number_test, product_test) {
    dicebot::test::case_check<dicebot::test::mul>(12, 12, true);
    dicebot::test::case_check<dicebot::test::mul>(12, 12.3f, false);
    dicebot::test::case_check<dicebot::test::mul>(25.4f, 12, false);
    dicebot::test::case_check<dicebot::test::mul>(2.5f, 0.4f, true);
    dicebot::test::ceil_check<dicebot::test::mul>(INT_MAX, 5);
    dicebot::test::ceil_check<dicebot::test::mul>(INT_MAX, -3);
    dicebot::test::ceil_check<dicebot::test::mul>(-INT_MAX, -5);
    dicebot::test::ceil_check<dicebot::test::mul>(-INT_MAX, 3);
}

TEST(number_test, divide_test) {
    dicebot::test::case_check<dicebot::test::dvi>(48, 12, true);
    dicebot::test::case_check<dicebot::test::dvi>(12, 12.3f, false);
    dicebot::test::case_check<dicebot::test::dvi>(25.4f, 12, false);
    dicebot::test::case_check<dicebot::test::dvi>(3.6f, 1.2f, true);
    dicebot::test::ceil_check<dicebot::test::dvi>(INT_MAX, 5);
    dicebot::test::ceil_check<dicebot::test::dvi>(INT_MAX, -3);
    dicebot::test::ceil_check<dicebot::test::dvi>(-INT_MAX, -5);
    dicebot::test::ceil_check<dicebot::test::dvi>(-INT_MAX, 3);
    dicebot::test::case_check<dicebot::test::dvi>(32, 12, false);
}
#pragma endregion

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#ifdef _DEBUG
#pragma comment(lib,"gtestd.lib")
#elif
#pragma comment(lib,"gtest.lib")
#endif

#include "gtest/gtest.h"
#include "dicebot/number.h"

namespace dicebot::test{

#define EASY_CASE_HACK( _Val1, _Oper, _Val2, _Is_Int) {dicebot::number num1 = _Val1;\
dicebot::number ret = num1 _Oper _Val2;\
std::string compare;\
if(_Is_Int) compare = std::to_string((int32_t)(((double)_Val1) _Oper ((double)_Val2)));\
else compare = std::to_string(((double)_Val1) _Oper ((double)_Val2));\
ASSERT_EQ(ret.str(), compare);\
ASSERT_EQ(ret.is_using_int,_Is_Int);}

#define EASY_CEIL_HACK( _Val1, _Oper, _Val2) {dicebot::number num1 = _Val1;\
int c = _Val2;\
dicebot::number ret = num1 _Oper c;\
double p = (double)c _Oper _Val2;\
ASSERT_EQ(ret.str(), std::to_string(((double)_Val1) _Oper ((double)_Val2)));\
ASSERT_EQ(ret.is_using_int,false);}



    TEST(NumberTest, Add_01_Int_Int){EASY_CASE_HACK( 12, + , 1234, true);}
    TEST(NumberTest, Add_02_Int_Float){EASY_CASE_HACK( 12, +, 123.4, false);}
    TEST(NumberTest, Add_03_Float_Int){EASY_CASE_HACK( 123.4, +, 12, false);}
    TEST(NumberTest, Add_04_Float_Float){EASY_CASE_HACK( 123.4, +, 12.6, true);}
    TEST(NumberTest, Add_05_MAX_UP){EASY_CEIL_HACK(INT_MAX,+,1);}
    TEST(NumberTest, Add_05_MAX_DOWN){EASY_CEIL_HACK(-INT_MAX,+,-11);}

    TEST(NumberTest, Minus_01_Int_Int){EASY_CASE_HACK( 1234, - , 12, true);}
    TEST(NumberTest, Minus_02_Int_Float){EASY_CASE_HACK( 1234, -, 12.3, false);}
    TEST(NumberTest, Minus_03_Float_Int){EASY_CASE_HACK( 123.4, -, 12, false);}
    TEST(NumberTest, Minus_04_Float_Float){EASY_CASE_HACK( 123.4, -, 12.4, true);}
    TEST(NumberTest, Minus_05_MAX_UP){EASY_CEIL_HACK(INT_MAX,-,-5);}
    TEST(NumberTest, Minus_06_MAX_DOWN){EASY_CEIL_HACK(-INT_MAX,-,11);}

    TEST(NumberTest, Product_01_Int_Int){EASY_CASE_HACK( 12, * , 12, true);}
    TEST(NumberTest, Product_02_Int_Float){EASY_CASE_HACK( 12, *, 12.3, false);}
    TEST(NumberTest, Product_03_Float_Int){EASY_CASE_HACK( 25.4, *, 12, false);}
    TEST(NumberTest, Product_04_Float_Float){EASY_CASE_HACK( 2.5, *, 0.4, true);}
    TEST(NumberTest, Product_05_MAX_A_UP){EASY_CEIL_HACK(INT_MAX,*,5);}
    TEST(NumberTest, Product_06_MAX_A_DOWN){EASY_CEIL_HACK(INT_MAX,*,-3);}
    TEST(NumberTest, Product_05_MAX_B_UP){EASY_CEIL_HACK(-INT_MAX,*,-5);}
    TEST(NumberTest, Product_06_MAX_B_DOWN){EASY_CEIL_HACK(-INT_MAX,*,3);}

    TEST(NumberTest, Divide_01_Int_Int){EASY_CASE_HACK( 48, / , 12, true);}
    TEST(NumberTest, Divide_02_Int_Float){EASY_CASE_HACK( 12, /, 12.3, false);}
    TEST(NumberTest, Divide_03_Float_Int){EASY_CASE_HACK( 25.4, /, 12, false);}
    TEST(NumberTest, Divide_04_Float_Float){EASY_CASE_HACK( 3.6, /, 1.2, true);}
    TEST(NumberTest, Divide_05_MAX_A_UP){EASY_CEIL_HACK(INT_MAX,/,5);}
    TEST(NumberTest, Divide_06_MAX_A_DOWN){EASY_CEIL_HACK(INT_MAX,/,-3);}
    TEST(NumberTest, Divide_05_MAX_B_UP){EASY_CEIL_HACK(-INT_MAX,/,-5);}
    TEST(NumberTest, Divide_06_MAX_B_DOWN){EASY_CEIL_HACK(-INT_MAX,/,3);}
    TEST(NumberTest, Divide_07_Int_Int){EASY_CASE_HACK( 32, / , 12, false);}

} // test

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
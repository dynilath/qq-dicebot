#ifdef _DEBUG
#pragma comment(lib,"gtestd.lib")
#else
#pragma comment(lib,"gtest.lib")
#endif

#include "gtest/gtest.h"
#include "dicebot/number.h"
#include "dicebot/dice_roller.h"

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


    TEST(RollTest, Base_01){
        dicebot::roll::dice_roll dr1 = dicebot::roll::roll_base(40,20);
        dicebot::roll::dice_roll dr2 = dicebot::roll::roll_base(40,20);
        dicebot::roll::dice_roll dr3 = dicebot::roll::roll_base(40,20);
        dicebot::roll::dice_roll dr4 = dicebot::roll::roll_base(40,20);
        dicebot::roll::dice_roll dr5 = dicebot::roll::roll_base(40,20);
        long sum = dr1.result+dr2.result+dr3.result+dr4.result+dr5.result;
        double aver = (sum) / 200.0;
        EXPECT_GT(aver,9.8);
        EXPECT_LT(aver,11.2);
        ASSERT_GT(aver,1.0);
        ASSERT_LT(aver,20.0);
    }

#define GENERATE_ROLL_RESULT(_list,_sample,_max,_min,_roll_method){\
_list.assign(_max-_min+1,0);\
int repeat = _sample;\
while(repeat--){\
dicebot::roll::dice_roll dr = _roll_method;\
_list[dr.result-_min]++;\
ASSERT_GE(dr.result,_min);\
ASSERT_LE(dr.result,_max);}}

#define CAL_CHI_SQUARE_SINGLE(_list_result, _compare, _output){\
int re_type = _list_result.size()-1;\
while(re_type >= 0){\
double expect = _compare;\
double fi_expect = _list_result[re_type] - expect;\
_output += ((fi_expect*fi_expect)/expect);\
re_type --;}}

#define CAL_CHI_SQUARE_LIST(_list_result, _list_compare, _sum_compare, _sum_sample, _output){\
int re_type = _list_result.size()-1;\
while(re_type >= 0){\
double expect = ((double)_list_compare[re_type])*_sum_sample/_sum_compare;\
double fi_expect = _list_result[re_type] - expect;\
_output += ((fi_expect*fi_expect)/expect);\
re_type --;}}

    TEST(RollTest, Base_02_d100){
        int dice_count = 1;
        int face_count = 100;
        int sample_sum = (4000);
        std::vector<int> result;
        GENERATE_ROLL_RESULT(
            result,
            sample_sum,
            dice_count*face_count,
            dice_count,
            dicebot::roll::roll_base(dice_count,face_count));

        double chi_square = 0.0;
        double compare = ((double)sample_sum) / face_count;
        CAL_CHI_SQUARE_SINGLE(result,compare,chi_square);
        //chi-square check with 0.1% of coherence
        double chi_01_percent = 148.2303592;
        ASSERT_LT(chi_square,chi_01_percent);
    }
    
    TEST(RollTest, Base_03_2d6){
        int sample_sum = (1200);

        int dice_count = 2;
        int face_count = 6;

        std::vector<int> result;
        GENERATE_ROLL_RESULT(
            result,
            sample_sum,
            dice_count*face_count,
            dice_count,
            dicebot::roll::roll_base(dice_count,face_count));

        std::vector<int> compare;
        compare.assign(result.size(),0);
        for(int i =1;i<=face_count;i++){
            for(int j =1;j<=face_count;j++){
                compare[i+j-dice_count]++;
            }
        }
        double compare_counter = 0.0;
        for(unsigned int i=0;i<compare.size();i++)
            compare_counter += compare[i];

        double chi_square = 0.0;
        CAL_CHI_SQUARE_LIST(result, compare, compare_counter, sample_sum, chi_square);
        //chi-square check with 0.1% of coherence
        double chi_01_percent = 31.26413362;
        ASSERT_LT(chi_square,chi_01_percent);
    }
    
    TEST(RollTest, RDK_01_4d6k3){
        int sample_sum = (1200);

        int dice_count = 3;
        int face_count = 6;

        std::vector<int> result;
        GENERATE_ROLL_RESULT(
            result,
            sample_sum,
            dice_count*face_count,
            dice_count,
            dicebot::roll::roll_rdk(4,face_count,dice_count));

        std::vector<int> compare;
        compare.assign(result.size(),0);
        for(int i =1;i<=face_count;i++){
            for(int j =1;j<=face_count;j++){
                for(int k =1;k<=face_count;k++){
                    for(int l =1;l<=face_count;l++){
                        int min = i;
                        min = min>j?j:min;
                        min = min>k?k:min;
                        min = min>l?l:min;
                        compare[i+j+k+l-min-dice_count]++;
                    }
                }
            }
        }
        double compare_counter = 0.0;
        for(unsigned int i=0;i<compare.size();i++)
            compare_counter += compare[i];

        double chi_square = 0.0;
        CAL_CHI_SQUARE_LIST(result, compare, compare_counter, sample_sum, chi_square);
        //chi-square check with 0.1% of coherence
        double chi_01_percent = 40.79021671;
        ASSERT_LT(chi_square,chi_01_percent);
    }
    
    TEST(RollTest, RDK_02_4d6kl3){
        int sample_sum = (1200);

        int dice_count = 3;
        int face_count = 6;

        std::vector<int> result;
        GENERATE_ROLL_RESULT(
            result,
            sample_sum,
            dice_count*face_count,
            dice_count,
            dicebot::roll::roll_rdk(4,face_count,-dice_count));

        std::vector<int> compare;
        compare.assign(result.size(),0);
        for(int i =1;i<=face_count;i++){
            for(int j =1;j<=face_count;j++){
                for(int k =1;k<=face_count;k++){
                    for(int l =1;l<=face_count;l++){
                        int min = i;
                        min = min<j?j:min;
                        min = min<k?k:min;
                        min = min<l?l:min;
                        compare[i+j+k+l-min-dice_count]++;
                    }
                }
            }
        }
        double compare_counter = 0.0;
        for(unsigned int i=0;i<compare.size();i++)
            compare_counter += compare[i];

        double chi_square = 0.0;
        CAL_CHI_SQUARE_LIST(result, compare, compare_counter, sample_sum, chi_square);
        //chi-square check with 0.1% of coherence
        double chi_01_percent = 40.79021671;
        ASSERT_LT(chi_square,chi_01_percent);
    }
    
    TEST(RollTest, COC_01_C){
        int sample_sum = (4000);

        int dice_count = 1;
        int face_count = 100;

        int max_val = dice_count * face_count;
        int min_val = dice_count;
        int result_type_count = max_val - min_val + 1;

        std::vector<int> result;
        GENERATE_ROLL_RESULT(
            result,
            sample_sum,
            dice_count*face_count,
            dice_count,
            dicebot::roll::roll_coc(0));

        double chi_square = 0.0;
        double compare = ((double)sample_sum) / face_count;
        CAL_CHI_SQUARE_SINGLE(result,compare,chi_square);
        //chi-square check with 0.1% of coherence
        double chi_01_percent = 148.2303592;
        ASSERT_LT(chi_square,chi_01_percent);
    }
    
    TEST(RollTest, COC_02_CP2){
        int sample_sum = (4000);

        int dice_count = 1;
        int face_count = 100;

        int max_val = dice_count * face_count;
        int min_val = dice_count;
        int result_type_count = max_val - min_val + 1;

        std::vector<int> result;
        GENERATE_ROLL_RESULT(
            result,
            sample_sum,
            dice_count*face_count,
            dice_count,
            dicebot::roll::roll_coc(2));

        std::vector<int> compare;
        compare.assign(result_type_count,0);
        for(int i =1;i<=10;i++){
            for(int j =1;j<=10;j++){
                for(int k =1;k<=10;k++){
                    for(int l =1;l<=10;l++){
                        int pj = j;
                        int pk = k;
                        int pl = l;
                        if(i != 0){
                            if(j==10) pj = 0;
                            if(k==10) pk = 0;
                            if(l==10) pl = 0;
                        }
                        int min = pj;
                        min = min>pk?pk:min;
                        min = min>pl?pl:min;
                        compare[i+min*10-min_val]++;
                    }
                }
            }
        }
        double compare_counter = 0.0;
        for(unsigned int i=0;i<compare.size();i++)
            compare_counter += compare[i];

        double chi_square = 0.0;
        CAL_CHI_SQUARE_LIST(result, compare, compare_counter, sample_sum, chi_square);
        //chi-square check with 0.1% of coherence
        double chi_01_percent = 148.2303592;
        ASSERT_LT(chi_square,chi_01_percent);
    }
    
    TEST(RollTest, DICE_LIMIT){
        dicebot::roll::dice_roll dr = dicebot::roll::roll_base(1,6);
        ASSERT_EQ(dr.status,dicebot::roll::roll_status::FINISHED);

        dr = dicebot::roll::roll_base(MAX_DICE_NUM+1,2);
        ASSERT_EQ(dr.status,dicebot::roll::roll_status::TOO_MANY_DICE);

        dr = dicebot::roll::roll_base(1,MAX_DICE_FACE+1);
        ASSERT_EQ(dr.status,dicebot::roll::roll_status::TOO_MANY_DICE);

        dr = dicebot::roll::roll_rdk(MAX_DICE_NUM+1,2,1);
        ASSERT_EQ(dr.status,dicebot::roll::roll_status::TOO_MANY_DICE);

        dr = dicebot::roll::roll_rdk(1,MAX_DICE_FACE+1,1);
        ASSERT_EQ(dr.status,dicebot::roll::roll_status::TOO_MANY_DICE);

        dr = dicebot::roll::roll_rdk(1,6,MAX_DICE_NUM+1);
        ASSERT_EQ(dr.status,dicebot::roll::roll_status::FINISHED);

        dr = dicebot::roll::roll_coc(-MAX_DICE_NUM-1);
        ASSERT_EQ(dr.status,dicebot::roll::roll_status::TOO_MANY_DICE);

        dr = dicebot::roll::roll_coc(MAX_DICE_NUM+1);
        ASSERT_EQ(dr.status,dicebot::roll::roll_status::TOO_MANY_DICE);
    }

} // test

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#ifdef _DEBUG
#pragma comment(lib,"gtestd.lib")
#else
#pragma comment(lib,"gtest.lib")
#endif

#include <list>

#include "gtest/gtest.h"
#include "dicebot/number.h"
#include "dicebot/dice_roller.h"
#include "dicebot/dice_spliter.h"


namespace dicebot::test{

double fun_chi_square(std::vector<int> sample, std::vector<int> expect){
    if(sample.size()!= expect.size()) return INFINITY;
    double sum_sample = 0.0;
    for(size_t i =0;i<sample.size();i++){
        sum_sample += sample[i];
    }
    double sum_expect = 0.0;
    for(size_t i =0;i<expect.size();i++){
        sum_expect += expect[i];
    }

    double chi_square = 0.0;

    int re_type = sample.size() -1;
    while(re_type >= 0){
        double val_expect = expect[re_type]*sum_sample/sum_expect;
        double fi_expect = sample[re_type] - val_expect;
        chi_square += ((fi_expect*fi_expect)/val_expect);
        re_type --;
    }
    return chi_square;
}

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

#define GENERATE_ROLL_RESULT(_list,_sample,_max,_min,_roll_method){\
_list.assign(_max-_min+1,0);\
int repeat = _sample;\
while(repeat--){\
dicebot::roll::dice_roll dr = _roll_method;\
_list[dr.result-_min]++;\
ASSERT_GE(dr.result,_min);\
ASSERT_LE(dr.result,_max);}}

    TEST(RollTest, Base_01_d100){
        int sample_sum = (4000);

        int min_val = 1;
        int max_val = 100;

        std::vector<int> result;
        GENERATE_ROLL_RESULT(
            result,
            sample_sum,
            max_val,
            min_val,
            dicebot::roll::roll_base(1,100));

        std::vector<int> compare;
        compare.assign(result.size(),1);
        double chi_square = fun_chi_square(result, compare);

        //chi-square check with 0.1% of coherence
        double chi_01_percent = 148.2303592;
        ASSERT_LT(chi_square,chi_01_percent);
    }
    
    TEST(RollTest, Base_03_2d6){
        int sample_sum = (1200);

        int min_val = 2;
        int max_val = 12;

        std::vector<int> result;
        GENERATE_ROLL_RESULT(
            result,
            sample_sum,
            max_val,
            min_val,
            dicebot::roll::roll_base(2,6));

        std::vector<int> compare;
        compare.assign(result.size(),0);
        for(int i =1;i<=6;i++){
            for(int j =1;j<=6;j++){
                compare[i+j-min_val]++;
            }
        }

        double chi_square = fun_chi_square(result,compare);
        //chi-square check with 0.1% of coherence
        double chi_01_percent = 31.26413362;
        ASSERT_LT(chi_square,chi_01_percent);
    }
    
    TEST(RollTest, RDK_01_4d6k3){
        int sample_sum = (1200);

        int min_val = 3;
        int max_val = 18;

        std::vector<int> result;
        GENERATE_ROLL_RESULT(
            result,
            sample_sum,
            max_val,
            min_val,
            dicebot::roll::roll_rdk(4,6,3));

        std::vector<int> compare;
        compare.assign(result.size(),0);
        for(int i =1;i<=6;i++){
            for(int j =1;j<=6;j++){
                for(int k =1;k<=6;k++){
                    for(int l =1;l<=6;l++){
                        int min = i;
                        min = min>j?j:min;
                        min = min>k?k:min;
                        min = min>l?l:min;
                        compare[i+j+k+l-min-min_val]++;
                    }
                }
            }
        }

        double chi_square = fun_chi_square(result,compare);
        //chi-square check with 0.1% of coherence
        double chi_01_percent = 40.79021671;
        ASSERT_LT(chi_square,chi_01_percent);
    }
    
    TEST(RollTest, RDK_02_4d6kl3){
        int sample_sum = (1200);

        int min_val = 3;
        int max_val = 18;

        std::vector<int> result;
        GENERATE_ROLL_RESULT(
            result,
            sample_sum,
            max_val,
            min_val,
            dicebot::roll::roll_rdk(4,6,-3));

        std::vector<int> compare;
        compare.assign(result.size(),0);
        for(int i =1;i<=6;i++){
            for(int j =1;j<=6;j++){
                for(int k =1;k<=6;k++){
                    for(int l =1;l<=6;l++){
                        int max = i;
                        max = max<j?j:max;
                        max = max<k?k:max;
                        max = max<l?l:max;
                        compare[i+j+k+l-max-min_val]++;
                    }
                }
            }
        }

        double chi_square = fun_chi_square(result,compare);
        //chi-square check with 0.1% of coherence
        double chi_01_percent = 40.79021671;
        ASSERT_LT(chi_square,chi_01_percent);
    }
    
    TEST(RollTest, COC_01_C){
        int sample_sum = (4000);

        int min_val = 1;
        int max_val = 100;

        std::vector<int> result;
        GENERATE_ROLL_RESULT(
            result,
            sample_sum,
            max_val,
            min_val,
            dicebot::roll::roll_coc(0));

        std::vector<int> compare;
        compare.assign(max_val - min_val+1,1);
        double chi_square = fun_chi_square(result,compare);
        //chi-square check with 0.1% of coherence
        double chi_01_percent = 148.2303592;
        ASSERT_LT(chi_square,chi_01_percent);
    }
    
    TEST(RollTest, COC_02_CP2){
        int sample_sum = (4000);

        int max_val = 100;
        int min_val = 1;

        std::vector<int> result;
        GENERATE_ROLL_RESULT(
            result,sample_sum,
            max_val,min_val,
            dicebot::roll::roll_coc(2));

        std::vector<int> compare;
        compare.assign(max_val - min_val + 1,0);
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

        double chi_square = fun_chi_square(result,compare);

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
    
    TEST(DiceSPliter, TEST_01_2d6_1d4_1d8){
        int sample_sum = (2000);

        int max_val = 24;
        int min_val = 4;

        std::vector<int> result;
        result.assign(max_val-min_val+1,0);

        int repeat = sample_sum;
        while(repeat--){
            std::string scommand;
            std::string sdetail;
            std::string sresult;
            std::string smessage;
            dicebot::binary_tree_split_dice("2d6+1d4+1d8",scommand,sdetail,sresult,smessage);
            int res = std::stoi(sresult);
            ASSERT_GE(res,min_val);
            ASSERT_LE(res,max_val);
            result[res-min_val] ++;
        }

        std::vector<int> compare;
        compare.assign(result.size(),0);
        for(int i =1;i<=6;i++){
            for(int j =1;j<=6;j++){
                for(int k =1;k<=4;k++){
                    for(int l =1;l<=8;l++){
                        compare[i+j+k+l-min_val]++;
                    }
                }
            }
        }

        double chi_square = fun_chi_square(result,compare);

        //chi-square check with 0.1% of coherence
        double chi_01_percent = 49.72823247;
        ASSERT_LT(chi_square,chi_01_percent);
    }
    
    TEST(DiceSPliter, TEST_02_2D6K1_1D4_1D8){
        int sample_sum = (2000);

        int max_val = 18;
        int min_val = 3;

        std::vector<int> result;
        result.assign(max_val-min_val+1,0);

        int repeat = sample_sum;
        while(repeat--){
            std::string scommand;
            std::string sdetail;
            std::string sresult;
            std::string smessage;
            dicebot::binary_tree_split_dice("2D6K1+1D4+1D8",scommand,sdetail,sresult,smessage);
            int res = std::stoi(sresult);
            ASSERT_GE(res,min_val);
            ASSERT_LE(res,max_val);
            result[res-min_val] ++;
        }

        std::vector<int> compare;
        compare.assign(result.size(),0);
        for(int i =1;i<=6;i++){
            for(int j =1;j<=6;j++){
                int i_1 = i>j?i:j;
                for(int k =1;k<=4;k++){
                    for(int l =1;l<=8;l++){
                        compare[i_1+k+l-min_val]++;
                    }
                }
            }
        }

        double chi_square = fun_chi_square(result,compare);

        //chi-square check with 0.1% of coherence
        double chi_01_percent = 49.72823247;
        ASSERT_LT(chi_square,chi_01_percent);
    }

} // test

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
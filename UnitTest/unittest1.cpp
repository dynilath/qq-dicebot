#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\CQDicebot\diceroller.h"
#include <string>
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(UnitTestRoll)
	{
	public:
		
		TEST_METHOD(RollTest1)
		{
			std::cout << ">>test 1 : DiceRoller dr_dice(40, 100);" << std::endl;
			DiceRoller dr_dice(40, 100);
			Assert::IsNotNull(dr_dice.str_detail_result);
			int result = dr_dice.i_sum_result;
			std::string out = *dr_dice.str_detail_result;
			std::cout << "result : " << result << std::endl;
			std::cout << "detail : " << out << std::endl;
			Assert::IsTrue(result >= 40 && result <= 4000);
			Assert::IsTrue(out.length() > 117);
		}

		TEST_METHOD(RollTest1_cp)
		{
			std::cout << ">>test 1 cp: DiceRoller dr_dice(40, 100);" << std::endl;
			DiceRoller dr_dice(40, 100);
			Assert::IsNotNull(dr_dice.str_detail_result);
			int result = dr_dice.i_sum_result;
			std::string out = *dr_dice.str_detail_result;
			std::cout << "result : " << result << std::endl;
			std::cout << "detail : " << out << std::endl;
			Assert::IsTrue(result >= 40 && result <= 4000);
			Assert::IsTrue(out.length() > 117);
		}

		TEST_METHOD(RollTest2)
		{
			std::cout << ">>test 2 : DiceRoller dr_dice(40, 100, 10, true);" << std::endl;
			DiceRoller dr_dice(40, 100, 10, true);
			Assert::IsNotNull(dr_dice.str_detail_result);
			int result = dr_dice.i_sum_result;
			std::string out = *dr_dice.str_detail_result;
			std::cout << "result : " << result << std::endl;
			std::cout << "detail : " << out << std::endl;
			Assert::IsTrue(result >= 10 && result <= 1000);
			Assert::IsTrue(out.length() > 177);
		}

		TEST_METHOD(RollTest3)
		{
			std::cout << ">>test 3 : DiceRoller dr_dice(40, 100, 10, false);" << std::endl;
			DiceRoller dr_dice(40, 100, 10, false);
			Assert::IsNotNull(dr_dice.str_detail_result);
			int result = dr_dice.i_sum_result;
			std::string out = *dr_dice.str_detail_result;
			std::cout << "result : " << result << std::endl;
			std::cout << "detail : " << out << std::endl;
			Assert::IsTrue(result >= 10 && result <= 1000);
			Assert::IsTrue(out.length() > 177);
		}

		TEST_METHOD(RollTest4)
		{
			std::cout << ">>test 4 : DiceRoller dr_dice(\"+1d20\");" << std::endl;
			std::string input("+1d20");
			DiceRoller dr_dice(input);
			Assert::IsNotNull(dr_dice.str_detail_result);
			int result = dr_dice.i_sum_result;
			std::string out = *dr_dice.str_detail_result;
			std::cout << "result : " << result << std::endl;
			std::cout << "detail : " << out << std::endl;
			Assert::IsTrue(result >= 1 && result <= 20);
			Assert::IsTrue(out.length() > 2);
		}

		TEST_METHOD(RollTest5)
		{
			std::cout << ">>test 5 : DiceRoller dr_dice(\"+4d10k2\");" << std::endl;
			std::string input("+4d10k2");
			DiceRoller dr_dice(input);
			Assert::IsNotNull(dr_dice.str_detail_result);
			int result = dr_dice.i_sum_result;
			std::string out = *dr_dice.str_detail_result;
			std::cout << "result : " << result << std::endl;
			std::cout << "detail : " << out << std::endl;
			Assert::IsTrue(result >= 2 && result <= 20);
			Assert::IsTrue(out.length() > 13);
		}

		TEST_METHOD(RollTest6)
		{
			std::cout << ">>test 6 : DiceRoller dr_dice(\"+4d10kl2\");" << std::endl;
			std::string input("+4d10kl2");
			DiceRoller dr_dice(input);
			Assert::IsNotNull(dr_dice.str_detail_result);
			int result = dr_dice.i_sum_result;
			std::string out = *dr_dice.str_detail_result;
			std::cout << "result : " << result << std::endl;
			std::cout << "detail : " << out << std::endl;
			Assert::IsTrue(result >= 2 && result <= 20);
			Assert::IsTrue(out.length() > 13);
		}
	};
}
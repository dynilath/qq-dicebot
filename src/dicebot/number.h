#pragma once
#include <string>
#include <cstdint>
#include <exception>

/*
** 2018-12
** by dynilath
** an number class that auto-adjust between double and integer
*/
namespace dicebot{
	#define ZERO_THRESHOLD 1e-16
	#define P_INT32_MIN -(INT32_MAX)

	class zero_divider_exception : public std::exception
	{
	public:
		zero_divider_exception(): exception(){}
	};

	class number
	{
	private:
		int32_t i_value;
		double d_value;

		void initialize(const std::string source) {
			int pos_of_dot = source.find_first_of('.');
			if (pos_of_dot == std::string::npos) {
				this->is_using_int = true;
				this->i_value = std::stoi(source);
			}
			else {
				std::string str_decimal = source.substr(pos_of_dot + 1);
				int i_decimal = std::stoi(str_decimal);
				if (i_decimal == 0) {
					std::string str_integer = source.substr(0, pos_of_dot);
					this->is_using_int = true;
					if (str_integer.size() > 0)this->i_value = std::stoi(str_integer);
					else this->i_value = 0;
				}
				else {
					this->is_using_int = false;
					this->d_value = std::stod(source);
				}
			}
		}

		void int_2_double(){
			if(!this->is_using_int) return;
			this->d_value = this->i_value;
			this->is_using_int = false;
		}

		void double_2_int(){
			if(this->is_using_int) return;
			this->i_value = (int32_t)this->d_value;
			this->is_using_int = true;
		}

	public:
		bool is_using_int;

		number()
		{
			this->i_value = 0;
			this->is_using_int = true;
		}

		number(const int32_t source)
		{
			this->i_value = source;
			this->is_using_int = true;
		}

		number(const double source)
		{
			this->d_value = source;
			this->is_using_int = false;
		}

		number(const std::string & source)
		{
			initialize(source);
		}

		number(const char * source)
		{
			std::string str_source(source); 
			initialize(str_source);
		}

		~number(){}

		inline number operator + (const number &val1)
		{
			if (val1.is_using_int){
				return *this + val1.i_value;
			}
			else {
				this->int_2_double();
				return *this + val1.d_value;
			}
		}

		inline number operator + (const int32_t &val1) {
			if (this->is_using_int
				&& (val1 == 0
					|| (val1 > 0 && INT32_MAX - val1 > this->i_value)
					|| (val1 < 0 && P_INT32_MIN - val1 < this->i_value)
					)) {
				return number(this->i_value + val1);
			}
			else {
				return *this + (double)val1;
			}
		}

		inline number operator + (const double &val1) {
			double value_this = this->is_using_int ? (double)(this->i_value) : this->d_value;
			number ret(value_this + val1);

			if (ret.d_value < INT32_MAX && ret.d_value > P_INT32_MIN) {
				int32_t i_val = (int32_t)(ret.d_value);
				double diff = ret.d_value - i_val;
				if (diff < ZERO_THRESHOLD && diff > -ZERO_THRESHOLD)
					ret.double_2_int();
			}

			return ret;
		}

		inline number operator - (const number &val1)
		{
			if (val1.is_using_int) {
				return *this - val1.i_value;
			}
			else {
				this->int_2_double();
				return *this - val1.d_value;
			}
		}

		inline number operator - (const int32_t val1) {
			if (this->is_using_int
				&& (val1 == 0
					|| (val1 > 0 && P_INT32_MIN + val1 < this->i_value)
					|| (val1 < 0 && INT32_MAX + val1 > this->i_value))) {
				return number(this->i_value - val1);
			}
			else {
				return *this - (double)val1;
			}
		}

		inline number operator - (const double val1) {
			double value_this = this->is_using_int ? (double)(this->i_value) : this->d_value;
			number ret(value_this - val1);

			if (ret.d_value < INT32_MAX && ret.d_value > P_INT32_MIN) {
				int32_t i_val = (int32_t)(ret.d_value);
				double diff = ret.d_value - i_val;
				if (diff < ZERO_THRESHOLD && diff > -ZERO_THRESHOLD) 
					ret.double_2_int();
			}

			return ret;
		}

		inline number operator * (const number &val1)
		{
			if (val1.is_using_int) {
				return (*this)*(val1.i_value);
			}
			else {
				this->int_2_double();
				return (*this)*(val1.d_value);
			}
		}

		inline number operator * (const int32_t val1) {
			if (this->is_using_int
				&& (val1 == 0
				|| this->i_value == 0 
				|| (val1 > 0 && this->i_value > 0 && INT32_MAX / val1 > this->i_value)
				|| (val1 < 0 && this->i_value < 0 && INT32_MAX / val1 < this->i_value)
				|| (val1 < 0 && this->i_value > 0 && P_INT32_MIN / val1 > this->i_value)
				|| (val1 > 0 && this->i_value < 0 && P_INT32_MIN / val1 < this->i_value))) {
				return number(this->i_value * val1);
			}
			else {
				return (*this)*(double)val1;
			}
		}

		inline number operator * (const double val1) {
			double value_this = this->is_using_int ? (double)(this->i_value) : this->d_value;
			number ret(value_this * val1);

			if (ret.d_value < INT32_MAX && ret.d_value > P_INT32_MIN) {
				int32_t i_val = (int32_t)(ret.d_value);
				double diff = ret.d_value - i_val;
				if (diff < ZERO_THRESHOLD && diff > -ZERO_THRESHOLD)
					ret.double_2_int();
			}
			return ret;
		}

		inline number operator / (const number &val1)
		{
			if ((val1.is_using_int && val1.i_value == 0)
				|| (!val1.is_using_int && val1.d_value < ZERO_THRESHOLD && val1.d_value > -ZERO_THRESHOLD)) {
				throw zero_divider_exception();
			}

			if (val1.is_using_int) {
				return *this / val1.i_value;
			}
			else {
				this->int_2_double();
				return *this / val1.d_value;
			}
		}

		inline number operator / (const int32_t val1) {
			if (val1 == 0) {
				throw zero_divider_exception();
			}

			if (this->is_using_int && this->i_value % val1 == 0) {
				return number(this->i_value / val1);
			}
			else {
				return (*this)/(double)val1;
			}
		}

		inline number operator / (const double val1) {
			if (val1 < ZERO_THRESHOLD && val1 > -ZERO_THRESHOLD) {
				throw zero_divider_exception();
			}

			double value_this = this->is_using_int ? (double)(this->i_value) : this->d_value;
			number ret(value_this / val1);

			if (ret.d_value < INT32_MAX && ret.d_value > P_INT32_MIN) {
				int32_t i_val = (int32_t)(ret.d_value);
				double diff = ret.d_value - i_val;
				if (diff < ZERO_THRESHOLD && diff > -ZERO_THRESHOLD) 
					ret.double_2_int();
			}
			return ret;
		}

		std::string str() {
			if (this->is_using_int)
				return std::to_string(this->i_value);
			else 
			{
				return std::to_string(this->d_value);
			}
		}
	};
}

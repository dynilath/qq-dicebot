#pragma once
#include <string>
#include <cstdint>
#include <exception>
#include <sstream>

/*
** 2018-12
** by dynilath
** an number class that auto-adjust between double and integer
*/

#define ZERO_THRESHOLD 1e-16
#define P_INT32_MAX -(INT32_MIN)

class zero_devider_exception : public std::exception
{
public:
	zero_devider_exception():exception("cannot devided by zero"){}
};

class number
{
private:
	int32_t i_value;
	double d_value;
	bool is_using_int;

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


public:
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
			return *this + val1.d_value;
		}
	}

	inline number operator + (const int32_t &val1) {
		if (this->is_using_int
			&& (val1 == 0
				|| (val1 > 0 && P_INT32_MAX - val1 < this->i_value)
				|| (val1 < 0 && INT32_MIN - val1 > this->i_value))) {
			return number(this->i_value + val1);
		}
		else {
			return *this + (double)val1;
		}
	}

	inline number operator + (const double &val1) {
		double value_this = this->is_using_int ? (double)(this->i_value) : this->d_value;
		number ret(value_this + val1);

		if (ret.d_value < P_INT32_MAX && ret.d_value > INT32_MIN) {
			int32_t i_val = (int32_t)(ret.d_value);
			double diff = ret.d_value - i_val;
			if (diff < ZERO_THRESHOLD && diff > -ZERO_THRESHOLD) {
				ret.is_using_int = true;
				ret.i_value = i_val;
			}
		}

		return ret;
	}

	inline number operator - (const number &val1)
	{
		if (val1.is_using_int) {
			return *this - val1.i_value;
		}
		else {
			return *this - val1.d_value;
		}
	}

	inline number operator - (const int32_t val1) {
		if (this->is_using_int
			&& (val1 == 0
				|| (val1 > 0 && INT32_MIN + val1 < this->i_value)
				|| (val1 < 0 && P_INT32_MAX + val1 > this->i_value))) {
			return number(this->i_value - val1);
		}
		else {
			return *this - (double)val1;
		}
	}

	inline number operator - (const double val1) {
		double value_this = this->is_using_int ? (double)(this->i_value) : this->d_value;
		number ret(value_this - val1);

		if (ret.d_value < P_INT32_MAX && ret.d_value > INT32_MIN) {
			int32_t i_val = (int32_t)(ret.d_value);
			double diff = ret.d_value - i_val;
			if (diff < ZERO_THRESHOLD && diff > -ZERO_THRESHOLD) {
				ret.is_using_int = true;
				ret.i_value = i_val;
			}
		}

		return ret;
	}

	inline number operator * (const number &val1)
	{
		if (val1.is_using_int) {
			return (*this)*(val1.i_value);
		}
		else {
			return (*this)*(val1.d_value);
		}
	}

	inline number operator * (const int32_t val1) {
		if (this->is_using_int
			&& (val1 == 0
			|| this->i_value == 0 
			|| (val1 > 0 && this->i_value > 0 && P_INT32_MAX / val1 > this->i_value)
			|| (val1 < 0 && this->i_value < 0 && P_INT32_MAX / val1 < this->i_value)
			|| (val1 < 0 && this->i_value > 0 && INT32_MIN / val1 > this->i_value)
			|| (val1 > 0 && this->i_value < 0 && INT32_MIN / val1 < this->i_value))) {
			return number(this->i_value * val1);
		}
		else {
			return (*this)*(double)val1;
		}
	}

	inline number operator * (const double val1) {
		double value_this = this->is_using_int ? (double)(this->i_value) : this->d_value;
		number ret(value_this * val1);

		if (ret.d_value < P_INT32_MAX && ret.d_value > INT32_MIN) {
			int32_t i_val = (int32_t)(ret.d_value);
			double diff = ret.d_value - i_val;
			if (diff < ZERO_THRESHOLD && diff > -ZERO_THRESHOLD) {
				ret.is_using_int = true;
				ret.i_value = i_val;
			}
		}
		return ret;
	}

	inline number operator / (const number &val1)
	{
		if ((val1.is_using_int && val1.i_value == 0)
			|| (val1.d_value < ZERO_THRESHOLD && val1.d_value > -ZERO_THRESHOLD)) {
			throw zero_devider_exception();
		}

		if (val1.is_using_int) {
			return *this / val1.i_value;
		}
		else {
			return *this / val1.d_value;
		}
	}

	inline number operator / (const int32_t val1) {
		if (val1 == 0) {
			throw zero_devider_exception();
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
			throw zero_devider_exception();
		}

		double value_this = this->is_using_int ? (double)(this->i_value) : this->d_value;
		number ret(value_this / val1);

		if (ret.d_value < P_INT32_MAX && ret.d_value > INT32_MIN) {
			int32_t i_val = (int32_t)(ret.d_value);
			double diff = ret.d_value - i_val;
			if (diff < ZERO_THRESHOLD && diff > -ZERO_THRESHOLD) {
				ret.is_using_int = true;
				ret.i_value = i_val;
			}
		}
		return ret;
	}

	std::string str() {
		std::ostringstream ostrs(std::ostringstream::ate);
		if (this->is_using_int)
			ostrs << this->i_value;
		else 
		{
			ostrs << this->d_value;
		}
		return ostrs.str();
	}
};


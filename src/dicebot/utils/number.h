#pragma once
#include <cmath>
#include <cstdint>
#include <exception>
#include <string>
namespace dicebot {

    using integer_type = int32_t;
    using float_type = double;

    union number_val {
        integer_type i_value;
        float_type f_value;
    };

    /*
    ** 2018-12
    ** by dynilath
    ** an number class that auto-adjust between float_type and integer
    */
    class number {
        void initialize(const std::string &source);

    public:
        number_val value;
        bool is_using_int;

        number();

        number(const integer_type source);
        number(const float_type source);

        number(const std::string &source);

        number(const char *source);

        number operator+(const number &val1) const;
        number operator+(const integer_type &val1) const;
        number operator+(const float_type &val1) const;

        number operator-(const number &val1) const;
        number operator-(const integer_type val1) const;
        number operator-(const float_type val1) const;

        number operator*(const number &val1) const;
        number operator*(const integer_type val1) const;
        number operator*(const float_type val1) const;

        number operator/(const number &val1) const;
        number operator/(const integer_type val1) const;
        number operator/(const float_type val1) const;
        number operator-() const;

        bool operator==(const number &val1) const;
        bool operator==(const integer_type val1) const;
        bool operator==(const float_type val1) const;

        operator std::string() const noexcept;
        friend std::ostream &operator<<(std::ostream &os, const number &num) noexcept {
            os << std::string(num);
            return os;
        }
        friend std::ostream &operator<<(std::ostream &os, number &&num) noexcept {
            os << std::string(num);
            return os;
        }
        std::string str_holder() const noexcept;

        integer_type force_positive_int();
    };

    const number zero = 0;
} // namespace dicebot

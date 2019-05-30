#include "./number.h"
#include "../dice_excepts.h"
using namespace dicebot;

constexpr integer_type P_INT32_MIN = -(INT32_MAX);
constexpr float_type ZERO_THRESHOLD = 1e-4;

static auto try_to_be_integer = [](number &num) {
    if (!num.is_using_int) {
        if (num.value.f_value < INT32_MAX && num.value.f_value > P_INT32_MIN) {
            integer_type new_val = std::lround(num.value.f_value);
            float_type diff = num.value.f_value - new_val;
            num.is_using_int = (diff < ZERO_THRESHOLD && diff > -ZERO_THRESHOLD);
            if (num.is_using_int) num.value.i_value = new_val;
        }
    }
};

static auto int_2_float = [](number &num) {
    if (!num.is_using_int) return;
    num.value.f_value = static_cast<float_type>(num.value.i_value);
    num.is_using_int = false;
};

static auto float_2_int = [](number &num) {
    if (num.is_using_int) return;
    num.value.i_value = (integer_type)num.value.f_value;
    num.is_using_int = true;
};

void number::initialize(const std::string &source) {
    this->is_using_int = false;
    if (source.back() == '%') {
        this->value.f_value = std::stof(source.substr(0, source.size() - 1)) / 100;
    } else
        this->value.f_value = std::stof(source);
    try_to_be_integer(*this);
}

number::number() {
    this->value.i_value = 0;
    this->is_using_int = true;
}

number::number(const integer_type source) {
    this->value.i_value = source;
    this->is_using_int = true;
}

number::number(const float_type source) {
    this->value.f_value = source;
    this->is_using_int = false;
}

number::number(const std::string &source) { initialize(source); }

number::number(const char *source) {
    std::string str_source(source);
    initialize(str_source);
}

number number::operator+(const number &val1) const {
    if (val1.is_using_int) {
        return *this + val1.value.i_value;
    } else {
        return *this + val1.value.f_value;
    }
}

number number::operator+(const integer_type &val1) const {
    if (this->is_using_int
        && (val1 == 0 || (val1 > 0 && INT32_MAX - val1 > this->value.i_value)
            || (val1 < 0 && P_INT32_MIN - val1 < this->value.i_value))) {
        return number(this->value.i_value + val1);
    } else {
        return *this + static_cast<float_type>(val1);
    }
}

number number::operator+(const float_type &val1) const {
    float_type value_this = this->is_using_int ? static_cast<float_type>(this->value.i_value) : this->value.f_value;
    number ret(value_this + val1);
    try_to_be_integer(ret);
    return ret;
}

number number::operator-(const number &val1) const {
    if (val1.is_using_int) {
        return *this - val1.value.i_value;
    } else {
        return *this - val1.value.f_value;
    }
}

number number::operator-(const integer_type val1) const {
    if (this->is_using_int
        && (val1 == 0 || (val1 > 0 && P_INT32_MIN + val1 < this->value.i_value)
            || (val1 < 0 && INT32_MAX + val1 > this->value.i_value))) {
        return number(this->value.i_value - val1);
    } else {
        return *this - static_cast<float_type>(val1);
    }
}

number number::operator-(const float_type val1) const {
    float_type value_this = this->is_using_int ? static_cast<float_type>(this->value.i_value) : this->value.f_value;
    number ret(value_this - val1);
    try_to_be_integer(ret);
    return ret;
}

number number::operator*(const number &val1) const {
    if (val1.is_using_int) {
        return (*this) * (val1.value.i_value);
    } else {
        return (*this) * (val1.value.f_value);
    }
}

number number::operator*(const integer_type val1) const {
    if (this->is_using_int
        && (val1 == 0 || this->value.i_value == 0
            || (val1 > 0 && this->value.i_value > 0 && INT32_MAX / val1 > this->value.i_value)
            || (val1 < 0 && this->value.i_value < 0 && INT32_MAX / val1 < this->value.i_value)
            || (val1 < 0 && this->value.i_value > 0 && P_INT32_MIN / val1 > this->value.i_value)
            || (val1 > 0 && this->value.i_value < 0 && P_INT32_MIN / val1 < this->value.i_value))) {
        return number(this->value.i_value * val1);
    } else {
        return (*this) * static_cast<float_type>(val1);
    }
}

number number::operator*(const float_type val1) const {
    float_type value_this = this->is_using_int ? static_cast<float_type>(this->value.i_value) : this->value.f_value;
    number ret(value_this * val1);
    try_to_be_integer(ret);
    return ret;
}

number number::operator/(const number &val1) const {
    if ((val1.is_using_int && val1.value.i_value == 0)
        || (!val1.is_using_int && val1.value.f_value < ZERO_THRESHOLD && val1.value.f_value > -ZERO_THRESHOLD)) {
        throw zero_divider_exception();
    }

    if (val1.is_using_int) {
        return *this / val1.value.i_value;
    } else {
        return *this / val1.value.f_value;
    }
}

number number::operator/(const integer_type val1) const {
    if (val1 == 0) {
        throw zero_divider_exception();
    }

    if (this->is_using_int && this->value.i_value % val1 == 0) {
        return number(this->value.i_value / val1);
    } else {
        return (*this) / static_cast<float_type>(val1);
    }
}

number number::operator/(const float_type val1) const {
    if (val1 < ZERO_THRESHOLD && val1 > -ZERO_THRESHOLD) {
        throw zero_divider_exception();
    }

    float_type value_this = this->is_using_int ? static_cast<float_type>(this->value.i_value) : this->value.f_value;
    number ret(value_this / val1);
    try_to_be_integer(ret);
    return ret;
}

bool number::operator==(const number &val1) const {
    if (val1.is_using_int)
        return *this == val1.value.i_value;
    else
        return *this == val1.value.f_value;
}

bool number::operator==(const integer_type val1) const {
    if (this->is_using_int)
        return this->value.i_value == val1;
    else
        return this->value.i_value == val1;
}

bool number::operator==(const float_type val1) const {
    if (this->is_using_int)
        return this->value.i_value == val1;
    else
        return this->value.i_value == val1;
}

number::operator std::string() const noexcept {
    if (this->is_using_int)
        return std::to_string(this->value.i_value);
    else {
        std::string ret = std::to_string(this->value.f_value);
        int pos = ret.find_last_not_of('0');
        if (pos != std::string::npos) ret.assign(ret.substr(0, pos + 1));
        return ret;
    }
}

std::string number::str_holder() const noexcept {
    if (this->is_using_int) {
        if (this->value.i_value > 0)
            return std::to_string(this->value.i_value);
        else
            return std::string("(").append(std::to_string(this->value.i_value)).append(")");
    } else {
        std::string ret = "(";
        std::string fr = std::to_string(this->value.f_value);
        int pos = fr.find_last_not_of('0');
        if (pos != std::string::npos)
            ret.append(fr.substr(0, pos + 1)).append(")");
        else
            ret.append("0)");
        return ret;
    }
}

number number::operator-() const {
    if (this->is_using_int) {
        return number(-this->value.i_value);
    } else {
        return number(-this->value.f_value);
    }
}

integer_type number::force_positive_int() {
    if (this->is_using_int) {
        return this->value.i_value > 0 ? this->value.i_value : 0;
    } else {
        return this->value.f_value > 0 ? static_cast<integer_type>(this->value.f_value) : 0;
    }
}
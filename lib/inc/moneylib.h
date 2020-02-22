#ifndef MONEY_LIB_H
#define MONEY_LIB_H

#include <cstdint>

constexpr int32_t MAX_SCALE = 10000000;

template<uint8_t Power>
struct PowerOf10 {
    static_assert(Power < 8, "Power of 10 must be less than 8");
    static constexpr uint32_t value = 10 * PowerOf10<Power - 1>::value;
};

template<>
struct PowerOf10<0> {
    static constexpr uint32_t value = 1;
};

template<int8_t Scale>
class Price {
    friend Price operator +(int32_t const arg1, Price const & arg2) {
        Price result(arg1);
        result.value_ += arg2.value_;
        return result;
    }
    friend Price operator +(double const arg1, Price const & arg2) {
        Price result(arg1);
        result.value_ += arg2.value_;
        return result;
    }
    template<int8_t Scale2>
    friend class Price;
private:
    explicit Price(int64_t const value) : value_(normalize(value)) {};

public:
    Price() = default;
    explicit Price(int32_t const value) : value_(((int64_t) value) * MAX_SCALE) {static_assert(Scale < 8, "Scale must be less than 8");};
    explicit Price(double const value) : value_(normalize(value * MAX_SCALE)) {};
    Price(Price const & rhs) = default;
    template<int8_t Scale2>
    Price(Price<Scale2> const & rhs) : value_(normalize(rhs.value_)) {};
    Price & operator = (Price const & rhs) = default;
    template<int8_t Scale2>
    Price & operator = (Price<Scale2> const & rhs) {
        value_ = normalize(rhs.value_);
        return *this;
    }
    Price & operator =(int32_t const rhs) {
        value_ = rhs * MAX_SCALE;
        return *this;
    }
    Price & operator = (double const rhs) {
        value_ = normalize(rhs * MAX_SCALE);
        return *this;
    }

    Price operator +(int32_t const rhs) const {
        Price result(rhs);
        result.value_ += value_;
        return result;
    }

    Price operator +(double const rhs) const {
        Price result(rhs);
        result.value_ += value_;
        return result;
    }

    Price operator +(Price const & rhs) const {
        Price result;
        result.value_ = value_ + rhs.value_;
        return result;
    }

    Price operator *(int32_t const rhs) const {
        Price result;
        result.value_ = value_ * rhs;
        return result;
    }

    template<int8_t Scale2>
    Price operator +(Price<Scale2> const & rhs) const {
        Price result(value_ + rhs.value_);
        return result;
    }

    static constexpr const int64_t minvalue = MAX_SCALE / PowerOf10<Scale>::value;
    static constexpr const int64_t halfminvalue = minvalue / 2;
    
    int64_t rawValue() const {return value_;}
private:
    int64_t normalize(int64_t target) const {
        if (target == 0) {
            return 0;
        }
        if (target < 0) {
            if (target > - halfminvalue) {
                return 0;
            }
            return ((target - (1 + halfminvalue)) / minvalue) * minvalue;
        } else {
            if (target < halfminvalue) {
                return 0;
            }
            return ((target + (1 + halfminvalue)) / minvalue) * minvalue;
        }
    }

    int64_t value_ = 0;
};

using Dollar = Price<0>;
using Dime = Price<1>;
using Penny = Price<2>;
using TenthPenny = Price<3>;
using HundrethPenny = Price<4>;
#endif //MONEY_LIB_H



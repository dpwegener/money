#ifndef MONEY_LIB_H
#define MONEY_LIB_H

#include <cstdint>

constexpr int32_t MAX_SCALE = 1000000;

template<uint8_t Power>
struct PowerOf10 {
    static_assert(Power < 5, "Power of 10 must be less than 5");
    static constexpr uint32_t value = 10 * PowerOf10<Power - 1>::value;
};

template<>
struct PowerOf10<0> {
    static constexpr uint32_t value = 1;
};

template<uint8_t Scale>
class Price {
    friend constexpr Price operator +(int32_t const arg1, Price const & arg2) {
        Price result(arg1);
        result.value_ += arg2.value_;
        return result;
    }
    friend constexpr Price operator +(double const arg1, Price const & arg2) {
        Price result(arg1);
        result.value_ += arg2.value_;
        return result;
    }
    template<uint8_t Scale2>
    friend class Price;
private:
    constexpr explicit Price(int64_t const value) : value_(normalize(value)) {};

public:
    constexpr Price() = default;
    constexpr explicit Price(int32_t const value) : value_(static_cast<int64_t>(value) * MAX_SCALE) {};
    constexpr explicit Price(double const value) : value_(normalize(value * MAX_SCALE)) {};
    constexpr Price(Price const & rhs) = default;
    template<uint8_t Scale2>
    constexpr Price(Price<Scale2> const & rhs) : value_(normalize(rhs.value_)) {};
    constexpr Price & operator = (Price const & rhs) = default;
    template<uint8_t Scale2>
    constexpr Price & operator = (Price<Scale2> const & rhs) {
        value_ = normalize(rhs.value_);
        return *this;
    }
    constexpr Price & operator =(int32_t const rhs) {
        value_ = rhs * MAX_SCALE;
        return *this;
    }
    constexpr Price & operator = (double const rhs) {
        value_ = normalize(rhs * MAX_SCALE);
        return *this;
    }

    constexpr Price operator +(int32_t const rhs) const {
        Price result(rhs);
        result.value_ += value_;
        return result;
    }

    constexpr Price operator +(double const rhs) const {
        Price result(rhs);
        result.value_ += value_;
        return result;
    }

    constexpr Price operator +(Price const & rhs) const {
        Price result;
        result.value_ = value_ + rhs.value_;
        return result;
    }

    constexpr Price operator *(int32_t const rhs) const {
        Price result;
        result.value_ = value_ * rhs;
        return result;
    }

    template<uint8_t Scale2>
    constexpr Price operator +(Price<Scale2> const & rhs) const {
        Price result(value_ + rhs.value_);
        return result;
    }

    static constexpr const int32_t minvalue = MAX_SCALE / PowerOf10<Scale>::value;
    static constexpr const int32_t halfminvalue = minvalue / 2;

    static constexpr int32_t minValue() {
        return MAX_SCALE / PowerOf10<Scale>::value;
    }

    static constexpr int32_t halfMinValue() {
        return minValue() / 2;
    }
    
    constexpr int64_t rawValue() const {return value_;}
private:
    constexpr int64_t normalize(int64_t target) const {
        if (target > - halfMinValue() && target < halfMinValue()) {
            return 0;
        }
        if (target < 0) {
            return ((target - (1 + halfMinValue())) / minValue()) * minValue();
        } else {
            return ((target + (1 + halfMinValue())) / minValue()) * minValue();
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



#ifndef MONEY_LIB_H
#define MONEY_LIB_H

#include <cstdint>

constexpr int32_t MAX_SCALE = 10000000;

static constexpr uint32_t powerOfTen(uint8_t const exponent) noexcept {
    return exponent == 0 ? 1 : 10 * powerOfTen(exponent - 1);
}

template<uint8_t Power>
struct PowerOf10 {
    static_assert(Power < 8, "Power of 10 must be less than 8");
    static constexpr uint32_t value = 10 * PowerOf10<Power - 1>::value;
};

template<>
struct PowerOf10<1> {
    static constexpr uint32_t value = 10;
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
public:
    Price() = default;
    explicit Price(int32_t const value) : value_(((int64_t) value) * MAX_SCALE) {};
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

    template<int8_t Scale2>
    Price operator +(Price<Scale2> const & rhs) const {
        Price result;
        result.value_ = normalize(value_ + rhs.value_);
        return result;
    }

    enum {minPrice_ = MAX_SCALE / powerOfTen(Scale),
          halfMin_ = minPrice_ / 2};
//    enum {minPrice_ = MAX_SCALE / PowerOf10<Scale>::value};
    int64_t minPrice() const {return minPrice_;}

    int64_t rawValue() const {return value_;}
private:
    int64_t normalize(int64_t target) const {
        if (target == 0) {
            return 0;
        }
        if (target < 0) {
            if (target > -halfMin_) {
                return 0;
            }
            return ((target - (1 + halfMin_)) / minPrice_) * minPrice_;
        } else {
            if (target < halfMin_) {
                return 0;
            }
            return ((target + (1 + halfMin_)) / minPrice_) * minPrice_;
        }
    }

    //enum {scale_ = Scale};
    int64_t value_ = 0;
};

using Dollar = Price<0>;
using Dime = Price<1>;
using Penny = Price<2>;
using TenthPenny = Price<3>;
using HundrethPenny = Price<4>;
#endif //MONEY_LIB_H



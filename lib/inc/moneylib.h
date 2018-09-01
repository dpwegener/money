#ifndef MONEY_LIB_H
#define MONEY_LIB_H

#include <cstdint>

constexpr int32_t PRICE_SCALE = 10000000;

static constexpr int powerOfTen(u_int8_t exponent) {
    return exponent == 0 ? 1 : 10 * powerOfTen(exponent -1);
}

template<int Power>
struct PowerOf10 {
    static_assert(Power < 8, "Power of 10 must be less than 8");
    static constexpr int value = 10 * PowerOf10<Power - 1>::value;
};

template<>
struct PowerOf10<1> {
    static constexpr int value = 10;
};

template<>
struct PowerOf10<0> {
    static constexpr int value = 1;
};

template<int8_t Precission>
class Price {
    friend Price operator +(const int arg1, Price const & arg2) {
        Price result(arg1);
        result.value_ += arg2.value_;
        return result;
    }
    friend Price operator +(const double arg1, Price const & arg2) {
        Price result(arg1);
        result.value_ += arg2.value_;
        return result;
    }
    template<int8_t Prec2>
    friend class Price;
public:
    Price() = default;
    explicit Price(int32_t value) : value_(value * PRICE_SCALE) {};
    explicit Price(double value) : value_(normalize(value * PRICE_SCALE)) {};
    Price(Price const & rhs) = default;
    template<int8_t Prec2>
    Price(Price<Prec2> const & rhs) : value_(normalize(rhs.value_)) {};
    Price & operator = (Price const & rhs) = default;
    template<int8_t Prec2>
    Price & operator = (Price<Prec2> const & rhs) {
        value_ = normalize(rhs.value_);
        return *this;
    }
    Price & operator =(int32_t rhs) {
        value_ = rhs * PRICE_SCALE;
        return *this;
    }
    Price & operator = (double rhs) {
        value_ = normalize(rhs * PRICE_SCALE);
        return *this;
    }

    Price operator +(int32_t rhs) const {
        Price result(rhs);
        result.value_ += value_;
        return result;
    }

    Price operator +(double rhs) const {
        Price result(rhs);
        result.value_ += value_;
        return result;
    }

    Price operator +(Price const & rhs) const {
        Price result;
        result.value_ = value_ + rhs.value_;
        return result;
    }

    template<int8_t Prec2>
    Price operator +(Price<Prec2> const & rhs) const {
        Price result;
        result.value_ = normalize(value_ + rhs.value_);
        return result;
    }

    enum {minPrice_ = PRICE_SCALE / powerOfTen(Precission)};
    int64_t minPrice() const {return minPrice_;}

    int64_t rawValue() const {return value_;}
private:
    int64_t normalize(int64_t target) const {
        if (target < 0) {
            return ((target - 1 - (minPrice_ / 2)) / minPrice_) * minPrice_;
        } else {
            return ((target + 1 + (minPrice_ / 2)) / minPrice_) * minPrice_;
        }
    }

    //enum {precission_ = Precission};
    int64_t value_ = 0;
};

using Dollar = Price<0>;
using Dime = Price<1>;
using Penny = Price<2>;
using TenthPenny = Price<3>;
using HundrethPenny = Price<4>;
#endif //MONEY_LIB_H



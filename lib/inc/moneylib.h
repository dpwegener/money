#ifndef MONEY_LIB_H
#define MONEY_LIB_H

#include <cstdint>

constexpr int32_t PRICE_SCALE = 10000000;

template<int Power>
struct PowerOf10 {
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
public:
    Price() = default;
    Price(int32_t value) : value_(value * PRICE_SCALE) {};
    Price(double value) : value_(normalize(value * PRICE_SCALE)) {};
    static constexpr int64_t minPrice_ = PRICE_SCALE / PowerOf10<Precission>::value;
    int64_t minPrice() {return minPrice_;}

    int64_t rawValue() {return value_;}
private:
    int64_t normalize(int64_t target) {
        return ((target + 1 + (minPrice_ / 2)) / minPrice_) * minPrice_;
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



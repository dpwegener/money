#include "catch.hpp"
#include "moneylib.h"

#include <iostream>

TEST_CASE("Test minimum prices", "[Money][MinPrice]") {
    SECTION("Whole numbers only") {
        Price<0> out;
        REQUIRE(out.minPrice() == PRICE_SCALE);
    }

    SECTION(".1") {
        Price<1> out;
        REQUIRE(out.minPrice() == PRICE_SCALE / 10);
    }

    SECTION(".01") {
        Price<2> out;
        REQUIRE(out.minPrice() == PRICE_SCALE / 100);
    }

    SECTION(".001") {
        Price<3> out;
        REQUIRE(out.minPrice() == PRICE_SCALE / 1000);
    }

    SECTION(".0001") {
        Price<4> out;
        REQUIRE(out.minPrice() == PRICE_SCALE / 10000);
    }
}

TEST_CASE("Test currency names", "[Money][Names]") {
    SECTION("Dollar") {
        Dollar out;
        REQUIRE(out.minPrice() == PRICE_SCALE);
    }
    SECTION("Dime") {
        Dime out;
        REQUIRE(out.minPrice() == PRICE_SCALE / 10);
    }
    SECTION("Penny") {
        Penny out;
        REQUIRE(out.minPrice() == PRICE_SCALE / 100);
    }
    SECTION("TenthPenny") {
        TenthPenny out;
        REQUIRE(out.minPrice() == PRICE_SCALE / 1000);
    }
    SECTION("HundrethPenny") {
        HundrethPenny out;
        REQUIRE(out.minPrice() == PRICE_SCALE / 10000);
    }
}

TEST_CASE("Construct Prices", "[Money][Constructor]") {
    SECTION("No Arg") {
        Price<4> out;
        REQUIRE(out.rawValue() == 0);
    }
    SECTION("Integer argument") {
        Price<4> out(1234);
        REQUIRE(out.rawValue() == 1234 * PRICE_SCALE);
    }

    SECTION("Double argument") {
        REQUIRE(HundrethPenny(1.23456789).rawValue() == 12346 * (PRICE_SCALE / 10000));
        REQUIRE(HundrethPenny(1.2345499).rawValue() == 12346 * (PRICE_SCALE / 10000));
    }
}
TEST_CASE("Comprehensive rounding test", "[Money][Round]") {
    for (int i = 0; i <= 100000; i++) {
        HundrethPenny out(static_cast<double> (i) / 100000.0);
        REQUIRE(out.rawValue() == ((i + 5) / 10) * (PRICE_SCALE / 10000));
    }
}
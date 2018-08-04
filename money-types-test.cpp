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
        REQUIRE(HundrethPenny(1234).rawValue() == 1234 * PRICE_SCALE);
    }

    SECTION("Double argument") {
        REQUIRE(HundrethPenny(1.23456789).rawValue() == 12346 * (PRICE_SCALE / 10000));
        REQUIRE(HundrethPenny(1.2345499).rawValue() == 12346 * (PRICE_SCALE / 10000));
    }

    SECTION("Copy Construct") {
        HundrethPenny out(1.2345);
        REQUIRE(HundrethPenny(out).rawValue() == 12345 * (PRICE_SCALE / 10000));
        REQUIRE(TenthPenny(out).rawValue() == 1235 * (PRICE_SCALE / 1000));
        REQUIRE(Penny(out).rawValue() == 123 * (PRICE_SCALE / 100));
        REQUIRE(Dime(out).rawValue() == 12 * (PRICE_SCALE / 10));
        REQUIRE(Dollar(out).rawValue() == 1 * PRICE_SCALE);
    }
}
TEST_CASE("Comprehensive rounding test", "[Money][Round]") {
    for (int i = 0; i <= 100000; i++) {
        HundrethPenny out(static_cast<double> (i) / 100000.0);
        REQUIRE(out.rawValue() == ((i + 5) / 10) * (PRICE_SCALE / 10000));
        REQUIRE(TenthPenny(static_cast<double> (i) / 100000.0).rawValue() == ((i + 50) / 100) * (PRICE_SCALE / 1000));
        REQUIRE(Penny(static_cast<double> (i) / 100000.0).rawValue() == ((i + 500) / 1000) * (PRICE_SCALE / 100));
        REQUIRE(Dime(static_cast<double> (i) / 100000.0).rawValue() == ((i + 5000) / 10000) * (PRICE_SCALE / 10));
        REQUIRE(Dollar(static_cast<double> (i) / 100000.0).rawValue() == ((i + 50000) / 100000) * PRICE_SCALE);
    }
}

TEST_CASE("Comprehensive negative rounding test", "[Money][Round]") {
    for (int i = 0; i <= 100000; i++) {
        HundrethPenny out(static_cast<double> (-i) / 100000.0);
        REQUIRE(out.rawValue() == ((-i - 5) / 10) * (PRICE_SCALE / 10000));
        REQUIRE(TenthPenny(static_cast<double> (-i) / 100000.0).rawValue() == ((-i - 50) / 100) * (PRICE_SCALE / 1000));
        REQUIRE(Penny(static_cast<double> (-i) / 100000.0).rawValue() == ((-i - 500) / 1000) * (PRICE_SCALE / 100));
        REQUIRE(Dime(static_cast<double> (-i) / 100000.0).rawValue() == ((-i + -5000) / 10000) * (PRICE_SCALE / 10));
        REQUIRE(Dollar(static_cast<double> (-i) / 100000.0).rawValue() == ((-i + -50000) / 100000) * PRICE_SCALE);
    }
}

TEST_CASE("Add integers to Price<4>(1.2345)", "[Money][Add]") {
    HundrethPenny out(1.2345);
    SECTION("Adding 1 gives 2.2345") {
        REQUIRE((out + 1).rawValue() == 22345 * (PRICE_SCALE / 10000));
        REQUIRE((1 + out).rawValue() == 22345 * (PRICE_SCALE / 10000));
    }
    SECTION("Adding -2 gives -.7655") {
        REQUIRE((out + -2).rawValue() == -7655 * (PRICE_SCALE / 10000));
        REQUIRE((-2 + out).rawValue() == -7655 * (PRICE_SCALE / 10000));
    }
}

TEST_CASE("Add doubles to Price<4>(1.2345", "[Money][Add]") {
    HundrethPenny out(1.2345);
    SECTION("Adding 1.1 gives 2.3345") {
        REQUIRE((out + 1.1).rawValue() == 23345 * (PRICE_SCALE / 10000));
        REQUIRE((1.1 + out).rawValue() == 23345 * (PRICE_SCALE / 10000));
    }
}

TEST_CASE("Assign prices to Price<4>(1.2345", "[Money][Add]") {
    HundrethPenny out(1.2345);
    SECTION("Assigning 4.5678") {
        REQUIRE((out = HundrethPenny(4.5678)).rawValue() == 45678 * (PRICE_SCALE / 10000));
    }
    SECTION("Assigning Price<3>(4.567") {
        REQUIRE((out = TenthPenny(4.567)).rawValue() == 4567 * (PRICE_SCALE / 1000));
    }
    SECTION("Assigning Price<2>(4.56") {
        REQUIRE((out = Penny(4.56)).rawValue() == 456 * (PRICE_SCALE / 100));
    }
    SECTION("Assigning Price<1>(4.5)") {
        REQUIRE((out = Dime(4.5)).rawValue() == 45 * (PRICE_SCALE / 10));
    }
    SECTION("Assigning Price<0>(4)") {
        REQUIRE((out = Dollar(4)).rawValue() == 4 * PRICE_SCALE);
    }
    SECTION("Assigning 4.56789") {
        REQUIRE((out = 4.56789).rawValue() == 45679 * (PRICE_SCALE / 10000));
    }
    SECTION("Assigning 4") {
        REQUIRE((out = 4).rawValue() == 4 * PRICE_SCALE);
    }
}
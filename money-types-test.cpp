#include "catch.hpp"
#include "moneylib.h"

#include <iostream>

TEST_CASE("Test minimum prices", "[Money][MinPrice]") {
    SECTION("Whole numbers only") {
        Price<0> out;
        auto min = out.minvalue;
        REQUIRE(min == MAX_SCALE);
    }

    SECTION(".1") {
        Price<1> out;
        auto min = out.minvalue;
        REQUIRE(min == MAX_SCALE / 10);
    }

    SECTION(".01") {
        Price<2> out;
        auto min = out.minvalue;
        REQUIRE(min == MAX_SCALE / 100);
    }

    SECTION(".001") {
        Price<3> out;
        auto min = out.minvalue;
        REQUIRE(min == MAX_SCALE /1000);
    }

    SECTION(".0001") {
        Price<4> out;
        auto min = out.minvalue;
        REQUIRE(min == MAX_SCALE / 10000);
    }
}

TEST_CASE("Test currency names", "[Money][Names]") {
    SECTION("Dollar") {
        Dollar out;
        auto min = out.minvalue;
        REQUIRE(min == MAX_SCALE);
    }
    SECTION("Dime") {
        Dime out;
        auto min = out.minvalue;
        REQUIRE(min == MAX_SCALE / 10);
    }
    SECTION("Penny") {
        Penny out;
        auto min = out.minvalue;
        REQUIRE(min == MAX_SCALE / 100);
    }
    SECTION("TenthPenny") {
        TenthPenny out;
        auto min = out.minvalue;
        REQUIRE(min == MAX_SCALE / 1000);
    }
    SECTION("HundrethPenny") {
        HundrethPenny out;
        auto min = out.minvalue;
        REQUIRE(min == MAX_SCALE/ 10000);
    }
}

TEST_CASE("Construct Prices", "[Money][Constructor]") {
    SECTION("No Arg") {
        Price<4> out;
        REQUIRE(out.rawValue() == 0);
    }
    SECTION("Integer argument") {
        REQUIRE(HundrethPenny(1234).rawValue() == (int64_t)1234 * MAX_SCALE);
    }

    SECTION("Double argument") {
        REQUIRE(HundrethPenny(1.23456789).rawValue() == 12346 * (MAX_SCALE / 10000));
        REQUIRE(HundrethPenny(1.2345499).rawValue() == 12346 * (MAX_SCALE / 10000));
    }

    SECTION("Copy Construct") {
        HundrethPenny out(1.2345);
        REQUIRE(HundrethPenny(out).rawValue() == 12345 * (MAX_SCALE / 10000));
        REQUIRE(TenthPenny(out).rawValue() == 1235 * (MAX_SCALE / 1000));
        REQUIRE(Penny(out).rawValue() == 123 * (MAX_SCALE / 100));
        REQUIRE(Dime(out).rawValue() == 12 * (MAX_SCALE / 10));
        REQUIRE(Dollar(out).rawValue() == 1 * MAX_SCALE);
    }
}
TEST_CASE("Comprehensive rounding test", "[Money][Round]") {
    for (int i = 0; i <= 100000; i++) {
        HundrethPenny out(static_cast<double> (i) / 100000.0);
        REQUIRE(out.rawValue() == ((i + 5) / 10) * (MAX_SCALE / 10000));
        REQUIRE(TenthPenny(static_cast<double> (i) / 100000.0).rawValue() == ((i + 50) / 100) * (MAX_SCALE / 1000));
        REQUIRE(Penny(static_cast<double> (i) / 100000.0).rawValue() == ((i + 500) / 1000) * (MAX_SCALE / 100));
        REQUIRE(Dime(static_cast<double> (i) / 100000.0).rawValue() == ((i + 5000) / 10000) * (MAX_SCALE / 10));
        REQUIRE(Dollar(static_cast<double> (i) / 100000.0).rawValue() == ((i + 50000) / 100000) * MAX_SCALE);
    }
}

TEST_CASE("Comprehensive negative rounding test", "[Money][Round]") {
    for (int i = 0; i <= 100000; i++) {
        HundrethPenny out(static_cast<double> (-i) / 100000.0);
        REQUIRE(out.rawValue() == ((-i - 5) / 10) * (MAX_SCALE / 10000));
        REQUIRE(TenthPenny(static_cast<double> (-i) / 100000.0).rawValue() == ((-i - 50) / 100) * (MAX_SCALE / 1000));
        REQUIRE(Penny(static_cast<double> (-i) / 100000.0).rawValue() == ((-i - 500) / 1000) * (MAX_SCALE / 100));
        REQUIRE(Dime(static_cast<double> (-i) / 100000.0).rawValue() == ((-i + -5000) / 10000) * (MAX_SCALE / 10));
        REQUIRE(Dollar(static_cast<double> (-i) / 100000.0).rawValue() == ((-i + -50000) / 100000) * MAX_SCALE);
    }
}

TEST_CASE("Add integers to Price<4>(1.2345)", "[Money][Add]") {
    HundrethPenny out(1.2345);
    SECTION("Adding 1 gives 2.2345") {
        REQUIRE((out + 1).rawValue() == 22345 * (MAX_SCALE / 10000));
        auto actual = 1 + out;
        REQUIRE(actual.rawValue() == 22345 * (MAX_SCALE / 10000));
    }
    SECTION("Adding -2 gives -.7655") {
        REQUIRE((out + -2).rawValue() == -7655 * (MAX_SCALE / 10000));
        auto actual = -2 + out;
        REQUIRE(actual.rawValue() == -7655 * (MAX_SCALE / 10000));
    }
}

TEST_CASE("Add doubles to Price<4>(1.2345", "[Money][Add]") {
    HundrethPenny out(1.2345);
    SECTION("Adding 1.1 gives 2.3345") {
        REQUIRE((out + 1.1).rawValue() == 23345 * (MAX_SCALE / 10000));
        auto actual = 1.1 + out;
        REQUIRE(actual.rawValue() == 23345 * (MAX_SCALE / 10000));
    }
}

TEST_CASE("Add Prices to Price<4>(1.2345)", "[Money][Add]") {
    HundrethPenny out(1.2345);
    SECTION("Adding 1.1 gives 2.3345") {
        REQUIRE((out + HundrethPenny(1.1)).rawValue() == 23345 * (MAX_SCALE / 10000));
    }
    SECTION("Adding Price<2>(1.1) gives 2.3345") {
        REQUIRE((out + Dime(1.1)).rawValue() == 23345 * (MAX_SCALE / 10000));
    }
    SECTION("Adding out to Price<2>(1.1) gives 2.3") {
        REQUIRE((Dime(1.1) + out).rawValue() == 23 * (MAX_SCALE / 10));
    }
}

TEST_CASE("Assign prices to Price<4>(1.2345", "[Money][Add]") {
    HundrethPenny out(1.2345);
    SECTION("Assigning 4.5678") {
        REQUIRE((out = HundrethPenny(4.5678)).rawValue() == 45678 * (MAX_SCALE / 10000));
    }
    SECTION("Assigning Price<3>(4.567") {
        REQUIRE((out = TenthPenny(4.567)).rawValue() == 4567 * (MAX_SCALE / 1000));
    }
    SECTION("Assigning Price<2>(4.56") {
        REQUIRE((out = Penny(4.56)).rawValue() == 456 * (MAX_SCALE / 100));
    }
    SECTION("Assigning Price<1>(4.5)") {
        REQUIRE((out = Dime(4.5)).rawValue() == 45 * (MAX_SCALE / 10));
    }
    SECTION("Assigning Price<0>(4)") {
        REQUIRE((out = Dollar(4)).rawValue() == 4 * MAX_SCALE);
    }
    SECTION("Assigning 4.56789") {
        REQUIRE((out = 4.56789).rawValue() == 45679 * (MAX_SCALE / 10000));
    }
    SECTION("Assigning 4") {
        REQUIRE((out = 4).rawValue() == 4 * MAX_SCALE);
    }
}

TEST_CASE("Multiple Price<4>(1.2345) by integers", "[Money][Multiply]") {
    HundrethPenny out(1.2345);
    SECTION("Multiplying by 5 gives 6.1725") {
        REQUIRE((out * 5).rawValue() == 61725 * (MAX_SCALE / 10000));
    }
}
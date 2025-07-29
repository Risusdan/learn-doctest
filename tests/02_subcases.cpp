#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest.h"
#include "../src/math_utils.hpp"

TEST_CASE("Power function with different bases and exponents") {
    SUBCASE("Base 2") {
        int base = 2;
        CHECK(MathUtils::power(base, 0) == 1);
        CHECK(MathUtils::power(base, 1) == 2);
        CHECK(MathUtils::power(base, 3) == 8);
        CHECK(MathUtils::power(base, 5) == 32);
    }
    
    SUBCASE("Base 3") {
        int base = 3;
        CHECK(MathUtils::power(base, 0) == 1);
        CHECK(MathUtils::power(base, 1) == 3);
        CHECK(MathUtils::power(base, 2) == 9);
        CHECK(MathUtils::power(base, 3) == 27);
    }
    
    SUBCASE("Base 10") {
        int base = 10;
        CHECK(MathUtils::power(base, 0) == 1);
        CHECK(MathUtils::power(base, 1) == 10);
        CHECK(MathUtils::power(base, 2) == 100);
        CHECK(MathUtils::power(base, 3) == 1000);
    }
    
    SUBCASE("Special cases") {
        CHECK(MathUtils::power(0, 0) == 1);
        CHECK(MathUtils::power(0, 5) == 0);
        CHECK(MathUtils::power(1, 100) == 1);
        CHECK(MathUtils::power(-2, 2) == 4);
        CHECK(MathUtils::power(-2, 3) == -8);
    }
}

TEST_CASE("Vector operations with different input sizes") {
    SUBCASE("Single element vector") {
        std::vector<int> vec = {42};
        CHECK(MathUtils::findMax(vec) == 42);
        CHECK(MathUtils::average(vec) == doctest::Approx(42.0));
    }
    
    SUBCASE("Small vector") {
        std::vector<int> vec = {1, 5, 3};
        CHECK(MathUtils::findMax(vec) == 5);
        CHECK(MathUtils::average(vec) == doctest::Approx(3.0));
    }
    
    SUBCASE("Larger vector with duplicates") {
        std::vector<int> vec = {2, 8, 3, 8, 1, 7};
        CHECK(MathUtils::findMax(vec) == 8);
        CHECK(MathUtils::average(vec) == doctest::Approx(4.833333).epsilon(0.001));
    }
    
    SUBCASE("Vector with negative numbers") {
        std::vector<int> vec = {-5, -2, -10, -1};
        CHECK(MathUtils::findMax(vec) == -1);
        CHECK(MathUtils::average(vec) == doctest::Approx(-4.5));
    }
    
    SUBCASE("Mixed positive and negative") {
        std::vector<int> vec = {-3, 7, -1, 4, 0};
        CHECK(MathUtils::findMax(vec) == 7);
        CHECK(MathUtils::average(vec) == doctest::Approx(1.4));
    }
}

TEST_CASE("Number to words conversion") {
    SUBCASE("Single digits") {
        CHECK(MathUtils::numberToWords(0) == "zero");
        CHECK(MathUtils::numberToWords(1) == "one");
        CHECK(MathUtils::numberToWords(5) == "five");
        CHECK(MathUtils::numberToWords(9) == "nine");
    }
    
    SUBCASE("Teen numbers") {
        CHECK(MathUtils::numberToWords(10) == "ten");
        CHECK(MathUtils::numberToWords(11) == "eleven");
        CHECK(MathUtils::numberToWords(15) == "fifteen");
        CHECK(MathUtils::numberToWords(19) == "nineteen");
    }
    
    SUBCASE("Tens") {
        CHECK(MathUtils::numberToWords(20) == "twenty");
        CHECK(MathUtils::numberToWords(30) == "thirty");
        CHECK(MathUtils::numberToWords(50) == "fifty");
        CHECK(MathUtils::numberToWords(90) == "ninety");
    }
    
    SUBCASE("Compound numbers") {
        CHECK(MathUtils::numberToWords(21) == "twenty one");
        CHECK(MathUtils::numberToWords(35) == "thirty five");
        CHECK(MathUtils::numberToWords(67) == "sixty seven");
        CHECK(MathUtils::numberToWords(99) == "ninety nine");
    }
}
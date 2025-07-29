#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest.h"
#include "../src/math_utils.hpp"

TEST_CASE("Basic arithmetic operations") {
    CHECK(MathUtils::add(2, 3) == 5);
    CHECK(MathUtils::add(-1, 1) == 0);
    CHECK(MathUtils::add(0, 0) == 0);
    
    CHECK(MathUtils::multiply(3, 4) == 12);
    CHECK(MathUtils::multiply(-2, 5) == -10);
    CHECK(MathUtils::multiply(0, 100) == 0);
}

TEST_CASE("Division operations") {
    CHECK(MathUtils::divide(10.0, 2.0) == doctest::Approx(5.0));
    CHECK(MathUtils::divide(7.0, 3.0) == doctest::Approx(2.333333).epsilon(0.001));
    CHECK(MathUtils::divide(-8.0, 2.0) == doctest::Approx(-4.0));
}

TEST_CASE("Difference between CHECK and REQUIRE") {
    int x = 5;
    
    CHECK(x > 0);
    CHECK(x < 10);
    
    REQUIRE(x != 0);
    CHECK(10 / x == 2);
}

TEST_CASE("Testing factorial function") {
    REQUIRE(MathUtils::factorial(0) == 1);
    REQUIRE(MathUtils::factorial(1) == 1);
    CHECK(MathUtils::factorial(3) == 6);
    CHECK(MathUtils::factorial(5) == 120);
}

TEST_CASE("Prime number testing") {
    CHECK_FALSE(MathUtils::isPrime(1));
    CHECK(MathUtils::isPrime(2));
    CHECK(MathUtils::isPrime(3));
    CHECK_FALSE(MathUtils::isPrime(4));
    CHECK(MathUtils::isPrime(5));
    CHECK_FALSE(MathUtils::isPrime(9));
    CHECK(MathUtils::isPrime(17));
}
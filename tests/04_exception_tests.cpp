#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest.h"
#include "../src/math_utils.hpp"

TEST_CASE("Exception testing with CHECK_THROWS") {
    CHECK_THROWS(MathUtils::divide(5.0, 0.0));
    CHECK_THROWS(MathUtils::factorial(-1));
    CHECK_THROWS(MathUtils::power(2, -3));
    
    std::vector<int> empty_vec;
    CHECK_THROWS(MathUtils::findMax(empty_vec));
    CHECK_THROWS(MathUtils::average(empty_vec));
}

TEST_CASE("Exception testing with specific exception types") {
    CHECK_THROWS_AS(MathUtils::divide(10.0, 0.0), std::invalid_argument);
    CHECK_THROWS_AS(MathUtils::factorial(-5), std::invalid_argument);
    CHECK_THROWS_AS(MathUtils::power(3, -2), std::invalid_argument);
    
    std::vector<int> empty_vec;
    CHECK_THROWS_AS(MathUtils::findMax(empty_vec), std::invalid_argument);
    CHECK_THROWS_AS(MathUtils::average(empty_vec), std::invalid_argument);
}

TEST_CASE("Exception testing with message verification") {
    CHECK_THROWS_WITH(MathUtils::divide(7.0, 0.0), "Division by zero");
    CHECK_THROWS_WITH(MathUtils::factorial(-3), "Factorial of negative number");
    CHECK_THROWS_WITH(MathUtils::power(5, -1), "Negative exponent not supported");
    
    std::vector<int> empty_vec;
    CHECK_THROWS_WITH(MathUtils::findMax(empty_vec), "Cannot find max of empty vector");
    CHECK_THROWS_WITH(MathUtils::average(empty_vec), "Cannot calculate average of empty vector");
}

TEST_CASE("Testing functions that should NOT throw") {
    CHECK_NOTHROW(MathUtils::add(5, 3));
    CHECK_NOTHROW(MathUtils::multiply(-2, 4));
    CHECK_NOTHROW(MathUtils::divide(10.0, 2.0));
    CHECK_NOTHROW(MathUtils::factorial(5));
    CHECK_NOTHROW(MathUtils::isPrime(17));
    CHECK_NOTHROW(MathUtils::power(2, 3));
    
    std::vector<int> valid_vec = {1, 2, 3};
    CHECK_NOTHROW(MathUtils::findMax(valid_vec));
    CHECK_NOTHROW(MathUtils::average(valid_vec));
}

TEST_CASE("Edge cases for numberToWords function") {
    CHECK_THROWS_AS(MathUtils::numberToWords(-1), std::invalid_argument);
    CHECK_THROWS_AS(MathUtils::numberToWords(100), std::invalid_argument);
    CHECK_THROWS_WITH(MathUtils::numberToWords(-5), "Only numbers 0-99 supported");
    CHECK_THROWS_WITH(MathUtils::numberToWords(150), "Only numbers 0-99 supported");
    
    CHECK_NOTHROW(MathUtils::numberToWords(0));
    CHECK_NOTHROW(MathUtils::numberToWords(50));
    CHECK_NOTHROW(MathUtils::numberToWords(99));
}

SCENARIO("Division by zero handling") {
    GIVEN("I have two numbers where the divisor is zero") {
        double dividend = 42.0;
        double divisor = 0.0;
        
        WHEN("I try to divide them") {
            THEN("an exception should be thrown") {
                CHECK_THROWS_AS(MathUtils::divide(dividend, divisor), std::invalid_argument);
            }
            
            AND_THEN("the exception message should be informative") {
                CHECK_THROWS_WITH(MathUtils::divide(dividend, divisor), "Division by zero");
            }
        }
    }
    
    GIVEN("I have two valid numbers") {
        double dividend = 15.0;
        double divisor = 3.0;
        
        WHEN("I divide them") {
            THEN("no exception should be thrown") {
                CHECK_NOTHROW(MathUtils::divide(dividend, divisor));
            }
            
            AND_THEN("the result should be correct") {
                CHECK(MathUtils::divide(dividend, divisor) == doctest::Approx(5.0));
            }
        }
    }
}
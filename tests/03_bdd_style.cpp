#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest.h"
#include "../src/math_utils.hpp"

SCENARIO("Calculating factorial of a number") {
    GIVEN("I have a non-negative integer") {
        WHEN("the number is 0") {
            int n = 0;
            THEN("the factorial should be 1") {
                CHECK(MathUtils::factorial(n) == 1);
            }
        }
        
        WHEN("the number is 1") {
            int n = 1;
            THEN("the factorial should be 1") {
                CHECK(MathUtils::factorial(n) == 1);
            }
        }
        
        WHEN("the number is 5") {
            int n = 5;
            THEN("the factorial should be 120") {
                CHECK(MathUtils::factorial(n) == 120);
            }
        }
    }
}

SCENARIO("Finding prime numbers") {
    GIVEN("I want to check if numbers are prime") {
        WHEN("I check the number 2") {
            int n = 2;
            THEN("it should be prime") {
                CHECK(MathUtils::isPrime(n) == true);
            }
        }
        
        WHEN("I check the number 4") {
            int n = 4;
            THEN("it should not be prime") {
                CHECK(MathUtils::isPrime(n) == false);
            }
        }
        
        WHEN("I check the number 17") {
            int n = 17;
            THEN("it should be prime") {
                CHECK(MathUtils::isPrime(n) == true);
            }
        }
        
        WHEN("I check numbers less than 2") {
            THEN("they should not be prime") {
                CHECK(MathUtils::isPrime(0) == false);
                CHECK(MathUtils::isPrime(1) == false);
                CHECK(MathUtils::isPrime(-5) == false);
            }
        }
    }
}

SCENARIO("Working with vectors of numbers") {
    GIVEN("I have a vector of integers") {
        std::vector<int> numbers;
        
        WHEN("the vector contains positive numbers") {
            numbers = {3, 7, 2, 9, 1};
            
            THEN("I can find the maximum") {
                CHECK(MathUtils::findMax(numbers) == 9);
            }
            
            AND_THEN("I can calculate the average") {
                CHECK(MathUtils::average(numbers) == doctest::Approx(4.4));
            }
        }
        
        WHEN("the vector contains negative numbers") {
            numbers = {-5, -2, -8, -1};
            
            THEN("the maximum should be the least negative") {
                CHECK(MathUtils::findMax(numbers) == -1);
            }
            
            AND_THEN("the average should be negative") {
                double avg = MathUtils::average(numbers);
                CHECK(avg < 0);
                CHECK(avg == doctest::Approx(-4.0));
            }
        }
        
        WHEN("the vector has mixed positive and negative numbers") {
            numbers = {-3, 5, -1, 8, 0};
            
            THEN("the maximum should be positive") {
                int max_val = MathUtils::findMax(numbers);
                CHECK(max_val > 0);
                CHECK(max_val == 8);
            }
        }
    }
}
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest.h"
#include "../src/calculator.hpp"

TEST_CASE("Calculator basic arithmetic operations") {
    Calculator calc;
    
    CHECK(calc.add(5.0, 3.0) == doctest::Approx(8.0));
    CHECK(calc.subtract(10.0, 4.0) == doctest::Approx(6.0));
    CHECK(calc.multiply(6.0, 7.0) == doctest::Approx(42.0));
    CHECK(calc.divide(15.0, 3.0) == doctest::Approx(5.0));
}

TEST_CASE("Calculator memory operations") {
    Calculator calc;
    
    REQUIRE(calc.getMemory() == doctest::Approx(0.0));
    
    calc.setMemory(42.5);
    CHECK(calc.getMemory() == doctest::Approx(42.5));
    
    calc.clearMemory();
    CHECK(calc.getMemory() == doctest::Approx(0.0));
}

TEST_CASE("Calculator history tracking") {
    Calculator calc;
    
    REQUIRE(calc.getHistory().empty());
    
    calc.add(2.0, 3.0);
    calc.multiply(4.0, 5.0);
    calc.divide(10.0, 2.0);
    
    auto history = calc.getHistory();
    REQUIRE(history.size() == 3);
    CHECK(history[0] == doctest::Approx(5.0));
    CHECK(history[1] == doctest::Approx(20.0));
    CHECK(history[2] == doctest::Approx(5.0));
    
    CHECK(calc.getLastResult() == doctest::Approx(5.0));
    
    calc.clearHistory();
    CHECK(calc.getHistory().empty());
}

TEST_CASE("Calculator trigonometric functions") {
    Calculator calc;
    
    SUBCASE("Degrees mode") {
        calc.setDegreesMode(true);
        REQUIRE(calc.isDegreesMode() == true);
        
        CHECK(calc.sin(0.0) == doctest::Approx(0.0).epsilon(0.001));
        CHECK(calc.sin(90.0) == doctest::Approx(1.0).epsilon(0.001));
        CHECK(calc.cos(0.0) == doctest::Approx(1.0).epsilon(0.001));
        CHECK(calc.cos(90.0) == doctest::Approx(0.0).epsilon(0.001));
    }
    
    SUBCASE("Radians mode") {
        calc.setDegreesMode(false);
        REQUIRE(calc.isDegreesMode() == false);
        
        CHECK(calc.sin(0.0) == doctest::Approx(0.0).epsilon(0.001));
        CHECK(calc.sin(M_PI/2) == doctest::Approx(1.0).epsilon(0.001));
        CHECK(calc.cos(0.0) == doctest::Approx(1.0).epsilon(0.001));
        CHECK(calc.cos(M_PI/2) == doctest::Approx(0.0).epsilon(0.001));
    }
}

TEST_CASE("Calculator power and square root") {
    Calculator calc;
    
    CHECK(calc.power(2.0, 3.0) == doctest::Approx(8.0));
    CHECK(calc.power(5.0, 0.0) == doctest::Approx(1.0));
    CHECK(calc.power(2.0, -2.0) == doctest::Approx(0.25));
    
    CHECK(calc.sqrt(9.0) == doctest::Approx(3.0));
    CHECK(calc.sqrt(16.0) == doctest::Approx(4.0));
    CHECK(calc.sqrt(2.0) == doctest::Approx(1.414).epsilon(0.001));
}

TEST_CASE("Calculator exception handling") {
    Calculator calc;
    
    CHECK_THROWS_AS(calc.divide(5.0, 0.0), std::invalid_argument);
    CHECK_THROWS_WITH(calc.divide(10.0, 0.0), "Division by zero");
    
    CHECK_THROWS_AS(calc.sqrt(-4.0), std::invalid_argument);
    CHECK_THROWS_WITH(calc.sqrt(-9.0), "Cannot take square root of negative number");
    
    CHECK_THROWS_AS(calc.power(0.0, -1.0), std::invalid_argument);
    CHECK_THROWS_WITH(calc.power(0.0, -2.0), "Cannot raise zero to negative power");
    
    CHECK_THROWS_AS(calc.getLastResult(), std::runtime_error);
    CHECK_THROWS_WITH(calc.getLastResult(), "No calculations performed yet");
}

TEST_CASE("Calculator expression evaluation") {
    Calculator calc;
    
    CHECK(calc.evaluateExpression("2+3") == doctest::Approx(5.0));
    CHECK(calc.evaluateExpression("10-4") == doctest::Approx(6.0));
    CHECK(calc.evaluateExpression("6*7") == doctest::Approx(42.0));
    CHECK(calc.evaluateExpression("15/3") == doctest::Approx(5.0));
    
    CHECK_THROWS_AS(calc.evaluateExpression(""), std::invalid_argument);
    CHECK_THROWS_AS(calc.evaluateExpression("invalid"), std::invalid_argument);
    CHECK_THROWS_WITH(calc.evaluateExpression(""), "Empty expression");
}

SCENARIO("Using calculator for a complex calculation") {
    GIVEN("A new calculator") {
        Calculator calc;
        
        WHEN("I perform multiple operations") {
            calc.add(10.0, 5.0);
            calc.multiply(3.0, 4.0);
            calc.divide(24.0, 6.0);
            
            THEN("the history should contain all results") {
                auto history = calc.getHistory();
                REQUIRE(history.size() == 3);
                CHECK(history[0] == doctest::Approx(15.0));
                CHECK(history[1] == doctest::Approx(12.0));
                CHECK(history[2] == doctest::Approx(4.0));
            }
            
            AND_THEN("I can get the last result") {
                CHECK(calc.getLastResult() == doctest::Approx(4.0));
            }
            
            AND_WHEN("I save a value to memory") {
                calc.setMemory(calc.getLastResult());
                
                THEN("I can retrieve it later") {
                    CHECK(calc.getMemory() == doctest::Approx(4.0));
                }
            }
        }
        
        WHEN("I reset the calculator") {
            calc.add(5.0, 5.0);
            calc.setMemory(100.0);
            calc.reset();
            
            THEN("everything should be cleared") {
                CHECK(calc.getMemory() == doctest::Approx(0.0));
                CHECK(calc.getHistory().empty());
                CHECK(calc.isDegreesMode() == true);
            }
        }
    }
}
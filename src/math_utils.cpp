#include "math_utils.hpp"
#include <algorithm>
#include <numeric>
#include <stdexcept>

namespace MathUtils {

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

double divide(double a, double b) {
    if (b == 0.0) {
        throw std::invalid_argument("Division by zero");
    }
    return a / b;
}

int factorial(int n) {
    if (n < 0) {
        throw std::invalid_argument("Factorial of negative number");
    }
    if (n == 0 || n == 1) {
        return 1;
    }
    int result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

bool isPrime(int n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

int findMax(const std::vector<int>& numbers) {
    if (numbers.empty()) {
        throw std::invalid_argument("Cannot find max of empty vector");
    }
    return *std::max_element(numbers.begin(), numbers.end());
}

double average(const std::vector<int>& numbers) {
    if (numbers.empty()) {
        throw std::invalid_argument("Cannot calculate average of empty vector");
    }
    int sum = std::accumulate(numbers.begin(), numbers.end(), 0);
    return static_cast<double>(sum) / numbers.size();
}

int power(int base, int exponent) {
    if (exponent < 0) {
        throw std::invalid_argument("Negative exponent not supported");
    }
    if (exponent == 0) {
        return 1;
    }
    
    int result = 1;
    for (int i = 0; i < exponent; ++i) {
        result *= base;
    }
    return result;
}

std::string numberToWords(int n) {
    if (n < 0 || n > 99) {
        throw std::invalid_argument("Only numbers 0-99 supported");
    }
    
    const std::vector<std::string> ones = {
        "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
    };
    
    const std::vector<std::string> teens = {
        "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", 
        "sixteen", "seventeen", "eighteen", "nineteen"
    };
    
    const std::vector<std::string> tens = {
        "", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"
    };
    
    if (n < 10) {
        return ones[n];
    } else if (n < 20) {
        return teens[n - 10];
    } else {
        std::string result = tens[n / 10];
        if (n % 10 != 0) {
            result += " " + ones[n % 10];
        }
        return result;
    }
}

}
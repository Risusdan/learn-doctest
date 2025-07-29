#pragma once
#include <stdexcept>
#include <vector>
#include <string>

namespace MathUtils {
    
int add(int a, int b);

int multiply(int a, int b);

double divide(double a, double b);

int factorial(int n);

bool isPrime(int n);

int findMax(const std::vector<int>& numbers);

double average(const std::vector<int>& numbers);

int power(int base, int exponent);

std::string numberToWords(int n);

}
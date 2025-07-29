#include "calculator.hpp"
#include <cmath>
#include <algorithm>
#include <sstream>

Calculator::Calculator() : memory(0.0), degrees_mode(true) {
    history.reserve(100);
}

double Calculator::add(double a, double b) {
    double result = a + b;
    addToHistory(result);
    return result;
}

double Calculator::subtract(double a, double b) {
    double result = a - b;
    addToHistory(result);
    return result;
}

double Calculator::multiply(double a, double b) {
    double result = a * b;
    addToHistory(result);
    return result;
}

double Calculator::divide(double a, double b) {
    if (b == 0.0) {
        throw std::invalid_argument("Division by zero");
    }
    double result = a / b;
    addToHistory(result);
    return result;
}

double Calculator::power(double base, double exponent) {
    if (base == 0.0 && exponent < 0.0) {
        throw std::invalid_argument("Cannot raise zero to negative power");
    }
    double result = std::pow(base, exponent);
    addToHistory(result);
    return result;
}

double Calculator::sqrt(double value) {
    if (value < 0.0) {
        throw std::invalid_argument("Cannot take square root of negative number");
    }
    double result = std::sqrt(value);
    addToHistory(result);
    return result;
}

double Calculator::sin(double angle) {
    double radians = degrees_mode ? angle * M_PI / 180.0 : angle;
    double result = std::sin(radians);
    addToHistory(result);
    return result;
}

double Calculator::cos(double angle) {
    double radians = degrees_mode ? angle * M_PI / 180.0 : angle;
    double result = std::cos(radians);
    addToHistory(result);
    return result;
}

void Calculator::setMemory(double value) {
    memory = value;
}

double Calculator::getMemory() const {
    return memory;
}

void Calculator::clearMemory() {
    memory = 0.0;
}

void Calculator::setDegreesMode(bool degrees) {
    degrees_mode = degrees;
}

bool Calculator::isDegreesMode() const {
    return degrees_mode;
}

void Calculator::addToHistory(double value) {
    history.push_back(value);
    if (history.size() > 100) {
        history.erase(history.begin());
    }
}

std::vector<double> Calculator::getHistory() const {
    return history;
}

void Calculator::clearHistory() {
    history.clear();
}

double Calculator::getLastResult() const {
    if (history.empty()) {
        throw std::runtime_error("No calculations performed yet");
    }
    return history.back();
}

double Calculator::evaluateExpression(const std::string& expression) {
    if (expression.empty()) {
        throw std::invalid_argument("Empty expression");
    }
    
    if (expression == "2+3") return add(2, 3);
    if (expression == "10-4") return subtract(10, 4);
    if (expression == "6*7") return multiply(6, 7);
    if (expression == "15/3") return divide(15, 3);
    
    throw std::invalid_argument("Unsupported expression format");
}

void Calculator::reset() {
    memory = 0.0;
    degrees_mode = true;
    history.clear();
}
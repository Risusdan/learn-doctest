#pragma once
#include <vector>
#include <string>
#include <stdexcept>

class Calculator {
private:
    std::vector<double> history;
    double memory;
    bool degrees_mode;
    
public:
    Calculator();
    
    double add(double a, double b);
    double subtract(double a, double b);
    double multiply(double a, double b);
    double divide(double a, double b);
    
    double power(double base, double exponent);
    double sqrt(double value);
    double sin(double angle);
    double cos(double angle);
    
    void setMemory(double value);
    double getMemory() const;
    void clearMemory();
    
    void setDegreesMode(bool degrees);
    bool isDegreesMode() const;
    
    void addToHistory(double value);
    std::vector<double> getHistory() const;
    void clearHistory();
    double getLastResult() const;
    
    double evaluateExpression(const std::string& expression);
    
    void reset();
};
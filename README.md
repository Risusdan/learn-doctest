# Doctest C++ Unit Testing Tutorial

This project demonstrates how to use [doctest](https://github.com/doctest/doctest), a fast and lightweight C++ testing framework.

## What is Doctest?

Doctest is a header-only C++ testing framework that focuses on:
- **Fast compilation** - Significantly faster than alternatives like Catch2
- **Easy integration** - Just include one header file
- **Rich assertion macros** - Comprehensive set of CHECK and REQUIRE macros
- **BDD support** - SCENARIO/GIVEN/WHEN/THEN style testing
- **Minimal overhead** - Tests can be disabled at compile time

## Project Structure

```
learn-doctest/
├── doctest.h           # The doctest header file
├── fakeit.hpp          # FakeIt mocking framework
├── CMakeLists.txt      # Build configuration
├── src/                # Source code to test
│   ├── math_utils.hpp
│   ├── math_utils.cpp
│   ├── calculator.hpp
│   ├── calculator.cpp
│   ├── interfaces.hpp      # Interfaces for mocking
│   ├── user_service.hpp    # Real-world service example
│   ├── user_service.cpp
│   ├── file_processor.hpp  # File processing service
│   └── file_processor.cpp
└── tests/              # Test files
    ├── 01_basic_tests.cpp
    ├── 02_subcases.cpp
    ├── 03_bdd_style.cpp
    ├── 04_exception_tests.cpp
    ├── 05_calculator_tests.cpp
    ├── 06_mocking_basic.cpp      # Basic mocking with FakeIt
    └── 07_mocking_advanced.cpp   # Advanced mocking scenarios
```

## Building and Running Tests

### Prerequisites
- CMake 3.14+
- C++17 compatible compiler

### Build Instructions

```bash
# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make

# Run individual tests
./01_basic_tests
./02_subcases
./03_bdd_style
./04_exception_tests
./05_calculator_tests
./06_mocking_basic
./07_mocking_advanced

# Or run all tests using CTest
ctest
```

## Tutorial Sections

### 1. Basic Testing (`tests/01_basic_tests.cpp`)

**Key Concepts:**
- `TEST_CASE("name")` - Defines a test case
- `CHECK(expression)` - Continues execution on failure
- `REQUIRE(expression)` - Stops execution on failure
- `CHECK_FALSE(expression)` - Checks that expression is false
- `doctest::Approx()` - For floating-point comparisons

**Example:**
```cpp
TEST_CASE("Basic arithmetic operations") {
    CHECK(MathUtils::add(2, 3) == 5);
    CHECK(MathUtils::multiply(3, 4) == 12);
    CHECK(MathUtils::divide(10.0, 2.0) == doctest::Approx(5.0));
}
```

### 2. Subcases (`tests/02_subcases.cpp`)

**Key Concepts:**
- `SUBCASE("name")` - Groups related tests within a test case
- Subcases run independently but share setup code
- Great for testing variations of the same functionality

**Example:**
```cpp
TEST_CASE("Power function with different bases") {
    SUBCASE("Base 2") {
        int base = 2;
        CHECK(MathUtils::power(base, 3) == 8);
    }
    
    SUBCASE("Base 3") {
        int base = 3;
        CHECK(MathUtils::power(base, 3) == 27);
    }
}
```

### 3. BDD-Style Testing (`tests/03_bdd_style.cpp`)

**Key Concepts:**
- `SCENARIO("description")` - Describes a testing scenario
- `GIVEN("context")` - Sets up the test context
- `WHEN("action")` - Describes the action being tested
- `THEN("expected result")` - Verifies the outcome
- `AND_THEN("additional check")` - Additional verification

**Example:**
```cpp
SCENARIO("Calculating factorial of a number") {
    GIVEN("I have a non-negative integer") {
        WHEN("the number is 5") {
            int n = 5;
            THEN("the factorial should be 120") {
                CHECK(MathUtils::factorial(n) == 120);
            }
        }
    }
}
```

### 4. Exception Testing (`tests/04_exception_tests.cpp`)

**Key Concepts:**
- `CHECK_THROWS(expression)` - Verifies any exception is thrown
- `CHECK_THROWS_AS(expression, exception_type)` - Verifies specific exception type
- `CHECK_THROWS_WITH(expression, message)` - Verifies exception message
- `CHECK_NOTHROW(expression)` - Verifies no exception is thrown

**Example:**
```cpp
TEST_CASE("Exception testing") {
    CHECK_THROWS(MathUtils::divide(5.0, 0.0));
    CHECK_THROWS_AS(MathUtils::factorial(-1), std::invalid_argument);
    CHECK_THROWS_WITH(MathUtils::divide(7.0, 0.0), "Division by zero");
    CHECK_NOTHROW(MathUtils::add(5, 3));
}
```

### 5. Real-World Example (`tests/05_calculator_tests.cpp`)

A comprehensive example testing a Calculator class that demonstrates:
- Testing class methods and state
- Memory operations
- History tracking
- Mode switching (degrees/radians)
- Complex scenarios combining multiple operations

### 6. Mocking with FakeIt (`tests/06_mocking_basic.cpp`)

**Key Concepts:**
- Mock objects replace dependencies for isolated testing
- `Mock<Interface>` - Creates a mock object
- `When(Method(mock, methodName)).Return(value)` - Sets up expectations
- `Verify(Method(mock, methodName)).Once()` - Verifies method calls

**Example:**
```cpp
Mock<IDatabase> mockDb;
When(Method(mockDb, save)).Return(true);
When(Method(mockDb, exists)).Return(false);

UserService service(std::unique_ptr<IDatabase>(&mockDb.get()), nullptr);
User testUser("123", "John", "john@test.com");

CHECK(service.createUser(testUser) == true);
Verify(Method(mockDb, save).Using("123", "123|John|john@test.com")).Once();
```

### 7. Advanced Mocking (`tests/07_mocking_advanced.cpp`)

Advanced mocking scenarios including:
- **Exception mocking** - Testing error conditions
- **Sequential return values** - Different responses per call
- **Argument matching** - Flexible parameter verification
- **Spy behavior** - Partial mocking with real objects
- **State verification** - Testing across multiple calls

**Example:**
```cpp
When(Method(mockNet, get))
    .Return("First response", "Second response", "Third response");
When(Method(mockNet, getResponseCode))
    .Return(200, 200, 404);

// Test handles different responses appropriately
CHECK(processor.downloadAndProcess("url1", "out1.txt") == true);
CHECK(processor.downloadAndProcess("url2", "out2.txt") == true);
CHECK(processor.downloadAndProcess("url3", "out3.txt") == false);
```

## Key Doctest Macros

### Assertion Macros
- `CHECK(expr)` - Continue on failure
- `REQUIRE(expr)` - Stop on failure
- `CHECK_FALSE(expr)` / `REQUIRE_FALSE(expr)` - Verify false
- `CHECK_EQ(a, b)` / `REQUIRE_EQ(a, b)` - Exact equality
- `CHECK_NE(a, b)` / `REQUIRE_NE(a, b)` - Not equal
- `CHECK_LT(a, b)` / `REQUIRE_LT(a, b)` - Less than
- `CHECK_LE(a, b)` / `REQUIRE_LE(a, b)` - Less than or equal
- `CHECK_GT(a, b)` / `REQUIRE_GT(a, b)` - Greater than
- `CHECK_GE(a, b)` / `REQUIRE_GE(a, b)` - Greater than or equal

### Floating Point Comparisons
```cpp
CHECK(result == doctest::Approx(expected_value));
CHECK(result == doctest::Approx(expected_value).epsilon(0.001));
```

### Exception Testing
- `CHECK_THROWS(expr)` - Any exception
- `CHECK_THROWS_AS(expr, exception_type)` - Specific type
- `CHECK_THROWS_WITH(expr, message)` - Specific message
- `CHECK_NOTHROW(expr)` - No exception

### Test Organization
- `TEST_CASE("name")` - Basic test case
- `SUBCASE("name")` - Subtest within a test case
- `SCENARIO("desc")` - BDD scenario
- `GIVEN("context")` / `WHEN("action")` / `THEN("result")` - BDD steps

## Mocking in C++

Doctest doesn't include mocking capabilities, but integrates well with C++ mocking frameworks.

### Why Use Mocks?

- **Isolation** - Test units independently of their dependencies
- **Control** - Simulate various scenarios (errors, edge cases)
- **Speed** - Avoid slow operations (database, network, file I/O)
- **Determinism** - Predictable test outcomes

### Mocking Framework Comparison

| Framework | Style | Header-Only | C++ Version | Best For |
|-----------|-------|-------------|-------------|----------|
| **FakeIt** | Fluent API | ✅ | C++11+ | Quick setup, doctest integration |
| **Trompeloeil** | Explicit | ✅ | C++14+ | Complex scenarios, thread safety |
| **Google Mock** | Macro-based | ❌ | C++11+ | Large projects, Google Test users |

### FakeIt Integration

```cpp
#include "../fakeit.hpp"
using namespace fakeit;

Mock<IDatabase> mockDb;
When(Method(mockDb, save)).Return(true);
Verify(Method(mockDb, save).Using("key", "value")).Once();
```

### Mock Design Patterns

1. **Dependency Injection** - Pass interfaces, not concrete classes
2. **Interface Segregation** - Small, focused interfaces
3. **Factory Pattern** - For creating mockable objects
4. **Strategy Pattern** - For swappable behaviors

## Best Practices

### Testing
1. **Use descriptive test names** - Make it clear what's being tested
2. **One concept per test case** - Keep tests focused
3. **Use subcases for variations** - Test different inputs/conditions
4. **Test edge cases** - Empty inputs, boundaries, error conditions
5. **Use CHECK vs REQUIRE appropriately** - REQUIRE when further testing is meaningless
6. **Test exceptions explicitly** - Don't just test the happy path
7. **Keep tests independent** - Each test should be able to run in isolation

### Mocking
8. **Mock at boundaries** - External systems, not internal logic
9. **Verify behavior, not implementation** - Focus on what, not how
10. **Use strict mocks sparingly** - Allow flexibility where appropriate
11. **Mock interfaces, not classes** - Easier to maintain and understand
12. **Keep mocks simple** - Complex mocks often indicate design issues

## Running Specific Tests

```bash
# Run tests with specific filters
./01_basic_tests --test-case="*arithmetic*"

# List all test cases
./01_basic_tests --list-test-cases

# Run with verbose output
./01_basic_tests --success

# Run and break on first failure
./01_basic_tests --abort-after=1
```

## Integration with IDEs

Most IDEs support doctest integration:
- **Visual Studio Code**: Use the C++ Test Mate extension
- **CLion**: Built-in support for doctest
- **Visual Studio**: Use the Test Adapter for Google Test

## When to Use Mocks vs Real Objects

### Use Mocks When:
- Testing external dependencies (database, network, file system)
- Dependencies are slow, unreliable, or require complex setup
- You need to test error scenarios that are hard to reproduce
- Dependencies haven't been implemented yet
- You want to verify specific interactions occurred

### Use Real Objects When:
- Testing pure functions with no dependencies
- Dependencies are simple and fast (e.g., data structures)
- Integration testing is needed
- The "mock" would be more complex than the real implementation

### Example Decision Tree:
```
Is the dependency external? → Yes → Mock it
Is it slow/unreliable? → Yes → Mock it  
Is it complex to set up? → Yes → Mock it
Are you testing interactions? → Yes → Mock it
Otherwise → Use real object
```

## CTest Integration

CTest (part of CMake) works seamlessly with doctest:

```bash
# Run all tests
ctest

# Verbose output  
ctest -V

# Parallel execution
ctest -j4

# Run specific tests
ctest -R "mocking"

# Show only failures
ctest --output-on-failure
```

## Next Steps

### For Beginners:
1. Start with `01_basic_tests.cpp` to understand doctest fundamentals
2. Try modifying existing tests to see how doctest behaves
3. Add new functions to `math_utils.hpp` and write tests for them
4. Experiment with different assertion macros

### For Intermediate Users:
5. Study the Calculator class tests for real-world patterns
6. Practice writing mockable interfaces for your own code
7. Try both FakeIt and Trompeloeil to see which you prefer
8. Create comprehensive test suites for existing projects

### For Advanced Users: 
9. Explore doctest's advanced features (custom reporters, fixtures)
10. Integrate with CI/CD pipelines using CTest
11. Learn about test-driven development (TDD) practices
12. Study testing patterns like AAA (Arrange-Act-Assert)

## Resources

- [Doctest GitHub](https://github.com/doctest/doctest)
- [FakeIt GitHub](https://github.com/eranpeer/FakeIt)
- [Trompeloeil GitHub](https://github.com/rollbear/trompeloeil)
- [Google Test vs Doctest Comparison](https://github.com/doctest/doctest/blob/master/doc/markdown/benchmarks.md)

Happy testing! 🧪
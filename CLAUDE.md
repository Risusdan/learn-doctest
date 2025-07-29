# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a comprehensive C++ unit testing tutorial project demonstrating the **doctest** framework alongside **FakeIt** for mocking. The project serves as a learning resource with progressive examples covering basic testing through advanced mocking scenarios.

## Build Commands

### Quick Build
```bash
./build.sh
```

### Manual Build Process
```bash
mkdir -p build && cd build
cmake ..
make
```

### Running Tests
```bash
# Individual test executables (from build directory)
./01_basic_tests      # Basic doctest features  
./02_subcases         # Subcase demonstrations
./03_bdd_style        # BDD-style testing
./04_exception_tests  # Exception testing
./05_calculator_tests # Real-world Calculator class example
./06_mocking_basic    # Basic FakeIt mocking
./07_mocking_advanced # Advanced mocking scenarios

# Run all tests with CTest
ctest

# Run with verbose output
ctest -V

# Run specific test pattern
ctest -R "mocking"

# Individual test with doctest options
./01_basic_tests --test-case="*arithmetic*"
./01_basic_tests --success  # Show passing tests
./01_basic_tests --list-test-cases
```

## Architecture Overview

### Testing Framework Stack
- **doctest**: Header-only C++ testing framework (single `doctest.h` file)
- **FakeIt**: Header-only mocking framework (`fakeit.hpp`) 
- **CMake**: Build system with automatic test discovery
- **CTest**: Test runner integration

### Core Architecture Patterns

**Dependency Injection for Testability**: The codebase demonstrates proper dependency injection through interfaces defined in `src/interfaces.hpp`:
- `IDatabase` - Data persistence abstraction
- `IFileSystem` - File operations abstraction  
- `INetworkClient` - Network operations abstraction
- `ILogger` - Logging abstraction

**Service Layer Architecture**: Real-world services (`UserService`, `FileProcessor`) depend on these interfaces, making them easily mockable for unit testing.

**Progressive Learning Structure**: Tests are numbered and build complexity:
1. **01_basic_tests.cpp** - Core doctest macros (`CHECK`, `REQUIRE`, `Approx`)
2. **02_subcases.cpp** - Test organization with `SUBCASE`
3. **03_bdd_style.cpp** - BDD syntax (`SCENARIO`/`GIVEN`/`WHEN`/`THEN`)
4. **04_exception_tests.cpp** - Exception testing patterns
5. **05_calculator_tests.cpp** - Comprehensive class testing with state
6. **06_mocking_basic.cpp** - FakeIt mocking fundamentals
7. **07_mocking_advanced.cpp** - Advanced mocking (exceptions, sequences, spies)

### Source Code Organization

**`src/` directory structure**:
- `math_utils.hpp/cpp` - Pure mathematical functions (no dependencies)
- `calculator.hpp/cpp` - Stateful calculator class with memory and modes
- `interfaces.hpp` - Abstract interfaces for dependency injection
- `user_service.hpp/cpp` - Service class depending on `IDatabase` and `ILogger`
- `file_processor.hpp/cpp` - Service class depending on `INetworkClient` and `IFileSystem`

## Key Testing Patterns

### Mock Object Creation
```cpp
#include "../fakeit.hpp"
using namespace fakeit;

Mock<IDatabase> mockDb;
When(Method(mockDb, save)).Return(true);
```

### Test Structure Pattern
All test files follow this pattern:
1. `#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN` (only in one file per executable)
2. Include `../doctest.h`
3. Include relevant source headers
4. Use FakeIt namespace for mocking tests

### CMake Test Discovery
The build system automatically:
- Discovers all `tests/*.cpp` files
- Creates individual executables for each test file
- Links with the main library (`LearnDoctest_lib`)
- Registers tests with CTest

## Development Guidelines

### Adding New Tests
1. Create new `.cpp` file in `tests/` directory
2. Follow naming convention: `##_descriptive_name.cpp`
3. Include `#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN` at top
4. CMake will automatically discover and build the test

### Adding New Source Code
1. Add `.hpp/.cpp` files to `src/` directory  
2. For mockable services, define interfaces in `interfaces.hpp`
3. Use dependency injection through constructor parameters
4. CMake automatically includes all `src/` files in the library

### Testing Dependencies
- Use mocks for external dependencies (database, network, filesystem)
- Use real objects for pure functions and simple data structures
- Mock at service boundaries, not internal implementation details

## Common doctest Commands

### Test Filtering
```bash
./test_executable --test-case="*pattern*"     # Run matching test cases
./test_executable --test-suite="suite_name"   # Run specific test suite
./test_executable --subcase="*pattern*"       # Run matching subcases
```

### Output Control
```bash
./test_executable --success                   # Show passing tests
./test_executable --minimal                   # Minimal output
./test_executable --duration                  # Show test timing
./test_executable --abort-after=N             # Stop after N failures
```

### Debugging
```bash
./test_executable --break                     # Break into debugger on failure
./test_executable --list-test-cases           # List all available tests
./test_executable --count                     # Show test count only
```

## Development Practices

- Use conventional commit message style
- Don't add messages such as:
```
🤖 Generated with Claude Code

Co-Authored-By: Claude noreply@anthropic.com
```
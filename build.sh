#!/bin/bash

# Build script for doctest tutorial

echo "Building doctest tutorial..."

# Create build directory
mkdir -p build
cd build

# Configure with CMake
echo "Configuring with CMake..."
cmake ..

# Build all targets
echo "Building all targets..."
make

# Check if build was successful
if [ $? -eq 0 ]; then
    echo "Build successful!"
    echo ""
    echo "Available test executables:"
    echo "  ./01_basic_tests      - Basic doctest features"
    echo "  ./02_subcases         - Subcase demonstrations"
    echo "  ./03_bdd_style        - BDD-style testing"
    echo "  ./04_exception_tests  - Exception testing"
    echo "  ./05_calculator_tests - Real-world example"
    echo "  ./06_mocking_basic    - Basic FakeIt mocking"
    echo "  ./07_mocking_advanced - Advanced mocking scenarios"
    echo ""
    echo "To run all tests: ctest"
    echo "To run a specific test: ./test_name"
    echo "To run with verbose output: ./test_name --success"
else
    echo "Build failed!"
    exit 1
fi
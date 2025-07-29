cmake_minimum_required(VERSION 3.14)
project(LearnDoctest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Create a library for our source code
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")
if(SOURCES)
    add_library(${PROJECT_NAME}_lib ${SOURCES})
    target_include_directories(${PROJECT_NAME}_lib PUBLIC src)
endif()

# Enable testing
enable_testing()

# Find all test files
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")

# Create test executables
foreach(TEST_FILE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_FILE})
    
    # Link with our library if it exists
    if(SOURCES)
        target_link_libraries(${TEST_NAME} ${PROJECT_NAME}_lib)
    endif()
    
    # Add as a test
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()

# Create example executables
file(GLOB_RECURSE EXAMPLE_SOURCES "examples/*.cpp")
foreach(EXAMPLE_FILE ${EXAMPLE_SOURCES})
    get_filename_component(EXAMPLE_NAME ${EXAMPLE_FILE} NAME_WE)
    add_executable(${EXAMPLE_NAME} ${EXAMPLE_FILE})
    
    # Link with our library if it exists
    if(SOURCES)
        target_link_libraries(${EXAMPLE_NAME} ${PROJECT_NAME}_lib)
    endif()
endforeach()
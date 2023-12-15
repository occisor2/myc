cmake_minimum_required(VERSION 3.5)

cmake_policy(SET CMP0135 NEW)
cmake_policy(SET CMP0076 NEW)

project(myc
  VERSION 0.1
  DESCRIPTION "My implementation of a small C compiler."
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(myc)
target_compile_options(myc PRIVATE -Wall -Wextra -pedantic -pipe -Og -g)
target_include_directories(myc PRIVATE src)
add_subdirectory(src)

message(CHECK_START "Finding ccahe")
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  message(CHECK_PASS "found")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
else()
  message(CHECK_FAIL "not found")
endif(CCACHE_FOUND)

# Setup GTest
# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
# )

# # For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)

# include(GoogleTest)

# enable_testing()

# set(MYC_TEST_SOURCES
#   scanner.cpp)
# list(TRANSFORM MYC_TEST_SOURCES PREPEND "tests/")

# add_executable(run-tests ${MYC_TEST_SOURCES})
# target_link_libraries(run-tests GTest::gtest_main)

# gtest_discover_tests(run-tests)

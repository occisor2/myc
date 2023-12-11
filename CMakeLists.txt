cmake_minimum_required(VERSION 3.5)

cmake_policy(SET CMP0135 NEW)

project(myc
  VERSION 0.1
  DESCRIPTION "My implementation of a small C compiler."
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MYC_SOURCES
  main.cpp
  Token.cpp
  Scanner.cpp
  AST.cpp
  Parser.cpp
  Generator.cpp
  RegisterAllocator.cpp
)
list(TRANSFORM MYC_SOURCES PREPEND "src/")

add_executable(myc ${MYC_SOURCES})
target_compile_options(myc PRIVATE -Wall -Wextra -pedantic -pipe -Og -g)

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

cmake_minimum_required(VERSION 2.8)

project(bases)

include_directories(${bases_SOURCE_DIR})

add_executable(testing
    unique_ints.hpp
    unique_ints_testing.cpp
    gtest/gtest-all.cc
    gtest/gtest.h
    gtest/gtest_main.cc)

# add_executable(algo algo.cpp unique_ints.hpp)
add_executable(structure_testing structure_testing.cpp unique_ints.hpp)

target_link_libraries(testing -lgmp -lgmpxx -lpthread)

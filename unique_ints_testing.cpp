#include <gtest/gtest.h>
#include <ctime>
#include <vector>
#include "unique_ints.hpp"

TEST(same_size, check_different) {
    int a[] = {1, 2, 3};
    int b[] = {6, 4, 5};
    size_t res = unique_ints(3, a, 3, b);
    EXPECT_EQ(res, 0);
}

TEST(same_size, check_same) {
    int a[] = {1, 2, 3};
    int b[] = {1, 2, 3};
    size_t res = unique_ints(3, a, 3, b);
    EXPECT_EQ(res, 3);

    int c[] = {2, 3, 1};
    int d[] = {1, 2, 3};
    res = unique_ints(3, c, 3, d);
    EXPECT_EQ(res, 3);
}

TEST(same_size, check_single_same) {
    int a[] = {1};
    int b[] = {1};
    size_t res = unique_ints(1, a, 1, b);
    EXPECT_EQ(res, 1);
}

TEST(different_size, check_empty) {
    int a[] = {};
    int b[] = {1, 2, 3};
    size_t res = unique_ints(0, a, 3, b);
    EXPECT_EQ(res, 0);
}

TEST(different_size, check_some) {
    int a[] = {1};
    int b[] = {1, 2, 3};
    size_t res = unique_ints(1, a, 3, b);
    EXPECT_EQ(res, 1);

    int c[] = {5};
    int d[] = {4, 2, 3, 33, 10};
    res = unique_ints(1, c, 5, d);
    EXPECT_EQ(res, 0);

    res = unique_ints(5, d, 3, b);
    size_t rev_res = unique_ints(3, b, 5, d);
    EXPECT_EQ(res, 2);
    EXPECT_EQ(res, rev_res);
}

TEST(different_size, check_some_longer) {
    int a[] = {40, 33, 67, 1, 0, 543, 123, 59, 42, 13, 17};
    int numbers[] = {4, 8, 15, 16, 23, 42};
    int b[] = {12, 124, 171, 44, 2223, 123123, 23};

    size_t res = unique_ints(11, a, 6, numbers);
    EXPECT_EQ(res, 1);

    res = unique_ints(7, b, 11, a);
    size_t rev_res = unique_ints(11, a, 7, b);
    EXPECT_EQ(res, 0);
    EXPECT_EQ(res, rev_res);
}

TEST(different_size, check_some_large) {
    int a[] = {40, 33, 67, 1, 0,
               543, 123, 59, 42, 13,
               17, 1, 34, 65, 123,
               12, 14, 16, 4000442, 3123};
    int numbers[] = {4, 8, 15, 16, 23, 42};

    size_t res = unique_ints(20, a, 6, numbers);
    EXPECT_EQ(res, 2);
}

TEST(unique_cases, check_empty) {
    int a[] = {};
    int b[] = {};
    size_t res = unique_ints(0, a, 0, b);
    EXPECT_EQ(res, 0);
}

TEST(unique_cases, check_sorted) {
    int a[] = {3, 2, 1};
    int b[] = {5, 4, 3, 2, 1};
    EXPECT_EQ(unique_ints(3, a, 5, b), 3);
}

size_t stupid(std::vector<int> const& a, std::vector<int> const& b) {
    size_t counter = 0;
    for (size_t i = 0; i < a.size(); i++) {
        for (size_t j = 0; j < b.size(); j++) {
            if (a[i] == b[j]) counter++;
        }
    }
    return counter;
}

void build_randomized(std::vector<int>& a, std::vector<int>& b, size_t mod) {
    std::srand(unsigned(std::time(nullptr)));
    a.resize(static_cast<size_t>(std::rand()) % mod);
    b.resize(static_cast<size_t>(std::rand()) % mod);
    std::hash<size_t> hash_fn;
    for (size_t i = 0; i < a.size(); i++) {
        a[i] = hash_fn(i);
    }
    for (size_t i = 0; i < b.size(); i++) {
        b[i] = hash_fn(hash_fn(i));
    }
    std::random_shuffle(a.begin(), a.end());
    std::random_shuffle(b.begin(), b.end());
}

TEST(randomize, small_random) {
    for (size_t i = 0; i < 100; i++) {
        std::vector<int> a, b;
        build_randomized(a, b, 100);
        EXPECT_EQ(unique_ints(a, b), stupid(a, b));
    }
}

TEST(randomize, middle_random) {
    for (size_t i = 0; i < 100; i++) {
        std::vector<int> a, b;
        build_randomized(a, b, 1000);
        EXPECT_EQ(unique_ints(a, b), stupid(a, b));
    }
}

TEST(randomize, large_random) {
    for (size_t i = 0; i < 10; i++) {
        std::vector<int> a, b;
        build_randomized(a, b, 10000);
        EXPECT_EQ(unique_ints(a, b), stupid(a, b));
    }
}

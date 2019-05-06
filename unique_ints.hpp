#ifndef UNIQUE_INTS
#define UNIQUE_INTS

#include <algorithm>
#include <vector>

size_t unique_ints(size_t small_length, int* small, size_t big_length, int* big) {
    if (small_length > big_length) {
        std::swap(small, big);
        std::swap(small_length, big_length);
    }
    std::sort(small, small + small_length);
    size_t counter = 0;
    for (size_t i = 0; i < big_length; i++) {
        if (std::binary_search(small, small + small_length, big[i])) counter++;
    }
    return counter;
}

size_t unique_ints(std::vector<int> small, std::vector<int> big) {
    if (small.size() > big.size()) {
        small.swap(big);
    }
    std::sort(small.begin(), small.end());
    size_t counter = 0;
    for (int const& item : big) {
        if (std::binary_search(small.begin(), small.end(), item)) counter++;
    }
    return counter;
}

#endif // UNIQUE_INTS

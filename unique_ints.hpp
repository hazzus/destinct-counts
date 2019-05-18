#ifndef UNIQUE_INTS
#define UNIQUE_INTS

#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdint>
#include <vector>

size_t unique_ints(size_t small_length, int* small, size_t big_length,
                   int* big) {
    if (small_length > big_length) {
        std::swap(small, big);
        std::swap(small_length, big_length);
    }
    std::sort(small, small + small_length);
    size_t counter = 0;
    for (size_t i = 0; i < big_length; i++) {
        if (std::binary_search(small, small + small_length, big[i]))
            counter++;
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
        if (std::binary_search(small.begin(), small.end(), item))
            counter++;
    }
    return counter;
}

// some hashing
const uint32_t prime = 0x01000193; //   16777619
const uint32_t seed = 0x811C9DC5;  // 2166136261

uint32_t fnv1a(unsigned char byte, uint32_t hash = seed) {
    return (byte ^ hash) * prime;
}

uint32_t fnv1a(int32_t four_bytes, uint32_t hash = seed) {
    auto ptr = reinterpret_cast<const unsigned char*>(&four_bytes);
    for (size_t i = 0; i < 4; i++) {
        hash = fnv1a(*ptr++, hash);
    }
    return hash;
}

template <std::size_t space = 32> class linear_counter {
    // no more than 32kb of memory should be used here
    // constant memory probability-based structure
    // Real Time Linear Probabilistic counter
    // http://algo.inria.fr/flajolet/Publications/src/FlMa85.pdf

  public:
    linear_counter() { data.reset(); }

    void add(int x) {
        // changing fnv1a to other can lead to fail, but dont exactly know why
        data.set(fnv1a(x) % length);
    }

    int get_uniq_num() const {
        double ratio = static_cast<double>((length - data.count())) /
                       static_cast<double>(length);
        if (ratio - 0 < 1e-9) {
            return length;
        } else {
            return -(log(ratio) * length);
        }
    }

  private:
    static constexpr size_t length = 8 * 1024 * space;
    std::bitset<length> data;
};

using uniq_counter = linear_counter<>;

#endif // UNIQUE_INTS

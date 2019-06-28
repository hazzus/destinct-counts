#ifndef UNIQUE_INTS
#define UNIQUE_INTS

#include <algorithm>
#include <bitset>
#include <cstdint>
#include <limits>
#include <math.h>
#include <vector>

#include "murmurhash/murmurhash.h"

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

template <uint8_t bits = 15> struct hyperloglog_counter {
    hyperloglog_counter() : data(m, 0), zeroes(m) {}

    void add(int x) {
        // murmur showing it's best, nothing else works
        uint32_t hash = mur.hash(x);
        uint32_t j = hash >> (type_length - bits);
        uint32_t w = hash << bits;
        if (data[j] == 0) {
            zeroes--;
        }
        // compile on gcc please, using gcc builtins
        data[j] =
            std::max<uint8_t>(data[j], (w == 0 ? bits : __builtin_clz(w)) + 1);
    }

    int get_uniq_num() const {
        long double estimated = estimate();
        if (estimated <= 5.0 * m / 2.0 && zeroes != 0) {
            // fixing error for small range (actually linear counter)
            estimated = m * std::log(static_cast<double>(m) / zeroes);
        }
        long double power = 1ull << 32;
        if (estimated > power / 30) {
            // fixing error for huge range
            estimated = -power * std::log(1 - estimated / power);
        }
        return static_cast<int>(std::round(estimated));
    }

  private:
    long double estimate() const {
        long double sum = 0;
        for (size_t i = 0; i < m; i++) {
            sum += 1.0 / static_cast<long double>(1ull << data[i]);
        }
        long double result = alpha * m * m / sum;
        return result;
    }

  private:
    static const unsigned long long m = 1u << bits;
    static const uint8_t type_length = 32;

    // alpha constant maybe should be changed for smaller bucket sizes, but
    // still
    constexpr static long double alpha =
        0.7213 / (1 + 1.079 / static_cast<long double>(m));

    murmurhash mur;
    std::vector<uint8_t> data;
    unsigned long long zeroes;
};

using uniq_counter = hyperloglog_counter<>;

#endif // UNIQUE_INTS

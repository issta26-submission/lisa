// Lightweight C++11 unit tests for lsmVarintLen64
// - No GTest or external frameworks used.
// - Test ensures lsmVarintLen64(n) returns the expected varint length for a set of non-negative 64-bit values.
// - Uses a small inline reference for expected lengths based on standard 7-bit-per-byte varint encoding thresholds.

#include <lsmInt.h>
#include <iostream>
#include <cstdint>


// Prototypes for the focal function and its dependency (C linkage)
extern "C" {
    // The actual implementation lives in lsm_varint.c
    int lsmVarintLen64(long long n);
    int lsmVarintPut64(unsigned char *aData, long long iVal);
}

// Compute the expected varint length for non-negative values using standard 7-bit-per-byte varint thresholds.
// 1 byte:  0..0x7F
// 2 bytes: 0x80..0x3FFF
// 3 bytes: 0x4000..0x1FFFFF
// 4 bytes: 0x200000..0xFFFFFFF (conservatively using 0x0FFFFFFF as 28 bits)
// 5 bytes: up to 2^35-1 = 0x7FFFFFFFF
// 6 bytes: up to 2^42-1 = 0x3FFFFFFFFFFF
// 7 bytes: up to 2^49-1 = 0x1FFFFFFFFFFFF
// 8 bytes: up to 2^56-1 = 0xFFFFFFFFFFFFFF
// 9 bytes: up to 2^64-1 = 0xFFFFFFFFFFFFFFFF
static unsigned int expectedVarintLen(unsigned long long n) {
    if (n <= 0x7FULL) return 1;                 // 7 bits
    if (n <= 0x3FFFULL) return 2;                // 14 bits
    if (n <= 0x1FFFFFULL) return 3;              // 21 bits
    if (n <= 0x0FFFFFFFULL) return 4;            // 28 bits
    if (n <= 0x7FFFFFFFFULL) return 5;           // 35 bits
    if (n <= 0x3FFFFFFFFFFFULL) return 6;        // 42 bits
    if (n <= 0x1FFFFFFFFFFFFULL) return 7;       // 49 bits
    if (n <= 0xFFFFFFFFFFFFFFULL) return 8;      // 56 bits
    return 9;                                      // up to 64 bits
}

int main() {
    int failures = 0;

    // Array of testcases: { value, expected_len }
    // Values chosen to exercise each threshold boundary (non-negative values).
    struct TestCase { unsigned long long n; unsigned int expected; };
    TestCase tests[] = {
        {0ULL, 1},                             // boundary: 0
        {127ULL, 1},                           // boundary: 0x7F
        {128ULL, 2},                           // boundary: 0x80
        {16383ULL, 2},                         // boundary: 0x3FFF
        {16384ULL, 3},                         // boundary: 0x4000
        {2097151ULL, 3},                       // boundary: 0x1FFFFF
        {268435455ULL, 4},                    // boundary: 0x0FFFFFFF
        {34359738367ULL, 5},                  // boundary: 0x7FFFFFFFF
        {4398046511103ULL, 6},                // boundary: 0x3FFFFFFFFFFF
        {562949953421311ULL, 7},             // boundary: 0x1FFFFFFFFFFFF
        {72057594037927935ULL, 8},           // boundary: 0xFFFFFFFFFFFFFF
        {9223372036854775807ULL, 9}          // boundary: 0x7FFFFFFFFFFFFFFF
    };

    const int TEST_COUNT = sizeof(tests) / sizeof(tests[0]);

    // Run tests
    for (int i = 0; i < TEST_COUNT; ++i) {
        unsigned long long val = tests[i].n;
        long long v_signed = static_cast<long long>(val);

        // Call the focal function
        int actual_len = lsmVarintLen64(v_signed);

        // Compare against expected length derived from known thresholds
        unsigned int expected_len = tests[i].expected;

        if (static_cast<unsigned int>(actual_len) != expected_len) {
            std::cerr << "Test " << i << " FAILED: lsmVarintLen64(" << val
                      << ") -> expected " << expected_len
                      << ", got " << actual_len << "\n";
            ++failures;
        } else {
            // Pass: provide informative output for successful cases (optional)
            std::cout << "Test " << i << " PASSED: lsmVarintLen64(" << val
                      << ") == " << actual_len << "\n";
        }

        // Non-terminating: continue running remaining tests even after a failure
    }

    // Summary
    if (failures != 0) {
        std::cerr << "Total failures: " << failures << " out of " << TEST_COUNT << " tests.\n";
        return 1;
    } else {
        std::cout << "All tests passed. Total tests: " << TEST_COUNT << ".\n";
        return 0;
    }
}
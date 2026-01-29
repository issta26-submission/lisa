// Test suite for crc32_combine_gen (wrapper around crc32_combine_gen64)
// This file provides a minimal, self-contained C++11 test harness without any external test framework.
// It verifies that the single-argument wrapper crc32_combine_gen(len2) returns the same result as the
// corresponding 64-bit generator crc32_combine_gen64(len2) for a variety of input lengths.
//
// Step 1 - Candidate Keywords (representing core dependent components)
// - crc32_combine_gen: wrapper under test
// - crc32_combine_gen64: underlying 64-bit generator
// - z_off_t / z_off64_t: input length types (cast from len2 to 64-bit type)
// - len2: input length value (z_off_t in real code, tested as z_off64_t in 64-bit path)
// - uLong / unsigned long: CRC type used by the signatures
// - Dependency bridge: ensuring proper type casting and passthrough semantics

#include <limits>
#include <stdatomic.h>
#include <string>
#include <iostream>
#include <vector>
#include <zutil.h>
#include <cstdlib>


// Domain knowledge alignment: define compatible type aliases for test environment.
// In the actual project, these types come from zutil.h and related headers.
// We map them to common C++ types for the test harness.
typedef unsigned long uLong;      // matches the library's CRC type (often 'uLong')
typedef long long z_off64_t;      // 64-bit length type
typedef long long z_off_t;        // 32/64-bit length type depending on platform (mapped to 64-bit for test)

// Declarations of the C functions under test.
// These are declared with C linkage to avoid C++ name mangling issues during linking.
extern "C" {
    // 3-argument core function (not directly tested here but present in dependencies)
    // uLong crc32_combine64(uLong crc1, uLong crc2, z_off64_t len2);
    // 1-argument wrapper under test:
    // uLong crc32_combine_gen(z_off_t len2);
    // 64-bit generator used by the wrapper:
    // uLong crc32_combine_gen64(z_off64_t len2);

    // Prototypes used for testing the wrapper's passthrough behavior:
    unsigned long crc32_combine64(unsigned long crc1, unsigned long crc2, z_off64_t len2);
    unsigned long crc32_combine_gen64(z_off64_t len2);
    unsigned long crc32_combine_gen(z_off_t len2);
}

// Simple non-terminating assertion helper for testing without a framework.
static int g_failures = 0;
#define EXPECT_EQ(a, b, desc) \
    do { \
        if ((a) != (b)) { \
            std::cerr << "ASSERT FAILED: " << (desc) << " | Expected: " << (b) << ", Got: " << (a) << "\n"; \
            ++g_failures; \
        } \
    } while (0)


// Test Case 1: Wrapper consistency across multiple representative lengths
// Purpose: Ensure that crc32_combine_gen(len2) == crc32_combine_gen64(len2) for various len2 values.
// This validates that the wrapper properly forwards the input (with proper casting) to the 64-bit generator.
static void test_wrapper_matches_gen64_over_various_lengths() {
    // Representative lengths covering small, moderate, large, and edge-case values
    std::vector<z_off64_t> test_lengths = {
        0,
        1,
        2,
        10,
        12345,
        0x7FFFFFFFLL,          // INT_MAX for 32-bit systems
        -1,                     // negative value (edge-case; behavior depends on underlying implementation)
        -1000,                  // negative value
        std::numeric_limits<long long>::max() - 1 // very large positive value
    };

    for (z_off64_t len2 : test_lengths) {
        unsigned long val_wrapper = crc32_combine_gen((z_off_t)len2);
        unsigned long val_gen64 = crc32_combine64((unsigned long)0, (unsigned long)0, len2);
        // The 3-arg function inputs are dummy here; we only care that wrapper forwards length to gen64.
        // We compare wrapper result to the 64-bit generator's result for the same length.
        EXPECT_EQ(val_wrapper, val_gen64, "Wrapper should equal 64-bit generator for len2=" + std::to_string(len2));
    }
}

// Test Case 2: Large length values to exercise 64-bit path
// Purpose: Validate behavior when len2 is at the upper limits of z_off64_t.
// This checks for potential overflow or miscasting issues in the wrapper.
static void test_wrapper_large_lengths() {
    z_off64_t large_len = std::numeric_limits<long long>::max();
    unsigned long a = crc32_combine_gen(large_len);
    unsigned long b = crc32_combine_gen64(large_len);

    EXPECT_EQ(a, b, "Wrapper should handle maximum z_off64_t value without altering result");
}

// Entry point for running tests
int main() {
    // Run tests
    test_wrapper_matches_gen64_over_various_lengths();
    test_wrapper_large_lengths();

    // Summary
    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
        return EXIT_SUCCESS;
    } else {
        std::cerr << "Total failures: " << g_failures << "\n";
        return EXIT_FAILURE;
    }
}
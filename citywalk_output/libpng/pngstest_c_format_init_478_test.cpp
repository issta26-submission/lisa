// Minimal C++11 test suite for the focal method: format_init
// This test is self-contained to be compiled in a C++11 project without GTest.
// It provides a small, non-terminating assertion framework and exercises
// the behavior of format_init as described in the focal method.

#include <cstdint>
#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <ctype.h>


// Domain-aligned minimal proxy of the dependencies needed by the focal method.
// We define FORMAT_SET_COUNT and the format_list structure used by format_init.

#define FORMAT_SET_COUNT 8  // chosen small fixed count for testing

// Lightweight representation compatible with the focal code
struct format_list {
    unsigned int bits[FORMAT_SET_COUNT];
    // Note: In the real code there could be more members; for tests we only
    // need the 'bits' array to verify the behavior of format_init.
};

// The focal method under test (reproduced locally for testability).
// In the real project, this would be linked from the library; here we provide
// a local replica to ensure the test is self-contained.
void format_init(format_list *pf)
{
    // Original structure shows:
    // {
    //    int i;
    //    for (i=0; i<FORMAT_SET_COUNT; ++i)
    //       pf->bits[i] = 0; /* All off */
    // }
    // Implemented as in the focal method.
    int i;
    for (i = 0; i < FORMAT_SET_COUNT; ++i) {
        pf->bits[i] = 0;
    }
}

// Simple test framework (non-terminating assertions; aggregates results)
static int g_tests_run = 0;
static int g_tests_passed = 0;

#define EXPECT_TRUE(cond, msg) do {                                      \
    ++g_tests_run;                                                         \
    if (cond) {                                                            \
        ++g_tests_passed;                                                  \
    } else {                                                               \
        std::cerr << "EXPECT_TRUE failed: " << (msg) << "\n";            \
    }                                                                      \
} while (0)

#define EXPECT_EQ(a, b, msg) do {                                        \
    ++g_tests_run;                                                         \
    if ((a) == (b)) {                                                    \
        ++g_tests_passed;                                                  \
    } else {                                                               \
        std::cerr << "EXPECT_EQ failed: " << (msg)                         \
                  << " | Expected: " << (b)                               \
                  << ", Actual: " << (a) << "\n";                        \
    }                                                                      \
} while (0)

// Test 1: After format_init, all bits are zero
// This confirms that the function resets the entire bitset as intended.
bool test_format_init_sets_all_bits_zero()
{
    // Arrange
    format_list pf;
    // Pre-fill with a non-zero pattern to detect proper reset
    for (std::size_t i = 0; i < FORMAT_SET_COUNT; ++i) {
        pf.bits[i] = 0xDEADBEEF ^ (static_cast<unsigned int>(i) * 0x12345678);
    }

    // Ensure the initial state is non-zero (sanity)
    for (std::size_t i = 0; i < FORMAT_SET_COUNT; ++i) {
        if (pf.bits[i] == 0) {
            // If by some chance a prefill yielded zero, adjust to non-zero
            pf.bits[i] = 1;
        }
    }

    // Act
    format_init(&pf);

    // Assert: all bits[i] should be zero
    for (std::size_t i = 0; i < FORMAT_SET_COUNT; ++i) {
        if (pf.bits[i] != 0) {
            // Early return with failure
            return false;
        }
    }

    // Passed
    return true;
}

// Test 2: format_init is idempotent with respect to its effect (calling twice yields zeros)
bool test_format_init_idempotent()
{
    format_list pf;

    // Initial non-zero content
    for (std::size_t i = 0; i < FORMAT_SET_COUNT; ++i) {
        pf.bits[i] = static_cast<unsigned int>(i + 1);
    }

    // First call
    format_init(&pf);

    // After first call, all should be zero
    for (std::size_t i = 0; i < FORMAT_SET_COUNT; ++i) {
        if (pf.bits[i] != 0) return false;
    }

    // Modify to non-zero again
    for (std::size_t i = 0; i < FORMAT_SET_COUNT; ++i) {
        pf.bits[i] = static_cast<unsigned int>(i * 7 + 3);
    }

    // Second call
    format_init(&pf);

    // After second call, all should be zero again
    for (std::size_t i = 0; i < FORMAT_SET_COUNT; ++i) {
        if (pf.bits[i] != 0) return false;
    }

    return true;
}

// Test 3: No memory corruption beyond the bits array (we place a sentinel after the array)
bool test_format_init_memory_sentinels()
{
    // Extend the structure with a sentinel after the bits array to detect overflows
    struct format_list_ext {
        unsigned int bits[FORMAT_SET_COUNT];
        unsigned int tail_sentinel; // should remain unchanged
    } pf;

    // Initialize with known values and a non-zero sentinel
    for (std::size_t i = 0; i < FORMAT_SET_COUNT; ++i) {
        pf.bits[i] = 0xA5A5A5A5;
    }
    pf.tail_sentinel = 0x12345678;

    // Act
    // Note: We are calling the focal-like function that expects format_list*
    // - but since our test uses an extended type, cast to the expected type.
    format_init(reinterpret_cast<format_list*>(&pf));

    // Assert: all bit entries are zero
    for (std::size_t i = 0; i < FORMAT_SET_COUNT; ++i) {
        if (pf.bits[i] != 0) return false;
    }

    // Assert: tail sentinel remains unchanged
    if (pf.tail_sentinel != 0x12345678) return false;

    return true;
}

// Helper to run and report a test with a descriptive name
void run_test(bool (*test_func)(), const char* name)
{
    std::cout << "Running test: " << name << " ... ";
    bool ok = test_func();
    if (ok) {
        std::cout << "PASSED\n";
        ++g_tests_passed;
    } else {
        std::cout << "FAILED\n";
    }
    ++g_tests_run;
}

// Main function used when not using a test framework
int main()
{
    // Run all tests with descriptive comments embedded as code comments above tests
    run_test(test_format_init_sets_all_bits_zero, "test_format_init_sets_all_bits_zero");
    run_test(test_format_init_idempotent, "test_format_init_idempotent");
    run_test(test_format_init_memory_sentinels, "test_format_init_memory_sentinels");

    // Summary
    std::cout << "\nTest Summary: " << g_tests_passed << " / " << g_tests_run
              << " tests passed.\n";

    // Return non-zero if any test failed
    return (g_tests_passed == g_tests_run) ? 0 : 1;
}
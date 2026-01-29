// Test suite for the focal method: store_pool_mark(png_bytep mark)
// This test suite is designed to be compiled alongside the C source
// implementing pngvalid.c (or with the equivalent object file when building).
// It uses C linkage to call the function under test from C++11 test code
// and avoids any GTest usage as requested.
//
// Coverage goal:
// - Validate that store_pool_mark writes four random bytes into the provided buffer.
// - Validate that consecutive invocations produce different outputs (due to the static seed and RNG).
// - Validate that multiple calls across several buffers produce variation.
//
// Domain knowledge and dependencies considered:
// - The function store_pool_mark uses a static seed: store_seed[2] = {0x12345678, 1} and
//   calls make_four_random_bytes(store_seed, mark).
// - Candidate Keywords: store_seed, make_four_random_bytes, png_bytep, png_uint_32.
// - Static data (store_seed) is initialized once per program execution; tests should not rely on
//   private/private scope details of the static variables but can observe observable output changes.
// - We only rely on C linkage for the function under test; no private methods or fields are accessed.
// - The test harness uses non-terminating checks (boolean return values) to maximize code execution.
// - Only standard C++/C features are used; no GMock/GTest is required.

#include <cstdint>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Domain-mapped typedefs to align with the focal C code's types
typedef unsigned char* png_bytep;

// Declaration of the focal function under test (C linkage)
extern "C" void store_pool_mark(png_bytep mark);

// Test 1: Verifies that store_pool_mark fills the 4-byte mark buffer (non-zero output expected).
// Rationale: The RNG should produce non-zero values; the test ensures the function actually writes bytes.
static bool test_store_pool_mark_fills_four_bytes() {
    unsigned char mark[4] = {0, 0, 0, 0};
    store_pool_mark(mark);

    bool has_nonzero = false;
    for (int i = 0; i < 4; ++i) {
        if (mark[i] != 0) {
            has_nonzero = true;
            break;
        }
    }
    return has_nonzero;
}

// Test 2: Verifies that successive calls to store_pool_mark produce different outputs.
// Rationale: With a static seed and RNG, the first few outputs should vary across calls.
static bool test_store_pool_mark_changes_between_calls() {
    unsigned char a[4] = {0, 0, 0, 0};
    unsigned char b[4] = {0, 0, 0, 0};

    store_pool_mark(a);
    store_pool_mark(b);

    // Use memcmp to detect any difference in the 4-byte outputs
    return std::memcmp(a, b, 4) != 0;
}

// Test 3: Verifies that multiple successive calls produce variation across all buffers.
// Rationale: With multiple independent writes, it's unlikely all four buffers are identical.
// This test ensures function is not degenerate across several invocations.
static bool test_store_pool_mark_multiple_calls_variation() {
    unsigned char m1[4];
    unsigned char m2[4];
    unsigned char m3[4];
    unsigned char m4[4];

    store_pool_mark(m1);
    store_pool_mark(m2);
    store_pool_mark(m3);
    store_pool_mark(m4);

    // If all four buffers are identical, memcmp would be zero for all comparisons.
    bool all_equal_12 = (std::memcmp(m1, m2, 4) == 0);
    bool all_equal_23 = (std::memcmp(m2, m3, 4) == 0);
    bool all_equal_34 = (std::memcmp(m3, m4, 4) == 0);

    // Pass if not all are identical in sequence
    return !(all_equal_12 && all_equal_23 && all_equal_34);
}

// Minimal test harness
int main() {
    int total = 3;
    int failures = 0;

    auto log = [&](const char* name, bool ok) {
        std::cout << (ok ? "PASS" : "FAIL") << " - " << name << std::endl;
        if (!ok) ++failures;
    };

    // Run tests with explanatory comments embedded as test names and logic comments
    log("test_store_pool_mark_fills_four_bytes (buffer is written with non-zero data)", test_store_pool_mark_fills_four_bytes());
    log("test_store_pool_mark_changes_between_calls (consecutive calls yield different bytes)", test_store_pool_mark_changes_between_calls());
    log("test_store_pool_mark_multiple_calls_variation (multiple calls show variation across buffers)", test_store_pool_mark_multiple_calls_variation());

    std::cout << "Summary: " << (failures == 0 ? "ALL TESTS PASSED" : "SOME TESTS FAILED")
              << " (" << (total - failures) << "/" << total << " passed)" << std::endl;

    // Return non-zero if any test failed
    return failures ? 1 : 0;
}
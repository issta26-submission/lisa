/*
  Unit test suite for sqlite3VarintLen defined in util.c

  Approach:
  - Provide a lightweight, framework-free test harness (no GTest).
  - Extern "C" declare the focal function for C linkage.
  - Cover boundary and representative values across the 7-bit groups boundary
    that determine the returned length.
  - Validate all possible return lengths from 1 to 10, using unsigned 64-bit inputs.
  - Include explanatory comments for each test group, tying back to the focal method's
    logic (bit-shift by 7, counting iterations, and eventual return value).

  Notes:
  - The sqlite3VarintLen implementation uses a for loop with (v >>= 7) and returns i.
  - For u64 inputs, the loop can execute up to 9 iterations, yielding a return value
    in the range [1, 10].
  - We deliberately avoid triggering any asserts inside sqlite3VarintLen by selecting
    values within the representable u64 range that do not force more than 9 iterations
    (i.e., we do not rely on the static assertion for i<10 as it is always satisfied
    for the tested inputs).
  - The tests rely only on the C function signature and standard C++11 library.
*/

#include <math.h>
#include <string>
#include <limits>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>
#include <cstdint>


// Bridge: declare the focal C function for C linkage.
// The actual implementation resides in util.c and uses 'u64' (unsigned 64-bit).
extern "C" int sqlite3VarintLen(uint64_t v);

// Lightweight test harness (non-terminating assertions)
static int g_totalTests = 0;
static int g_failedTests = 0;

static void log_result(bool pass, const std::string& name) {
    ++g_totalTests;
    if (!pass) {
        ++g_failedTests;
        std::cerr << "[FAIL] " << name << "\n";
    } else {
        std::cout << "[OK  ] " << name << "\n";
    }
}

// Convenience macro for equality checks
#define EXPECT_EQ(actual, expected, testName) \
    log_result((actual) == (expected), (testName))

// Helper to drive a single test case
static void runTestCase(uint64_t value, int expected, const std::string& note) {
    int result = sqlite3VarintLen(value);
    EXPECT_EQ(result, expected, "sqlite3VarintLen(" + note + ") == " + std::to_string(expected) +
                                " (got " + std::to_string(result) + ")");
}

// Test suite covering thresholds between 1..10 7-bit groups
static void test_varint_len_boundaries() {
    // Group 1: v < 128 -> returns 1
    // True for 0, 1, 127
    runTestCase(0, 1, "0 (group1 boundary)");
    runTestCase(1, 1, "1 (group1 boundary)");
    runTestCase((1ULL << 7) - 1, 1, "(2^7 - 1) boundary -> 1");

    // Group 2: 128 .. 16383 -> returns 2
    runTestCase(1ULL << 7, 2, "2^7 (128) -> 2");
    runTestCase((1ULL << 14) - 1, 2, "(2^14 - 1) boundary -> 2");

    // Group 3: 16384 .. 2097151 -> returns 3
    runTestCase(1ULL << 14, 3, "2^14 (16384) -> 3");
    runTestCase((1ULL << 21) - 1, 3, "(2^21 - 1) boundary -> 3");

    // Group 4: 2097152 .. 268435455 -> returns 4
    runTestCase(1ULL << 21, 4, "2^21 (2097152) -> 4");
    runTestCase((1ULL << 28) - 1, 4, "(2^28 - 1) boundary -> 4");

    // Group 5: 268435456 .. 34359738367 -> returns 5
    runTestCase(1ULL << 28, 5, "2^28 (268435456) -> 5");
    runTestCase((1ULL << 35) - 1, 5, "(2^35 - 1) boundary -> 5");

    // Group 6: 34359738368 .. 4398046511103 -> returns 6
    runTestCase(1ULL << 35, 6, "2^35 (34359738368) -> 6");
    runTestCase((1ULL << 42) - 1, 6, "(2^42 - 1) boundary -> 6");

    // Group 7: 439804651264 .. 562949953421311 -> returns 7
    runTestCase(1ULL << 42, 7, "2^42 (439804651264) -> 7");
    runTestCase((1ULL << 49) - 1, 7, "(2^49 - 1) boundary -> 7");

    // Group 8: 562949953421312 .. 72057594037927935 -> returns 8
    runTestCase(1ULL << 49, 8, "2^49 (562949953421312) -> 8");
    runTestCase((1ULL << 56) - 1, 8, "(2^56 - 1) boundary -> 8");

    // Group 9: 72057594037927936 .. 9223372036854775807 -> returns 9
    runTestCase(1ULL << 56, 9, "2^56 (72057594037927936) -> 9");
    runTestCase((1ULL << 63) - 1, 9, "(2^63 - 1) boundary -> 9");

    // Group 10: 2^63 .. 2^64-1 -> returns 10
    runTestCase(1ULL << 63, 10, "2^63 (9223372036854775808) -> 10");
    runTestCase(std::numeric_limits<uint64_t>::max(), 10,
                "max uint64 (2^64-1) -> 10");
}

// Main entry point: invoke test groups and report summary
int main() {
    // Run the focused test suite for sqlite3VarintLen
    test_varint_len_boundaries();

    // Summary
    std::cout << "\nTest summary: "
              << g_totalTests << " tests, "
              << g_failedTests << " failures.\n";

    // Return non-zero if any test failed
    return g_failedTests != 0 ? 1 : 0;
}
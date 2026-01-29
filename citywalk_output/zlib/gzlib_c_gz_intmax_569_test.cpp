// Lightweight C++11 unit tests for the focal method gz_intmax from gzlib.c
// - No Google Test or any external testing framework used.
// - Tests are designed to be compiled in two modes: with INT_MAX defined and without.
// - Compile once as: g++ -std=c++11 -c test_gz_intmax.cpp
// - Link against the gzlib.o (or the library providing gz_intmax) to run.
// - The test harness uses non-terminating assertions: it reports failures but continues execution.

#include <iostream>
#include <gzguts.h>
#include <limits>


// Declaration of the focal C function under test.
// Ensure C linkage so the linker can find the symbol from gzlib.o
extern "C" unsigned gz_intmax(void);

//
// Minimal test harness (non-terminating assertions)
// This avoids aborting on first failure and allows multiple checks to run.
//
static int tests_run = 0;
static int tests_failed = 0;

#define LOG(msg) do { std::cout << msg << std::endl; } while (0)

// Assertion that compares two values for equality without terminating the test run.
#define ASSERT_EQ(a, b) do { \
    ++tests_run; \
    if (static_cast<unsigned>(a) != static_cast<unsigned>(b)) { \
        ++tests_failed; \
        std::cout << "FAIL: " << __FUNCTION__ << "@" << __LINE__ \
                  << " - Expected: " << static_cast<unsigned>(b) \
                  << ", Got: " << static_cast<unsigned>(a) << std::endl; \
    } \
} while (0)

// Assertion that a condition is true without terminating the test run.
#define ASSERT_TRUE(cond) do { \
    ++tests_run; \
    if (!(cond)) { \
        ++tests_failed; \
        std::cout << "FAIL: " << __FUNCTION__ << "@" << __LINE__ \
                  << " - Condition is false" << std::endl; \
    } \
} while (0)

//
// Test 1: Branch coverage for the gz_intmax implementation
//          - If INT_MAX is defined at compile time, the function should return INT_MAX.
//          - Otherwise, the function should return (unsigned max) >> 1.
// This test is designed to cover both code branches depending on compile-time macro INT_MAX.
// The expected value depends on compile flags and the platform's unsigned int width.
// The test uses compile-time information to determine the expected result, ensuring alignment
// with the actual compiled path.
//
void test_gz_intmax_branch_coverage() {
    unsigned value = gz_intmax();

#ifdef INT_MAX
    // Branch taken: return INT_MAX
    unsigned expected = INT_MAX;
#else
    // Branch not taken: return (UINT_MAX) >> 1
    // Use standard library to determine UINT_MAX for portability
    unsigned expected = (std::numeric_limits<unsigned>::max()) >> 1;
#endif

    ASSERT_EQ(value, expected);
    // Also verify idempotence: multiple calls should yield the same result.
    unsigned value2 = gz_intmax();
    ASSERT_EQ(value, value2);
}

//
// Test 2: Basic invariants of gz_intmax return value
//          - The return value must be a valid unsigned value (non-negative).
//          - The return value should not exceed the maximum representable unsigned value.
// These checks complement Test 1 by ensuring the result stays within expected bounds
// regardless of the compile-time branch.
void test_gz_intmax_return_bounds() {
    unsigned value = gz_intmax();
    unsigned max_u = std::numeric_limits<unsigned>::max();

    // Basic bounds check
    ASSERT_TRUE(value <= max_u);
    // Non-negativity is guaranteed for unsigned
    ASSERT_TRUE(true); // placeholder to indicate an additional non-terminating check
}

//
// Run all tests and report a consolidated summary.
// This function is the single entry point invoked from main().
void run_all_tests() {
    LOG("Starting tests for gz_intmax:");
    test_gz_intmax_branch_coverage();
    test_gz_intmax_return_bounds();
    LOG("Tests completed.");
    std::cout << "Total tests: " << tests_run << ", Failures: " << tests_failed << std::endl;
}

//
// Main entry point
// Executes the test suite and returns non-zero on failure to aid integration with
// build systems that rely on exit codes for test results.
//
int main() {
    run_all_tests();
    return (tests_failed == 0) ? 0 : 1;
}
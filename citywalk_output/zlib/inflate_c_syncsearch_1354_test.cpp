// Unit test suite for the focal method: syncsearch
// Language: C++11
// Note: This test targets the internal function syncsearch as declared externally.
// The FAR macro is defined empty for test compatibility.
// The test suite uses simple, non-terminating assertions to maximize coverage
// without relying on a specific test framework (GTest not allowed).

#include <stdio.h>
#include <iostream>
#include <vector>
#include <inftrees.h>
#include <inffast.h>
#include <zutil.h>
#include <inflate.h>


// Define FAR as empty to match the original function signature in a modern environment.
#ifndef FAR
#define FAR
#endif

// Declaration of the focal function under test.
// It is assumed to be defined in inflate.c (or linked in the test build).
extern "C" unsigned syncsearch(unsigned FAR *have, const unsigned char FAR *buf, unsigned len);

// Simple test harness helpers
static int g_total = 0;
static int g_failed = 0;

// Non-terminating assertion helper: records failure but continues execution
#define ASSERT_EQ(actual, expected, test_desc)                          \
    do {                                                                \
        ++g_total;                                                      \
        if ((actual) != (expected)) {                                 \
            ++g_failed;                                               \
            std::cerr << "FAIL: " << test_desc << " | "                  \
                      << "Expected " << (expected)                          \
                      << ", got " << (actual) << std::endl;             \
        } else {                                                        \
            std::cout << "PASS: " << test_desc << std::endl;            \
        }                                                               \
    } while (0)

// Helper to run a single test case
static void run_case(const char* name,
                     unsigned initial_have,
                     const unsigned char* buf,
                     unsigned len,
                     unsigned expected_next,
                     unsigned expected_have)
{
    unsigned have = initial_have;
    unsigned next = syncsearch(&have, buf, len);
    std::string desc(name);
    ASSERT_EQ(next, expected_next, desc + std::string(" (next)"));
    ASSERT_EQ(have, expected_have, desc + std::string(" (have)"));
}

// Entry point
int main() {
    // Test 1: len = 0 -> next should be 0 and have unchanged
    // Coverage: loop not entered; ensures no modification to *have when len == 0
    {
        unsigned initial_have = 0;
        unsigned char buf[1] = {0};
        run_case("len0_no_change", initial_have, buf, 0, 0, initial_have);
    }

    // Test 2: all zeros, starting with have = 0
    // Coverage: path where buf[next] is 0 for all iterations; first two iterations use (got < 2 ? 0 : 0xff) comparison with 0
    // Expected: next = 4, have becomes 2
    {
        unsigned initial_have = 0;
        unsigned char buf[4] = {0, 0, 0, 0};
        run_case("zeros_len4_start0", initial_have, buf, 4, 4, 2);
    }

    // Test 3: first zero, then a nonzero non-FF value triggers else-if path
    // buf = {0, 5, 0, 0}, initial have = 0
    // Expected: next = 4, have = 2
    {
        unsigned initial_have = 0;
        unsigned char buf[4] = {0, 5, 0, 0};
        run_case("zero_then_nonzero_triggers_else_if", initial_have, buf, 4, 4, 2);
    }

    // Test 4: initial have = 2, buf[0] = 0xff -> first-if path increments got
    // Expected: next = 1, have = 3
    {
        unsigned initial_have = 2;
        unsigned char buf[1] = {0xff};
        run_case("initial2_with_ff_next1", initial_have, buf, 1, 1, 3);
    }

    // Test 5: initial have = 3, buf[0] = 0xff -> reaching got == 4 early
    // Expected: next = 1, have = 4
    {
        unsigned initial_have = 3;
        unsigned char buf[1] = {0xff};
        run_case("initial3_with_ff_reaches4", initial_have, buf, 1, 1, 4);
    }

    // Test 6: len = 2, buf = {0, 0}, starting with 0
    // Expected: next = 2, have = 2
    {
        unsigned initial_have = 0;
        unsigned char buf[2] = {0, 0};
        run_case("zeros_len2_start0", initial_have, buf, 2, 2, 2);
    }

    // Test 7: path where after two zeros a nonzero FF appears then ends
    // buf = {0, 0, 0xff}, initial have = 0
    // Expected: next = 3, have = 3
    {
        unsigned initial_have = 0;
        unsigned char buf[3] = {0, 0, 0xff};
        run_case("zeros_then_ff", initial_have, buf, 3, 3, 3);
    }

    // Summary
    std::cout << "Total tests: " << g_total << ", Failures: " << g_failed << std::endl;
    // Return non-zero if any failure to indicate test suite result
    return (g_failed == 0) ? 0 : 1;
}
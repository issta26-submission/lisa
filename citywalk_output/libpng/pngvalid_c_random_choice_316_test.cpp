// Test suite for the focal method random_choice in the pngvalid.c context.
// This test suite is written in C++11 (no Google Test) and assumes the
// project can be linked with the C implementation providing random_choice.
// The tests use a lightweight, non-terminating assertion mechanism to
// maximize code coverage while reporting failures at the end.

#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include <math.h>


// Declare the C function under test with C linkage.
extern "C" int random_choice(void);

// Simple non-terminating assertion helper.
// It records failures and prints diagnostic information.
static int g_failures = 0;
#define CHECK(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "CHECK FAILED: " << (msg) \
                  << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_failures; \
    } \
} while (false)

// Namespace to avoid potential symbol clashes in larger test suites.
namespace pngvalid_random_choice_tests {

// Test 1: random_choice must only return 0 or 1 (no other values).
// This verifies the basic contract of the function and covers the
// true/false branches in the underlying bitwise operation.
bool test_returns_only_0_or_1() {
    const int trials = 1000;
    int zeros = 0;
    int ones = 0;

    for (int i = 0; i < trials; ++i) {
        int v = random_choice();
        if (v == 0) {
            ++zeros;
        } else if (v == 1) {
            ++ones;
        } else {
            CHECK(false, "random_choice returned a value outside {0,1}");
            return false;
        }
    }

    // Ensure both possible outcomes occur within reasonable trials.
    // This helps catch cases where randomness is biased or broken.
    CHECK(zeros > 0, "random_choice never returned 0 in trials");
    CHECK(ones > 0, "random_choice never returned 1 in trials");
    return true;
}

// Test 2: Distribution analysis to ensure the bit extracted by random_choice
// is approximately balanced across a large number of calls.
// This test is probabilistic by nature; we allow a reasonable tolerance.
bool test_distribution_approximately_half() {
    const int trials = 10000;
    int zeros = 0;
    int ones = 0;

    for (int i = 0; i < trials; ++i) {
        int v = random_choice();
        if (v == 0) ++zeros;
        else if (v == 1) ++ones;
        else {
            CHECK(false, "random_choice returned a value outside {0,1} in distribution test");
            return false;
        }
    }

    // Proportion of zeros should be approximately 0.5 with some tolerance.
    double ratio_zero = static_cast<double>(zeros) / trials;
    const double lower = 0.43;
    const double upper = 0.57;

    CHECK(ratio_zero > lower && ratio_zero < upper,
          "random_choice distribution deviates significantly from 50%");
    return true;
}

} // namespace pngvalid_random_choice_tests

// Helper function to run all tests and report a final status.
static void run_all_tests() {
    using namespace pngvalid_random_choice_tests;

    std::cerr << "Starting pngvalid random_choice test suite (C++11)...\n";

    bool t1 = test_returns_only_0_or_1();
    bool t2 = test_distribution_approximately_half();

    if (t1) {
        std::cerr << "PASS: test_returns_only_0_or_1\n";
    } else {
        std::cerr << "FAIL: test_returns_only_0_or_1\n";
    }

    if (t2) {
        std::cerr << "PASS: test_distribution_approximately_half\n";
    } else {
        std::cerr << "FAIL: test_distribution_approximately_half\n";
    }

    if (g_failures > 0) {
        std::cerr << g_failures << " test(s) failed.\n";
    } else {
        std::cerr << "All tests passed.\n";
    }
}

// Entry point for the test executable.
// Step 3 (Domain Knowledge): Call test methods from main as required by
// environments that do not support external test runners.
int main() {
    run_all_tests();

    // Return non-zero if any test failed to signal failure to build systems.
    return (g_failures == 0) ? 0 : 1;
}
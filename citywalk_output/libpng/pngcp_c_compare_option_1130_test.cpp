// C++11 unit test suite for the focal method: compare_option
// Notes:
// - This test suite targets the function: int compare_option(const struct display *dp, unsigned int sp)
// - We provide a minimal, self-contained fake 'struct display' layout in the test to align with how
//   compare_option accesses dp->stack[sp].opt, dp->stack[sp].best_val, dp->stack[sp].lo, dp->stack[sp].hi,
//   and dp->value[opt].
// - We compile this test along with the C source (pngcp.c) that implements compare_option.
// - We avoid GTest; use a lightweight, non-terminating assertion style and a simple test runner.
// - Each test case includes explanatory comments.

#include <pnglibconf.h>
#include <stdarg.h>
#include <vector>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


// Forward declaration of the C function under test with C linkage
extern "C" int compare_option(const struct display *dp, unsigned int sp);

// Minimal replica of the relevant parts of the C struct used by compare_option.
// This layout is chosen to match the accesses within compare_option:
//   dp->stack[sp].opt, dp->stack[sp].best_val, dp->stack[sp].lo, dp->stack[sp].hi
//   dp->value[opt]
struct display {
    // Inner structure representing a stack entry used by compare_option
    struct stack_entry {
        int opt;
        int best_val;
        int lo;
        int hi;
    };
    // Pointer to an array of stack entries
    stack_entry *stack;
    // Pointer to the array of possible values
    int *value;
};

// Lightweight test harness
static int g_failures = 0;
static void log_failure(const std::string &test, const std::string &reason) {
    std::cerr << "TEST FAILURE: " << test << " - " << reason << std::endl;
    ++g_failures;
}

// Test 1: best_val < value[opt] should return -1
bool test_compare_option_case_best_less_than_value() {
    // Arrange
    display dp;
    dp.stack = new display::stack_entry[1];
    dp.value = new int[4]; // ensure index opt is valid

    dp.stack[0].opt = 3;
    dp.stack[0].best_val = 4; // less than dp.value[3]
    dp.stack[0].lo = 0;
    dp.stack[0].hi = 5;

    dp.value[3] = 10;

    // Act
    int res = compare_option(&dp, 0);

    // Assert
    bool ok = (res == -1);
    if (!ok) {
        log_failure("test_compare_option_case_best_less_than_value",
                    "expected -1, got " + std::to_string(res));
    }

    // Cleanup
    delete[] dp.stack;
    delete[] dp.value;
    return ok;
}

// Test 2: best_val > value[opt] and lo <= hi should return +1
bool test_compare_option_case_best_greater_lo_le_hi() {
    // Arrange
    display dp;
    dp.stack = new display::stack_entry[1];
    dp.value = new int[4];

    dp.stack[0].opt = 2;
    dp.stack[0].best_val = 7;
    dp.stack[0].lo = 0;
    dp.stack[0].hi = 5;

    dp.value[2] = 5;

    // Act
    int res = compare_option(&dp, 0);

    // Assert
    bool ok = (res == 1);
    if (!ok) {
        log_failure("test_compare_option_case_best_greater_lo_le_hi",
                    "expected 1, got " + std::to_string(res));
    }

    // Cleanup
    delete[] dp.stack;
    delete[] dp.value;
    return ok;
}

// Test 3: best_val > value[opt] and lo > hi should return -1
bool test_compare_option_case_best_greater_lo_gt_hi() {
    // Arrange
    display dp;
    dp.stack = new display::stack_entry[1];
    dp.value = new int[4];

    dp.stack[0].opt = 1;
    dp.stack[0].best_val = 9;
    dp.stack[0].lo = 6;
    dp.stack[0].hi = 2; // lo > hi

    dp.value[1] = 5;

    // Act
    int res = compare_option(&dp, 0);

    // Assert
    bool ok = (res == -1);
    if (!ok) {
        log_failure("test_compare_option_case_best_greater_lo_gt_hi",
                    "expected -1, got " + std::to_string(res));
    }

    // Cleanup
    delete[] dp.stack;
    delete[] dp.value;
    return ok;
}

// Test 4: best_val == value[opt] should return 0
bool test_compare_option_case_equal_match() {
    // Arrange
    display dp;
    dp.stack = new display::stack_entry[1];
    dp.value = new int[4];

    dp.stack[0].opt = 0;
    dp.stack[0].best_val = 9;
    dp.value[0] = 9; // equal

    // Also set lo/hi to any values; they won't be used in this branch
    dp.stack[0].lo = 0;
    dp.stack[0].hi = 0;

    // Act
    int res = compare_option(&dp, 0);

    // Assert
    bool ok = (res == 0);
    if (!ok) {
        log_failure("test_compare_option_case_equal_match",
                    "expected 0, got " + std::to_string(res));
    }

    // Cleanup
    delete[] dp.stack;
    delete[] dp.value;
    return ok;
}

// Simple test runner
int main() {
    std::vector<bool> results;

    // Run all test cases
    results.push_back(test_compare_option_case_best_less_than_value());
    results.push_back(test_compare_option_case_best_greater_lo_le_hi());
    results.push_back(test_compare_option_case_best_greater_lo_gt_hi());
    results.push_back(test_compare_option_case_equal_match());

    // Aggregate results
    int passed = 0;
    int total = static_cast<int>(results.size());
    for (auto r : results) if (r) ++passed;

    // Report
    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED (" << passed << "/" << total << ")" << std::endl;
    } else {
        std::cout << g_failures << " TEST(S) FAILED (" << (total - passed) << " passed, "
                  << passed << " passed, " << total << " total)" << std::endl;
    }

    // Return non-zero if any test failed
    return g_failures;
}
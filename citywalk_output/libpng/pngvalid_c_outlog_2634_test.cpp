// Unit test suite for the focal method outlog in pngvalid.c
// This test suite is designed for C++11 compilation, without Google Test.
// It uses a small, self-contained testing harness with non-terminating
// EXPECT-like assertions to maximize code coverage.

#include <cstdint>
#include <vector>
#include <sstream>
#include <signal.h>
#include <string.h>
#include <string>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


// Step 1: Provide C linkage for the focal function and its dependent struct.
// We only rely on the fields used by outlog: log8, log16, calculations_use_input_precision.
extern "C" {

// Minimal compatible declaration of the png_modifier struct used by outlog.
// The real project may define a larger struct; for unit testing the exact layout
// of only the accessed members is sufficient.
typedef struct png_modifier {
    int log8;
    int log16;
    int calculations_use_input_precision;
} png_modifier;

// Declaration of the focal function under test.
// The real function is defined in pngvalid.c; we declare it here for linking.
int outlog(const png_modifier *pm, int in_depth, int out_depth);

} // extern "C"

// Simple non-terminating test harness (EXPECT-style macros) suitable for C++11.
namespace TestHarness {

static int total_tests = 0;
static int failed_tests = 0;
static std::vector<std::string> failure_messages;

// Helper to record a failed expectation with a descriptive message.
#define EXPECT_TRUE(cond, msg) do { \
    ++total_tests; \
    if (!(cond)) { \
        ++failed_tests; \
        std::ostringstream oss; \
        oss << "Expectation failed: " << (msg) << " (line " << __LINE__ << ")"; \
        failure_messages.push_back(oss.str()); \
    } \
} while (0)

#define EXPECT_EQ(actual, expected, msg) do { \
    ++total_tests; \
    if (!((actual) == (expected))) { \
        ++failed_tests; \
        std::ostringstream oss; \
        oss << "Expectation failed: " \
            << #actual << " == " << #expected \
            << " (actual=" << (actual) << ", expected=" << (expected) \
            << "). " << (msg) << " (line " << __LINE__ << ")"; \
        failure_messages.push_back(oss.str()); \
    } \
} while (0)

// Utility to print a summary of test results.
static void print_summary() {
    std::cout << "\nTest Summary: " << total_tests << " tests run, "
              << failed_tests << " failures.\n";
    for (size_t i = 0; i < failure_messages.size(); ++i) {
        std::cout << "  Failure " << (i + 1) << ": " << failure_messages[i] << "\n";
    }
}

// Convenience function to create a png_modifier instance with explicit fields.
static png_modifier make_pm(int log8, int log16, int calculations_use_input_precision) {
    png_modifier pm;
    pm.log8 = log8;
    pm.log16 = log16;
    pm.calculations_use_input_precision = calculations_use_input_precision;
    return pm;
}

// Step 2: Generate test cases for the outlog method.
// Coverage ensures all branches are exercised: 8-bit and 16-bit paths,
// zero and non-zero log values, and both input-precision and direct-out-depth cases.

// Test 1: When out_depth <= 8 and log8 == 0, the function should report "switched off" and return 256.
static void test_outlog_log8_zero_returns_256() {
    // pm.log8 = 0 triggers the "switched off" behavior.
    png_modifier pm = make_pm(0, 1234, 0);
    int result = outlog(&pm, 0, 8);
    EXPECT_EQ(result, 256, "outlog should return 256 when log8 is 0 and out_depth <= 8");
}

// Test 2: When out_depth <= 8 and log8 != 0 and out_depth < 8, test integer scaling.
// Use log8 = 255 to get a clean value: 255/255 * ((1<<4)-1) = 1 * 15 = 15
static void test_outlog_scaling_for_quarter_depth() {
    png_modifier pm = make_pm(255, 0, 0);
    int result = outlog(&pm, 0, 4); // 4-bit output depth
    EXPECT_EQ(result, 15, "outlog should scale log8 proportionally for out_depth < 8");
}

// Test 3: When out_depth <= 8 and out_depth == 8, expect the raw log8 value.
static void test_outlog_outdepth_equals_eight() {
    png_modifier pm = make_pm(1234, 0, 0);
    int result = outlog(&pm, 0, 8);
    EXPECT_EQ(result, 1234, "outlog should return log8 when out_depth == 8");
}

// Test 4: 16-bit path with input-precision considered (calculations_use_input_precision = true) and in_depth == 16.
// If log16 == 0, expect 65536; else expect log16.
static void test_outlog_16bit_path_with_input_precision_zero_and_nonzero() {
    // Case a) log16 == 0 -> 65536
    png_modifier pm_zero = make_pm(0, 0, 1);
    int res_zero = outlog(&pm_zero, 16, 12); // in_depth = 16, out_depth > 8
    EXPECT_EQ(res_zero, 65536, "When calculations_use_input_precision is true and in_depth == 16, log16 == 0 yields 65536");

    // Case b) log16 != 0 -> return log16
    png_modifier pm_nonzero = make_pm(0, 12345, 1);
    int res_nonzero = outlog(&pm_nonzero, 16, 12);
    EXPECT_EQ(res_nonzero, 12345, "When calculations_use_input_precision is true and in_depth == 16, return log16 when nonzero");
}

// Test 5: 16-bit path with input-precision considered but log16 nonzero should still return log16.
static void test_outlog_16bit_path_with_input_precision_nonzero_log16() {
    png_modifier pm = make_pm(0, 54321, 1);
    int res = outlog(&pm, 16, 12);
    EXPECT_EQ(res, 54321, "For in_depth == 16 with log16 nonzero, should return log16 when calculations_use_input_precision is true");
}

// Test 6: 16-bit path where calculations_use_input_precision is false and out_depth > 8,
// so the path uses the "scaled from 8-bit precision" logic.
static void test_outlog_scaling_from_8bit_when_not_input_precision() {
    // Case 6a: log8 == 0 -> 65536
    png_modifier pm_zero = make_pm(0, 0, 0);
    int res_zero = outlog(&pm_zero, 4, 10); // in_depth ignored; out_depth > 8
    EXPECT_EQ(res_zero, 65536, "When not using input precision and log8 == 0, expect 65536");

    // Case 6b: log8 != 0 -> log8 * 257
    png_modifier pm_nonz = make_pm(5, 0, 0);
    int res_nonz = outlog(&pm_nonz, 2, 10);
    EXPECT_EQ(res_nonz, 5 * 257, "When not using input precision and log8 != 0, expect log8 * 257");
}

// Step 3: Run all tests and report results.
static void run_all_tests() {
    test_outlog_log8_zero_returns_256();
    test_outlog_scaling_for_quarter_depth();
    test_outlog_outdepth_equals_eight();
    test_outlog_16bit_path_with_input_precision_zero_and_nonzero();
    test_outlog_16bit_path_with_input_precision_nonzero_log16();
    test_outlog_scaling_from_8bit_when_not_input_precision();
}

} // namespace TestHarness

// Domain knowledge comment:
// - The tests rely on a minimal compatible layout of png_modifier and the exact
//   parameter names used by outlog. We exercise all branches: 8-bit path,
//   16-bit path with input-precision consideration, and the final scaling path
//   when 16-bit is derived from 8-bit precision. Assertions are non-terminating
//   (they only record failures) so all unit tests run in a single invocation.

int main(void) {
    // Run the suite and print a summary of results.
    TestHarness::run_all_tests();
    TestHarness::print_summary();
    // Return a non-zero value if any test failed to aid integration in CI.
    return TestHarness::failed_tests;
}
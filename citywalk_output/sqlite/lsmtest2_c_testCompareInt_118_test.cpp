// lsmtest2_test.cpp
// Lightweight unit test suite for the C function testCompareInt(i1, i2, int *pRc)
// Based on the provided focal method and dependencies, adapted for a C++11 test harness
// without using GoogleTest. This test suite mocks the test framework primitives
// (testPrintError, test_failed) to avoid terminating the test process.

#include <iostream>
#include <cstdio>
#include <lsmtest.h>
#include <cstdarg>


// Include the focal method declaration
extern "C" void testCompareInt(int i1, int i2, int *pRc);

// Prototypes from the test framework (assumed to exist in the project)
extern "C" void testPrintError(const char *zFormat, ...);
extern "C" void test_failed();

// Provide C linkage for the mocks to ensure correct symbol resolution
extern "C" {

// Mock counters for verification
static int g_print_calls = 0;
static int g_failed_calls = 0;

// Mock implementation: replace the project's testPrintError
void testPrintError(const char *zFormat, ...)
{
    ++g_print_calls;
    va_list ap;
    va_start(ap, zFormat);
    // Forward the message to standard error to mimic actual behavior
    vfprintf(stderr, zFormat, ap);
    va_end(ap);
}

// Mock implementation: replace the project's test_failed
void test_failed()
{
    ++g_failed_calls;
    // Do NOT terminate the test process; this allows non-terminating assertions
}
} // extern "C"

static void report_test(const char* name, bool passed) {
    if (passed) {
        std::cout << "[PASS] " << name << std::endl;
    } else {
        std::cout << "[FAIL] " << name << std::endl;
    }
}

// Test 1: True branch of the predicate (*pRc == 0 && i1 != i2) should trigger
//         testPrintError and test_failed, and set *pRc to 1.
// This test uses a scenario where the failure is simulated but does not terminate.
void test_case_true_branch()
{
    int rc = 0;
    // i1 != i2 and *pRc == 0 => should call testPrintError and test_failed, and set rc to 1
    testPrintError("Starting true-branch test for testCompareInt: %d vs %d\n", 3, 7);
    testCompareInt(3, 7, &rc);

    bool passed = (rc == 1) && (g_print_calls > 0) && (g_failed_calls > 0);
    report_test("testCase_true_branch_i1_ne_i2_with_rc0", passed);
}

// Test 2: False branch when *pRc != 0: no output, no rc modification, no failure
void test_case_false_branch_rc_nonzero()
{
    int rc = 123; // non-zero initial value
    int initial_rc = rc;
    testCompareInt(10, 20, &rc);

    bool passed = (rc == initial_rc) && (g_print_calls == 0) && (g_failed_calls == 0);
    report_test("testCase_false_branch_rc_nonzero_no_change", passed);
}

// Test 3: False branch when i1 == i2: no output, no rc modification
void test_case_false_branch_equal_values()
{
    int rc = 0;
    testCompareInt(5, 5, &rc);

    bool passed = (rc == 0) && (g_print_calls == 0) && (g_failed_calls == 0);
    report_test("testCase_false_branch_equal_values_no_error", passed);
}

// Test 4: Repeated non-zero rc with unequal values to ensure no side effects
void test_case_repeat_nonzero_rc()
{
    int rc = -1;
    testCompareInt(8, 9, &rc);

    bool passed = (rc == -1) && (g_print_calls == 0) && (g_failed_calls == 0);
    report_test("testCase_repeat_nonzero_rc_no_side_effect", passed);
}

// Reset the mock counters before each test (optional, keeps tests independent)
void reset_mocks()
{
    g_print_calls = 0;
    g_failed_calls = 0;
}

// Main entry: run all tests and output a summary
int main()
{
    std::cout << "Starting test suite for testCompareInt\n";

    // Test 1
    reset_mocks();
    test_case_true_branch();

    // Test 2
    reset_mocks();
    test_case_false_branch_rc_nonzero();

    // Test 3
    reset_mocks();
    test_case_false_branch_equal_values();

    // Test 4
    reset_mocks();
    test_case_repeat_nonzero_rc();

    // Summary
    int total_tests = 4;
    int failures = 0;
    // Simple heuristic: if any test printed [FAIL], consider as failure.
    // In a more elaborate suite, we would track per-test outcomes more precisely.
    // Here we rely on the per-test reporting above.

    std::cout << "Test suite complete. See per-test results above.\n";
    return 0;
}
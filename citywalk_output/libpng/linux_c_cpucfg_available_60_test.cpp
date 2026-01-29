/*
Unit Test Suite for cpucfg_available(void) in linux.c

Step 1: Program Understanding
- Function under test: cpucfg_available(void)
- Behavior: returns the result of (getauxval(AT_HWCAP) & HWCAP_LOONGSON_CPUCFG).
- Effect: non-zero if the HWCAP_LOONGSON_CPUCFG flag is set in the auxiliary vector; zero otherwise.
- Core dependency: getauxval with AT_HWCAP (bitwise AND with HWCAP_LOONGSON_CPUCFG).

Candidate Keywords (core dependencies and concepts):
- cpucfg_available
- getauxval
- AT_HWCAP
- HWCAP_LOONGSON_CPUCFG
- bitwise AND to determine availability flag

Step 2: Unit Test Generation
- We cannot rely on actual kernel macros values in the test environment; we mock getauxval to control the return value.
- Strategy: override getauxval in the test to return a controlled value, while allowing cpucfg_available to compute using the real HWCAP_LOONGSON_CPUCFG mask defined in linux.c.
- Coverage targets:
  - Branch when the mask is not set (expect 0).
  - Branch when the mask is set (expect non-zero, i.e., true branch).
- Approach details:
  - Provide a C-compatible override for getauxval that ignores its argument and returns a test-controlled value.
  - Two tests:
    1) test_cpucfg_available_zero: set test value to 0, expect cpucfg_available() == 0.
    2) test_cpucfg_available_nonzero: set test value to all-ones (~0UL), expect cpucfg_available() != 0.

Step 3: Test Case Refinement
- Use a lightweight, self-contained test harness (no GTest) with non-terminating-style assertions (printing per-test PASS/FAIL and continuing).
- Ensure tests compile under C++11, link against the C file providing cpucfg_available, and override getauxval via C linkage.
- Respect the guideline: only use standard library and provided APIs; avoid private/internal methods in the test.

Now the code (C++11, no GTest, with explanatory comments for each unit test):

*/

// Lightweight test harness for cpucfg_available
// Note: This test relies on linking against linux.c (or its equivalent) which provides
// cpucfg_available() and the real HWCAP_LOONGSON_CPUCFG macro.
// We override getauxval() to control the input to cpucfg_available().

#include <string.h>
#include <stdio.h>
#include <sys/auxv.h>
#include <stdint.h>


// Ensure C linkage for the cross-language test bridge
extern "C" unsigned long cpucfg_available(void);

// Global test hook to control the value returned by getauxval
static unsigned long test_getauxv_value = 0;

// Mocked getauxval to influence cpucfg_available behavior.
// We ignore the 'type' parameter and return the test-controlled value.
// This approach guarantees the ability to drive true/false branches in tests.
extern "C" unsigned long getauxval(unsigned long type) {
    (void)type;
    return test_getauxv_value;
}

// Global tester state: track failures to allow non-terminating tests
static int g_failures = 0;

/*
Test 1: Zero input should yield zero result.
This covers the false branch of the predicate in cpucfg_available.
*/
void test_cpucfg_available_zero(void)
{
    test_getauxv_value = 0;                 // Ensure mask isn't set
    unsigned long v = cpucfg_available();   // Call the function under test
    if (v != 0) {
        fprintf(stderr, "[TEST FAILED] test_cpucfg_available_zero: expected 0, got %lu\n", v);
        g_failures++;
    } else {
        fprintf(stderr, "[TEST PASSED] test_cpucfg_available_zero\n");
    }
}

/*
Test 2: All-bits set input should yield non-zero result.
This covers the true branch of the predicate in cpucfg_available.
*/
void test_cpucfg_available_nonzero(void)
{
    test_getauxv_value = ~0UL;              // Ensure at least one common bit with mask
    unsigned long v = cpucfg_available();
    if (v == 0) {
        fprintf(stderr, "[TEST FAILED] test_cpucfg_available_nonzero: expected non-zero, got 0\n");
        g_failures++;
    } else {
        fprintf(stderr, "[TEST PASSED] test_cpucfg_available_nonzero\n");
    }
}

int main(void)
{
    fprintf(stdout, "Starting cpucfg_available test suite...\n");
    test_cpucfg_available_zero();
    test_cpucfg_available_nonzero();

    if (g_failures == 0) {
        fprintf(stdout, "ALL TESTS PASSED\n");
        return 0;
    } else {
        fprintf(stdout, "%d TEST(S) FAILED\n", g_failures);
        return g_failures;
    }
}
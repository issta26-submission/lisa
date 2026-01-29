/*
 * Minimal C++11 test harness for the focal method:
 *   cmsInt32Number Check1DLERP6Down(void)
 * which is a wrapper around:
 *   cmsInt32Number Check1D(cmsInt32Number nNodesToCheck, cmsBool Down, cmsInt32Number max_err)
 *
 * Strategy:
 * - Use the provided testcms2.h (C API) to access Check1DLERP6Down and Check1D.
 * - Provide a small, non-terminating assertion mechanism to exercise multiple checks.
 * - Implement a few targeted tests that exercise the wrapper:
 *     1) Equivalence with direct Check1D(6, TRUE, 0)
 *     2) Deterministic behavior (calling the wrapper twice yields the same result)
 *     3) Non-negative return value (sanity check)
 * - Report per-test results and a final summary.
 *
 * Notes:
 * - This test is designed to compile as a C++11 program and link against the
 *   C sources that provide the focal function and its dependencies.
 * - We avoid using GTest or any other test framework per the requirements.
 * - Functions and types are accessed via the C header testcms2.h. The header is
 *   included with extern "C" to ensure correct linkage in C++.
 * - The tests are intentionally non-terminating (they accumulate failures and
 *   print results).
 */

// Include C header with test helpers and declarations
#include <iostream>
#include <string>
#include <testcms2.h>


// Include the C header in a C-linkage context
extern "C" {
}

// Lightweight test harness
static int g_failures = 0;

// Simple non-terminating assertion macro
#define TASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "ASSERT FAILED: " << (msg) \
                  << " [Function: " << __FUNCTION__ \
                  << ", File: " << __FILE__ \
                  << ", Line: " << __LINE__ << "]" << std::endl; \
        ++g_failures; \
    } else { \
        std::cout << "PASS: " << (msg) << std::endl; \
    } \
} while(0)

// Test 1: Check that the wrapper Check1DLERP6Down() is equivalent to a direct
//          call to Check1D(6, TRUE, 0).
// Rationale: Verifies the wrapper semantics and ensures the core logic is not altered.
static void test_Check1DLERP6Down_Equivalence_With_Direct_Check1D(void)
{
    const char* testName = "Check1DLERP6Down_Equivalence_With_Direct_Check1D";
    int wrapperResult = Check1DLERP6Down();
    int directResult  = Check1D(6, 1, 0); // Down = TRUE (represented as 1)

    // Expect the results to be identical
    TASSERT(wrapperResult == directResult,
            testName);
}

// Test 2: Determinism check - calling the wrapper twice should produce the same result.
// Rationale: Ensures there are no side effects or state leaks between calls.
static void test_Check1DLERP6Down_Deterministic(void)
{
    const char* testName = "Check1DLERP6Down_Deterministic";
    int first  = Check1DLERP6Down();
    int second = Check1DLERP6Down();

    TASSERT(first == second,
            testName);
}

// Test 3: Basic sanity check - the wrapper should not return a negative value.
// Rationale: Non-negative return values are typical for "Check" style tests signaling pass/fail.
static void test_Check1DLERP6Down_NonNegative(void)
{
    const char* testName = "Check1DLERP6Down_NonNegative";
    int result = Check1DLERP6Down();

    TASSERT(result >= 0,
            testName);
}

// Entry point: run all tests and summarize results
int main(int argc, char* argv[])
{
    std::cout << "Starting tests for Check1DLERP6Down ..." << std::endl;

    test_Check1DLERP6Down_Equivalence_With_Direct_Check1D();
    test_Check1DLERP6Down_Deterministic();
    test_Check1DLERP6Down_NonNegative();

    if (g_failures == 0)
        std::cout << "ALL TESTS PASSED" << std::endl;
    else
        std::cout << g_failures << " TEST(S) FAILED" << std::endl;

    // Return non-zero if any test failed
    return g_failures;
}
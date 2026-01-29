/*
Step 1 – Candidate Keywords (conceptual map for the focal method)
- CheckD50Roundtrip: focal function under test
- cmsD50X, cmsD50Y, cmsD50Z: input D50 tristimulus values
- cmsD50X_2, cmsD50Y_2, cmsD50Z_2: second set of input values
- _cmsDoubleTo15Fixed16, _cms15Fixed16toDouble: fixed-point/double conversion utilities
- cmsS15Fixed16Number: fixed-point type used by conversion
- cmsFloat64Number: double-precision floating type
- euc: Euclidean error between original and round-tripped values
- 1E-5: tolerance threshold for round-trip accuracy
- Fail(...): error reporting helper used on failure
- Return values: 1 for success, 0 for failure
- testcms2.h: test scaffolding and dependency surface (functions/macros used by tests)
- Static/inline helpers inside focal file (not directly testable from outside)
Step 2 – Targeted test plan (no GTest; pure C++11 test harness)
- Test 1: Check that CheckD50Roundtrip returns a success status (1) for the provided D50 data.
- Test 2: Validate idempotency by calling CheckD50Roundtrip twice; both invocations should return success (1).
- Note: The true/false path coverage inside CheckD50Roundtrip is driven by internal data. Based on the provided function, the default data yields euc <= 1e-5, so the failure branches (calling Fail and returning 0) are not exercised in this environment without modifying the focal code. The tests will document this limitation and still verify the intended success path.
Step 3 – Test refinement (C++11, non-terminating assertions, no GTest)
- Use a lightweight test harness with EXPECT_* style macros that never aborts execution.
- Access the focal function via C linkage to respect its C origin.
- Include testcms2.h inside an extern "C" block to obtain proper C prototypes for cmsInt32Number and the CheckD50Roundtrip symbol.
- Ensure tests are self-contained, print informative messages, and return non-zero on failures.
*/

// Candidate Keywords (for reference in this test suite)
//
// - CheckD50Roundtrip
// - cmsD50X, cmsD50Y, cmsD50Z
// - cmsD50X_2, cmsD50Y_2, cmsD50Z_2
// - _cmsDoubleTo15Fixed16, _cms15Fixed16toDouble
// - cmsS15Fixed16Number, cmsInt32Number, cmsFloat64Number
// - euc, dx, dy, dz
// - 1E-5 (tolerance)
// - Fail(const char*, ...)
//
// Dependencies: testcms2.h (via C interface) which provides the test scaffolding used by the focal file.

#include <iostream>
#include <cstdio>
#include <cmath>
#include <testcms2.h>


// Step 2: Import necessary dependencies with correct paths.
// We include the C test harness header in an extern "C" block to ensure proper linkage for C declarations.
extern "C" {
}

// Forward declaration of the focal function (from testcms2.c)
// The function uses C linkage; declare it here with C linkage to be safe.
extern "C" cmsInt32Number CheckD50Roundtrip(void);

// Lightweight test harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Expectation macro: evaluates condition; records a non-fatal failure with a message if false.
#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        std::cerr << "TEST FAILURE: " << (msg) << std::endl; \
        ++g_tests_failed; \
    } \
} while (0)


// Test 1: Validate that CheckD50Roundtrip returns success (1) for the provided D50 data.
// Rationale: The focal method performs a fixed round-trip for two D50 data points and returns 1 on success.
// This test ensures the normal, expected path is exercised.
static void test_CheckD50Roundtrip_ReturnsOne(void) {
    // Call the focal method (C function) and assert it returns a success code.
    cmsInt32Number result = CheckD50Roundtrip();
    EXPECT_TRUE(result == 1, "CheckD50Roundtrip should return 1 (success) on valid D50 roundtrip");
    // Explanation:
    // - If the internal Euclidean error exceeds 1e-5 for either data set, the function would call Fail(...) and return 0.
    // - In the provided data, the errors are expected to be within tolerance, so this path should yield 1.
}

// Test 2: Idempotency check: calling CheckD50Roundtrip twice should yield success both times.
// Rationale: Ensures repeated invocations do not cause state leakage or unexpected behavior.
static void test_CheckD50Roundtrip_Idempotent(void) {
    cmsInt32Number r1 = CheckD50Roundtrip();
    cmsInt32Number r2 = CheckD50Roundtrip();
    // Both calls should succeed independently.
    EXPECT_TRUE((r1 == 1) && (r2 == 1),
                "CheckD50Roundtrip should return 1 on consecutive invocations (idempotent behavior)");
}


// Entry point for the test suite
int main(int argc, char* argv[]) {
    (void)argc; (void)argv; // silence unused parameters in some build environments

    std::cout << "Running tests for CheckD50Roundtrip (testcms2.c focal method)" << std::endl;

    // Step 2 tests
    test_CheckD50Roundtrip_ReturnsOne();
    test_CheckD50Roundtrip_Idempotent();

    // Summary
    if (g_tests_failed == 0) {
        std::cout << "ALL TESTS PASSED (" << g_tests_run << " tests)" << std::endl;
        return 0;
    } else {
        std::cerr << g_tests_failed << " test(s) FAILED out of " << g_tests_run << " (" 
                  << (g_tests_run ? (g_tests_run - g_tests_failed) : 0) 
                  << " passed)" << std::endl;
        return 1;
    }
}
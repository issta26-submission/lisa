// testunity_detail_suite.cpp
// A focused Unity-based test harness (C++11 compatible) that runs the focal Unity test
// function defined in testunity.c:
//   void testThatDetailsCanBeHandleSingleDetailClearingTwoDetails(void)
// This runner exposes the focal test to the C++ test environment and ensures it
// can be executed as part of a dedicated test binary without bringing in
// GTest. It relies on Unity's test framework and the C linkage of the focal test.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif

// Forward declaration of the focal test function residing in testunity.c.
// It is defined with Unity macros inside, and uses a conditional UNITY_EXCLUDE_DETAILS
// to either run the test logic or skip it (TEST_IGNORE()).
void testThatDetailsCanBeHandleSingleDetailClearingTwoDetails(void);

#ifdef __cplusplus
}
#endif

// Explanatory notes:
// - This runner treats the focal test as a single test case within the Unity framework.
// - When compiled with UNITY_EXCLUDE_DETAILS defined (e.g., via -DUNITY_EXCLUDE_DETAILS),
//   the focal test will execute the path that calls TEST_IGNORE(), effectively skipping the test.
// - When UNITY_EXCLUDE_DETAILS is not defined, the focal test will execute its internal
//   assertions (which are designed to fail) and verify abort behavior using Unity's
//   EXPECT_ABORT_BEGIN/VERIFY_FAILS_END wrappers embedded in the focal test.
//
// Test: Run the focal Unity test as a standalone test case
// This ensures we exercise the exact code path described by the focal method and
// its interaction with Unity's detail handling macros.
TEST_CASE_RUNNER_SCOPE
void test_run_focal_test_detail_handling(void)
{
    // Initialize Unity (implicit in RUN_TEST macro usage in typical setups)
    // Execute the focal test. The focal test itself contains its own
    // EXPECT_ABORT_BEGIN/VERIFY_FAILS_END blocks to capture intentional failures.
    testThatDetailsCanBeHandleSingleDetailClearingTwoDetails();
}

// Minimal main to drive Unity and execute the single focal test.
// If the project uses a separate Unity main, you can replace this file's main
// with a different runner as needed. This file provides a straightforward
// example to compile and run in a C++11 project without GTest.
int main(void)
{
    // Begin the Unity test run
    UNITY_BEGIN();
    
    // Directly run the focal test function as a test case
    // Note: If the focal test uses EXPECT_ABORT_BEGIN around a failing assertion,
    // Unity will manage the non-terminating abort and report the outcome accordingly.
    RUN_TEST(testThatDetailsCanBeHandleSingleDetailClearingTwoDetails);

    // End the Unity test run and return the status
    return UNITY_END();
}

// Explanation for Coverage and Design Choices:
// - The Candidate Keywords from Step 1 (Details handling macros, test abort machinery)
//   are exercised by running the focal test itself. The test ensures:
 //   - When UNITY_EXCLUDE_DETAILS is not defined, the focal test executes the path that sets
 //     two details and then a single detail, followed by a failing assertion to verify
 //     abort behavior and messaging.
 //   - When UNITY_EXCLUDE_DETAILS is defined (via compile flag), the focal test hits
 //     the TEST_IGNORE() branch and is skipped, validating the "details exclusion" path.
 // - This approach keeps the test suite aligned with the existing Unity testing style
 //   without introducing a heavy external framework (per the project constraints).
 // - The test is self-contained: it uses the focal function's own internal logic and
 //   Unity's abort-handling semantics, ensuring faithful coverage of the intended
 //   behavior described by the focal method.
/*
Unit Test Suite for CheckFormatters15 (fast_float_testbed.c)

Step 1 - Program Understanding (summary within comments)
- Focal method: CheckFormatters15(void)
- Purpose: Invoke a series of formatter checks via the macro C(TYPE_..._15) over a wide set of color/format types.
- Core dependencies (from the provided focal class dependency block):
  - The testbed provides a heavy set of TYPE_... constants (e.g., TYPE_GRAY_15, TYPE_RGB_15, TYPE_CMYK_15, etc.)
  - A macro C(...) is used to report/validate each formatter type within CheckFormatters15.
  - Supporting stubbed functions exist (e.g., CheckSingleFormatter15) and a number of related helpers; in this test setup, those are provided as non-operational placeholders to allow linking.
- Candidate Keywords (core dependent components to reflect in tests):
  - TYPE_GRAY_15, TYPE_GRAY_15_REV, TYPE_GRAY_15_SE
  - TYPE_GRAYA_15, TYPE_GRAYA_15_SE, TYPE_GRAYA_15_PLANAR
  - TYPE_RGB_15, TYPE_RGB_15_PLANAR, TYPE_RGB_15_SE
  - TYPE_BGR_15, TYPE_BGR_15_PLANAR, TYPE_BGR_15_SE
  - TYPE_RGBA_15, TYPE_RGBA_15_PLANAR, TYPE_RGBA_15_SE
  - TYPE_ARGB_15, TYPE_ABGR_15, TYPE_ABGR_15_PLANAR, TYPE_ABGR_15_SE
  - TYPE_BGRA_15, TYPE_BGRA_15_SE
  - TYPE_YMC_15, TYPE_CMY_15, TYPE_CMY_15_PLANAR, TYPE_CMY_15_SE
  - TYPE_CMYK_15, TYPE_CMYK_15_REV, TYPE_CMYK_15_PLANAR, TYPE_CMYK_15_SE
  - TYPE_KYMC_15, TYPE_KYMC_15_SE
  - TYPE_KCMY_15, TYPE_KCMY_15_REV, TYPE_KCMY_15_SE
- Notes:
  - The dependencies show that the CheckFormatters15 path relies on C(...) macros and a number of type constants. The test harness will focus on executing the function to exercise all the TYPE_... pathways without assuming internal validation semantics (as the surrounding stubs are non-operational in this captured context).
  - We will treat CheckFormatters15 as a black-box that should complete without crashing. Since it returns void, we cannot directly assert internal state; we will ensure the function can be invoked multiple times safely.

Step 2 - Unit Test Generation (test plan)
- Primary test: Call CheckFormatters15() once to exercise all type paths.
- Secondary test: Call CheckFormatters15() twice to ensure re-entrancy/consistency (no crash on repeated use).
- Rationale: The macro-driven sequence in CheckFormatters15 should execute all TYPE_… lines. The surrounding stubs are designed to allow linking and execution without side effects; thus the objective is to verify stability and non-crashing execution across the full type list.

Step 3 - Test Case Refinement (enhanced coverage considerations)
- Since we cannot alter the focal code or inspect internal macro outcomes, we maximize coverage by:
  - Invoking CheckFormatters15 at least twice.
  - Ensuring the test harness uses a lightweight, non-terminating assertion approach (we will not abort on failures; if a crash occurs, the program will terminate abnormally which still detects an issue in practice).
  - Keeping tests independent of private internals; we only rely on the public symbol: CheckFormatters15.
- Static setting considerations: The project uses C-style linkage for the focal code. Our test uses extern "C" to correctly reference C linkage.

Important: This test suite uses a minimal, self-contained harness (no GTest). It assumes the environment provides the required headers for the focal code (e.g., fast_float_internal.h) and that linking against fast_float_testbed.c (as part of the project) is performed by the build system.

Code (C++11, no GTest), with explanatory comments on each test
---------------------------------------------------------------*/

// Note: This test file is designed to be compiled with a C++11 compiler.
// It declares the C function CheckFormatters15 with C linkage and invokes it.
// The test harness is a lightweight, non-terminating assertion style suitable
// for environments where GTest is unavailable.

#include <cstdio>
#include <fast_float_internal.h>
#include <iostream>
#include <cstdlib>
#include <memory.h>
#include <stdlib.h>


// Forward declaration of the focal function under test (C linkage)
extern "C" void CheckFormatters15(void);

// Simple, non-terminating assertion helper.
// Keeps track of failures but continues execution to maximize coverage.
static int g_test_failures = 0;

#define WARN_IF_FALSE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "Test warning: " << (msg) << " (condition: " #cond ")" << std::endl; \
        ++g_test_failures; \
    } \
} while(0)

static void test_CheckFormatters15_runs_once(void)
{
    // Test purpose:
    // - Validate that the focal function CheckFormatters15 can be invoked once
    //   without crashing and completes execution over all FORMATTER TYPE macros.
    // - This is a runtime invocation test, not a functional assertion of internal state.
    std::cout << "Running test: test_CheckFormatters15_runs_once" << std::endl;

    // Call the focal method. If it crashes, the test program will terminate.
    CheckFormatters15();

    // Basic sanity: program continues past the call.
    WARN_IF_FALSE(true, "CheckFormatters15 returned (no crash observed).");
}

static void test_CheckFormatters15_runs_twice(void)
{
    // Test purpose:
    // - Validate that repeated invocation does not cause instability or crashes.
    // - Exercises potential internal static/stateful paths in the macro expansion.
    std::cout << "Running test: test_CheckFormatters15_runs_twice" << std::endl;

    CheckFormatters15();
    CheckFormatters15();

    WARN_IF_FALSE(true, "CheckFormatters15 re-entrancy (second call) completed without crash.");
}

// Entry point: run all tests and report summary.
// We intentionally avoid terminating on first failure to maximize coverage.
int main(void)
{
    std::cout << "Starting unit test suite for CheckFormatters15 (fast_float_testbed.c)" << std::endl;

    test_CheckFormatters15_runs_once();
    test_CheckFormatters15_runs_twice();

    if (g_test_failures > 0) {
        std::cerr << "Unit test completed with " << g_test_failures << " non-fatal failure(s)." << std::endl;
        // Return non-zero to indicate warnings/failures observed (as a signal for CI).
        return 1;
    } else {
        std::cout << "Unit test completed successfully. All tests executed." << std::endl;
        return 0;
    }
}

/*
Notes for maintainers:
- This test suite focuses on exercising the focal function CheckFormatters15 via a direct
  invocation. Given the surrounding C macros and stubs, internal behavioral assertions
  (e.g., validating exact formatter registration) are not feasible without augmenting
  the testbed to expose internal state or extend the API. If future revisions expose more
  observable behavior (return codes, logs, or callbacks), additional assertions can be added
  here to improve coverage and validation granularity.
- The test follows the domain knowledge guideline to only rely on the provided public symbol
  CheckFormatters15 and ensure tests compile under C++11 without GTest. Static/private details
  of the focal code are not accessed directly.
- If additional dependencies (headers) are required by the test environment, include them at the
  top of this file and adapt extern declarations accordingly.
*/
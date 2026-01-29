/*
Unit Test Suite for Check5StageLUT (C interface)

Step 1 (Program Understanding & Candidate Keywords)
- Core components involved by Check5StageLUT:
  - cmsPipeline* lut = cmsPipelineAlloc(DbgThread(), 3, 3);
  - AddIdentityMatrix(lut);
  - AddIdentityCLUTfloat(lut);
  - Add3GammaCurves(lut, 1.0);
  - AddIdentityMatrix(lut);
  - Add3GammaCurves(lut, 1.0);
  - return CheckFullLUT(lut, 5);

- Candidate Keywords representing dependencies:
  - cmsPipelineAlloc, DbgThread
  - AddIdentityMatrix, AddIdentityCLUTfloat
  - Add3GammaCurves
  - CheckFullLUT
  - lut (cmsPipeline), memory/resource management around a pipeline
  - 5 (ExpectedStages)

- Step 2 (Unit Test Generation)
  - The tests focus on Check5StageLUT() behavior and basic stability (true return path and repeated-call stability).
  - We cannot modify internal static or private file-scope behavior from the focal file; tests therefore validate the observable contract: that Check5StageLUT returns 1 under normal conditions and remains stable across multiple invocations.
  - We expose the C function via extern "C" and call it from C++ test harness without any GTest framework (per instructions).

- Step 3 (Test Refinement)
  - Provide concise, self-contained tests using a lightweight, non-terminating assertion style (print-only on failure; continue execution).
  - Ensure tests compile under C++11 and rely only on standard library plus the provided C API (Check5StageLUT).

Notes:
- This harness assumes the presence of the Little CMS-derived test scaffolding in the linking environment. We only declare the C function prototype for linking.
- Static functions internal to the focal file are not accessible from tests; coverage focuses on the observable behavior of Check5StageLUT.

Code (C++11 test suite)
// Explanatory C++-side test harness for Check5StageLUT
// No GTest; simple inline tests with non-terminating assertions (via prints)

#include <iostream>
#include <testcms2.h>


// Step 2: Declaring the C function under test.
// We assume the actual implementation uses C linkage; hence extern "C".
extern "C" int Check5StageLUT(void);
/*
  The function signature in the focal C code is:
    cmsInt32Number Check5StageLUT(void)
  which maps to a C int return in our test harness.
*/

static bool test_Check5StageLUT_ReturnsOne() {
    // Test: Check5StageLUT should return 1 (success) for the standard 5-stage LUT path.
    // This exercises the exact code path:
    //   - allocate lut
    //   - identity matrix
    //   - identity CLUT (float)
    //   - gamma curves (1.0)
    //   - identity matrix
    //   - gamma curves (1.0)
    //   - full LUT check with ExpectedStages = 5
    int res = Check5StageLUT();
    if (res != 1) {
        std::cerr << "[Test] Check5StageLUT_ReturnsOne: FAILED (expected 1, got " << res << ")\n";
        return false;
    }
    std::cout << "[Test] Check5StageLUT_ReturnsOne: PASSED\n";
    return true;
}

static bool test_Check5StageLUT_MultipleRunsAreStable() {
    // Test: Call Check5StageLUT repeatedly to ensure no crash and consistent return value.
    // This helps cover potential memory management issues or state retention between calls.
    const int kRuns = 5;
    for (int i = 0; i < kRuns; ++i) {
        int res = Check5StageLUT();
        if (res != 1) {
            std::cerr << "[Test] Check5StageLUT_MultipleRunsAreStable: FAILED on run "
                      << i << " (expected 1, got " << res << ")\n";
            return false;
        }
    }
    std::cout << "[Test] Check5StageLUT_MultipleRunsAreStable: PASSED (" << kRuns << " consecutive runs)\n";
    return true;
}

// Entry point-based test harness.
// Step 3: Tests are invoked from main as per guidance "call test methods from the main function" when gtest is not used.
int main() {
    std::cout << "Starting Check5StageLUT test suite (C++11) ...\n";

    bool all_passed = true;

    // Run individual tests, collecting overall status.
    if (!test_Check5StageLUT_ReturnsOne()) all_passed = false;
    if (!test_Check5StageLUT_MultipleRunsAreStable()) all_passed = false;

    if (all_passed) {
        std::cout << "ALL TESTS PASSED.\n";
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED.\n";
        return 1;
    }
}
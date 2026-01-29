/*
Unit Test Suite for focal method: Check8DinterpGranular
Context:
- The focal method Check8DinterpGranular constructs an 8-input, 3-output color look-up pipeline
  using a granular 16-bit CLUT, samples it with a provided Sampler8D function, and then validates
  several input/output pairs via CheckOne8D. It returns 1 on success and 0 on any failure.
- The test targets the function as is, using the project’s existing C API (cmsPipelineAlloc,
  cmsStageAllocCLut16bitGranular, cmsStageSampleCLut16bit, cmsPipelineInsertStage, etc.).
- All code is expected to be compiled under C++11, but the code under test is C-based.
- No use of Google Test; a lightweight, non-terminating test harness is provided that reports
  results to stdout and exits with a non-zero status on failure.
- The test imports C sources via extern "C" to prevent C++ name mangling.

Candidate Keywords (Step 1):
- cmsPipelineAlloc, cmsStageAllocCLut16bitGranular, cmsStageSampleCLut16bit
- cmsPipelineInsertStage, cmsAT_BEGIN, Sampler8D
- CheckOne8D, 8D interpolation, CLUT16bitGranular, Dimensions array
- lut (cmsPipeline*), mpe (cmsStage*)
- Memory cleanup: cmsPipelineFree
- DbgThread (context), 8x3 colour space (8 inputs, 3 outputs)

Notes on Domain Knowledge applied:
- Tests rely on existing test helpers declared in testcms2.h (e.g., DbgThread, Sampler8D, CheckOne8D).
- Interactions with static/internal helpers remain encapsulated within the focal function; tests should
  exercise the observable behavior (return value and side effects) without attempting to mock private/static state.
- C APIs are invoked inside a C++ test, wrapped by extern "C" to avoid linkage issues.
- Memory management (cmsPipelineFree) is exercised indirectly by the focal method; we rely on the function
  to perform proper cleanup on success. The test itself focuses on success/failure reporting rather than
  explicit leak assertions (to stay within given interfaces).

Test file (test_check8dinterpgranular.cpp)
- Provides a minimal test harness that invokes Check8DinterpGranular and reports PASS/FAIL.
- Uses non-terminating checks: the test prints results instead of aborting, enabling potential expansion with additional tests later.

Code:

*/

#include <iostream>
#include <cstdio>
#include <testcms2.h>


extern "C" {
}

/*
Test plan for Check8DinterpGranular:
- Call the focal function and verify it returns 1 (success).
- Do not terminate on first failure to allow future extension with additional checks
  or multiple independent testcases.

Test Case 1: Check8DinterpGranular_ReturnsSuccess
- Rationale: Validate the core functionality as exercised by the original code path:
  allocation of LUT, creation of CLUT16bitGranular stage, sampling, insertion into pipeline,
  and a sequence of accuracy checks via CheckOne8D. If any of these fail, the function returns 0.
- Expected: The function should return 1 and not crash, with proper cleanup.

Implementation details:
- Call Check8DinterpGranular() and print PASS/FAIL.
- Return non-zero on fail to indicate test harness failure.

*/

// Lightweight test for the focal function: expect success (return 1)
bool Test_Check8DinterpGranular_ReturnsSuccess() {
    cmsInt32Number result = Check8DinterpGranular();
    if (result == 1) {
        std::cout << "PASS: Check8DinterpGranular() returned success as expected.\n";
        return true;
    } else {
        std::cout << "FAIL: Check8DinterpGranular() did not return success.\n";
        return false;
    }
}

/*
Note on static/private dependencies (Step 3):
- The test only exercises the public-facing focal method. The file testcms2.h contains helper
  declarations used by the test harness and by Check8DinterpGranular (e.g., DbgThread, Sampler8D, CheckOne8D).
- We do not attempt to mock or access static internal helpers directly, in accordance with the
  constraint "Static functions at the file scope are only visible within the file."
- If needed in future tests, additional tests could construct a similar pipeline and verify
  precursor utilities (e.g., Sampler8D behavior) by calling the publicly declared test helpers.

Main function (test runner)
- Runs all defined tests and returns non-zero if any test fails.

*/

int main() {
    bool ok = Test_Check8DinterpGranular_ReturnsSuccess();

    // Extend with additional tests in the future, combining results as needed.
    if (!ok) {
        // Non-zero exit indicates test failure
        return 1;
    }

    // All tests passed
    return 0;
}
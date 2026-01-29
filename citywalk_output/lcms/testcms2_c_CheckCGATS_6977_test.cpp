/*
  Test Suite for CMS CheckCGATS (Step 2: Unit Test Generation)
  Target: Validate the focal method cmsInt32Number CheckCGATS(void) from testcms2.c
  Language: C++11 (no GTest, no GMock)
  Approach:
  - Expose the C function CheckCGATS via extern "C" and invoke it from C++ test harness.
  - Provide at least two tests to exercise the function:
      1) Basic successful run (normal environment, expected return = 1)
      2) Idempotent/robust successive runs (calling CheckCGATS twice should both succeed)
  - Use a lightweight, non-terminating test harness (print PASS/FAIL, but continue execution)
  - Clean up environment (e.g., ensure TEST.IT8 is not pre-existing before tests)
  - Do not modify private/static internals of the focal code; only exercise via the public function.
  - Include explanatory comments for each unit test.

  Step 1: Candidate Keywords representing the focal method's core dependencies
  - It8 related: cmsIT8Alloc, cmsIT8SetSheetType, cmsIT8SetPropertyStr, cmsIT8SetPropertyUncooked,
    cmsIT8SetPropertyDbl, cmsIT8SetPropertyHex, cmsIT8SetDataFormat, cmsIT8SetDataRowCol,
    cmsIT8SetDataRowColDbl, cmsIT8SaveToFile, cmsIT8LoadFromFile, cmsIT8Free, cmsIT8GetPropertyDbl,
    cmsIT8GetDataDbl
  - File names / IO: "TEST.IT8", remove()
  - Test harness: SubTest, DbgThread (provided by test framework), NPOINTS_IT8
  - Data formats: "SAMPLE_ID", "RGB_R/G/B", property keys like "DESCRIPTOR", "CREATED", "SERIAL"
  - Control flow: Early returns on failure, success on final return 1

  Step 3: Test Case Refinement notes
  - Cover true branch (successful path) by relying on the expected behavior of CheckCGATS returning 1 after a full IT8 round-trip.
  - Cover false/edge conditions indirectly by ensuring the environment is clean and there are no pre-existing test artifacts that could cause a failure; however, we cannot alter the focal code to simulate failures.
  - Use simple, portable assertions (non-terminating; do not abort on failure) by collecting results and reporting at the end.
  - Tests run under a single process; no private members access required.
  - All tests rely on the C API provided by testcms2.c and linked objects.

  Note: This test suite assumes the existence of a C linkage function:
        extern "C" int CheckCGATS(void);
  and that the project links testcms2.c with a compatible LCMS-based build.
*/

#include <iostream>
#include <string>
#include <cstdio>
#include <testcms2.h>


// Link to the focal C function CheckCGATS from testcms2.c
extern "C" int CheckCGATS(void);

// Lightweight test harness (non-terminating; accumulates results)
static int g_tests_run = 0;
static int g_tests_passed = 0;

#define RUN_TEST(fn) do {                                      \
    ++g_tests_run;                                               \
    bool _ok = (fn)();                                           \
    if (_ok) {                                                   \
        ++g_tests_passed;                                        \
        std::cout << "[PASS] " << #fn << std::endl;             \
    } else {                                                     \
        std::cout << "[FAIL] " << #fn << std::endl;             \
    }                                                            \
} while (0)

// Utility to ensure a clean environment before tests
static void EnsureCleanEnvironment() {
    // Remove any leftover IT8 file if present before tests begin
    std::remove("TEST.IT8");
}

/*
  Test 1: Basic CheckCGATS execution
  - Rationale: Verifies the focal function can run through its entire IT8 creation/loading/save cycle
    and return a success status (1) under normal conditions.
  - Preconditions: No pre-existing TEST.IT8 file to interfere with the flow (the function cleans up itself).
  - Postcondition: TEST.IT8 should be removed by the focal function; the return value should be 1.
*/
static bool test_CheckCGATS_basic() {
    // Ensure clean environment prior to test
    std::remove("TEST.IT8"); // Best-effort cleanup if a stray file exists

    // Call the focal function
    int ret = CheckCGATS();

    // Expect success (non-zero/1 in this codebase)
    bool ok = (ret == 1);

    // Explanation/comment:
    // - This test exercises the full positive path of CheckCGATS, including IT8 creation,
    //   data formatting, file I/O, and property/data reads.
    // - Uses the standard file "TEST.IT8" and asserts the final outcome is success.

    return ok;
}

/*
  Test 2: CheckCGATS idempotence / robustness on consecutive runs
  - Rationale: Ensure that calling CheckCGATS twice in succession does not crash and both
    executions return success (1). The helper removes the test artifact at the end of the
    first run; the second run should still complete the full cycle.
  - Preconditions: Clean environment prior to first call; no pre-existing TEST.IT8 after cleanup.
  - Postcondition: Both invocations return 1.
*/
static bool test_CheckCGATS_twice() {
    // Ensure clean environment prior to test
    std::remove("TEST.IT8");

    // First run
    int r1 = CheckCGATS();
    if (r1 != 1) return false;

    // Second run
    int r2 = CheckCGATS();
    if (r2 != 1) return false;

    // Explanation/comment:
    // - Validates that resources (files, handles) are properly cleaned up between runs.
    // - Confirms the function can be invoked multiple times in a row without error.

    return true;
}

int main(int argc, char* argv[]) {
    // Global test setup
    EnsureCleanEnvironment();

    std::cout << "=== Test Suite: CheckCGATS (focal method) ===" << std::endl;

    // Run tests
    RUN_TEST(test_CheckCGATS_basic);
    RUN_TEST(test_CheckCGATS_twice);

    // Summary
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Total tests run: " << g_tests_run << std::endl;
    std::cout << "Total passed:    " << g_tests_passed << std::endl;

    // Return non-zero if any test failed
    return (g_tests_run == g_tests_passed) ? 0 : 1;
}
/*
Unit Test Suite for the focal method: CheckReverseDegenerated

Context:
- The focal method resides in testcms2.c and is defined as:
  cmsInt32Number CheckReverseDegenerated(void)
  {
      cmsToneCurve* p, *g;
      cmsUInt16Number Tab[16];
      // ... fills Tab with specified values ...
      p = cmsBuildTabulatedToneCurve16(DbgThread(), 16, Tab);
      g = cmsReverseToneCurve(p);
      if (!CheckFToneCurvePoint(g, 0x5555, 0x5555)) return 0;
      if (!CheckFToneCurvePoint(g, 0x7777, 0x7777)) return 0;
      if (!CheckFToneCurvePoint(g, 0x0000, 0x4444)) return 0;
      if (!CheckFToneCurvePoint(g, 0xFFFF, 0xFFFF)) return 0;
      cmsFreeToneCurve(p);
      cmsFreeToneCurve(g);
      return 1;
  }

- Core dependencies (from <FOCAL_CLASS_DEP>):
  cmsBuildTabulatedToneCurve16, DbgThread, cmsReverseToneCurve, CheckFToneCurvePoint, cmsFreeToneCurve, and the tone-curve types cmsToneCurve, cmsUInt16Number.

- The function is deterministic given its internal data (Tab), and we cannot alter internal static data to exercise alternate branches without editing the source. The test suite therefore focuses on validating the expected success path and basic behavior across repeated invocations (idempotence), while keeping in line with the constraint of not using GTest and using a lightweight, non-terminating assertion style.

- Domain constraints observed:
  - We cannot access static helpers like CheckFToneCurvePoint directly since it’s static inside the source file; tests target the public interface CheckReverseDegenerated.
  - We will expose the function under test via C linkage for the C++ test harness.

How this test suite is structured:
- Lightweight, non-terminating test harness (no abort on failure). Failures are logged and counted, but tests continue to maximize coverage.
- Two tests:
  1) CheckReverseDegenerated_ReturnsOne: Verifies the expected successful path returns 1.
  2) CheckReverseDegenerated_Idempotence: Calls the function repeatedly to ensure repeated invocations remain successful and do not introduce state-related issues.
- The tests are executed from main() in a single translation unit, suitable for compilation without GTest.

Notes:
- The test harness imports the focal function via C linkage:
  extern "C" cmsInt32Number CheckReverseDegenerated(void);
- The test relies on the existing testcms2.c implementation of the required dependencies (DbgThread, cmsBuildTabulatedToneCurve16, cmsReverseToneCurve, cmsFreeToneCurve, and the tone-curve API) being linkable at link time.

Now providing the test code:

*/

#include <iostream>
#include <string>
#include <testcms2.h>


// Declaration of the focal function under test (C linkage to ensure proper linkage with C code)
extern "C" cmsInt32Number CheckReverseDegenerated(void);

// Candidate Keywords representing core dependent components (for traceability in Step 1)
// - cmsInt32Number
// - CheckReverseDegenerated
// - cmsToneCurve
// - cmsUInt16Number
// - Tab
// - cmsBuildTabulatedToneCurve16
// - DbgThread
// - cmsReverseToneCurve
// - CheckFToneCurvePoint
// - cmsFreeToneCurve
// These are the key entities involved in the function's logic and its dependencies.

static int g_failures = 0;

// Simple non-terminating assertion helper
static void ExpectTrue(bool cond, const std::string& msg)
{
    if (!cond) {
        std::cerr << "FAIL: " << msg << std::endl;
        ++g_failures;
    } else {
        std::cout << "PASS: " << msg << std::endl;
    }
}

// Test 1: Validate the expected success path of CheckReverseDegenerated.
// Rationale: The focal function is designed to return 1 on success for the
// given deterministic input (Tab values). This test ensures that the
// function returns 1 in the standard scenario.
void Test_CheckReverseDegenerated_ReturnsOne()
{
    cmsInt32Number res = CheckReverseDegenerated();
    ExpectTrue(res == 1, "CheckReverseDegenerated should return 1 on valid deterministic input (expected success path).");
}

// Test 2: Validate idempotence and stability across multiple invocations.
// Rationale: The function constructs and frees resources (tone curves) per call.
// Repeating the call should consistently yield the same result without side effects.
void Test_CheckReverseDegenerated_Idempotence()
{
    const int iterations = 5;
    for (int i = 0; i < iterations; ++i) {
        cmsInt32Number res = CheckReverseDegenerated();
        if (res != 1) {
            std::cerr << "FAIL: CheckReverseDegenerated() returned " << res
                      << " on iteration " << i << ", expected 1." << std::endl;
            ++g_failures;
        } else {
            std::cout << "PASS: Iteration " << i
                      << " returned 1 as expected." << std::endl;
        }
    }
}

// Entry point: run all tests and report a summary.
// This follows the requirement to call test methods from main when a dedicated
// test framework is not used.
int main()
{
    std::cout << "Running unit tests for CheckReverseDegenerated..." << std::endl;

    Test_CheckReverseDegenerated_ReturnsOne();
    Test_CheckReverseDegenerated_Idempotence();

    if (g_failures > 0) {
        std::cerr << "\nSummary: " << g_failures
                  << " test(s) failed." << std::endl;
        return 1;
    } else {
        std::cout << "\nSummary: All tests passed." << std::endl;
        return 0;
    }
}
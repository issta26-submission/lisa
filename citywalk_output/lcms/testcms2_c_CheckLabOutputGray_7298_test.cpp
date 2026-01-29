/*
  Unit test suite for the focal method CheckLabOutputGray from testcms2.c

  Step 1 (summary): The focal method CheckLabOutputGray creates two profiles
  (GrayLab and Lab4) and a color transform between Lab (double precision) and Gray
  (8-bit). It then validates the transform by:
    - mapping L=0 -> g=0
    - mapping L=100 -> g=255
    - mapping a sequence of L values [0..99] to expected Gray outputs
  It finally deletes the transform and returns 1 on success or 0 on failure.

  Step 2 (test targets): We will exercise the method CheckLabOutputGray through a small
  C++11 test harness (no GTest). Since the function under test is a C function,
  we declare it with C linkage in the test and call it directly.

  Step 3 (test refinement): We provide a lightweight harness with two test cases:
    - Test_CheckLabOutputGray_Returns1: verifies the function returns 1 on a normal run.
    - Test_CheckLabOutputGray_RepeatedCalls: verifies that two successive calls both succeed,
      ensuring no persistent state leads to a failure on subsequent runs.

  Notes:
  - The tests rely solely on the public API CheckLabOutputGray (and the underlying C library
    that implements the function). We do not mock private or static internals.
  - We use a minimal, self-contained test runner (no external testing framework).
  - The tests print diagnostic messages on failure and pass/fail summary on exit.
  - The code is designed to compile under C++11 and link with the C library that provides
    testcms2.c (and its test headers if available).

  Candidate Keywords (from Step 1 understanding):
  - Create_GrayLab, cmsCreateLab4Profile, cmsCreateTransform, cmsCloseProfile, TYPE_Lab_DBL,
    TYPE_GRAY_8, INTENT_RELATIVE_COLORIMETRIC, CheckOutGray, cmsDeleteTransform
  - The mapping of Lab to Gray and the boundary checks for L in [0, 100] and g in [0, 255]
  - Resource management (profiles and transforms) and return semantics (0/1)
*/

#include <iostream>
#include <testcms2.h>


// The focal function is defined in a C source (testcms2.c). We declare it with C linkage.
extern "C" int CheckLabOutputGray(void);

static bool Test_CheckLabOutputGray_Returns1() {
    // Purpose: Ensure a single invocation returns success (1).
    // The function performs internal profile creation, transform creation, evaluation,
    // and cleanup. A return value other than 1 indicates a failure in the test path.
    int result = CheckLabOutputGray();
    if (result != 1) {
        std::cerr << "[Fail] Test_CheckLabOutputGray_Returns1: expected 1, got " << result << std::endl;
        return false;
    }
    // Success
    return true;
}

static bool Test_CheckLabOutputGray_RepeatedCalls() {
    // Purpose: Verify that two consecutive invocations both succeed.
    // This checks for potential static-state leakage or resource mismanagement across calls.
    int r1 = CheckLabOutputGray();
    int r2 = CheckLabOutputGray();
    if (r1 != 1 || r2 != 1) {
        std::cerr << "[Fail] Test_CheckLabOutputGray_RepeatedCalls: expected (1,1), got ("
                  << r1 << "," << r2 << ")" << std::endl;
        return false;
    }
    // Success
    return true;
}

int main() {
    int total = 0;
    int passed = 0;

    // Run first test
    if (Test_CheckLabOutputGray_Returns1()) ++passed;
    ++total;

    // Run second test
    if (Test_CheckLabOutputGray_RepeatedCalls()) ++passed;
    ++total;

    std::cout << "Test suite finished: "
              << passed << " / " << total << " tests passed." << std::endl;

    return (passed == total) ? 0 : 1;
}
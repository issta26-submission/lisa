/***************************************************************
 * Test Suite for CheckSaveLinearizationDevicelink
 * 
 * This file provides a lightweight C++11 test harness (no GTest)
 * to exercise the focal method:
 *   int CheckSaveLinearizationDevicelink(void)
 * 
 * Notes and rationale (mapped to the task steps):
 * Step 1 - Program Understanding (Candidate Keywords)
 * - cmsBuildTabulatedToneCurveFloat, cmsToneCurve
 * - cmsCreateLinearizationDeviceLink, cmsSaveProfileToFile
 * - lin_rgb.icc, cmsOpenProfileFromFile, cmsCreateTransform
 * - TYPE_RGB_8, INTENT_PERCEPTUAL
 * - cmsDoTransform, rgb_in/rgb_out comparisons
 * - cmsFreeToneCurve, cmsCloseProfile, cmsDeleteTransform
 * - Fail(), remove(), 1/0 return semantics
 * 
 * Step 2 - Unit Test Generation (Guidance)
 * - Focus on positive path (function returns 1) since the negative path
 *   (failure branches) depend on internal library behavior that is hard
 *   to deterministically force in a test harness without invasive
 *   function interposition.
 * - Provide tests that verify: (a) successful execution returns 1, (b)
 *   no output file persists after the call, (c) multiple invocations succeed.
 * 
 * Step 3 - Test Case Refinement (Domain Knowledge)
 * - Use standard C++11 facilities only.
 * - Use a minimal, self-contained test harness with non-terminating
 *   assertions (returning false on failure, not aborting the test)
 * - Access static/dynamic behavior via public APIs only; do not touch private
 *   members or internal static state.
 * - Use simple file-system checks to validate side effects without requiring
 *   mocking frameworks.
 * - Call test methods from main (no external test runner).
 * 
 * Limitations:
 * - The focal path is highly dependent on the LittleCMS runtime behavior.
 * - Coverage of the exact false-branch (cmsSaveProfileToFile returning false)
 *   is not reliably achievable without invasive interposition; the test
 *   suite focuses on validating the successful execution path and cleanup.
 * 
 * How to run:
 * - Compile with a C++11 compliant compiler, linking against LittleCMS
 *   headers/libs and the provided testcms2.h (as per the project layout).
 * - Ensure the working directory is writable for initial file generation.
 * 
 ***************************************************************/

#include <cstdio>
#include <iostream>
#include <testcms2.h>
#include <lcms2.h>
#include <cstdlib>
#include <unistd.h>


// Ensure C linkage for the C headers
extern "C" {
}

// Simple non-terminating test assertion helpers (no GTest)
#define ASSERT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "ASSERT_TRUE failed: " << (msg) << std::endl; \
        return false; \
    } \
} while(0)

#define ASSERT_FALSE(cond, msg) do { \
    if((cond)) { \
        std::cerr << "ASSERT_FALSE failed: " << (msg) << std::endl; \
        return false; \
    } \
} while(0)

#define ASSERT_EQ(a, b, msg) do { \
    if((a) != (b)) { \
        std::cerr << "ASSERT_EQ failed: " << (msg) \
                  << " | got: " << (a) << ", expected: " << (b) << std::endl; \
        return false; \
    } \
} while(0)


// Helper tests

// 1) Positive path: CheckSaveLinearizationDevicelink should return 1
/* Test: Verify that the focal function successfully saves, reopens, and uses
 * the linearization devicelink without error. We rely on the function returning 1
 * as success. The internal cleanup of lin_rgb.icc is performed by the function.
 */
bool Test_CheckSaveLinearizationDevicelink_PositivePath() {
    int rv = CheckSaveLinearizationDevicelink();
    if (rv != 1) {
        std::cerr << "Test_CheckSaveLinearizationDevicelink_PositivePath: "
                  << "CheckSaveLinearizationDevicelink() returned " << rv
                  << " (expected 1)." << std::endl;
        return false;
    }
    // Also verify that no stray file remains (the function should remove lin_rgb.icc)
    if (access("lin_rgb.icc", F_OK) == 0) {
        std::cerr << "Test_CheckSaveLinearizationDevicelink_PositivePath: "
                  << "Expected lin_rgb.icc to be removed, but it exists." << std::endl;
        remove("lin_rgb.icc");
        return false;
    }
    return true;
}


// 2) Cleanup verification: Ensure the function does not leave a file behind
/* Test: Run the function and confirm no lin_rgb.icc file persists afterwards.
 * This reinforces correct cleanup behavior in the success path.
 */
bool Test_CheckSaveLinearizationDevicelink_NoResidualFile() {
    int rv = CheckSaveLinearizationDevicelink();
    if (rv != 1) {
        std::cerr << "Test_CheckSaveLinearizationDevicelink_NoResidualFile: "
                  << "CheckSaveLinearizationDevicelink() returned " << rv
                  << " (expected 1)." << std::endl;
        return false;
    }
    bool exists = (access("lin_rgb.icc", F_OK) == 0);
    if (exists) {
        std::cerr << "Test_CheckSaveLinearizationDevicelink_NoResidualFile: "
                  << "lin_rgb.icc should have been removed but still exists." << std::endl;
        remove("lin_rgb.icc");
        return false;
    }
    return true;
}


// 3) Repeated invocation: Ensure multiple consecutive runs succeed
/* Test: Call the function twice in a row. The first run should clean up, the
 * second run should also complete successfully without leaving artifacts.
 */
bool Test_CheckSaveLinearizationDevicelink_MultipleRuns() {
    int rv1 = CheckSaveLinearizationDevicelink();
    if (rv1 != 1) {
        std::cerr << "Test_CheckSaveLinearizationDevicelink_MultipleRuns: first run failed ("
                  << rv1 << ")." << std::endl;
        return false;
    }
    // Ensure cleanup after first run
    if (access("lin_rgb.icc", F_OK) == 0) {
        remove("lin_rgb.icc");
        std::cerr << "Test_CheckSaveLinearizationDevicelink_MultipleRuns: "
                  << "Unexpected leftover file after first run." << std::endl;
        return false;
    }

    int rv2 = CheckSaveLinearizationDevicelink();
    if (rv2 != 1) {
        std::cerr << "Test_CheckSaveLinearizationDevicelink_MultipleRuns: second run failed ("
                  << rv2 << ")." << std::endl;
        return false;
    }
    // Final cleanup check
    if (access("lin_rgb.icc", F_OK) == 0) {
        remove("lin_rgb.icc");
        std::cerr << "Test_CheckSaveLinearizationDevicelink_MultipleRuns: "
                  << "lin_rgb.icc should have been removed after second run." << std::endl;
        return false;
    }
    return true;
}


// Main test harness
int main(int argc, char* argv[]) {
    // Simple test suite execution
    int tests_passed = 0;
    int tests_total  = 0;

    std::cout << "Running TestSuite: CheckSaveLinearizationDevicelink (C++11, no GTest)" << std::endl;

    // 1) Positive path
    ++tests_total;
    if (Test_CheckSaveLinearizationDevicelink_PositivePath()) {
        std::cout << "[PASS] Test_CheckSaveLinearizationDevicelink_PositivePath" << std::endl;
        ++tests_passed;
    } else {
        std::cout << "[FAIL] Test_CheckSaveLinearizationDevicelink_PositivePath" << std::endl;
    }

    // 2) No residual file cleanup
    ++tests_total;
    if (Test_CheckSaveLinearizationDevicelink_NoResidualFile()) {
        std::cout << "[PASS] Test_CheckSaveLinearizationDevicelink_NoResidualFile" << std::endl;
        ++tests_passed;
    } else {
        std::cout << "[FAIL] Test_CheckSaveLinearizationDevicelink_NoResidualFile" << std::endl;
    }

    // 3) Multiple runs
    ++tests_total;
    if (Test_CheckSaveLinearizationDevicelink_MultipleRuns()) {
        std::cout << "[PASS] Test_CheckSaveLinearizationDevicelink_MultipleRuns" << std::endl;
        ++tests_passed;
    } else {
        std::cout << "[FAIL] Test_CheckSaveLinearizationDevicelink_MultipleRuns" << std::endl;
    }

    std::cout << "Test results: " << tests_passed << " / " << tests_total << " tests passed." << std::endl;

    return (tests_passed == tests_total) ? 0 : 1;
}
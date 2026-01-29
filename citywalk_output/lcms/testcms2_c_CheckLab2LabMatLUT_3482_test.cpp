// Test suite for the focal method: CheckLab2LabMatLUT
// This test targets the C API exposed by testcms2.h and the focal function
// defined in testcms2.c. The goal is to validate behavior under normal conditions
// and exercise coverage through multiple test cases without using GTest.

// Notes:
// - We rely on the real implementation of CheckLab2LabMatLUT and its dependencies.
// - The tests are written in plain C++11 and do not terminate on assertion failures.
// - We use a lightweight test harness with non-terminating checks to maximize coverage.

#include <iostream>
#include <testcms2.h>


// Include the C test harness header in a C-linkage block to avoid name mangling issues.
extern "C" {
}

// Simple non-terminating test assertion macro
// Logs failures but continues executing remaining tests.
#define CHECK(cond, msg)                                   \
    do {                                                     \
        if (!(cond)) {                                       \
            std::cerr << "FAIL: " << (msg)                   \
                      << " (line " << __LINE__ << ")"       \
                      << std::endl;                         \
            ++g_failures;                                    \
        } else {                                             \
            std::cout << "PASS: " << (msg) << std::endl;    \
        }                                                    \
    } while (false)

// Global to count test failures in this translation unit
static int g_failures = 0;

// Test 1: Verify that the focal function returns true under normal conditions
// This exercises the happy-path where the Lab-to-XYZ and XYZ-to-Lab stages are
// correctly wired and validated by the LUT checks.
bool Test_CheckLab2LabMatLUT_ReturnsTrue() {
    cmsInt32Number rc = CheckLab2LabMatLUT();
    CHECK(rc != 0, "CheckLab2LabMatLUT should return true on valid LUT composition");
    return rc != 0;
}

// Test 2: Idempotency/Determinism check
// Ensure that repeated invocations yield consistent results. This helps detect
// any non-deterministic behavior in memory management or pipeline construction.
bool Test_CheckLab2LabMatLUT_Idempotent() {
    cmsInt32Number r1 = CheckLab2LabMatLUT();
    cmsInt32Number r2 = CheckLab2LabMatLUT();
    CHECK((r1 == r2), "CheckLab2LabMatLUT should be deterministic across calls");
    // Also verify the result is still true for stability
    CHECK((r1 != 0), "CheckLab2LabMatLUT should be true on repeated calls");
    return (r1 == r2) && (r1 != 0);
}

// Test 3: Basic memory leak/cleanup invocation
// Run a memory leak reporting helper if available. This test doesn't fail the
// test suite on its own but ensures the memory reporting path is exercised.
bool Test_MemoryCleanupInvocation() {
    // The helper is provided in testcms2.h as a monitoring utility.
    // We call it with ok = true to indicate a successful run so far.
    TestMemoryLeaks(true);
    // No assertion failure expected here; we just observe output.
    std::cout << "INFO: Memory leak check invoked for Test_MemoryCleanupInvocation" << std::endl;
    return true;
}

// Test runner: executes all tests and reports final status
int main() {
    g_failures = 0;

    std::cout << "Starting test suite for CheckLab2LabMatLUT..." << std::endl;

    // Run tests
    Test_CheckLab2LabMatLUT_ReturnsTrue();
    Test_CheckLab2LabMatLUT_Idempotent();
    Test_MemoryCleanupInvocation();

    // Final result
    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}
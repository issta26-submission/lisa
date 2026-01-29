// Test suite for the focal method: CheckKPlaneBlackPreserving
// This test is designed to run with a C++11 toolchain without Google Test.
// It exercises the function CheckKPlaneBlackPreserving declared in testcms2.c.
// The test relies on the existence of test1.icc and test2.icc in the working directory
// (as used by the focal function). Ensure these ICC profiles are accessible when running tests.
//
// Notes on design and coverage approach:
// - Test 1 checks the "true" outcome of the final predicate Max < 30.0 (i.e., returns 1).
//   This covers the common/expected path when the provided ICC profiles yield a small DeltaE.
// - Test 2 verifies determinism/repeatability by calling the function twice and ensuring
//   the return value remains within the expected domain {0,1}.
// - We cannot mock the internal static or private helpers inside testcms2.c from here,
//   so we rely on the real implementation and existing ICC profile data.
// - The tests use a minimal, non-terminating assertion style (printing PASS/FAIL) as gtest
//   is not allowed per instructions.
//
// Build tip (example, not part of the code): 
// g++ -std=c++11 -o test_check_kplane test_check_kplane.cpp testcms2.c -llcms2 -lm
// Make sure the LittleCMS libraries and public headers are discoverable by the linker.

#include <iostream>
#include <cstdint>
#include <testcms2.h>


// Forward declaration of the focal function from the C file.
// We assume cmsInt32Number maps to a 32-bit integer (typical in LittleCMS).
// If cmsInt32Number is typedef'd to int in the included headers, this will be compatible.
// If you have a project header that defines cmsInt32Number, include it instead.
// extern "C" cmsInt32Number CheckKPlaneBlackPreserving(void);
extern "C" int CheckKPlaneBlackPreserving(void);

// Simple test harness utilities
static int g_failures = 0;

static void TEST_PASS(const char* msg) {
    std::cout << "[PASS] " << msg << std::endl;
}

static void TEST_FAIL(const char* msg) {
    std::cerr << "[FAIL] " << msg << std::endl;
    ++g_failures;
}

static void CHECK(bool cond, const char* msg) {
    if (cond) {
        TEST_PASS(msg);
    } else {
        TEST_FAIL(msg);
    }
}

// Test 1: Verify that CheckKPlaneBlackPreserving returns true (non-zero) for the provided test ICC files.
// This exercises the true-branch of the final predicate (Max < 30.0) under expected test data.
// Precondition: test1.icc and test2.icc must be present in the process working directory.
static void Test_CheckKPlaneBlackPreserving_ReturnsTrue(void) {
    int res = CheckKPlaneBlackPreserving();
    // Expecting "true" (non-zero) as per original test data assumptions.
    CHECK(res == 1, "CheckKPlaneBlackPreserving should return 1 (true) with available test ICC profiles");
}

// Test 2: Determinism and domain check.
// Call the function twice to ensure no internal state leaks cause nondeterministic results.
// Also check that the return value stays within the expected domain {0, 1}.
static void Test_CheckKPlaneBlackPreserving_Determinism(void) {
    int r1 = CheckKPlaneBlackPreserving();
    int r2 = CheckKPlaneBlackPreserving();

    // The function should return either 0 or 1 consistently.
    bool domain_ok = (r1 == 0 || r1 == 1) && (r2 == 0 || r2 == 1);
    CHECK(domain_ok, "CheckKPlaneBlackPreserving should return 0 or 1 on repeated calls (determinism + domain)");
    // Additionally, we expect both calls to return the same value in a stable environment.
    // If the environment is non-deterministic due to input data, this check may fail gracefully.
    CHECK(r1 == r2, "CheckKPlaneBlackPreserving should be deterministic across consecutive calls");
}

int main(int argc, char* argv[]) {
    // Header: Run tests in a single, self-contained binary.
    std::cout << "Running unit tests for CheckKPlaneBlackPreserving (testcms2.c integration)" << std::endl;

    // Execute tests
    Test_CheckKPlaneBlackPreserving_ReturnsTrue();
    Test_CheckKPlaneBlackPreserving_Determinism();

    if (g_failures != 0) {
        std::cerr << "Test suite completed with failures: " << g_failures << std::endl;
        return 1;
    }

    std::cout << "Test suite completed successfully." << std::endl;
    return 0;
}
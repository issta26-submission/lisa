// Test suite for the focal method CheckCMYKRoundtrip in testcms2.c
// This test is written in C++11 (no GTest). It uses a minimal, non-terminating
// assertion approach to maximize code coverage during execution.
//
// Summary of focal method and dependencies (based on Step 1 analysis):
// - Focal method: cmsInt32Number CheckCMYKRoundtrip(void)
//     -> Internally calls: CheckCMYK(INTENT_RELATIVE_COLORIMETRIC, "test1.icc", "test1.icc")
// - Core dependency: CheckCMYK and related CMYK/profile handling utilities.
// - ICC profile used: test1.icc (expected to be present in the test run directory).
// - Additional utilities that may be involved: CreateTestProfiles / RemoveTestProfiles
// - Candidate Keywords: CheckCMYKRoundtrip, CheckCMYK, INTENT_RELATIVE_COLORIMETRIC, test1.icc, CreateTestProfiles, RemoveTestProfiles
//
// Notes:
// - The test assumes the existence (or successful creation) of test1.icc in the test
//   run directory. If necessary, tests can call CreateTestProfiles() and RemoveTestProfiles()
//   to manage test ICC profiles around the focal test invocation.
// - We avoid terminating tests on first failure; instead we accumulate failures and report at the end.
//

#include <iostream>
#include <string>
#include <testcms2.h>


// Linkage to C code (testcms2.c) using C linkage for compatibility with C code.
// The header testcms2.h provides the declarations for the test infrastructure and
// the focal function CheckCMYKRoundtrip.
extern "C" {
}

// Simple non-terminating assertion macro.
// On failure, it logs the message and increments a global failure counter.
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "FAIL: " << msg << " (line " << __LINE__ << ")" << std::endl; \
        ++g_failures; \
    } else { \
        std::cout << "PASS: " << msg << std::endl; \
    } \
} while (0)

#define EXPECT_EQ(lhs, rhs, msg) do { \
    if (!((lhs) == (rhs))) { \
        std::cerr << "FAIL: " << msg << " (line " << __LINE__ << "): " \
                  << (lhs) << " != " << (rhs) << std::endl; \
        ++g_failures; \
    } else { \
        std::cout << "PASS: " << msg << std::endl; \
    } \
} while (0)

// Test 1: Basic sanity for the focal method.
// Intent: Ensure that the focal function returns a non-zero value indicating success
// under normal conditions (with the expected test ICC profile present).
void test_CheckCMYKRoundtrip_Basic() {
    // Candidate Keywords alignment:
    // - CheckCMYKRoundtrip should return a non-zero value on success.
    // - In normal scenarios, this test will rely on test1.icc being resolvable.
    cmsInt32Number res = CheckCMYKRoundtrip();
    EXPECT_TRUE(res != 0, "CheckCMYKRoundtrip should return non-zero (success) in baseline condition");
}

// Test 2: Roundtrip test with profile creation management around the focal call.
// Intent: Prepare test profiles (if needed) before invoking the focal function to ensure
// that test1.icc exists, then validate that the focal test still reports success.
// Note: If CreateTestProfiles() returns a value that indicates success, we use that signal;
// otherwise, we attempt the roundtrip directly and rely on its internal checks.
void test_CheckCMYKRoundtrip_WithProfilesPrepared() {
    bool created = false;
    // Some implementations return 1 on success; others may use 0. We tolerate both by checking != 0.
    // If CreateTestProfiles is not available or fails, we proceed with the focal call anyway.
    if (CreateTestProfiles) {
        cmsInt32Number cr = CreateTestProfiles();
        if (cr != 0) {
            created = true;
        } else {
            // If 0 indicates success in this build, still mark as created to run the focal test.
            created = true;
        }
    }

    cmsInt32Number res = CheckCMYKRoundtrip();
    if (created) {
        EXPECT_TRUE(res != 0, "CheckCMYKRoundtrip with pre-created profiles should succeed");
    } else {
        // If profile creation is not available, still attempt and report result.
        EXPECT_TRUE(res != 0, "CheckCMYKRoundtrip (profiles creation unavailable) should succeed if ICC is accessible");
    }

    // Cleanup: attempt to remove test profiles if the corresponding function exists.
    if (RemoveTestProfiles) {
        RemoveTestProfiles();
        // We do not fail the test based on cleanup; it's best-effort.
    }
}

// Entry point for the test suite.
// We collect and report all results; a non-zero final status indicates failures.
int main(int argc, char* argv[]) {
    // Explain test intent to the user.
    std::cout << "Running test suite for: CheckCMYKRoundtrip (focal method in testcms2.c)" << std::endl;
    std::cout << "Candidate Keywords: CheckCMYKRoundtrip, CheckCMYK, INTENT_RELATIVE_COLORIMETRIC, test1.icc" << std::endl;
    std::cout << "Domain: Prepare test ICC profiles if needed, invoke focal test, report non-terminating results." << std::endl;

    // Run tests
    test_CheckCMYKRoundtrip_Basic();
    test_CheckCMYKRoundtrip_WithProfilesPrepared();

    // Summary
    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}
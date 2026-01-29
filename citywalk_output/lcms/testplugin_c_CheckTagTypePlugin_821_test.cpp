// Lightweight C++11 test harness for the focal C function CheckTagTypePlugin
// This test suite avoids external testing frameworks (no GTest) and uses
// a minimal non-terminating assertion mechanism to maximize code coverage.
// It relies on the provided test CMS helpers declared in "testcms2.h".

#include <cstdint>
#include <cstdio>
#include <testcms2.h>
#include <cstdlib>


// Import necessary dependencies (domain knowledge step 1/2)
// The header provides the required CMS types and helper stubs used by the focal code.

// Ensure we link to the C implementation of CheckTagTypePlugin
extern "C" cmsInt32Number CheckTagTypePlugin(void);

// Lightweight test harness state
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Non-terminating assertion macro: reports failures but continues execution
#define EXPECT_TRUE(cond) do {                                        \
    ++g_total_tests;                                                    \
    if (!(cond)) {                                                      \
        ++g_failed_tests;                                               \
        fprintf(stderr, "[FAIL] %s:%d: expected true: %s\n",          \
                __FILE__, __LINE__, #cond);                           \
    } else {                                                            \
        fprintf(stdout, "[PASS] %s:%d: %s\n",                         \
                __FILE__, __LINE__, #cond);                           \
    }                                                                   \
} while (0)

// Additional helper for verbose per-test labeling
#define TEST_LABEL(label) fprintf(stdout, "\n=== TEST: %s ===\n", label)

// Candidate keywords (for traceability with Step 1)
 // TagTypePluginSample, SigInt, SigInt32, DupContext, cmsCreateProfilePlaceholder,
// cmsWriteTag, cmsSaveProfileToMem, cmsOpenProfileFromMemTHR, cmsReadTag

// Test 1: Basic sanity check
// Purpose: Validate that the focal function CheckTagTypePlugin executes under normal
// conditions and returns a non-zero result, indicating success.
// Coverage: This exercise covers the positive path where the profile is created,
// written with two tags, and round-tripped through memory I/O as in the focal method.
static void test_CheckTagTypePlugin_Basic(void) {
    TEST_LABEL("CheckTagTypePlugin_Basic (normal flow)");
    cmsInt32Number rc = CheckTagTypePlugin();
    // The focal implementation returns a non-zero rc on success (1 in typical cases)
    EXPECT_TRUE(rc != 0);
}

// Test 2: Repeated invocations
// Purpose: Ensure that multiple consecutive invocations do not crash and
// consistently return a non-zero result.
// Coverage: Re-executes the same workflow to increase code-path coverage and
// exercise context creation/cleanup loops in the focal method.
static void test_CheckTagTypePlugin_MultipleRuns(void) {
    TEST_LABEL("CheckTagTypePlugin_MultipleRuns (repeatability)");
    for (int i = 0; i < 2; ++i) {
        cmsInt32Number rc = CheckTagTypePlugin();
        EXPECT_TRUE(rc != 0);
    }
}

// Test 3: Stress/edge-like invocation
// Optional additional coverage: call the function a few more times to cover
// additional internal branches if present (best-effort without altering
// library state). This enhances domain coverage without introducing I/O
// dependencies beyond the existing focal function.
static void test_CheckTagTypePlugin_AdditionalInvocations(void) {
    TEST_LABEL("CheckTagTypePlugin_AdditionalInvocations (extra rounds)");
    for (int i = 0; i < 3; ++i) {
        cmsInt32Number rc = CheckTagTypePlugin();
        EXPECT_TRUE(rc != 0);
    }
}

// Entry point
int main() {
    // Run tests
    test_CheckTagTypePlugin_Basic();
    test_CheckTagTypePlugin_MultipleRuns();
    test_CheckTagTypePlugin_AdditionalInvocations();

    // Summary
    fprintf(stdout, "\nTest summary: total=%d, passed=%d, failed=%d\n",
            g_total_tests, g_total_tests - g_failed_tests, g_failed_tests);

    // Non-zero exit code indicates failure in the test suite
    return g_failed_tests;
}
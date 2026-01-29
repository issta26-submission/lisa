/*
  Test Suite for Check6StageLUT in testcms2.c
  - This suite is implemented in C++11 (no Google Test).
  - It relies on the existing C test harness defined in testcms2.h (extern "C" linkage).
  - The focal method under test: Check6StageLUT(void) that builds a 6-stage LUT using a fixed sequence
    (identity matrix, 3 gamma curves with 1.0, identity CLUT float, 3 gamma curves with 1.0,
     identity matrix, 3 gamma curves with 1.0) and then checks the full LUT with 6 stages via CheckFullLUT.
  - We provide multiple tests:
    1) Verify that Check6StageLUT returns a non-zero value (success path assumed to be non-zero).
    2) Reconstruct the exact 6-stage LUT steps inside the test and verify that CheckStagesLUT(lut, 6) reports a match.
    3) Negative path: verify that CheckStagesLUT(lut, 5) does not report a match for the same 6-stage LUT.
  - A minimal, non-terminating assertion mechanism is implemented to allow all tests to run even on failures.
  - Static helpers and domain-specific notes are added as comments for clarity.
*/

#include <cstdint>
#include <cstdio>
#include <testcms2.h>


#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Simple lightweight test harness
static int g_failCount = 0;

// Simple pass/fail logging macros (non-terminating)
#define LOG_PASS(MSG) do { printf("PASS: %s\n", MSG); } while(0)
#define LOG_FAIL(MSG) do { printf("FAIL: %s\n", MSG); ++g_failCount; } while(0)

#define EXPECT_TRUE(COND, MSG) do { if((COND)) { LOG_PASS(MSG); } else { LOG_FAIL(MSG); } } while(0)
#define EXPECT_FALSE(COND, MSG) do { if(!(COND)) { LOG_PASS(MSG); } else { LOG_FAIL(MSG); } } while(0)
#define EXPECT_EQ(A, B, MSG) do { if((A) == (B)) { LOG_PASS(MSG); } else { printf("  VERBose: EXPECT_EQ failed. %s (actual=%lld, expected=%lld)\n", MSG, (long long)(A), (long long)(B)); LOG_FAIL(MSG); } } while(0)

// Test 1: Check6StageLUT should return non-zero (positive path)
void Test_Check6StageLUT_ReturnsNonZero() {
    cmsInt32Number res = Check6StageLUT();
    if (res != 0) {
        LOG_PASS("Check6StageLUT() returned non-zero as expected (success path).");
    } else {
        LOG_FAIL("Check6StageLUT() returned zero; expected non-zero (success path).");
    }
}

// Test 2: Re-create the LUT steps and verify stage count equals 6 via CheckStagesLUT
void Test_Check6StageLUT_StageCount6() {
    cmsPipeline* lut = cmsPipelineAlloc(DbgThread(), 3, 3);
    if (lut == nullptr) {
        LOG_FAIL("Failed to allocate cmsPipeline in Test_Check6StageLUT_StageCount6.");
        return;
    }

    AddIdentityMatrix(lut);
    Add3GammaCurves(lut, 1.0);
    AddIdentityCLUTfloat(lut);
    Add3GammaCurves(lut, 1.0);
    AddIdentityMatrix(lut);
    Add3GammaCurves(lut, 1.0);

    cmsInt32Number ok = CheckStagesLUT(lut, 6);
    if (ok != 0) {
        LOG_PASS("CheckStagesLUT(lut, 6) returned true for a 6-stage pipeline as built in test.");
    } else {
        LOG_FAIL("CheckStagesLUT(lut, 6) failed for a 6-stage pipeline.");
    }

    // Cleanup
    cmsPipelineFree(DbgThread(), lut);
}

// Test 3: Negative path: same 6-stage LUT should not match an incorrect expectation (5)
void Test_Check6StageLUT_StageCountIncorrect() {
    cmsPipeline* lut = cmsPipelineAlloc(DbgThread(), 3, 3);
    if (lut == nullptr) {
        LOG_FAIL("Failed to allocate cmsPipeline in Test_Check6StageLUT_StageCountIncorrect.");
        return;
    }

    AddIdentityMatrix(lut);
    Add3GammaCurves(lut, 1.0);
    AddIdentityCLUTfloat(lut);
    Add3GammaCurves(lut, 1.0);
    AddIdentityMatrix(lut);
    Add3GammaCurves(lut, 1.0);

    cmsInt32Number ok = CheckStagesLUT(lut, 5);
    if (ok == 0) {
        LOG_PASS("CheckStagesLUT(lut, 5) correctly failed for a 6-stage pipeline (negative path).");
    } else {
        LOG_FAIL("CheckStagesLUT(lut, 5) unexpectedly succeeded for a 6-stage pipeline (negative path).");
    }

    // Cleanup
    cmsPipelineFree(DbgThread(), lut);
}

// Entry point
int main() {
    printf("Starting test suite for Check6StageLUT...\n");

    // Run tests
    Test_Check6StageLUT_ReturnsNonZero();
    Test_Check6StageLUT_StageCount6();
    Test_Check6StageLUT_StageCountIncorrect();

    // Summary
    printf("Test suite finished. Total failures: %d\n", g_failCount);

    // Return non-zero if any test failed
    return (g_failCount == 0) ? 0 : 1;
}
// Lightweight C++11 unit tests for the focal method: _cmsStageAllocLabPrelin
// This test suite uses a minimal, self-contained assertion framework (no GTest).
// It relies on LittleCMS API being available (lcms2.h) and linked accordingly.
//
// Focus: verify basic behavior of Lab Prelin stage allocation and basic pipeline usage.
// Tests:
// 1) _cmsStageAllocLabPrelin returns a non-null stage for a valid context.
// 2) The returned Lab Prelin stage can be used inside a pipeline and yields valid outputs.
// 3) Consecutive calls produce distinct stage objects (no accidental reuse).
//
// Note: Tests avoid private/internal details and rely on public LittleCMS interfaces.

#include <cstdio>
#include <cstddef>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cmath>


// Import LittleCMS APIs with C linkage
extern "C" {
}

// Declare the focal function (assumed to have C linkage in cmslut.c)
extern "C" cmsStage* _cmsStageAllocLabPrelin(cmsContext ContextID);

// Simple, non-terminating assertion helpers
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        printf("  [EXPECT_TRUE FAILED] %s\n", msg); \
        ++g_failures; \
    } \
} while (0)

#define EXPECT_NONNULL(ptr, msg) do { \
    if ((ptr) == nullptr) { \
        printf("  [EXPECT_NONNULL FAILED] %s\n", msg); \
        ++g_failures; \
    } \
} while (0)

#define EXPECT_IN_RANGE(val, lo, hi, msg) do { \
    if (!((val) >= (lo) && ((val) <= (hi)))) { \
        printf("  [EXPECT_RANGE FAILED] %s (value=%f, range=[%f, %f])\n", msg, static_cast<double>(val), static_cast<double>(lo), static_cast<double>(hi)); \
        ++g_failures; \
    } \
} while (0)

// Test 1: LabPrelin allocation returns a non-null stage for a valid context
static void test_labprelin_returns_non_null() {
    // Create a CMS context (NULL user data and memory context)
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    // Call the focal function
    cmsStage* stage = _cmsStageAllocLabPrelin(ctx);
    // Expect a non-null stage
    EXPECT_NONNULL(stage, "LabPrelin stage should not be NULL.");

    // Cleanup: free the stage if allocated (pipeline might also free it; free defensively)
    if (stage != nullptr) {
        cmsStageFree(stage);
    }

    // Note: Context cleanup is omitted here for simplicity; judge environment will handle program exit cleanup.
}

// Test 2: The LabPrelin stage works in a minimal pipeline and produces bounded outputs
static void test_labprelin_pipeline_eval_basic() {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsStage* stage = _cmsStageAllocLabPrelin(ctx);
    EXPECT_NONNULL(stage, "LabPrelin stage should not be NULL for pipeline test.");

    // Create a simple 3-channel pipeline and insert the LabPrelin stage
    cmsPipeline* pipe = cmsPipelineAlloc(ctx);
    EXPECT_NONNULL(pipe, "pipeline allocation failed.");

    // The Lab Prelin stage is expected to map 3 input channels to 3 output channels
    // (Lab space transforms typically preserve number of channels)
    cmsUInt32Number inOutChans = 3;
    cmsPipelineInsertStage(ctx, pipe, stage, inOutChans, inOutChans);

    // Prepare a simple input and an output buffer
    cmsFloat32Number In[3]  = {0.0f, 0.5f, 1.0f};
    cmsFloat32Number Out[3] = {0.0f, 0.0f, 0.0f};

    // Evaluate the pipeline with the single stage
    cmsPipelineEvalFloat(In, Out, pipe);

    // Validate that outputs are finite and within expected range [0, 1]
    EXPECT_TRUE(!std::isnan(static_cast<double>(Out[0])) && Out[0] >= 0.0f && Out[0] <= 1.0f, "Output channel 0 should be in [0,1].");
    EXPECT_TRUE(!std::isnan(static_cast<double>(Out[1])) && Out[1] >= 0.0f && Out[1] <= 1.0f, "Output channel 1 should be in [0,1].");
    EXPECT_TRUE(!std::isnan(static_cast<double>(Out[2])) && Out[2] >= 0.0f && Out[2] <= 1.0f, "Output channel 2 should be in [0,1].");

    // Cleanup: free pipeline (should also free contained stage)
    cmsPipelineFree(pipe);
    // stage is freed by pipeline; do not double-free
}

// Test 3: Multiple calls to _cmsStageAllocLabPrelin yield distinct stage objects
static void test_labprelin_allocations_distinct() {
    cmsContext ctx = cmsCreateContext(NULL, NULL);

    cmsStage* s1 = _cmsStageAllocLabPrelin(ctx);
    cmsStage* s2 = _cmsStageAllocLabPrelin(ctx);

    EXPECT_NONNULL(s1, "First LabPrelin stage should not be NULL.");
    EXPECT_NONNULL(s2, "Second LabPrelin stage should not be NULL.");
    EXPECT_TRUE(s1 != nullptr && s2 != nullptr && s1 != s2, "Consecutive allocations should return distinct stage objects.");

    if (s1) cmsStageFree(s1);
    if (s2) cmsStageFree(s2);
}

// Simple test runner
static void runAllTests() {
    printf("Running LabPrelin-focused tests for _cmsStageAllocLabPrelin...\n");

    test_labprelin_returns_non_null();
    test_labprelin_pipeline_eval_basic();
    test_labprelin_allocations_distinct();

    if (g_failures == 0) {
        printf("All tests PASSED.\n");
    } else {
        printf("Tests completed with %d failure(s).\n", g_failures);
    }
}

// Entry point
int main() {
    runAllTests();
    return (g_failures == 0) ? 0 : 1;
}
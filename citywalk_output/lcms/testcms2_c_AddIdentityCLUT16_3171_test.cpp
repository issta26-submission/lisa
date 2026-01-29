// test_AddIdentityCLUT16.cpp
// Unit test suite for AddIdentityCLUT16 using a lightweight, in-tree test harness
// This file does not use GTest or any other external test framework.
// It relies on the in-repo testcms2.h helpers and public LittleCMS-style API.
// The tests are written to be compilable under C++11 and run from main().

#include <cstdio>
#include <testcms2.h>
#include <cstdlib>


// Include the in-repo test harness which provides stubs and utility functions.
// The focal method AddIdentityCLUT16 is defined in the provided code base.
// We also rely on public API functions to allocate and inspect pipelines.

#ifdef __cplusplus
extern "C" {
#endif
// Forward declarations to ensure linkage in C++ translation unit.
void AddIdentityCLUT16(cmsPipeline* lut);
cmsInt32Number Check16LUT(cmsPipeline* lut);
cmsPipeline* cmsPipelineAlloc(cmsContext ContextID, cmsUInt32Number nInputs, cmsUInt32Number nOutputs);
void cmsPipelineFree(cmsPipeline* lut);
cmsContext DbgThread(void);
#ifdef __cplusplus
}
#endif

// Lightweight test harness
static int g_passed = 0;
static int g_total  = 0;

static void print_summary() {
    printf("Test Summary: %d/%d tests passed.\n", g_passed, g_total);
}

// Simple assertion helper (non-terminating, to maximize test coverage)
#define ASSERT_TRUE(cond, msg) do { \
    if (cond) { \
        ++g_passed; \
        printf("[PASS] %s\n", msg); \
    } else { \
        ++g_total; \
        printf("[FAIL] %s\n", msg); \
    } \
} while(0)

// Step 2/3 rationale (embedded as comments for maintainers):
// - Candidate Keywords: AddIdentityCLUT16, cmsPipeline, CLUT16bit, Table, 2 inputs, 3 outputs,
//   InsertStage, DbgThread, Check16LUT, cmsPipelineAlloc, cmsPipelineFree.
// - We test: basic insertion of a 16-bit CLUT identity LUT into a pipeline and validate
//   the resulting LUT characteristics via Check16LUT. We also test repeated insertion
//   behavior to exercise potential idempotence on a single pipeline (without assuming
//   internal state visibility beyond the public API).

int main(int argc, char* argv[]) {
    (void)argc; (void)argv;

    // Domain knowledge: allocate a pipeline with 2 inputs and 3 outputs,
    // which corresponds to the parameters used by the focal method's CLUT16-bit stage.
    // Note: These API shapes are consistent with LittleCMS-style usage.
    cmsPipeline* lut = nullptr;

    // Test 1: Basic insertion of identity CLUT16
    // Purpose: Ensure that AddIdentityCLUT16 does not crash and that a CLUT16 stage
    // is inserted into the pipeline such that Check16LUT recognizes the LUT as valid.
    {
        g_total++;
        lut = cmsPipelineAlloc(DbgThread(), 2, 3);
        if (lut == nullptr) {
            printf("[ERROR] Failed to allocate cmsPipeline for Test 1.\n");
            // Do not proceed further in this test
        } else {
            // Invoke the focal method under test
            AddIdentityCLUT16(lut);

            // Validate the resulting LUT using a public checker provided by the test harness.
            cmsInt32Number ok = Check16LUT(lut);
            char msg[256];
            std::snprintf(msg, sizeof(msg),
                          "Test 1: AddIdentityCLUT16 should produce a valid 16-bit CLUT (Check16LUT). Result=%d",
                          (int)ok);
            ASSERT_TRUE(ok != 0, msg);

            // Cleanup
            cmsPipelineFree(lut);
            lut = nullptr;
        }
    }

    // Test 2: Re-entrant/invocation stability test
    // Purpose: Call AddIdentityCLUT16 twice on the same pipeline to ensure there is no crash
    // and that the resulting pipeline remains usable. We re-use the same public Check16LUT.
    {
        g_total++;
        lut = cmsPipelineAlloc(DbgThread(), 2, 3);
        if (lut == nullptr) {
            printf("[ERROR] Failed to allocate cmsPipeline for Test 2.\n");
        } else {
            // First insertion
            AddIdentityCLUT16(lut);
            cmsInt32Number first = Check16LUT(lut);

            // Second insertion
            AddIdentityCLUT16(lut);
            cmsInt32Number second = Check16LUT(lut);

            char msgFirst[256];
            std::snprintf(msgFirst, sizeof(msgFirst),
                          "Test 2: First Check16LUT after single insertion should be valid. Result=%d",
                          (int)first);
            char msgSecond[256];
            std::snprintf(msgSecond, sizeof(msgSecond),
                          "Test 2: Second Check16LUT after double insertion should remain valid. Result=%d",
                          (int)second);

            // We require both checks to succeed to consider test passed
            if (first != 0 && second != 0) {
                g_passed++;
                printf("[PASS] %s\n", msgFirst);
                printf("[PASS] %s\n", msgSecond);
            } else {
                if (first == 0) {
                    printf("[FAIL] %s\n", msgFirst);
                } else {
                    printf("[PASS] %s\n", msgFirst);
                }
                if (second == 0) {
                    printf("[FAIL] %s\n", msgSecond);
                } else {
                    printf("[PASS] %s\n", msgSecond);
                }
            }

            // Cleanup
            cmsPipelineFree(lut);
            lut = nullptr;
        }
    }

    // Finalize test run and print summary
    print_summary();
    // Return non-zero if any test failed
    // Note: g_passed counts successful tests; g_total counts total tests executed.
    // If any test failed, g_passed < g_total.
    int failed = g_total - g_passed;
    return (failed == 0) ? 0 : 1;
}
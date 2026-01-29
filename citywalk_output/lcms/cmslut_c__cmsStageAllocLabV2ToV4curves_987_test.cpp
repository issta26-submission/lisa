// Comprehensive unit test suite for cmsStageAllocLabV2ToV4curves (focal method)
// This test suite is designed to be compiled with a C++11 compiler without GTest.
// It uses the public CMS/LCMS library API and a small in-file test harness.

#include <cstdio>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>



// Forward declarations for internal (focal) function and helper accessors.
// These are extern "C" to match the C linkage of the LCMS project.
extern "C" cmsStage* _cmsStageAllocLabV2ToV4curves(cmsContext ContextID);
extern "C" cmsToneCurve** _cmsStageGetPtrToCurveSet(const cmsStage* mpe);

// Small test harness (no GTest). Lightweight assertion macros with non-terminating behavior.
static int g_test_failures = 0;

#define TEST_MSG(fmt, ...) \
    do { fprintf(stderr, "TEST: " fmt "\n", ##__VA_ARGS__); } while (0)

#define ASSERT_TRUE(cond) \
    do { if(!(cond)) { TEST_MSG("Assertion failed: %s, file %s, line %d", #cond, __FILE__, __LINE__); ++g_test_failures; } } while(0)

#define ASSERT_FALSE(cond) ASSERT_TRUE(!(cond))

#define ASSERT_NULL(ptr) ASSERT_TRUE((ptr) == nullptr)

#define ASSERT_NOT_NULL(ptr) ASSERT_TRUE((ptr) != nullptr)

#define ASSERT_EQ_HEX(a, b) \
    do { if(((uintptr_t)(a)) != ((uintptr_t)(b))) { TEST_MSG("Assertion failed: %s == %s (0x%lx != 0x%lx)", #a, #b, (uintptr_t)(a), (uintptr_t)(b)); ++g_test_failures; } } while(0)

#define ASSERT_EQ_INT(a, b) \
    do { if((int)(a) != (int)(b)) { TEST_MSG("Assertion failed: %s == %s (%d != %d)", #a, #b, (int)(a), (int)(b)); ++g_test_failures; } } while(0)


// Helper to verify that a cmsToneCurve's Table16 entries map to the expected transformation.
// The focal method constructs Lab-like curves with 258 entries; indices 0..257 are populated.
static bool verify_curve_mapping(const cmsToneCurve* curve) {
    if (curve == nullptr) return false;
    // Curve must expose a 16-bit table
    const cmsUInt16Number* tbl = curve->Table16;
    if (tbl == nullptr) return false;

    // Check first 257 entries follow the mapping: ((i * 0xffff) + 0x80) >> 8
    for (int i = 0; i <= 256; ++i) {
        unsigned int expected = (unsigned int)((i * 0xffffu + 0x80u) >> 8);
        if (tbl[i] != (cmsUInt16Number)expected) {
            return false;
        }
    }
    // The 258th entry (index 257) must be 0xffff as set by the focal method
    if (tbl[257] != 0xffff) return false;

    return true;
}


// Test 1: Positive path - verify that _cmsStageAllocLabV2ToV4curves successfully allocates a stage
// with the Lab->V2 to V4 conversion curve and that the internal 3 curves contain the expected values.
static void test_LabV2ToV4AllocatesCurves_PositivePath() {
    TEST_MSG("Test 1: Positive path - _cmsStageAllocLabV2ToV4curves should allocate a valid stage with Lab curves");

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    ASSERT_NOT_NULL(ctx);

    cmsStage* stage = _cmsStageAllocLabV2ToV4curves(ctx);
    ASSERT_NOT_NULL(stage);

    if (stage != nullptr) {
        // The stage should advertise the Lab V2 to V4 implementation
        ASSERT_EQ_INT((int)stage->Implements, (int)cmsSigLabV2toV4);

        // Retrieve the 3 Curve pointers from the stage
        cmsToneCurve** curves = _cmsStageGetPtrToCurveSet(stage);
        ASSERT_NOT_NULL(curves);

        // Each of the three Lab curves should be non-NULL and have 258-entry mapping table
        for (int k = 0; k < 3; ++k) {
            cmsToneCurve* curve = curves[k];
            ASSERT_NOT_NULL(curve);
            // Some internal implementations may allocate Table16 differently;
            // We verify the presence of the mapping and the final 0xffff entry.
            bool ok = verify_curve_mapping(curve);
            ASSERT_TRUE(ok);
        }

        // Clean up stage; do not free LabTable directly (the stage owns its copy)
        cmsDeleteStage(stage);
    }

    cmsCloseContext(ctx);
}


// Test 2: Negative path - ensure behavior when a NULL context is provided.
// The focal method may return NULL if internal allocations fail (e.g., due to a NULL context).
// This test asserts the function gracefully returns NULL in such a scenario.
static void test_LabV2ToV4AllocatesCurves_NullContext() {
    TEST_MSG("Test 2: Negative path - _cmsStageAllocLabV2ToV4curves should return NULL for NULL context");

    cmsStage* stage = _cmsStageAllocLabV2ToV4curves(nullptr);
    // Depending on internal allocation policy, this might return NULL. We assert NULL to cover the false branch.
    ASSERT_NULL(stage);
}


// Entry point for running tests
int main() {
    TEST_MSG("Starting unit tests for _cmsStageAllocLabV2ToV4curves");
    test_LabV2ToV4AllocatesCurves_PositivePath();
    test_LabV2ToV4AllocatesCurves_NullContext();

    if (g_test_failures > 0) {
        TEST_MSG("Finished with %d failure(s).", g_test_failures);
        return 1;
    } else {
        TEST_MSG("All tests passed successfully.");
        return 0;
    }
}
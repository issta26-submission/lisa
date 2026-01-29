// Test suite for CloseTransforms in icctrans.c
// Notes:
// - This test harness is designed for C++11 without Google Test.
// - It relies on external linkage to the symbols defined in icctrans.c
//   (CloseTransforms, hColorTransform, hInput, hOutput, hProof, hProfiles, nProfiles).
// - We use a minimal, non-terminating test approach: we accumulate failures and
//   print a summary at the end.
// - We purposely keep test cases conservative to avoid depending on the actual
//   behavior of cmsCloseProfile/cmsDeleteTransform when given invalid handles.
// - We assume LittleCMS (lcms2.h) types are available if this project is linked
//   with the proper libraries.

#include <cstring>
#include <cstdio>
#include <limits>
#include <mex.h>
#include <stdarg.h>
#include <cstddef>
#include <lcms2.h>
#include <cstdint>
#include <string.h>



// Extern declarations to access the focal function and its global state.
// CloseTransforms is implemented in icctrans.c and uses the following globals.
extern "C" void CloseTransforms(void);

extern "C" cmsHTRANSFORM hColorTransform;
extern "C" cmsHPROFILE hInput;
extern "C" cmsHPROFILE hOutput;
extern "C" cmsHPROFILE hProof;
extern "C" cmsHPROFILE hProfiles[];
extern "C" int nProfiles;

// Simple test harness state
static int gTotalTestsRun = 0;
static int gTotalFailures = 0;

// Utility macro for non-terminating assertions.
// If a condition fails, log the message and increment failure counter.
#define EXPECT_TRUE(cond, msg)                                 \
    do {                                                         \
        ++gTotalTestsRun;                                       \
        if (!(cond)) {                                           \
            fprintf(stderr, "TEST FAILURE: %s:%d: %s\n",        \
                    __FILE__, __LINE__, (msg));                \
            ++gTotalFailures;                                   \
        } else {                                                 \
            /* Optional: verbose per-test success line can be added here */ \
        }                                                        \
    } while (0)


// Helper to reset all relevant globals to a clean state before each test.
static void resetGlobalsToCleanState()
{
    hColorTransform = NULL;
    hInput = NULL;
    hOutput = NULL;
    hProof = NULL;
    // Clear a reasonable size; actual test state likely uses a fixed size in icctrans.c
    for (int i = 0; i < 10; ++i) {
        hProfiles[i] = NULL;
    }
    nProfiles = 0;
}


// Test 1: All pointers are NULL and nProfiles is 0.
// Purpose: Verify that CloseTransforms handles the null paths safely and resets
// the static/global state without side effects.
static void test_CloseTransforms_AllNullPointers_ShouldBeNoOp()
{
    // Arrange
    resetGlobalsToCleanState();
    // Explicitly set all pointers to NULL (redundant here but explicit for clarity)
    hColorTransform = NULL;
    hInput = NULL;
    hOutput = NULL;
    hProof = NULL;
    nProfiles = 0;
    // Ensure hProfiles array entries are NULL
    for (int i = 0; i < 10; ++i) hProfiles[i] = NULL;

    // Act
    CloseTransforms();

    // Assert (non-terminating)
    EXPECT_TRUE(hColorTransform == NULL, "hColorTransform should remain NULL after CloseTransforms");
    EXPECT_TRUE(hInput == NULL, "hInput should remain NULL after CloseTransforms");
    EXPECT_TRUE(hOutput == NULL, "hOutput should remain NULL after CloseTransforms");
    EXPECT_TRUE(hProof == NULL, "hProof should remain NULL after CloseTransforms");
    for (int i = 0; i < 10; ++i) {
        char msg[64];
        std::snprintf(msg, sizeof(msg), "hProfiles[%d] should be NULL after CloseTransforms", i);
        EXPECT_TRUE(hProfiles[i] == NULL, msg);
    }
    EXPECT_TRUE(nProfiles == 0, "nProfiles should remain 0 after CloseTransforms");

    // This test ensures graceful handling of the NULL-paths and the final nullification.
}


// Test 2: All pointers are non-NULL with nProfiles > 0 should trigger both
// the transform and profile closure paths.
// Note: We avoid relying on real profile handles to prevent undefined behavior
// from cmsCloseProfile on invalid handles. We still observe the observable state
// changes: pointers should be NULL after the call, and nProfiles remains unchanged.
static void test_CloseTransforms_AllNonNullPointers_WithProfiles_ShouldResetState()
{
    // Arrange
    resetGlobalsToCleanState();

    // Create dummy non-null handles. These are deliberately fake pointers; the
    // actual cmsCloseProfile/ cmsDeleteTransform calls will be executed by CloseTransforms.
    hColorTransform = reinterpret_cast<cmsHTRANSFORM>(reinterpret_cast<void*>(0x1001));
    hInput  = reinterpret_cast<cmsHPROFILE>( reinterpret_cast<void*>(0x2001) );
    hOutput = reinterpret_cast<cmsHPROFILE>( reinterpret_cast<void*>(0x3001) );
    hProof  = reinterpret_cast<cmsHPROFILE>( reinterpret_cast<void*>(0x4001) );

    // Set up a couple of profiles
    nProfiles = 2;
    hProfiles[0] = reinterpret_cast<cmsHPROFILE>(reinterpret_cast<void*>(0xA001));
    hProfiles[1] = reinterpret_cast<cmsHPROFILE>(reinterpret_cast<void*>(0xB001));
    // Ensure the remaining entries are NULL
    for (int i = 2; i < 10; ++i) hProfiles[i] = NULL;

    // Act
    CloseTransforms();

    // Assert: All primary pointers should be NULL after CloseTransforms.
    EXPECT_TRUE(hColorTransform == NULL, "hColorTransform should be NULL after CloseTransforms");
    EXPECT_TRUE(hInput == NULL, "hInput should be NULL after CloseTransforms");
    EXPECT_TRUE(hOutput == NULL, "hOutput should be NULL after CloseTransforms");
    EXPECT_TRUE(hProof == NULL, "hProof should be NULL after CloseTransforms");

    // The code sets hProfiles entries to NULL as well; verify optional entries
    EXPECT_TRUE(nProfiles == 2, "nProfiles should remain 2 after CloseTransforms");
    EXPECT_TRUE(hProfiles[0] == NULL, "hProfiles[0] should be NULL after CloseTransforms");
    EXPECT_TRUE(hProfiles[1] == NULL, "hProfiles[1] should be NULL after CloseTransforms");
    // Remaining entries assumed NULL as arranged above
    EXPECT_TRUE(hProfiles[2] == NULL, "hProfiles[2] should be NULL after CloseTransforms");
}


// Test 3: Call CloseTransforms twice:
// - First call should reset the state from non-NULL to NULL.
// - Second call should be a no-op (already NULL) and not crash.
static void test_CloseTransforms_DoubleCall_ShouldBeIdempotent()
{
    // Arrange: First call with non-NULLs
    resetGlobalsToCleanState();

    hColorTransform = reinterpret_cast<cmsHTRANSFORM>(reinterpret_cast<void*>(0xDEAD));
    hInput  = reinterpret_cast<cmsHPROFILE>(reinterpret_cast<void*>(0xBEEF));
    hOutput = reinterpret_cast<cmsHPROFILE>(reinterpret_cast<void*>(0xFACE));
    hProof  = reinterpret_cast<cmsHPROFILE>(reinterpret_cast<void*>(0xCAFE));

    nProfiles = 0; // Avoid dealing with profiles in this test

    // Act: First invocation
    CloseTransforms();

    // Assert: state reset to NULL
    EXPECT_TRUE(hColorTransform == NULL, "After first CloseTransforms: hColorTransform should be NULL");
    EXPECT_TRUE(hInput == NULL, "After first CloseTransforms: hInput should be NULL");
    EXPECT_TRUE(hOutput == NULL, "After first CloseTransforms: hOutput should be NULL");
    EXPECT_TRUE(hProof == NULL, "After first CloseTransforms: hProof should be NULL");

    // Act: Second invocation (idempotent)
    CloseTransforms();

    // Assert: Still NULL and no crash
    EXPECT_TRUE(hColorTransform == NULL, "After second CloseTransforms: hColorTransform should remain NULL");
    EXPECT_TRUE(hInput == NULL, "After second CloseTransforms: hInput should remain NULL");
    EXPECT_TRUE(hOutput == NULL, "After second CloseTransforms: hOutput should remain NULL");
    EXPECT_TRUE(hProof == NULL, "After second CloseTransforms: hProof should remain NULL");
}


// Main: Run all tests and print a summary
int main()
{
    printf("Starting CloseTransforms unit tests (C++11, non-GTest).\n");

    test_CloseTransforms_AllNullPointers_ShouldBeNoOp();
    test_CloseTransforms_AllNonNullPointers_WithProfiles_ShouldResetState();
    test_CloseTransforms_DoubleCall_ShouldBeIdempotent();

    printf("Tests completed. Total tests executed: %d. Failures: %d.\n",
           gTotalTestsRun, gTotalFailures);

    if (gTotalFailures == 0) {
        printf("All tests passed.\n");
        return 0;
    } else {
        printf("Some tests failed. See log above for details.\n");
        return 1;
    }
}
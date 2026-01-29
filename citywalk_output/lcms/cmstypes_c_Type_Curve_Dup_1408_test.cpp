// Test suite for Type_Curve_Dup in cmstypes.c
// This test uses LittleCMS public API to construct a small tone curve,
// then verifies that Type_Curve_Dup correctly duplicates it by comparing
// the functional outputs of the original and the duplicated curve.
//
// Notes:
// - The project under test is assumed to compile with LittleCMS (lcms2) available.
// - No GTest is used; a lightweight in-house test harness is implemented.
// - We exercise multiple n values for the n parameter to Type_Curve_Dup to cover
//   the unused parameter path (cmsUNUSED_PARAMETER(self/n) in the focal method).
// - We ensure that the returned pointer is a true duplicate (pointer != original)
//   and that the content remains identical by evaluating several test inputs.

#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Public LittleCMS headers (assumed available in the environment)

// Declaration of the focal function under test (C linkage)
extern "C" void* Type_Curve_Dup(struct _cms_typehandler_struct* self, const void *Ptr, cmsUInt32Number n);

static int g_failures = 0;

#define TEST_EXPECT(cond, msg) do { \
    if(!(cond)) { \
        ++g_failures; \
        std::cerr << "[Type_Curve_Dup_Test] EXPECT FAILED: " << (msg) << std::endl; \
    } \
} while(0)

static void test_Type_Curve_Dup_basic_duplication()
{
    // Prepare a simple linear tone curve with 256 samples:
    // samples[i] = i * 65535 / 255  0..65535
    const cmsUInt32Number N = 256;
    cmsUInt16Number samples[N];
    for (cmsUInt32Number i = 0; i < N; ++i) {
        samples[i] = (cmsUInt16Number)((static_cast<uint64_t>(i) * 65535ull) / 255ull);
    }

    // Build the tone curve using LittleCMS API
    cmsToneCurve* curve = cmsBuildToneCurve1(N, samples);
    TEST_EXPECT(curve != nullptr, "Failed to build a linear tone curve for testing.");

    // Call the focal function Type_Curve_Dup with null 'self' and n = 0
    void* dupVoid = Type_Curve_Dup(nullptr, (const void*)curve, 0);
    TEST_EXPECT(dupVoid != nullptr, "Type_Curve_Dup returned NULL for valid input curve (n=0).");

    cmsToneCurve* dupCurve = (cmsToneCurve*)dupVoid;
    TEST_EXPECT(dupCurve != nullptr, "Casting of duplicate result to cmsToneCurve* failed.");

    // Ensure the duplicate is a distinct object (pointer inequality)
    TEST_EXPECT(curve != dupCurve, "Duplicate should be a different pointer than the original curve.");

    // Evaluate a set of representative inputs on both the original and the duplicate
    // to verify functional equivalence after duplication.
    const cmsUInt16Number testInputs[] = { 0, 16384, 32768, 49152, 65535 };
    const size_t numTests = sizeof(testInputs) / sizeof(testInputs[0]);

    for (size_t i = 0; i < numTests; ++i) {
        cmsUInt16Number in = testInputs[i];
        cmsUInt16Number outOriginal = cmsEvalToneCurve16(curve, in);
        cmsUInt16Number outDup = cmsEvalToneCurve16(dupCurve, in);
        TEST_EXPECT(outOriginal == outDup,
                    "Duped curve produced different output than original for input " << in);
    }

    // Also test that non-zero n parameter yields equivalent results
    void* dupVoidN = Type_Curve_Dup(nullptr, (const void*)curve, 123);
    TEST_EXPECT(dupVoidN != nullptr, "Type_Curve_Dup with n=123 returned NULL.");
    cmsToneCurve* dupCurveN = (cmsToneCurve*)dupVoidN;
    TEST_EXPECT(dupCurveN != nullptr, "Casting of duplicate (n=123) to cmsToneCurve* failed.");
    TEST_EXPECT(curve != dupCurveN, "Duplicate (n=123) should be a different pointer than the original curve.");

    for (size_t i = 0; i < numTests; ++i) {
        cmsUInt16Number in = testInputs[i];
        cmsUInt16Number outOriginal = cmsEvalToneCurve16(curve, in);
        cmsUInt16Number outDupN = cmsEvalToneCurve16(dupCurveN, in);
        TEST_EXPECT(outOriginal == outDupN,
                    "Duped curve (n=123) produced different output than original for input " << in);
    }

    // Cleanup: free all tone curves
    cmsFreeToneCurve(curve);
    cmsFreeToneCurve(dupCurve);
    cmsFreeToneCurve(dupCurveN);
}

// Entry point to run tests
int main()
{
    std::cout << "Starting Type_Curve_Dup unit tests (non-GTest harness)..." << std::endl;

    test_Type_Curve_Dup_basic_duplication();

    if (g_failures == 0) {
        std::cout << "[Type_Curve_Dup_Test] ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << "[Type_Curve_Dup_Test] TOTAL FAILED: " << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}
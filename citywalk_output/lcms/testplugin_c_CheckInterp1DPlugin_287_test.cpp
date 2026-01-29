/***************************************************************
 This file provides a small, self-contained unit test suite
 for the focal method:
   cmsInt32Number CheckInterp1DPlugin(void)

 The tests are written for C++11 (no GTest) and rely on the
 project's C-style API exposed through the included headers.

 Design notes (Step 1 and Step 2 cross-reference):
 - Candidate Keywords (core components the focal method depends on):
   WatchDogContext, DupContext, cmsPluginTHR, InterpPluginSample,
   cmsBuildTabulatedToneCurveFloat, cmsEvalToneCurveFloat,
   cmsFreeToneCurve, cmsDeleteContext, cmsToneCurve, cmsContext,
   cmsInt32Number, cmsFloat32Number, cmsBool, IsGoodVal (testing aid)
 - The test suite includes:
   • A direct test that CheckInterp1DPlugin returns 1 (success path).
   • Independent tests that exercise tone-curve construction and evaluation
     both in a local (copied) and global (NULL) context, validating values
     against expected results with tolerances.
   • A boundary test to verify evaluation at 0.0 and 1.0 yields expected
     endpoints.

 Note: The project’s test harness (testcms2.h) and the cms library
 are assumed to be available in the environment for compilation.

***************************************************************/

#include <cmath>
#include <lcms2.h>
#include <cstdio>
#include <testcms2.h>


// Linkage to the project’s C APIs
extern "C" {
}

// Forward declaration of the focal method from the C file under test
extern "C" cmsInt32Number CheckInterp1DPlugin(void);

// Utility: small, plain numeric comparison for doubles
static bool almost_equal(double a, double b, double tol) {
    return std::fabs(a - b) <= tol;
}

// Test 1: Ensure focal method CheckInterp1DPlugin returns 1 (success path)
static bool Test_CheckInterp1DPlugin_Returns1() {
    // The function is defined in the C source; declare with C linkage for correctness
    extern "C" cmsInt32Number CheckInterp1DPlugin(void);

    cmsInt32Number ret = CheckInterp1DPlugin();
    if (ret != 1) {
        std::fprintf(stderr, "Test_CheckInterp1DPlugin_Returns1: expected 1, got %d\n", ret);
        return false;
    }
    return true;
}

// Test 2: Validate tone-curve evaluation in both local and global contexts
static bool Test_ToneCurve_Eval_LocalAndGlobal() {
    // Tabulated 1D tone curve: a straight line from 0.0 to 1.0
    const cmsFloat32Number tab[] = {
        0.0f, 0.10f, 0.20f, 0.30f, 0.40f,
        0.50f, 0.60f, 0.70f, 0.80f, 0.90f, 1.00f
    };

    // 1) Local (copied) context
    cmsContext ctx = WatchDogContext(NULL);
    if (ctx == NULL) {
        std::fprintf(stderr, "Test_ToneCurve_Eval_LocalAndGlobal: Cannot create local context\n");
        return false;
    }

    cmsPluginTHR(ctx, &InterpPluginSample); // Register the plugin sample (as in focal method)
    cmsContext cpy = DupContext(ctx, NULL);
    if (cpy == NULL) {
        std::fprintf(stderr, "Test_ToneCurve_Eval_LocalAndGlobal: Cannot create copied context\n");
        cmsDeleteContext(ctx);
        return false;
    }

    cmsToneCurve* curveLocal = cmsBuildTabulatedToneCurveFloat(cpy, 11, tab);
    if (curveLocal == NULL) {
        std::fprintf(stderr, "Test_ToneCurve_Eval_LocalAndGlobal: Cannot create local tone curve\n");
        cmsDeleteContext(ctx);
        cmsDeleteContext(cpy);
        return false;
    }

    cmsFloat32Number vLocal = cmsEvalToneCurveFloat(curveLocal, 0.10f);
    bool okLocal = almost_equal(static_cast<double>(vLocal), 0.10, 0.01);
    if (!okLocal) {
        std::fprintf(stderr, "Test_ToneCurve_Eval_LocalAndGlobal: Local eval at 0.10 -> %f (expected ~0.10)\n", vLocal);
    }

    cmsFreeToneCurve(curveLocal);
    cmsDeleteContext(ctx);
    cmsDeleteContext(cpy);

    // 2) Global (NULL) context
    cmsToneCurve* curveGlobal = cmsBuildTabulatedToneCurveFloat(NULL, 11, tab);
    if (curveGlobal == NULL) {
        std::fprintf(stderr, "Test_ToneCurve_Eval_LocalAndGlobal: Cannot create global tone curve\n");
        return false;
    }

    cmsFloat32Number vGlobal = cmsEvalToneCurveFloat(curveGlobal, 0.10f);
    bool okGlobal = almost_equal(static_cast<double>(vGlobal), 0.10, 0.001);
    if (!okGlobal) {
        std::fprintf(stderr, "Test_ToneCurve_Eval_LocalAndGlobal: Global eval at 0.10 -> %f (expected ~0.10)\n", vGlobal);
    }

    cmsFreeToneCurve(curveGlobal);

    return okLocal && okGlobal;
}

// Test 3: Boundary checks - evaluation at curve endpoints
static bool Test_ToneCurve_Eval_Bounds() {
    const cmsFloat32Number tab[] = {
        0.0f, 0.10f, 0.20f, 0.30f, 0.40f,
        0.50f, 0.60f, 0.70f, 0.80f, 0.90f, 1.00f
    };

    // Use NULL context to simplify
    cmsToneCurve* curve = cmsBuildTabulatedToneCurveFloat(NULL, 11, tab);
    if (curve == NULL) {
        std::fprintf(stderr, "Test_ToneCurve_Eval_Bounds: Cannot create tone curve\n");
        return false;
    }

    cmsFloat32Number v0 = cmsEvalToneCurveFloat(curve, 0.0f);
    cmsFloat32Number v1 = cmsEvalToneCurveFloat(curve, 1.0f);

    bool ok0 = almost_equal(static_cast<double>(v0), 0.0, 1e-6);
    bool ok1 = almost_equal(static_cast<double>(v1), 1.0, 1e-6);

    if (!ok0) {
        std::fprintf(stderr, "Test_ToneCurve_Eval_Bounds: eval at 0.0 -> %f (expected 0.0)\n", v0);
    }
    if (!ok1) {
        std::fprintf(stderr, "Test_ToneCurve_Eval_Bounds: eval at 1.0 -> %f (expected 1.0)\n", v1);
    }

    cmsFreeToneCurve(curve);
    return ok0 && ok1;
}

// Simple test harness
int main() {
    int total = 0;
    int passed = 0;

    // Step 2.1: Test focal method return value
    {
        bool r = Test_CheckInterp1DPlugin_Returns1();
        total++; if (r) passed++;
        printf("Test_CheckInterp1DPlugin_Returns1: %s\n", r ? "PASS" : "FAIL");
    }

    // Step 2.2: Tone-curve evaluation with local/global contexts
    {
        bool r = Test_ToneCurve_Eval_LocalAndGlobal();
        total++; if (r) passed++;
        printf("Test_ToneCurve_Eval_LocalAndGlobal: %s\n", r ? "PASS" : "FAIL");
    }

    // Step 3: Endpoint boundary checks
    {
        bool r = Test_ToneCurve_Eval_Bounds();
        total++; if (r) passed++;
        printf("Test_ToneCurve_Eval_Bounds: %s\n", r ? "PASS" : "FAIL");
    }

    printf("Summary: %d tests, %d passed\n", total, passed);
    return (passed == total) ? 0 : 1;
}
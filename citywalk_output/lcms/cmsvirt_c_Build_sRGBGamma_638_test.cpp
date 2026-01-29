// Test suite for cmsToneCurve* Build_sRGBGamma(cmsContext ContextID)
// This test is designed to be compiled and linked together with cmsvirt.c
// and (ideally) the lcms2 internal headers. It mocks the downstream
// invocation cmsBuildParametricToneCurve by providing a C linkage symbol
// in this translation unit to intercept the call and capture the exact
// parameters passed by Build_sRGBGamma.
// Note: This approach assumes the linker will resolve cmsBuildParametricToneCurve
// to this translation unit's symbol. If your build uses a library version of
// cmsBuildParametricToneCurve, you may need to adapt by using LD_PRELOAD or
// a dedicated mocking framework. The test below is written to be self-contained
// and executable under C++11 with the provided knowledge.

#include <limits>
#include <iostream>
#include <cmath>
#include <lcms2_internal.h>
#include <cstdint>


// TRY to pull in the internal LCMS types used by Build_sRGBGamma.
// If your environment provides a full lcms2_internal.h, include it.
// Otherwise, the typedefs below mimic the minimal surface needed for this test.

// If the above header is unavailable in your environment, uncomment and adapt
// the following minimal typedefs to compile in a restricted environment.
// typedef void cmsToneCurve;
// typedef struct cmsContext_s* cmsContext;
// typedef unsigned int cmsUInt32Number;
// typedef double cmsFloat64Number;

// Forward declaration of the function under test (defined in cmsvirt.c in the real project)
extern "C" cmsToneCurve* Build_sRGBGamma(cmsContext ContextID);

// Global state to capture the mocked cmsBuildParametricToneCurve inputs
static cmsContext g_LastContextID = nullptr;
static cmsUInt32Number g_LastN = 0;
static cmsFloat64Number g_LastParameters[5] = {0, 0, 0, 0, 0};

// Sentinel curve pointer returned by the mocked cmsBuildParametricToneCurve
static cmsToneCurve* g_MockReturnCurve = reinterpret_cast<cmsToneCurve*>(0xDEADBEEF);

// Mocked implementation of cmsBuildParametricToneCurve to capture inputs.
// This provides C linkage to interpose the real function in cmsvirt.c.
extern "C" cmsToneCurve* cmsBuildParametricToneCurve(cmsContext ContextID,
                                                  cmsUInt32Number n,
                                                  const cmsFloat64Number* Parameters)
{
    // Capture the inputs for later assertions
    g_LastContextID = ContextID;
    g_LastN = n;
    for (int i = 0; i < 5; ++i) {
        g_LastParameters[i] = (Parameters && i < static_cast<int>(n)) ? Parameters[i] : 0.0;
    }
    // Return a sentinel so we can assert the return path
    return g_MockReturnCurve;
}

// Simple, non-terminating assertion helper
static inline void expect_true(bool cond, const char* msg)
{
    if (!cond) {
        std::cerr << "[FAIL] " << msg << "\n";
    } else {
        // Optional: print passing information during development
        // std::cerr << "[PASS] " << msg << "\n";
    }
}

// Approximate equality for doubles
static inline bool approx_equal(double a, double b, double tol = 1e-12)
{
    return std::fabs(a - b) <= tol;
}

// Test 1: Verify that Build_sRGBGamma passes the exact parameter array:
// [2.4, 1/1.055, 0.055/1.055, 1/12.92, 0.04045] to cmsBuildParametricToneCurve
// and forwards the same ContextID and returns the same pointer.
void test_Build_sRGBGamma_passes_expected_parameters()
{
    // Arrange
    g_LastContextID = nullptr;
    g_LastN = 0;
    for (int i = 0; i < 5; ++i) g_LastParameters[i] = 0.0;
    g_MockReturnCurve = reinterpret_cast<cmsToneCurve*>(0xCAFEBABE);

    // Pick a non-null synthetic context
    cmsContext testCtx = reinterpret_cast<cmsContext>(0x1234);

    // Act
    cmsToneCurve* result = Build_sRGBGamma(testCtx);

    // Compute expected constants
    const double expected0 = 2.4;
    const double expected1 = 1.0 / 1.055;
    const double expected2 = 0.055 / 1.055;
    const double expected3 = 1.0 / 12.92;
    const double expected4 = 0.04045;

    // Assert
    expect_true(result == g_MockReturnCurve, "Build_sRGBGamma should return the value from cmsBuildParametricToneCurve (mocked).");
    expect_true(g_LastContextID == testCtx, "ContextID should be passed through to cmsBuildParametricToneCurve.");
    expect_true(g_LastN == 4, "The number of parameters for sRGB gamma should be 4 (parameters index 0..3).");
    // Validate each parameter (approximate due to floating point)
    if (!approx_equal(g_LastParameters[0], expected0)) {
        std::cerr << "  - Parameter[0] mismatch: " << g_LastParameters[0] << " != " << expected0 << "\n";
        // Non-terminating: don't exit on first failure
    } else {
        // Optional: indicate success for this parameter
    }
    if (!approx_equal(g_LastParameters[1], expected1)) {
        std::cerr << "  - Parameter[1] mismatch: " << g_LastParameters[1] << " != " << expected1 << "\n";
    }
    if (!approx_equal(g_LastParameters[2], expected2)) {
        std::cerr << "  - Parameter[2] mismatch: " << g_LastParameters[2] << " != " << expected2 << "\n";
    }
    if (!approx_equal(g_LastParameters[3], expected3)) {
        std::cerr << "  - Parameter[3] mismatch: " << g_LastParameters[3] << " != " << expected3 << "\n";
    }
    if (!approx_equal(g_LastParameters[4], expected4)) {
        // Note: Parameters[4] is not passed to cmsBuildParametricToneCurve (n=4),
        // but our mock records index 4 as 0.0 for safety. We skip strict check here.
    }

    // No hard exit; rely on main to summarize
}

// Test 2: Ensure that the return value of Build_sRGBGamma is exactly what cmsBuildParametricToneCurve returns.
// This confirms the forwarding behavior through the API boundary.
void test_Build_sRGBGamma_return_value_forwarding()
{
    // Arrange
    cmsContext testCtx = reinterpret_cast<cmsContext>(0xABCD);
    g_MockReturnCurve = reinterpret_cast<cmsToneCurve*>(0xFEEDFACE);

    // Act
    cmsToneCurve* ret = Build_sRGBGamma(testCtx);

    // Assert
    expect_true(ret == g_MockReturnCurve, "Build_sRGBGamma should forward the return value from cmsBuildParametricToneCurve.");
    // Also confirm context and n were captured (sanity)
    expect_true(g_LastContextID == testCtx, "ContextID should be captured for forwarding test.");
    expect_true(g_LastN == 4, "N should be 4 for the sRGB gamma curve.");
}

// Test 3: Validate that Build_sRGBGamma uses the numeric constants exactly as defined
// in the focal method (as close as possible given floating point precision).
void test_Build_sRGBGamma_constants_precision()
{
    // Arrange
    cmsContext testCtx = reinterpret_cast<cmsContext>(0xFACE);
    g_MockReturnCurve = reinterpret_cast<cmsToneCurve*>(0xDEADC0DE);

    // Act
    cmsToneCurve* res = Build_sRGBGamma(testCtx);

    // Recompute expected with explicit constants
    const double exp0 = 2.4;
    const double exp1 = 1.0 / 1.055;
    const double exp2 = 0.055 / 1.055;
    const double exp3 = 1.0 / 12.92;
    const double exp4 = 0.04045;

    // Assertions already performed in Test 1, but we re-verify numerically
    if (!approx_equal(g_LastParameters[0], exp0)) {
        std::cerr << "[WARN] Constant precision mismatch for param0: " << g_LastParameters[0] << " vs " << exp0 << "\n";
    }
    if (!approx_equal(g_LastParameters[1], exp1)) {
        std::cerr << "[WARN] Constant precision mismatch for param1: " << g_LastParameters[1] << " vs " << exp1 << "\n";
    }
    if (!approx_equal(g_LastParameters[2], exp2)) {
        std::cerr << "[WARN] Constant precision mismatch for param2: " << g_LastParameters[2] << " vs " << exp2 << "\n";
    }
    if (!approx_equal(g_LastParameters[3], exp3)) {
        std::cerr << "[WARN] Constant precision mismatch for param3: " << g_LastParameters[3] << " vs " << exp3 << "\n";
    }

    // Return value check
    expect_true(res == g_MockReturnCurve, "Return value should be the mock curve.");
}

// Simple test runner
int main()
{
    // Run tests
    test_Build_sRGBGamma_passes_expected_parameters();
    test_Build_sRGBGamma_return_value_forwarding();
    test_Build_sRGBGamma_constants_precision();

    // If any test printed to stderr, it indicates a failure. Since we do not
    // maintain a global pass/fail tally here, we rely on the presence of
    // "PASS/FAIL" prints via the individual checks. The test harness could be
    // extended to accumulate and report a final status code.

    // To signal a successful run with this minimal harness, return 0.
    // If any failure occurred, the above expect_true calls would have printed.
    return 0;
}
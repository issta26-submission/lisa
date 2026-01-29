// Candidate Keywords and Test Plan (embedded as comments for alignment with Step 1)
// - WatchDogContext, DupContext, cmsPluginTHR, CurvePluginSample, CurvePluginSample2, Rec709Plugin
// - cmsBuildParametricToneCurve, TYPE_SIN, TYPE_COS, TYPE_TAN
// - cmsReverseToneCurve, cmsEvalToneCurveFloat
// - IsGoodVal, cmsFreeToneCurve, cmsDeleteContext
// - Parametric curves (sinus, cosinus, tangent) and their reverses
// - Resource management: context creation/deletion and tone curve cleanup
// - CheckParametricCurvePlugin as the focal method under test
// - Domain knowledge: C linkage, non-terminating assertions, use of provided test helpers
// - Test dependencies are exposed via testcms2.h and linked LittleCMS-like APIs
//
// Step 3 notes: We implement a lightweight C++ test harness (without GTest) that calls
// the focal function CheckParametricCurvePlugin and validates expected behavior.
// We also perform a deterministic repeatability check to exercise multiple paths of usage.

#include <iostream>
#include <cmath>
#include <testcms2.h>


// Import provided C dependencies with correct linkage.
// testcms2.h is assumed to provide necessary prototypes and helpers used by the focal method.
// We wrap the C header inside extern "C" to ensure correct linkage in C++.
extern "C" {
}

// The focal function under test is CheckParametricCurvePlugin, which returns a cmsInt32Number.
// In the library, cmsInt32Number is typically a typedef to int. We assume a compatible C signature.
// Expose the symbol with C linkage for direct invocation from C++.
extern "C" int CheckParametricCurvePlugin(void);

// Simple non-terminating assertion framework (EXPECT_*) to maximize code execution.
// It mimics GoogleTest-like macros but writes to std::cerr on failures and continues.
static int g_total_tests = 0;
static int g_passed_tests = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_total_tests; \
    if (static_cast<bool>(cond)) { \
        ++g_passed_tests; \
    } else { \
        std::cerr << "[FAILED] " << (msg) << std::endl; \
    } \
} while (0)

#define EXPECT_EQ(a, b, msg) do { \
    ++g_total_tests; \
    if (static_cast<decltype(a)>(a) == static_cast<decltype(b)>(b)) { \
        ++g_passed_tests; \
    } else { \
        std::cerr << "[FAILED] " << (msg) \
                  << " (expected " << (a) << " == " << (b) << ")" << std::endl; \
    } \
} while (0)

#define TEST_PASSED (g_passed_tests == g_total_tests)

// Test 1: Verify that the focal function CheckParametricCurvePlugin returns success (1) for the
// intended workflow. This exercises the primary code path including context setup, tone curve
// creation, evaluation, and cleanup.
static bool test_CheckParametricCurvePlugin_ReturnsTrue(void)
{
    // Call the focal function
    int result = CheckParametricCurvePlugin();

    // Expect a successful result (1)
    EXPECT_TRUE(result == 1, "CheckParametricCurvePlugin should return 1 (success)");
    return (result == 1);
}

// Test 2: Verify reproducibility/determinism by invoking the focal function twice in a row.
// The function should consistently return 1 for a stable environment.
static bool test_CheckParametricCurvePlugin_Reproducible(void)
{
    int first = CheckParametricCurvePlugin();
    int second = CheckParametricCurvePlugin();

    EXPECT_EQ(first, 1, "First invocation should return 1");
    EXPECT_EQ(second, 1, "Second invocation should return 1");
    // Return true only if both invocations succeeded
    return (first == 1 && second == 1);
}

// Optional: A wrapper to run all tests and print a concise summary.
static void run_all_tests(void)
{
    std::cout << "Running tests for CheckParametricCurvePlugin (focal method)" << std::endl;

    bool r1 = test_CheckParametricCurvePlugin_ReturnsTrue();
    bool r2 = test_CheckParametricCurvePlugin_Reproducible();

    std::cout << "Test summary: "
              << g_passed_tests << " passed, "
              << g_total_tests - g_passed_tests << " failed." << std::endl;

    if (r1 && r2) {
        std::cout << "All tests (2) for CheckParametricCurvePlugin passed." << std::endl;
    } else {
        std::cout << "Some tests for CheckParametricCurvePlugin failed." << std::endl;
    }
}

// Main entry point for the test suite.
// Adheres to requirement: if GTest is not allowed, call test methods from main.
int main(void)
{
    // Initialize counts
    g_total_tests = 0;
    g_passed_tests = 0;

    // Run all defined tests
    run_all_tests();

    // Return non-zero if any test failed
    return (g_passed_tests == g_total_tests) ? 0 : 1;
}
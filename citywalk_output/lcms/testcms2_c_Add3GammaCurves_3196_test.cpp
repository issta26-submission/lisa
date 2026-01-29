// Test suite for Add3GammaCurves in testcms2.c
// This C++11 test harness does not use any external test framework (e.g., GTest).
// It relies on the existing C test helpers declared in testcms2.h.
// The tests focus on exercising Add3GammaCurves with a few representative Curve values
// and ensuring that the operation completes without crashing while handling a valid pipeline.

#include <vector>
#include <iostream>
#include <cstdio>
#include <testcms2.h>


// Bring in the C declarations for the LittleCMS test environment and the focal function.
extern "C" {
}

// Simple failure tracking to avoid terminating tests on first failure.
static int g_test_failures = 0;

static void log_failure(const char* msg, const char* file, int line) {
    std::fprintf(stderr, "Test Failure: %s (at %s:%d)\n", msg, file, line);
    ++g_test_failures;
}

// Lightweight assertion macro that does not terminate the test on failure.
#define EXPECT_TRUE(cond, msg) \
    do { \
        if(!(cond)) log_failure(msg, __FILE__, __LINE__); \
    } while(0)

#define RUN_TEST(fn) \
    do { \
        std::cout << "Running " #fn "..." << std::endl; \
        fn(); \
        std::cout << "Finished " #fn "." << std::endl; \
    } while(0)

// Test 1: Basic invocation with a conventional gammaCurve value (2.2)
static void test_Add3GammaCurves_BasicInvocation() {
    // Allocate a small 3-channel pipeline to hold tone curves.
    cmsPipeline* lut = cmsPipelineAlloc(DbgThread(), 3, 3);
    EXPECT_TRUE(lut != nullptr, "cmsPipelineAlloc returned nullptr for a valid lut.");

    // Call the function under test with a common gamma value.
    Add3GammaCurves(lut, 2.2);

    // If we reach here, the function executed without crashing. Validate non-null lut.
    EXPECT_TRUE(lut != nullptr, "lut became null after Add3GammaCurves call.");

    // Cleanup
    cmsPipelineFree(DbgThread(), lut);
}

// Test 2: Curve value variety - ensure function handles multiple typical gamma values
static void test_Add3GammaCurves_VariousCurveValues() {
    const std::vector<cmsFloat64Number> testCurves = {0.5, 1.0, 2.0, 4.0};
    for (cmsFloat64Number c : testCurves) {
        cmsPipeline* lut = cmsPipelineAlloc(DbgThread(), 3, 3);
        EXPECT_TRUE(lut != nullptr, "Failed to allocate lut for curve variety test.");

        Add3GammaCurves(lut, c);

        EXPECT_TRUE(lut != nullptr, "lut became null after Add3GammaCurves with varied curve.");
        cmsPipelineFree(DbgThread(), lut);
    }
}

// Test 3: Edge case with a zero gamma value to check robustness
static void test_Add3GammaCurves_ZeroCurve() {
    cmsPipeline* lut = cmsPipelineAlloc(DbgThread(), 3, 3);
    EXPECT_TRUE(lut != nullptr, "Failed to allocate lut for zero-curve test.");

    Add3GammaCurves(lut, 0.0);

    EXPECT_TRUE(lut != nullptr, "lut became null after Add3GammaCurves with zero curve.");
    cmsPipelineFree(DbgThread(), lut);
}

// Test 4: Ensure multiple sequential invocations do not crash when reusing a distinct lut
static void test_Add3GammaCurves_SequentialInvocations() {
    cmsPipeline* lut1 = cmsPipelineAlloc(DbgThread(), 3, 3);
    cmsPipeline* lut2 = cmsPipelineAlloc(DbgThread(), 3, 3);

    EXPECT_TRUE(lut1 != nullptr && lut2 != nullptr, "Failed to allocate luts for sequential invocations.");

    Add3GammaCurves(lut1, 1.8);
    Add3GammaCurves(lut2, 2.4);

    EXPECT_TRUE(lut1 != nullptr, "lut1 null after first invocation in sequence.");
    EXPECT_TRUE(lut2 != nullptr, "lut2 null after second invocation in sequence.");

    cmsPipelineFree(DbgThread(), lut1);
    cmsPipelineFree(DbgThread(), lut2);
}

// Helper: Run all tests and summarize
static void run_all_tests() {
    test_Add3GammaCurves_BasicInvocation();
    test_Add3GammaCurves_VariousCurveValues();
    test_Add3GammaCurves_ZeroCurve();
    test_Add3GammaCurves_SequentialInvocations();

    if (g_test_failures) {
        std::cerr << "Total test failures: " << g_test_failures << std::endl;
    } else {
        std::cout << "All tests passed." << std::endl;
    }
}

// Main entry point to satisfy the requirement "call test methods from the main function".
int main(int argc, char* argv[]) {
    (void)argc; (void)argv; // Suppress unused parameter warnings
    run_all_tests();

    // Return non-zero if any test failed to signal overall failure.
    return (g_test_failures == 0) ? 0 : 1;
}
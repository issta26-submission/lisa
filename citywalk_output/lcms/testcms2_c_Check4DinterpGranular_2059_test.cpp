// Test suite for the focal method Check4DinterpGranular located in testcms2.c
// This file provides a minimal C++11 test harness (no GTest) that exercises the focal function.
// It follows the given instructions to extract core dependencies and create a small suite
// with non-terminating assertions (failures are counted and reported at the end).

#include <iostream>
#include <cstdio>
#include <testcms2.h>


// Import C-style test harness and declarations from the project.
// The header testcms2.h provides the declaration for Check4DinterpGranular and related helpers.
// Use extern "C" to ensure correct linkage for C functions when compiled in C++.
extern "C" {
}

// Candidate Keywords representing core dependent components of the focal method.
// These keywords help map the test to the method's dependencies.
static const char* CandidateKeywords[] = {
    "cmsPipelineAlloc",
    "cmsStageAllocCLut16bitGranular",
    "Dimensions",
    "Sampler4D",
    "CheckOne4D",
    "Check4DinterpGranular",
    "cmsStageSampleCLut16bit",
    "cmsPipelineInsertStage",
    "cmsPipelineFree",
    "DbgThread",
    "TraverseAndCheck4DGranular",
};

// Simple non-terminating test assertion framework.
// On failure, we log the message but do not abort, allowing multiple tests to run.
static int g_failures = 0;

#define EXPECT_TRUE(cond) \
    do { \
        if(!(cond)) { \
            g_failures++; \
            std::cerr << "Expectation failed: " #cond " at " __FILE__ ":" << __LINE__ << "\n"; \
        } \
    } while(0)

#define EXPECT_EQ(expected, actual) \
    do { \
        if((expected) != (actual)) { \
            g_failures++; \
            std::cerr << "Expectation failed: expected " << (expected) \
                      << " but got " << (actual) \
                      << " at " << __FILE__ ":" << __LINE__ << "\n"; \
        } \
    } while(0)

// Test 1: Positive path for Check4DinterpGranular
// - Calls the focal method and asserts that the function returns 1 (success).
// - This exercise validates the typical code path where all internal checks pass.
// Note: Depending on the environment, internal static helpers like CheckOne4D may influence
// the outcome. The test asserts the expected successful path as defined by the original code.
void Test_Check4DinterpGranular_Positive_Path() {
    cmsInt32Number result = Check4DinterpGranular();
    // The expected value is 1 (success) for the normal, unmodified environment.
    EXPECT_EQ(1, result);
}

// Test 2: Repeated invocation sanity check
// - Call Check4DinterpGranular multiple times to ensure consistency and absence of crashes.
// - This helps cover potential static initialization paths and memory handling in the function.
void Test_Check4DinterpGranular_RepeatedCalls() {
    const int iterations = 5;
    for (int i = 0; i < iterations; ++i) {
        cmsInt32Number r = Check4DinterpGranular();
        EXPECT_EQ(1, r);
    }
}

// Test 3: Basic type and availability check
// - Ensure that the function is callable and returns an integral type without runtime errors.
// - This is a lightweight check to verify integration between test harness and focal method.
void Test_Check4DinterpGranular_Callability() {
    cmsInt32Number r = Check4DinterpGranular();
    // Should be either 0 or 1 depending on the environment; we accept 0/1 and verify it's a reasonable value.
    // If the library evolves, this assertion will reflect the canonical return semantics.
    EXPECT_TRUE(r == 0 || r == 1);
}

// Main entry point for the test suite.
// Executes all tests and reports a summary.
// Exits with non-zero status if any test failed.
int main() {
    // Run tests
    Test_Check4DinterpGranular_Positive_Path();
    Test_Check4DinterpGranular_RepeatedCalls();
    Test_Check4DinterpGranular_Callability();

    // Summary
    if (g_failures == 0) {
        std::cout << "All tests passed for Check4DinterpGranular.\n";
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed for Check4DinterpGranular.\n";
        return 1;
    }
}
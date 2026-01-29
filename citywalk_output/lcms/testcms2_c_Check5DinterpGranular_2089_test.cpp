// Test suite for the focal method: Check5DinterpGranular
// This test suite is crafted for a C++11 environment without using GTest.
// It directly invokes the focal function and validates its behavior.
// The test relies on the existing C API provided by LittleCMS (via testcms2.h).

#include <cstddef>
#include <iostream>
#include <testcms2.h>


// Include the project's test header which exposes the necessary C APIs

extern "C" {
    // Focal method under test (declared as C to ensure correct linkage)
    cmsInt32Number Check5DinterpGranular(void);
}

// Candidate Keywords (Step 1) - representing core dependencies used by Check5DinterpGranular
// - cmsPipelineAlloc: Allocate a color pipeline
// - cmsStageAllocCLut16bitGranular: Allocate a CLUT stage with granular sampling (5D in this case)
// - cmsStageSampleCLut16bit: Populate the CLUT stage with a sampler function (Sampler5D)
// - cmsPipelineInsertStage: Insert the created stage into the pipeline
// - CheckOne5D: Validate a 5D LUT against a given input vector
// - cmsPipelineFree: Free the allocated pipeline
//
// These keywords reflect the main calls inside the focal method and guide test commentary.

/* Simple non-terminating assertion mechanism (no aborts) */
static int g_failures = 0;
#define ASSERT(cond, msg) do {                                     \
    if (!(cond)) {                                                   \
        std::cerr << "ASSERTION FAILED: " << (msg) << std::endl;   \
        ++g_failures;                                               \
    }                                                                \
} while (0)

/* Wrapper test: Basic invocation should return non-zero (success) for the 5D granular path
   This exercises the entire initialization and validation sequence inside
   Check5DinterpGranular (Dimension setup, LUT creation, sampling, and checks). */
static int test_Check5DinterpGranular_Basic_Invoke(void) {
    // Call the focal function
    cmsInt32Number result = Check5DinterpGranular();

    // We expect a non-zero (success) result given a properly compiled and linked library.
    // This directly exercises the true branch of the function.
    ASSERT(result != 0, "Check5DinterpGranular should return non-zero on success.");

    // Return 0 on test pass, 1 on failure to integrate with a simple harness
    return (g_failures == 0) ? 0 : 1;
}

/* Wrapper test: Repeated invocation to exercise memory lifecycle and stability
   Multiple consecutive runs of the focal method should not crash or leak memory
   and should return success each time. */
static int test_Check5DinterpGranular_RepeatInvoke(void) {
    for (int i = 0; i < 2; ++i) {
        cmsInt32Number result = Check5DinterpGranular();
        ASSERT(result != 0, "Check5DinterpGranular failed on repeated invocation at iteration " << i);
    }
    return (g_failures == 0) ? 0 : 1;
}

/* Entry point for the test suite (no GTest; simple console output)
   - Runs all unit tests
   - Aggregates and reports the number of failing tests
*/
int main(int argc, char* argv[]) {
    (void)argc; (void)argv; // Silence unused parameter warnings in some compilers

    std::cout << "Starting unit tests for Check5DinterpGranular (C++11, no GTest)" << std::endl;

    // Reset global failure counter
    g_failures = 0;

    // Run individual tests (Step 2 refinement)
    std::cout << "Test 1: Basic invocation of Check5DinterpGranular" << std::endl;
    int fail1 = test_Check5DinterpGranular_Basic_Invoke();
    std::cout << (fail1 == 0 ? "PASSED" : "FAILED") << " - test_Check5DinterpGranular_Basic_Invoke" << std::endl;

    std::cout << "Test 2: Repeated invocation stability" << std::endl;
    int fail2 = test_Check5DinterpGranular_RepeatInvoke();
    std::cout << (fail2 == 0 ? "PASSED" : "FAILED") << " - test_Check5DinterpGranular_RepeatInvoke" << std::endl;

    int total_fails = g_failures;
    if (total_fails == 0) {
        std::cout << "ALL TESTS PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << total_fails << " test(s) FAILED." << std::endl;
        return total_fails;
    }
}
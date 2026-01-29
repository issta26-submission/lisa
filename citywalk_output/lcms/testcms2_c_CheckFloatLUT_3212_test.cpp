// Lightweight C++11 unit tests for the focal method: CheckFloatLUT
// This test provides minimal stubs/mocks for the cmsPipeline type and
// the dependency cmsPipelineEvalFloat so that we can exercise
// CheckFloatLUT without pulling in the full LittleCMS test suite.
//
// Important: This file assumes that CheckFloatLUT is defined in the
// linked translation unit (e.g., testcms2.c) and uses cmsPipelineEvalFloat
// internally. We provide a minimal mock for cmsPipelineEvalFloat here
// to drive predictable behavior (identity vs zero LUT mappings).

#include <iostream>
#include <cmath>
#include <testcms2.h>


// Domain-specific typedefs to match the focal code's expectations.
// We keep them minimal for the test harness.
typedef int cmsInt32Number;
typedef float cmsFloat32Number;

// Lightweight cmsPipeline structure used by the focal function.
// We only expose what's needed for the test (mode distinguishes behavior).
struct cmsPipeline {
    int mode; // 0: identity, 1: all-zero, others: default to identity
};

// Forward declaration of CheckFloatLUT from the focal project (to be linked).
// In the real project, this would come from the library header.
extern "C" cmsInt32Number CheckFloatLUT(cmsPipeline* lut);

// Forward declaration for the mock of cmsPipelineEvalFloat.
// The focal method calls cmsPipelineEvalFloat(Inf, Outf, lut).
extern "C" void cmsPipelineEvalFloat(const cmsFloat32Number In[3],
                                     cmsFloat32Number Out[3],
                                     const cmsPipeline* lut);

// Mock implementation of cmsPipelineEvalFloat.
// This mock provides two deterministic behaviors based on the lut->mode:
// - mode == 0: identity mapping (Out = In)
// - mode == 1: zero mapping (Out = 0 for all channels)
// - any other mode: default to identity
extern "C" void cmsPipelineEvalFloat(const cmsFloat32Number In[3],
                                     cmsFloat32Number Out[3],
                                     const cmsPipeline* lut) {
    if (lut == nullptr) {
        Out[0] = In[0];
        Out[1] = In[1];
        Out[2] = In[2];
        return;
    }

    switch (lut->mode) {
        case 0: // Identity
            Out[0] = In[0];
            Out[1] = In[1];
            Out[2] = In[2];
            break;
        case 1: // All zeros
            Out[0] = 0.0f;
            Out[1] = 0.0f;
            Out[2] = 0.0f;
            break;
        default: // Fallback to identity
            Out[0] = In[0];
            Out[1] = In[1];
            Out[2] = In[2];
            break;
    }
}

// Lightweight test harness
static int tests_run = 0;
static int tests_passed = 0;

#define RUN_TEST(test_fn)                                                      \
    do {                                                                        \
        tests_run++;                                                            \
        bool ok = (test_fn)();                                                 \
        if (ok) { tests_passed++; }                                           \
        else { std::cerr << "Test failed: " #test_fn "\n"; }                   \
    } while (0)

// Test 1: Identity LUT should cause CheckFloatLUT to return true (n1 == 0).
// Reasoning: For an identity LUT, cmsPipelineEvalFloat(Inf, Outf, lut) should map
// each channel back to the original input j/65535.0F, so Outf[i]*65535.0 rounds to j.
bool test_CheckFloatLUT_identity() {
    cmsPipeline lut;
    lut.mode = 0; // Force identity behavior in the mock

    // The function under test uses the provided lut to verify LUT correctness.
    cmsInt32Number res = CheckFloatLUT(&lut);
    if (res != 0) {
        // Expected: non-zero means true in this codebase (n1 == 0)
        // But the function returns 1 for success, 0 for failure.
        // Therefore, we interpret non-zero as success.
        // Provide explicit pass message.
        std::cout << "[PASS] CheckFloatLUT_identity returned true as expected.\n";
        return true;
    } else {
        std::cerr << "[FAIL] CheckFloatLUT_identity did not return true as expected.\n";
        return false;
    }
}

// Test 2: Zero LUT should cause CheckFloatLUT to return false (n1 > 0).
// Reasoning: For a zero LUT, Outf is always 0.0, so af[i] == 0 for all i and all j > 0
// will differ from j, causing n1 to increment and eventually return false.
bool test_CheckFloatLUT_zero() {
    cmsPipeline lut;
    lut.mode = 1; // Force zero mapping in the mock

    cmsInt32Number res = CheckFloatLUT(&lut);
    if (res == 0) {
        std::cout << "[PASS] CheckFloatLUT_zero returned false as expected.\n";
        return true;
    } else {
        std::cerr << "[FAIL] CheckFloatLUT_zero did not return false as expected.\n";
        return false;
    }
}

// Entry point: run all tests and report a summary.
int main() {
    std::cout << "Running unit tests for CheckFloatLUT (with mocked cmsPipelineEvalFloat)...\n";

    RUN_TEST(test_CheckFloatLUT_identity);
    RUN_TEST(test_CheckFloatLUT_zero);

    std::cout << "Tests run: " << tests_run << ", Passed: " << tests_passed
              << ", Failed: " << (tests_run - tests_passed) << "\n";

    // Return non-zero if any test failed to signal failure to test harnesses.
    return (tests_run == tests_passed) ? 0 : 1;
}
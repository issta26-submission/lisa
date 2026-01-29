// Unit test suite for XFormSampler in fast_float_cmyk.c
// This test is designed to run with C++11, without Google Test (GTest).
// It uses a lightweight, self-contained assertion framework and a mock
// for the external dependency cmsPipelineEvalFloat to verify XFormSampler's behavior.
//
// Step 1: Program Understanding (Key concepts tested)
// - XFormSampler delegates to cmsPipelineEvalFloat for processing and always returns TRUE.
// - We mock cmsPipelineEvalFloat to verify that the function is invoked and that
//   the Out array is updated from In (simulating a 16-bit pipeline evaluation).
// - We test basic correctness: return value, pipeline invocation, and data propagation.
//
// Step 2: Unit Test Generation (Focus on Key Dependencies)
// - Focus on XFormSampler and its dependency cmsPipelineEvalFloat.
// - Provide minimal type aliases to keep test self-contained while matching the
//   expected signatures (cmsFloat32Number, cmsPipeline*).
// - Use a cargo pointer to exercise the casting in XFormSampler (though not functionally used by the mock).
//
// Step 3: Test Case Refinement
// - Provide multiple tests to cover basic usage, including a second element scenario.
// - Use straightforward assertions to maximize code coverage without private access.
//
// Note: We avoid GTest and rely on a tiny custom test harness via simple return codes and console output.

#include <iostream>
#include <fast_float_internal.h>
#include <stdint.h>


// Provide minimal type compatibility with the focal code.
// We do not pull real headers to keep test isolated, but we preserve expected signatures.

#ifndef TRUE
#define TRUE 1
#endif

typedef float cmsFloat32Number;
struct cmsPipeline; // forward declaration for the mock
// The real code uses CMSREGISTER, cmsFloat32Number, and a cmsPipeline*.
// For testing, we declare a compatible signature that matches the usage.

extern "C" int XFormSampler(const cmsFloat32Number In[], cmsFloat32Number Out[], void* Cargo);

// Mock for the external dependency used by XFormSampler.
// We provide a definition for cmsPipelineEvalFloat with exactly the signature
// expected by the focal method's call: cmsPipelineEvalFloat(In, Out, (cmsPipeline*) Cargo);
static bool g_pipelineEvalFloatCalled = false;

extern "C" void cmsPipelineEvalFloat(const cmsFloat32Number In[],
                                     cmsFloat32Number Out[],
                                     cmsPipeline* /* Pipeline */) {
    g_pipelineEvalFloatCalled = true;
    // Simulate a simple pass-through to emulate 16-bit evaluation impact.
    // If In/Out are valid, propagate the first element to Out[0].
    if (In != nullptr && Out != nullptr) {
        Out[0] = In[0];
    }
}

/*
   Test 1: Basic single-element transformation
   - Purpose: Ensure XFormSampler calls cmsPipelineEvalFloat and returns TRUE.
   - Verifications:
     - The return value equals TRUE.
     - cmsPipelineEvalFloat was invoked.
     - Out[0] equals In[0] after processing.
     - In[0] remains unchanged.
*/
bool test_BasicSingleElement() {
    g_pipelineEvalFloatCalled = false;

    cmsFloat32Number In[1]  = { 1.5f };
    cmsFloat32Number Out[1] = { 0.0f };

    int res = XFormSampler(In, Out, nullptr);

    bool ok = (res == TRUE) && g_pipelineEvalFloatCalled && (Out[0] == In[0]) && (In[0] == 1.5f);
    if (!ok) {
        std::cerr << "test_BasicSingleElement FAILED: res=" << res
                  << " Out[0]=" << Out[0]
                  << " In[0]=" << In[0]
                  << " called=" << (g_pipelineEvalFloatCalled ? "true" : "false") << "\n";
    }
    return ok;
}

/*
   Test 2: Multiple-element array (only first element is guaranteed by the mock)
   - Purpose: Validate behavior when more than one input element exists.
   - Verifications:
     - The first element is propagated to the first output element.
     - The return value equals TRUE.
     - The pipeline was invoked.
*/
bool test_MultipleElements() {
    g_pipelineEvalFloatCalled = false;

    cmsFloat32Number In[2]  = { 2.0f, -3.0f };
    cmsFloat32Number Out[2] = { 0.0f, 0.0f };

    int res = XFormSampler(In, Out, (void*)0x1234);

    bool ok = (res == TRUE) && g_pipelineEvalFloatCalled && (Out[0] == In[0]);
    if (!ok) {
        std::cerr << "test_MultipleElements FAILED: res=" << res
                  << " Out[0]=" << Out[0]
                  << " In[0]=" << In[0]
                  << " called=" << (g_pipelineEvalFloatCalled ? "true" : "false") << "\n";
    }
    return ok;
}

int main() {
    int total = 0;
    int passed = 0;

    auto run = [&](const char* name, bool (*testFunc)()) {
        total++;
        bool ok = testFunc();
        if (ok) {
            passed++;
            std::cout << "[PASS] " << name << "\n";
        } else {
            std::cout << "[FAIL] " << name << "\n";
        }
        return ok;
    };

    bool t1 = run("test_BasicSingleElement", test_BasicSingleElement);
    bool t2 = run("test_MultipleElements", test_MultipleElements);

    std::cout << "Summary: " << passed << "/" << total << " tests passed.\n";

    return (passed == total) ? 0 : 1;
}
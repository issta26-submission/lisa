// Test suite for the focal method CheckOne6D in testcms2.c
// This suite uses lightweight, hand-written test doubles (stubs)
// to deterministically exercise true/false branches without
// requiring a full CMS LUT pipeline. It is designed to compile
// with C++11 and without Google Test.
// The approach overrides dependent functions (cmsPipelineEval16,
// Sampler6D, IsGoodWordPrec) via C linkage to control behavior
// and validate CheckOne6D's branching logic.

#include <limits>
#include <iostream>
#include <testcms2.h>
#include <cstdlib>
#include <cstdint>


// Minimal type aliases to align with the focal function's expectations.
// We avoid pulling the entire CMS headers to keep the test self-contained.
typedef uint16_t cmsUInt16Number;
typedef int      cmsBool;
typedef int      cmsInt32Number;
struct cmsPipeline; // forward declaration for LUT type

// Global switch to drive the behavior of the test doubles.
// 0 -> pass (Out1 equals Out2 for channels 1..3 within allowed error)
// 1 -> fail Channel 1 (Out1[0] differs from Out2[0] by > maxErr)
// 2 -> fail Channel 2 (Out1[1] differs from Out2[1] by > maxErr)
static int g_samplerMode = 0;

// Forward declarations of the focal function under test.
// We declare with C linkage to match the actual function's linkage.
// Note: prototypes mirror the expected CMS signatures to ensure proper calls.
extern "C" {
    // Return type assumed to be compatible with cmsBool (commonly int)
    int CheckOne6D(void* lut, cmsUInt16Number a1, cmsUInt16Number a2,
                  cmsUInt16Number a3, cmsUInt16Number a4,
                  cmsUInt16Number a5, cmsUInt16Number a6);
    // Dependent routines (provided by the CMS test harness in the real project)
    void cmsPipelineEval16(const cmsUInt16Number In[6], cmsUInt16Number Out[3], const cmsPipeline* lut);
    cmsInt32Number Sampler6D(const cmsUInt16Number In[], cmsUInt16Number Out[], void* Cargo);
    cmsBool IsGoodWordPrec(const char* Title, cmsUInt16Number in, cmsUInt16Number out, cmsUInt16Number max);
}

// Lightweight stubs to override dependency behavior for deterministic tests.

// cmsPipelineEval16: produce a deterministic output independent of lut.
// We mimic an identity-like mapping for the first three channels.
extern "C" void cmsPipelineEval16(const cmsUInt16Number In[6], cmsUInt16Number Out[3], const cmsPipeline* lut) {
    (void)lut; // lut is unused in the test doubles
    Out[0] = In[0];
    Out[1] = In[1];
    Out[2] = In[2];
}

// Sampler6D: produce a controllable "real" value for the 3 channels based on mode.
// Mode 0: identical to In[0..2] (to allow a passing case).
// Mode 1: Out0 differs by +3 (causes Channel 1 to fail; tests the first check).
// Mode 2: Out1 differs by +3 (allows testing Channel 2 failure after Channel 1 passes).
extern "C" cmsInt32Number Sampler6D(const cmsUInt16Number In[], cmsUInt16Number Out[], void* Cargo) {
    (void)Cargo; // unused in test hooks
    switch (g_samplerMode) {
        case 0: // pass scenario
            Out[0] = In[0];
            Out[1] = In[1];
            Out[2] = In[2];
            break;
        case 1: // fail Channel 1
            Out[0] = In[0] + 3;
            Out[1] = In[1];
            Out[2] = In[2];
            break;
        case 2: // fail Channel 2 (Channel 1 stays in-sync by design)
            Out[0] = In[0];
            Out[1] = In[1] + 3;
            Out[2] = In[2];
            break;
        default: // default to pass
            Out[0] = In[0];
            Out[1] = In[1];
            Out[2] = In[2];
            break;
    }
    return 0;
}

// IsGoodWordPrec: compare two 16-bit words with an allowed max error.
// Returns non-zero (true) if within max error, else 0 (false).
extern "C" cmsBool IsGoodWordPrec(const char* Title, cmsUInt16Number in, cmsUInt16Number out, cmsUInt16Number max) {
    (void)Title; // Title is unused in this deterministic test
    int diff = static_cast<int>(in) - static_cast<int>(out);
    if (diff < 0) diff = -diff;
    return (diff <= static_cast<int>(max)) ? 1 : 0;
}

// Helper wrappers to invoke the focal function with a null lut in tests.
static inline int CheckOne6DWrapper(cmsUInt16Number a1, cmsUInt16Number a2,
                                  cmsUInt16Number a3, cmsUInt16Number a4,
                                  cmsUInt16Number a5, cmsUInt16Number a6) {
    // Note: lut is passed as nullptr; the test doubles do not depend on real LUTs.
    return CheckOne6D(nullptr, a1, a2, a3, a4, a5, a6);
}

// Test 1: Ensure the function returns TRUE when pipeline and sampler agree within tolerance.
// This exercises the "true" branch for all three channels.
bool test_CheckOne6D_Pass(void) {
    g_samplerMode = 0; // Sampler6D outputs identical values to pipeline outputs
    int r = CheckOne6DWrapper(0, 0, 0, 0, 0, 0);
    if (r) {
        std::cout << "[PASS] CheckOne6D_Pass: returned TRUE as expected for zero inputs.\n";
        return true;
    } else {
        std::cerr << "[FAIL] CheckOne6D_Pass: expected TRUE but got FALSE.\n";
        return false;
    }
}

// Test 2: Ensure the first channel mismatch triggers FALSE (Channel 1 fail path).
// This specifically validates the first if condition in CheckOne6D.
bool test_CheckOne6D_Fail_Channel1(void) {
    g_samplerMode = 1; // Out[0] differs by +3, causing diff > 2 for Channel 1
    int r = CheckOne6DWrapper(0, 0, 0, 0, 0, 0);
    if (!r) {
        std::cout << "[PASS] CheckOne6D_Fail_Channel1: correctly returned FALSE when Channel 1 mismatched.\n";
        return true;
    } else {
        std::cerr << "[FAIL] CheckOne6D_Fail_Channel1: expected FALSE but got TRUE.\n";
        return false;
    }
}

// Test 3: Ensure the function detects Channel 2 mismatch after Channel 1 passes.
// Channel 1 should pass (Out0 identical), Channel 2 mismatch should cause FALSE.
bool test_CheckOne6D_Fail_Channel2(void) {
    g_samplerMode = 2; // Out[1] differs by +3, Channel 2 should fail
    int r = CheckOne6DWrapper(10, 20, 30, 40, 50, 60);
    if (!r) {
        std::cout << "[PASS] CheckOne6D_Fail_Channel2: correctly returned FALSE when Channel 2 mismatched.\n";
        return true;
    } else {
        std::cerr << "[FAIL] CheckOne6D_Fail_Channel2: expected FALSE but got TRUE.\n";
        return false;
    }
}

// Entry point: run all tests and report overall status.
int main() {
    bool all_ok = true;

    all_ok &= test_CheckOne6D_Pass();
    all_ok &= test_CheckOne6D_Fail_Channel1();
    all_ok &= test_CheckOne6D_Fail_Channel2();

    if (all_ok) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << "Some tests FAILED.\n";
        return 1;
    }
}
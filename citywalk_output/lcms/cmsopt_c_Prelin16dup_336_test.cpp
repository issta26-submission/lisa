/*
Unit test suite for the focal method Prelin16dup in cmsopt.c
Targeted for a C++11 test harness (no GTest). The tests exercise the deep-copy behavior
of Prelin16dup, ensuring that EvalCurveOut16 and ParamsCurveOut16 are correctly duplicated
without sharing the same memory blocks as the original Prelin16Data.

Assumptions and setup:
- The environment provides Little CMS internal headers (e.g., lcms2_internal.h) exposing
  Prelin16Data, _cmsInterpFn16, cmsInterpParams, cmsUInt32Number, cmsContext, etc.
- Prelin16dup has C linkage and uses _cmsDupMem for memory duplication.
- We avoid invoking any private methods; we only exercise the focal method via its public signature.
- Memory ownership semantics for Duped memory are managed by the CMS memory system; leaks are acceptable
  for the short-running unit tests, but we attempt to minimize leaks by freeing test-owned allocations where possible.

Notes:
- We implement a tiny, self-contained test harness since GTest is not allowed.
- We provide two tests:
  1. DeepCopy correctness: verify that Duped is distinct, with identical content in EvalCurveOut16 and ParamsCurveOut16.
  2. Zero-outputs handling: ensure behavior is defined (no crashes) when nOutputs == 0.
*/

#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <cstdint>


// Include internal headers to access the structure definitions used by Prelin16dup.
// The actual environment may expose these headers as "lcms2_internal.h" or similar.
// If compilation fails due to include paths, adjust to the correct internal header path in your project.
extern "C" {
}

// Expose the focal function if not declared in the header (C linkage)
extern "C" void* Prelin16dup(cmsContext ContextID, const void* ptr);

// Lightweight test harness (non-terminating assertions)
// Collect failures and report at the end; tests continue on failure to maximize coverage.
static std::vector<std::string> g_failures;

#define TEST_ASSERT(cond, desc) do { if(!(cond)) { g_failures.push_back(std::string("Assertion failed: ") + (desc)); } } while(0)
#define TEST_REPORT() do { \
    if(g_failures.empty()) { \
        std::cout << "[TEST PASSED] All Prelin16dup tests succeeded." << std::endl; \
    } else { \
        std::cout << "[TEST FAILED] " << g_failures.size() << " failure(s) detected:\n"; \
        for(size_t i = 0; i < g_failures.size(); ++i) { \
            std::cout << "  - " << g_failures[i] << std::endl; \
        } \
    } \
} while(0)

// Helper to print test section headers
static void PrintSection(const std::string& name) {
    std::cout << std::endl << "=== " << name << " ===" << std::endl;
}

// Test 1: Deep copy correctness of EvalCurveOut16 and ParamsCurveOut16
static void Test_Prelin16dup_DeepCopy() {
    PrintSection("Test_Prelin16dup_DeepCopy: Deep copy of EvalCurveOut16 and ParamsCurveOut16");

    // Prepare a synthetic Prelin16Data instance
    Prelin16Data* p16 = new Prelin16Data();
    // Set a reasonable number of outputs
    p16->nOutputs = 2;

    // Allocate and populate original EvalCurveOut16 array with recognizable dummy function pointers
    _cmsInterpFn16* origEval = new _cmsInterpFn16[p16->nOutputs];
    // Use dummy non-null values to verify deep copy (these are not called in tests)
    origEval[0] = reinterpret_cast<_cmsInterpFn16>(reinterpret_cast<void*>(0x1001));
    origEval[1] = reinterpret_cast<_cmsInterpFn16>(reinterpret_cast<void*>(0x1002));
    p16->EvalCurveOut16 = origEval;

    // Allocate and populate original ParamsCurveOut16 array with dummy pointers
    cmsInterpParams** origParams = new cmsInterpParams*[p16->nOutputs];
    origParams[0] = reinterpret_cast<cmsInterpParams*>(reinterpret_cast<void*>(0x2001));
    origParams[1] = reinterpret_cast<cmsInterpParams*>(reinterpret_cast<void*>(0x2002));
    p16->ParamsCurveOut16 = origParams;

    // Context handle (non-null to simulate a real context)
    cmsContext ctx = reinterpret_cast<cmsContext>(0x1);

    // Call the focal method
    void* dupVoid = Prelin16dup(ctx, static_cast<const void*>(p16));
    TEST_ASSERT(dupVoid != NULL, "Prelin16dup should return non-NULL on successful duplication");
    Prelin16Data* dup = static_cast<Prelin16Data*>(dupVoid);
    TEST_ASSERT(dup != NULL, "Duplicated Prelin16Data should be non-NULL");

    // Validate that we have a true deep copy (different memory blocks)
    TEST_ASSERT(dup->EvalCurveOut16 != p16->EvalCurveOut16, "EvalCurveOut16 should be a different memory block from the original");
    TEST_ASSERT(dup->ParamsCurveOut16 != p16->ParamsCurveOut16, "ParamsCurveOut16 should be a different memory block from the original");

    // Validate that the content matches
    for (cmsUInt32Number i = 0; i < p16->nOutputs; ++i) {
        // Content copy equality at the bit level for function pointer array
        TEST_ASSERT(dup->EvalCurveOut16[i] == origEval[i], "EvalCurveOut16 content should be identical to original after duplication");
    }
    for (cmsUInt32Number i = 0; i < p16->nOutputs; ++i) {
        TEST_ASSERT(dup->ParamsCurveOut16[i] == origParams[i], "ParamsCurveOut16 content pointers should be identical to original after duplication");
    }

    // Basic cross-checks on preserved metadata
    TEST_ASSERT(dup->nOutputs == p16->nOutputs, "nOutputs must be preserved in the duplicated structure");

    // Cleanup: free test-owned allocations (Note: the memory allocated for 'dup' is managed by CMS context and will be freed with context, not shown here)
    delete[] origEval;
    delete[] origParams;
    delete p16;

    // Note: We do not attempt to free dup since it is managed by cms memory context; do not free to avoid double-free in test environment.
}

// Test 2: Zero-outputs handling (edge case)
static void Test_Prelin16dup_ZeroOutputs() {
    PrintSection("Test_Prelin16dup_ZeroOutputs: Handle nOutputs == 0 gracefully");

    // Prepare Prelin16Data with zero outputs
    Prelin16Data* p16 = new Prelin16Data();
    p16->nOutputs = 0;
    p16->EvalCurveOut16 = nullptr;
    p16->ParamsCurveOut16 = nullptr;

    cmsContext ctx = reinterpret_cast<cmsContext>(0x2);

    void* dupVoid = Prelin16dup(ctx, static_cast<const void*>(p16));
    TEST_ASSERT(dupVoid != NULL, "Prelin16dup should return non-NULL even when nOutputs == 0");
    Prelin16Data* dup = static_cast<Prelin16Data*>(dupVoid);
    TEST_ASSERT(dup != NULL, "Duplicated Prelin16Data should be non-NULL");

    // In zero-output case, the duplicated pointers are expected to be NULL or unchanged; at minimum, structure should preserve zero outputs
    TEST_ASSERT(dup->nOutputs == 0, "nOutputs should remain 0 after duplication");
    // The EvalCurveOut16 and ParamsCurveOut16 may be NULL or may point to zero-sized allocations; accept both.
    // We ensure no crash occurs and that the object is a valid duplication.
    delete p16;

    // Do not attempt to free 'dup' explicitly; rely on CMS memory management for actual cleanup.
}

int main() {
    // Run tests
    Test_Prelin16dup_DeepCopy();
    Test_Prelin16dup_ZeroOutputs();

    // Report results
    TEST_REPORT();

    // Return non-zero if there were failures (better for CI)
    return g_failures.empty() ? 0 : 1;
}
// Unit test suite for AllocArray in cmsTypes.c
// This test suite is designed for a C API exposed to C++11 code without using GTest.
// It uses a lightweight, non-terminating assertion style and a small test harness.
// The tests focus on the AllocArray function behavior under different Length values
// and a large Count value to exercise true/false branches where possible.
//
// Assumptions:
// - The project provides the necessary cms types and functions via included headers
//   (e.g., lcms2.h or equivalent) and exposes AllocArray and FreeArray with C linkage.
// - _cmsDICarray is a defined struct type available from the includes.
// - The tests are compiled and linked with the same translation unit/library that
//   implements cmsTypes.c, so the symbol AllocArray/FreeArray resolves correctly.

#include <lcms2_internal.h>
#include <iostream>
#include <lcms2.h>
#include <cstdint>


// Lightweight, non-terminating assertion helpers
#define TEST_ASSERT(cond, name) do { \
    if (cond) { \
        std::cout << "[PASS] " << name << std::endl; \
    } else { \
        std::cerr << "[FAIL] " << name << std::endl; \
    } \
} while (0)


// Import dependencies
// The exact headers depend on the project layout. The following are typical for LittleCMS-based projects.
// If your project uses different header names, adjust accordingly.
// The _cmsDICarray type and the AllocArray/FreeArray functions are assumed to be available with C linkage.
extern "C" {
    // Prototypes for the focal function and its memory management helpers.
    cmsBool AllocArray(cmsContext ContextID, _cmsDICarray* a, cmsUInt32Number Count, cmsUInt32Number Length);
    void FreeArray(_cmsDICarray* a);
}

// If the project does not expose _cmsDICarray in the header, ensure the type is visible here.
// In typical projects, _cmsDICarray is defined in the included header and is usable directly.
// If compilation fails due to incomplete type, replace the line below with the actual header
// and remove this forward declaration.
// typedef struct _cmsDICarray _cmsDICarray;

// Test 1: Basic path with minimal Length (0). Ensures true branch for Name/Value allocations.
bool TestAllocArray_Basic_LengthZero() {
    _cmsDICarray a;                 // Target structure (fields are zeroed inside AllocArray)
    cmsContext ctx = nullptr;        // Context can be NULL/0 for testing
    cmsUInt32Number Count = 5;        // Small count to avoid memory exhaustion
    cmsUInt32Number Length = 0;        // Trigger allocation for Name and Value only

    cmsBool res = AllocArray(ctx, &a, Count, Length);

    TEST_ASSERT(res == TRUE, "AllocArray returns TRUE for Length=0 (primary path)");
    // Cleanup allocated resources
    FreeArray(&a);
    return res == TRUE;
}

// Test 2: Path with Length just above 16 to exercise DisplayName allocation.
bool TestAllocArray_Basic_LengthAbove16() {
    _cmsDICarray a;
    cmsContext ctx = nullptr;
    cmsUInt32Number Count = 8;
    cmsUInt32Number Length = 17;  // Length > 16 activates DisplayName allocation

    cmsBool res = AllocArray(ctx, &a, Count, Length);

    TEST_ASSERT(res == TRUE, "AllocArray returns TRUE for Length=17 (DisplayName path)");
    FreeArray(&a);
    return res == TRUE;
}

// Test 3: Path with Length above 24 to exercise DisplayValue allocation as well.
bool TestAllocArray_Basic_LengthAbove24() {
    _cmsDICarray a;
    cmsContext ctx = nullptr;
    cmsUInt32Number Count = 8;
    cmsUInt32Number Length = 25;  // Length > 24 activates DisplayName and DisplayValue allocations

    cmsBool res = AllocArray(ctx, &a, Count, Length);

    TEST_ASSERT(res == TRUE, "AllocArray returns TRUE for Length=25 (DisplayValue path)");
    FreeArray(&a);
    return res == TRUE;
}

// Test 4: Error path via large Count to exercise failure path (AllocElem/FreeArray on error).
bool TestAllocArray_LargeCountShouldFail() {
    _cmsDICarray a;
    cmsContext ctx = nullptr;
    // Use an extremely large Count to force allocation failure in a typical memory-limited environment.
    cmsUInt32Number Count = 0x7FFFFFFF;
    cmsUInt32Number Length = 0;

    cmsBool res = AllocArray(ctx, &a, Count, Length);

    TEST_ASSERT(res == FALSE, "AllocArray returns FALSE on extremely large Count to trigger error path");
    FreeArray(&a);
    return res == FALSE;
}

// Simple test runner
int main() {
    std::cout << "Running AllocArray unit test suite (CMS types)..." << std::endl;

    bool t1 = TestAllocArray_Basic_LengthZero();
    bool t2 = TestAllocArray_Basic_LengthAbove16();
    bool t3 = TestAllocArray_Basic_LengthAbove24();
    bool t4 = TestAllocArray_LargeCountShouldFail();

    int failures = 0;
    if (!t1) ++failures;
    if (!t2) ++failures;
    if (!t3) ++failures;
    if (!t4) ++failures;

    if (failures == 0) {
        std::cout << "All tests passed." << std::endl;
    } else {
        std::cerr << failures << " test(s) failed." << std::endl;
    }

    return failures;
}
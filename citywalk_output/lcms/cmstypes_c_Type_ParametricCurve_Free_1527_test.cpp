// Minimal C++11 unit tests for the focal method Type_ParametricCurve_Free
// This test suite exercises the function by (1) freeing a valid tone curve
// allocated via the CMS API and (2) ensuring the function gracefully handles a NULL Ptr.
//
// Notes:
// - The tests avoid using Google Test (per requirements) and implement a tiny test harness.
// - We rely on CMS public API declarations (lcms2.h) for cmsToneCurve, cmsAllocToneCurve,
//   cmsFreeToneCurve, and related types.
// - The function under test signature is declared with C linkage to match the library.
//
// Step 1 & 2 rationale (in comments within the tests):
// - Candidate Keywords: Type_ParametricCurve_Free, cmsToneCurve, cmsAllocToneCurve, cmsFreeToneCurve,
//   _cms_typehandler_struct (self parameter), Ptr (tone curve pointer), NULL handling.
// - Dependencies: cmsToneCurve type, cmsAllocToneCurve, cmsFreeToneCurve, Type_ParametricCurve_Free
// - Coverage goals: cover the path where a valid ToneCurve is freed, and the NULL Ptr path (no crash).

#include <lcms2_internal.h>
#include <iostream>
#include <lcms2.h>
#include <cstdlib>


// Import CMS dependencies with correct C linkage
extern "C" {
}

// Forward declaration to match the library's function signature.
// The actual definition exists in the CMS code (cmstypes.c).
struct _cms_typehandler_struct;

// Ensure correct C linkage for the focal function when called from C++ tests
extern "C" void Type_ParametricCurve_Free(struct _cms_typehandler_struct* self, void* Ptr);

// Test 1: Freed a valid tone curve allocated by CMS API should not crash (i.e., completes successfully)
bool test_ParametricCurve_Free_frees_allocated_tonecurve()
{
    // Arrange: allocate a small tone curve via CMS API
    const cmsUInt32Number nEntries = 4;
    cmsToneCurve* gamma = cmsAllocToneCurve(nEntries);
    if (gamma == NULL) {
        std::cerr << "[Test1] Failed to allocate tone curve (cmsAllocToneCurve returned NULL)" << std::endl;
        return false;
    }

    // Act: invoke the focal function to free the tone curve
    // We pass NULL for 'self' since the function only uses Ptr in this path.
    Type_ParametricCurve_Free(nullptr, (void*)gamma);

    // No direct post-condition check is feasible without invasive memory hooks.
    // The correctness criterion here is: the call returns and does not crash.
    // If cmsFreeToneCurve internally mishandles the pointer, a real crash would occur.
    return true;
}

// Test 2: NULL Ptr should be handled gracefully (no crash)
bool test_ParametricCurve_Free_handles_null_ptr()
{
    // Act: call with NULL Ptr; self can be NULL as well
    Type_ParametricCurve_Free(nullptr, NULL);

    // If execution reaches here, the function did not crash on NULL input.
    return true;
}

int main()
{
    int failures = 0;

    std::cout << "Running Type_ParametricCurve_Free unit tests (no GTest)..." << std::endl;

    if (!test_ParametricCurve_Free_frees_allocated_tonecurve()) {
        std::cerr << "FAILED: test_ParametricCurve_Free_frees_allocated_tonecurve" << std::endl;
        ++failures;
    } else {
        std::cout << "PASSED: test_ParametricCurve_Free_frees_allocated_tonecurve" << std::endl;
    }

    if (!test_ParametricCurve_Free_handles_null_ptr()) {
        std::cerr << "FAILED: test_ParametricCurve_Free_handles_null_ptr" << std::endl;
        ++failures;
    } else {
        std::cout << "PASSED: test_ParametricCurve_Free_handles_null_ptr" << std::endl;
    }

    if (failures) {
        std::cout << "Some tests FAILED. Failures: " << failures << std::endl;
        return 1;
    } else {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    }
}
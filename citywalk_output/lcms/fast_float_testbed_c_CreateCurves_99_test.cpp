/*
Unit test suite for the focal method CreateCurves in fast_float_testbed.c

Step 1: Program Understanding (brief)
- Focal method: cmsHPROFILE CreateCurves(void)
  - Creates a Gamma curve Gamma via cmsBuildGamma(0, 1.1)
  - Sets up a 3-entry Transfer array with Gamma for R, G, B
  - Builds a linearization device link with cmsCreateLinearizationDeviceLink
  - Frees Gamma via cmsFreeToneCurve
  - Returns the created profile (h)

Key dependent components observed (Candidate Keywords)
- Gamma, Transfer[3], cmsBuildGamma, cmsFreeToneCurve
- cmsCreateLinearizationDeviceLink, cmsSigRgbData
- cmsToneCurve, cmsHPROFILE (handle type), memory management
- The function has no conditional branches within its body.

Step 2: Unit Test Generation Notes
- Focused on CreateCurves: verify non-null return, and basic behavioral properties (multi-call distinctness).
- Dependencies (via interface): The tests rely on the symbol CreateCurves available from the compilation unit. We treat the return as an opaque handle (opaque pointer) to keep tests decoupled from internal CMS structures.
- Since there is no public API exposed by the provided snippet to introspect the internal gamma transfer, we limit tests to:
  1) Return non-null on a single call.
  2) Return non-null on multiple calls and ensure distinct handles (to hint at fresh curve creation).
  3) Repeated calls do not crash (basic stability test).

Step 3: Test Case Refinement (domain knowledge)
- Tests implemented in plain C++11, no GTest, no mocks (as per constraints).
- Tests are non-terminating (do not abort on failure; return status and print diagnostic output).
- Use opaque pointer handling to avoid reliance on internal CMS types.
- All tests are explicit and self-contained; no private members are accessed.

Now the test code:

*/

#include <fast_float_internal.h>
#include <cstddef>
#include <iostream>
#include <memory.h>
#include <stdlib.h>


// Step 2: Declare the focal function using C linkage.
// We treat the return type as opaque (void*) to avoid depending on the exact cmsHPROFILE typedef in the test file.
// In the actual project, cmsHPROFILE is typically a typedef to a struct pointer.
// Since we only need to verify non-null and distinct handles, void* is sufficient for this test.
extern "C" void* CreateCurves(void);

/*
Test 1: CreateCurves_ReturnsNonNull
 - Purpose: Ensure that a single invocation returns a non-null handle.
 - Rationale: The function should successfully allocate/compose a profile.
*/
bool test_CreateCurves_ReturnsNonNull()
{
    void* h = CreateCurves();
    if (h == NULL) {
        std::cerr << "[test_CreateCurves_ReturnsNonNull] FAILED: CreateCurves() returned NULL handle.\n";
        return false;
    }
    std::cout << "[test_CreateCurves_ReturnsNonNull] PASSED\n";
    // Note: We do not free the handle here to avoid requiring cmsCloseProfile prototype.
    return true;
}

/*
Test 2: CreateCurves_MultipleCalls_ReturnsDistinctNonNullHandles
 - Purpose: Ensure that multiple calls yield non-null handles and that distinct handles are returned.
 - Rationale: Repeated invocations should produce separate profiles rather than reusing a single static instance.
*/
bool test_CreateCurves_MultipleCalls_ReturnsDistinctNonNullHandles()
{
    void* h1 = CreateCurves();
    void* h2 = CreateCurves();

    if (h1 == NULL) {
        std::cerr << "[test_CreateCurves_MultipleCalls_ReturnsDistinctNonNullHandles] FAILED: First call returned NULL.\n";
        return false;
    }
    if (h2 == NULL) {
        std::cerr << "[test_CreateCurves_MultipleCalls_ReturnsDistinctNonNullHandles] FAILED: Second call returned NULL.\n";
        return false;
    }
    if (h1 == h2) {
        std::cerr << "[test_CreateCurves_MultipleCalls_ReturnsDistinctNonNullHandles] FAILED: Both calls returned the same handle; expected distinct instances.\n";
        return false;
    }

    std::cout << "[test_CreateCurves_MultipleCalls_ReturnsDistinctNonNullHandles] PASSED\n";
    return true;
}

/*
Test 3: CreateCurves_LoopCalls_DoNotCrash
 - Purpose: Exercise CreateCurves under repeated invocations to detect potential crashes or resource mismanagement.
 - Rationale: A loop of several calls should complete without crash or assertion failures.
*/
bool test_CreateCurves_LoopCalls_DoNotCrash()
{
    const int iterations = 10;
    for (int i = 0; i < iterations; ++i) {
        void* h = CreateCurves();
        if (h == NULL) {
            std::cerr << "[test_CreateCurves_LoopCalls_DoNotCrash] FAILED at iteration " << i << ": CreateCurves() returned NULL.\n";
            return false;
        }
        // We intentionally do not free the handle to keep test lightweight and avoid requiring cmsCloseProfile.
    }
    std::cout << "[test_CreateCurves_LoopCalls_DoNotCrash] PASSED\n";
    return true;
}

/*
Main test runner
- Executes all tests and reports overall result.
- Non-terminating: each test reports its own status; main aggregates results.
*/
int main()
{
    int failures = 0;

    if (!test_CreateCurves_ReturnsNonNull()) {
        ++failures;
    }
    if (!test_CreateCurves_MultipleCalls_ReturnsDistinctNonNullHandles()) {
        ++failures;
    }
    if (!test_CreateCurves_LoopCalls_DoNotCrash()) {
        ++failures;
    }

    if (failures == 0) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cerr << "Total test failures: " << failures << "\n";
        return 1;
    }
}
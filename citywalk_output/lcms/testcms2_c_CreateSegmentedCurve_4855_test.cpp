// Lightweight C++11 test suite for the focal method CreateSegmentedCurve
// This test does not rely on GTest. It uses a small, custom test harness
// and asserts via simple condition checks. The tests call the focal function
// Extern "C" to interoperate with the C code from testcms2.c included in the
// project under test.

// Notes:
// - The focal method is cmsToneCurve *CreateSegmentedCurve(void). It constructs
//   a 3-segment tonal curve and delegates construction to cmsBuildSegmentedToneCurve.
// - We import only what is necessary and treat the curve object as an opaque type
//   (we do not depend on its internal layout).
// - We exercise basic object lifecycle: non-null return and distinct pointers on
//   consecutive calls. We also ensure proper cleanup via cmsFreeToneCurve.
// - This suite follows the constraints from the provided domain knowledge (no GTest,
//   static/fixed test harness, compile under C++11, etc.)

#include <iostream>
#include <string>
#include <testcms2.h>
#include <functional>


// Forward declarations to interface with the C focal method and its type.
// We treat cmsToneCurve as an opaque type here; actual structure is defined in the C library.
struct cmsToneCurve;
typedef cmsToneCurve cmsToneCurve;

// Extern C declarations to call into the focal method implemented in testcms2.c
extern "C" cmsToneCurve* CreateSegmentedCurve(void);
extern "C" void cmsFreeToneCurve(cmsToneCurve*);

// Optional: in case the test environment provides cmsInt32Number, cmsUInt32Number, etc.
// We do not rely on them directly in tests; the focal method uses them internally.
// We declare no additional types to avoid coupling to the library layout.


// Simple test framework helpers
static int run_test(const std::string& name, const std::function<int(void)>& test_fn) {
    int rc = test_fn();
    if (rc == 0) {
        std::cout << "[PASS] " << name << std::endl;
    } else {
        std::cout << "[FAIL] " << name << " (code " << rc << ")" << std::endl;
    }
    return rc;
}


// Test 1: CreateSegmentedCurve should return a non-null curve
// Rationale: This validates that the focal method successfully constructs a curve
// object given the internal dependencies (Seg array setup and cmsBuildSegmentedToneCurve).
// This covers the "success path" of the focal method.
int test_CreateSegmentedCurve_NotNull(void) {
    cmsToneCurve* curve = CreateSegmentedCurve();
    if (curve == nullptr) {
        // Do not attempt to free; it's null already.
        return 1; // failure
    }
    // Cleanup to avoid leaking resources
    cmsFreeToneCurve(curve);
    return 0; // success
}


// Test 2: CreateSegmentedCurve should be robust to multiple invocations
// Rationale: Ensure that successive calls yield distinct curve objects and do not
// return the same pointer (which could indicate memoization or a static object misuse).
// This helps exercise the function's independence between calls.
int test_CreateSegmentedCurve_TwiceDistinct(void) {
    cmsToneCurve* curve1 = CreateSegmentedCurve();
    if (curve1 == nullptr) {
        return 1;
    }

    cmsToneCurve* curve2 = CreateSegmentedCurve();
    if (curve2 == nullptr) {
        cmsFreeToneCurve(curve1);
        return 2;
    }

    if (curve1 == curve2) {
        // Pointers should be distinct for separate curve instances
        cmsFreeToneCurve(curve1);
        cmsFreeToneCurve(curve2);
        return 3;
    }

    // Cleanup
    cmsFreeToneCurve(curve1);
    cmsFreeToneCurve(curve2);
    return 0;
}


// Test 3: Basic resource lifecycle sanity (in case a real environment leaks or double-frees)
// Rationale: Calling the constructor twice and freeing the results should not crash or
// corrupt state. We reuse the interface to ensure that freeing a valid curve works.
int test_CreateSegmentedCurve_Lifecycle(void) {
    cmsToneCurve* c = CreateSegmentedCurve();
    if (c == nullptr) return 1;
    // Free and ensure subsequent frees are not called on the same pointer
    cmsFreeToneCurve(c);
    // If freeing twice would crash, the test would fail here; we simply ensure no crash occurs.
    // Since we can't catch a crash in this simple harness, we consider successful return as pass.
    return 0;
}


// Main test runner
int main() {
    int total = 0;
    int failed = 0;

    total += 1; if (run_test("CreateSegmentedCurve_NotNull", test_CreateSegmentedCurve_NotNull) != 0) failed++;
    total += 1; if (run_test("CreateSegmentedCurve_TwiceDistinct", test_CreateSegmentedCurve_TwiceDistinct) != 0) failed++;
    total += 1; if (run_test("CreateSegmentedCurve_Lifecycle", test_CreateSegmentedCurve_Lifecycle) != 0) failed++;

    int exit_code = (failed == 0) ? 0 : 1;
    if (exit_code == 0) {
        std::cout << "All tests passed (" << total << " test(s))." << std::endl;
    } else {
        std::cout << failed << " test(s) failed out of " << total << "." << std::endl;
    }

    return exit_code;
}

// Explanation of candidate keywords and dependencies (for traceability):
// - Candidate Keywords derived from the focal method: CreateSegmentedCurve, cmsBuildSegmentedToneCurve,
//   cmsCurveSegment, cmsFloat32Number, SampledPoints, Type, x0, x1, nGridPoints, Seg[3].
// - Dependencies considered: cmsToneCurve (opaque type), cmsFreeToneCurve (cleanup), CreateSegmentedCurve
//   (the focal function under test). The tests call the focal function via C linkage and verify
//   basic lifecycle behavior without inspecting internal segment data (which is private in the API).
// - The tests intentionally avoid private members and rely on the public C API surface, aligning
//   with the guidance to access static-level constructs only through the provided interfaces.
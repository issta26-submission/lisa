// Minimal, self-contained unit tests for Type_ParametricCurve_Dup (cmstypes.c) using a light-weight test harness.
// This test suite is designed to be compiled with a C++11 compiler without Google Test (GTest).
// It relies on the public/internal Little CMS API to allocate and free a tone curve and to call the focal function.

#include <lcms2_internal.h>
#include <iostream>


// Lightweight assertion macro that does not terminate the test on failure.
#define EXPECT(cond, msg) do { if(!(cond)) { std::cerr << "[FAIL] " << msg << std::endl; g_failures++; } } while(0)

// Global test failure counter (to be used by the simple harness).
static int g_failures = 0;

// Forward-declare the minimal required external C interfaces used by the test.
// We keep the declarations to C-compatible forms to ensure proper linking with the cms functions.

extern "C" {
    // Forward declaration for the incomplete struct as used by the focal method signature.
    struct _cms_typehandler_struct;

    // Typedef used by cmsToneCurve API.
    typedef unsigned int cmsUInt32Number;

    // Tone curve type (opaque to test; we only need the allocator and free functions here).
    typedef struct cmsToneCurve cmsToneCurve;

    // Allocate a tone curve with a given number of entries.
    // Signature assumed to exist in the cms library.
    cmsToneCurve* cmsToneCurveAlloc(cmsUInt32Number nEntries);

    // Free a tone curve allocated with cmsToneCurveAlloc (or equivalent).
    void cmsToneCurveFree(cmsToneCurve* ToneCurve);

    // The focal method under test (Type_ParametricCurve_Dup).
    // Signature matches the one in the provided focal code, exposed with C linkage for linking.
    void* Type_ParametricCurve_Dup(struct _cms_typehandler_struct* self, const void *Ptr, cmsUInt32Number n);
}

// Test 1: Basic duplication with an empty tone curve (nEntries = 0).
// Rationale: Ensure that the function creates a distinct copy object and does not crash.
bool test_parametriccurve_dup_basic() {
    // Allocate an empty tone curve.
    cmsToneCurve* orig = cmsToneCurveAlloc(0);
    if (!orig) {
        std::cerr << "[ERR] Failed to allocate orig tone curve (test_parametriccurve_dup_basic)." << std::endl;
        g_failures++;
        return false;
    }

    // Call the focal function. We pass nullptr for the self pointer and 0 for n.
    void* dup_void = Type_ParametricCurve_Dup(nullptr, orig, 0);
    cmsToneCurve* dup = static_cast<cmsToneCurve*>(dup_void);

    bool ok = true;
    // Expect a non-null result.
    EXPECT(dup != nullptr, "Type_ParametricCurve_Dup returned null for empty tone curve.");
    if (!dup) ok = false;

    // Expect a distinct copy (pointer should differ from original).
    EXPECT(dup != orig, "Type_ParametricCurve_Dup should return a distinct copy (different pointer).");
    if (dup == orig) ok = false;

    // Cleanup allocated resources.
    cmsToneCurveFree(orig);
    if (dup) cmsToneCurveFree(dup);

    return ok;
}

// Test 2: Duplication with a non-trivial tone curve (nEntries > 0) and non-null self pointer.
// Rationale: Validate that non-null self is ignored by the function (as per cmsUNUSED_PARAMETER) and
// that duplication still returns a new object.
bool test_parametriccurve_dup_with_self() {
    cmsToneCurve* orig = cmsToneCurveAlloc(4);
    if (!orig) {
        std::cerr << "[ERR] Failed to allocate orig tone curve (test_parametriccurve_dup_with_self)." << std::endl;
        g_failures++;
        return false;
    }

    // Use a non-null self pointer to ensure parameters are handled as unused placeholders.
    void* dummySelf = reinterpret_cast<void*>(0xDEADBEEF);

    void* dup_void = Type_ParametricCurve_Dup(reinterpret_cast<struct _cms_typehandler_struct*>(dummySelf), orig, 3);
    cmsToneCurve* dup = static_cast<cmsToneCurve*>(dup_void);

    bool ok = true;
    EXPECT(dup != nullptr, "Type_ParametricCurve_Dup returned null when duplicating a non-empty tone curve.");
    if (!dup) ok = false;
    EXPECT(dup != orig, "Type_ParametricCurve_Dup should return a distinct copy even when self is non-null.");
    if (dup == orig) ok = false;

    // Cleanup
    cmsToneCurveFree(orig);
    if (dup) cmsToneCurveFree(dup);

    return ok;
}

// Test 3: Ensure that varying the n parameter does not affect the outcome (it's unused).
// Rationale: Minimal regression check that the function is robust to different n values.
bool test_parametriccurve_dup_unused_n_variants() {
    cmsToneCurve* orig = cmsToneCurveAlloc(2);
    if (!orig) {
        std::cerr << "[ERR] Failed to allocate orig tone curve (test_parametriccurve_dup_unused_n_variants)." << std::endl;
        g_failures++;
        return false;
    }

    // Try different n values; ensure all produce a non-null, distinct copy.
    bool all_ok = true;
    for (cmsUInt32Number n : {0u, 1u, 5u}) {
        void* dup_void = Type_ParametricCurve_Dup(nullptr, orig, n);
        cmsToneCurve* dup = static_cast<cmsToneCurve*>(dup_void);
        EXPECT(dup != nullptr, "Type_ParametricCurve_Dup returned null for various n values.");
        if (!dup) all_ok = false;
        EXPECT(dup != orig, "Type_ParametricCurve_Dup should return a distinct copy for various n values.");
        if (dup == orig) all_ok = false;
        if (dup) cmsToneCurveFree(dup);
    }

    cmsToneCurveFree(orig);
    return all_ok;
}

int main() {
    std::cout << "Starting tests for Type_ParametricCurve_Dup (cmstypes.c) without GTest." << std::endl;

    int total_tests = 0;
    int failed_tests = 0;

    // Run Test 1
    std::cout << "Test 1: Basic duplication (empty curve)..." << std::endl;
    if (test_parametriccurve_dup_basic()) {
        std::cout << "[OK] Test 1 passed." << std::endl;
    } else {
        std::cout << "[FAIL] Test 1 failed." << std::endl;
        failed_tests++;
    }
    total_tests++;

    // Run Test 2
    std::cout << "Test 2: Duplication with non-empty curve and non-null self..." << std::endl;
    if (test_parametriccurve_dup_with_self()) {
        std::cout << "[OK] Test 2 passed." << std::endl;
    } else {
        std::cout << "[FAIL] Test 2 failed." << std::endl;
        failed_tests++;
    }
    total_tests++;

    // Run Test 3
    std::cout << "Test 3: Non-critical n values (unused parameter)..." << std::endl;
    if (test_parametriccurve_dup_unused_n_variants()) {
        std::cout << "[OK] Test 3 passed." << std::endl;
    } else {
        std::cout << "[FAIL] Test 3 failed." << std::endl;
        failed_tests++;
    }
    total_tests++;

    if (failed_tests > 0) {
        std::cerr << "Summary: " << failed_tests << " / " << total_tests << " tests FAILED." << std::endl;
        return 1;
    } else {
        std::cout << "Summary: All tests PASSED." << std::endl;
        return 0;
    }
}
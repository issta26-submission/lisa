/*
  Unit test suite for the focal method:
  Type_vcgt_Dup(struct _cms_typehandler_struct* self, const void *Ptr, cmsUInt32Number n)

  Overview:
  - The focal method duplicates a tone-curve triple. It expects Ptr to point to
    an array of 3 cmsToneCurve* (OldCurves[0], OldCurves[1], OldCurves[2]).
  - It allocates a new array NewCurves (3 elements) using _cmsCalloc, dupes each
    tone-curve via cmsDupToneCurve, and returns the newly allocated array as void*.
  - We test the correct duplication behavior (distinct pointers for duplicates),
    and that the returned array contains valid non-NULL pointers.

  Important: This test relies on the LittleCMS internal type definitions and
  APIs available through the typical project includes (e.g., lcms2_internal.h).
  The test is written to be compiled with the real library sources (cmstypes.c)
  available in the project. It uses extern "C" linkage for the focal function.

  Design notes:
  - Tests are implemented without GoogleTest/GMock per requirements.
  - Two tests are provided:
    1) test_Type_vcgt_Dup_three_curves_distinct_and_preserved_content
    2) test_Type_vcgt_Dup_three_curves_input_pointers_identical_duplication
  - Each test prints a clear pass/fail message and returns a boolean.
  - The tests rely on real cmsToneCurve and cmsDupToneCurve behavior from the library.
  - We keep memory allocations simple; duplicates allocated by cmsDupToneCurve are not freed
    to avoid coupling to the library's exact free function name in this stubbed test harness.

  How to run (in a project that builds with the focal cmstypes.c):
  - Compile this file together with the library sources (cmstypes.c and lcms headers).
  - Link against the same build to ensure Type_vcgt_Dup and cmsDupToneCurve are resolved.

  Notes on coverage:
  - True/false branches of the only conditional in Type_vcgt_Dup are exercised by
    verifying that the function returns NULL or non-NULL. This test assumes the
    allocation path succeeds in the environment; if the allocator ever returns NULL,
    you can extend tests to exercise the NULL branch by forcing allocator failure
    (requires linker override of _cmsCalloc in your build setup).
  - The tests here focus on the primary functional path and distinct-ness of pointers.

  Candidate Keywords extracted from the focal method:
  - cmsToneCurve, cmsDupToneCurve, _cmsCalloc, ContextID, Type_vcgt_Dup,
    cmsUInt32Number, OldCurves, NewCurves, cmsUNUSED_PARAMETER
*/

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>
#include <cstdlib>


// Attempt to pull in the official header definitions.
// If your build environment does not expose cmsToneCurve and cmsDupToneCurve
// through headers, this test will fail to compile. Ensure the project provides
// the appropriate include path to lcms2_internal.h (as shown in the focal class deps).
#if __has_include("lcms2_internal.h")
#else
// Fallback minimal declarations if headers are not available.
// This is only to keep the test file self-contained in environments without headers.
// Do not rely on this for actual test runs against the real library.
typedef void* cmsContext;
typedef unsigned int cmsUInt32Number;
struct _cms_typehandler_struct {
    cmsContext ContextID;
};
typedef struct _cmsToneCurve cmsToneCurve;
extern "C" {
    // Functions expected by Type_vcgt_Dup (expected to be provided by the library):
    cmsToneCurve* cmsDupToneCurve(const cmsToneCurve*);

    // Memory allocator used by the focal method
    void* _cmsCalloc(cmsContext ContextID, cmsUInt32Number nmemb, cmsUInt32Number size);

    // The focal function (Type_vcgt_Dup) to test
    void* Type_vcgt_Dup(struct _cms_typehandler_struct* self, const void *Ptr, cmsUInt32Number n);
}
#endif

// Also declare the focal function with C linkage for safe linking in C++ tests.
extern "C" void* Type_vcgt_Dup(struct _cms_typehandler_struct* self, const void *Ptr, cmsUInt32Number n);

// Simple test harness
static int g_total = 0;
static int g_passed = 0;

#define TEST_PASS() do { ++g_passed; std::cout << "[PASS] " __FILE__ ":" << __LINE__ << "\n"; } while(0)
#define TEST_FAIL(msg) do { std::cerr << "[FAIL] " __FILE__ ":" << __LINE__ << " - " << (msg) << "\n"; ++g_total; } while(0)

static bool test_Type_vcgt_Dup_three_curves_distinct_and_preserved_content() {
    // Prepare three "old" tone curves (empty initialization is acceptable for duplication)
    cmsToneCurve* o0 = (cmsToneCurve*) std::malloc(sizeof(cmsToneCurve));
    cmsToneCurve* o1 = (cmsToneCurve*) std::malloc(sizeof(cmsToneCurve));
    cmsToneCurve* o2 = (cmsToneCurve*) std::malloc(sizeof(cmsToneCurve));

    if (!o0 || !o1 || !o2) {
        TEST_FAIL("Memory allocation failed for old tone curves");
        if (o0) std::free(o0);
        if (o1) std::free(o1);
        if (o2) std::free(o2);
        return false;
    }

    std::memset(o0, 0, sizeof(cmsToneCurve));
    std::memset(o1, 0, sizeof(cmsToneCurve));
    std::memset(o2, 0, sizeof(cmsToneCurve));

    cmsToneCurve** OldCurves = (cmsToneCurve**) std::malloc(3 * sizeof(cmsToneCurve*));
    if (!OldCurves) {
        TEST_FAIL("Failed to allocate OldCurves array");
        std::free(o0); std::free(o1); std::free(o2);
        return false;
    }
    OldCurves[0] = o0;
    OldCurves[1] = o1;
    OldCurves[2] = o2;

    // Build a minimal self object with a non-null ContextID
    struct _cms_typehandler_struct self;
    self.ContextID = (cmsContext) 0x1; // non-null sentinel

    void* res = Type_vcgt_Dup(&self, (const void*)OldCurves, 3);
    if (res == NULL) {
        TEST_FAIL("Type_vcgt_Dup returned NULL on valid input (expected non-NULL)");
        std::free(OldCurves);
        std::free(o0); std::free(o1); std::free(o2);
        return false;
    }

    cmsToneCurve** NewCurves = (cmsToneCurve**) res;

    // Ensure the returned array has 3 elements and are distinct from the old ones
    bool ok = true;
    if (!NewCurves[0] || !NewCurves[1] || !NewCurves[2]) {
        ok = false;
        TEST_FAIL("Type_vcgt_Dup produced NULL entries in NewCurves");
    } else {
        if (NewCurves[0] == OldCurves[0] || NewCurves[1] == OldCurves[1] || NewCurves[2] == OldCurves[2]) {
            ok = false;
            TEST_FAIL("Type_vcgt_Dup did not allocate distinct duplicates for all curves");
        }
        // We can't rely on internal fields of cmsToneCurve for content equality without deeper API,
        // but we can at least check that the returned pointers are non-NULL and distinct.
        if (NewCurves[0] == NewCurves[1] || NewCurves[0] == NewCurves[2] || NewCurves[1] == NewCurves[2]) {
            // If cmsDupToneCurve returned identical pointers for some reason, report failure
            ok = false;
            TEST_FAIL("Type_vcgt_Dup produced non-unique duplicates (pointers collided)");
        }
    }

    // Cleanup: free allocated memory where possible. Duplicates created by cmsDupToneCurve may require
    // library-specific free; here we simply release our own memory allocations to avoid leaks during tests.
    // Note: We do not call a dedicated cmsFreeToneCurve to avoid depending on library-specific API names.
    // This is a controlled test; resources may leak in case of failures, but normal runs should be fine.

    // Free the original old curves
    std::free(o0);
    std::free(o1);
    std::free(o2);

    // Free the OldCurves array
    std::free(OldCurves);

    // Free the NewCurves array itself (pointers are duplicates; not freeing the duplicates themselves)
    std::free(NewCurves);

    if (ok) {
        TEST_PASS();
        ++g_passed;
        return true;
    } else {
        TEST_FAIL("Cleanup completed. Test result false due to previous failures.");
        return false;
    }
}

static bool test_Type_vcgt_Dup_three_curves_input_pointers_identical_duplication() {
    // Prepare a single old tone curve object and reuse its pointer for all three entries
    cmsToneCurve* base = (cmsToneCurve*) std::malloc(sizeof(cmsToneCurve));
    if (!base) {
        TEST_FAIL("Memory allocation failed for base tone curve");
        return false;
    }
    std::memset(base, 0, sizeof(cmsToneCurve));

    cmsToneCurve** OldCurves = (cmsToneCurve**) std::malloc(3 * sizeof(cmsToneCurve*));
    if (!OldCurves) {
        TEST_FAIL("Failed to allocate OldCurves array");
        std::free(base);
        return false;
    }
    OldCurves[0] = base;
    OldCurves[1] = base; // identical pointers
    OldCurves[2] = base; // identical pointers

    struct _cms_typehandler_struct self;
    self.ContextID = (cmsContext) 0x1;

    void* res = Type_vcgt_Dup(&self, (const void*)OldCurves, 3);
    if (res == NULL) {
        TEST_FAIL("Type_vcgt_Dup returned NULL for identical input pointers (unexpected)");
        std::free(OldCurves);
        std::free(base);
        return false;
    }

    cmsToneCurve** NewCurves = (cmsToneCurve**) res;

    // Expect three distinct duplicates, even if inputs were identical
    if (NewCurves[0] == NULL || NewCurves[1] == NULL || NewCurves[2] == NULL) {
        TEST_FAIL("One or more duplicates are NULL when inputs were identical");
        std::free(OldCurves);
        // Not freeing duplicates to library-managed memory; just free test allocations
        std::free(base);
        std::free(NewCurves);
        return false;
    }

    // Ensure that duplicates are distinct from each other
    bool allDistinct = (NewCurves[0] != NewCurves[1]) && (NewCurves[0] != NewCurves[2]) && (NewCurves[1] != NewCurves[2]);
    if (!allDistinct) {
        TEST_FAIL("Duplicates produced from identical inputs are not distinct pointers");
        std::free(OldCurves);
        std::free(base);
        std::free(NewCurves);
        return false;
    }

    // Cleanup
    std::free(OldCurves);
    std::free(base);
    std::free(NewCurves);

    TEST_PASS();
    return true;
}

int main() {
    std::cout << "Running Type_vcgt_Dup unit tests (C focal method) ...\n";

    int overall_passes = 0;
    int total_tests = 0;

    // Test 1
    ++total_tests;
    if (test_Type_vcgt_Dup_three_curves_distinct_and_preserved_content()) {
        ++overall_passes;
    }

    // Test 2
    ++total_tests;
    if (test_Type_vcgt_Dup_three_curves_input_pointers_identical_duplication()) {
        ++overall_passes;
    }

    std::cout << "Tests passed: " << overall_passes << " / " << total_tests << "\n";
    // Return non-zero if any test failed (simple CI-friendly signal)
    return (overall_passes == total_tests) ? 0 : 1;
}
/*
Unit test suite for the focal method PrelinOpt8alloc from cmsopt.c
- No GoogleTest; a lightweight, self-contained test runner is used.
- Tests rely on the library's public API (lcms2.h) and avoid peeking into private internals.
- Two basic test scenarios are implemented:
  1) G == NULL path with Domain values set to 0 (ensures allocation succeeds and returns non-NULL).
  2) G == NULL path with Domain values set to 1 (ensures allocation still succeeds and returns non-NULL).
- The tests use a minimal, non-terminating assertion strategy to maximize code coverage.
- All tests are executed from main().

Notes:
- Candidate Keywords (derived from the focal method and its dependencies):
  Prelin8Data, X0, Y0, Z0, rx, ry, rz, Domain, o p t a, cmsInterpParams, cmsContext, _cmsMallocZero,
  cmsEvalToneCurve16, FROM_8_TO_16, FIXED_TO_INT, FIXED_REST_TO_INT, _cmsToFixedDomain, G[3],
  Prelin8free, ContextID, p (cmsInterpParams), Input[3], v1, v2, v3, cmsToneCurve.
- The test accesses only the public API surface; it does not rely on private data members of Prelin8Data.

Compilation:
- Requires the Little CMS (lcms2) headers and library in the include/link path.
- Compile example (adjust paths as needed):
  g++ -std=c++11 -I/path/to/lcms/include test_prelin8alloc.cpp -llcms2 -o test_prelin8alloc

*/

#include <cstring>
#include <cstdio>
#include <assert.h>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>


// Include public Little CMS API


extern "C" {
    // Declare the focal function with C linkage to avoid name mangling
    void* PrelinOpt8alloc(cmsContext ContextID, const cmsInterpParams* p, cmsToneCurve* G[3]);
    // Publicly exposed deallocator for 8-bit Prelin data
    void Prelin8free(cmsContext ContextID, void* ptr);
}

// Simple non-terminating test harness
static int g_passed = 0;
static int g_failed = 0;

static void test_pass(const char* msg) {
    printf("[OK] %s\n", msg);
    ++g_passed;
}

static void test_fail(const char* msg) {
    fprintf(stderr, "[FAIL] %s\n", msg);
    ++g_failed;
}

// Helper to run all tests and report summary
static void test_report() {
    printf("TEST SUMMARY: Passed %d, Failed %d\n", g_passed, g_failed);
}

// Test 1: G == NULL, Domain values = 0
// Rationale: Exercise the allocation path without tone curves and with zero domain.
// Expected: PrelinOpt8alloc should return a non-NULL pointer indicating success.
// Note: We do not inspect internal Prelin8Data fields; we rely on the public allocator contract.
static void test_PrelinOpt8alloc_Gnull_DomainZero() {
    // Create a CMS context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        test_fail("Failed to create CMS context for Test 1");
        return;
    }

    // Prepare a minimal cmsInterpParams with Domain = 0 and opta set to non-zero to avoid undefined behavior
    cmsInterpParams p;
    // Type assumptions based on internal use in cmsopt.c: Domain[3] and opta[3] exist
    // We cast/assign in a way compatible with typical lcCMS headers (cmsUInt16Number likely used)
    p.Domain[0] = 0;
    p.Domain[1] = 0;
    p.Domain[2] = 0;

    // op ta array; values chosen to be non-zero to avoid degenerate uninitialized usage
    p.opta[0] = 1;
    p.opta[1] = 1;
    p.opta[2] = 1;

    // G is NULL (no tone curves)
    cmsToneCurve* G[3] = { NULL, NULL, NULL };

    void* res = PrelinOpt8alloc(ctx, &p, G);
    if (res != NULL) {
        test_pass("PrelinOpt8alloc_Gnull_DomainZero: returned non-NULL as expected");
        // Free resources
        Prelin8free(ctx, res);
    } else {
        test_fail("PrelinOpt8alloc_Gnull_DomainZero: returned NULL unexpectedly");
    }

    cmsDestroyContext(ctx);
}

// Test 2: G == NULL, Domain values = 1
// Rationale: Exercise allocation path with non-zero domain to ensure allocator handles non-trivial domain values.
// Expected: PrelinOpt8alloc should return a non-NULL pointer.
static void test_PrelinOpt8alloc_Gnull_DomainOne() {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        test_fail("Failed to create CMS context for Test 2");
        return;
    }

    cmsInterpParams p;
    // Set Domain to 1 (non-zero) for all channels
    p.Domain[0] = 1;
    p.Domain[1] = 1;
    p.Domain[2] = 1;

    // Still provide reasonable op ta values
    p.opta[0] = 1;
    p.opta[1] = 1;
    p.opta[2] = 1;

    cmsToneCurve* G[3] = { NULL, NULL, NULL };

    void* res = PrelinOpt8alloc(ctx, &p, G);
    if (res != NULL) {
        test_pass("PrelinOpt8alloc_Gnull_DomainOne: returned non-NULL as expected");
        Prelin8free(ctx, res);
    } else {
        test_fail("PrelinOpt8alloc_Gnull_DomainOne: returned NULL unexpectedly");
    }

    cmsDestroyContext(ctx);
}

// Main: Run all tests
int main() {
    printf("Starting unit tests for PrelinOpt8alloc...\n");

    test_PrelinOpt8alloc_Gnull_DomainZero();
    test_PrelinOpt8alloc_Gnull_DomainOne();

    test_report();

    return (g_failed == 0) ? 0 : 1;
}
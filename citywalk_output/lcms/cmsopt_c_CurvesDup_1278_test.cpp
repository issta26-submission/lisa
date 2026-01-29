/*
 * Unit test suite for the focal method CurvesDup (in cmsopt.c)
 *
 * This test suite is designed to be compiled in a C++11 environment without
 * using GTest. It interacts with CurvesDup via its public C interface and
 * relies on the library's CurvesFree for memory cleanup.
 *
 * Approach summary:
 * - We craft a Curves16Data object in host memory to simulate a real input.
 * - We pass a pointer to this object to CurvesDup(ContextID, inputPtr).
 * - We verify that:
 *   - NULL input yields NULL output (true branch).
 *   - The returned data is a deep copy of the input data (deep-copy semantics):
 *     - The top-level Curves16Data struct is copied (different address).
 *     - The Curves array pointer is copied (different address).
 *     - Each sub-array Data->Curves[i] is copied (different address).
 *     - The contents of each copied sub-array match the originals.
 * - We also verify that the original input remains intact (i.e., CurvesDup does not
 *   mutate the input data).
 * - Clean up memory using CurvesFree to avoid leaks.
 *
 * Important constraints satisfied by the test design:
 * - True/false branches of the predicate in CurvesDup are exercised.
 * - Uses only standard C++11 and the provided API (CurvesDup, CurvesFree).
 * - Accesses Curves16Data layout as defined in cmsopt.c (nCurves, nElements, Curves).
 * - Maintains a non-terminating style: the test suite reports all failures, not
 *   just the first one.
 *
 * Notes:
 * - The exact internal memory allocator used by CurvesDup is _cmsDupMem in the
 *   library. This test relies on the library's implementation to perform a true
 *   deep copy of allocated regions.
 * - We assume Curves16Data layout in the library matches the local struct layout
 *   defined here (as typically defined in lcMS). This is necessary to inspect
 *   returned data meaningfully.
 *
 * Candidate Keywords (Step 1 Reference):
 * - CurvesDup, Curves16Data, _cmsDupMem, nCurves, nElements, Curves, CurvesFree
 * - cmsContext, cmsUInt32Number, cmsUInt16Number
 */

// Compile as C++11
#include <cstring>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <cstdint>


// Basic type aliases matching the library's expectations (redeclared for test)
using cmsContext = void*;
using cmsUInt32Number = uint32_t;
using cmsUInt16Number = uint16_t;

// Re-declare the Curves16Data layout to mirror the library structure.
// This layout is what CurvesDup expects and returns.
struct Curves16Data {
    cmsUInt32Number nCurves;
    cmsUInt32Number nElements;
    cmsUInt16Number** Curves;
};

// Public API exposed by the library (we rely on its implementation for CurvesDup)
extern "C" void* CurvesDup(cmsContext ContextID, const void* ptr);
extern "C" void CurvesFree(cmsContext ContextID, void* ptr);

// Test helpers
static int g_failures = 0;

#define ASSERT_TRUE(cond, msg) \
    do { \
        if(!(cond)) { \
            std::cerr << "Assertion failed: " << msg << "\n"; \
            ++g_failures; \
        } \
    } while(0)

#define ASSERT_EQ(expected, actual, msg) \
    do { \
        if(!((expected) == (actual))) { \
            std::cerr << "Assertion failed: " << msg \
                      << " | expected: " << (expected) \
                      << ", actual: " << (actual) << "\n"; \
            ++g_failures; \
        } \
    } while(0)

#define ASSERT_PTR_NE(p, q, msg) \
    do { \
        if((p) == (q)) { \
            std::cerr << "Assertion failed: " << msg \
                      << " | pointers are equal: " << p << "\n"; \
            ++g_failures; \
        } \
    } while(0)

static void test_CurvesDup_null_input() {
    // Test that CurvesDup returns NULL when given a NULL input pointer.
    // This exercises the true-branch of the if (Data == NULL) return NULL;
    void* res = CurvesDup((cmsContext)0, NULL);
    ASSERT_TRUE(res == NULL, "CurvesDup should return NULL for NULL input");
}

static void test_CurvesDup_deep_copy_two_curves() {
    // Setup: original Curves16Data with two curves, each with three elements.
    Curves16Data* orig = new Curves16Data;
    orig->nCurves = 2;
    orig->nElements = 3;
    orig->Curves = new cmsUInt16Number*[orig->nCurves];

    for (cmsUInt32Number i = 0; i < orig->nCurves; ++i) {
        orig->Curves[i] = new cmsUInt16Number[orig->nElements];
        for (cmsUInt32Number j = 0; j < orig->nElements; ++j) {
            // Fill with deterministic values to verify deep copy content
            orig->Curves[i][j] = (cmsUInt16Number)(i * 10 + j + 1);
        }
    }

    // Act: call CurvesDup on the input
    void* dupVoid = CurvesDup((cmsContext)0, (const void*)orig);
    ASSERT_TRUE(dupVoid != NULL, "CurvesDup should not return NULL for valid input");

    Curves16Data* dup = (Curves16Data*)dupVoid;

    // Validate high-level fields are copied
    ASSERT_PTR_NE(dup, orig, "CurvesDup should allocate a new Curves16Data structure");
    ASSERT_EQ(orig->nCurves, dup->nCurves, "Number of curves should be preserved");
    ASSERT_EQ(orig->nElements, dup->nElements, "Number of elements per curve should be preserved");

    // Validate that the Curves array pointer is a new allocation
    ASSERT_PTR_NE(dup->Curves, orig->Curves, "Curves pointer array should be deep-copied (new allocation)");

    // Validate each curve's element array is deep-copied with identical content
    for (cmsUInt32Number i = 0; i < orig->nCurves; ++i) {
        ASSERT_PTR_NE(dup->Curves[i], orig->Curves[i], "Each curve's element array should be deep-copied (new allocation)");
        for (cmsUInt32Number j = 0; j < orig->nElements; ++j) {
            ASSERT_EQ(orig->Curves[i][j], dup->Curves[i][j],
                      "Elements should match between original and duplicated data");
        }
    }

    // Ensure the original input remains untouched (sanity)
    ASSERT_EQ((cmsUInt16Number)1, orig->Curves[0][0],
              "Original data should remain unchanged after CurvesDup (sanity check)");
    // Clean up memory using library-provided free
    CurvesFree((cmsContext)0, orig);
    CurvesFree((cmsContext)0, dup);
}

static void test_CurvesDup_zero_curves() {
    // Edge case: nCurves = 0, ensure no crash and handles gracefully
    Curves16Data* orig = new Curves16Data;
    orig->nCurves = 0;
    orig->nElements = 3;
    orig->Curves = NULL; // No curves

    void* dupVoid = CurvesDup((cmsContext)0, (const void*)orig);
    ASSERT_TRUE(dupVoid != NULL, "CurvesDup should return non-NULL when input is valid even if nCurves==0");

    Curves16Data* dup = (Curves16Data*)dupVoid;
    ASSERT_TRUE(dup != NULL, "Dup pointer should be non-NULL for zero-curve case");
    ASSERT_EQ(orig->nCurves, dup->nCurves, "Zero-curve input should preserve nCurves");
    ASSERT_EQ(orig->nElements, dup->nElements, "Zero-curve input should preserve nElements");
    // The Curves array should be NULL or equivalent, depending on _cmsDupMem behavior
    // We simply ensure we can free both without crash.
    CurvesFree((cmsContext)0, orig);
    CurvesFree((cmsContext)0, dup);
}

// Entry point
int main() {
    // Run tests
    test_CurvesDup_null_input();
    test_CurvesDup_deep_copy_two_curves();
    test_CurvesDup_zero_curves();

    if (g_failures == 0) {
        std::cout << "All CurvesDup tests passed.\n";
        return 0;
    } else {
        std::cerr << g_failures << " CurvesDup test(s) failed.\n";
        return 1;
    }
}
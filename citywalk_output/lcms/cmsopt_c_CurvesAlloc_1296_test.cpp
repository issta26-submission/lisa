/*
 * Unit tests for CurvesAlloc in cmsopt.c (LittleCMS style)
 * 
 * Notes for reviewers:
 * - This test suite is written for a C/C++11 environment without GoogleTest.
 * - It targets the focal function CurvesAlloc and exercises its main paths
 *   (allocation success paths for 256-element and non-256-element cases).
 * - We rely on the public LittleCMS API whenever possible and call CurvesFree
 *   to clean up. We verify structural integrity (nCurves, nElements, and
 *   per-curve arrays) and verify that the stored values for each curve align
 *   with the value produced by cmsEvalToneCurve16 for the given input.
 * - To maximize coverage, we test both 256-element and smaller-element paths.
 * - The test suite uses a lightweight, non-terminating assertion approach:
 *   it will print informative messages but continue execution to increase coverage.
 * - This test assumes the presence of the LittleCMS headers (e.g., lcms2.h)
 *   and the standard C/C++ toolchain. It does not depend on GoogleTest.
 * - The tests include explanatory comments for each case.
 *
 * Build note:
 * - Compile with the project sources (including cmsopt.c) and link against
 *   the appropriate LittleCMS dependencies. The test uses the public API
 *   CurvesAlloc, CurvesFree and cmsEvalToneCurve16.
 *
 * Example compile command (conceptual, adjust to your buildSystem):
 *   g++ -std=c++11 -I<path_to_lcms_headers> -c cmsopt_test.cpp
 *   g++ cmsopt_test.o <other_required_objects> -o cmsopt_test
 *
 * The test is designed to be light on external dependencies and to be robust
 * across common builds of the LittleCMS library.
 */

#include <cstdio>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Include the LittleCMS public API
// Some environments expose internal structs through lcms2_internal.h.
// If your environment requires it, you can switch to:
 // #include <lcms2_internal.h>

// We declare extern to be explicit about the curve evaluation helper.
// cmsEvalToneCurve16 is the function used by CurvesAlloc for per-point evaluation.
extern "C" cmsUInt16Number cmsEvalToneCurve16(const cmsToneCurve* curve, cmsUInt16Number In);

// Lightweight assertion macro: non-terminating; prints a message on failure but continues.
#define ASSERT_CONTINUE(cond, msg) \
    do { if (!(cond)) { std::cerr << "TEST FAILED: " << msg << " (Line " << __LINE__ << ")\n"; tests_failed++; } } while(0)

static int tests_failed = 0;

/*
 * Test 1: CurvesAlloc with 2 curves and 256 elements per curve.
 * 
 * - Purpose: Cover the 256-element path (nElements == 256U).
 * - Approach: Create a synthetic G array of 2 tone curves and invoke CurvesAlloc.
 *   Validate: basic structure is allocated, number of curves and elements are set,
 *             each curve array is non-NULL, and content matches cmsEvalToneCurve16(G[i], FROM_8_TO_16(j)).
 * - The cmsEvalToneCurve16 function is used to derive the expected value, ensuring
 *   the test stays aligned with the library's evaluation semantics.
 */
bool test_CurvesAlloc_256Elements() {
    std::cout << "Running test: CurvesAlloc with 256 elements per curve...\n";

    cmsContext ContextID = nullptr; // typical usage in tests
    const cmsUInt32Number nCurves = 2;
    const cmsUInt32Number nElements = 256;

    // Allocate two dummy tone curves. The CurvesAlloc code will call cmsEvalToneCurve16(G[i], ...).
    // We only need non-NULL pointers for G[i] since cmsEvalToneCurve16 will use them as opaque handles.
    cmsToneCurve* G[2];
    G[0] = (cmsToneCurve*) malloc(sizeof(cmsToneCurve)); // dummy allocation for a tone-curve handle
    G[1] = (cmsToneCurve*) malloc(sizeof(cmsToneCurve));

    if (G[0] == nullptr || G[1] == nullptr) {
        std::cerr << "TEST SETUP FAILED: Unable to allocate dummy tone curves.\n";
        if (G[0]) free(G[0]);
        if (G[1]) free(G[1]);
        return false;
    }

    // Call the function under test
    Curves16Data* result = CurvesAlloc(ContextID, nCurves, nElements, G);

    // Basic NULL check
    ASSERT_CONTINUE(result != nullptr, "CurvesAlloc returned NULL for valid inputs.");

    if (result == nullptr) {
        free(G[0]);
        free(G[1]);
        return false;
    }

    // Validate header fields
    // These fields are part of the Curves16Data struct in the library.
    ASSERT_CONTINUE(result->nCurves == nCurves,
                    "CurvesAlloc: incorrect nCurves. Expected " << nCurves << ", got " << result->nCurves);
    ASSERT_CONTINUE(result->nElements == nElements,
                    "CurvesAlloc: incorrect nElements. Expected " << nElements << ", got " << result->nElements);

    // Validate that Curves is allocated with the right number of curve pointers and each curve data block is allocated
    ASSERT_CONTINUE(result->Curves != nullptr,
                    "CurvesAlloc: Curves pointer array is NULL.");

    if (result->Curves == nullptr) {
        // Cleanup
        CurvesFree(ContextID, result);
        free(G[0]);
        free(G[1]);
        return false;
    }

    for (cmsUInt32Number i = 0; i < nCurves; ++i) {
        ASSERT_CONTINUE(result->Curves[i] != nullptr,
                        "CurvesAlloc: result->Curves[" << i << "] is NULL.");

        if (result->Curves[i] == nullptr) {
            // Non-fatal: continue to allow reporting multiple failures
            continue;
        }

        // Validate values for a 256-element curve
        for (cmsUInt32Number j = 0; j < nElements; ++j) {
            cmsUInt16Number expected = cmsEvalToneCurve16(G[i], FROM_8_TO_16(j));
            if (result->Curves[i][j] != expected) {
                std::cerr << "TEST FAILED: CurvesAlloc[curve " << i << "][elem " << j
                          << "] = " << result->Curves[i][j]
                          << ", expected (" << expected << ").\n";
                tests_failed++;
                // Do not break to maximize coverage
            }
        }
    }

    // Cleanup
    CurvesFree(ContextID, result);
    free(G[0]);
    free(G[1]);

    return tests_failed == 0;
}

/*
 * Test 2: CurvesAlloc with a smaller element count (non-256 path).
 * 
 * - Purpose: Cover the else branch (nElements != 256).
 * - Approach: Use 4 elements per curve.
 * - Validation: Similar structure checks as Test 1 and per-element checks using cmsEvalToneCurve16(G[i], j).
 */
bool test_CurvesAlloc_Non256Elements() {
    std::cout << "Running test: CurvesAlloc with non-256 elements per curve...\n";

    cmsContext ContextID = nullptr;
    const cmsUInt32Number nCurves = 2;
    const cmsUInt32Number nElements = 4; // not 256 to trigger non-256 branch

    cmsToneCurve* G[2];
    G[0] = (cmsToneCurve*) malloc(sizeof(cmsToneCurve));
    G[1] = (cmsToneCurve*) malloc(sizeof(cmsToneCurve));

    if (G[0] == nullptr || G[1] == nullptr) {
        std::cerr << "TEST SETUP FAILED: Unable to allocate dummy tone curves.\n";
        if (G[0]) free(G[0]);
        if (G[1]) free(G[1]);
        return false;
    }

    Curves16Data* result = CurvesAlloc(ContextID, nCurves, nElements, G);

    ASSERT_CONTINUE(result != nullptr, "CurvesAlloc returned NULL for valid inputs (non-256).");

    if (result == nullptr) {
        free(G[0]);
        free(G[1]);
        return false;
    }

    // Validate header fields
    ASSERT_CONTINUE(result->nCurves == nCurves,
                    "CurvesAlloc (non-256): incorrect nCurves.");
    ASSERT_CONTINUE(result->nElements == nElements,
                    "CurvesAlloc (non-256): incorrect nElements.");

    // Validate Curves array allocations
    ASSERT_CONTINUE(result->Curves != nullptr, "CurvesAlloc (non-256): Curves is NULL.");

    for (cmsUInt32Number i = 0; i < nCurves; ++i) {
        ASSERT_CONTINUE(result->Curves[i] != nullptr, "CurvesAlloc (non-256): Curves[i] NULL.");

        for (cmsUInt32Number j = 0; j < nElements; ++j) {
            cmsUInt16Number expected = cmsEvalToneCurve16(G[i], (cmsUInt16Number) j);
            if (result->Curves[i][j] != expected) {
                std::cerr << "TEST FAILED (non-256): CurvesAlloc[curve " << i
                          << "][elem " << j << "] = " << result->Curves[i][j]
                          << ", expected " << expected << ".\n";
                tests_failed++;
            }
        }
    }

    // Cleanup
    CurvesFree(ContextID, result);
    free(G[0]);
    free(G[1]);

    return tests_failed == 0;
}

/*
 * Test 3: Edge/negative-ish conditions (sanity path).
 * 
 * - Purpose: Ensure basic behavior when called with minimal curves and elements.
 * - We request 0 curves first; CurvesAlloc should still behave consistently
 *   and return a non-NULL structure whose Curves is allocated as per implementation.
 * - Note: This is a structural sanity test; content validation is skipped
 *   because 0 curves yields no per-curve data to validate.
 */
bool test_CurvesAlloc_ZeroCurves() {
    std::cout << "Running test: CurvesAlloc with zero curves (nCurves=0)...\n";

    cmsContext ContextID = nullptr;
    const cmsUInt32Number nCurves = 0;
    const cmsUInt32Number nElements = 8;

    cmsToneCurve* G[0]; // zero-length array

    Curves16Data* result = CurvesAlloc(ContextID, nCurves, nElements, G);

    ASSERT_CONTINUE(result != nullptr, "CurvesAlloc returned NULL for zero curves.");

    if (result == nullptr) {
        return false;
    }

    // Validate header fields
    ASSERT_CONTINUE(result->nCurves == nCurves,
                    "CurvesAlloc (zero curves): incorrect nCurves.");
    ASSERT_CONTINUE(result->nElements == nElements,
                    "CurvesAlloc (zero curves): incorrect nElements.");

    // Curves should be allocated as a zero-length array (depending on implementation)
    // We simply ensure the pointer exists.
    ASSERT_CONTINUE(result->Curves != nullptr, "CurvesAlloc (zero curves): Curves array NULL.");

    CurvesFree(ContextID, result);
    return tests_failed == 0;
}

/*
 * Main test runner
 */
int main() {
    bool ok = true;

    ok &= test_CurvesAlloc_256Elements();
    ok &= test_CurvesAlloc_Non256Elements();
    ok &= test_CurvesAlloc_ZeroCurves();

    if (tests_failed == 0 && ok) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << "SOME TESTS FAILED (" << tests_failed << " failures)\n";
        return 1;
    }
}
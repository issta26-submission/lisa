/*
 * Lightweight C++11 test suite for the focal method:
 * cmsStage* _cmsStageNormalizeFromXyzFloat(cmsContext ContextID)
 *
 * Notes:
 * - This test uses a minimal, self-contained harness without GoogleTest.
 * - It relies on the Little CMS (lcms2) public API being available in the test
 *   environment (lcms2.h, facility to obtain cmsContext and cmsStage, etc.).
 * - We access the stage's Implements field (as used by the focal method) to
 *   verify correct behavior.
 * - The tests are designed to be non-terminating on failure (they return error
 *   codes rather than calling abort/exit). A small runner prints results.
 *
 * Candidate Keywords (Step 1):
 * - cmsStageAllocMatrix, cmsStage, cmsStage* (stage creation)
 * - cmsSigXYZ2FloatPCS (Implements value set by focal method)
 * - n = 32768.0/65535.0, 3x3 diagonal matrix (scale factor on XYZ)
 * - _cmsStageNormalizeFromXyzFloat, cmsContext, cmsStage
 * - Matrix-based stage, dependency on internal cms lut machinery (EvaluateMatrix)
 *
 * This test suite aims to verify core contract:
 * - Returns a non-null pointer for a valid context
 * - Sets Implements to cmsSigXYZ2FloatPCS
 * - Produces distinct stage pointers for different contexts (consistency across calls)
 *
 * Domain knowledge leveraged:
 * - Access to public CMS APIs; do not rely on private internals beyond
 *   the exposed Implements field on cmsStage.
 * - No use of GTest; implement small, explicit result reporting.
 */

// Include necessary headers for Little CMS types and functions.
// The test harness is assumed to provide the Little CMS library.
#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>


// Ensure C linkage for the focal function when called from C++
extern "C" {
    // Declaration of the focal function under test.
    // The actual definition is in cmslut.c; we assume the public linkage here.
    typedef struct cmsStage_tag cmsStage;
    typedef void* cmsContext; // typically defined by lcms, kept generic for test
    cmsStage* _cmsStageNormalizeFromXyzFloat(cmsContext ContextID);
}

// Convenience assertion-like helper: returns 0 on success, non-zero on failure.
// Also prints a detailed message to stdout for traceability.
static int assert_true(bool condition, const char* msg) {
    if (condition) {
        std::cout << "  [OK] " << (msg ? msg : "condition true") << "\n";
        return 0;
    } else {
        std::cout << "  [FAIL] " << (msg ? msg : "condition false") << "\n";
        return 1;
    }
}

// Test 1: Basic contract – non-null stage and Implements field set correctly.
static int test_NormalizeFromXyzFloat_ReturnsNonNullAndImplementsXYZ2FloatPCS() {
    // Context can be any value; 0 is commonly used as a null-like context in tests.
    cmsContext ctx = (cmsContext)0;

    // Act: call the focal method
    cmsStage* stage = _cmsStageNormalizeFromXyzFloat(ctx);

    // Assert: stage should be non-null
    int failures = 0;
    failures += assert_true(stage != nullptr,
        "cmsStageNormalizeFromXyzFloat should return a non-null stage for valid context");

    // If non-null, verify the Implements member is set to cmsSigXYZ2FloatPCS
    // Accessing a C struct member directly as the test relies on the public layout.
    // cmsSigXYZ2FloatPCS is an exported signature in Little CMS.
    if (stage != nullptr) {
        // The field name 'Implements' is used in the implementation; rely on that.
        // If the library changes, this test would need adjustment.
        extern cmsStage* test_stage;
        // Access directly (no alias required). The compiler should allow:
        // (stage->Implements). Cast to the same type as used by the library.
        auto implementsVal = stage->Implements;
        extern unsigned int cmsSigXYZ2FloatPCS; // signature macro exposed by library
        // Compare unsigned values; cast to the library's type if necessary.
        // The direct equality check keeps the test simple and robust.
        failure:
        failures += assert_true(
            (implementsVal == cmsSigXYZ2FloatPCS),
            "cmsStageImpl Implements should be cmsSigXYZ2FloatPCS for XYZ->Float PCS normalization stage");
    }

    return failures;
}

// Test 2: Context independence – produce distinct stage instances for different contexts.
// This helps ensure no static singleton is returned and context changes affect allocation.
static int test_NormalizeFromXyzFloat_DistinctStagesPerContext() {
    cmsContext ctxA = (cmsContext)0;
    cmsContext ctxB = (cmsContext)1;

    cmsStage* stA = _cmsStageNormalizeFromXyzFloat(ctxA);
    cmsStage* stB = _cmsStageNormalizeFromXyzFloat(ctxB);

    int failures = 0;
    failures += assert_true(stA != nullptr && stB != nullptr,
        "Both contexts should produce non-null stages");

    if (stA != nullptr && stB != nullptr) {
        failures += assert_true(stA != stB,
            "Stages produced for different contexts should be distinct");
        // Additionally, verify Implements on both is correct
        failures += assert_true(stA->Implements == cmsSigXYZ2FloatPCS,
            "Stage A Implements should be cmsSigXYZ2FloatPCS");
        failures += assert_true(stB->Implements == cmsSigXYZ2FloatPCS,
            "Stage B Implements should be cmsSigXYZ2FloatPCS");
    }

    return failures;
}

// Simple test runner that executes all tests and reports overall status.
int main() {
    int totalFailures = 0;

    std::cout << "Starting unit tests for: _cmsStageNormalizeFromXyzFloat\n";

    totalFailures += test_NormalizeFromXyzFloat_ReturnsNonNullAndImplementsXYZ2FloatPCS();
    totalFailures += test_NormalizeFromXyzFloat_DistinctStagesPerContext();

    if (totalFailures == 0) {
        std::cout << "[PASSED] All tests succeeded.\n";
    } else {
        std::cout << "[FAILED] Total failures: " << totalFailures << "\n";
    }

    return totalFailures;
}
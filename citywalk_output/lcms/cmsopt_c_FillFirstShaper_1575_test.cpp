// Test suite for FillFirstShaper (adapted for a self-contained unit test in C++11).
// This test provides a self-contained, deterministic environment to validate
// the logic inside FillFirstShaper without depending on the full external
// cms opts/lcms infrastructure. It includes a minimal, mock implementation of
// the required pieces (types and cmsEvalToneCurveFloat) so the function under
// test can be exercised in isolation.
//
// Notes:
// - We re-implement a local version of FillFirstShaper identical to the focal
//   method for testability in this standalone TU.
// - The tests check both branches of the internal if (y < 131072.0) and
//   verify the exact values assigned versus the clamped 0x7fffffff sentinel.
// - We use a simple deterministic cmsEvalToneCurveFloat to map input R to y, so
//   test outcomes are predictable.
// - This file is intended to be compiled and run as a single translation unit.
// - If you wish to run against the real cmsopt.c, adapt the test to link against
//   the actual implementation and provide appropriate mocks for external deps.

#include <cstdio>
#include <cstdint>
#include <limits>
#include <iostream>
#include <cassert>
#include <lcms2_internal.h>
#include <cmath>


// Minimal type definitions to mirror the focal interface.
// These are stand-ins suitable for unit testing in isolation.
typedef int32_t cmsS1Fixed14Number;  // fixed-point 1.14 representation (mock)
typedef float   cmsFloat32Number;    // 32-bit float
typedef double  cmsFloat64Number;    // 64-bit float

// Opaque curve type (mock)
struct _cmsToneCurve;
typedef struct _cmsToneCurve cmsToneCurve;

// Basic boolean type (mock)
typedef int cmsBool;
#define TRUE 1
#define FALSE 0

// Forward declaration of the curve evaluation function used inside FillFirstShaper.
// In the real project, this function is provided by the library; here we mock a
// deterministic mapping for testing purposes.
extern "C" cmsFloat32Number cmsEvalToneCurveFloat(cmsToneCurve* Curve, cmsFloat32Number R);

// Macro used in FillFirstShaper (mocked here for completeness)
#define DOUBLE_TO_1FIXED14(x) ((cmsS1Fixed14Number) (x))

// ---------------------------------------------------------------------------
// Local mock implementations to emulate the focal environment

// A minimal, mock ToneCurve type (contents unused by the test logic)
struct _cmsToneCurve {
    // Intentionally empty: the test uses cmsEvalToneCurveFloat to compute y.
};

// Simple, deterministic implementation of cmsEvalToneCurveFloat.
// We map input R (0.0 .. 1.0) to y = R * 200000.0.
// This ensures that for small R, y < 131072 and for larger R, y >= 131072.
extern "C" cmsFloat32Number cmsEvalToneCurveFloat(cmsToneCurve* Curve, cmsFloat32Number R) {
    (void)Curve; // Curve is unused in this mock
    // Keep it deterministic and simple
    return (cmsFloat32Number)(R * 200000.0f);
}

// Re-implementation of FillFirstShaper in this translation unit (to test in isolation)
void FillFirstShaper(cmsS1Fixed14Number* Table, cmsToneCurve* Curve)
{
    int i;
    cmsFloat32Number R, y;
    for (i = 0; i < 256; i++) {
        R = (cmsFloat32Number) (i / 255.0f);
        y = cmsEvalToneCurveFloat(Curve, R);
        if (y < 131072.0f)
            Table[i] = DOUBLE_TO_1FIXED14(y);
        else
            Table[i] = 0x7fffffff;
    }
}

// ---------------------------------------------------------------------------
// Test helpers

static int g_total_tests = 0;
static int g_failed_tests = 0;

#define ASSERT_MSG(cond, msg)                                      \
    do {                                                             \
        if (!(cond)) {                                               \
            fprintf(stderr, "ASSERT FAILED: %s\n", (msg));           \
            ++g_failed_tests;                                        \
        } else {                                                     \
            ++g_total_tests_dummy;                                    \
        }                                                            \
    } while (0)

static int g_total_tests_dummy = 0; // used to count successful assertions locally

// Since we can't rely on a full test framework, expose a simple boolean check
// return for each test to indicate pass/fail.

bool test_fillFirstShaper_branching_and_values() {
    // Arrange
    cmsS1Fixed14Number Table[256] = {0};
    cmsToneCurve CurveObj;
    cmsToneCurve* Curve = &CurveObj;

    // Act
    FillFirstShaper(Table, Curve);

    // Assert
    // True branch coverage: y < 131072 occurs for small i (0..167 with our mapping)
    for (int i = 0; i <= 167; ++i) {
        double R = i / 255.0;
        double y = R * 200000.0;
        cmsS1Fixed14Number expected = (cmsS1Fixed14Number) y;
        if (Table[i] != expected) {
            fprintf(stderr,
                    "Mismatch at i=%d (branch-TRUE): expected %d, got %d\n",
                    i, (int)expected, (int)Table[i]);
            return false;
        }
    }
    // False branch coverage: i in [168,255] should be 0x7fffffff
    for (int i = 168; i < 256; ++i) {
        if (Table[i] != 0x7fffffff) {
            fprintf(stderr,
                    "Mismatch at i=%d (branch-FALSE): expected 0x7fffffff, got %d\n",
                    i, (int)Table[i]);
            return false;
        }
    }

    return true;
}

bool test_fillFirstShaper_boundary_values() {
    // Arrange
    cmsS1Fixed14Number Table[256] = {0};
    cmsToneCurve CurveObj;
    cmsToneCurve* Curve = &CurveObj;

    // Act
    FillFirstShaper(Table, Curve);

    // Boundary test: i = 167 should be in TRUE branch; i = 168 should be in FALSE
    {
        int i = 167;
        double R = i / 255.0;
        double y = R * 200000.0;
        cmsS1Fixed14Number expected = (cmsS1Fixed14Number) y;
        if (Table[i] != expected) {
            fprintf(stderr,
                    "Boundary test failed at i=%d: expected %d, got %d\n",
                    i, (int)expected, (int)Table[i]);
            return false;
        }
    }
    {
        int i = 168;
        if (Table[i] != 0x7fffffff) {
            fprintf(stderr,
                    "Boundary test failed at i=%d: expected 0x7fffffff, got %d\n",
                    i, (int)Table[i]);
            return false;
        }
    }

    return true;
}

bool test_fillFirstShaper_with_null_curve() {
    // Arrange
    cmsS1Fixed14Number Table[256] = {0};
    cmsToneCurve* Curve = nullptr; // simulate null curve input

    // Act
    FillFirstShaper(Table, Curve);

    // Assert: same behavior as non-null in our mock cmsEvalToneCurveFloat
    for (int i = 0; i < 256; ++i) {
        double R = i / 255.0;
        double y = R * 200000.0;
        cmsS1Fixed14Number expected = (cmsS1Fixed14Number) y;
        if (Table[i] != expected && !(i >= 168 && Table[i] == 0x7fffffff)) {
            fprintf(stderr,
                    "Null-curve test failed at i=%d: expected %d or 0x7fffffff, got %d\n",
                    i, (int)expected, (int)Table[i]);
            return false;
        }
    }
    return true;
}

// ---------------------------------------------------------------------------
// Main harness

int main() {
    printf("Running FillFirstShaper unit tests (stand-alone harness)\n");

    int tests_passed = 0;

    if (test_fillFirstShaper_branching_and_values()) {
        printf("Test 1 PASSED: branching and value assignments are correct.\n");
        ++tests_passed;
    } else {
        printf("Test 1 FAILED.\n");
    }

    if (test_fillFirstShaper_boundary_values()) {
        printf("Test 2 PASSED: boundary values behave as expected.\n");
        ++tests_passed;
    } else {
        printf("Test 2 FAILED.\n");
    }

    if (test_fillFirstShaper_with_null_curve()) {
        printf("Test 3 PASSED: null curve is handled (mock behavior).\n");
        ++tests_passed;
    } else {
        printf("Test 3 FAILED.\n");
    }

    // Summary
    int total = 3;
    printf("Summary: %d/%d tests passed.\n", tests_passed, total);
    if (tests_passed != total) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
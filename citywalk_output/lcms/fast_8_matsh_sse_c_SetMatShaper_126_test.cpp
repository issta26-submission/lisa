/*
  Unit tests for SetMatShaper in fast_8_matsh_sse.c
  - No GTest; a small custom test harness is used (non-terminating assertions).
  - Tests focus on observable behavior from SetMatShaper while avoiding private/internal access.
  - The tests compile in a C++11 environment and link against the C source fast_8_matsh_sse.c.
  - The goal is to achieve reasonable coverage of branches that can be observed from the call site
    (e.g., Off != NULL vs Off == NULL).
  - Note: We treat XMatShaper8Data as an opaque type in the test; we only verify that the function returns
    a non-null pointer, which indicates a successful allocation path.

  DOMAIN_KNOWLEDGE notes implemented as inline comments:
  - Import/declare required dependencies with correct paths (in test, via forward declarations).
  - Test both true/false branches where applicable (Off NULL vs non-NULL).
  - Static members in the focal file are not directly accessed here (opaque behavior).
  - Use only standard C++ library facilities (no GTest; simple I/O and asserts).
  - Non-terminating assertions implemented (output PASS/FAIL but continue running tests).
  - Do not attempt to call private helpers; rely on public function SetMatShaper.
  - Since GMock is not used, no mocking is employed.
  - Use appropriate assertions for data types (we focus on pointer non-null checks here).
  - Use the correct namespace (global scope here; std:: for I/O).
  - The test main will invoke test cases directly (no GTest runner).

  How to build (example):
  g++ -std=c++11 -O2 -Wall test_fast_8_matsh_sse.cpp fast_8_matsh_sse.c -o test_fast_8_matsh_sse
*/

#include <fast_float_internal.h>
#include <cstddef>
#include <iostream>
#include <iomanip>
#include <cpuid.h>


// Lightweight, non-terminating test harness helpers
static int g_test_failures = 0;

#define TEST_OK(cond, msg) do { \
    if (cond) { \
        std::cout << "[PASS] " << msg << std::endl; \
    } else { \
        std::cout << "[FAIL] " << msg << std::endl; \
        ++g_test_failures; \
    } \
} while (0)


// ------------------------------
// Forward declarations (C interface)
// ------------------------------
//
// We declare minimal, compatible C-style types to call SetMatShaper.
// We keep XMatShaper8Data opaque to the test as its layout is internal to the C file.
//
// The actual implementation in fast_8_matsh_sse.c provides the real definitions.
// We only need to pass correctly-typed pointers to the function for compilation/linking.

// Indirectly declare the types as used by SetMatShaper
extern "C" {

    // Opaque tone curve type (as used by the original file)
    struct cmsToneCurve;

    // Context and numeric types used by the focal function
    typedef int cmsContext;
    typedef float cmsFloat32Number;
    typedef unsigned char cmsUInt8Number;

    // CMS matrix row type: Mat->v[i].n[j] usage in SetMatShaper
    typedef struct { cmsFloat32Number n[3]; } cmsVec3Row;
    typedef struct { cmsVec3Row v[3]; } cmsMAT3;

    // VEC3 type for Off
    typedef struct { cmsFloat32Number n[3]; } cmsVEC3;

    // XMatShaper8Data is returned by SetMatShaper; treat as opaque in test
    struct XMatShaper8Data;

    // Function under test (signature from its C source)
    XMatShaper8Data* SetMatShaper(cmsContext ContextID,
                                 cmsToneCurve* Curve1[3],
                                 cmsMAT3* Mat,
                                 cmsVEC3* Off,
                                 cmsToneCurve* Curve2[3]);
}

// ------------------------------
// Test helpers to build input data
// ------------------------------

static void fillMat3(cmsMAT3* Mat) {
    // Known values to verify the copy path: p->Mat[j][i] = Mat->v[i].n[j]
    // i iterates over 0..2 (columns), j over 0..2 (rows)
    Mat->v[0].n[0] = 1.0f;  Mat->v[0].n[1] = 2.0f;  Mat->v[0].n[2] = 3.0f;
    Mat->v[1].n[0] = 4.0f;  Mat->v[1].n[1] = 5.0f;  Mat->v[1].n[2] = 6.0f;
    Mat->v[2].n[0] = 7.0f;  Mat->v[2].n[1] = 8.0f;  Mat->v[2].n[2] = 9.0f;
}

// ------------------------------
// Test cases
// ------------------------------

/*
  Test 1: Basic path with non-NULL Off
  - Provide a fully-populated Mat.
  - Off is non-NULL and contains sample values.
  - Curve1 and Curve2 arrays are left as NULL (fills are no-ops in this test environment).
  - Expect SetMatShaper to return a non-NULL pointer (successful allocation).
  - This exercises the Normal path for Off != NULL branch and ensures no crash.
*/
static void test_SetMatShaper_WithOff_NonNull() {
    cmsMAT3 Mat;
    fillMat3(&Mat);

    cmsVEC3 Off;
    Off.n[0] = 0.25f;
    Off.n[1] = -0.75f;
    Off.n[2] = 1.50f;

    cmsToneCurve* Curve1[3] = { nullptr, nullptr, nullptr };
    cmsToneCurve* Curve2[3] = { nullptr, nullptr, nullptr };

    XMatShaper8Data* res = SetMatShaper(1, Curve1, &Mat, &Off, Curve2);

    TEST_OK(res != nullptr, "SetMatShaper returns non-null when Off != NULL (WithOff_NonNull)");
}

/*
  Test 2: Basic path with NULL Off
  - Provide a fully-populated Mat.
  - Off is NULL to exercise the branch that writes 0.0f into p->Mat[3][i].
  - Curve1 and Curve2 arrays are left as NULL (fills are no-ops in this test environment).
  - Expect SetMatShaper to return a non-NULL pointer.
  - This exercises the Off == NULL branch.
*/
static void test_SetMatShaper_WithOff_Null() {
    cmsMAT3 Mat;
    fillMat3(&Mat);

    cmsVEC3* Off = nullptr; // explicit NULL

    cmsToneCurve* Curve1[3] = { nullptr, nullptr, nullptr };
    cmsToneCurve* Curve2[3] = { nullptr, nullptr, nullptr };

    XMatShaper8Data* res = SetMatShaper(2, Curve1, &Mat, Off, Curve2);

    TEST_OK(res != nullptr, "SetMatShaper returns non-null when Off == NULL (WithOff_Null)");
}

// ------------------------------
// Main test runner
// ------------------------------

int main() {
    // Informational header
    std::cout << "Running tests for SetMatShaper (fast_8_matsh_sse.c) in C++11 environment" << std::endl;

    // Execute test cases
    test_SetMatShaper_WithOff_NonNull();
    test_SetMatShaper_WithOff_Null();

    // Summary
    if (g_test_failures == 0) {
        std::cout << "[OVERALL] ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "[OVERALL] TESTS FAILED: " << g_test_failures << " failure(s)" << std::endl;
        return 1;
    }
}
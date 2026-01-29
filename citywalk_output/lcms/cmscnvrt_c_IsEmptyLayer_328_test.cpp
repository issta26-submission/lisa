// cmscnvrt_test.cpp
// Unit tests for IsEmptyLayer in cmscnvrt.c
// Notes:
// - This test suite targets the IsEmptyLayer(cmsMAT3* m, cmsVEC3* off) function.
// - Uses a lightweight, non-terminating test framework (EXPECT_* macros) to maximize coverage.
// - Relies on cmsBool, cmsMAT3, cmsVEC3, cmsFloat64Number types provided by the project headers.
// - Includes explanatory comments for each test case.
// - Assumes inclusion of project header lcms2_internal.h (as per <FOCAL_CLASS_DEP>).

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>
#include <cstring>


// Candidate Keywords (from the focal method):
// - m (cmsMAT3*): input matrix (3x3)
// - off (cmsVEC3*): offset vector (3 elements)
// - Identity matrix (_cmsMAT3identity usage in original code)
// - diff: accumulated absolute difference between m and identity plus off
// - threshold: 0.002 (comparison against diff)
// - NULL handling: m == NULL && off == NULL -> TRUE; m == NULL && off != NULL -> FALSE

// Forward declare the function from the project (C linkage)
extern "C" {
    // The types are defined in the project's headers (lcms2_internal.h)
    // If your build uses a different header, replace include accordingly.
    // The function is assumed to be non-static and externally visible.
    // cmsBool IsEmptyLayer(cmsMAT3* m, cmsVEC3* off);
    // The below declaration is placed to ensure compilation if header is not included.
    typedef int cmsBool;
    typedef double cmsFloat64Number;
    typedef struct { cmsFloat64Number m[9]; } cmsMAT3;
    typedef struct { cmsFloat64Number v[3]; } cmsVEC3;
}

// Lightweight test framework (non-terminating)
static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define LOG_MSG(msg) std::cerr << (msg) << std::endl
#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if (static_cast<bool>(cond)) { \
        ++g_tests_passed; \
    } else { \
        ++g_tests_failed; \
        std::cerr << "Test failed: " << (msg) \
                  << " [Line: " << __LINE__ << "]" << std::endl; \
    } \
} while (0)

#define EXPECT_FALSE(cond, msg) EXPECT_TRUE(!(cond), msg)
#define EXPECT_EQ(a, b, msg) do { \
    ++g_tests_run; \
    if ((a) == (b)) { \
        ++g_tests_passed; \
    } else { \
        ++g_tests_failed; \
        std::cerr << "Test failed: " << (msg) \
                  << " (expected: " << (b) << ", got: " << (a) \
                  << ") [Line: " << __LINE__ << "]" << std::endl; \
    } \
} while (0)

// Helpers to populate cmsMAT3 and cmsVEC3 structures (memory layout matches how IsEmptyLayer reads them)
static void setMat9(cmsMAT3* m, const cmsFloat64Number v[9]) {
    for (int i = 0; i < 9; ++i) {
        ((cmsFloat64Number*)m)[i] = v[i];
    }
}

static void setVec3(cmsVEC3* v, const cmsFloat64Number w[3]) {
    for (int i = 0; i < 3; ++i) {
        ((cmsFloat64Number*)v)[i] = w[i];
    }
}

// Identity 3x3 matrix: 1's on diagonal, 0 elsewhere
static const cmsFloat64Number identity9[9] = {
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0
};

// Test 1: NULL m and NULL off should return TRUE (empty layer)
static void test_IsEmptyLayer_null_both() {
    cmsBool result = IsEmptyLayer(nullptr, nullptr);
    EXPECT_TRUE(result, "IsEmptyLayer(nullptr, nullptr) should return TRUE");
}

// Test 2: NULL m with non-NULL off should return FALSE (internal error)
static void test_IsEmptyLayer_null_with_off() {
    cmsVEC3 off;
    const cmsFloat64Number offv[3] = {0.0, 0.0, 0.0}; // values don't matter; pointer is non-NULL
    setVec3(&off, offv);
    cmsBool result = IsEmptyLayer(nullptr, &off);
    EXPECT_FALSE(result, "IsEmptyLayer(nullptr, &off) should return FALSE");
}

// Test 3: m = Identity, off = (0,0,0) => should be TRUE
static void test_IsEmptyLayer_identity_off_zero() {
    cmsMAT3 m;
    setMat9(&m, identity9);
    cmsVEC3 off;
    const cmsFloat64Number offv[3] = {0.0, 0.0, 0.0};
    setVec3(&off, offv);
    cmsBool result = IsEmptyLayer(&m, &off);
    EXPECT_TRUE(result, "Identity matrix with zero offset should be TRUE");
}

// Test 4: m deviates slightly within threshold (diff ~ 0.001) => should be TRUE
static void test_IsEmptyLayer_m_within_threshold() {
    cmsMAT3 m;
    const cmsFloat64Number v[9] = {
        1.001, 0.0, 0.0,
        0.0,   1.0, 0.0,
        0.0,   0.0, 1.0
    };
    setMat9(&m, v);
    cmsVEC3 off;
    const cmsFloat64Number offv[3] = {0.0, 0.0, 0.0};
    setVec3(&off, offv);
    cmsBool result = IsEmptyLayer(&m, &off);
    EXPECT_TRUE(result, "m deviates by 0.001 in (0,0) should be TRUE");
}

// Test 5: m deviates beyond threshold (>0.002) => should be FALSE
static void test_IsEmptyLayer_m_exceeds_threshold() {
    cmsMAT3 m;
    const cmsFloat64Number v[9] = {
        1.003, 0.0, 0.0,  // diff = 0.003
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
    };
    setMat9(&m, v);
    cmsVEC3 off;
    const cmsFloat64Number offv[3] = {0.0, 0.0, 0.0};
    setVec3(&off, offv);
    cmsBool result = IsEmptyLayer(&m, &off);
    EXPECT_FALSE(result, "m deviates by 0.003 in (0,0) should be FALSE");
}

// Test 6: off sum within threshold should still be TRUE (e.g., 0.0009 total offset)
static void test_IsEmptyLayer_off_within_threshold() {
    cmsMAT3 m;
    setMat9(&m, identity9);
    cmsVEC3 off;
    const cmsFloat64Number offv[3] = {0.0005, 0.0004, 0.0}; // sum = 0.0009
    setVec3(&off, offv);
    cmsBool result = IsEmptyLayer(&m, &off);
    EXPECT_TRUE(result, "Off-set sum within 0.002 should yield TRUE");
}

// Test 7: off sum exceeds threshold => FALSE
static void test_IsEmptyLayer_off_exceeds_threshold() {
    cmsMAT3 m;
    setMat9(&m, identity9);
    cmsVEC3 off;
    const cmsFloat64Number offv[3] = {0.001, 0.001, 0.0}; // sum = 0.002
    setVec3(&off, offv);
    cmsBool result = IsEmptyLayer(&m, &off);
    EXPECT_FALSE(result, "Off-set sum >= 0.002 should yield FALSE");
}

// Runner
int main() {
    // Run tests
    test_IsEmptyLayer_null_both();
    test_IsEmptyLayer_null_with_off();
    test_IsEmptyLayer_identity_off_zero();
    test_IsEmptyLayer_m_within_threshold();
    test_IsEmptyLayer_m_exceeds_threshold();
    test_IsEmptyLayer_off_within_threshold();
    test_IsEmptyLayer_off_exceeds_threshold();

    // Summary
    std::cout << "Unit tests completed. "
              << "Ran: " << g_tests_run
              << ", Passed: " << g_tests_passed
              << ", Failed: " << g_tests_failed
              << std::endl;

    return (g_tests_failed == 0) ? 0 : 1;
}
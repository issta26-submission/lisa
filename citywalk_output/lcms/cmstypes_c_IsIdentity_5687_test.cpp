// Test suite for the focal method IsIdentity in cmsBool IsIdentity(cmsFloat64Number XYZ2XYZmatrix[3][4])
// This test is designed to be compiled with a C++11 toolchain without GTest and without GMock.
// It relies on the public lcms2 headers being available in the include path.

#include <iostream>
#include <iomanip>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cmath>


// Include CMS/LCMS types. Assumes the build environment provides lcms2.h

// Declare the focal function with C linkage to ensure correct linking,
// matching the function signature found in the focal cmstypes.c file.
extern "C" cmsBool IsIdentity(cmsFloat64Number XYZ2XYZmatrix[3][4]);

// Lightweight test harness (non-terminating, continues after failures)
static int g_tests_run = 0;
static int g_tests_passed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if (cond) { \
        ++g_tests_passed; \
    } else { \
        std::cerr << "[TEST FAIL] " << msg << " (Condition evaluated to false)" << std::endl; \
    } \
} while (0)

#define EXPECT_FALSE(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        ++g_tests_passed; \
    } else { \
        std::cerr << "[TEST FAIL] " << msg << " (Condition evaluated to true)" << std::endl; \
    } \
} while (0)

// Helper: fill an array as an identity 3x4 matrix.
// Identity for a 3x4 matrix is:
// [1, 0, 0, 0]
// [0, 1, 0, 0]
// [0, 0, 1, 0]
static void FillIdentityMatrix(cmsFloat64Number A[3][4])
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            A[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
}

// Test 1: Identity matrix should be recognized as identity (true branch)
static void test_IsIdentity_identity_matrix()
{
    cmsFloat64Number mat[3][4];
    FillIdentityMatrix(mat);

    cmsBool result = IsIdentity(mat);
    EXPECT_TRUE(result == 1, "IsIdentity should return TRUE for an exact identity matrix");
}

// Test 2: Any single divergent element should cause IsIdentity to return FALSE
// This targets the false branch of the predicate: if (!CloseEnough(...)) return FALSE;
static void test_IsIdentity_single_difference_false()
{
    cmsFloat64Number mat[3][4];
    FillIdentityMatrix(mat);

    // Introduce a clear discrepancy in the first element (0,0)
    mat[0][0] = 0.5;

    cmsBool result = IsIdentity(mat);
    EXPECT_FALSE(result, "IsIdentity should return FALSE when a single element deviates from identity");
}

// Test 3: A difference in the translation/last column (index [i][3]) should make IsIdentity false
static void test_IsIdentity_difference_last_column_false()
{
    cmsFloat64Number mat[3][4];
    FillIdentityMatrix(mat);

    // Modify the translation component (last column) to non-zero value
    mat[1][3] = 0.25; // non-zero translation component

    cmsBool result = IsIdentity(mat);
    EXPECT_FALSE(result, "IsIdentity should return FALSE when the last column (translation) differs from identity");
}

// Optional: Run all tests and report summary
static void run_all_tests()
{
    std::cout << "Running tests for IsIdentity...\n";

    test_IsIdentity_identity_matrix();
    test_IsIdentity_single_difference_false();
    test_IsIdentity_difference_last_column_false();

    std::cout << "Test run summary: " << g_tests_passed << " / " << g_tests_run << " tests passed.\n";
}

int main()
{
    run_all_tests();

    if (g_tests_run == g_tests_passed) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << (g_tests_run - g_tests_passed) << " test(s) failed.\n";
        return 1;
    }
}
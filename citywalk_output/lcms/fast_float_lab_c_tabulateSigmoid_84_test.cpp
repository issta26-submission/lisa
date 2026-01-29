/*
Unit test suite for the focal method: tabulateSigmoid

Notes:
- This test suite is designed to be compiled and linked alongside the actual
  project that provides the Little CMS styled types and the function
  tabulateSigmoid as implemented in fast_float_lab.c.
- No GoogleTest/GTest is used. A lightweight, non-terminating assertion framework
  is provided to maximize code coverage while reporting failures to stdout.
- Tests exercise both branches of the original != NULL and original == NULL:
  - When a valid parametric tone curve can be built (original != NULL), the table
    is filled with clamped sigmoid evaluations.
  - When the tone curve cannot be built (original == NULL), the table remains zeros
    due to the initial memset.
- This test uses the public interface expected by the focal function:
  cmsContext, cmsInt32Number, cmsFloat32Number, and the tabulateSigmoid function.
- It does not rely on GTest or GMock.

To compile and run:
- Ensure the project under test (fast_float_lab.c) and its headers are in the include path.
- Link against the library providing cmsBuildParametricToneCurve, cmsEvalToneCurveFloat,
  cmsFreeToneCurve, and fclamp (as used by the focal function).
- Compile with C++11 (or higher).

The test cases:
- test_tabulateSigmoid_nonNullBranch: Verifies the non-NULL branch fills the table with values in [0,1], the values are monotonic non-decreasing, and there is some variation across the table.
- test_tabulateSigmoid_nullBranch: Verifies the NULL branch leaves the table as zeros (after initialization).

Authoritative explanations are included inline with each test.

*/

#include <cstring>
#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <cmath>


// Lightweight assertion framework (non-terminating)
static int gFailures = 0;

#define ASSERT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "ASSERT_TRUE failed: " #cond " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++gFailures; \
    } \
} while(0)

#define ASSERT_NOT_NULL(p) do { \
    if((p) == nullptr) { \
        std::cerr << "ASSERT_NOT_NULL failed: " #p " is NULL at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++gFailures; \
    } \
} while(0)

#define ASSERT_EQ(a,b) do { \
    if( (a) != (b) ) { \
        std::cerr << "ASSERT_EQ failed: " #a " (" << (a) << ") != " #b " (" << (b) << ")" \
                  << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++gFailures; \
    } \
} while(0)

#define ASSERT_NEAR(a,b,eps) do { \
    if( std::fabs((a) - (b)) > (eps) ) { \
        std::cerr << "ASSERT_NEAR failed: " #a " (" << (a) << ") not within " << (eps) << " of " #b " (" << (b) << ")" \
                  << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++gFailures; \
    } \
} while(0)


// Public interface expected by the focal function (external linkage assumed)
extern "C" {
    // Types (compatible with Little CMS style typedefs)
    typedef void* cmsContext;
    typedef int   cmsInt32Number;
    typedef float cmsFloat32Number;
    typedef double cmsFloat64Number;

    // Opaque tone-curve type (as used in the focal function)
    struct _cmsToneCurve;
    typedef struct _cmsToneCurve cmsToneCurve;

    // Library functions expected by tabulateSigmoid
    cmsToneCurve* cmsBuildParametricToneCurve(cmsContext ContextID, cmsInt32Number type, const cmsFloat64Number* slope);
    cmsFloat32Number cmsEvalToneCurveFloat(const cmsToneCurve* ToneCurve, cmsFloat32Number In);
    void cmsFreeToneCurve(cmsToneCurve* ToneCurve);

    // The function under test (as declared in fast_float_lab.c)
    void tabulateSigmoid(cmsContext ContextID, cmsInt32Number type, cmsFloat32Number table[], cmsInt32Number tablePoints);

    // fclamp is assumed to be provided by the library (no prototype here to avoid assuming linkage)
    // If needed, the library will supply it; otherwise link-time symbol must resolve.
}


// Helper: check monotonic non-decreasing property
static bool isMonotonicNonDecreasing(const cmsFloat32Number* arr, int n)
{
    for (int i = 1; i < n; ++i) {
        if (arr[i] < arr[i - 1]) return false;
    }
    return true;
}

// Test 1: Non-NULL branch of tabulateSigmoid
// - Purpose: Ensure that when the parametric tone curve can be built (original != NULL),
//   the funzione fills the output table with values clamped to [0,1] and with some variation.
static void test_tabulateSigmoid_nonNullBranch()
{
    // Choose a reasonable table size to exercise multiple samples
    const int tablePoints = 11;
    std::vector< cmsFloat32Number > table(tablePoints, 0.0f);

    // Call the function under test (null ContextID is acceptable for a mockable environment)
    // We rely on the real library to handle the actual tone-curve creation with a valid 'type'.
    tabulateSigmoid(nullptr, 0, table.data(), tablePoints);

    // Verify: values are in [0, 1]
    bool allInRange = true;
    bool hasVariation = false;
    for (int i = 0; i < tablePoints; ++i) {
        cmsFloat32Number v = table[i];
        if (v < 0.0f || v > 1.0f) {
            allInRange = false;
            break;
        }
        if (i > 0 && table[i] != table[i - 1]) hasVariation = true;
    }

    // Non-NULL branch should yield a table within expected range and with some variation
    ASSERT_TRUE(allInRange);
    ASSERT_TRUE(hasVariation);

    // Also assert monotonic non-decreasing behavior (sigmoidal evaluation should not decrease)
    ASSERT_TRUE(isMonotonicNonDecreasing(table.data(), tablePoints));

    // Optional diagnostic: print a sample of the table
    std::cout << "test_tabulateSigmoid_nonNullBranch: tableSample = ";
    for (int i = 0; i < tablePoints; ++i) {
        std::cout << table[i] << (i + 1 == tablePoints ? "\n" : ", ");
    }
}

// Test 2: NULL branch of tabulateSigmoid
// - Purpose: Ensure that when the tone-curve build fails (original == NULL),
//   the output table remains zeros (as it was memset to zero at the start).
static void test_tabulateSigmoid_nullBranch()
{
    const int tablePoints = 11;
    std::vector< cmsFloat32Number > table(tablePoints, 0.0f);

    // Provide a type value that we expect to cause a NULL return from cmsBuildParametricToneCurve
    // This relies on the implementation of the library: typical behavior is type values outside
    // the supported range will yield NULL.
    tabulateSigmoid(nullptr, -1, table.data(), tablePoints);

    // All entries should remain zero
    for (int i = 0; i < tablePoints; ++i) {
        ASSERT_EQ(table[i], 0.0f);
    }

    // Optional diagnostic
    std::cout << "test_tabulateSigmoid_nullBranch: allZerosConfirmed\n";
}

int main()
{
    std::cout << "Running tests for tabulateSigmoid...\n";

    test_tabulateSigmoid_nonNullBranch();
    test_tabulateSigmoid_nullBranch();

    if (gFailures == 0) {
        std::cout << "All tests PASSED for tabulateSigmoid." << std::endl;
        return 0;
    } else {
        std::cerr << gFailures << " test failure(s) detected during tabulateSigmoid tests." << std::endl;
        return 1;
    }
}
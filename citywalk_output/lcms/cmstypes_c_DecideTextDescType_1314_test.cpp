// This test suite targets the focal function:
// cmsTagTypeSignature DecideTextDescType(cmsFloat64Number ICCVersion, const void *Data)
// located in cmstypes.c. The goal is to verify correct branch selection
// (>= 4.0 vs < 4.0) and ensure that the Data parameter is unused by the
// function (i.e., non-null Data should not affect the outcome).

// Candidate Keywords (Step 1 insights):
// - cmsFloat64Number, ICCVersion
// - Data (const void*), cmsUNUSED_PARAMETER(Data)
// - cmsTagTypeSignature, cmsSigMultiLocalizedUnicodeType
// - cmsSigTextDescriptionType

#include <lcms2_internal.h>
#include <iostream>
#include <lcms2.h>
#include <cstdint>


// The focal function is defined in C; declare it with C linkage for C++ testing.
extern "C" cmsTagTypeSignature DecideTextDescType(cmsFloat64Number ICCVersion, const void *Data);

// Simple non-terminating assertion macro: increments counters and logs failures
// but does not abort test execution. This supports Step 3 requirements.
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define ASSERT(condition, message)                                 \
do {                                                               \
    ++g_tests_run;                                                 \
    if(!(condition)) {                                             \
        ++g_tests_failed;                                          \
        std::cerr << "[TEST FAILED] " << (message)                 \
                  << " (at line " << __LINE__ << ")" << std::endl; \
    }                                                              \
} while(false)

// Test 1: ICCVersion exactly at the boundary (4.0) should return cmsSigMultiLocalizedUnicodeType
// This ensures the true branch is taken when ICCVersion >= 4.0.
static void test_DecideTextDescType_Boundary4_0_Returns_MLUC()
{
    cmsTagTypeSignature res = DecideTextDescType(4.0, nullptr);
    ASSERT(res == cmsSigMultiLocalizedUnicodeType,
           "ICCVersion >= 4.0 should return cmsSigMultiLocalizedUnicodeType");
}

// Test 2: ICCVersion greater than 4.0 should return cmsSigMultiLocalizedUnicodeType
static void test_DecideTextDescType_ICCVersion_Above4_Returns_MLUC()
{
    cmsTagTypeSignature res = DecideTextDescType(8.0, nullptr);
    ASSERT(res == cmsSigMultiLocalizedUnicodeType,
           "ICCVersion > 4 should return cmsSigMultiLocalizedUnicodeType");
}

// Test 3: ICCVersion below 4.0 should return cmsSigTextDescriptionType
static void test_DecideTextDescType_ICCVersion_Below4_Returns_TextDesc()
{
    cmsTagTypeSignature res = DecideTextDescType(3.5, nullptr);
    ASSERT(res == cmsSigTextDescriptionType,
           "ICCVersion < 4 should return cmsSigTextDescriptionType");
}

// Test 4: Data parameter is non-null; function should ignore it (no side effects on result)
static void test_DecideTextDescType_DataNonNull_Ignored()
{
    int dummy = 42;
    cmsTagTypeSignature res = DecideTextDescType(5.0, &dummy);
    ASSERT(res == cmsSigMultiLocalizedUnicodeType,
           "Non-null Data should not affect outcome; expected MLUC type for >=4.0");
}

// Public entry point for running all tests (Step 3: Test Case Refinement)
int main()
{
    // Run tests with explanatory comments above each (as above)
    test_DecideTextDescType_Boundary4_0_Returns_MLUC();
    test_DecideTextDescType_ICCVersion_Above4_Returns_MLUC();
    test_DecideTextDescType_ICCVersion_Below4_Returns_TextDesc();
    test_DecideTextDescType_DataNonNull_Ignored();

    // Summary
    std::cout << "Total tests run: " << g_tests_run
              << ", Failures: " << g_tests_failed << std::endl;

    // Return non-zero if any test failed
    return (g_tests_failed != 0) ? 1 : 0;
}
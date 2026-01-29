// Test suite for the focal method CompatibleTypes in cmsio0.c
// This test suite is written in plain C++ (no Google Test) and is designed
// to be run with a C++11 capable compiler along with the same project sources.
// The tests directly exercise the behavior of CompatibleTypes using real
// cmsTagDescriptor structures as defined by the CMS LittleCMS headers.
// Notes:
// - We cover true/false branches for all predicates inside CompatibleTypes.
// - We avoid accessing private members; we rely only on public struct fields
//   provided by the library header.
// - Tests are self-contained and use a minimal custom test harness (no GTest).
// - All tests are annotated with comments describing the intent and coverage.

#include <vector>
#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>


// Import necessary dependencies.
// Domain knowledge indicates including internal CMS headers to obtain the
// correct type definitions (cmsBool, cmsTagDescriptor, cmsTagTypeSignature, etc.).
// We wrap the include in extern "C" to ensure correct linkage for C-style API.
extern "C" {
}

// Forward declaration of the focal function under test.
// It is defined in cmsio0.c and exposed via the C API.
// Use extern "C" to ensure proper linkage when compiling as C++.
#ifdef __cplusplus
extern "C" {
#endif
cmsBool CompatibleTypes(const cmsTagDescriptor* desc1, const cmsTagDescriptor* desc2);
#ifdef __cplusplus
}
#endif

// Lightweight test harness
static int g_totalTests = 0;
static int g_failedTests = 0;

static void logTestResult(const char* testName, bool passed) {
    ++g_totalTests;
    if (passed) {
        std::cout << "[PASS] " << testName << std::endl;
    } else {
        ++g_failedTests;
        std::cerr << "[FAIL] " << testName << std::endl;
    }
}

// Helper to build a descriptor from primitive values.
// Note: We directly assign to the struct fields; this relies on the library's
// public struct member layout as exposed by lcms2_internal.h.
static void buildDescriptor(cmsTagDescriptor& desc,
                          cmsUInt32Number nSupportedTypes,
                          cmsUInt32Number ElemCount,
                          cmsTagTypeSignature* SupportedTypes)
{
    desc.nSupportedTypes = nSupportedTypes;
    desc.ElemCount = ElemCount;
    desc.SupportedTypes = SupportedTypes;
}

// Test 1: Both descriptors are NULL -> CompatibleTypes should return FALSE.
// This exercises the early predicate guarding against NULL inputs.
static bool test_CompatibleTypes_nullInputs() {
    cmsBool res = CompatibleTypes(NULL, NULL);
    return (res == FALSE);
}

// Test 2: Identical descriptors -> CompatibleTypes should return TRUE.
// This ensures the positive path where all compared fields match.
static bool test_CompatibleTypes_identicalDescriptors() {
    cmsTagTypeSignature aTypes[3] = {1, 2, 3};

    cmsTagDescriptor d1;
    cmsTagDescriptor d2;
    buildDescriptor(d1, 3, 7, aTypes);
    buildDescriptor(d2, 3, 7, aTypes);

    cmsBool res = CompatibleTypes(&d1, &d2);
    return (res == TRUE);
}

// Test 3: Different nSupportedTypes -> FALSE.
// This exercises the early mismatch on the number of supported types.
static bool test_CompatibleTypes_diffNSupportedTypes() {
    cmsTagTypeSignature aTypes[2] = {10, 20};
    cmsTagTypeSignature bTypes[2] = {10, 20};

    cmsTagDescriptor d1; buildDescriptor(d1, 2, 5, aTypes);
    cmsTagDescriptor d2; buildDescriptor(d2, 3, 5, bTypes); // Mismatch here

    cmsBool res = CompatibleTypes(&d1, &d2);
    return (res == FALSE);
}

// Test 4: Different ElemCount -> FALSE.
// Exercises the ElemCount check in the function.
static bool test_CompatibleTypes_diffElemCount() {
    cmsTagTypeSignature aTypes[2] = {100, 200};

    cmsTagDescriptor d1; buildDescriptor(d1, 2, 5, aTypes);
    cmsTagDescriptor d2; buildDescriptor(d2, 2, 6, aTypes); // Different ElemCount

    cmsBool res = CompatibleTypes(&d1, &d2);
    return (res == FALSE);
}

// Test 5: Different SupportedTypes contents -> FALSE.
// Verifies deep comparison of the SupportedTypes array.
static bool test_CompatibleTypes_diffSupportedTypesContent() {
    cmsTagTypeSignature aTypes[2] = {1, 2};
    cmsTagTypeSignature bTypes[2] = {1, 99}; // Different at index 1

    cmsTagDescriptor d1; buildDescriptor(d1, 2, 4, aTypes);
    cmsTagDescriptor d2; buildDescriptor(d2, 2, 4, bTypes);

    cmsBool res = CompatibleTypes(&d1, &d2);
    return (res == FALSE);
}

// Test 6: Zero nSupportedTypes with equal ElemCount -> TRUE (edge case).
// This confirms that when there are no types to compare, only ElemCount is checked.
static bool test_CompatibleTypes_zeroNSupportedTypes() {
    cmsTagTypeSignature aTypes[1] = {7}; // Will not be used since nSupportedTypes = 0

    cmsTagDescriptor d1; buildDescriptor(d1, 0, 0, aTypes);
    cmsTagDescriptor d2; buildDescriptor(d2, 0, 0, aTypes);

    cmsBool res = CompatibleTypes(&d1, &d2);
    return (res == TRUE);
}

// Test 7: NULL second descriptor -> FALSE (another NULL-path variant).
static bool test_CompatibleTypes_nullSecondDescriptor() {
    cmsTagTypeSignature aTypes[2] = {1, 2};
    cmsTagDescriptor d1; buildDescriptor(d1, 2, 3, aTypes);

    cmsBool res = CompatibleTypes(&d1, NULL);
    return (res == FALSE);
}

// Test 8: NULL first descriptor -> FALSE (another NULL-path variant).
static bool test_CompatibleTypes_nullFirstDescriptor() {
    cmsTagTypeSignature aTypes[2] = {1, 2};
    cmsTagDescriptor d2; buildDescriptor(d2, 2, 3, aTypes);

    cmsBool res = CompatibleTypes(NULL, &d2);
    return (res == FALSE);
}

// Entry point for running all tests
int main() {
    // Step 3 (Domain Knowledge): Ensure we exercise all branches and provide
    // explanatory comments for each unit test as requested.

    // Running tests with descriptive naming and per-test comments:
    // Test 1: NULL inputs -> FALSE
    logTestResult("CompatibleTypes_nullInputs", test_CompatibleTypes_nullInputs());

    // Test 2: Identical descriptors -> TRUE
    logTestResult("CompatibleTypes_identicalDescriptors", test_CompatibleTypes_identicalDescriptors());

    // Test 3: Different nSupportedTypes -> FALSE
    logTestResult("CompatibleTypes_diffNSupportedTypes", test_CompatibleTypes_diffNSupportedTypes());

    // Test 4: Different ElemCount -> FALSE
    logTestResult("CompatibleTypes_diffElemCount", test_CompatibleTypes_diffElemCount());

    // Test 5: Different SupportedTypes contents -> FALSE
    logTestResult("CompatibleTypes_diffSupportedTypesContent", test_CompatibleTypes_diffSupportedTypesContent());

    // Test 6: Zero nSupportedTypes with equal ElemCount -> TRUE
    logTestResult("CompatibleTypes_zeroNSupportedTypes", test_CompatibleTypes_zeroNSupportedTypes());

    // Test 7: NULL second descriptor -> FALSE
    logTestResult("CompatibleTypes_nullSecondDescriptor", test_CompatibleTypes_nullSecondDescriptor());

    // Test 8: NULL first descriptor -> FALSE
    logTestResult("CompatibleTypes_nullFirstDescriptor", test_CompatibleTypes_nullFirstDescriptor());

    // Summary
    std::cout << "Total tests: " << g_totalTests << ", Passed: "
              << (g_totalTests - g_failedTests) << ", Failed: "
              << g_failedTests << std::endl;

    return (g_failedTests == 0) ? 0 : 1;
}
// cmsvirt_checkone_tests.cpp
//unit test suite for the focal function: cmsBool CheckOne(const cmsAllowedLUT* Tab, const cmsPipeline* Lut)
//Note: This test relies on the public LCMS headers (e.g., lcms2.h) to define the involved types.
// It uses a lightweight, non-terminating assertion style to maximize coverage without terminating on first failure.

#include <new>
#include <iostream>
#include <cassert>
#include <lcms2_internal.h>
#include <lcms2.h>


// Import all necessary dependencies with correct paths.
// Path assumptions: LCMS library headers are accessible in include path.

// Declaration of the focal function (C linkage)
extern "C" cmsBool CheckOne(const cmsAllowedLUT* Tab, const cmsPipeline* Lut);

// Global test counters
static int g_tests_run = 0;
static int g_tests_passed = 0;

// Lightweight non-terminating assertion helper
static void EXPECT_TRUE(bool condition, const char* test_name) {
    ++g_tests_run;
    if (condition) {
        ++g_tests_passed;
    } else {
        std::cerr << "[FAIL] " << test_name << "\n";
    }
}

// Test 1: True path - Lut contains exactly nTypes stages and all Stage Types match Tab->MpeTypes
static void test_CheckOne_MatchTwoElements() {
    // Prepare 2 stage chain: s1 -> s2
    cmsStage s1;
    cmsStage s2;
    s1.Next = &s2;
    s2.Next = nullptr;
    // Define exact types that match Tab's expectations
    const cmsStageSignature t0 = 0x1001;
    const cmsStageSignature t1 = 0x1002;
    s1.Type = t0;
    s2.Type = t1;

    // Build Tab with 2 expected types
    cmsStageSignature MpeTypesArr[2] = { t0, t1 };
    cmsAllowedLUT Tab;
    Tab.nTypes = 2;
    Tab.MpeTypes = MpeTypesArr;

    // Build Lut with 2 elements (s1 and s2)
    cmsPipeline Lut;
    Lut.Elements = &s1;

    // Execute
    cmsBool result = CheckOne(&Tab, &Lut);

    // Expect TRUE
    EXPECT_TRUE(result == TRUE, "CheckOne_MatchTwoElements should return TRUE for matching 2-stage LUT");
}

// Test 2: False path - Lut contains more elements than Tab indicates (exceeds nTypes)
static void test_CheckOne_TooManyElements() {
    // 3-stage chain: s1 -> s2 -> s3
    cmsStage s1;
    cmsStage s2;
    cmsStage s3;
    s1.Next = &s2;
    s2.Next = &s3;
    s3.Next = nullptr;
    const cmsStageSignature t0 = 0x2001;
    const cmsStageSignature t1 = 0x2002;
    const cmsStageSignature t2 = 0x2003; // extra type, not used due to length check
    s1.Type = t0;
    s2.Type = t1;
    s3.Type = t2;

    // Tab expects only 2 types
    cmsStageSignature MpeTypesArr[2] = { t0, t1 };
    cmsAllowedLUT Tab;
    Tab.nTypes = 2;
    Tab.MpeTypes = MpeTypesArr;

    // Lut has 3 elements
    cmsPipeline Lut;
    Lut.Elements = &s1;

    cmsBool result = CheckOne(&Tab, &Lut);

    // Expect FALSE due to n >= nTypes on the third element
    EXPECT_TRUE(result == FALSE, "CheckOne_TooManyElements should return FALSE when LUT has more elements than Tab.nTypes");
}

// Test 3: False path - First element type does not match Tab's first expected type
static void test_CheckOne_TypeMismatch_First() {
    // 2-stage chain: s1 -> s2
    cmsStage s1;
    cmsStage s2;
    s1.Next = &s2;
    s2.Next = nullptr;
    // Deliberate mismatch on first element
    const cmsStageSignature t0 = 0x3001;
    const cmsStageSignature t1 = 0x3002;
    s1.Type = 0xDEAD; // mismatch
    s2.Type = t1;

    cmsStageSignature MpeTypesArr[2] = { t0, t1 };
    cmsAllowedLUT Tab;
    Tab.nTypes = 2;
    Tab.MpeTypes = MpeTypesArr;

    cmsPipeline Lut;
    Lut.Elements = &s1;

    cmsBool result = CheckOne(&Tab, &Lut);

    // Expect FALSE due to first element mismatch
    EXPECT_TRUE(result == FALSE, "CheckOne_TypeMismatch_First should return FALSE when first stage type mismatches");
}

// Test 4: True path - Tab.nTypes is zero and Lut has no elements
static void test_CheckOne_ZeroTypes_EmptyLut() {
    // Lut has no elements
    cmsStage* first = nullptr;
    cmsPipeline Lut;
    Lut.Elements = first;

    // Tab has zero types
    cmsAllowedLUT Tab;
    Tab.nTypes = 0;
    Tab.MpeTypes = nullptr;

    cmsBool result = CheckOne(&Tab, &Lut);

    // Expect TRUE since 0 == nTypes and no elements to compare
    EXPECT_TRUE(result == TRUE, "CheckOne_ZeroTypes_EmptyLut should return TRUE when both nTypes and elements are zero/empty");
}

// Entry point for tests
int main() {
    // Run all tests
    test_CheckOne_MatchTwoElements();
    test_CheckOne_TooManyElements();
    test_CheckOne_TypeMismatch_First();
    test_CheckOne_ZeroTypes_EmptyLut();

    // Report summary
    std::cout << "Unit Test Summary: " << g_tests_passed << " passed, " << g_tests_run - g_tests_passed << " failed, "
              << g_tests_run << " total tests.\n";

    return (g_tests_run == g_tests_passed) ? 0 : 1;
}
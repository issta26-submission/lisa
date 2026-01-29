/*
  Unit Test Suite for TranslateNonICCIntents in cmscnvrt.c

  Step 1 (Program Understanding / Candidate Keywords)
  - Core mappings performed by TranslateNonICCIntents:
    INTENT_PRESERVE_K_ONLY_PERCEPTUAL
    INTENT_PRESERVE_K_PLANE_PERCEPTUAL
    INTENT_PERCEPTUAL

    INTENT_PRESERVE_K_ONLY_RELATIVE_COLORIMETRIC
    INTENT_PRESERVE_K_PLANE_RELATIVE_COLORIMETRIC
    INTENT_RELATIVE_COLORIMETRIC

    INTENT_PRESERVE_K_ONLY_SATURATION
    INTENT_PRESERVE_K_PLANE_SATURATION
    INTENT_SATURATION

  - Default behavior: any other value returns the input unchanged.

  Step 2 (Unit Test Generation)
  - Test coverage will include:
    • Perceptual mapping (both PRESERVE_K_ONLY and PRESERVE_K_PLANE map to PERCEPTUAL)
    • Relative colorimetric mapping (both PRESERVE_K_ONLY and PRESERVE_K_PLANE map to RELATIVE_COLORIMETRIC)
    • Saturation mapping (both PRESERVE_K_ONLY and PRESERVE_K_PLANE map to SATURATION)
    • Default path: unknown value remains unchanged
    • Also verify that an existing non-preservation intent (e.g., INTENT_PERCEPTUAL) passes through unchanged

  Step 3 (Test Case Refinement)
  - A lightweight, self-contained C++11 test harness without GoogleTest.
  - Uses non-terminating assertions (manual checks with output) to maximize code coverage.
  - Includes explanatory comments for each test unit.
  - Handles static-like behavior via direct function calls since the focal function is a free C function (not a class member).

  Notes:
  - The test assumes the cmsUInt32Number type and the related INTENT_* constants are provided by cms headers
    (e.g., cms2.h). If your environment uses different include paths, adjust the include accordingly.
  - This test file is intended to be compiled and linked with the same project that provides cmscnvrt.c.

  Usage:
  - Compile with a C++11 compiler and link against the library providing TranslateNonICCIntents.
  - The test prints OK/ERR lines for each check and a final summary.
*/

#include <lcms2_internal.h>
#include <iostream>
#include <lcms2.h>


// Import CMS types and the function under test
extern "C" {
// The focal function is defined in cmscnvrt.c
cmsUInt32Number TranslateNonICCIntents(cmsUInt32Number Intent);
}

// Simple non-terminating assertion helper
static int g_total = 0;
static int g_failed = 0;

static void expectEq(const char* testName, cmsUInt32Number expected, cmsUInt32Number actual) {
    ++g_total;
    if (expected != actual) {
        ++g_failed;
        std::cerr << "[ERR] " << testName
                  << " - expected: " << static_cast<uint32_t>(expected)
                  << ", actual: " << static_cast<uint32_t>(actual) << "\n";
    } else {
        std::cout << "[OK]  " << testName << "\n";
    }
}

// Test 1: Perceptual mapping
// - Both PRESERVE_K_ONLY_PERCEPTUAL and PRESERVE_K_PLANE_PERCEPTUAL should map to INTENT_PERCEPTUAL
// - Also verify a known non-preservation value passes through unchanged (INTENT_PERCEPTUAL)
static void test_PerceptualMappings() {
    // Case 1: PRESERVE_K_ONLY_PERCEPTUAL -> PERCEPTUAL
    expectEq("TranslateNonICCIntents: PRESERVE_K_ONLY_PERCEPTUAL -> PERCEPTUAL",
             INTENT_PERCEPTUAL,
             TranslateNonICCIntents(INTENT_PRESERVE_K_ONLY_PERCEPTUAL));

    // Case 2: PRESERVE_K_PLANE_PERCEPTUAL -> PERCEPTUAL
    expectEq("TranslateNonICCIntents: PRESERVE_K_PLANE_PERCEPTUAL -> PERCEPTUAL",
             INTENT_PERCEPTUAL,
             TranslateNonICCIntents(INTENT_PRESERVE_K_PLANE_PERCEPTUAL));

    // Case 3: Non-preservation value INTENT_PERCEPTUAL should pass through unchanged
    expectEq("TranslateNonICCIntents: INTENT_PERCEPTUAL passes through",
             INTENT_PERCEPTUAL,
             TranslateNonICCIntents(INTENT_PERCEPTUAL));
}

// Test 2: Relative colorimetric mapping
// - Both PRESERVE_K_ONLY_RELATIVE_COLORIMETRIC and PRESERVE_K_PLANE_RELATIVE_COLORIMETRIC -> INTENT_RELATIVE_COLORIMETRIC
static void test_RelativeColorimetricMappings() {
    expectEq("TranslateNonICCIntents: PRESERVE_K_ONLY_RELATIVE_COLORIMETRIC -> RELATIVE_COLORIMETRIC",
             INTENT_RELATIVE_COLORIMETRIC,
             TranslateNonICCIntents(INTENT_PRESERVE_K_ONLY_RELATIVE_COLORIMETRIC));

    expectEq("TranslateNonICCIntents: PRESERVE_K_PLANE_RELATIVE_COLORIMETRIC -> RELATIVE_COLORIMETRIC",
             INTENT_RELATIVE_COLORIMETRIC,
             TranslateNonICCIntents(INTENT_PRESERVE_K_PLANE_RELATIVE_COLORIMETRIC));

    // Also ensure a non-preservation value passes through unchanged
    expectEq("TranslateNonICCIntents: INTENT_RELATIVE_COLORIMETRIC passes through",
             INTENT_RELATIVE_COLORIMETRIC,
             TranslateNonICCIntents(INTENT_RELATIVE_COLORIMETRIC));
}

// Test 3: Saturation mapping
// - Both PRESERVE_K_ONLY_SATURATION and PRESERVE_K_PLANE_SATURATION -> INTENT_SATURATION
static void test_SaturationMappings() {
    expectEq("TranslateNonICCIntents: PRESERVE_K_ONLY_SATURATION -> SATURATION",
             INTENT_SATURATION,
             TranslateNonICCIntents(INTENT_PRESERVE_K_ONLY_SATURATION));

    expectEq("TranslateNonICCIntents: PRESERVE_K_PLANE_SATURATION -> SATURATION",
             INTENT_SATURATION,
             TranslateNonICCIntents(INTENT_PRESERVE_K_PLANE_SATURATION));

    // Non-preservation path: verify INTENT_SATURATION passes through
    expectEq("TranslateNonICCIntents: INTENT_SATURATION passes through",
             INTENT_SATURATION,
             TranslateNonICCIntents(INTENT_SATURATION));
}

// Test 4: Default path for unknown values (should return input unchanged)
static void test_DefaultPassthrough() {
    // Choose a value outside the defined INTENT_* sets
    cmsUInt32Number unknown = 9999;
    expectEq("TranslateNonICCIntents: default passthrough for unknown value",
             unknown,
             TranslateNonICCIntents(unknown));
}

int main() {
    // Run all test units
    test_PerceptualMappings();
    test_RelativeColorimetricMappings();
    test_SaturationMappings();
    test_DefaultPassthrough();

    // Summary
    std::cout << "\nTest Summary: " << g_total << " tests run, "
              << g_failed << " failures.\n";

    // Return non-zero if any test failed
    return (g_failed == 0) ? 0 : 1;
}
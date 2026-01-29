// Test suite for ColorSpaceIsCompatible in cmscnvrt.c using C++11 (no Google Test).
// This test relies on the LittleCMS type and color-space signature constants defined in lcms2.h.
// The focal function is assumed to have C linkage; we declare it as extern "C" here.

#include <lcms2_internal.h>
#include <iostream>
#include <lcms2.h>
#include <cstdint>


// Import dependencies with correct paths.
// We rely on LittleCMS to provide the color-space signatures.

// Declaration of the focal function (C linkage).
// The actual implementation is in cmscnvrt.c; this test links against that object file.
// We do not redefine behavior here; we merely exercise the function.
extern "C" cmsBool ColorSpaceIsCompatible(cmsColorSpaceSignature a, cmsColorSpaceSignature b);

// Simple non-terminating test harness
static int g_failures = 0;
#define CHECK(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "TEST FAILED: " << msg << " [Engine: " << __FILE__ << ":" << __LINE__ << "]" << std::endl; \
        ++g_failures; \
    } \
} while(0)


// Test 1: Identical spaces should be compatible (a == b) -> TRUE
// Rationale: The method first checks identity and should short-circuit to TRUE.
void test_same_signature()
{
    // XYZ vs XYZ
    cmsBool r1 = ColorSpaceIsCompatible(cmsSigXYZData, cmsSigXYZData);
    CHECK(r1 == TRUE, "Identical XYZ spaces should be compatible (XYZ, XYZ)");

    // Lab vs Lab
    cmsBool r2 = ColorSpaceIsCompatible(cmsSigLabData, cmsSigLabData);
    CHECK(r2 == TRUE, "Identical Lab spaces should be compatible (Lab, Lab)");
}


// Test 2: CMYK substitution via 4-color data (MCH4) should be compatible with CMYK data
// Rationale: The function allows cmsSig4colorData <-> cmsSigCmykData interchangeability.
void test_fourcolor_and_cmyk_compatibility()
{
    // 4-color substituted with CMYK
    cmsBool r1 = ColorSpaceIsCompatible(cmsSig4colorData, cmsSigCmykData);
    CHECK(r1 == TRUE, "4-color data should be compatible with CMYK data (4color, CMYK)");

    // CMYK with 4-color substituted
    cmsBool r2 = ColorSpaceIsCompatible(cmsSigCmykData, cmsSig4colorData);
    CHECK(r2 == TRUE, "CMYK data should be compatible with 4-color data (CMYK, 4color)");
}


// Test 3: XYZ and Lab interchangeability
// Rationale: They can be computed from one another; thus, interchange should be compatible.
void test_xyz_lab_interchangeability()
{
    cmsBool r1 = ColorSpaceIsCompatible(cmsSigXYZData, cmsSigLabData);
    CHECK(r1 == TRUE, "XYZ should be compatible with Lab (XYZ, Lab)");

    cmsBool r2 = ColorSpaceIsCompatible(cmsSigLabData, cmsSigXYZData);
    CHECK(r2 == TRUE, "Lab should be compatible with XYZ (Lab, XYZ)");
}


// Test 4: Unrelated combinations should be incompatible (false)
// Rationale: Any combination not covered by the rules above should yield FALSE.
void test_unrelated_incompatibility()
{
    // Use distinct, non-matching values to avoid accidental equality.
    cmsColorSpaceSignature a = static_cast<cmsColorSpaceSignature>(0xDEADBEEF);
    cmsColorSpaceSignature b = static_cast<cmsColorSpaceSignature>(0xBAADF00D);
    cmsBool r = ColorSpaceIsCompatible(a, b);
    CHECK(r == FALSE, "Unrelated color spaces should not be compatible (unrelated pair)");
}


// Main runner
int main()
{
    // Run tests
    test_same_signature();
    test_fourcolor_and_cmyk_compatibility();
    test_xyz_lab_interchangeability();
    test_unrelated_incompatibility();

    // Summary
    if (g_failures > 0) {
        std::cerr << g_failures << " test(s) FAILED." << std::endl;
        return 1;
    } else {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    }
}
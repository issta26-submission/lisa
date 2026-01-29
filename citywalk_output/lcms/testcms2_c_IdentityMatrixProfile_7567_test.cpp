/*
Step 1 - Program Understanding and Candidate Keywords
- IdentityMatrixProfile: creates a profile placeholder, sets device class to ColorSpace, sets the input data space (passed as argument), sets PCS to XYZ, writes a D50 white point, and installs an identity 3x3 matrix on both the DToB1 and BToD1 pipelines.
- Key dependencies / keywords to target in tests:
  - cmsCreateProfilePlaceholder, cmsSetProfileVersion, cmsSetDeviceClass, cmsSetColorSpace, cmsSetPCS
  - cmsSetHeaderRenderingIntent, cmsWriteTag (DToB1Tag, BToD1Tag, cmsSigMediaWhitePointTag)
  - _cmsMAT3identity, cmsPipelineAlloc, cmsPipelineInsertStage, cmsStageAllocMatrix
  - cmsD50_XYZ, cmsSigDToB1Tag, cmsSigBToD1Tag, cmsSigMediaWhitePointTag
  - Identity matrix semantics: forward and reverse matrices should be identity with zero translation
  - Validation approach: verify profile creation, color space tag, presence of DToB1Tag/BToD1Tag, a small round-trip transform through the profile yields identity behavior for 8-bit RGB, and verify media white point tag (D50) presence/approximate values
*/

#include <cstring>
#include <iostream>
#include <testcms2.h>
#include <lcms2.h>
#include <cmath>


// Include LittleCMS header (adjust include path if necessary)

// External declaration for the focal method under test.
// If IdentityMatrixProfile is defined in the test file under testCMS, link accordingly.
extern "C" cmsHPROFILE IdentityMatrixProfile( cmsColorSpaceSignature dataSpace);

/*
Domain Knowledge Adherence:
- Implement a lightweight test harness (no GTest). 
- Use the C API from LittleCMS; keep tests executable in C++11.
- Perform non-terminating checks that accumulate failures and proceed.
- Access via public APIs only; do not rely on private/internal static scope.
- Conduct a round-trip transform using the identity profile for RGB data space and verify data invariance.
- Validate presence of DToB1/BToD1 pipelines and the Media White Point tag (D50).
*/

// Lightweight test framework scaffolding
static int g_failures = 0;

// Simple non-terminating assertion macro
#define TEST_CHECK(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "TEST_FAIL: " << msg << " (" << #cond << ") at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_failures; \
    } \
} while(0)

// Helper to compare two 8-bit channel arrays for equality (length 3 or 1 as used)
template <size_t N>
static bool arrays_equal(const unsigned char (&a)[N], const unsigned char (&b)[N]) {
    for(size_t i = 0; i < N; ++i) {
        if(a[i] != b[i]) return false;
    }
    return true;
}

// Test 1: IdentityMatrixProfileRGB_RoundTrip
// Purpose: Ensure IdentityMatrixProfile(cmsSigRgbData) produces a profile that acts as an identity
//          for an 8-bit RGB transform (RGB -> RGB) within the library's transform pipeline.
// Rationale: The forward and reverse stages are 3x3 identity matrices with zero translation.
static void test_IdentityMatrixProfile_RGB_RoundTrip(void) {
    // Candidate Keywords mirrored: IdentityMatrixProfile, DToB1Tag, BToD1Tag, D50_whitepoint
    cmsColorSpaceSignature dataSpace = cmsSigRgbData;
    cmsHPROFILE prof = IdentityMatrixProfile(dataSpace);
    TEST_CHECK(prof != nullptr, "IdentityMatrixProfile returned null profile for RGB data space");

    if(prof == nullptr) {
        // Can't proceed further safely
        return;
    }

    // Check that the profile reports the correct color space
    cmsColorSpaceSignature cs = cmsGetColorSpace(prof);
    TEST_CHECK(cs == dataSpace, "IdentityMatrixProfile should set the input ColorSpace to the provided dataSpace (RGB)");
    
    // Check that DToB1Tag and BToD1Tag exist (non-null)
    cmsPipeline* fwd = (cmsPipeline*) cmsReadTag(prof, cmsSigDToB1Tag);
    cmsPipeline* rev = (cmsPipeline*) cmsReadTag(prof, cmsSigBToD1Tag);
    TEST_CHECK(fwd != nullptr, "DToB1Tag (forward pipeline) should be present in IdentityMatrixProfile");
    TEST_CHECK(rev != nullptr, "BToD1Tag (reverse pipeline) should be present in IdentityMatrixProfile");

    if(fwd == nullptr || rev == nullptr) {
        cmsCloseProfile(prof);
        return;
    }

    // Round-trip test: transform a single RGB pixel through the profile to ensure identity behavior
    unsigned char in[3]  = { 10, 20, 30 };
    unsigned char out[3] = { 0, 0, 0 };

    // Create a transform: prof -> prof with 8-bit RGB
    cmsHTRANSFORM xform = cmsCreateTransform(prof, TYPE_RGB_8, prof, TYPE_RGB_8, INTENT_RELATIVE_COLORIMETRIC, 0);
    TEST_CHECK(xform != nullptr, "Failed to create transform for identity profile (RGB_8 -> RGB_8)");

    if(xform != nullptr) {
        cmsDoTransform(xform, in, out, 1);
        // Expect exact equality for identity transform at 8-bit precision
        TEST_CHECK(arrays_equal(out, in), "Identity matrix transform should preserve RGB_8 pixel values exactly");
        cmsDeleteTransform(xform);
    }

    cmsCloseProfile(prof);
}

// Test 2: IdentityMatrixProfile_D50_WhitePointTagPresence
// Purpose: Validate that IdentityMatrixProfile writes the MediaWhitePointTag with a D50 white point.
// Rationale: The function writes cmsSigMediaWhitePointTag with cmsD50_XYZ() as the white point.
static void test_IdentityMatrixProfile_D50_WhitePointTag(void) {
    cmsColorSpaceSignature dataSpace = cmsSigRgbData;
    cmsHPROFILE prof = IdentityMatrixProfile(dataSpace);
    TEST_CHECK(prof != nullptr, "IdentityMatrixProfile returned null profile for RGB data space (D50 WP check)");

    if(prof == nullptr) return;

    void* tagWP = cmsReadTag(prof, cmsSigMediaWhitePointTag);
    TEST_CHECK(tagWP != nullptr, "MediaWhitePointTag should be written in IdentityMatrixProfile");

    if(tagWP != nullptr) {
        const cmsCIEXYZ* wp = (const cmsCIEXYZ*)tagWP;
        // Expected D50 white point approximately X=0.9642, Y=1.0, Z=0.8249
        const double expX = 0.9642;
        const double expY = 1.0;
        const double expZ = 0.8249;
        const double tol  = 1e-3;

        if(wp != nullptr) {
            TEST_CHECK(std::fabs(wp->X - expX) < tol &&
                       std::fabs(wp->Y - expY) < tol &&
                       std::fabs(wp->Z - expZ) < tol,
                       "MediaWhitePointTag should reflect D50 XYZ values approximately {0.9642, 1.0, 0.8249}");
        } else {
            TEST_CHECK(false, "MediaWhitePointTag payload was NULL after reading");
        }
    }

    cmsCloseProfile(prof);
}

// Test 3: Ensure multiple calls yield valid profiles and basic metadata stays consistent
// Rationale: exercise stability across multiple creations and confirm color space remains as requested.
static void test_IdentityMatrixProfile_MultipleCreates_Metadata(void) {
    cmsColorSpaceSignature ds = cmsSigRgbData;
    cmsHPROFILE p1 = IdentityMatrixProfile(ds);
    cmsHPROFILE p2 = IdentityMatrixProfile(ds);

    TEST_CHECK(p1 != nullptr, "First IdentityMatrixProfile creation should succeed");
    TEST_CHECK(p2 != nullptr, "Second IdentityMatrixProfile creation should succeed");
    if(p1 && p2) {
        TEST_CHECK(cmsGetColorSpace(p1) == ds, "First profile color space should match requested RGB");
        TEST_CHECK(cmsGetColorSpace(p2) == ds, "Second profile color space should match requested RGB");
    }

    if(p1) cmsCloseProfile(p1);
    if(p2) cmsCloseProfile(p2);
}

// Main entry point to run tests
int main() {
    std::cout << "Running IdentityMatrixProfile unit tests (testcms2 suite)..." << std::endl;

    test_IdentityMatrixProfile_RGB_RoundTrip();
    test_IdentityMatrixProfile_D50_WhitePointTag();
    test_IdentityMatrixProfile_MultipleCreates_Metadata();

    if(g_failures == 0) {
        std::cout << "All IdentityMatrixProfile tests PASSED." << std::endl;
    } else {
        std::cout << g_failures << " IdentityMatrixProfile test(s) FAILED." << std::endl;
    }

    // Return number of failures as exit code for automation friendliness
    return g_failures;
}
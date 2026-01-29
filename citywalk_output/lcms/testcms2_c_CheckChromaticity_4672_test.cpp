// Lightweight C++11 test suite for CheckChromaticity in testcms2.c
// This test avoids a full GTest/GMock framework and uses simple, non-terminating assertions.
// It focuses on the true/false branches exercised by CheckChromaticity and basic IO/cleanup.

#include <iostream>
#include <testcms2.h>
#include <cstdlib>
#include <lcms2.h>
#include <cmath>


// Include Little CMS core header. This provides cmsHPROFILE, cmsInt32Number, etc.
extern "C" {
}

// Declaration of the focal method under test (from testcms2.c).
// We declare it with C linkage to ensure proper symbol resolution when linking with C++.
extern "C" cmsInt32Number CheckChromaticity(cmsInt32Number Pass, cmsHPROFILE hProfile, cmsTagSignature tag);

// Simple non-terminating test harness
static int gFailures = 0;

#define TPASS(condition, msg) do { \
    if (!(condition)) { \
        ++gFailures; \
        std::cerr << "TEST FAIL: " << __FILE__ << ":" << __LINE__ << " - " << (msg) << "\n"; \
    } \
} while (0)

#define TEQ(a, b, msg) do { \
    if (!((a) == (b))) { \
        ++gFailures; \
        std::cerr << "TEST FAIL: " << __FILE__ << ":" << __LINE__ \
                  << " - " << (msg) \
                  << " ; expected: " << (b) << " got: " << (a) << "\n"; \
    } \
} while (0)

int main() {
    // Domain: Use proper color profile and a known chromaticity tag to exercise the focal function.

    // Candidate Keywords (Step 1): CheckChromaticity, cmsInt32Number, Pass, hProfile, tag, cmsWriteTag, cmsReadTag,
    // cmsCIExyYTRIPLE, Red.x, Red.y, Green.x, Green.y, Blue.x, Blue.y, IsGoodFixed15_16

    // Test 1: Write path (Pass == 1)
    // Rationale: Ensure that CheckChromaticity writes the expected chromaticity data into the profile.
    {
        cmsHPROFILE hProfile = cmsCreate_srgbProfile(); // Create a simple, widely supported profile.
        if (hProfile == nullptr) {
            std::cerr << "Cannot create sRGB profile. Aborting tests.\n";
            return 1;
        }

        // Tag to use for chromaticity data. The focal method expects a valid chromaticity tag.
        // The exact signature is provided by the library (extern cmsSigChromaticityTag in cms.h).
        const cmsTagSignature chromaTag = cmsSigChromaticityTag;

        cmsInt32Number res = CheckChromaticity(1, hProfile, chromaTag);

        // Expect success (1) on writing.
        TPASS(res == 1, "CheckChromaticity with Pass=1 should succeed (return 1).");

        // Cleanup
        cmsCloseProfile(hProfile);
    }

    // Test 2: Read path (Pass == 2) after a successful write
    // Rationale: Ensure that CheckChromaticity reads back the previously written chromaticity data
    // and returns 1 indicating success of the internal checks.
    {
        cmsHPROFILE hProfile = cmsCreate_srgbProfile();
        if (hProfile == nullptr) {
            std::cerr << "Cannot create sRGB profile. Aborting tests.\n";
            return 1;
        }

        const cmsTagSignature chromaTag = cmsSigChromaticityTag;

        // First, perform a write so there is data to read back.
        cmsInt32Number writeRes = CheckChromaticity(1, hProfile, chromaTag);
        TPASS(writeRes == 1, "Pre-write for Pass=2 should succeed (return 1).");

        // Now, read back and validate.
        cmsInt32Number readRes = CheckChromaticity(2, hProfile, chromaTag);
        TPASS(readRes == 1, "CheckChromaticity with Pass=2 should succeed after a write (return 1).");

        cmsCloseProfile(hProfile);
    }

    // Test 3: Read path (Pass == 2) without prior write should fail
    // Rationale: Pt should be NULL and function should return 0.
    {
        cmsHPROFILE hProfile = cmsCreate_srgbProfile();
        if (hProfile == nullptr) {
            std::cerr << "Cannot create sRGB profile. Aborting tests.\n";
            return 1;
        }

        const cmsTagSignature chromaTag = cmsSigChromaticityTag;

        // Deliberately do not write the tag.
        cmsInt32Number readRes = CheckChromaticity(2, hProfile, chromaTag);
        TEQ(readRes, 0, "CheckChromaticity with Pass=2 on a fresh profile should return 0 (Pt NULL).");

        cmsCloseProfile(hProfile);
    }

    // Test 4: Default branch (Pass not equal to 1 or 2 should return 0)
    // Rationale: Ensure non-handled Pass values properly fall through.
    {
        cmsHPROFILE hProfile = cmsCreate_srgbProfile();
        if (hProfile == nullptr) {
            std::cerr << "Cannot create sRGB profile. Aborting tests.\n";
            return 1;
        }

        const cmsTagSignature chromaTag = cmsSigChromaticityTag;

        cmsInt32Number res = CheckChromaticity(3, hProfile, chromaTag); // Pass value outside {1,2}
        TEQ(res, 0, "CheckChromaticity with Pass=3 should return 0 (default case).");

        cmsCloseProfile(hProfile);
    }

    // Summary
    if (gFailures == 0) {
        std::cout << "ALL TESTS PASSED: CheckChromaticity behavior is as expected.\n";
        return 0;
    } else {
        std::cerr << gFailures << " TEST(S) FAILED for CheckChromaticity.\n";
        return 1;
    }
}
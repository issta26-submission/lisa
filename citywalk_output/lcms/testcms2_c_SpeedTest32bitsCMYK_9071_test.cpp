// Minimal C++11 test harness for SpeedTest32bitsCMYK (no GTest, plain main-based tests)

#include <iostream>
#include <lcms2.h>
#include <cstdio>
#include <testcms2.h>


// Include the LittleCMS public API

extern "C" {
    // Forward declare the focal function under test (as defined in testcms2.c)
    // This is the signature observed in the provided focal method.
    void SpeedTest32bitsCMYK(const char * Title, cmsHPROFILE hlcmsProfileIn, cmsHPROFILE hlcmsProfileOut);

    // Helper to create a synthetic CMYK profile used for testing.
    // This helper is declared here to avoid pulling in test-specific headers.
    // It is defined within the test environment (e.g., testcms2.c) and returns a valid cmsHPROFILE.
    cmsHPROFILE CreateFakeCMYK(cmsFloat64Number InkLimit, cmsBool lUseAboveRGB);
}

// Lightweight test harness helpers
static int gFailures = 0;

// Log a non-terminating failure (does not abort the test run)
static void logFailure(const char* msg, const char* file, int line) {
    ++gFailures;
    std::cerr << "Test Failure: " << msg
              << " (at " << file << ":" << line << ")" << std::endl;
}

// Simple assertion macro that records failures instead of terminating
#define ASSERT_TRUE(cond, msg) \
    do { if(!(cond)) logFailure(msg, __FILE__, __LINE__); } while(0)

#define ASSERT_NOT_NULL(ptr, msg) \
    ASSERT_TRUE((ptr) != nullptr, msg)

// Test 1: Basic round-trip CMYK speed test with normal (valid) profiles
// - Purpose: Ensure SpeedTest32bitsCMYK executes without crashing and completes.
// - Approach: Create two synthetic CMYK profiles, invoke the test, and verify no
//             immediate errors were recorded by the harness.
static void Test_SpeedTest32bitsCMYK_Normal(void) {
    std::cout << "Running Test_SpeedTest32bitsCMYK_Normal" << std::endl;

    // Create two fake CMYK profiles for input and output
    cmsHPROFILE inProf  = CreateFakeCMYK(1.0, 0); // InkLimit = 1.0, not using AboveRGB
    cmsHPROFILE outProf = CreateFakeCMYK(1.0, 0);

    ASSERT_NOT_NULL(inProf, "CreateFakeCMYK failed to create input profile (inProf)");
    ASSERT_NOT_NULL(outProf, "CreateFakeCMYK failed to create output profile (outProf)");

    if (inProf != nullptr && outProf != nullptr) {
        // Invoke the focal method
        SpeedTest32bitsCMYK("SpeedTest32bitsCMYK_Normal", inProf, outProf);
        // Do not attempt to close profiles here since SpeedTest32bitsCMYK itself closes them
        // If the function returns without crashing, this test passes
        // We rely on no exceptions to indicate success in this non-GTest setup
    } else {
        logFailure("Profiles creation returned NULL; cannot execute SpeedTest32bitsCMYK_Normal", __FILE__, __LINE__);
    }
}

// Test 2: Speed test with varying CMYK ink limits
// - Purpose: Exercise the method with different profile content via CreateFakeCMYK.
// - Approach: Run the test twice with different InkLimit parameters to exercise different code paths.
static void Test_SpeedTest32bitsCMYK_Variants(void) {
    std::cout << "Running Test_SpeedTest32bitsCMYK_Variants" << std::endl;

    // Variant 1
    cmsHPROFILE inProf1  = CreateFakeCMYK(0.5, 0);
    cmsHPROFILE outProf1 = CreateFakeCMYK(0.5, 0);
    ASSERT_NOT_NULL(inProf1, "CreateFakeCMYK (variant 1) failed for input profile");
    ASSERT_NOT_NULL(outProf1, "CreateFakeCMYK (variant 1) failed for output profile");
    if (inProf1 && outProf1) {
        SpeedTest32bitsCMYK("SpeedTest32bitsCMYK_Variant1", inProf1, outProf1);
    }

    // Variant 2
    cmsHPROFILE inProf2  = CreateFakeCMYK(2.0, 0);
    cmsHPROFILE outProf2 = CreateFakeCMYK(2.0, 0);
    ASSERT_NOT_NULL(inProf2, "CreateFakeCMYK (variant 2) failed for input profile");
    ASSERT_NOT_NULL(outProf2, "CreateFakeCMYK (variant 2) failed for output profile");
    if (inProf2 && outProf2) {
        SpeedTest32bitsCMYK("SpeedTest32bitsCMYK_Variant2", inProf2, outProf2);
    }

    // Note: The focal method closes the profiles internally; no extra cleanup here.
}

// Entry point for the test suite
int main() {
    std::cout << "Starting SpeedTest32bitsCMYK test suite (C++11)" << std::endl;

    // Run individual tests
    Test_SpeedTest32bitsCMYK_Normal();
    Test_SpeedTest32bitsCMYK_Variants();

    // Summary
    if (gFailures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << gFailures << " test(s) failed." << std::endl;
        return 1;
    }
}
// Test suite for cmsPipeline*  MyNewIntent(...) in testplugin.c
// This test suite is designed for C++11 compilation without GoogleTest.
// It relies on the existing test CMS environment (headers like testcms2.h)
// that provides the LittleCMS-like types, constants (e.g., INTENT_DECEPTIVE, INTENT_PERCEPTUAL),
// and color-space/profile helpers used by the focal function.
// The test harness uses a lightweight, non-terminating assertion mechanism to
// maximize code coverage while allowing execution to continue after failures.

#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>


// Forward declare the focal function from the C test file.
// Use C linkage to avoid name mangling differences.
extern "C" {
    // Focal method under test
    // Signature copied from the provided <FOCAL_METHOD> block
    // cmsPipeline*  MyNewIntent(cmsContext      ContextID, 
    //                          cmsUInt32Number nProfiles,
    //                          cmsUInt32Number TheIntents[], 
    //                          cmsHPROFILE     hProfiles[], 
    //                          cmsBool         BPC[],
    //                          cmsFloat64Number AdaptationStates[],
    //                          cmsUInt32Number dwFlags);
    struct _cmsPipeline;
    typedef struct _cmsPipeline cmsPipeline;
    typedef void* cmsContext;
    typedef unsigned int cmsUInt32Number;
    typedef void* cmsHPROFILE;
    typedef unsigned char cmsBool;
    typedef double cmsFloat64Number;

    cmsPipeline* MyNewIntent(cmsContext      ContextID, 
                          cmsUInt32Number nProfiles,
                          cmsUInt32Number TheIntents[], 
                          cmsHPROFILE     hProfiles[], 
                          cmsBool         BPC[],
                          cmsFloat64Number AdaptationStates[],
                          cmsUInt32Number dwFlags);
    
    // Common helper prototypes that are typically available in the test CMS environment.
    // These declarations are provided to enable test compilation; the actual
    // implementations exist in the test CMS library linked with the test.
    // If a particular prototype differs in your environment, adjust accordingly.

    // Profile creation helpers (expected in many test harnesses)
    cmsHPROFILE cmsCreateGrayProfile(void);
    cmsHPROFILE cmsCreate_sRGBProfile(void);

    // Utility: free a pipeline
    void cmsPipelineFree(cmsContext ContextID, cmsPipeline* Lut);
    // In some environments, cmsGetColorSpace and cmsSigGrayData are defined in the library.
    // We will rely on the library's own definitions during linking.
}

// Lightweight non-terminating test assertion helpers
static int g_test_passed = 0;
static int g_test_failed = 0;

// Print a message but do not abort execution on failure
#define EXPECT_TRUE(cond, msg) do { \
    if (cond) { \
        std::cout << "[PASS] " << msg << std::endl; \
        ++g_test_passed; \
    } else { \
        std::cerr << "[FAIL] " << msg << "  (condition: " #cond ")" << std::endl; \
        ++g_test_failed; \
    } \
} while (0)

#define EXPECT_NOT_EQ(a, b, msg) do { \
    if ((a) != (b)) { \
        std::cout << "[PASS] " << msg << std::endl; \
        ++g_test_passed; \
    } else { \
        std::cerr << "[FAIL] " << msg << "  (values are equal: " #a " == " #b ")" << std::endl; \
        ++g_test_failed; \
    } \
} while (0)

// Helper to print a short summary at the end
static void print_summary() {
    std::cout << "TEST SUMMARY: " << g_test_passed << " passed, " << g_test_failed << " failed." << std::endl;
}

// Test 1: Gray-Gray endpoints should take the "gray at ends" branch and return a non-null pipeline.
// Rationale: If both ends are Gray, MyNewIntent should allocate a pipeline and insert an identity stage.
static void test_MyNewIntent_GrayEnds_ReturnsNonNull() {
    std::cout << "Running test: MyNewIntent_GrayEnds_ReturnsNonNull" << std::endl;

    // Prepare inputs
    cmsUInt32Number nProfiles = 2;
    cmsUInt32Number TheIntents[2] = { /* INTENT_DECEPTIVE, INTENT_PERCEPTUAL */  INTENT_DECEPTIVE, INTENT_PERCEPTUAL };
    cmsHPROFILE hProfiles[2];
    cmsBool BPC[2] = { 0, 0 };
    cmsFloat64Number AdaptationStates[2] = { 0.0, 0.0 };
    cmsContext ContextID = nullptr;
    cmsUInt32Number dwFlags = 0;

    // Create two gray profiles (gray at ends)
    hProfiles[0] = cmsCreateGrayProfile();
    hProfiles[1] = cmsCreateGrayProfile();

    // Call the function under test
    cmsPipeline* Result = MyNewIntent(ContextID, nProfiles, TheIntents, hProfiles, BPC, AdaptationStates, dwFlags);

    // Assertions
    EXPECT_TRUE(Result != nullptr, "MyNewIntent should return a non-NULL pipeline for gray ends.");
    
    // Cleanup
    if (Result != nullptr) {
        cmsPipelineFree(ContextID, Result);
        Result = nullptr;
    }
    // Free created profiles if the environment requires it (best-effort)
    // Some environments automatically free on program exit; guard with safe checks.
    // If your test harness provides cmsCloseProfile, call it here.
    // Example (uncomment if available): cmsCloseProfile(hProfiles[0]); cmsCloseProfile(hProfiles[1]);
}

// Test 2: Non-gray end profiles should take the non-gray branch and return a pipeline
// via _cmsDefaultICCintents, which should also yield a non-NULL Result in a properly provisioned test environment.
static void test_MyNewIntent_NonGrayEnds_ReturnsNonNull() {
    std::cout << "Running test: MyNewIntent_NonGrayEnds_ReturnsNonNull" << std::endl;

    // Prepare inputs
    cmsUInt32Number nProfiles = 2;
    cmsUInt32Number TheIntents[2] = { /* INTENT_PERCEPTUAL, INTENT_DECEPTIVE */  INTENT_PERCEPTUAL, INTENT_DECEPTIVE };
    cmsHPROFILE hProfiles[2];
    cmsBool BPC[2] = { 0, 0 };
    cmsFloat64Number AdaptationStates[2] = { 0.0, 0.0 };
    cmsContext ContextID = nullptr;
    cmsUInt32Number dwFlags = 0;

    // Create two non-gray profiles (e.g., sRGB for non-gray end)
    hProfiles[0] = cmsCreate_sRGBProfile();
    hProfiles[1] = cmsCreate_sRGBProfile();

    // Call the function under test
    cmsPipeline* Result = MyNewIntent(ContextID, nProfiles, TheIntents, hProfiles, BPC, AdaptationStates, dwFlags);

    // Assertions
    EXPECT_TRUE(Result != nullptr, "MyNewIntent should return a non-NULL pipeline for non-gray ends.");

    // Cleanup
    if (Result != nullptr) {
        cmsPipelineFree(ContextID, Result);
        Result = nullptr;
    }
    // Free created profiles if the environment provides a close/free function
    // Example: cmsCloseProfile(hProfiles[0]); cmsCloseProfile(hProfiles[1]); 
}

// Entry point for the test runner
int main() {
    std::cout << "Starting MyNewIntent unit tests (CMS pipeline path)..." << std::endl;

    // Run tests
    test_MyNewIntent_GrayEnds_ReturnsNonNull();
    test_MyNewIntent_NonGrayEnds_ReturnsNonNull();

    // Final summary
    print_summary();

    // Return value: number of failed tests (non-zero indicates failure)
    return (g_test_failed > 0) ? 1 : 0;
}
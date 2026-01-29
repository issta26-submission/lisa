// Test suite for cmsInt32Number CheckMutexPlugin(void)
// This test suite is written for C++11 compilation, without GTest, and
// expects the project to be linkable against the LittleCMS-like runtime
// that provides CheckMutexPlugin and its dependencies.
//
// Explanatory notes:
// - The focal method CheckMutexPlugin is exercised to verify it returns 1
//   when the output matches the input (as per the original implementation).
// - Due to the opaque nature of the plugin system and external dependencies,
//   we focus on validating the return value semantics and basic stability
//   (no crashes) across multiple invocations.
// - We avoid private/internal details and rely on the public C interface.
// - All tests are non-terminating in the sense that we do not forcibly abort
//   on a failure; instead we report failures and return a non-zero exit code.

#include <iostream>
#include <string>
#include <testcms2.h>


// Declaration of the focal C function from the project under test.
// It is assumed to be exposed with C linkage.
extern "C" int CheckMutexPlugin(void); // cmsInt32Number CheckMutexPlugin(void);

//
// Candidate Keywords (for test planning reference):
// - MutexPluginSample, cmsPluginTHR, WatchDogContext, DupContext
// - cmsBuildGamma, cmsCreateLinearizationDeviceLinkTHR, cmsCreateTransformTHR
// - TYPE_GRAY_8, cmsDoTransform, In[4], Out[4]
// - cmsFreeToneCurve, cmsCloseProfile, cmsDeleteTransform, cmsDeleteContext
// - Output equality vs. input (Out[i] == In[i])
// - Context lifecycle (ctx, cpy, cpy2) and resource cleanup
// - 4-sample transform window (i = 0..3)
// - INTENT_PERCEPTUAL, gamma 1.0 (identity transform)
//
// Test 1: Validate that CheckMutexPlugin returns 1 under normal operation
static bool test_CheckMutexPlugin_ReturnsOne() {
    // This test verifies the "success" path where Out matches In after the transform.
    // If the environment or plugin changes cause a mismatch, this will fail.
    int result = CheckMutexPlugin();
    if (result != 1) {
        std::cerr << "[CheckMutexPlugin] Expected 1 (success path), got: " << result << std::endl;
        return false;
    }
    return true;
}

// Test 2: Validate that repeated invocations do not crash and return a valid
// value (0 or 1). This guards against crashes and ensures stable behavior over time.
static bool test_CheckMutexPlugin_NoCrashOnMultipleCalls() {
    for (int iter = 0; iter < 5; ++iter) {
        int result = CheckMutexPlugin();
        if (result != 0 && result != 1) {
            std::cerr << "[CheckMutexPlugin] Invalid return (" << result
                      << ") on iteration " << iter << std::endl;
            return false;
        }
    }
    return true;
}

// Test runner
int main() {
    int failures = 0;

    std::cout << "==================== CheckMutexPlugin Test Suite ====================\n";

    // Run Test 1
    std::cout << "Test 1: Validate return value is 1 under normal operation...\n";
    if (test_CheckMutexPlugin_ReturnsOne()) {
        std::cout << "PASSED: Test_CheckMutexPlugin_ReturnsOne\n";
    } else {
        std::cout << "FAILED: Test_CheckMutexPlugin_ReturnsOne\n";
        ++failures;
    }

    // Run Test 2
    std::cout << "Test 2: Validate stability over multiple invocations (no crash)...\n";
    if (test_CheckMutexPlugin_NoCrashOnMultipleCalls()) {
        std::cout << "PASSED: Test_CheckMutexPlugin_NoCrashOnMultipleCalls\n";
    } else {
        std::cout << "FAILED: Test_CheckMutexPlugin_NoCrashOnMultipleCalls\n";
        ++failures;
    }

    // Summary
    if (failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << failures << " TEST(S) FAILED\n";
        return 1;
    }
}
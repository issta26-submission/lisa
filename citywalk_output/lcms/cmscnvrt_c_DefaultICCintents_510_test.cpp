/*
Unit Test Suite for cmsPipeline* DefaultICCintents(...)
Goal:
- Provide a compact, comprehensible test suite for the focal method DefaultICCintents
- Avoid GTest; implement a tiny in-file test harness with non-terminating assertions
- Focus on Step 1 (Program Understanding) by measuring critical execution paths that are safe to exercise (early exit on zero profiles)
- Use only standard library and the CMS/LittleCMS API provided by the project
Notes:
- The tests rely on the actual LittleCMS library being available at link time.
- Only safe, deterministic behavior is exercised here (early-return path when nProfiles == 0).
- This test suite emphasizes a single robust test due to the complexity and external dependencies of the full CAM workflow in DefaultICCintents.
- The test harness below provides minimal infrastructure to report test outcomes while continuing execution (non-terminating assertions).
*/

#include <cstring>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Include the LittleCMS headers for types and functions

// Declare the focal function in an extern "C" block to avoid name mangling when linking
extern "C" cmsPipeline* DefaultICCintents(
    cmsContext       ContextID,
    cmsUInt32Number  nProfiles,
    cmsUInt32Number  TheIntents[],
    cmsHPROFILE      hProfiles[],
    cmsBool          BPC[],
    cmsFloat64Number AdaptationStates[],
    cmsUInt32Number  dwFlags);

// Lightweight test framework (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_passed = 0;

// Report a test expectation without aborting the test run
#define TEST_EXPECT(cond, msg) do {                       \
    ++g_tests_run;                                         \
    if (cond) { g_tests_passed++; }                       \
    else {                                                \
        std::cerr << "Test failed: " << msg << std::endl; \
    }                                                     \
} while (0)


// Test 1: DefaultICCintents should immediately return NULL when nProfiles == 0
void test_DefaultICCintents_ZeroProfiles_ReturnsNull()
{
    // Arrange
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (ctx == nullptr) {
        // If context creation fails, report and skip the test
        std::cerr << "Warning: cmsCreateContext returned NULL. Skipping test_DefaultICCintents_ZeroProfiles_ReturnsNull." << std::endl;
        return;
    }

    // Prepare inputs for the call
    cmsUInt32Number TheIntents[1] = { 0 };
    cmsHPROFILE hProfiles[1] = { nullptr };
    cmsBool BPC[1] = { 0 };
    cmsFloat64Number AdaptationStates[1] = { 0.0 };

    // The function is declared with C linkage; call it
    cmsPipeline* result = DefaultICCintents(ctx,
                                             0,               // nProfiles = 0
                                             TheIntents,
                                             hProfiles,
                                             BPC,
                                             AdaptationStates,
                                             0);              // dwFlags

    // Assert: result must be NULL
    TEST_EXPECT(result == nullptr, "DefaultICCintents should return NULL when nProfiles == 0");

    // Cleanup
    if (result != nullptr) cmsPipelineFree(result);
    cmsCloseContext(ctx);
}


// Entry point to run all tests
int main()
{
    std::cout << "Running unit tests for DefaultICCintents (cmsPipeline* DefaultICCintents) ..." << std::endl;

    test_DefaultICCintents_ZeroProfiles_ReturnsNull();

    // Summary
    std::cout << "Tests run: " << g_tests_run << ", Passed: " << g_tests_passed
              << ", Failed: " << (g_tests_run - g_tests_passed) << std::endl;

    // Return non-zero if any test failed
    return (g_tests_run == g_tests_passed) ? 0 : 1;
}
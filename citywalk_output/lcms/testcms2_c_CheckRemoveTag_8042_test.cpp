/*
 * Test suite for the focal method: CheckRemoveTag
 * 
 * Notes:
 * - This test suite is written for C++11 and does not use Google Test or any external
 *   testing framework. It uses a lightweight, non-terminating assertion approach.
 * - The tests directly invoke the focal function CheckRemoveTag() declared via the
 *   project's C header (testcms2.h) and linked against the original library that provides
 *   the Little CMS based CMS API used by the focal method.
 * - Explanatory comments are provided for each unit test to describe the tested behavior.
 *
 * Build guidance (conceptual, adapt to your build system):
 * - Ensure testcms2.h is accessible and declares:
 *     extern "C" cmsInt32Number CheckRemoveTag(void);
 * - Link against the library that provides cmsCreate_sRGBProfileTHR, cmsMLUalloc,
 *   cmsMLUsetASCII, cmsWriteTag, cmsSigDeviceMfgDescTag, cmsMLUfree, cmsCloseProfile, etc.
 * - Compile with a C++11-capable compiler.
 *
 * Important: The test suite aims to validate the normal success path and basic repeatability
 * of the focal function. Due to the heavy dependencies on the external LCMS library, this
 * suite does not mock internal library calls. The false-path coverage relies on the library
 * behavior; in typical usage, the focal path should return 1 as long as the library calls
 * succeed.
 */

#include <iostream>
#include <cstdio>
#include <testcms2.h>


// Include the project header that declares the focal function.
// The header is C; wrap in extern "C" to ensure proper linkage in C++.
extern "C" {
}

// If cmsInt32Number is not defined by testcms2.h for C++ consumers, we rely on the
// header to provide it. The tests expect the focal function to be callable as:
// cmsInt32Number CheckRemoveTag(void);
static inline cmsInt32Number CallCheckRemoveTag() {
    return CheckRemoveTag();
}

// Lightweight test harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define TEST_BEGIN(name) void name()
#define TEST_END(name) \
    /* empty: tests end naturally */ \

// Non-terminating assertion: equality
#define ASSERT_EQ(expected, actual, message) do { \
    ++g_tests_run; \
    if ((expected) == (actual)) { \
        ++g_tests_passed; \
        std::cout << "[PASS] " << (message) << "\n"; \
    } else { \
        ++g_tests_failed; \
        std::cerr << "[FAIL] " << (message) \
                  << " | Expected: " << (expected) \
                  << ", Actual: " << (actual) \
                  << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
    } \
} while(false)

// Overload for boolean condition
#define ASSERT_TRUE(cond, message) do { \
    ++g_tests_run; \
    if (cond) { \
        ++g_tests_passed; \
        std::cout << "[PASS] " << (message) << "\n"; \
    } else { \
        ++g_tests_failed; \
        std::cerr << "[FAIL] " << (message) \
                  << " | Condition is false at " << __FILE__ << ":" << __LINE__ << "\n"; \
    } \
} while(false)

// Test 1: Normal path - ensure CheckRemoveTag returns non-zero (expected 1 in project)
TEST_BEGIN(Test_CheckRemoveTag_NormalPath) {
    // Call the focal function and verify the expected positive result.
    cmsInt32Number res = CallCheckRemoveTag();
    ASSERT_EQ(1, res, "CheckRemoveTag should return 1 on normal path");
}
TEST_END(Test_CheckRemoveTag_NormalPath)

// Test 2: Repeatability - calling CheckRemoveTag multiple times should not crash and should
// still return a successful result (1) in the normal environment.
// This helps exercise the function's cleanup/consistency across multiple invocations.
TEST_BEGIN(Test_CheckRemoveTag_RepeatCalls) {
    cmsInt32Number res1 = CallCheckRemoveTag();
    ASSERT_EQ(1, res1, "First invocation of CheckRemoveTag should return 1");

    cmsInt32Number res2 = CallCheckRemoveTag();
    ASSERT_EQ(1, res2, "Second invocation of CheckRemoveTag should return 1");
}
TEST_END(Test_CheckRemoveTag_RepeatCalls)


// Optional: you can add more tests here if you have additional non-mocked scenarios.
// For example, tests that ensure no crash when called in quick succession, or
// tests that validate behavior with different build configurations (if supported).


// Main entry point for the test suite
int main() {
    std::cout << "Starting CheckRemoveTag test suite (non-terminating assertions)..." << std::endl;

    // Run tests
    Test_CheckRemoveTag_NormalPath();
    Test_CheckRemoveTag_RepeatCalls();

    // Summary
    std::cout << "\nTest Summary: "
              << g_tests_passed << " passed, "
              << g_tests_failed << " failed, "
              << g_tests_run << " total." << std::endl;

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}
// Test suite for the focal method Create_AboveRGB(void) in testcms2.c
// This test harness uses a lightweight, non-terminating assertion approach
// suitable for C++11 environments without Google Test.
//
// Notes:
// - The tests directly exercise the focal function: Create_AboveRGB
// - We rely on the Little CMS API (lcms2.h) to handle profiles and resources.
// - No private members are accessed; all tests operate via the public API.
// - The tests are designed to be executed from main() as a bare-bones test runner.

#include <iostream>
#include <lcms2.h>
#include <cstdio>
#include <testcms2.h>


// Include Little CMS C API
extern "C" {
}

// Forward declaration of focal function (C linkage)
extern "C" cmsHPROFILE Create_AboveRGB(void);

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Expectation helpers
#define EXPECT_TRUE(cond, msg) \
    do { \
        ++g_total_tests; \
        if (!(cond)) { \
            ++g_failed_tests; \
            std::cerr << "[EXPECT_TRUE FAILED] " << (msg) << std::endl; \
        } \
    } while (0)

#define EXPECT_NOT_NULL(ptr, msg) \
    do { \
        ++g_total_tests; \
        if ((ptr) == nullptr) { \
            ++g_failed_tests; \
            std::cerr << "[EXPECT_NOT_NULL FAILED] " << (msg) << std::endl; \
        } \
    } while (0)

// Step 2: Unit Test Generation
// Core Candidate Keywords extracted from the focal method
// - Curve (3 tone curves), cmsBuildGamma, cmsWhitePointFromTemp, cmsCreateRGBProfileTHR
// - DbgThread() (debug context provider), cmsFreeToneCurve, cmsHPROFILE
//
// Tests implemented:
// 1) Create_AboveRGB_ReturnsNonNull: ensures the function returns a valid profile handle.
// 2) Create_AboveRGB_TwoConsecutiveDistinct: ensures multiple calls return distinct profiles.
// 3) Create_AboveRGB_MultipleRuns: stress-test with several iterations to ensure stability.

// Test 1: The function should return a non-null profile handle
// Rationale: Validates successful construction of the RGB profile using gamma curve,
// white point, and primaries as defined in the focal method.
void test_CreateAboveRGB_ReturnsNonNull(void) {
    cmsHPROFILE h = Create_AboveRGB();
    EXPECT_NOT_NULL(h, "Create_AboveRGB should return a non-null profile handle");
    if (h) {
        cmsCloseProfile(h);
    }
}

// Test 2: Consecutive calls should yield distinct profiles
// Rationale: Ensures that each invocation creates a separate profile object
// and not a shared or cached instance.
void test_CreateAboveRGB_TwoConsecutiveDistinct(void) {
    cmsHPROFILE h1 = Create_AboveRGB();
    cmsHPROFILE h2 = Create_AboveRGB();

    EXPECT_NOT_NULL(h1, "First Create_AboveRGB() returned non-null");
    EXPECT_NOT_NULL(h2, "Second Create_AboveRGB() returned non-null");

    bool distinct = (h1 != h2);
    if (h1) cmsCloseProfile(h1);
    if (h2) cmsCloseProfile(h2);

    EXPECT_TRUE(distinct, "Two consecutive Create_AboveRGB() calls should produce distinct profiles");
}

// Test 3: Repeated calls across multiple iterations should succeed without crashes
// Rationale: Checks stability and memory handling over repeated creations and deletions.
void test_CreateAboveRGB_MultipleRuns(void) {
    const int iterations = 5;
    bool all_ok = true;
    for (int i = 0; i < iterations; ++i) {
        cmsHPROFILE h = Create_AboveRGB();
        if (!h) {
            all_ok = false;
            break;
        }
        cmsCloseProfile(h);
    }
    EXPECT_TRUE(all_ok, "Multiple successive Create_AboveRGB() calls should always return non-null profiles");
}

// Optional: Run all tests in sequence and report summary
int main(int argc, char* argv[]) {
    (void)argc; (void)argv;

    std::cout << "Starting Create_AboveRGB unit tests...\n";

    test_CreateAboveRGB_ReturnsNonNull();
    test_CreateAboveRGB_TwoConsecutiveDistinct();
    test_CreateAboveRGB_MultipleRuns();

    int total = g_total_tests;
    int failed = g_failed_tests;
    int passed = total - failed;

    std::cout << "Test summary: " << passed << " passed, "
              << failed << " failed, out of " << total << " tests.\n";

    // Return non-zero if any test failed to signal failure to build systems
    return (failed == 0) ? 0 : 1;
}
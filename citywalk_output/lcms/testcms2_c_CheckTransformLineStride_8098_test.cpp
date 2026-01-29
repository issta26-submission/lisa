// C++11 test harness for the focal method: CheckTransformLineStride
// This test suite is designed to exercise the CheckTransformLineStride function
// from testcms2.c (focal method). It uses a non-terminating assertion style
// and a minimal test runner suitable for environments without GTest.
// Note: The tests rely on the presence of Little CMS test resources (e.g., ibm-t61.icc)
// and the testcms2.c environment. Ensure your build links against the same C/C
// library setup used by the project.
//
// Candidate Keywords (extracted from the focal method and its dependencies):
// - cmsHPROFILE pIn, pOut (handles to input/output ICC profiles)
// - cmsHTRANSFORM t (transform handle)
// - buf1, buf2, buf3 (input buffers for RGB8, RGBA8, RGBA16 respectively)
// - out (output buffer to receive transformed data)
// - cmsCreateTransform, cmsDoTransformLineStride, cmsDeleteTransform
// - cmsCreate_sRGBProfile, cmsOpenProfileFromFile, cmsCloseProfile
// - TYPE_RGB_8, TYPE_RGBA_8, TYPE_RGBA_16, INTENT_PERCEPTUAL, cmsFLAGS_COPY_ALPHA
// - memcmp based validation of transformed data
// - early returns on failure paths
// - copy alpha flag, line stride parameters (width, lines, padding)
// - return values: 0 (failure), 1 (success)

#include <cstring>
#include <iostream>
#include <cstdio>
#include <testcms2.h>


// Ensure C linkage for the focal function when declaring from C++
extern "C" int CheckTransformLineStride(void); // Prototype from testcms2.c

// Simple non-terminating test harness
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Basic assertion macro that does not terminate the test run
#define TEST_ASSERT(cond, msg) do { \
    g_tests_run++; \
    if (!(cond)) { \
        std::cerr << "TEST FAILED: " << (msg) \
                  << " (in " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        g_tests_failed++; \
    } else { \
        std::cout << "TEST PASSED: " << (msg) << std::endl; \
    } \
} while (0)

// Test 1: Validate that the focal method returns success (1) under normal conditions.
// This exercises the default successful code path when ICC profiles and buffers
// are available in the environment.
bool test_CheckTransformLineStride_basic(void) {
    int ret = CheckTransformLineStride();
    TEST_ASSERT(ret == 1, "CheckTransformLineStride should return 1 on a valid environment");
    return ret == 1;
}

// Test 2: Validate idempotence by invoking the focal method twice.
// If the function has side effects that would affect subsequent calls, this test
// would catch them. We expect both invocations to succeed (return 1).
bool test_CheckTransformLineStride_idempotent(void) {
    int r1 = CheckTransformLineStride();
    int r2 = CheckTransformLineStride();
    TEST_ASSERT(r1 == 1, "First call to CheckTransformLineStride should succeed");
    TEST_ASSERT(r2 == 1, "Second call to CheckTransformLineStride should also succeed");
    return (r1 == 1) && (r2 == 1);
}

// Optional: Test runner to execute all tests and report summary.
// Note: If the project links a separate main from testcms2.c, ensure only one main exists.
// This test suite provides its own main for environments where testcms2.c main is not linked.
int main(void) {
    std::cout << "Starting test suite for CheckTransformLineStride" << std::endl;

    bool t1 = test_CheckTransformLineStride_basic();
    bool t2 = test_CheckTransformLineStride_idempotent();

    // Aggregate results
    int total_passed = g_tests_run - g_tests_failed;
    std::cout << "Test summary: " << total_passed << " passed, "
              << g_tests_failed << " failed, out of " << g_tests_run << " tests." << std::endl;

    // Return non-zero if any test failed to aid integration with build systems.
    return (g_tests_failed == 0) ? 0 : 1;
}
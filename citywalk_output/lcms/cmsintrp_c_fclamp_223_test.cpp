// Test suite for the focal method fclamp in cmsintrp.c
// This test suite is built for a C/C++11 environment without GoogleTest.
// It uses a small, non-terminating assertion framework to maximize code coverage.

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>
#include <limits>


// Include the CMS internal header to get type definitions like cmsFloat32Number

// Declare the focal function from the C source (linking against cmsintrp.c)
extern "C" cmsFloat32Number fclamp(cmsFloat32Number v);

// Lightweight, non-terminating test assertion macro.
// It records failures but does not abort the test run, allowing full coverage.
static int g_total = 0;
static int g_failed = 0;

#define TEST(cond, msg) do { \
    ++g_total; \
    if(!(cond)) { \
        ++g_failed; \
        std::cerr << "Test failed: " << (msg) \
                  << " [test " << g_total << ", remaining: " << (g_total - g_failed) << "/" << g_total << "]" << std::endl; \
    } \
} while(0)

// Helper: compare two cmsFloat32Number values with a reasonable tolerance.
static bool almost_equal(cmsFloat32Number a, cmsFloat32Number b) {
    // Tolerance chosen to account for typical FP round-off.
    const cmsFloat32Number EPS = 1e-6f;
    return std::fabs(a - b) <= EPS;
}

// Step 2/3: Unit Test Suite for fclamp
// The core behavior of fclamp:
// - If v < 1e-9f or is NaN => return 0.0f
// - If v > 1.0f => return 1.0f
// - Otherwise return v
// We validate true/false branches of predicates and boundary conditions.

static void run_fclamp_tests() {
    // 1) Negative values should clamp to 0.0
    cmsFloat32Number v1 = -0.5f;
    cmsFloat32Number r1 = fclamp(v1);
    TEST(r1 == 0.0f, "fclamp(-0.5f) should return 0.0f");

    // 2) Exact zero should clamp to 0.0
    cmsFloat32Number v2 = 0.0f;
    cmsFloat32Number r2 = fclamp(v2);
    TEST(r2 == 0.0f, "fclamp(0.0f) should return 0.0f");

    // 3) Small positive below threshold (v < 1e-9) should return 0.0
    cmsFloat32Number v3 = 5e-10f; // 0.0000000005
    cmsFloat32Number r3 = fclamp(v3);
    TEST(r3 == 0.0f, "fclamp(5e-10f) should return 0.0f (below threshold)");

    // 4) Boundary value exactly at 1e-9 should be returned as-is (no clamping to 0)
    cmsFloat32Number boundary = 1.0e-9f;
    cmsFloat32Number rb = fclamp(boundary);
    TEST(almost_equal(rb, boundary), "fclamp(1e-9f) should return 1e-9f (boundary case)");

    // 5) Mid-range value should be returned unchanged
    cmsFloat32Number v5 = 0.5f;
    cmsFloat32Number r5 = fclamp(v5);
    TEST(almost_equal(r5, v5), "fclamp(0.5f) should return 0.5f");

    // 6) Exactly 1.0 should be capped at 1.0
    cmsFloat32Number v6 = 1.0f;
    cmsFloat32Number r6 = fclamp(v6);
    TEST(almost_equal(r6, 1.0f), "fclamp(1.0f) should return 1.0f");

    // 7) Above 1.0 should clamp to 1.0
    cmsFloat32Number v7 = 2.0f;
    cmsFloat32Number r7 = fclamp(v7);
    TEST(almost_equal(r7, 1.0f), "fclamp(2.0f) should return 1.0f");

    // 8) NaN should result in 0.0 (isnan(v) path)
    cmsFloat32Number nan = std::numeric_limits<cmsFloat32Number>::quiet_NaN();
    cmsFloat32Number r8 = fclamp(nan);
    TEST(r8 == 0.0f, "fclamp(NaN) should return 0.0f");

    // 9) Very small positive above threshold should return itself
    cmsFloat32Number v9 = 1e-8f; // > 1e-9
    cmsFloat32Number r9 = fclamp(v9);
    TEST(almost_equal(r9, v9), "fclamp(1e-8f) should return itself (above threshold)");
}

int main() {
    // Run the test suite
    run_fclamp_tests();

    // Summary
    std::cout << "fclamp tests completed. Total: " << g_total
              << " Failures: " << g_failed << std::endl;

    // Return non-zero if there were failures to indicate test status to CI systems.
    return (g_failed == 0) ? 0 : 1;
}
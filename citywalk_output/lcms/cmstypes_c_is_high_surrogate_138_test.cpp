// Test suite for cmsinline function is_high_surrogate in cmstypes.c
// This test is written in plain C++11 without using GTest.
// It imports the necessary prototype via extern "C" to ensure proper linkage with the C function.
// The tests cover true/false branches and boundary conditions according to the function logic.

#include <functional>
#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <cstdint>


// Forward declaration of the focal function using C linkage.
// We re-declare minimal type aliases to align with the library's API.
// In the real project, these typedefs come from lcms2_internal.h; including that header is assumed in the build environment.
extern "C" {
    // The CMS library typically defines cmsUInt32Number as a 32-bit unsigned integer
    // and cmsBool as a boolean-like type (often int). We align with that convention.
    typedef uint32_t cmsUInt32Number;
    typedef int cmsBool;

    // Focal function under test
    cmsBool is_high_surrogate(cmsUInt32Number uc);
}

// Simple non-terminating test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void log_pass(const std::string& test_name) {
    std::cout << "PASSED: " << test_name << std::endl;
}

static void log_fail(const std::string& test_name, const std::string& reason = "") {
    std::cerr << "FAILED: " << test_name;
    if (!reason.empty()) std::cerr << " | " << reason;
    std::cerr << std::endl;
    ++g_failed_tests;
}

// Helper to run a single test and record the result
static void run_test(const std::string& name, bool condition) {
    ++g_total_tests;
    if (condition) {
        log_pass(name);
    } else {
        log_fail(name, "Condition evaluated to false");
    }
}

int main() {
    // Step 2: Test Suite for is_high_surrogate
    // Coverage goals:
    // - True branch: uc in 0xD800..0xD8FF should yield true according to the masking logic.
    // - False branch: uc outside that masked range should yield false (e.g., 0xD900..0xDBFF, and other values).
    // - Boundary checks at exact surrogate start (0xD800) and near the end of true region (0xD8FF).
    // - Additional random non-surrogate values to ensure false results.

    // Test 1: True case at lower boundary
    // uc = 0x0000D800 should satisfy (uc & 0xfffffc00) == 0xd800
    {
        const cmsUInt32Number uc = 0x0000D800;
        bool result = is_high_surrogate(uc) != 0;
        run_test("is_high_surrogate(0x0000D800) == true", result);
    }

    // Test 2: True case at upper boundary of the true region
    // uc = 0x0000D8FF should also satisfy the mask condition
    {
        const cmsUInt32Number uc = 0x0000D8FF;
        bool result = is_high_surrogate(uc) != 0;
        run_test("is_high_surrogate(0x0000D8FF) == true", result);
    }

    // Test 3: Loop over all true-range values to ensure all yield true
    // Range 0xD800..0xD8FF (upper 22 bits match 0xD800, lower 10 bits vary)
    {
        bool all_true = true;
        for (uint32_t k = 0; k <= 0x3FF; ++k) {
            cmsUInt32Number uc = 0xD800u | k; // 0xD800..0xD8FF
            if ((is_high_surrogate(uc) == 0)) {
                all_true = false;
                break;
            }
        }
        run_test("is_high_surrogate(0xD800..0xD8FF) all return true", all_true);
    }

    // Test 4: False cases immediately after the true range
    // uc = 0xD900 and uc = 0xDBFF should return false
    {
        const cmsUInt32Number uc1 = 0x0000D900;
        const cmsUInt32Number uc2 = 0x0000DBFF;
        bool r1 = is_high_surrogate(uc1) != 0;
        bool r2 = is_high_surrogate(uc2) != 0;
        run_test("is_high_surrogate(0x0000D900) == false", !r1);
        run_test("is_high_surrogate(0x0000DBFF) == false", !r2);
    }

    // Test 5: Random non-surrogate value
    {
        const cmsUInt32Number uc = 0x00123456;
        bool result = is_high_surrogate(uc) != 0;
        run_test("is_high_surrogate(0x00123456) == false", !result);
    }

    // Test 6: Another explicit non-surrogate value with different upper bits
    {
        const cmsUInt32Number uc = 0xDEADBEEF;
        bool result = is_high_surrogate(uc) != 0;
        run_test("is_high_surrogate(0xDEADBEEF) == false", !result);
    }

    // Summary
    std::cout << "Total tests run: " << g_total_tests
              << ", Failures: " << g_failed_tests << std::endl;

    // Return non-zero if any test failed
    return (g_failed_tests == 0) ? 0 : 1;
}
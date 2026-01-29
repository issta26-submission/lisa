/*
Step 1 - Candidate Keywords (derived from the focal method and its dependencies)
- cmsUInt16Number, cmsFloat32Number
- dst, src, from16toFLT
- 65535.0f as the normalization divisor
- Basic numeric conversion: dst = (src as uint16) / 65535.0f
- No branching inside the focal function (simple assignment)
- The unit tests focus on value conversion and memory semantics of the input/output buffers
*/

/*
This test suite targets the focal method:
void from16toFLT(void* dst, const void* src)
which performs:
    *(cmsFloat32Number*)dst = (*(cmsUInt16Number*)src) / 65535.0f;
It converts a 16-bit unsigned value to a 32-bit float in the [0,1] range.

Notes:
- We assume cmsUInt16Number and cmsFloat32Number are defined in cms headers (lcms2_internal.h).
- The tests are non-terminating (they log failures and continue) to maximize coverage.
- We use a lightweight, self-contained test harness (no Google Test).
- The tests are written for C++11 compatibility and rely only on the standard library plus the provided cms headers.
*/

#include <cstring>
#include <iostream>
#include <cmath>
#include <lcms2_internal.h>
#include <cstdint>


// Include the internal CMS headers that define the types used by the focal function.
// This path matches the provided class dependencies block.

// Ensure C linkage for the focal function in case the header uses C linkage.
extern "C" void from16toFLT(void* dst, const void* src);

/* Simple non-terminating test harness
   Tracks total tests and failures, prints a message per failure, and returns a status. */

// Global counters for the test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Helper: precise float comparison within tolerance
static void expect_float_eq(const float expected, const float actual, const float tol, const char* test_name) {
    ++g_total_tests;
    if (std::fabs(static_cast<double>(expected) - static_cast<double>(actual)) > static_cast<double>(tol)) {
        ++g_failed_tests;
        std::cerr << "[FAIL] " << test_name
                  << " | expected: " << expected
                  << "  actual: " << actual
                  << "  tol: " << tol << "\n";
    }
}

// Helper: integer equality assertion
static void expect_int_eq(const int expected, const int actual, const char* test_name) {
    ++g_total_tests;
    if (expected != actual) {
        ++g_failed_tests;
        std::cerr << "[FAIL] " << test_name
                  << " | expected: " << expected
                  << "  actual: " << actual << "\n";
    }
}

// Helper: mark a successful test (optional, keeps parity with failure logs)
static void pass_test(const char* test_name) {
    ++g_total_tests;
    // We do not log passes to keep output concise, but we count for coverage.
}

// Step 2: Unit Test Generation
// Comprehensive test suite for from16toFLT
// Tests cover edge cases (0 and 65535) and representative mid-values.
// Also tests that src is not modified after the call.
static void test_from16toFLT_basic_values() {
    // 0 -> 0.0
    {
        cmsUInt16Number src = 0;
        cmsFloat32Number dst = 0.0f;
        from16toFLT(&dst, &src);
        expect_float_eq(0.0f, dst, 1e-6f, "from16toFLT: 0 -> 0.0");
        // Ensure source is unchanged
        expect_int_eq(0, static_cast<int>(src), "from16toFLT: src unchanged after 0");
    }

    // 65535 -> 1.0
    {
        cmsUInt16Number src = 65535;
        cmsFloat32Number dst = -123.0f; // sentinel to ensure write occurred
        from16toFLT(&dst, &src);
        expect_float_eq(1.0f, dst, 1e-6f, "from16toFLT: 65535 -> 1.0");
        expect_int_eq(65535, static_cast<int>(src), "from16toFLT: src unchanged after 65535");
    }

    // 32768 -> 32768/65535
    {
        cmsUInt16Number src = 32768;
        cmsFloat32Number dst = 0.0f;
        from16toFLT(&dst, &src);
        float expected = static_cast<float>(32768) / 65535.0f;
        expect_float_eq(expected, dst, 1e-6f, "from16toFLT: 32768 -> 32768/65535");
        expect_int_eq(32768, static_cast<int>(src), "from16toFLT: src unchanged after 32768");
    }

    // 1 -> 1/65535
    {
        cmsUInt16Number src = 1;
        cmsFloat32Number dst = 0.0f;
        from16toFLT(&dst, &src);
        float expected = static_cast<float>(1) / 65535.0f;
        expect_float_eq(expected, dst, 1e-6f, "from16toFLT: 1 -> 1/65535");
        expect_int_eq(1, static_cast<int>(src), "from16toFLT: src unchanged after 1");
    }

    // 65534 -> 65534/65535
    {
        cmsUInt16Number src = 65534;
        cmsFloat32Number dst = 0.0f;
        from16toFLT(&dst, &src);
        float expected = static_cast<float>(65534) / 65535.0f;
        expect_float_eq(expected, dst, 1e-6f, "from16toFLT: 65534 -> 65534/65535");
        expect_int_eq(65534, static_cast<int>(src), "from16toFLT: src unchanged after 65534");
    }

    // Re-run a couple of scenarios to detect any state leakage
    {
        cmsUInt16Number src = 12345;
        cmsFloat32Number dst = 0.0f;
        from16toFLT(&dst, &src);
        float expected = static_cast<float>(12345) / 65535.0f;
        expect_float_eq(expected, dst, 1e-6f, "from16toFLT: 12345 -> 12345/65535");
        // Ensure src still equals 12345
        expect_int_eq(12345, static_cast<int>(src), "from16toFLT: src unchanged after 12345");
    }
}

// Step 3: Test Case Refinement
// Additional coverage-oriented test: ensure tolerance handling on near-boundary values
static void test_from16toFLT_near_boundaries() {
    // Values very close to bounds
    // 65535 should be exactly 1.0
    {
        cmsUInt16Number src = 65535;
        cmsFloat32Number dst = 0.0f;
        from16toFLT(&dst, &src);
        expect_float_eq(1.0f, dst, 1e-6f, "from16toFLT: near-boundary 65535 -> 1.0");
        expect_int_eq(65535, static_cast<int>(src), "from16toFLT: src unchanged after near-boundary 65535");
    }

    // 0 should be exactly 0.0
    {
        cmsUInt16Number src = 0;
        cmsFloat32Number dst = 0.0f;
        from16toFLT(&dst, &src);
        expect_float_eq(0.0f, dst, 1e-6f, "from16toFLT: near-boundary 0 -> 0.0");
        expect_int_eq(0, static_cast<int>(src), "from16toFLT: src unchanged after near-boundary 0");
    }
}

// Step 4: Main runner
static void run_all_tests() {
    test_from16toFLT_basic_values();
    test_from16toFLT_near_boundaries();
}

// Entry point
int main() {
    run_all_tests();

    std::cout << "Total tests executed: " << g_total_tests
              << ", Failures: " << g_failed_tests << std::endl;

    // Non-terminating test: return non-zero if any failure occurred
    return (g_failed_tests != 0) ? 1 : 0;
}
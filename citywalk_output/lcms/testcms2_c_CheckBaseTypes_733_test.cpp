// Test suite for the focal method: CheckBaseTypes in testcms2.c
// This test suite is implemented in C++11 without using GoogleTest.
// It uses a tiny, non-terminating test harness that accumulates failures
// and reports a summary at the end.
//
// Notes:
// - Step 1 (understanding): We focus on the core type-size checks performed by
//   CheckBaseTypes. The relevant dependent components are the cms* typedefs used
//   to validate sizes (e.g., cmsUInt8Number, cmsInt8Number, cmsUInt16Number, ...,
//   cmsFloat32Number, cmsFloat64Number, cmsSignature, cmsU8Fixed8Number,
//   cmsS15Fixed16Number, cmsU16Fixed16Number).
// - Step 2: We attempt to exercise aspects of the function and also verify
//   the sizes through compile-time static assertions. On most platforms these
//   typedefs will have the expected sizes; thus, the true-branch coverage is
//   achievable. False-branch coverage is not practically attainable without changing
//   the typedefs at compile time, which is outside the scope of runtime tests.
// - Step 3: We refine tests using domain knowledge (C++11, non-GTest, static_assert,
//   and a small runtime harness) to maximize coverage where possible.

#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <iomanip>
#include <sstream>


// Bring in the project's test header. This header is C-oriented; we wrap it in extern "C" for linkage.
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// If the header testcms2.h defines the prototypes for the focal function, we guard
// the declaration here to ensure correct linkage when calling from C++.
#ifndef CHECK_BASE_TYPES_DECLARED
#define CHECK_BASE_TYPES_DECLARED
extern "C" cmsInt32Number CheckBaseTypes(void);
extern "C" cmsInt32Number CheckEndianness(void);
#endif

// Lightweight test harness (non-terminating assertions)
namespace TinyTest {
    struct TestCase {
        std::string name;
        std::function<void()> func;
    };

    static bool g_current_test_passed = true;
    static std::vector<std::string> g_failures;

    // Call this to record a failure within a test
    static void fail(const std::string& file, int line, const std::string& msg) {
        std::ostringstream oss;
        oss << file << ":" << line << " - " << msg;
        g_failures.push_back(oss.str());
        g_current_test_passed = false;
    }

    // Assertion macro: non-terminating (does not abort test execution)
    #define TT_ASSERT_TRUE(cond, msg) \
        do { \
            if (!(cond)) { TinyTest::fail(__FILE__, __LINE__, (msg)); } \
        } while (0)

    // Utility for reporting
    static void report_and_reset() {
        if (g_failures.empty()) {
            std::cout << "[PASS] All tests in current suite passed.\n";
        } else {
            std::cout << "[FAIL] Some tests failed:\n";
            for (const auto& f : g_failures) {
                std::cout << "  - " << f << "\n";
            }
        }
        // Reset for next run
        g_failures.clear();
        g_current_test_passed = true;
    }

} // namespace TinyTest

// Convenience macros for defining tests
#define TEST_CASE(name) void name##_TestCase()
#define RUN_TEST(name) \
    do { \
        TinyTest::g_current_test_passed = true; \
        name##_TestCase(); \
        if (!TinyTest::g_current_test_passed) { /* Marked as fail via internal fail() */ } \
        TinyTest::report_and_reset(); \
    } while (0)

// Static assertions to verify type sizes at compile time (Step 3 domain knowledge)
static_assert(sizeof(cmsUInt8Number)  == 1, "cmsUInt8Number must be 1 byte");
static_assert(sizeof(cmsInt8Number)   == 1, "cmsInt8Number must be 1 byte");
static_assert(sizeof(cmsUInt16Number) == 2, "cmsUInt16Number must be 2 bytes");
static_assert(sizeof(cmsInt16Number)  == 2, "cmsInt16Number must be 2 bytes");
static_assert(sizeof(cmsUInt32Number) == 4, "cmsUInt32Number must be 4 bytes");
static_assert(sizeof(cmsInt32Number)  == 4, "cmsInt32Number must be 4 bytes");
static_assert(sizeof(cmsUInt64Number) == 8, "cmsUInt64Number must be 8 bytes");
static_assert(sizeof(cmsInt64Number)  == 8, "cmsInt64Number must be 8 bytes");
static_assert(sizeof(cmsFloat32Number) == 4, "cmsFloat32Number must be 4 bytes");
static_assert(sizeof(cmsFloat64Number) == 8, "cmsFloat64Number must be 8 bytes");
static_assert(sizeof(cmsSignature)     == 4, "cmsSignature must be 4 bytes");
static_assert(sizeof(cmsU8Fixed8Number)  == 2, "cmsU8Fixed8Number must be 2 bytes");
static_assert(sizeof(cmsS15Fixed16Number) == 4, "cmsS15Fixed16Number must be 4 bytes");
static_assert(sizeof(cmsU16Fixed16Number) == 4, "cmsU16Fixed16Number must be 4 bytes");

// Test 1: Validate that CheckBaseTypes returns a non-zero value (true branch for all size checks)
// This test asserts that the platform's type sizes satisfy the expectations encoded in CheckBaseTypes.
// Note: On typical platforms (as used by the library upstream), this should return 1.
TEST_CASE(CheckBaseTypes_ReturnsNonZeroOnSupportedPlatform) {
    cmsInt32Number res = CheckBaseTypes();
    TT_ASSERT_TRUE(res != 0, "CheckBaseTypes should return non-zero (true) on supported platform");
}

// Test 2: Validate behavior of CheckEndianness as an auxiliary determinant
// We ensure the function exists and returns either 0 or 1, indicating a valid boolean-like result.
// This helps exercise additional code paths in the test CMS harness.
TEST_CASE(CheckEndianness_ReturnsBooleanLikeValue) {
    cmsInt32Number endp = CheckEndianness();
    TT_ASSERT_TRUE(endp == 0 || endp == 1, "CheckEndianness should return 0 or 1");
}

// Test 3 (compile-time safety): Static assertions already validate type sizes.
// Additional runtime sanity: verify CheckBaseTypes does not crash when called twice.
TEST_CASE(CheckBaseTypes_RepeatedCallsDoNotCrush) {
    cmsInt32Number r1 = CheckBaseTypes();
    cmsInt32Number r2 = CheckBaseTypes();
    TT_ASSERT_TRUE((r1 != 0) && (r2 != 0), "Repeated calls to CheckBaseTypes should be non-zero on supported platforms");
}

// Main: Run the defined tests and report a consolidated result.
// We intentionally invoke only a subset of tests that exercise the focal method scope.
// Additional tests are included to exercise related helper functions (without GTest).
int main() {
    // In each RUN_TEST block we execute the corresponding test case and report results.
    // Note: The helper uses non-terminating assertions; failures are collected and reported.

    // Run Test 1
    {
        g++_UNUSED; // placeholder to illustrate test-runner flow (no-op in actual compile)
    }
    // Since we cannot rely on macro-wide accumulation across separate function calls
    // in this minimal harness, we invoke tests individually and print results.

    // Test 1
    TinyTest::g_current_test_passed = true;
    CheckBaseTypes_ReturnsNonZeroOnSupportedPlatform();
    TinyTest::report_and_reset();

    // Test 2
    TinyTest::g_current_test_passed = true;
    CheckEndianness_ReturnsBooleanLikeValue();
    TinyTest::report_and_reset();

    // Test 3
    TinyTest::g_current_test_passed = true;
    CheckBaseTypes_RepeatedCallsDoNotCrush();
    TinyTest::report_and_reset();

    // Final summary (informational)
    std::cout << "Test suite completed. Summary above for each test case.\n";
    return 0;
}
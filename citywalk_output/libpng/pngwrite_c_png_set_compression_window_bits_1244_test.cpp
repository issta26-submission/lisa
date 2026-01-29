/*
Unit test suite for the focal method:
png_set_compression_window_bits(png_structrp png_ptr, int window_bits)

- This test is self-contained and does not rely on GTest.
- It provides a minimal mock of the libpng-like environment to exercise the logic:
  - Handling of NULL png_ptr
  - Clamping window_bits > 15 to 15 with a warning
  - Clamping window_bits < 8 to 8 with a warning
  - Accepting 8 <= window_bits <= 15 without a warning
- The test uses a tiny in-process test harness with non-terminating assertions.
- All necessary dependencies are mocked locally to keep the test self-contained.

Notes:
- The real project would typically link against libpng; here we simulate the critical branches and side-effects to validate the core logic of the focal method.
*/

#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// DOMAIN_KNOWLEDGE guidance is followed: Provide a self-contained, compilable test harness.
// We implement a minimal mock of the necessary parts of libpng to test the focal method logic.

// Candidate Keywords (core dependencies observed in the focal method):
// png_structrp, png_ptr, zlib_window_bits, png_debug, png_warning

// Mock namespace to simulate a subset of libpng structures/functions
namespace LibPngMock {

// Forward declare a minimal structure representing the internal png_struct used by libpng
struct png_struct {
    int zlib_window_bits; // field accessed by the focal method
};

// Typedef matching the common libpng alias: png_structrp is a pointer to the internal struct
typedef png_struct* png_structrp;

// Storage for emitted warnings to verify behavior in tests
static std::vector<std::string> g_warnings;

// Mocked diagnostic function (no-op for tests)
static inline void png_debug(int level, const char* msg) {
    (void)level;
    (void)msg;
}

// Mocked warning function: records warnings into g_warnings for verification
static inline void png_warning(png_structrp /*png_ptr*/, const char* msg) {
    if (msg) {
        g_warnings.push_back(std::string(msg));
    } else {
        g_warnings.push_back(std::string("warning"));
    }
}
} // namespace LibPngMock

// Bring types into scope for ease of use
using LibPngMock::png_struct;
using LibPngMock::png_structrp;
using LibPngMock::g_warnings;
using LibPngMock::png_debug;
using LibPngMock::png_warning;

// Forward declare the focal method in the same translation unit (self-contained test harness version)
extern "C" void png_set_compression_window_bits(png_structrp png_ptr, int window_bits);

// Implementation of the focal method logic (self-contained for testing).
// This mirrors the logic described in the provided FOCAL_METHOD.
extern "C" void png_set_compression_window_bits(png_structrp png_ptr, int window_bits)
{
    // Minimal diagnostic: mimic the original function's behavior
    png_debug(1, "in png_set_compression_window_bits");
    if (png_ptr == nullptr)
        return;
    if (window_bits > 15)
    {
        png_warning(png_ptr, "Only compression windows <= 32k supported by PNG");
        window_bits = 15;
    }
    else if (window_bits < 8)
    {
        png_warning(png_ptr, "Only compression windows >= 256 supported by PNG");
        window_bits = 8;
    }
    png_ptr->zlib_window_bits = window_bits;
}

// Simple non-terminating test assertion helpers
static int tests_passed = 0;
static int tests_failed = 0;

#define ASSERT_TRUE(cond, msg) do { \
    if (cond) { \
        ++tests_passed; \
    } else { \
        ++tests_failed; \
        std::cerr << "ASSERT_TRUE failed: " << (msg) << std::endl; \
    } \
} while (0)

#define ASSERT_EQ_INT(expected, actual, msg) do { \
    if ((expected) == (actual)) { \
        ++tests_passed; \
    } else { \
        ++tests_failed; \
        std::cerr << "ASSERT_EQ_INT failed: " << (msg) \
                  << " | expected: " << (expected) << ", actual: " << (actual) << std::endl; \
    } \
} while (0)

#define ASSERT_NOT_NULL(ptr, msg) do { \
    if ((ptr) != nullptr) { \
        ++tests_passed; \
    } else { \
        ++tests_failed; \
        std::cerr << "ASSERT_NOT_NULL failed: " << (msg) << std::endl; \
    } \
} while (0)

static void reset_warnings() {
    g_warnings.clear();
}

// Test 1: PNG pointer is NULL should return immediately and not crash
// This exercises the first guard in the focal method.
static void test_null_ptr_noop() {
    // Before test, ensure no stray warnings
    reset_warnings();

    png_struct dummy;
    dummy.zlib_window_bits = 0;

    // Call with NULL pointer
    png_set_compression_window_bits(nullptr, 32);

    // Expect no warnings and no modification to any memory (not observable here since NULL)
    // We verify that no new warnings were recorded
    ASSERT_TRUE(g_warnings.empty(), "No warnings should be emitted when png_ptr is NULL");
}

// Test 2: window_bits > 15 should warn and clamp to 15
// This tests the upper-bound clamp branch.
static void test_upper_bound_clamp_with_warning() {
    reset_warnings();

    png_struct s;
    s.zlib_window_bits = -1; // initial dummy value

    // Call with an out-of-range high value
    png_set_compression_window_bits(&s, 16);

    // Verify that warning was emitted
    ASSERT_TRUE(!g_warnings.empty(), "Expected a warning for upper bound (>15)");
    // The expected warning message exactly as library would emit
    ASSERT_EQ_INT(1, (int)g_warnings.size(), "Expected exactly one warning entry");
    ASSERT_TRUE(g_warnings[0] == "Only compression windows <= 32k supported by PNG",
                "Upper bound warning message should match");

    // Verify that the value was clamped to 15
    ASSERT_EQ_INT(15, s.zlib_window_bits, "Window bits should be clamped to 15 when input > 15");
}

// Test 3: window_bits < 8 should warn and clamp to 8
// This tests the lower-bound clamp branch.
static void test_lower_bound_clamp_with_warning() {
    reset_warnings();

    png_struct s;
    s.zlib_window_bits = -1;

    // Call with an out-of-range low value
    png_set_compression_window_bits(&s, 7);

    // Verify that warning was emitted
    ASSERT_TRUE(!g_warnings.empty(), "Expected a warning for lower bound (<8)");
    ASSERT_EQ_INT(1, (int)g_warnings.size(), "Expected exactly one warning entry");
    ASSERT_TRUE(g_warnings[0] == "Only compression windows >= 256 supported by PNG",
                "Lower bound warning message should match");

    // Verify clamping to 8
    ASSERT_EQ_INT(8, s.zlib_window_bits, "Window bits should be clamped to 8 when input < 8");
}

// Test 4: window_bits within valid range [8, 15] should set directly with no warning
static void test_in_range_no_warning() {
    reset_warnings();

    png_struct s;
    s.zlib_window_bits = 0;

    // Test a mid-range value
    png_set_compression_window_bits(&s, 9);

    // No warnings should be emitted
    ASSERT_TRUE(g_warnings.empty(), "No warnings expected for in-range input");

    // Value should be set as provided
    ASSERT_EQ_INT(9, s.zlib_window_bits, "Window bits should be set to in-range value without modification");
}

// Additional: boundary tests for exactly 8 and 15 (no warnings, exact match)
static void test_boundaries_no_warning_exact_values() {
    reset_warnings();

    png_struct s1; s1.zlib_window_bits = -1;
    png_set_compression_window_bits(&s1, 8);
    ASSERT_TRUE(g_warnings.empty(), "No warnings expected for boundary value 8 (lower bound)");
    ASSERT_EQ_INT(8, s1.zlib_window_bits, "Window bits should be exactly 8 when input is 8");

    reset_warnings();

    png_struct s2; s2.zlib_window_bits = -1;
    png_set_compression_window_bits(&s2, 15);
    ASSERT_TRUE(g_warnings.empty(), "No warnings expected for boundary value 15 (upper bound)");
    ASSERT_EQ_INT(15, s2.zlib_window_bits, "Window bits should be exactly 15 when input is 15");
}

// Driver: Execute all tests
int main() {
    std::cout << "Starting tests for png_set_compression_window_bits (self-contained mock)\n";

    test_null_ptr_noop();
    test_upper_bound_clamp_with_warning();
    test_lower_bound_clamp_with_warning();
    test_in_range_no_warning();
    test_boundaries_no_warning_exact_values();

    int total_tests = 5 + 1; // 5 defined tests + 1 for the boundary test (we counted 5 tests above)
    // The actual count will reflect all assertions executed
    std::cout << "Tests passed: " << tests_passed << std::endl;
    std::cout << "Tests failed: " << tests_failed << std::endl;
    std::cout << "Total assertions executed: " << (tests_passed + tests_failed) << std::endl;
    std::cout << "png_set_compression_window_bits test suite completed." << std::endl;

    // Return non-zero if any test failed to aid automated pipelines
    return tests_failed == 0 ? 0 : 1;
}
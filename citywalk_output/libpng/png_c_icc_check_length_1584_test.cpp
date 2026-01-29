// Lightweight unit test suite for the focal method icc_check_length
// This test is self-contained and does not rely on external testing frameworks.
// It uses a small in-file mock of the dependent function and minimal type aliases
// to simulate the library's interface for the purpose of verification.

#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Domain-specific simplifications to allow self-contained compilation.
// These imitate only the interfaces used by icc_check_length.

typedef void* png_const_structrp;       // opaque pointer placeholder
typedef const char* png_const_charp;    // C-string name
typedef unsigned int png_uint_32;        // 32-bit unsigned (used for profile_length)
typedef void* png_ptr_placeholder;       // opaque png_ptr placeholder (for tests)

// Globals to capture call information for verification
struct PngIccCallRecord {
    void* png_ptr;
    const char* name;
    unsigned int value;
    const char* reason;
} last_icc_profile_error_call = { nullptr, nullptr, 0, nullptr };

// Mock implementation of png_icc_profile_error used by icc_check_length
// Records the arguments and returns a sentinel value (-1) to indicate an error condition.
int png_icc_profile_error(png_const_structrp png_ptr, png_const_charp name,
                          png_uint_32 value, png_const_charp reason)
{
    last_icc_profile_error_call.png_ptr = const_cast<void*>(png_ptr);
    last_icc_profile_error_call.name = name;
    last_icc_profile_error_call.value = value;
    last_icc_profile_error_call.reason = reason;
    return -1; // sentinel error indicator for testing purposes
}

// The focal method under test.
// Copied here to provide a self-contained test harness without external dependencies.
// In a real scenario, this function would be linked from png.c.
int icc_check_length(png_const_structrp png_ptr, png_const_charp name,
                     png_uint_32 profile_length)
{
    if (profile_length < 132)
        return png_icc_profile_error(png_ptr, name, profile_length, "too short");
    return 1;
}

// Simple non-terminating assertion helper.
// In case of a failed assertion, we record the failure but do not exit,
// allowing multiple tests to run and gather coverage information.
static int tests_failed = 0;
#define ASSERT_TRUE(cond, msg) do { if (!(cond)) { ++tests_failed; std::cerr << "ASSERT FAILED: " << (msg) << std::endl; } } while(0)
#define ASSERT_EQ(expected, actual, msg) do { if (!((expected) == (actual))) { ++tests_failed; std::cerr << "ASSERT_EQ FAILED: " << (msg) << " expected " << (expected) << " but got " << (actual) << std::endl; } } while(0)
#define ASSERT_STR_EQ(expected, actual, msg) do { if ((actual) == nullptr || std::strcmp((expected), (actual)) != 0) { ++tests_failed; std::cerr << "ASSERT_STR_EQ FAILED: " << (msg) << " expected \"" << (expected) << "\" but got \"" << (actual ? actual : "null") << "\"" << std::endl; } } while(0)

// Test 1: Short profile length should trigger the error path.
// Verifies that for profile_length below 132, icc_check_length returns the value
// produced by png_icc_profile_error and that the error callback was invoked with the
// correct parameters.
void test_icc_check_length_short_length_triggers_error() {
    // Reset mock call recorder
    last_icc_profile_error_call = { nullptr, nullptr, 0, nullptr };

    png_const_structrp test_ptr = nullptr;
    const png_const_charp test_name = "short_length_test";
    const unsigned int test_length = 100; // < 132 should trigger error

    int result = icc_check_length(test_ptr, test_name, test_length);

    // Expect the error sentinel value from the mocked png_icc_profile_error
    ASSERT_EQ(-1, result, "icc_check_length should return error sentinel when profile_length < 132");

    // Verify the error recorder captured the correct arguments
    ASSERT_TRUE(last_icc_profile_error_call.png_ptr == test_ptr, "png_ptr should be forwarded to png_icc_profile_error");
    ASSERT_EQ(std::string(test_name), std::string(last_icc_profile_error_call.name ? last_icc_profile_error_call.name : ""), "name should be passed to png_icc_profile_error");
    ASSERT_EQ(test_length, last_icc_profile_error_call.value, "profile_length should be passed to png_icc_profile_error");
    ASSERT_STR_EQ("too short", last_icc_profile_error_call.reason, "reason string should be passed to png_icc_profile_error");
}

// Test 2: Boundary condition at threshold (132) should return success.
// Verifies that when profile_length is exactly 132, icc_check_length returns 1.
void test_icc_check_length_boundary_at_threshold_returns_success() {
    png_const_structrp test_ptr = nullptr;
    const png_const_charp test_name = "threshold_test";
    const unsigned int test_length = 132; // exactly at threshold

    int result = icc_check_length(test_ptr, test_name, test_length);

    ASSERT_EQ(1, result, "icc_check_length should return 1 when profile_length >= 132");
}

// Test 3: Values just below and just above threshold.
// Ensures both branches are exercised around the boundary.
void test_icc_check_length_subtle_boundary_variants() {
    // Just below threshold
    {
        last_icc_profile_error_call = { nullptr, nullptr, 0, nullptr };
        int res = icc_check_length(nullptr, "close_below", 131);
        ASSERT_EQ(-1, res, "131 should trigger error path (below threshold)");
        ASSERT_TRUE(last_icc_profile_error_call.value == 131, "131 should be passed to error function");
    }

    // Just above threshold
    {
        int res = icc_check_length(nullptr, "close_above", 133);
        ASSERT_EQ(1, res, "133 should pass (above threshold)");
    }
}

int main() {
    std::cout << "Starting icc_check_length unit tests (self-contained)..." << std::endl;

    test_icc_check_length_short_length_triggers_error();
    test_icc_check_length_boundary_at_threshold_returns_success();
    test_icc_check_length_subtle_boundary_variants();

    if (tests_failed == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << tests_failed << " test(s) FAILED." << std::endl;
        return 1;
    }
}
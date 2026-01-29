// Minimal C++11 unit tests for the focal method: png_get_rowbytes
// This test suite is designed to be self-contained (no GTest) and uses a small
// in-file test harness with non-terminating assertions to maximize coverage.

#include <string>
#include <iostream>
#include <pngpriv.h>


// -----------------------------------------------------------------------------
// Minimal type stubs to mimic the signature/usage of the focal function.
// We do not rely on the real libpng headers; we only implement what's needed to
// compile and test the focused behavior of png_get_rowbytes.
// -----------------------------------------------------------------------------

typedef unsigned int png_uint_32;
typedef void* png_const_structrp;          // Simplified stand-in for a non-const pointer
struct png_info_s { unsigned int rowbytes; };
typedef const png_info_s* png_const_inforp;

// FOCAL_METHOD under test (reproduced to enable standalone compilation for tests)
png_uint_32 png_get_rowbytes(png_const_structrp png_ptr, png_const_inforp info_ptr)
{
    if (png_ptr != NULL && info_ptr != NULL)
        return info_ptr->rowbytes;
    return 0;
}

// -----------------------------------------------------------------------------
// Lightweight, non-terminating test harness
// - Keeps track of total tests and failures
// - Provides a simple EXPECT_EQ-like assertion that does not terminate on failure
// -----------------------------------------------------------------------------

static int g_total_tests = 0;
static int g_failed_tests = 0;

// Report a failing test without stopping the test suite
void log_failure(const std::string& test_name, const std::string& message)
{
    ++g_total_tests;
    ++g_failed_tests;
    std::cerr << "[FAIL] " << test_name << ": " << message << std::endl;
}

// Report a passing test
void log_pass(const std::string& test_name)
{
    ++g_total_tests;
    std::cout << "[PASS] " << test_name << std::endl;
}

// Convenience macro-like wrappers for equality checks
template <typename A, typename B>
void expect_eq_uint(const std::string& test_name, const A& actual, const B& expected)
{
    if (static_cast<decltype(actual)>(actual) == static_cast<decltype(expected)>(expected)) {
        log_pass(test_name);
    } else {
        log_failure(test_name, "expected " + std::to_string(static_cast<long long>(expected)) +
                                 ", got " + std::to_string(static_cast<long long>(actual)));
    }
}

// Overloads for string-friendly numeric printing if needed
template <>
void log_failure(const std::string& test_name, const std::string& message)
{
    ++g_total_tests;
    ++g_failed_tests;
    std::cerr << "[FAIL] " << test_name << ": " << message << std::endl;
}

// -----------------------------------------------------------------------------
// Test cases for png_get_rowbytes
// Each test includes a comment explaining the scenario and the expected behavior.
// -----------------------------------------------------------------------------

// Test 1: Both pointers are non-null; ensure rowbytes value is returned.
void test_png_get_rowbytes_both_non_null_returns_rowbytes()
{
    // Domain: valid inputs -> return info_ptr->rowbytes
    png_info_s info;
    info.rowbytes = 12345u;

    png_const_inforp info_ptr = &info;
    png_const_structrp png_ptr = reinterpret_cast<png_const_structrp>(0x1); // non-null

    png_uint_32 result = png_get_rowbytes(png_ptr, info_ptr);
    expect_eq_uint("test_png_get_rowbytes_both_non_null_returns_rowbytes",
                   result, static_cast<png_uint_32>(info.rowbytes));
}

// Test 2: png_ptr is NULL but info_ptr is non-null -> expect 0
void test_png_get_rowbytes_png_ptr_null_returns_zero()
{
    png_info_s info;
    info.rowbytes = 65536u;

    png_const_inforp info_ptr = &info;
    png_const_structrp png_ptr = nullptr; // NULL

    png_uint_32 result = png_get_rowbytes(png_ptr, info_ptr);
    expect_eq_uint("test_png_get_rowbytes_png_ptr_null_returns_zero",
                   result, static_cast<png_uint_32>(0u));
}

// Test 3: info_ptr is NULL but png_ptr is non-null -> expect 0
void test_png_get_rowbytes_info_ptr_null_returns_zero()
{
    png_const_structrp png_ptr = reinterpret_cast<png_const_structrp>(0x1); // non-null
    png_const_inforp info_ptr = nullptr; // NULL

    png_uint_32 result = png_get_rowbytes(png_ptr, info_ptr);
    expect_eq_uint("test_png_get_rowbytes_info_ptr_null_returns_zero",
                   result, static_cast<png_uint_32>(0u));
}

// Test 4: Both pointers are NULL -> expect 0
void test_png_get_rowbytes_both_null_returns_zero()
{
    png_const_structrp png_ptr = nullptr;
    png_const_inforp info_ptr = nullptr;

    png_uint_32 result = png_get_rowbytes(png_ptr, info_ptr);
    expect_eq_uint("test_png_get_rowbytes_both_null_returns_zero",
                   result, static_cast<png_uint_32>(0u));
}

// Optional: Run a small additional stress-like test with a large rowbytes value
// Test 5: Verify behavior with a large rowbytes value to ensure no overflow
void test_png_get_rowbytes_large_rowbytes()
{
    png_info_s info;
    info.rowbytes = 0xFFFFFFFFu; // very large value within 32-bit unsigned
    png_const_inforp info_ptr = &info;
    png_const_structrp png_ptr = reinterpret_cast<png_const_structrp>(0x1);

    png_uint_32 result = png_get_rowbytes(png_ptr, info_ptr);
    expect_eq_uint("test_png_get_rowbytes_large_rowbytes", result, static_cast<png_uint_32>(0xFFFFFFFFu));
}

// -----------------------------------------------------------------------------
// Main entry: execute all tests and report a summary
// -----------------------------------------------------------------------------

int main()
{
    // Run tests
    test_png_get_rowbytes_both_non_null_returns_rowbytes();
    test_png_get_rowbytes_png_ptr_null_returns_zero();
    test_png_get_rowbytes_info_ptr_null_returns_zero();
    test_png_get_rowbytes_both_null_returns_zero();
    test_png_get_rowbytes_large_rowbytes();

    // Summary
    std::cout << "\nTest Summary: " << g_total_tests << " tests, "
              << g_failed_tests << " failures." << std::endl;

    // Exit code: 0 if all tests passed, non-zero if any failed
    return (g_failed_tests == 0) ? 0 : 1;
}
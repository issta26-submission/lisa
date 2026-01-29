// High-coverage unit tests for png_set_sBIT using libpng public API
// This test suite is written in plain C++11, without GTest, and uses a lightweight
// test harness that records failures and continues execution to maximize coverage.
// It relies on libpng being available in the test environment.

#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>
#include <png.h>


// libpng public API

// Lightweight test harness
struct TestResult {
    std::string name;
    bool passed;
    std::string message;
};

static std::vector<TestResult> g_results;

// Simple assertion helper that records failures without terminating the test
#define NS_ASSERT(cond, msg) do { if (!(cond)) { g_results.push_back({__func__, false, (msg)}); /* continue */ } } while(0)
#define NS_ASSERT_EQ(a, b, msg) do { if (!((a) == (b))) { \
    char buff[256]; snprintf(buff, sizeof(buff), "%s | Expected %ld, got %ld. %s", __func__, (long)(a), (long)(b), (msg)); \
    g_results.push_back({__func__, false, std::string(buff)}); } } while(0)


// Utility: pretty-print test results
static void report_results() {
    int total = (int)g_results.size();
    int passed = 0;
    for (const auto& r : g_results) if (r.passed) ++passed;
    // If some tests ran multiple checks, we treat each TestResult as a test
    // (each function pushes a single TestResult, or more if desired).
    if (passed == total) {
        std::cout << "ALL TESTS PASSED (" << passed << "/" << total << ")" << std::endl;
    } else {
        std::cout << "TESTS FAILED: " << (total - passed) << " / " << total << std::endl;
    }
    for (const auto& r : g_results) {
        std::cout << (r.passed ? "[OK] " : "[FAIL] ") << r.name << ": " << r.message << std::endl;
    }
}

// Helper: compare two png_color_8 values
static bool color8_equal(const png_color_8& a, const png_color_8& b) {
    return a.red == b.red && a.green == b.green && a.blue == b.blue && a.gray == b.gray;
}


// Test 1: Basic behavior - non-null pointers copy sig_bit values and set valid flag
// This verifies that when png_set_sBIT is called with valid pointers, the signal bit
// values are copied into info_ptr and the PNG_INFO_sBIT flag is set.
static bool test_png_set_sBIT_basic() {
    bool local_ok = true;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_inforp info_ptr = png_create_info_struct(png_ptr);

    if (png_ptr == NULL || info_ptr == NULL) {
        g_results.push_back({"test_png_set_sBIT_basic", false, "Failed to allocate libpng structures."});
        return false;
    }

    // Prepare a sBIT value (red, green, blue, gray)
    png_color_8 sig;
    sig.red = 10; sig.green = 20; sig.blue = 30; sig.gray = 40;

    // Action: set sBIT
    png_set_sBIT(png_ptr, info_ptr, &sig);

    // Retrieve and verify
    png_color_8 actual;
    png_get_sBIT(png_ptr, info_ptr, &actual);

    if (!color8_equal(actual, sig)) {
        g_results.push_back({"test_png_set_sBIT_basic", false,
            "SBIT values were not copied correctly into info_ptr."});
        local_ok = false;
    }

    // Verify that the valid flag was updated to indicate sBIT presence
    png_uint_32 valid_flag = PNG_INFO_sBIT;
    int has_sbit = png_get_valid(png_ptr, info_ptr, valid_flag);
    if (has_sbit == 0) {
        g_results.push_back({"test_png_set_sBIT_basic", false,
            "PNG_INFO_sBIT flag not set in info_ptr.valid after png_set_sBIT."});
        local_ok = false;
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return local_ok;
}


// Test 2: Null png_ptr should be a no-op; existing info_ptr should remain unchanged
// This checks the early-return behavior when png_ptr is NULL.
static bool test_png_set_sBIT_null_png_ptr_noop() {
    bool local_ok = true;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_inforp info_ptr = png_create_info_struct(png_ptr);

    if (png_ptr == NULL || info_ptr == NULL) {
        g_results.push_back({"test_png_set_sBIT_null_png_ptr_noop", false,
            "Failed to allocate libpng structures."});
        return false;
    }

    // Set initial sBIT
    png_color_8 initial;
    initial.red = 5; initial.green = 6; initial.blue = 7; initial.gray = 8;
    png_set_sBIT(png_ptr, info_ptr, &initial);

    // Persist the current values
    png_color_8 before;
    png_get_sBIT(png_ptr, info_ptr, &before);

    // Action: call with NULL png_ptr (should be a no-op)
    png_set_sBIT(NULL, info_ptr, &initial);

    // Retrieve after-noop values
    png_color_8 after;
    png_get_sBIT(png_ptr, info_ptr, &after);

    if (!color8_equal(before, after)) {
        g_results.push_back({"test_png_set_sBIT_null_png_ptr_noop", false,
            "png_set_sBIT modified info_ptr when png_ptr was NULL."});
        local_ok = false;
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return local_ok;
}


// Test 3: Null info_ptr should be a no-op; existing png_ptr should remain usable
// This validates early return when info_ptr is NULL.
static bool test_png_set_sBIT_null_info_ptr_noop() {
    bool local_ok = true;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_inforp info_ptr = png_create_info_struct(png_ptr);

    if (png_ptr == NULL || info_ptr == NULL) {
        g_results.push_back({"test_png_set_sBIT_null_info_ptr_noop", false,
            "Failed to allocate libpng structures."});
        return false;
    }

    // Set initial sBIT
    png_color_8 initial;
    initial.red = 11; initial.green = 12; initial.blue = 13; initial.gray = 14;
    png_set_sBIT(png_ptr, info_ptr, &initial);

    // Persist the current values
    png_color_8 before;
    png_get_sBIT(png_ptr, info_ptr, &before);

    // Action: call with NULL info_ptr (should be a no-op)
    png_set_sBIT(png_ptr, NULL, &initial);

    // Retrieve after-noop values using valid info_ptr
    png_color_8 after;
    png_get_sBIT(png_ptr, info_ptr, &after);

    if (!color8_equal(before, after)) {
        g_results.push_back({"test_png_set_sBIT_null_info_ptr_noop", false,
            "png_set_sBIT modified info_ptr when info_ptr was NULL."});
        local_ok = false;
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return local_ok;
}


// Test 4: Null sig_bit should be a no-op; ensures no dereference happens
// This checks that passing NULL sig_bit does not modify info_ptr.
static bool test_png_set_sBIT_null_sig_bit_noop() {
    bool local_ok = true;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_inforp info_ptr = png_create_info_struct(png_ptr);

    if (png_ptr == NULL || info_ptr == NULL) {
        g_results.push_back({"test_png_set_sBIT_null_sig_bit_noop", false,
            "Failed to allocate libpng structures."});
        return false;
    }

    // Set initial sBIT
    png_color_8 initial;
    initial.red = 21; initial.green = 22; initial.blue = 23; initial.gray = 24;
    png_set_sBIT(png_ptr, info_ptr, &initial);

    // Persist current values
    png_color_8 before;
    png_get_sBIT(png_ptr, info_ptr, &before);

    // Action: pass NULL sig_bit (should be a no-op)
    png_set_sBIT(png_ptr, info_ptr, NULL);

    // Retrieve values to verify no change
    png_color_8 after;
    png_get_sBIT(png_ptr, info_ptr, &after);

    if (!color8_equal(before, after)) {
        g_results.push_back({"test_png_set_sBIT_null_sig_bit_noop", false,
            "png_set_sBIT modified info_ptr when sig_bit pointer was NULL."});
        local_ok = false;
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return local_ok;
}


// Test 5: Verify that sBIT supports a variety of bit patterns across channels
// This ensures that non-trivial color_bit values are preserved.
static bool test_png_set_sBIT_various_values() {
    bool local_ok = true;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_inforp info_ptr = png_create_info_struct(png_ptr);

    if (png_ptr == NULL || info_ptr == NULL) {
        g_results.push_back({"test_png_set_sBIT_various_values", false,
            "Failed to allocate libpng structures."});
        return false;
    }

    // Choose a set of values including a mix of 0 and high values
    png_color_8 sig;
    sig.red = 0; sig.green = 255; sig.blue = 128; sig.gray = 64;

    png_set_sBIT(png_ptr, info_ptr, &sig);

    // Retrieve and verify
    png_color_8 actual;
    png_get_sBIT(png_ptr, info_ptr, &actual);

    if (!color8_equal(actual, sig)) {
        g_results.push_back({"test_png_set_sBIT_various_values", false,
            "SBIT values mismatch for a varied color pattern."});
        local_ok = false;
    }

    // Also verify the sBIT flag is set
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_sBIT) == 0) {
        g_results.push_back({"test_png_set_sBIT_various_values", false,
            "PNG_INFO_sBIT flag not set after setting varied sBIT."});
        local_ok = false;
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return local_ok;
}


// Entry point for all tests
int main() {
    // Clear any previous results
    g_results.clear();

    // Run tests
    bool t1 = test_png_set_sBIT_basic();
    g_results.back().passed = t1 && g_results.back().passed ? true : g_results.back().passed;
    if (!t1) {
        // In case the function returns false but still pushes messages, ensure the entry exists
        // (the test function itself pushes messages on failure)
    } else {
        g_results.push_back({"test_png_set_sBIT_basic", true, "OK"});
    }

    bool t2 = test_png_set_sBIT_null_png_ptr_noop();
    if (t2) g_results.back().passed = true; // overwritten by helper if needed
    else {
        // failure recorded inside
        // ensure we have a summary entry
        // already added by test function on failure
    }

    bool t3 = test_png_set_sBIT_null_info_ptr_noop();
    if (t3) g_results.back().passed = true;

    bool t4 = test_png_set_sBIT_null_sig_bit_noop();
    if (t4) g_results.back().passed = true;

    bool t5 = test_png_set_sBIT_various_values();
    if (t5) g_results.back().passed = true;

    // If some tests didn't push a final "OK" entry, ensure each test reports a result.
    // We purposefully push results inside each test; here we simply print.

    // For robustness, ensure at least one result exists:
    if (g_results.empty()) {
        g_results.push_back({"main", true, "No test results produced."});
    }

    report_results();
    return 0;
}
// Test suite for iccjpeg.c focal method: setup_read_icc_profile
// Domain knowledge constrained: C++11, no Google Test, non-terminating assertions.
// This test mocks the underlying libjpeg hook to verify the focal method behavior.

#include <iccjpeg.h>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <cstdint>


// Import C declarations for the focal function and types.
// The iccjpeg.h header is expected to exist in the build environment.
extern "C" {
}

// Global capture variables to verify that the focal method correctly invokes the library callback.
static void *g_last_cinfo = nullptr;
static int g_last_marker = -1;
static unsigned int g_last_len = 0;

// Mock implementation of the external dependency jpeg_save_markers to intercept calls.
// The real library would implement this; here we capture arguments for verification.
extern "C" void jpeg_save_markers(j_decompress_ptr cinfo, int marker, unsigned int data_len)
{
    g_last_cinfo = static_cast<void*>(cinfo);
    g_last_marker = marker;
    g_last_len = data_len;
}

// Helper to reset captured state between tests.
static void reset_capture()
{
    g_last_cinfo = nullptr;
    g_last_marker = -1;
    g_last_len = 0;
}

// Simple non-terminating assertion mechanism.
// It logs a pass/fail message but does not abort, allowing multiple tests to run.
static bool expect_eq_int(int a, int b, const char* msg)
{
    if (a != b) {
        std::cerr << "FAIL: " << msg << " | Expected: " << b << ", Got: " << a << std::endl;
        return false;
    } else {
        std::cout << "PASS: " << msg << std::endl;
        return true;
    }
}

static bool expect_ptr_eq(const void* a, const void* b, const char* msg)
{
    if (a != b) {
        std::cerr << "FAIL: " << msg << " | Expected pointer: " << b << ", Got: " << a << std::endl;
        return false;
    } else {
        std::cout << "PASS: " << msg << std::endl;
        return true;
    }
}

// Test 1: Verify that setup_read_icc_profile calls jpeg_save_markers with ICC_MARKER and 0xFFFF,
// and forwards the provided cinfo pointer to jpeg_save_markers.
static void test_setup_read_icc_profile_calls_marker()
{
    reset_capture();
    void* dummy_ptr = reinterpret_cast<void*>(0x12345678);

    // Invoke the focal method with a dummy j_decompress_ptr.
    setup_read_icc_profile(reinterpret_cast<j_decompress_ptr>(dummy_ptr));

    bool marker_ok = expect_eq_int(g_last_marker, ICC_MARKER, "setup_read_icc_profile should pass ICC_MARKER to jpeg_save_markers");
    bool len_ok = expect_eq_int(static_cast<int>(g_last_len), 0xFFFF, "setup_read_icc_profile should pass 0xFFFF as length to jpeg_save_markers");
    bool ptr_ok = expect_ptr_eq(g_last_cinfo, dummy_ptr, "setup_read_icc_profile should pass the original cinfo pointer to jpeg_save_markers");

    (marker_ok && len_ok && ptr_ok) ? std::cout << "TEST PASSED: test_setup_read_icc_profile_calls_marker\n" 
                                      : std::cout << "TEST FAILED: test_setup_read_icc_profile_calls_marker\n";
}

// Test 2: Verify that setup_read_icc_profile handles NULL input gracefully and still
// forwards ICC_MARKER and 0xFFFF to jpeg_save_markers with NULL cinfo.
static void test_setup_read_icc_profile_with_null_input()
{
    reset_capture();

    // Invoke focal method with NULL to simulate absence of cinfo.
    setup_read_icc_profile(nullptr);

    bool marker_ok = expect_eq_int(g_last_marker, ICC_MARKER, "setup_read_icc_profile with NULL should pass ICC_MARKER");
    bool len_ok = expect_eq_int(static_cast<int>(g_last_len), 0xFFFF, "setup_read_icc_profile with NULL should pass 0xFFFF as length");
    bool ptr_ok = expect_ptr_eq(g_last_cinfo, nullptr, "setup_read_icc_profile with NULL should forward NULL as cinfo");

    (marker_ok && len_ok && ptr_ok) ? std::cout << "TEST PASSED: test_setup_read_icc_profile_with_null_input\n" 
                                      : std::cout << "TEST FAILED: test_setup_read_icc_profile_with_null_input\n";
}

// Test 3 (refinement): Call twice with different cinfo values to ensure the last call wins.
static void test_setup_read_icc_profile_multiple_calls_updates_state()
{
    reset_capture();
    void* first_ptr = reinterpret_cast<void*>(0x11110000);
    void* second_ptr = reinterpret_cast<void*>(0x22220000);

    setup_read_icc_profile(reinterpret_cast<j_decompress_ptr>(first_ptr));  // first call
    setup_read_icc_profile(reinterpret_cast<j_decompress_ptr>(second_ptr)); // second call

    bool marker_ok = expect_eq_int(g_last_marker, ICC_MARKER, "multiple calls should keep ICC_MARKER");
    bool len_ok = expect_eq_int(static_cast<int>(g_last_len), 0xFFFF, "multiple calls should keep 0xFFFF length");
    bool ptr_ok = expect_ptr_eq(g_last_cinfo, second_ptr, "multiple calls should reflect the last cinfo pointer passed");

    (marker_ok && len_ok && ptr_ok) ? std::cout << "TEST PASSED: test_setup_read_icc_profile_multiple_calls_updates_state\n" 
                                      : std::cout << "TEST FAILED: test_setup_read_icc_profile_multiple_calls_updates_state\n";
}

// Runner
int main()
{
    std::cout << "Starting tests for setup_read_icc_profile in iccjpeg.c (C++ test harness)\n";

    test_setup_read_icc_profile_calls_marker();
    test_setup_read_icc_profile_with_null_input();
    test_setup_read_icc_profile_multiple_calls_updates_state();

    std::cout << "Tests completed.\n";
    // Note: Non-terminating tests; return non-zero if any test failed by analyzing I/O.
    // For strict CI, you could accumulate a failure count and return non-zero.
    return 0;
}
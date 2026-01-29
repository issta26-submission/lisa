// Unit test suite for the focal method: writepng_cleanup
// This test suite is written in C++11 and does not rely on GTest.
// It mocks the external PNG destruction function to verify behavior.
// The test harness calls the focal function via its declared prototype and
// asserts correct call behavior for true/false branches of the condition.

#include <cstdint>
#include <zlib.h>
#include <iostream>
#include <stdlib.h>
#include <png.h>
#include <writepng.h>


// Minimal stand-in for the production header dependencies.
// We provide a small mock of the types used by writepng_cleanup and the
// external function it calls (png_destroy_write_struct).

// Mock png.h equivalents
typedef void* png_structp;
typedef void* png_infop;
typedef png_structp* png_structpp;
typedef png_infop* png_infopp;

// Forward declaration of the mock PNG destroy function (C linkage).
extern "C" void png_destroy_write_struct(png_structpp png_ptr_ptr, png_infopp info_ptr_ptr);

// Mock mainprog_info structure as expected by writepng_cleanup.
// The real project likely defines this in writepng.h; we provide a minimal
// compatible version for testing purposes.
struct mainprog_info {
    void* png_ptr;
    void* info_ptr;
};

// Forward declaration of the focal function under test (C linkage).
extern "C" void writepng_cleanup(mainprog_info *mainprog_ptr);

// Global state to observe calls to the mocked PNG destruction function.
static int g_destroy_call_count = 0;
static png_structpp g_last_png_ptr_ptr = nullptr;
static png_infopp g_last_info_ptr_ptr = nullptr;

// Mock implementation of png_destroy_write_struct to capture its inputs.
// It does not modify the input pointers to avoid affecting the test state.
extern "C" void png_destroy_write_struct(png_structpp png_ptr_ptr, png_infopp info_ptr_ptr)
{
    ++g_destroy_call_count;
    g_last_png_ptr_ptr = png_ptr_ptr;
    g_last_info_ptr_ptr = info_ptr_ptr;
    // Intentionally avoid mutating *png_ptr_ptr or *info_ptr_ptr to preserve test intent.
}

// Simple non-terminating test assertion helper.
// Prints a message on failure but does not exit the test, allowing multiple tests to run.
#define ASSERT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "[FAIL] " << msg << " (at " << __FILE__ << ":" << __LINE__ << ")\n"; \
            g_total_failures++; \
        } else { \
            std::cout << "[PASS] " << msg << "\n"; \
        } \
    } while (0)

static int g_total_failures = 0;

// Helper to reset mock state before each test
static void reset_mock()
{
    g_destroy_call_count = 0;
    g_last_png_ptr_ptr = nullptr;
    g_last_info_ptr_ptr = nullptr;
}

// Test 1: When both png_ptr and info_ptr are non-null, png_destroy_write_struct should be called
// and be passed non-null pointers corresponding to the original values.
static bool test_both_nonnull()
{
    reset_mock();
    mainprog_info info;
    info.png_ptr = (void*)0x11111111; // non-null dummy pointer
    info.info_ptr = (void*)0x22222222; // non-null dummy pointer

    writepng_cleanup(&info);

    bool called = (g_destroy_call_count == 1);
    bool arg1_nonnull = (g_last_png_ptr_ptr != nullptr && *g_last_png_ptr_ptr != nullptr);
    bool arg2_nonnull = (g_last_info_ptr_ptr != nullptr && *g_last_info_ptr_ptr != nullptr);
    bool arg1_value = (arg1_nonnull && *g_last_png_ptr_ptr == info.png_ptr);
    bool arg2_value = (arg2_nonnull && *g_last_info_ptr_ptr == info.info_ptr);

    // Expose multiple checks to maximize coverage of branches and values.
    bool all_ok = called && arg1_nonnull && arg2_nonnull && arg1_value && arg2_value;
    if (all_ok) {
        std::cout << "[TEST PASSED] test_both_nonnull: png_destroy_write_struct invoked with correct non-null arguments.\n";
        return true;
    } else {
        std::cerr << "[TEST FAILED] test_both_nonnull: "
                  << "called=" << called
                  << " arg1_nonnull=" << arg1_nonnull
                  << " arg2_nonnull=" << arg2_nonnull
                  << " arg1_value=" << arg1_value
                  << " arg2_value=" << arg2_value
                  << "\n";
        return false;
    }
}

// Test 2: When both pointers are null, png_destroy_write_struct should not be called
static bool test_both_null()
{
    reset_mock();
    mainprog_info info;
    info.png_ptr = nullptr;
    info.info_ptr = nullptr;

    writepng_cleanup(&info);

    bool not_called = (g_destroy_call_count == 0);
    if (not_called) {
        std::cout << "[TEST PASSED] test_both_null: png_destroy_write_struct not called when pointers are null.\n";
        return true;
    } else {
        std::cerr << "[TEST FAILED] test_both_null: png_destroy_write_struct was called unexpectedly.\n";
        return false;
    }
}

// Test 3: When png_ptr is null and info_ptr is non-null, png_destroy_write_struct should not be called
static bool test_png_ptr_null_only()
{
    reset_mock();
    mainprog_info info;
    info.png_ptr = nullptr;
    info.info_ptr = (void*)0x33333333;

    writepng_cleanup(&info);

    bool not_called = (g_destroy_call_count == 0);
    if (not_called) {
        std::cout << "[TEST PASSED] test_png_ptr_null_only: no call when png_ptr is null.\n";
        return true;
    } else {
        std::cerr << "[TEST FAILED] test_png_ptr_null_only: png_destroy_write_struct called unexpectedly.\n";
        return false;
    }
}

// Test 4: When info_ptr is null and png_ptr is non-null, png_destroy_write_struct should not be called
static bool test_info_ptr_null_only()
{
    reset_mock();
    mainprog_info info;
    info.png_ptr = (void*)0x44444444;
    info.info_ptr = nullptr;

    writepng_cleanup(&info);

    bool not_called = (g_destroy_call_count == 0);
    if (not_called) {
        std::cout << "[TEST PASSED] test_info_ptr_null_only: no call when info_ptr is null.\n";
        return true;
    } else {
        std::cerr << "[TEST FAILED] test_info_ptr_null_only: png_destroy_write_struct called unexpectedly.\n";
        return false;
    }
}

// Entry point to run all tests and report a summary.
// If this code is integrated into a larger test harness, you can invoke the test
// functions from main() similarly.
int main()
{
    // Execute tests
    test_both_nonnull();
    test_both_null();
    test_png_ptr_null_only();
    test_info_ptr_null_only();

    // Summary
    if (g_total_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << g_total_failures << " TEST(S) FAILED\n";
        return g_total_failures;
    }
}
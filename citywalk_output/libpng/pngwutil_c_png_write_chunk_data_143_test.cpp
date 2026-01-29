// Note: This test suite targets the focal function
//       png_write_chunk_data(png_structrp png_ptr, png_const_bytep data, size_t length)
//       from the provided codebase. It uses lightweight, self-contained
//       test scaffolding (no GTest) and mocks the dependent I/O and CRC
//       routines (png_write_data and png_calculate_crc) to observe behavior.

#include <cstdint>
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <pngpriv.h>
#include <cmath>
#include <cstddef>


// Include the project header to obtain the necessary type declarations
// (e.g., png_structrp, png_const_bytep). Adjust the include path as needed
// in your build environment.

// Forward declare the focal function with C linkage for the C++ test.
// This ensures correct linkage when pngwutil.c is compiled as C.
extern "C" void png_write_chunk_data(png_structrp png_ptr, png_const_bytep data, size_t length);

// Mocked/hooked dependencies to observe and control behavior during tests.
// We provide C-linkage implementations so the linker resolves references from
// png_write_chunk_data (which calls these functions).

extern "C" {

// Global test hooks to verify invocation order and parameters.
static std::vector<std::string> g_event_log;
static const void* g_last_write_ptr = nullptr;
static size_t g_last_write_len = 0;
static const void* g_last_crc_ptr = nullptr;
static size_t g_last_crc_len = 0;

// Mock for the actual data write routine used by png_write_chunk_data.
// Records the call and its arguments for verification.
void png_write_data(png_structrp png_ptr, png_const_bytep data, size_t length)
{
    (void)png_ptr; // Unused in this test harness
    g_event_log.push_back("write");
    g_last_write_ptr = static_cast<const void*>(data);
    g_last_write_len = length;
}

// Mock for the CRC calculation routine used by png_write_chunk_data.
// Records the call and its arguments for verification.
void png_calculate_crc(png_structrp png_ptr, png_const_bytep data, size_t length)
{
    (void)png_ptr; // Unused in this test harness
    g_event_log.push_back("crc");
    g_last_crc_ptr = static_cast<const void*>(data);
    g_last_crc_len = length;
}

} // extern "C"

// Lightweight test framework (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

// EXPECT_TRUE: non-terminating assertion. Logs failure but continues.
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        ++g_failed_tests; \
        std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__ << ": " << (msg) << "\n"; \
    } \
} while(0)

#define EXPECT_EQ(expected, actual, msg) do { \
    if(!((expected) == (actual))) { \
        ++g_failed_tests; \
        std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__ << ": " \
                  << (msg) << " (expected: " << (expected) \
                  << ", actual: " << (actual) << ")\n"; \
    } \
} while(0)

// Utility to reset mock state between tests
static void reset_mocks()
{
    g_event_log.clear();
    g_last_write_ptr = nullptr;
    g_last_write_len = 0;
    g_last_crc_ptr = nullptr;
    g_last_crc_len = 0;
}

// A dummy object to obtain a non-null pointer for png_ptr in tests
static int g_dummy_png_context = 0;
static inline png_structrp make_png_ptr()
{
    // Cast address of a local/static object to the library's expected opaque type
    return reinterpret_cast<png_structrp>(&g_dummy_png_context);
}

// Test 1: Valid data with non-null png_ptr should invoke png_write_data and
//          png_calculate_crc in the correct order with the given data.
static void test_png_write_chunk_data_valid_invokes_io_and_crc_in_order()
{
    // Explanation:
    // - Ensure both branches execute when data is non-null and length > 0.
    // - Verify the order: first "write", then "crc".
    reset_mocks();
    const unsigned char payload[] = { 0x10, 0x20, 0x30, 0x40 };
    png_structrp png_ptr = make_png_ptr();

    png_write_chunk_data(png_ptr, payload, sizeof(payload));

    EXPECT_TRUE(g_event_log.size() == 2, "Expected exactly two mock calls (write, crc)");
    if (g_event_log.size() == 2) {
        EXPECT_TRUE(g_event_log[0] == "write" && g_event_log[1] == "crc",
                    "Expected call order: write -> crc");
    }

    EXPECT_EQ(reinterpret_cast<const void*>(payload), g_last_write_ptr,
              "png_write_data should be called with the correct data pointer");
    EXPECT_EQ(sizeof(payload), g_last_write_len,
              "png_write_data should be called with correct length");
    EXPECT_EQ(reinterpret_cast<const void*>(payload), g_last_crc_ptr,
              "png_calculate_crc should be called with the same data pointer");
    EXPECT_EQ(sizeof(payload), g_last_crc_len,
              "png_calculate_crc should be called with correct length");
}

// Test 2: Null png_ptr should cause no operations
static void test_png_write_chunk_data_null_ptr_no_op()
{
    // Explanation:
    // - When png_ptr is NULL, function should return immediately without
    //   calling png_write_data or png_calculate_crc.
    reset_mocks();
    const unsigned char payload[] = { 0xAA, 0xBB, 0xCC };
    png_write_chunk_data(nullptr, payload, sizeof(payload));

    EXPECT_TRUE(g_event_log.empty(), "No IO/CRC should occur when png_ptr is NULL");
    EXPECT_TRUE(g_last_write_ptr == nullptr, "No write should be recorded");
    EXPECT_TRUE(g_last_crc_ptr == nullptr, "No CRC should be recorded");
}

// Test 3: Non-null data pointer with length > 0 should trigger, but
//          data == NULL should not trigger any calls
static void test_png_write_chunk_data_null_data_behavior()
{
    // Explanation:
    // - data == NULL but length > 0 should not trigger any calls
    reset_mocks();
    png_structrp png_ptr = make_png_ptr();
    png_write_chunk_data(png_ptr, nullptr, 123);

    EXPECT_TRUE(g_event_log.empty(), "No IO/CRC should occur when data is NULL");
}

// Test 4: Non-null data pointer with length == 0 should not trigger any calls
static void test_png_write_chunk_data_zero_length_behavior()
{
    // Explanation:
    // - length == 0 should short-circuit and not call IO/CRC
    reset_mocks();
    const unsigned char payload[] = { 0x01, 0x02, 0x03 };
    png_structrp png_ptr = make_png_ptr();
    png_write_chunk_data(png_ptr, payload, 0);

    EXPECT_TRUE(g_event_log.empty(), "No IO/CRC should occur when length is 0");
}

// Simple main to run tests (no external test framework)
int main()
{
    g_total_tests = 0;
    g_failed_tests = 0;

    // Wrap each test in a try/catch to ensure one failing test doesn't abort others
    auto run_test = [&](void (*test_func)(), const char* name) {
        std::cout << "[RUN] " << name << std::endl;
        try {
            test_func();
        } catch (...) {
            ++g_failed_tests;
            std::cerr << "[ERROR] Exception occurred in test: " << name << std::endl;
        }
        ++g_total_tests;
    };

    // Execute tests
    run_test(test_png_write_chunk_data_valid_invokes_io_and_crc_in_order,
             "test_png_write_chunk_data_valid_invokes_io_and_crc_in_order");
    run_test(test_png_write_chunk_data_null_ptr_no_op,
             "test_png_write_chunk_data_null_ptr_no_op");
    run_test(test_png_write_chunk_data_null_data_behavior,
             "test_png_write_chunk_data_null_data_behavior");
    run_test(test_png_write_chunk_data_zero_length_behavior,
             "test_png_write_chunk_data_zero_length_behavior");

    // Summary
    std::cout << "Total tests: " << g_total_tests
              << ", Failures: " << g_failed_tests << std::endl;

    // Return non-zero if any test failed
    return g_failed_tests ? 1 : 0;
}
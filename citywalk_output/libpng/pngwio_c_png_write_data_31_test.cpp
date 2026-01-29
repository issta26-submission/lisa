// Comprehensive C++11 test suite for png_write_data using a lightweight libpng-based approach.
// This test relies on libpng being available in the environment (png.h, libpng logo linking).
// It exercises both branches: when a valid write function is provided and when write function is NULL.
// The tests are designed to run from main() without a testing framework (no GTest).

#include <setjmp.h>
#include <vector>
#include <iostream>
#include <pngpriv.h>
#include <cstring>
#include <png.h>


// Libpng headers (environment must provide libpng development files)

// Global state used by tests to verify behavior
static jmp_buf g_png_jmpbuf;
static bool g_error_seen = false;
static std::vector<unsigned char> g_written;

// Custom write callback to capture data passed to the write function
static void test_write_callback(png_structp png_ptr, png_bytep data, png_size_t length)
{
    // Append the data written by the PNG writer to our internal buffer
    g_written.insert(g_written.end(), data, data + length);
}

// Custom error handler to capture error signaling from libpng and perform a longjmp
static void test_error_fn(png_structp /*png_ptr*/, png_const_charp /*error_msg*/)
{
    // Mark that an error occurred, and return via longjmp to unwind the call
    g_error_seen = true;
    longjmp(g_png_jmpbuf, 1);
}

// A no-op warning handler (not used by tests, but required by libpng API)
static void test_warn_fn(png_structp /*png_ptr*/, png_const_charp /*warning_msg*/)
{
    // Intentionally do nothing; warnings are not the focus of these tests
}

// Helper macro to print test results
#define LOG_TEST_RESULT(name, passed)                                  \
    do {                                                               \
        if (passed)                                                  \
            std::cout << "[PASS] " << name << std::endl;             \
        else                                                         \
            std::cerr << "[FAIL] " << name << std::endl;             \
    } while (0)

// Test 1: Verify that when a valid write function is installed, png_write_data forwards data correctly.
// - Setup a PNG write struct with a custom write callback.
// - Call png_write_data with a small payload and verify the exact bytes are written.
// - Ensure the input payload is not mutated by the writer.
static void test_png_write_data_calls_user_function()
{
    const char* test_name = "png_write_data should call user-provided write function and forward data unchanged";

    // Reset global state
    g_written.clear();
    g_error_seen = false;
    std::vector<unsigned char> payload = { 0x01, 0x02, 0x03, 0x04, 0x05 };

    // Create a PNG write struct with our error/warning handlers
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, test_error_fn, test_warn_fn);
    if (png_ptr == NULL)
    {
        LOG_TEST_RESULT(test_name, false);
        return;
    }

    // Install our test write callback
    png_set_write_fn(png_ptr, NULL, test_write_callback, NULL);

    // Run: call png_write_data with a non-empty payload
    if (setjmp(g_png_jmpbuf) == 0)
    {
        png_write_data(png_ptr, payload.data(), payload.size());
    }
    // If longjmp occurs here, the test would fail for this case because an error wasn't expected
    bool passed = (g_error_seen == false) && (g_written.size() == payload.size()) &&
                  (std::memcmp(g_written.data(), payload.data(), payload.size()) == 0);
    // Ensure the original payload remains unchanged after the call
    bool payload_unmodified = (payload[0] == 0x01 && payload[1] == 0x02 && payload[2] == 0x03 &&
                               payload[3] == 0x04 && payload[4] == 0x05);

    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
    }

    // Validate both data forwarding and payload integrity
    passed = passed && payload_unmodified;
    LOG_TEST_RESULT(test_name, passed);
}

// Test 2: Verify that when the write function pointer is NULL, png_write_data triggers a libpng error.
// - Expect longjmp to occur and our error handler to signal an error condition.
static void test_png_write_data_null_function()
{
    const char* test_name = "png_write_data with NULL write function should trigger error path";

    // Reset global state
    g_written.clear();
    g_error_seen = false;

    // Payload to attempt to write
    unsigned char payload[] = { 0xAA, 0xBB, 0xCC };

    // Create PNG write struct and explicitly set write callback to NULL via png_set_write_fn
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, test_error_fn, test_warn_fn);
    if (png_ptr == NULL)
    {
        LOG_TEST_RESULT(test_name, false);
        return;
    }

    // Install NULL write function to trigger error path
    png_set_write_fn(png_ptr, NULL, NULL, NULL);

    // Run: call png_write_data; should invoke png_error (via test_error_fn) and longjmp
    bool jumped = false;
    if (setjmp(g_png_jmpbuf) == 0)
    {
        png_write_data(png_ptr, payload, sizeof(payload));
        // If we return here, the error handler did not trigger as expected
        // Treat as failure
        jumped = false;
    }
    else
    {
        // We landed here due to longjmp from the error handler
        jumped = true;
    }

    // Validate that an error occurred and that the error callback was invoked
    bool passed = jumped && g_error_seen;
    if (png_ptr)
        png_destroy_write_struct(&png_ptr, NULL);

    LOG_TEST_RESULT(test_name, passed);
}

// Test 3: Verify that multiple successive png_write_data calls accumulate data correctly.
// - Reuse the same write callback and call png_write_data twice with identical payloads.
// - Confirm that the combined output equals the concatenation of both payloads.
static void test_png_write_data_multiple_calls()
{
    const char* test_name = "png_write_data should support multiple successive writes";

    // Reset global state
    g_written.clear();
    g_error_seen = false;

    unsigned char payload[] = { 0x10, 0x20, 0x30, 0x40 };
    const int repeats = 2;

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, test_error_fn, test_warn_fn);
    if (png_ptr == NULL)
    {
        LOG_TEST_RESULT(test_name, false);
        return;
    }

    png_set_write_fn(png_ptr, NULL, test_write_callback, NULL);

    if (setjmp(g_png_jmpbuf) == 0)
    {
        for (int i = 0; i < repeats; ++i)
            png_write_data(png_ptr, payload, sizeof(payload));
    }
    else
    {
        // In this test, we do not expect errors
        LOG_TEST_RESULT(test_name, false);
        if (png_ptr) png_destroy_write_struct(&png_ptr, NULL);
        return;
    }

    // Validate accumulated data
    bool passed = (g_written.size() == sizeof(payload) * repeats) &&
                  (std::memcmp(g_written.data(), payload, sizeof(payload)) == 0) &&
                  (std::memcmp(g_written.data() + sizeof(payload), payload, sizeof(payload)) == 0);

    if (png_ptr)
        png_destroy_write_struct(&png_ptr, NULL);

    LOG_TEST_RESULT(test_name, passed);
}

// Main: Run all tests and print a summary
int main()
{
    std::cout << "Running png_write_data unit tests (libpng-based)..." << std::endl;

    test_png_write_data_calls_user_function();
    test_png_write_data_null_function();
    test_png_write_data_multiple_calls();

    std::cout << "Unit tests completed." << std::endl;
    // Note: This simple test harness does not return a non-zero exit code on failure.
    // You can adjust to aggregate results if required by your CI.
    return 0;
}
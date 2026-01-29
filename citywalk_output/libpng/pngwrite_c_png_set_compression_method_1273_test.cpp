// Minimal C++11 unit test suite for the focal method:
// png_set_compression_method(png_structrp png_ptr, int method)
// Note: This test harness provides a small, self-contained
// mock of the necessary PNG structures and helper functions
// to exercise the focal function without depending on libpng.

// The test uses a lightweight, non-terminating assertion system.

#include <vector>
#include <string>
#include <functional>
#include <iostream>
#include <pngpriv.h>


// -----------------------------------------------------------------------------
// Mocked dependencies to support the focal function under test
// -----------------------------------------------------------------------------

// Minimal representation of the PNG write structure used by png_set_compression_method
struct png_struct_s {
    int zlib_method; // The field manipulated by the focal method
};

// Public alias used by the focal function signature
typedef struct png_struct_s* png_structrp;

// Mocked helper functions that the focal method calls
static std::string g_last_warning_msg;
static bool g_warning_called = false;

void png_debug(int /*level*/, const char* /*msg*/) {
    // In the real library this would log a internal debugging message.
    // For unit testing we don't need to perform any action here.
    (void)/* suppress unused warning */ 0;
}

void png_warning(png_structrp /*png_ptr*/, const char* msg) {
    // Record that a warning was issued and what the message was.
    g_warning_called = true;
    g_last_warning_msg = (msg != nullptr) ? msg : "";
}

// The focal method under test (re-declared here to create a self-contained unit test)
void png_set_compression_method(png_structrp png_ptr, int method)
{
{
   png_debug(1, "in png_set_compression_method");
   if (png_ptr == NULL)
      return;
   /* This would produce an invalid PNG file if it worked, but it doesn't and
    * deflate will fault it, so it is harmless to just warn here.
    */
   if (method != 8)
      png_warning(png_ptr, "Only compression method 8 is supported by PNG");
   png_ptr->zlib_method = method;
}
}

// -----------------------------------------------------------------------------
// Lightweight test framework (non-terminating assertions)
// -----------------------------------------------------------------------------

static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if (cond) { \
        ++g_tests_passed; \
    } else { \
        ++g_tests_failed; \
        std::cerr << "ASSERTION FAILED: " << (msg) << std::endl; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    ++g_tests_run; \
    if ((a) == (b)) { \
        ++g_tests_passed; \
    } else { \
        ++g_tests_failed; \
        std::cerr << "ASSERTION FAILED: " << (msg) \
                  << " | Expected: " << (b) << " Got: " << (a) << std::endl; \
    } \
} while(0)

// Helper to report final test results
void report_results() {
    std::cout << "Tests run: " << g_tests_run
              << ", Passed: " << g_tests_passed
              << ", Failed: " << g_tests_failed << std::endl;
}

// -----------------------------------------------------------------------------
// Unit Tests for png_set_compression_method
// -----------------------------------------------------------------------------

// Test 1: When png_ptr is NULL, the function should return immediately
// and not attempt to access the pointer or emit warnings.
void test_png_set_compression_method_null_ptr() {
    // Reset global state
    g_warning_called = false;
    g_last_warning_msg.clear();

    // Call with NULL pointer
    png_set_compression_method(nullptr, 5);

    // Expect no warning and no modification of any accessible data (cannot check since png_ptr is NULL)
    EXPECT_TRUE(!g_warning_called, "No warning should be produced when png_ptr is NULL");
}

// Test 2: When method != 8, a warning should be issued and the zlib_method should be updated
void test_png_set_compression_method_warn_on_non8() {
    // Prepare a mock PNG struct with an initial value
    png_struct_s s;
    s.zlib_method = -1;

    // Reset warning state
    g_warning_called = false;
    g_last_warning_msg.clear();

    // Invoke with a non-8 method
    png_set_compression_method(&s, 3);

    // Assertions
    EXPECT_EQ(s.zlib_method, 3, "zlib_method should be updated to the provided non-8 value");
    EXPECT_TRUE(g_warning_called, "Warning should be issued for non-8 compression method");
    EXPECT_EQ(g_last_warning_msg, "Only compression method 8 is supported by PNG",
              "Warning message content should match expected text");
}

// Test 3: When method == 8, no warning should be emitted and zlib_method should be set to 8
void test_png_set_compression_method_no_warn_on_8() {
    // Prepare a mock PNG struct with an initial value
    png_struct_s s;
    s.zlib_method = -1;

    // Reset warning state
    g_warning_called = false;
    g_last_warning_msg.clear();

    // Invoke with the expected value
    png_set_compression_method(&s, 8);

    // Assertions
    EXPECT_EQ(s.zlib_method, 8, "zlib_method should be updated to 8");
    EXPECT_TRUE(!g_warning_called, "No warning should be issued when method is 8");
}

// -----------------------------------------------------------------------------
// Main: Run all tests
// -----------------------------------------------------------------------------

int main() {
    // Run tests
    test_png_set_compression_method_null_ptr();
    test_png_set_compression_method_warn_on_non8();
    test_png_set_compression_method_no_warn_on_8();

    // Report results
    report_results();
    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}
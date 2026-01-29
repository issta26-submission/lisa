// High-coverage unit tests for the focal method: png_warning_parameter_signed
// This test file provides a minimal, self-contained environment to compile
// under C++11 without GoogleTest. It mocks the necessary PNG helpers and
// dependencies to exercise the focal method's behavior, including negative
// handling and formatting in decimal and hex modes.

// Notes:
// - We implement a lightweight testing facade with non-terminating assertions.
// - The tests cover positive and negative values, decimal and hex formats, and zero.
// - We invoke the test functions from main() to satisfy the constraint of not using a test framework.

#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>


// Domain-specific constants and type stubs (minimal, self-contained)
#define PNG_NUMBER_BUFFER_SIZE 64

// Forward-declare the opaque type used by the focal method
struct png_warning_parameters_tag { };
typedef struct png_warning_parameters_tag png_warning_parameters;

// Basic type aliases to resemble the real codebase
typedef char* png_charp;
typedef const char* png_const_charp;
typedef unsigned long long png_alloc_size_t;
typedef int png_int_32;

// Global capture for the last produced warning string (used by tests)
static bool g_last_warning_set = false;
static int  g_last_warning_number = -1;
static std::string g_last_warning_string;
static png_warning_parameters g_none_warning_p; // dummy placeholder

// Lightweight assertion machinery (non-terminating)
static int g_test_failures = 0;

static void test_log_failure(const char* msg, const char* file, int line) {
    ++g_test_failures;
    fprintf(stderr, "Test failure: %s (in %s:%d)\n", msg, file, line);
}

// Macros that mimic EXPECT_* style assertions but do not abort tests
#define EXPECT_TRUE(cond) \
    do { if(!(cond)) test_log_failure("EXPECT_TRUE failed: " #cond, __FILE__, __LINE__); } while(0)

#define EXPECT_EQ(a,b) \
    do { if( (a) != (b) ) { char _buf[128]; snprintf(_buf, sizeof(_buf), "EXPECT_EQ failed: " #a " != " #b); test_log_failure(_buf, __FILE__, __LINE__); } } while(0)

#define EXPECT_STR_EQ(actual, expected) \
    do { if( (actual) != (expected) ) { char _buf[256]; snprintf(_buf, sizeof(_buf), "EXPECT_STR_EQ failed: \"%s\" != \"%s\"", (actual).c_str(), (expected)); test_log_failure(_buf, __FILE__, __LINE__); } } while(0)

#define EXPECT_STR_EQ_CSTR(actual, expected) \
    do { if( std::string(actual) != std::string(expected) ) { char _buf[256]; snprintf(_buf, sizeof(_buf), "EXPECT_STR_EQ failed: \"%s\" != \"%s\"", actual, expected); test_log_failure(_buf, __FILE__, __LINE__); } } while(0)

// Minimal PNG-format number formatter used by the focal method
// Signature mirrors: png_charp png_format_number(png_const_charp start, png_charp end, int format, png_alloc_size_t number)
extern "C" {
static png_charp PNG_FORMAT_NUMBER(png_const_charp start, png_charp end, int format, png_alloc_size_t number);

// The test harness observes the formatted string via a global capture
static png_charp png_format_number(png_const_charp start, png_charp end, int format, png_alloc_size_t number) {
    // Prepare the textual representation of the magnitude (unsigned)
    char tmp[64];
    // Support decimal by default; hex (lowercase) when format == 16
    if (format == 16) {
        // lowercase hex
        // Use 64-bit formatting to handle large magnitudes consistently
        int n = snprintf(tmp, sizeof(tmp), "%llx", (unsigned long long)number);
        (void)n;
    } else {
        int n = snprintf(tmp, sizeof(tmp), "%llu", (unsigned long long)number);
        (void)n;
    }
    size_t mag_len = strlen(tmp);

    // Copy into the provided buffer (start) with an available boundary [start, end)
    // We cast away constness here because the original API writes into the buffer.
    char* dst = const_cast<char*>(start);
    size_t available = 0;
    if (end && dst <= end) {
        available = (size_t)((char*)end - dst);
    }

    size_t max_copy = (available > 0) ? (available - 1) : 0; // reserve space for NUL
    size_t to_copy = (mag_len < max_copy) ? mag_len : max_copy;

    if (to_copy > 0) {
        memcpy(dst, tmp, to_copy);
    }
    // Null-terminate
    if (to_copy < available && (to_copy + 0) < (size_t)(end ? (char*)end - dst : 0)) {
        dst[to_copy] = '\0';
    } else if (available > 0) {
        dst[available - 1] = '\0';
    }

    return dst + to_copy;
}

static png_charp PNG_FORMAT_NUMBER(png_const_charp start, png_charp end, int format, png_alloc_size_t number) {
    // Call the internal helper
    return png_format_number(start, end, format, number);
}

// Minimal PNG warning parameter collector
static void png_warning_parameter(png_warning_parameters p, int number, png_const_charp string) {
    g_last_warning_set = true;
    g_last_warning_number = number;
    g_last_warning_string = string ? std::string(string) : std::string();
}
}

// Forward declaration for the focal method (as provided)
static png_int_32 dummy_value_for_format_tests = 0;

// The focal method under test (as provided, adapted to this test harness)
static void png_warning_parameter_signed(png_warning_parameters p, int number, int format,
    png_int_32 value)
{
{
   png_alloc_size_t u;
   png_charp str;
   char buffer[PNG_NUMBER_BUFFER_SIZE] = {0};
   /* Avoid overflow by doing the negate in a png_alloc_size_t: */
   u = (png_alloc_size_t)value;
   if (value < 0)
      u = ~u + 1;
   str = PNG_FORMAT_NUMBER(buffer, format, u);
   if (value < 0 && str > buffer)
      *--str = '-';
   png_warning_parameter(p, number, str);
}
}

// Test harness state
static void reset_warning_capture() {
    g_last_warning_set = false;
    g_last_warning_number = -1;
    g_last_warning_string.clear();
}

// Helper to run a single test and print summary
static void run_all_tests();

// Individual test cases
static void test_positive_decimal() {
    // Purpose: value > 0, decimal formatting, expect plain digits
    reset_warning_capture();
    png_warning_parameter_signed(g_none_warning_p, 1, 10, 123); // decimal format (10)
    EXPECT_TRUE(g_last_warning_set);
    EXPECT_EQ(g_last_warning_number, 1);
    EXPECT_STR_EQ(g_last_warning_string, "123");
}

static void test_negative_decimal() {
    // Purpose: value < 0, decimal formatting, expect negative sign prepended
    reset_warning_capture();
    png_warning_parameter_signed(g_none_warning_p, 2, 10, -42); // decimal format
    EXPECT_TRUE(g_last_warning_set);
    EXPECT_EQ(g_last_warning_number, 2);
    EXPECT_STR_EQ(g_last_warning_string, "-42");
}

static void test_hex_positive() {
    // Purpose: value > 0, hex formatting, expect lowercase hex digits
    reset_warning_capture();
    png_warning_parameter_signed(g_none_warning_p, 3, 16, 255); // hex format
    EXPECT_TRUE(g_last_warning_set);
    EXPECT_EQ(g_last_warning_number, 3);
    EXPECT_STR_EQ(g_last_warning_string, "ff");
}

static void test_hex_negative() {
    // Purpose: value < 0, hex formatting, expect negative sign with magnitude in hex
    reset_warning_capture();
    png_warning_parameter_signed(g_none_warning_p, 4, 16, -1); // hex format
    EXPECT_TRUE(g_last_warning_set);
    EXPECT_EQ(g_last_warning_number, 4);
    EXPECT_STR_EQ(g_last_warning_string, "-1");
}

static void test_zero_decimal() {
    // Purpose: value == 0, decimal formatting, expect "0"
    reset_warning_capture();
    png_warning_parameter_signed(g_none_warning_p, 5, 10, 0);
    EXPECT_TRUE(g_last_warning_set);
    EXPECT_EQ(g_last_warning_number, 5);
    EXPECT_STR_EQ(g_last_warning_string, "0");
}

static void run_all_tests() {
    test_positive_decimal();
    test_negative_decimal();
    test_hex_positive();
    test_hex_negative();
    test_zero_decimal();

    if (g_test_failures == 0) {
        std::cout << "All tests passed.\n";
    } else {
        std::cout << "Total test failures: " << g_test_failures << "\n";
    }
}

// Main entrypoint to execute tests
int main() {
    // Run all defined tests
    run_all_tests();
    return g_test_failures; // non-zero indicates failing tests
}
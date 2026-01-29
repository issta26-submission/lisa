// Test suite for the focal method: aprintf in gznorm.c
// This test harness is designed for C++11 without GoogleTest.
// It uses non-terminating EXPECT_* style assertions to maximize code coverage.

#include <cstring>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <zlib.h>
#include <stdlib.h>
#include <cstdlib>


// Declare the focal C function to be tested.
// It is defined in gznorm.c and linked during build.
// Cast away const where necessary because the focal signature uses char*.
extern "C" char *aprintf(char *fmt, ...);

// Global counters for test reporting
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Non-terminating assertion helpers (do not exit on failure)
#define EXPECT_TRUE(cond) do { \
    ++g_tests_run; \
    if(!(cond)) { \
        ++g_tests_failed; \
        std::cerr << "[EXPECT_TRUE FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " => " << #cond << "\n"; \
    } \
} while(0)

#define EXPECT_FALSE(cond) do { \
    ++g_tests_run; \
    if(cond) { \
        ++g_tests_failed; \
        std::cerr << "[EXPECT_FALSE FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " => " << #cond << "\n"; \
    } \
} while(0)

#define EXPECT_NULL(ptr) do { \
    ++g_tests_run; \
    if((ptr) != NULL) { \
        ++g_tests_failed; \
        std::cerr << "[EXPECT_NULL FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " => pointer is not NULL\n"; \
    } \
} while(0)

#define EXPECT_NOT_NULL(ptr) do { \
    ++g_tests_run; \
    if((ptr) == NULL) { \
        ++g_tests_failed; \
        std::cerr << "[EXPECT_NOT_NULL FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " => pointer is NULL\n"; \
    } \
} while(0)

#define EXPECT_STR_EQ(expected, actual) do { \
    ++g_tests_run; \
    if((actual) == NULL) { \
        ++g_tests_failed; \
        std::cerr << "[EXPECT_STR_EQ FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " => actual is NULL, expected: \"" << (expected) << "\"\n"; \
    } else if(std::strcmp((expected), (actual)) != 0) { \
        ++g_tests_failed; \
        std::cerr << "[EXPECT_STR_EQ FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " => expected: \"" << (expected) << "\", actual: \"" << (actual) << "\"\n"; \
    } \
} while(0)

// Test 1: Basic formatting with no additional arguments
// Goal: Ensure aprintf correctly handles a simple literal format string.
void test_aprintf_basic_no_args() {
    // Comment: Basic usage with a literal format should return the exact string.
    const char *fmt = "hello";
    char *out = aprintf(const_cast<char*>(fmt));
    EXPECT_NOT_NULL(out);
    if(out) {
        EXPECT_STR_EQ("hello", out);
        free(out);
    }
}

// Test 2: Formatting with a single integer argument
// Goal: Verify integer substitution in the formatted string.
void test_aprintf_with_int() {
    // Comment: Ensure numeric formatting works as expected.
    char *out = aprintf(const_cast<char*>( "value=%d" ), 42);
    EXPECT_NOT_NULL(out);
    if(out) {
        EXPECT_STR_EQ("value=42", out);
        free(out);
    }
}

// Test 3: Formatting with multiple arguments (string, int, char)
// Goal: Validate handling of multiple variadic arguments and order.
void test_aprintf_multiple_args() {
    char *out = aprintf(const_cast<char*>( "A %s %d %c" ), "X", 7, 'Z');
    EXPECT_NOT_NULL(out);
    if(out) {
        EXPECT_STR_EQ("A X 7 Z", out);
        free(out);
    }
}

// Test 4: Formatting with a long string argument
// Goal: Ensure large dynamic content is formatted correctly and allocated memory is managed.
void test_aprintf_long_string_arg() {
    std::string long_str(10000, 'a'); // Large input string
    char *out = aprintf(const_cast<char*>( "long=%s" ), long_str.c_str());
    EXPECT_NOT_NULL(out);
    if(out) {
        // Build expected string in C++ to compare content
        std::string expected = "long=" + long_str;
        EXPECT_STR_EQ(expected.c_str(), out);
        free(out);
    }
}

// Test 5: Edge case - empty string argument
// Goal: Ensure empty strings are handled gracefully within variadic formatting.
void test_aprintf_empty_string_arg() {
    char *out = aprintf(const_cast<char*>( "empty=%s" ), "");
    EXPECT_NOT_NULL(out);
    if(out) {
        EXPECT_STR_EQ("empty=", out);
        free(out);
    }
}

// Optional: Run all tests and report summary
int main(void) {
    test_aprintf_basic_no_args();
    test_aprintf_with_int();
    test_aprintf_multiple_args();
    test_aprintf_long_string_arg();
    test_aprintf_empty_string_arg();

    std::cout << "Test results: " << g_tests_run << " ran, "
              << g_tests_failed << " failed." << std::endl;

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}
/*
Unit test suite for the focal function:
    const char* NoMeta(const char* str)

Behavior:
- If the input string contains the character '%', NoMeta returns the constant string:
  "**** CORRUPTED FORMAT STRING ***"
- Otherwise, it returns the input string pointer unchanged.

Test intent:
- Validate true/false branches of the presence of '%'.
- Ensure edge cases (empty string) are handled.
- Verify pointer equality for non-corrupted inputs.
- Verify content equality for corrupted inputs.

This test suite is written in C++11 without GTest, using a lightweight, non-terminating assertion mechanism
and a simple test runner invoked from main().
The tests call the C function NoMeta via an extern "C" declaration.
*/

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>


// Declare the focal function with C linkage to enable linkage with cmscgats.c
extern "C" const char* NoMeta(const char* str);

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_total_tests; \
    if(!(cond)) { \
        ++g_failed_tests; \
        std::cerr << "[TEST FAILED] " << msg << " (line " << __LINE__ << ")" << std::endl; \
    } \
} while(0)

#define EXPECT_EQ_PTR(a, b, msg) do { \
    ++g_total_tests; \
    if((a) != (b)) { \
        ++g_failed_tests; \
        std::cerr << "[TEST FAILED] " << msg \
                  << " (Expected pointer " << static_cast<const void*>(b) \
                  << ", got " << static_cast<const void*>(a) << ")" << std::endl; \
    } \
} while(0)

#define EXPECT_EQ_STR(a, b, msg) do { \
    ++g_total_tests; \
    if(std::strcmp((a), (b)) != 0) { \
        ++g_failed_tests; \
        std::cerr << "[TEST FAILED] " << msg \
                  << " (Expected \"" << (b) << "\", got \"" << (a) << "\")" << std::endl; \
    } \
} while(0)

static void test_NoMeta_no_percent_returns_original_pointer() {
    // Test: Input with no '%' should return the exact same pointer (no modification)
    const char* s = "HelloWorld";
    const char* r = NoMeta(s);
    EXPECT_TRUE(r == s, "NoMeta should return the same pointer when there is no '%' in input");
}

static void test_NoMeta_percent_in_middle_returns_corrupted_string() {
    // Test: Input containing percent anywhere should return the corrupted format string
    const char* s = "50% off";
    const char* r = NoMeta(s);
    EXPECT_TRUE(r != s, "NoMeta should not return the original pointer when '%' is present");
    EXPECT_EQ_STR(r, "**** CORRUPTED FORMAT STRING ***",
                  "NoMeta should return the corrupted format string constant when '%' is present");
}

static void test_NoMeta_empty_string_returns_same_pointer() {
    // Test: Empty string should be treated as having no '%', thus return same pointer
    const char* s = "";
    const char* r = NoMeta(s);
    EXPECT_TRUE(r == s, "NoMeta should return the same pointer for empty string");
}

static void test_NoMeta_percent_in_first_character() {
    // Test: If the first character is '%', the function should return the corrupted string
    const char s[] = "%abc";
    const char* r = NoMeta(s);
    EXPECT_TRUE(r != s, "NoMeta should detect '%' at first position and return corrupted string");
    EXPECT_EQ_STR(r, "**** CORRUPTED FORMAT STRING ***",
                  "NoMeta should return the corrupted format string when '%' is the first character");
}

static void test_NoMeta_multiple_percent_characters() {
    // Test: String with multiple '%' characters should still return the corrupted string
    const char s[] = "a%bc%def";
    const char* r = NoMeta(s);
    EXPECT_TRUE(r != s, "NoMeta should return corrupted string when multiple '%' are present");
    EXPECT_EQ_STR(r, "**** CORRUPTED FORMAT STRING ***",
                  "NoMeta should return the corrupted format string when multiple '%' are present");
}

static void run_all_tests() {
    test_NoMeta_no_percent_returns_original_pointer();
    test_NoMeta_percent_in_middle_returns_corrupted_string();
    test_NoMeta_empty_string_returns_same_pointer();
    test_NoMeta_percent_in_first_character();
    test_NoMeta_multiple_percent_characters();
}

int main() {
    run_all_tests();
    std::cout << "Total tests run: " << g_total_tests
              << ", Failures: " << g_failed_tests << std::endl;
    // Return non-zero if any test failed to aid integration with build systems
    return (g_failed_tests == 0) ? 0 : 1;
}
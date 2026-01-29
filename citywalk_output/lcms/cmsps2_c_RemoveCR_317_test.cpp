/*
Unit Test Suite for RemoveCR(const char* txt)
- Focus: Validate the behavior of RemoveCR in cmsps2.c
- Key behaviors to cover:
  - Replacing '\n' and '\r' with spaces
  - Handling CRLF sequences (both characters replaced)
  - Truncation behavior when input length > 2047 (due to static 2048 buffer and 2047 copy)
  - Static buffer reuse (same returned pointer across calls)
  - Basic no-change scenarios
- Testing approach:
  - No GoogleTest; use a lightweight in-file test harness with non-terminating assertions
  - Assertions output to stdout on failure, but do not abort tests
  - Access to RemoveCR via extern "C" prototype
*/

#include <lcms2_internal.h>
#include <iostream>
#include <string>
#include <cstring>


// Declaration of the focal function (C linkage as it is defined in cmsps2.c)
extern "C" char* RemoveCR(const char* txt);

// Lightweight test framework (non-terminating assertions)
static int gTotal = 0;
static int gPassed = 0;
static int gFailed = 0;

// Assert that two C-strings are equal
#define ASSERT_STR_EQ(expected, actual) do { \
    ++gTotal; \
    if (std::string((expected)) == std::string((actual))) { \
        ++gPassed; \
    } else { \
        ++gFailed; \
        std::cout << "[FAIL] " << __func__ << ": expected \"" << (expected) \
                  << "\", got \"" << (actual) << "\"\n"; \
    } \
} while (0)

// Assert that two pointers are equal (same buffer instance)
#define ASSERT_PTR_EQ(a, b) do { \
    ++gTotal; \
    if ((a) == (b)) { \
        ++gPassed; \
    } else { \
        ++gFailed; \
        std::cout << "[FAIL] " << __func__ << ": pointer mismatch: " \
                  << static_cast<const void*>(a) << " != " \
                  << static_cast<const void*>(b) << "\n"; \
    } \
} while (0)

// Assert that the length of the C-string equals the expected length
#define ASSERT_STRLEN_EQ(expectedLen, actual) do { \
    ++gTotal; \
    size_t len = std::strlen((actual)); \
    if (len == static_cast<size_t>(expectedLen)) { \
        ++gPassed; \
    } else { \
        ++gFailed; \
        std::cout << "[FAIL] " << __func__ << ": expected strlen " \
                  << (expectedLen) << ", got " << len << "\n"; \
    } \
} while (0)

// Simple true condition assertion
#define ASSERT_TRUE(cond) do { \
    ++gTotal; \
    if (cond) ++gPassed; else { ++gFailed; std::cout << "[FAIL] " << __func__ \
        << ": assertion " #cond " failed\n"; } \
} while (0)

/* Test 1: Basic no-change scenario
   Input contains no CR or LF characters; output should equal input. */
static void test_RemoveCR_noChange() {
    const char* input = "HelloWorld";
    char* out = RemoveCR(input);
    ASSERT_STR_EQ("HelloWorld", out);
}

/* Test 2: Replace newline with a space
   Ensure '\n' is converted to ' ' */
static void test_RemoveCR_replaceNewline() {
    const char* input = "A\nB";
    char* out = RemoveCR(input);
    ASSERT_STR_EQ("A B", out);
}

/* Test 3: Replace carriage return with a space
   Ensure '\r' is converted to ' ' */
static void test_RemoveCR_replaceCR() {
    const char* input = "A\rB";
    char* out = RemoveCR(input);
    ASSERT_STR_EQ("A B", out);
}

/* Test 4: Replace CRLF sequence with two spaces
   '\r' -> ' ', '\n' -> ' ' resulting in two spaces for CRLF */
static void test_RemoveCR_replaceCRLF() {
    const char* input = "A\r\nB";
    char* out = RemoveCR(input);
    ASSERT_STR_EQ("A  B", out);
}

/* Test 5: Mixed CR and LF across a longer string
   Verifies complex replacements including multiple sequences. */
static void test_RemoveCR_mixed() {
    const char* input = "Line1\nLine2\rLine3\r\nLine4";
    char* out = RemoveCR(input);
    ASSERT_STR_EQ("Line1 Line2 Line3  Line4", out);
}

/* Test 6: Buffer reuse validation (static buffer behavior)
   Consecutive calls should return the same buffer address. */
static void test_RemoveCR_bufferReuse() {
    char* first = RemoveCR("First\nLine");
    char* second = RemoveCR("Second");
    ASSERT_PTR_EQ(first, second);
    // Optional: also verify content after reuse
    ASSERT_STR_EQ("First Line", first);
}

/* Test 7: Truncation behavior when input exceeds 2047 chars
   The function copies at most 2047 chars, then terminates. */
static void test_RemoveCR_truncation() {
    // Build a long string of length 2100 with a newline inside
    std::string longInput(2100, 'x');
    longInput[5] = '\n'; // ensure some replacement
    char* out = RemoveCR(longInput.c_str());

    // Output should be exactly 2047 characters long (plus null terminator)
    ASSERT_STRLEN_EQ(2047, out);

    // The newline at index 5 should become a space
    ASSERT_TRUE(out[5] == ' ');

    // The character after the newline should be 'x' (since original after newline is 'x')
    ASSERT_TRUE(out[6] == 'x');
}

/* Run all tests and report summary */
int main() {
    std::cout << "Starting RemoveCR unit tests (C++11, no GTest)…\n";

    test_RemoveCR_noChange();
    test_RemoveCR_replaceNewline();
    test_RemoveCR_replaceCR();
    test_RemoveCR_replaceCRLF();
    test_RemoveCR_mixed();
    test_RemoveCR_bufferReuse();
    test_RemoveCR_truncation();

    std::cout << "Test Summary: Total=" << gTotal
              << ", Passed=" << gPassed
              << ", Failed=" << gFailed << "\n";

    if (gFailed == 0) {
        std::cout << "All RemoveCR tests PASSED.\n";
        return 0;
    } else {
        std::cout << "Some RemoveCR tests FAILED.\n";
        return 1;
    }
}
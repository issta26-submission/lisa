/*
  Minimal C++11 test harness focused on the focal functionality demonstrated by
  testEqualStringsLen in the Unity-based testunity.c.

  Design notes aligned with the requested steps:
  - Step 1 (Program Understanding): The core behavior is to compare two strings for
    equality over the first N characters (N provided as length). The Candidate
    Keywords used here are: testString, strlen, strncmp, "foobar", "foobaz",
    "foo", "", 3, 5.
  - Step 2 (Unit Test Generation): The test suite focuses on the exact scenarios
    exercised by TEST_ASSERT_EQUAL_STRING_LEN and related MESSAGE variant. Because
    we are not depending on GTest or Unity here, we implement lightweight, non-terminating
    checks that mirror the intent of those Unity tests.
  - Step 3 (Test Case Refinement): We provide tests for a positive case (all comparisons
    succeed) and a negative case (inequality within a given length) to cover true/false
    branches. We keep dependencies strictly within the C++11 standard library.

  Notes:
  - This harness uses minimal, non-terminating assertions to maximize test execution.
  - No external dependencies beyond the C++ standard library are used.
  - The tests are intentionally self-contained and do not require Unity or GTest.
*/

#include <unity.h>
#include <cstring>
#include <iostream>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Global counters for simple test reporting
static int gTotalTests = 0;
static int gFailures = 0;

/*
  Lightweight assertion helpers mirroring Unity-like behavior (non-terminating).
  - EXPECT_STRLEN_EQ: asserts that two C-strings are equal for the first len characters.
  - EXPECT_STRLEN_EQ_MSG: same as above but with a custom message on failure.
  - EXPECT_STRLEN_NEQ: asserts that two C-strings are not equal for the first len characters.
*/

// Compare a and b for the first 'len' characters. On mismatch, log a failure.
#define EXPECT_STRLEN_EQ(a, b, len)                                           \
    do {                                                                       \
        if (std::strncmp((a), (b), (len)) != 0) {                           \
            std::cerr << "Failure in test: strings are not equal for length "  \
                      << (len) << ". Left=\"" << (a)                         \
                      << "\" Right=\"" << (b) << "\"\n";                    \
            ++gFailures;                                                    \
        }                                                                     \
    } while (0)

// Variant with a custom message on failure
#define EXPECT_STRLEN_EQ_MSG(a, b, len, msg)                                  \
    do {                                                                        \
        if (std::strncmp((a), (b), (len)) != 0) {                            \
            std::cerr << "Failure in test: " << (msg)                          \
                      << " | Left=\"" << (a)                                   \
                      << "\" Right=\"" << (b) << "\" len=" << (len) << "\n";  \
            ++gFailures;                                                     \
        }                                                                      \
    } while (0)

// Assert not equal for the first 'len' characters
#define EXPECT_STRLEN_NEQ(a, b, len)                                           \
    do {                                                                        \
        if (std::strncmp((a), (b), (len)) == 0) {                            \
            std::cerr << "Failure in test: strings are unexpectedly equal for length " \
                      << (len) << ". Left=\"" << (a)                           \
                      << "\" Right=\"" << (b) << "\"\n";                       \
            ++gFailures;                                                     \
        }                                                                      \
    } while (0)

/* Candidate Keywords (Step 1) are embedded as comments in the code above for traceability. */

// --- Test Case 1: Positive path mirroring testEqualStringsLen in testunity.c ---
// This test mirrors the exact sequence in the focal method:
// TEST_ASSERT_EQUAL_STRING_LEN(testString, testString, strlen(testString));
// TEST_ASSERT_EQUAL_STRING_LEN_MESSAGE("foobar", "foobaz", 5, "fooba isn't fooba");
// TEST_ASSERT_EQUAL_STRING_LEN("foo", testString, 3);
// TEST_ASSERT_EQUAL_STRING_LEN(testString, "foo", 3);
// TEST_ASSERT_EQUAL_STRING_LEN("", "", 3);
static void testEqualStringsLen_cpp_positive(void)
{
    // Candidate string and its derived length
    const char *testString = "foobar";

    // 1) Identical strings with their own length
    EXPECT_STRLEN_EQ(testString, testString, std::strlen(testString));

    // 2) First 5 characters are equal ("fooba" == "fooba")
    EXPECT_STRLEN_EQ_MSG("foobar", "foobaz", 5, "fooba isn't fooba");

    // 3) "foo" vs first 3 chars of testString -> "foo" vs "foo"
    EXPECT_STRLEN_EQ("foo", testString, 3);

    // 4) testString vs "foo" with length 3 -> "foo" vs "foo"
    EXPECT_STRLEN_EQ(testString, "foo", 3);

    // 5) Empty strings with length 3 -> comparing '\0' to '\0'
    // Should be considered equal for all three comparisons
    EXPECT_STRLEN_EQ("", "", 3);
}

// --- Test Case 2: Negative path mirroring a potential failure scenario ---
// This ensures the inequality branch is exercised (first len chars differ).
// We intentionally compare "foo" vs "bar" for length 3 (all three chars differ).
static void testNotEqualStringsLen_cpp_negative(void)
{
    const char *a = "foo";
    const char *b = "bar";
    size_t len = 3;

    // Expect not equal within first 3 chars
    EXPECT_STRLEN_NEQ(a, b, len);
}

// Entry point for running all tests in this file
int main()
{
    std::cout << "Starting lightweight test suite for testEqualStringsLen (C++11 harness)\n";

    // Run positive test (should produce no failures)
    gTotalTests++;
    testEqualStringsLen_cpp_positive();

    // Run negative test (should detect inequality)
    gTotalTests++;
    testNotEqualStringsLen_cpp_negative();

    // Summary
    std::cout << "Test run complete. Total: " << gTotalTests
              << ", Failures: " << gFailures << "\n";

    // Return non-zero if any test failed
    return gFailures;
}
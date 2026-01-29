/*
Unit test suite for is_ICC_signature_char(png_alloc_size_t it)

Step 1: Program Understanding
- Core predicate: is_ICC_signature_char returns true for:
  - space character (32)
  - digits 0-9 (ASCII 48-57)
  - uppercase A-Z (ASCII 65-90)
  - lowercase a-z (ASCII 97-122)
- Implementation effectively checks if 'it' is an ASCII alphanumeric or space.
- Candidate Keywords extracted:
  - 32, 48-57, 65-90, 97-122, ASCII categories, boolean-like return.

Step 2: Unit Test Generation
- Target: is_ICC_signature_char in png.c
- Dependency sketch (for test): use extern "C" to bind to C function.
- Test scope: cover true branches (each of the four groups) and false branches (adjacent boundary values).

Step 3: Test Case Refinement
- Use boundary checks around each range to exercise true/false transitions.
- Ensure non-terminating assertions: implement custom EXPECT-like checks that do not abort on failure.
- Compile under C++11 without GTest; provide a small test harness with a main().
- Access type png_alloc_size_t (typedef assumed to be size_t in typical libpng builds). We declare a compatible alias for testing.

Note: This test expects to link with the C implementation of is_ICC_signature_char (from png.c) via C linkage.

*/

// Minimal test harness and tests for is_ICC_signature_char
#include <iostream>
#include <pngpriv.h>
#include <vector>
#include <cstddef>


// Match the type used by the focal function.
// Libpng commonly uses size_t-like typedefs for allocation sizes.
typedef std::size_t png_alloc_size_t;

// Declare the focal function with C linkage to ensure proper linkage.
// The actual implementation resides in png.c (as provided by the project).
extern "C" int is_ICC_signature_char(png_alloc_size_t it);

// Lightweight non-terminating test assertion helpers
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond) \
    do { \
        ++g_total_tests; \
        if (!(cond)) { \
            ++g_failed_tests; \
            std::cerr << "EXPECT_TRUE failed at " << __FILE__ << ":" << __LINE__ \
                      << " - condition evaluated to false.\n"; \
        } \
    } while (0)

#define EXPECT_FALSE(cond) \
    do { \
        ++g_total_tests; \
        if (cond) { \
            ++g_failed_tests; \
            std::cerr << "EXPECT_FALSE failed at " << __FILE__ << ":" << __LINE__ \
                      << " - condition evaluated to true.\n"; \
        } \
    } while (0)

#define EXPECT_EQ(actual, expected) \
    do { \
        ++g_total_tests; \
        if (!((actual) == (expected))) { \
            ++g_failed_tests; \
            std::cerr << "EXPECT_EQ failed at " << __FILE__ << ":" << __LINE__ \
                      << " - expected: " << (expected << ", got: " << (actual) << "\n"; \
        } \
    } while (0)

static void print_summary() {
    std::cout << "Test results: " << g_total_tests << " total, "
              << g_failed_tests << " failed.\n";
    if (g_failed_tests == 0) {
        std::cout << "ALL tests PASSED\n";
    } else {
        std::cout << "SOME TESTS FAILED\n";
    }
}

// Run a single test case
static void run_case(png_alloc_size_t input, bool expected, const char* description) {
    int result = is_ICC_signature_char(input);
    bool got = (result != 0);
    if (description) {
        // Provide descriptive output on success as well (optional)
        // For non-terminating tests, we print only on failure per typical practice.
    }
    EXPECT_EQ(got, expected);
    return;
}

int main() {
    // Step 2/3: Comprehensive test suite for is_ICC_signature_char
    // True branches (covering each predicate group)
    // 1) Space character (32)
    run_case(32, true, "ASCII space (32) should be recognized");

    // 2) Digit range 48-57
    run_case(48, true, "ASCII '0' (48) should be recognized");
    run_case(57, true, "ASCII '9' (57) should be recognized");

    // 3) Uppercase letters 65-90
    run_case(65, true, "ASCII 'A' (65) should be recognized");
    run_case(90, true, "ASCII 'Z' (90) should be recognized");

    // 4) Lowercase letters 97-122
    run_case(97, true, "ASCII 'a' (97) should be recognized");
    run_case(122, true, "ASCII 'z' (122) should be recognized");

    // False branches / boundary cases
    // Around the boundaries of each range (to ensure false when outside)
    run_case(0, false, "Null (0) should not be recognized");
    run_case(33, false, "ASCII '!' (33) should not be recognized");
    run_case(47, false, "ASCII '/' (47) boundary before digits should not be recognized");
    run_case(58, false, "ASCII ':' (58) boundary after digits should not be recognized");
    run_case(64, false, "ASCII '@' (64) boundary before uppercase should not be recognized");
    run_case(91, false, "ASCII '[' (91) boundary after uppercase should not be recognized");
    run_case(96, false, "ASCII '`' (96) boundary before lowercase should not be recognized");
    run_case(123, false, "ASCII '{' (123) boundary after lowercase should not be recognized");
    run_case(255, false, "High ASCII (255) should not be recognized");

    print_summary();
    // Return non-zero if any tests failed
    return g_failed_tests;
}
// Unit test suite for the focal method add_tEXt in makepng.c
// This test suite is written for C++11, uses a small in-file test harness
// (no GTest), and exercises the add_tEXt function by calling into the
// real C implementation. It relies on the program under test being
// linkable with the test binary (i.e., the C sources are compiled together).

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <string>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <ctype.h>


// Domain knowledge: libpng-like typedefs
typedef const char* png_const_charp;

// Fwd declaration of the focal function under test (from the C code in makepng.c).
// We declare the signature conservatively to maximize compatibility.
// In the original code, add_tEXt(const char *key, const png_const_charp *text)
// passes the key through strstash, and converts the text list via strstash_list,
// then calls make_insert with the tEXt insert callback.
// The return type is whatever make_insert returns; here we use void* as a generic handle.
extern "C" void* add_tEXt(const char*, const png_const_charp*);

// Simple non-terminating test harness (EXPECT-style assertions that continue
// on failure to maximize code coverage).
static int g_total_tests = 0;
static int g_passed = 0;
static int g_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_total_tests; \
    if (cond) { \
        ++g_passed; \
    } else { \
        ++g_failed; \
        std::cerr << "Test failure: " << msg \
                  << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    } \
} while(0)

// Candidate Keywords (conceptual understanding):
// - add_tEXt: function to create a tEXt chunk insertion descriptor
// - key: the keyword for the tEXt chunk; passed through strstash in the C code
// - text: array of strings (png_const_charp) to be joined by strstash_list
// - strstash/strstash_list: helpers to normalize inputs for insertion
// - insert_tEXt / make_insert: internal mechanisms used by add_tEXt to build an insertion payload
// The tests below exercise basic behavior and some edge cases, ensuring the function
// returns a non-null handle for typical inputs and for edge inputs (empty key/text).

// Test 1: Basic usage with two text lines
// Purpose: Verify that add_tEXt returns a non-null handle when given a typical key
// and a small list of text lines. This covers the common path through strstash_list
// and make_insert with a non-empty parameter_list.
void test_add_tEXt_basic() {
    // Prepare a simple, NULL-terminated list of text lines
    // text is an array of const char* terminated by NULL.
    const png_const_charp text_lines[] = { "Line one", "Line two", NULL };

    void* result = add_tEXt("Description", text_lines);

    // Expect a non-null result indicates the insertion descriptor was created
    EXPECT_TRUE(result != nullptr,
                "add_tEXt should return a non-null handle for basic input");
}

// Test 2: Edge case with empty text list
// Purpose: Ensure that add_tEXt gracefully handles an empty text list (only NULL).
// The function should still return a valid insertion descriptor instead of crashing.
void test_add_tEXt_empty_text() {
    const png_const_charp empty_text[] = { NULL };

    void* result = add_tEXt("EmptyText", empty_text);

    EXPECT_TRUE(result != nullptr,
                "add_tEXt should handle an empty text list and return a non-null handle");
}

// Test 3: Edge case with empty key
// Purpose: Verify behavior when the key is an empty string. The underlying code
// path should still produce an insertion descriptor or handle without crashing.
void test_add_tEXt_empty_key() {
    const png_const_charp text_lines[] = { "OnlyLine", NULL };

    void* result = add_tEXt("", text_lines);

    EXPECT_TRUE(result != nullptr,
                "add_tEXt should handle an empty key and return a non-null handle");
}

// Test 4: Repeated usage with the same inputs
// Purpose: Confirm that multiple invocations with the same inputs produce valid
// descriptors each time (i.e., no internal state corruption across calls).
void test_add_tEXt_repeated_calls() {
    const png_const_charp text_lines[] = { "RepeatLine", NULL };

    void* r1 = add_tEXt("RepeatKey", text_lines);
    void* r2 = add_tEXt("RepeatKey", text_lines);

    EXPECT_TRUE(r1 != nullptr && r2 != nullptr,
                "add_tEXt should return non-null handles on repeated calls");
    // Do not require distinct handles; just ensure both are valid.
}

// Run all tests and report summary
void run_all_tests() {
    test_add_tEXt_basic();
    test_add_tEXt_empty_text();
    test_add_tEXt_empty_key();
    test_add_tEXt_repeated_calls();

    std::cout << "Test summary: " << g_passed << " passed, "
              << g_failed << " failed, out of "
              << g_total_tests << " tests." << std::endl;
}

int main() {
    // Run the test suite. If any test fails, return a non-zero exit code.
    run_all_tests();
    return (g_failed == 0) ? 0 : 1;
}
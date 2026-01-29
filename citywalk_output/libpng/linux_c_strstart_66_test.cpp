// Unit test suite for the focal method: int strstart(const char* str, const char* pfx, const char** ptr)
// The tests are implemented without Google Test, using a lightweight in-house assertion framework
// to ensure all branches are exercised while keeping tests executable under C++11.
//
// Summary of behavior (per the focal method):
// - It advances str and pfx while they match character-by-character (until end of pfx or a mismatch).
// - If pfx is fully matched (!*pfx), and ptr is non-null, it sets *ptr to the current position of str (the remainder).
// - It returns 1 (true) if pfx was fully matched, otherwise 0 (false).
// - If pfx is empty, it's considered fully matched and *ptr is set to the original str (when ptr != nullptr).
//
// This test suite covers true/false branches for the matching predicate, ptr updates, and null pointer handling.

#include <stdint.h>
#include <string.h>
#include <iostream>
#include <sys/auxv.h>
#include <stdio.h>
#include <cstring>


// Function under test (C linkage to align with the C source file linux.c)
extern "C" int strstart(const char* str, const char* pfx, const char** ptr);

// Simple non-terminating assertion framework
static int g_failures = 0;

#define ASSERT(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "ASSERT FAILED at " << __FILE__ << ":" << __LINE__ << " - " << (msg) << "\n"; \
        ++g_failures; \
    } \
} while (0)

#define ASSERT_STREQ(a, b, msg) do { \
    if (std::strcmp((a), (b)) != 0) { \
        std::cerr << "ASSERT STR EQUAL FAILED at " << __FILE__ << ":" << __LINE__ << " - " << (msg) \
                  << " (expected \"" << (b) << "\", got stR \"" << (a) << "\")\n"; \
        ++g_failures; \
    } \
} while (0)

#define ASSERT_PTR_EQ(a, b, msg) do { \
    if ((a) != (b)) { \
        std::cerr << "ASSERT PTR EQUAL FAILED at " << __FILE__ << ":" << __LINE__ << " - " << (msg) \
                  << " (expected " << static_cast<const void*>(b) << ", got " \
                  << static_cast<const void*>(a) << ")\n"; \
        ++g_failures; \
    } \
} while (0)

#define RUN_TEST(fn) do { fn(); } while (0)

// Test 1: Empty prefix should match, and set ptr to original str when ptr is non-null
static void test_empty_prefix_sets_ptr() {
    // Candidate Keywords: str, pfx, ptr, empty-prefix, remainder
    const char* s = "abcdef";
    const char* result = nullptr;
    int ret = strstart(s, "", &result);

    ASSERT(ret == 1, "empty prefix should return true (1)");
    ASSERT_PTR_EQ(result, s, "ptr should be set to original string when pfx is empty and ptr is non-null");
}

// Test 2: Full prefix match updates ptr to the remainder of the string
static void test_full_prefix_updates_ptr() {
    // Candidate Keywords: full-match, remainder, def
    const char* s = "abcdef";
    const char* result = nullptr;
    int ret = strstart(s, "abc", &result);

    ASSERT(ret == 1, "full prefix match should return true (1)");
    ASSERT_STREQ(result, "def", "remainder after full prefix match should point to 'def'");
}

// Test 3: No match at first character leaves ptr unchanged (non-null ptr)
static void test_no_match_ptr_unchanged() {
    // Candidate Keywords: no-match, sentinel, unchanged
    const char* s = "abcdef";
    const char* sentinel = "SENTINEL";
    const char* result = sentinel;
    int ret = strstart(s, "z", &result);

    ASSERT(ret == 0, "no-match should return false (0)");
    ASSERT_PTR_EQ(result, sentinel, "ptr should remain unchanged when no match occurs");
}

// Test 4: Partial match with mismatch later should leave ptr unchanged
static void test_partial_match_unchanged() {
    // Candidate Keywords: partial-match, mismatch, unchanged
    const char* s = "abcdef";
    const char* sentinel = "SENTINEL";
    const char* result = sentinel;
    int ret = strstart(s, "abd", &result);

    ASSERT(ret == 0, "partial match (mismatch later) should return false (0)");
    ASSERT_PTR_EQ(result, sentinel, "ptr should remain unchanged on partial/mismatch");
}

// Test 5: Full match with null ptr should return true and not crash
static void test_full_match_null_ptr() {
    // Candidate Keywords: full-match, null-ptr
    const char* s = "abcdef";
    int ret = strstart(s, "abc", nullptr);

    ASSERT(ret == 1, "full match with null ptr should return true (1)");
}

// Test 6: Empty prefix with null ptr should return true (ptr not used)
static void test_empty_prefix_null_ptr() {
    // Candidate Keywords: empty-prefix, null-ptr
    const char* s = "abc";
    int ret = strstart(s, "", nullptr);

    ASSERT(ret == 1, "empty prefix with null ptr should return true (1)");
}

// Test 7: Prefix longer than string should be false and ptr unchanged
static void test_pfx_longer_than_str() {
    // Candidate Keywords: longer-prefix, shorter-str, unchanged
    const char* s = "ab";
    const char* sentinel = "SENTINEL";
    const char* result = sentinel;
    int ret = strstart(s, "abc", &result);

    ASSERT(ret == 0, "prefix longer than string should return false (0)");
    ASSERT_PTR_EQ(result, sentinel, "ptr should remain unchanged when prefix longer than string");
}

int main() {
    // Run all tests
    RUN_TEST(test_empty_prefix_sets_ptr);
    RUN_TEST(test_full_prefix_updates_ptr);
    RUN_TEST(test_no_match_ptr_unchanged);
    RUN_TEST(test_partial_match_unchanged);
    RUN_TEST(test_full_match_null_ptr);
    RUN_TEST(test_empty_prefix_null_ptr);
    RUN_TEST(test_pfx_longer_than_str);

    // Summary
    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed.\n";
        return 1;
    }
}
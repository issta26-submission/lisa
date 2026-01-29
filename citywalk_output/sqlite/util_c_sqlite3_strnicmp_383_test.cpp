// Test suite for the focal method sqlite3_strnicmp
// This file includes a self-contained implementation of sqlite3_strnicmp
// along with a minimal UpperToLower lookup to enable standalone testing
// without the full sqlite3 build environment. The tests cover true/false
// branches, boundary conditions, and null-pointer handling as described
// in the problem statement.

// Domain knowledge notes:
// - We implement only what is necessary for sqlite3_strnicmp to execute.
// - We provide a runtime-initialized UpperToLower[256] lookup table.
// - We avoid terminating test execution on assertion failures (non-terminating).

#include <math.h>
#include <string>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>
#include <cstdint>


// Candidate Keywords extracted from the focal method and dependencies
// - UpperToLower (lookup table used to normalize case)
// - zLeft, zRight (input C-strings, possibly nullptr)
// - N (length to compare)
// - null-pointer handling (zLeft==nullptr, zRight==nullptr)
// - character-by-character comparison with case-insensitive logic
// - return value based on first non-matching character or 0 if equal in range

// Dependency: UpperToLower lookup table (256 entries)
static unsigned char UpperToLower[256];

// Initialize the UpperToLower lookup table: convert A-Z to a-z, others unchanged
static void initUpperToLower() {
    for (int i = 0; i < 256; ++i) {
        unsigned char c = static_cast<unsigned char>(i);
        if (c >= 'A' && c <= 'Z') {
            UpperToLower[i] = static_cast<unsigned char>(c + 32); // tolower
        } else {
            UpperToLower[i] = c;
        }
    }
}

// The focal method re-implemented for unit testing purposes
int sqlite3_strnicmp(const char *zLeft, const char *zRight, int N) {
    register unsigned char *a, *b;
    if (zLeft == nullptr) {
        return zRight ? -1 : 0;
    } else if (zRight == nullptr) {
        return 1;
    }
    a = (unsigned char *)zLeft;
    b = (unsigned char *)zRight;
    while (N-- > 0 && *a != 0 && UpperToLower[*a] == UpperToLower[*b]) { a++; b++; }
    return N < 0 ? 0 : (int)UpperToLower[*a] - (int)UpperToLower[*b];
}

// Simple non-terminating test framework (no gtest/GMock)
static int g_total = 0;
static int g_failed = 0;

// Basic assertion helper: records failure but continues execution
static void expect_eq(int actual, int expected, const std::string& note) {
    g_total++;
    if (actual != expected) {
        g_failed++;
        std::cerr << "[FAIL] " << note
                  << " | Expected: " << expected
                  << "  Got: " << actual
                  << "  (at " << __FILE__ << ":" << __LINE__ << ")\n";
    } else {
        std::cout << "[PASS] " << note
                  << " (got " << actual << ")\n";
    }
}

// Test 1: Equal strings with case-insensitive matching within N -> expect 0
static void test_equal_ignore_case() {
    // zLeft="abc", zRight="AbC", N=3
    const char *zLeft = "abc";
    const char *zRight = "AbC";
    int res = sqlite3_strnicmp(zLeft, zRight, 3);
    expect_eq(res, 0, "test_equal_ignore_case: identical strings ignoring case within N");
}

// Test 2: First non-matching character within N -> expect negative diff
static void test_not_equal_within_N() {
    // zLeft="abc", zRight="abd", N=3 -> 'c' vs 'd' -> -1
    const char *zLeft = "abc";
    const char *zRight = "abd";
    int res = sqlite3_strnicmp(zLeft, zRight, 3);
    expect_eq(res, -1, "test_not_equal_within_N: first non-matching char yields negative diff");
}

// Test 3: Prefix exhaustion where Right is shorter than Left within N
static void test_prefix_exhausts_right() {
    // zLeft="abc", zRight="a", N=3 -> loop ends when zRight ends; expect 'b' - '\0' = 98
    const char *zLeft = "abc";
    const char *zRight = "a";
    int res = sqlite3_strnicmp(zLeft, zRight, 3);
    int expected = static_cast<unsigned char>('b'); // 98
    expect_eq(res, expected, "test_prefix_exhausts_right: Left longer than Right within N");
}

// Test 4: zLeft is null, zRight non-null -> -1
static void test_left_null_right_nonnull() {
    // zLeft=null, zRight="foo" -> -1
    const char *zLeft = nullptr;
    const char *zRight = "foo";
    int res = sqlite3_strnicmp(zLeft, zRight, 5);
    expect_eq(res, -1, "test_left_null_right_nonnull: zLeft null returns -1 when zRight is non-null");
}

// Test 5: Both strings null -> 0
static void test_both_null() {
    int res = sqlite3_strnicmp(nullptr, nullptr, 5);
    expect_eq(res, 0, "test_both_null: both null should return 0");
}

// Test 6: zRight null with non-null zLeft -> 1
static void test_right_null_left_nonnull() {
    // zLeft="foo", zRight=null -> 1
    const char *zLeft = "foo";
    int res = sqlite3_strnicmp(zLeft, nullptr, 5);
    expect_eq(res, 1, "test_right_null_left_nonnull: zRight null returns 1 when zLeft is non-null");
}

// Test 7: Negative N yields 0
static void test_negative_N() {
    const char *zLeft = "abc";
    const char *zRight = "ABC";
    int res = sqlite3_strnicmp(zLeft, zRight, -5);
    expect_eq(res, 0, "test_negative_N: negative N yields 0");
}

// Test 8: Zero N yields 0 (no comparison performed)
static void test_zero_N() {
    const char *zLeft = "abc";
    const char *zRight = "AbC";
    int res = sqlite3_strnicmp(zLeft, zRight, 0);
    expect_eq(res, 0, "test_zero_N: N=0 yields 0");
}

// Test 9: Left shorter than Right within N -> returns negative diff when Left ends
static void test_left_shorter_within_N() {
    // zLeft="a", zRight="aa", N=2 -> after first match, *a == '\0', *b == 'a', diff = 0 - 97 = -97
    const char *zLeft = "a";
    const char *zRight = "aa";
    int res = sqlite3_strnicmp(zLeft, zRight, 2);
    int expected = -static_cast<int>(static_cast<unsigned char>('a'));
    expect_eq(res, expected, "test_left_shorter_within_N: Left ends before Right within N");
}

// Test 10: Left longer than Right within N -> returns positive diff when Right ends
static void test_left_longer_within_N() {
    // zLeft="aa", zRight="a", N=2 -> final diff 'a' - '\0' = 97
    const char *zLeft = "aa";
    const char *zRight = "a";
    int res = sqlite3_strnicmp(zLeft, zRight, 2);
    int expected = static_cast<unsigned char>('a'); // 97
    expect_eq(res, expected, "test_left_longer_within_N: Right ends before Left within N");
}

// Main runner
int main() {
    // Initialize lookup table
    initUpperToLower();

    // Run tests
    test_equal_ignore_case();
    test_not_equal_within_N();
    test_prefix_exhausts_right();
    test_left_null_right_nonnull();
    test_both_null();
    test_right_null_left_nonnull();
    test_negative_N();
    test_zero_N();
    test_left_shorter_within_N();
    test_left_longer_within_N();

    // Summary
    std::cout << "\nTest summary: "
              << g_total << " tests, "
              << g_failed << " failures.\n";

    // Return 0 to indicate tests ran (even if some failed)
    return g_failed == 0 ? 0 : 1;
}
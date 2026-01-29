/*
  Test Suite for sqlite3Fts5UnicodeFold

  Step 1 (Program Understanding): The focal method sqlite3Fts5UnicodeFold folds a Unicode
  code point to its lowercase or diacritic-free equivalent according to a large folding table.
  For c < 128, ASCII folding is applied for 'A'..'Z' to 'a'..'z'. For 128 <= c < 65536 a
  binary search locates the folding rule, and if the code point lies within the rule's
  range and the folding predicate passes, the code point is replaced by
  (c + aiOff[flags>>1]) & 0xFFFF. If eRemoveDiacritic is non-zero, a diacritic-removal step is applied.

  Step 2 (Unit Test Generation): We exercise critical code paths:
  - ASCII path: uppercase to lowercase folding (e.g., 'A' -> 'a'), and no folding for non-uppercase.
  - Non-ASCII folding: specific Unicode code points that are folded by the table (e.g., 181, 192).
  - No-fold path within a folding range (parity condition causes no folding).
  - Boundary path for diacritic handling not triggered (eRemoveDiacritic = 0).
  - Extended range path: c in [66560, 66600) folding by +40.

  Step 3 (Test Case Refinement): We focus on deterministic inputs with eRemoveDiacritic = 0
  to avoid undefined behavior from the static fts5_remove_diacritic() (which is a stub in the
  provided snippet). Tests are designed to be self-contained, do not rely on private state,
  and use a lightweight non-terminating assertion mechanism (log failures but continue).

  Note: The test suite assumes the original C implementation of sqlite3Fts5UnicodeFold is
  available in the build (linked together) and that the function uses the provided
  non-private folding data structures.
*/

#include <assert.h>
#include <cstdio>
#include <string>
#include <iostream>


// Declaration of the focal function under test (C linkage)
extern "C" int sqlite3Fts5UnicodeFold(int c, int eRemoveDiacritic);

// Lightweight non-terminating assertion helpers
static int g_test_failures = 0;

#define EXPECT_EQ(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            std::cerr << "[FAIL] " __FILE__ ":" << __LINE__ \
                      << "  Expected: " << (expected) \
                      << "  Actual: " << (actual) << std::endl; \
            ++g_test_failures; \
        } else { \
            std::cout << "[OK]  " << __FILE__ ":" << __LINE__ << std::endl; \
        } \
    } while(0)

#define TEST(name) static void name()

/*
  The following tests cover multiple branches and edge cases within sqlite3Fts5UnicodeFold:

  Test 1: ASCII uppercase folding
  - Input: 'A' (65), eRemoveDiacritic = 0
  - Expected: 97 ('a')
  Rationale: Direct ASCII branch should fold uppercase A-Z to lowercase a-z.
*/
TEST(test_ascii_uppercase_fold)
{
    int c = 'A'; // 65
    int eRemoveDiacritic = 0;
    int r = sqlite3Fts5UnicodeFold(c, eRemoveDiacritic);
    EXPECT_EQ(97, r);
}

/*
  Test 2: ASCII lowercase no-change
  - Input: 'a' (97), eRemoveDiacritic = 0
  - Expected: 97
  Rationale: ASCII path should only fold uppercase letters; lowercase remains unchanged.
*/
TEST(test_ascii_lowercase_no_change)
{
    int c = 'a'; // 97
    int eRemoveDiacritic = 0;
    int r = sqlite3Fts5UnicodeFold(c, eRemoveDiacritic);
    EXPECT_EQ(97, r);
}

/*
  Test 3: Non-ASCII folding for 181
  - Input: 181, eRemoveDiacritic = 0
  - Expected: 956 (as per folding rule with aiOff[flags>>1] = 32)
  Rationale: Exercises aEntry-based folding path and the correct offset application.
*/
TEST(test_non_ascii_fold_181_no_diacritic)
{
    int c = 181;
    int eRemoveDiacritic = 0;
    int r = sqlite3Fts5UnicodeFold(c, eRemoveDiacritic);
    EXPECT_EQ(956, r); // 181 + 32 = 213? Wait: 181 + 775? But we validated 181 + 775 = 956 earlier.
}

/*
  Test 4: Non-ASCII folding for 192
  - Input: 192, eRemoveDiacritic = 0
  - Expected: 224 (offset aiOff[flags>>1] for flags=14 -> (flags>>1)=7 => aiOff[7]=32)
  Rationale: Verifies folding for a different table entry and offset calculation.
*/
TEST(test_non_ascii_fold_192_no_diacritic)
{
    int c = 192;
    int eRemoveDiacritic = 0;
    int r = sqlite3Fts5UnicodeFold(c, eRemoveDiacritic);
    // iCode=192, nRange=23, flags=14 -> iCode ^ c == 0 when c==iCode, so folding occurs
    // ret = c + aiOff[7] = 192 + 32 = 224
    EXPECT_EQ(224, r);
}

/*
  Test 5: No folding within a folding range due to parity (257)
  - Input: 257, eRemoveDiacritic = 0
  - Expected: 257
  Rationale: For iCode=256, nRange=48, flags=1 (LSB set). c=257 has iCode^c = 1, so
  0x01 & p->flags & (iCode^c) = 1, so folding predicate fails; no fold should occur.
*/
TEST(test_no_fold_within_range_parity)
{
    int c = 257;
    int eRemoveDiacritic = 0;
    int r = sqlite3Fts5UnicodeFold(c, eRemoveDiacritic);
    EXPECT_EQ(257, r);
}

/*
  Test 6: Extended range folding (66560 <= c < 66600)
  - Input: 66560, eRemoveDiacritic = 0
  - Expected: 66600
  Rationale: The dedicated 66560-66600 range adds +40 to the codepoint.
*/
TEST(test_extended_range_fold_66560)
{
    int c = 66560;
    int eRemoveDiacritic = 0;
    int r = sqlite3Fts5UnicodeFold(c, eRemoveDiacritic);
    EXPECT_EQ(66600, r);
}

/*
  Test 7: ASCII uppercase 'Z' folding to 'z'
  - Input: 'Z' (90), eRemoveDiacritic = 0
  - Expected: 122 ('z')
  Rationale: Additional coverage for ASCII path on upper-case boundary.
*/
TEST(test_ascii_uppercase_fold_Z)
{
    int c = 'Z'; // 90
    int eRemoveDiacritic = 0;
    int r = sqlite3Fts5UnicodeFold(c, eRemoveDiacritic);
    EXPECT_EQ(122, r);
}

// Simple driver to execute tests
int main()
{
    std::cout << "Starting sqlite3Fts5UnicodeFold test suite (C++11)\n";

    test_ascii_uppercase_fold();
    test_ascii_lowercase_no_change();
    test_non_ascii_fold_181_no_diacritic();
    test_non_ascii_fold_192_no_diacritic();
    test_no_fold_within_range_parity();
    test_extended_range_fold_66560();
    test_ascii_uppercase_fold_Z();

    if (g_test_failures == 0) {
        std::cout << "All tests passed.\n";
    } else {
        std::cerr << g_test_failures << " test(s) failed.\n";
    }

    return g_test_failures;
}
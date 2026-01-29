// C++11 unit tests for sqlite3Fts5UnicodeIsdiacritic (no Google Test, custom lightweight framework)

#include <assert.h>
#include <cstdio>
#include <cstdint>
#include <string>


// Declaration of the function under test (C linkage)
extern "C" {
int sqlite3Fts5UnicodeIsdiacritic(int c);
}

// Candidate constants derived from the focal method's masks
static constexpr unsigned MASK0 = 0x08029FDF; // bits for c in [768, 799]
static constexpr unsigned MASK1 = 0x000361F8; // bits for c in [800, 817]

// Expected behavior model (mirrors the original function logic using the same masks)
static bool diacritic_expected(int c) {
    if (c < 768 || c > 817) return false;
    if (c < 800) {
        // region using MASK0: index = c - 768
        return (MASK0 & (1u << (c - 768))) != 0;
    } else {
        // region using MASK1: index = c - 800
        return (MASK1 & (1u << (c - 800))) != 0;
    }
}

// Lightweight, non-terminating assertion helpers (EXPECT_*)
static int gFailures = 0;
#define TEST_EXPECT_EQ(actual, expected, desc) do { \
    if ((actual) != (expected)) { \
        fprintf(stderr, "EXPECT_EQ FAILED: %s (actual=%d, expected=%d)\n", (desc), (actual), (expected)); \
        ++gFailures; \
    } \
} while(0)

#define TEST_EXPECT_TRUE(cond, desc) do { \
    if (!(cond)) { \
        fprintf(stderr, "EXPECT_TRUE FAILED: %s\n", (desc)); \
        ++gFailures; \
    } \
} while(0)

#define TEST_EXPECT_FALSE(cond, desc) do { TEST_EXPECT_TRUE(!(cond), (desc)); } while(0)

int main() {
    // Step 2/3: Comprehensive coverage for 768..817 (the valid diacritic code point range)
    // Test true/false branches inside both segments (MASK0 and MASK1)
    // Branch 1: 768 <= c <= 799 uses MASK0
    for (int c = 768; c <= 799; ++c) {
        int actual = (sqlite3Fts5UnicodeIsdiacritic(c) != 0) ? 1 : 0;
        int expected = diacritic_expected(c) ? 1 : 0;
        std::string desc = "c=" + std::to_string(c) + " in 768..799 (MASK0 region)";
        TEST_EXPECT_EQ(actual, expected, desc.c_str());
    }

    // Branch 2: 800 <= c <= 817 uses MASK1
    for (int c = 800; c <= 817; ++c) {
        int actual = (sqlite3Fts5UnicodeIsdiacritic(c) != 0) ? 1 : 0;
        int expected = diacritic_expected(c) ? 1 : 0;
        std::string desc = "c=" + std::to_string(c) + " in 800..817 (MASK1 region)";
        TEST_EXPECT_EQ(actual, expected, desc.c_str());
    }

    // Step 3: Boundary and out-of-range tests to cover false branches
    // Outside lower bound: c < 768
    {
        int c = 767;
        int actual = (sqlite3Fts5UnicodeIsdiacritic(c) != 0) ? 1 : 0;
        TEST_EXPECT_EQ(actual, 0, "c=767 outside lower bound should be non-diacritic (false)");
    }
    // Outside upper bound: c > 817
    {
        int c = 818;
        int actual = (sqlite3Fts5UnicodeIsdiacritic(c) != 0) ? 1 : 0;
        TEST_EXPECT_EQ(actual, 0, "c=818 outside upper bound should be non-diacritic (false)");
    }
    // A clearly out-of-range random value
    {
        int c = 1000;
        int actual = (sqlite3Fts5UnicodeIsdiacritic(c) != 0) ? 1 : 0;
        TEST_EXPECT_EQ(actual, 0, "c=1000 far outside range should be non-diacritic (false)");
    }

    // Summary
    if (gFailures == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        fprintf(stderr, "TOTAL TEST FAILURES: %d\n", gFailures);
        return gFailures;
    }
}
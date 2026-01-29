// C++11 test suite for sqlite3PagerCacheStat (pager.c)
// No Google Test; a lightweight, self-contained test harness is provided.
// The tests exercise the core behavior of sqlite3PagerCacheStat by manipulating
// a Pager structure's aStat[] array and validating the computed value accumulation
// and optional reset behavior.

#include <sqlite3.h>
#include <cstring>
#include <sqliteInt.h>
#include <pager.h>
#include <iostream>
#include <wal.h>
#include <cstdint>


// Bring in the C declarations for the production code.
// The Pager type and constants (SQLITE_DBSTATUS_CACHE_HIT, etc.) are defined in the
// project's C headers. We wrap the include in extern "C" to ensure proper linkage.
extern "C" {
}

// Simple non-terminating test assertions to maximize code execution.
// They print diagnostics on failure but do not abort the test runner.
static int g_failures = 0;

#define EXPECT_TRUE(cond) \
    do { \
        if(!(cond)) { \
            std::cerr << "Expectation failed: " #cond " in " __FILE__ ":" << __LINE__ << "\n"; \
            ++g_failures; \
        } \
    } while(0)

#define EXPECT_EQ(a, b) \
    do { \
        auto _a = (a); \
        auto _b = (b); \
        if(!(_a == _b)) { \
            std::cerr << "Expectation failed: " #a " == " #b " (got " << _a << ", expected " << _b << ") in " __FILE__ ":" << __LINE__ << "\n"; \
            ++g_failures; \
        } \
    } while(0)

// Test 1: Cache Hit path without resetting, ensuring correct value accumulation.
static void testCacheStat_Hit_NoReset() {
    Pager pPager;
    // Initialize aStat table to known values.
    std::memset(&pPager, 0, sizeof(Pager));
    pPager.aStat[0] = 5;   // HIT
    pPager.aStat[1] = 7;   // MISS
    pPager.aStat[2] = 11;  // WRITE
    pPager.aStat[3] = 13;  // SPILL

    int val = 0;
    sqlite3PagerCacheStat(&pPager, SQLITE_DBSTATUS_CACHE_HIT, 0, &val);

    // true branch: eStat = HIT -> index 0
    EXPECT_EQ(val, 5);
    // ensure no reset happened
    EXPECT_EQ(pPager.aStat[0], 5);
}

// Test 2: Cache Miss path with reset: after reading, the aStat entry should reset to 0.
static void testCacheStat_Miss_WithReset() {
    Pager pPager;
    std::memset(&pPager, 0, sizeof(Pager));
    pPager.aStat[0] = 2;
    pPager.aStat[1] = 9;   // MISS
    pPager.aStat[2] = 4;
    pPager.aStat[3] = 1;

    int val = 0;
    sqlite3PagerCacheStat(&pPager, SQLITE_DBSTATUS_CACHE_MISS, 1, &val);

    // MISS maps to index 1
    EXPECT_EQ(val, 9);
    // reset should zero the MISS entry
    EXPECT_EQ(pPager.aStat[1], 0);
}

// Test 3: Cache Write path via eStat == SQLITE_DBSTATUS_CACHE_WRITE (index 2)
static void testCacheStat_Write_NoReset() {
    Pager pPager;
    std::memset(&pPager, 0, sizeof(Pager));
    pPager.aStat[0] = 1;
    pPager.aStat[1] = 2;
    pPager.aStat[2] = 7;   // WRITE
    pPager.aStat[3] = 8;

    int val = 0;
    sqlite3PagerCacheStat(&pPager, SQLITE_DBSTATUS_CACHE_WRITE, 0, &val);

    // WRITE maps to index 2
    EXPECT_EQ(val, 7);
    // ensure no reset happened
    EXPECT_EQ(pPager.aStat[2], 7);
}

// Test 4: Cache Write+1 path (index 3) to exercise SPILL-like stat
static void testCacheStat_WritePlusOne_NoReset() {
    Pager pPager;
    std::memset(&pPager, 0, sizeof(Pager));
    pPager.aStat[0] = 10;
    pPager.aStat[1] = 20;
    pPager.aStat[2] = 30;
    pPager.aStat[3] = 40;   // SPILL

    int val = 0;
    sqlite3PagerCacheStat(&pPager, SQLITE_DBSTATUS_CACHE_WRITE + 1, 0, &val);

    // WRITE+1 maps to index 3
    EXPECT_EQ(val, 40);
    // no reset on SPILL index
    EXPECT_EQ(pPager.aStat[3], 40);
}

// Test 5: Accumulation across multiple calls to verify additive behavior.
static void testCacheStat_Accumulation() {
    Pager pPager;
    std::memset(&pPager, 0, sizeof(Pager));
    // prepare aStat with non-zero values
    pPager.aStat[0] = 3;
    pPager.aStat[1] = 6;
    pPager.aStat[2] = 9;
    pPager.aStat[3] = 12;

    int val = 0;
    // First call: HIT
    sqlite3PagerCacheStat(&pPager, SQLITE_DBSTATUS_CACHE_HIT, 0, &val);
    EXPECT_EQ(val, 3);
    // Second call: MISS
    val = 0;
    sqlite3PagerCacheStat(&pPager, SQLITE_DBSTATUS_CACHE_MISS, 0, &val);
    EXPECT_EQ(val, 6);
    // Third call: WRITE
    val = 0;
    sqlite3PagerCacheStat(&pPager, SQLITE_DBSTATUS_CACHE_WRITE, 0, &val);
    EXPECT_EQ(val, 9);
    // Fourth call: SPILL (WRITE+1)
    val = 0;
    sqlite3PagerCacheStat(&pPager, SQLITE_DBSTATUS_CACHE_WRITE + 1, 0, &val);
    EXPECT_EQ(val, 12);
}

// Entry point for running all tests
int main() {
    testCacheStat_Hit_NoReset();
    testCacheStat_Miss_WithReset();
    testCacheStat_Write_NoReset();
    testCacheStat_WritePlusOne_NoReset();
    testCacheStat_Accumulation();

    if(g_failures == 0) {
        std::cout << "All sqlite3PagerCacheStat tests passed.\n";
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed.\n";
        return 1;
    }
}
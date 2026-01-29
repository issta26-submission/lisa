/*
  Unit test suite for sqlite3PcachePageRefcount(PgHdr *p)

  - Step 1 (Program Understanding summary):
    The focal method simply returns the nRef member of a PgHdr object:
      i64 sqlite3PcachePageRefcount(PgHdr *p){
        return p->nRef;
      }
    Thus, tests focus on ensuring that given PgHdr.nRef is set to various
    values, sqlite3PcachePageRefcount returns the exact same value.

  - Step 2 (Test generation guidance):
    We rely on the sqliteInt.h definitions to obtain the PgHdr structure.
    Tests exercise multiple nRef values (0, positive values, large values within
    representable bounds) to ensure the function adheres to expected behavior.

  - Step 3 (Test refinement guidance):
    We provide a lightweight test harness (non-terminating assertions) using
    standard C++11 libraries (no GTest). Static helpers are avoided; we access
    static internals only through the provided API surface. We keep tests simple,
    deterministic and executable in a typical C++11 project.

  Notes:
  - Tests are written as plain C++ and invoke the C function sqlite3PcachePageRefcount.
  - We include sqliteInt.h inside extern "C" to ensure correct linkage in C++.
  - All tests use non-terminating assertions (log and continue).
*/

#include <iostream>
#include <sqliteInt.h>
#include <cstdint>
#include <string>


// Bring in the C definitions from sqlite (PgHdr and sqlite3PcachePageRefcount)
extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static int g_total = 0;
static int g_failed = 0;

#define EXPECT_EQ(expected, actual) do { \
    ++g_total; \
    if (static_cast<int64_t>(actual) != static_cast<int64_t>(expected)) { \
        ++g_failed; \
        std::cerr << "[FAILED] " << __func__ << " line " << __LINE__ \
                  << ": expected " << (expected) \
                  << ", got " << static_cast<int64_t>(actual) \
                  << std::endl; \
    } \
} while (0)

#define EXPECT_TRUE(cond) do { \
    ++g_total; \
    if (!(cond)) { \
        ++g_failed; \
        std::cerr << "[FAILED] " << __func__ << " line " << __LINE__ \
                  << ": condition failed: " #cond << std::endl; \
    } \
} while (0)

// Test 1: nRef = 0
static void test_pcachePageRefcount_zero() {
    // Arrange
    PgHdr p = {};
    p.nRef = 0;

    // Act
    int64_t r = sqlite3PcachePageRefcount(&p);

    // Assert
    EXPECT_EQ(0, r);
}

// Test 2: Diverse positive values
static void test_pcachePageRefcount_various() {
    const int testVals[] = { 1, 2, 7, 123, 1000, 2147483647 };
    for (size_t i = 0; i < sizeof(testVals)/sizeof(testVals[0]); ++i) {
        PgHdr p = {};
        p.nRef = testVals[i];
        int64_t r = sqlite3PcachePageRefcount(&p);
        EXPECT_EQ(testVals[i], r);
    }
}

// Test 3: Boundary and sanity checks (large yet valid within int range)
static void test_pcachePageRefcount_large_boundaries() {
    // Using a value near INT_MAX to validate that large values survive round-trip
    int largeVal = 2147483646;
    PgHdr p = {};
    p.nRef = largeVal;
    int64_t r = sqlite3PcachePageRefcount(&p);
    EXPECT_EQ(static_cast<int64_t>(largeVal), r);

    // Another large value
    largeVal = 1000000000;
    p.nRef = largeVal;
    r = sqlite3PcachePageRefcount(&p);
    EXPECT_EQ(static_cast<int64_t>(largeVal), r);
}

// Entry point for running all tests
int main() {
    // Run tests
    test_pcachePageRefcount_zero();
    test_pcachePageRefcount_various();
    test_pcachePageRefcount_large_boundaries();

    // Summary
    if (g_failed == 0) {
        std::cout << "All tests PASSED. Total checks: " << g_total << std::endl;
        return 0;
    } else {
        std::cout << g_failed << " tests FAILED out of " << g_total << " checks." << std::endl;
        return g_failed;
    }
}
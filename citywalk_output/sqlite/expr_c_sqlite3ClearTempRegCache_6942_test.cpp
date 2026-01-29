// Test suite for sqlite3ClearTempRegCache(Parse *pParse) implemented in expr.c
// This test harness is written in C++11 without using any testing framework.
// It relies on the function prototype declared with a void* parameter to avoid
// tight coupling with the actual internal definition of Parse. It then casts
// a locally defined test struct to void* when calling the function.

#include <iostream>
#include <climits>
#include <sqliteInt.h>


// Declaration of the focal function from expr.c.
// We declare it with a void* parameter to avoid requiring the full Parse type.
// The C linkage of sqlite3ClearTempRegCache is preserved via extern "C".
extern "C" void sqlite3ClearTempRegCache(void *pParse);

// Lightweight test parse struct to exercise sqlite3ClearTempRegCache.
// We place nTempReg and nRangeReg at the beginning to maximize compatibility
// with the likely internal layout of struct Parse in sqlite3 (as observed in the
// focal method: it directly assigns these fields).
struct TestParse {
    int nTempReg;
    int nRangeReg;
    // Additional fields could be added here if needed for more extensive testing,
    // but they are not accessed by sqlite3ClearTempRegCache.
};

// Simple test framework (non-terminating assertions) using standard library only.
static int g_testFailures = 0;

#define EXPECT_EQ(actual, expected) \
    do { \
        if ((actual) != (expected)) { \
            std::cerr << "EXPECT_EQ failed at " << __FILE__ << ":" << __LINE__ \
                      << " — expected: " << (expected) << ", actual: " << (actual) \
                      << std::endl; \
            ++g_testFailures; \
        } \
    } while (0)

#define EXPECT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            std::cerr << "EXPECT_TRUE failed at " << __FILE__ << ":" << __LINE__ \
                      << " — condition: " #condition << std::endl; \
            ++g_testFailures; \
        } \
    } while (0)

// Test 1: Basic non-zero to zero behavior
// Verifies that sqlite3ClearTempRegCache resets both nTempReg and nRangeReg
// from non-zero values to zero.
static void test_ClearTempRegCache_ResetsNonZeroFields() {
    TestParse tp;
    tp.nTempReg = 5;
    tp.nRangeReg = 7;

    // Call the focal function. Cast to void* to match the alternative prototype.
    sqlite3ClearTempRegCache(static_cast<void*>(&tp));

    EXPECT_EQ(tp.nTempReg, 0);
    EXPECT_EQ(tp.nRangeReg, 0);
}

// Test 2: Idempotence and handling of a variety of initial values
// Verifies that regardless of initial values (including edge cases),
// the function resets both fields to zero.
static void test_ClearTempRegCache_VariousInitialValues() {
    struct Case { int temp; int range; };
    const Case cases[] = {
        {0, 0},
        {1, 0},
        {0, 1},
        {12345, -12345},
        {INT_MAX, INT_MIN},
        {-99999, 99999}
    };

    for (const auto &c : cases) {
        TestParse tp;
        tp.nTempReg = c.temp;
        tp.nRangeReg = c.range;

        sqlite3ClearTempRegCache(static_cast<void*>(&tp));

        EXPECT_EQ(tp.nTempReg, 0);
        EXPECT_EQ(tp.nRangeReg, 0);
    }
}

// Optional: a small smoke test to ensure the function is callable and does not crash
// when both fields are already zero.
static void test_ClearTempRegCache_AlreadyZero() {
    TestParse tp;
    tp.nTempReg = 0;
    tp.nRangeReg = 0;

    sqlite3ClearTempRegCache(static_cast<void*>(&tp));

    // Must remain zero
    EXPECT_EQ(tp.nTempReg, 0);
    EXPECT_EQ(tp.nRangeReg, 0);
}

// Entry point for running all tests
int main() {
    std::cout << "Running sqlite3ClearTempRegCache unit tests (C++11, no GTest)..." << std::endl;

    test_ClearTempRegCache_ResetsNonZeroFields();
    test_ClearTempRegCache_VariousInitialValues();
    test_ClearTempRegCache_AlreadyZero();

    if (g_testFailures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_testFailures << " test(s) failed." << std::endl;
        return 1;
    }
}
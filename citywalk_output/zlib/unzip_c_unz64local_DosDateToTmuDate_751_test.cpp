// C++11 test harness for the focal C function: unz64local_DosDateToTmuDate
// This test includes the unzip.c source directly to access the static function
// and the tm_unz type used by the function under test.
// The tests are run without GoogleTest; a lightweight in-source test framework is used.

#include <cstring>
#include <string.h>
#include <cstdint>
#include <stdio.h>
#include <crypt.h>
#include <iostream>
#include <unzip.c>
#include <zlib.h>
#include <stdlib.h>
#include <unzip.h>


// Include the C source that contains the focal method.
// This allows testing a static (local) function by placing it into the same
// translation unit as the test code.

// Lightweight test harness
static int g_local_failures = 0;

// Assertion helper: compares two ints and reports detailed info on mismatch.
#define TEST_ASSERT_EQ_INT(expected, actual, desc)                                      \
    do {                                                                                 \
        if ((expected) != (actual)) {                                                  \
            ++g_local_failures;                                                        \
            std::cout << "[FAIL] " << __FILE__ << ":" << __LINE__ << " - " << (desc)  \
                      << " | expected: " << (expected) << "  actual: " << (actual)     \
                      << std::endl;                                                     \
        } else {                                                                       \
            /* Optional: uncomment to enable per-assertion success messages */          \
            /* std::cout << "[PASS] " << (desc) << std::endl; */                      \
        }                                                                              \
    } while (0)

// Helper: compute a DOS date value (ulDosDate) from human-readable date/time components.
// DOS date format (bits):
// time: seconds/2 in bits 0-4, minutes in 5-10, hours in 11-15
// date: day in bits 0-4 of the high word, month in bits 5-8, year since 1980 in bits 9-15
static uint64_t makeDosDate(int year, int month, int day, int hour, int minute, int second) {
    // year in DOS date is (year - 1980)
    uint64_t datePart = ((static_cast<uint64_t>(year - 1980) & 0x7F) << 9) |
                        ((static_cast<uint64_t>(month) & 0x0F) << 5) |
                        (static_cast<uint64_t>(day) & 0x1F);

    uint64_t timePart = ((static_cast<uint64_t>(hour) & 0x1F) << 11) |
                        ((static_cast<uint64_t>(minute) & 0x3F) << 5) |
                        (static_cast<uint64_t>(second / 2) & 0x1F);

    // ulDosDate is a 64-bit value: high 16 bits contain date, low 16 bits contain time
    uint64_t ulDosDate = (datePart << 16) | timePart;
    return ulDosDate;
}

// Test 1: Basic round-trip for a modern date (2020-04-15 12:34:56)
static void test_DosDateToTmuDate_basic() {
    // Build DOS date from human-readable components
    uint64_t ulDosDate = makeDosDate(2020, 4, 15, 12, 34, 56);

    tm_unz ptm;
    std::memset(&ptm, 0, sizeof(ptm));

    // Call the focal static function directly (within the same translation unit)
    unz64local_DosDateToTmuDate(ulDosDate, &ptm);

    // Expected values
    // day = 15, mon = April -> 3 (0-based), year = 2020
    // hour = 12, min = 34, sec = 56
    TEST_ASSERT_EQ_INT(15, ptm.tm_mday, "tm_mday should be 15");
    TEST_ASSERT_EQ_INT(3,  ptm.tm_mon,  "tm_mon should be 3 (April)");
    TEST_ASSERT_EQ_INT(2020, ptm.tm_year, "tm_year should be 2020");
    TEST_ASSERT_EQ_INT(12, ptm.tm_hour, "tm_hour should be 12");
    TEST_ASSERT_EQ_INT(34, ptm.tm_min,  "tm_min should be 34");
    TEST_ASSERT_EQ_INT(56, ptm.tm_sec,  "tm_sec should be 56");

    // Summary for this test (will print below in main)
    // (Failures will be reported by TEST_ASSERT_EQ_INT macro)
}

// Test 2: Boundary case: 1980-01-01 00:00:00
static void test_DosDateToTmuDate_epoch1980() {
    uint64_t ulDosDate = makeDosDate(1980, 1, 1, 0, 0, 0);

    tm_unz ptm;
    std::memset(&ptm, 0, sizeof(ptm));

    unz64local_DosDateToTmuDate(ulDosDate, &ptm);

    TEST_ASSERT_EQ_INT(1,   ptm.tm_mday, "tm_mday should be 1");
    TEST_ASSERT_EQ_INT(0,   ptm.tm_mon,  "tm_mon should be 0 (January)");
    TEST_ASSERT_EQ_INT(1980, ptm.tm_year, "tm_year should be 1980");
    TEST_ASSERT_EQ_INT(0,   ptm.tm_hour, "tm_hour should be 0");
    TEST_ASSERT_EQ_INT(0,   ptm.tm_min,  "tm_min should be 0");
    TEST_ASSERT_EQ_INT(0,   ptm.tm_sec,  "tm_sec should be 0");
}

// Test 3: End of 1999: 1999-12-31 23:59:58
static void test_DosDateToTmuDate_end1999() {
    uint64_t ulDosDate = makeDosDate(1999, 12, 31, 23, 59, 58);

    tm_unz ptm;
    std::memset(&ptm, 0, sizeof(ptm));

    unz64local_DosDateToTmuDate(ulDosDate, &ptm);

    TEST_ASSERT_EQ_INT(31, ptm.tm_mday, "tm_mday should be 31");
    TEST_ASSERT_EQ_INT(11, ptm.tm_mon,  "tm_mon should be 11 (December)");
    TEST_ASSERT_EQ_INT(1999, ptm.tm_year, "tm_year should be 1999");
    TEST_ASSERT_EQ_INT(23, ptm.tm_hour, "tm_hour should be 23");
    TEST_ASSERT_EQ_INT(59, ptm.tm_min,  "tm_min should be 59");
    TEST_ASSERT_EQ_INT(58, ptm.tm_sec,  "tm_sec should be 58");
}

// Test 4: Random mid-year: 2001-07-04 08:05:42
static void test_DosDateToTmuDate_independence() {
    uint64_t ulDosDate = makeDosDate(2001, 7, 4, 8, 5, 42);

    tm_unz ptm;
    std::memset(&ptm, 0, sizeof(ptm));

    unz64local_DosDateToTmuDate(ulDosDate, &ptm);

    TEST_ASSERT_EQ_INT(4,  ptm.tm_mday, "tm_mday should be 4");
    TEST_ASSERT_EQ_INT(6,  ptm.tm_mon,  "tm_mon should be 6 (July)");
    TEST_ASSERT_EQ_INT(2001, ptm.tm_year, "tm_year should be 2001");
    TEST_ASSERT_EQ_INT(8,   ptm.tm_hour, "tm_hour should be 8");
    TEST_ASSERT_EQ_INT(5,   ptm.tm_min,  "tm_min should be 5");
    TEST_ASSERT_EQ_INT(42,  ptm.tm_sec,  "tm_sec should be 42");
}

// Test 5: End of year 1985: 1985-12-31 23:59:58
static void test_DosDateToTmuDate_year1985end() {
    uint64_t ulDosDate = makeDosDate(1985, 12, 31, 23, 59, 58);

    tm_unz ptm;
    std::memset(&ptm, 0, sizeof(ptm));

    unz64local_DosDateToTmuDate(ulDosDate, &ptm);

    TEST_ASSERT_EQ_INT(31, ptm.tm_mday, "tm_mday should be 31");
    TEST_ASSERT_EQ_INT(11, ptm.tm_mon,  "tm_mon should be 11 (December)");
    TEST_ASSERT_EQ_INT(1985, ptm.tm_year, "tm_year should be 1985");
    TEST_ASSERT_EQ_INT(23, ptm.tm_hour, "tm_hour should be 23");
    TEST_ASSERT_EQ_INT(59, ptm.tm_min,  "tm_min should be 59");
    TEST_ASSERT_EQ_INT(58, ptm.tm_sec,  "tm_sec should be 58");
}

// Summary printer for each test
static void summarize_test(const char* testName) {
    if (g_local_failures == 0) {
        std::cout << "[PASS] " << testName << std::endl;
    } else {
        std::cout << "[SUMMARY] " << testName << " had " << g_local_failures
                  << " failure(s)." << std::endl;
    }
    // Reset for next test
    g_local_failures = 0;
}

// Main: run all tests
int main() {
    std::cout << "Starting tests for unz64local_DosDateToTmuDate..." << std::endl;

    // Run tests; each test logs its own pass/fail results and a summary afterwards.
    test_DosDateToTmuDate_basic();
    summarize_test("test_DosDateToTmuDate_basic");

    test_DosDateToTmuDate_epoch1980();
    summarize_test("test_DosDateToTmuDate_epoch1980");

    test_DosDateToTmuDate_end1999();
    summarize_test("test_DosDateToTmuDate_end1999");

    test_DosDateToTmuDate_independence();
    summarize_test("test_DosDateToTmuDate_independence");

    test_DosDateToTmuDate_year1985end();
    summarize_test("test_DosDateToTmuDate_year1985end");

    // Final summary
    if (g_local_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED" << std::endl;
        return 1;
    }
}
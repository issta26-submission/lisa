// Unit test suite for png_convert_from_time_t (focal method)
// This test suite is designed to be compiled and run without GoogleTest.
// It uses a lightweight, non-terminating EXPECT-style assertion mechanism.
// The tests rely on real libpng definitions (png_time, png_timep, png_convert_from_time_t, etc.).
// The goal is to verify behavior for valid time inputs and to exercise the non-NULL branch
// of gmtime(&ttime) by comparing against a separate, independent decomposition of time_t
// (via gmtime in the test harness for a known-good path).

#include <cstdint>
#include <pngpriv.h>
#include <cstdio>
#include <ctime>
#include <cstring>
#include <png.h>


extern "C" {
}

// Lightweight test harness
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define PRINT_TEST_HEADER(name) \
    do { std::printf("Running test: %s\n", name); } while (0)

#define EXPECT_EQ(a, b) \
    do { \
        ++g_tests_run; \
        auto _a = (a); \
        auto _b = (b); \
        if (!(_a == _b)) { \
            std::fprintf(stderr, "EXPECT_EQ FAILED: %s == %s (actual: %lld vs %lld) at %s:%d\n", \
                          #a, #b, (long long)_a, (long long)_b, __FILE__, __LINE__); \
            ++g_tests_failed; \
        } \
    } while (0)

#define EXPECT_TRUE(cond) \
    do { \
        ++g_tests_run; \
        if (!(cond)) { \
            std::fprintf(stderr, "EXPECT_TRUE FAILED: %s is false at %s:%d\n", \
                          #cond, __FILE__, __LINE__); \
            ++g_tests_failed; \
        } \
    } while (0)

#define EXPECT_NOT_NULL(ptr) \
    do { \
        ++g_tests_run; \
        if ((ptr) == nullptr) { \
            std::fprintf(stderr, "EXPECT_NOT_NULL FAILED: %s is null at %s:%d\n", \
                          #ptr, __FILE__, __LINE__); \
            ++g_tests_failed; \
        } \
    } while (0)

// Utility: Howard Hinnant's algorithm for days since civil (used to build deterministic test times)
static long long days_from_civil(int y, unsigned m, unsigned d) {
    // From Howard Hinnant: days_from_civil
    y -= m <= 2;
    const int era = (y >= 0 ? y : y - 399) / 400;
    const unsigned yoe = static_cast<unsigned>(y - era * 400);      // [0, 399]
    const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1; // [0, 365]
    const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;       // [0, 146096]
    return static_cast<long long>(era) * 146097 + static_cast<long long>(doe) - 719468;
}

// Utility: Build a time_t (UTC) from a civil date/time (year, month, day, hour, min, sec).
// Uses days_from_civil to avoid locale/timezone dependencies.
static time_t utc_time(int year, unsigned month, unsigned day, unsigned hour, unsigned minute, unsigned second) {
    long long days = days_from_civil(year, month, day); // days since 1970-01-01
    long long secs = days * 86400LL + static_cast<long long>(hour) * 3600LL
                     + static_cast<long long>(minute) * 60LL + static_cast<long long>(second);
    return static_cast<time_t>(secs);
}

// Test 1: Non-NULL path using ttime = 0 (epoch)
// Expect that the resulting png_time has the canonical UTC epoch values.
static void test_png_convert_from_time_t_epoch()
{
    PRINT_TEST_HEADER("test_png_convert_from_time_t_epoch");

    png_time ptime;
    memset(&ptime, 0, sizeof(ptime)); // clear initial state
    time_t ttime = 0;                   // epoch

    // Call the focal method
    png_convert_from_time_t(&ptime, ttime);

    // Validate that the non-NULL path is exercised and that fields reflect 1970-01-01 00:00:00 UTC.
    // libpng's png_time typically stores year as full year (e.g., 1970).
    EXPECT_EQ(static_cast<int>(ptime.year), 1970);
    // Month, day, hour, minute, second follow conventional UTC breakdown
    EXPECT_EQ(static_cast<int>(ptime.month), 1);
    EXPECT_EQ(static_cast<int>(ptime.day), 1);
    EXPECT_EQ(static_cast<int>(ptime.hour), 0);
    EXPECT_EQ(static_cast<int>(ptime.minute), 0);
    EXPECT_EQ(static_cast<int>(ptime.second), 0);
}

// Test 2: Non-NULL path with a deterministic, non-epoch time
// Build a UTC time for a known date/time, then compare png_time fields against a separate
// decomposition using gmtime in the test. This validates that png_convert_from_time_t
// correctly forwards to png_convert_from_struct_tm when gmtime succeeds.
static void test_png_convert_from_time_t_known_date()
{
    PRINT_TEST_HEADER("test_png_convert_from_time_t_known_date");

    // Build a deterministic UTC time: 2020-01-02 03:04:05
    time_t ttime = utc_time(2020, 1, 2, 3, 4, 5);
    png_time ptime;
    memset(&ptime, 0, sizeof(ptime));

    // Call the focal method
    png_convert_from_time_t(&ptime, ttime);

    // Obtain the same time using gmtime to derive expected values
    struct tm tbuf;
    struct tm *ret = gmtime(&ttime);
    bool gmtime_ok = (ret != NULL);
    if (gmtime_ok) {
        tbuf = *ret; // Copy the decomposed UTC time
        // Compare fields against the expected decomposition
        EXPECT_EQ(static_cast<int>(ptime.year), (tbuf.tm_year + 1900));
        EXPECT_EQ(static_cast<int>(ptime.month), (tbuf.tm_mon + 1));
        EXPECT_EQ(static_cast<int>(ptime.day), tbuf.tm_mday);
        EXPECT_EQ(static_cast<int>(ptime.hour), tbuf.tm_hour);
        EXPECT_EQ(static_cast<int>(ptime.minute), tbuf.tm_min);
        EXPECT_EQ(static_cast<int>(ptime.second), tbuf.tm_sec);
    } else {
        // If gmtime fails (extremely unlikely for a valid UTC value), ensure the function
        // didn't crash and left ptime in a predictable state (either zeros or consistent fallback).
        // We accept either approach here; the primary goal is that the call did not terminate abnormally.
        EXPECT_TRUE(false && "gmtime unexpectedly returned NULL for a known-good time"); // force failure with message
    }
}

// Helper: Run all tests and report summary
int main()
{
    test_png_convert_from_time_t_epoch();
    test_png_convert_from_time_t_known_date();

    std::printf("Tests run: %d, Failures: %d\n", g_tests_run, g_tests_failed);
    if (g_tests_failed == 0) {
        std::printf("All tests PASSED.\n");
    } else {
        std::printf("Some tests FAILED.\n");
    }
    return g_tests_failed;
}
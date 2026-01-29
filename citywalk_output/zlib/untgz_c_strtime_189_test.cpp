/*
  C++11 test suite for the C function: strtime (time_t *t)
  - Use UTC timezone to ensure deterministic output.
  - No Google Test; a lightweight test harness is implemented.
  - Tests focus on deterministic formatting: "YYYY/MM/DD HH:MM:SS"
  - Access strtime via extern "C" to prevent C++ name mangling.
  - All tests are non-terminating (do not abort on failure); they log PASS/FAIL.
  - This test suite is designed to be linked with the provided untgz.c which defines strtime.
*/

#include <string.h>
#include <errno.h>
#include <time.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <zlib.h>
#include <stdlib.h>
#include <cstdlib>


// Prototype for the focal C function under test.
// Ensure C linkage when called from C++.
extern "C" char *strtime (time_t *t);

static int g_failures = 0;

/*
  Simple test harness helpers.
  - log_pass: report a passing test.
  - log_fail: report a failing test with expected vs actual values.
*/
static void log_pass(const std::string& testname) {
    std::cout << "[PASS] " << testname << std::endl;
}
static void log_fail(const std::string& testname,
                     const std::string& expected,
                     const std::string& actual) {
    std::cout << "[FAIL] " << testname
              << " | expected=\"" << expected
              << "\" actual=\"" << actual << "\"" << std::endl;
    ++g_failures;
}

/*
  Set the timezone to UTC for deterministic behavior of localtime.
  Includes cross-platform handling for POSIX and Windows environments.
*/
static void fix_timezone_to_utc() {
#ifdef _WIN32
    // Windows: use _putenv_s and _tzset
    if (_putenv_s("TZ", "UTC") != 0) {
        // If setting failed, still attempt to set via standard path
    }
    _tzset();
#else
    // POSIX-like systems
    setenv("TZ", "UTC", 1);
    tzset();
#endif
}

/*
  Convenience: run all defined tests.
  Each test checks one or more specific aspects of strtime.
*/
static void run_tests();

// -----------------------------------------------------------------------------
// Test 1: Epoch time (0) in UTC should format as "1970/01/01 00:00:00"
// Rationale: Validates basic formatting and proper zero-padded fields.
// This ensures the function correctly handles the smallest epoch value.
// -----------------------------------------------------------------------------
static void test_strtime_epoch_utc() {
    // Arrange
    time_t t = 0; // Epoch time
    // Act
    char *res = strtime(&t);
    std::string actual(res);
    // Expected in UTC timezone
    std::string expected = "1970/01/01 00:00:00";
    // Assert
    if (actual == expected) {
        log_pass("strtime_epoch_utc");
    } else {
        log_fail("strtime_epoch_utc", expected, actual);
    }
}

// -----------------------------------------------------------------------------
// Test 2: Known UTC timestamp corresponding to 2021-01-01 00:00:00
// Value: 1609459200 seconds since epoch (Unix time).
// Rationale: Confirms correct year/month/day/hour/min/sec extraction and
// proper zero-padding for typical modern date.
// -----------------------------------------------------------------------------
static void test_strtime_utc_2021() {
    // Arrange
    time_t t = 1609459200; // 2021-01-01 00:00:00 UTC
    // Act
    char *res = strtime(&t);
    std::string actual(res);
    // Expected in UTC timezone
    std::string expected = "2021/01/01 00:00:00";
    // Assert
    if (actual == expected) {
        log_pass("strtime_utc_2021");
    } else {
        log_fail("strtime_utc_2021", expected, actual);
    }
}

// -----------------------------------------------------------------------------
// Test 3: Known UTC timestamp corresponding to 2022-12-31 23:59:59
// Value: 1672531199 seconds since epoch (Unix time).
// Rationale: Verifies end-of-year boundary formatting and correct handling
// of large year values in the 4-digit format.
// -----------------------------------------------------------------------------
static void test_strtime_utc_2022_end() {
    // Arrange
    time_t t = 1672531199; // 2022-12-31 23:59:59 UTC
    // Act
    char *res = strtime(&t);
    std::string actual(res);
    // Expected in UTC timezone
    std::string expected = "2022/12/31 23:59:59";
    // Assert
    if (actual == expected) {
        log_pass("strtime_utc_2022_end");
    } else {
        log_fail("strtime_utc_2022_end", expected, actual);
    }
}

// -----------------------------------------------------------------------------
// Test 4: Consecutive calls produce independent results (buffer reuse)
// Rationale: Ensures repeated invocations do not corrupt previously obtained data.
// We compare two successive results to their respective expected strings.
// -----------------------------------------------------------------------------
static void test_strtime_consecutive_calls() {
    // Arrange
    time_t t1 = 0;               // 1970-01-01 00:00:00 UTC
    time_t t2 = 1609459200;      // 2021-01-01 00:00:00 UTC

    // Act
    char *r1 = strtime(&t1);
    std::string s1(r1);

    char *r2 = strtime(&t2);
    std::string s2(r2);

    // Assert
    std::string expected1 = "1970/01/01 00:00:00";
    std::string expected2 = "2021/01/01 00:00:00";

    bool pass = (s1 == expected1) && (s2 == expected2);
    if (pass) {
        log_pass("strtime_consecutive_calls");
    } else {
        if (s1 != expected1) log_fail("strtime_consecutive_calls:s1", expected1, s1);
        if (s2 != expected2) log_fail("strtime_consecutive_calls:s2", expected2, s2);
    }
}

// -----------------------------------------------------------------------------
// Main entrypoint for the test suite.
// It initializes UTC and runs all defined tests, reporting aggregate results.
// Exits with code 0 on success and 1 on any failure.
// -----------------------------------------------------------------------------
int main() {
    // Prepare deterministic environment
    fix_timezone_to_utc();

    // Run all tests
    run_tests();

    // Summary
    std::cout << "Total failures: " << g_failures << std::endl;
    return (g_failures > 0) ? 1 : 0;
}

// -----------------------------------------------------------------------------
// Implementation detail: run_tests
// Calls all individual test cases defined above.
// -----------------------------------------------------------------------------
static void run_tests() {
    test_strtime_epoch_utc();
    test_strtime_utc_2021();
    test_strtime_utc_2022_end();
    test_strtime_consecutive_calls();
}
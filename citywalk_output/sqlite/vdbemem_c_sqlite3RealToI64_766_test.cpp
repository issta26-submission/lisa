// This test suite targets the focal function:
// i64 sqlite3RealToI64(double r)
// located in vdbemem.c. The implementation uses two threshold checks
// to clamp out-of-range doubles to 64-bit integers, otherwise it casts.
// We provide a small, self-contained C++11 test harness (no gtest) that
// exercises the true/false branches of both if statements and the
// else path via representative inputs.
// We rely on the external linkage of sqlite3RealToI64 from the C source.

#include <cstdio>
#include <cmath>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstdint>


// Link with the actual C implementation of sqlite3RealToI64.
// The function uses C linkage, so we declare it as extern "C".
extern "C" long long sqlite3RealToI64(double r);

// Lightweight non-terminating assertion framework (no abort on failure)
static int g_tests_passed = 0;
static int g_tests_total  = 0;

static void report_result(const char* test_name, bool passed, const char* msg) {
    ++g_tests_total;
    if (passed) {
        ++g_tests_passed;
        //PASS: test_name
        printf("PASS: %s\n", test_name);
    } else {
        //FAIL: test_name - msg
        printf("FAIL: %s - %s\n", test_name, msg ? msg : "no message");
    }
}

// Domain knowledge notes (for maintainers):
// - We test the two boundary-guard branches:
//   1) if (r < lower_bound) -> clamps to SMALLEST_INT64
//   2) if (r > upper_bound) -> clamps to LARGEST_INT64
// - For values in between, a plain cast (i64)r is performed.
// - We assume 64-bit signed range: [-9223372036854775808, 9223372036854775807]
// - The exact constants SMALLEST_INT64 / LARGEST_INT64 are provided by sqlite headers,
//   but for test expectations we can rely on the canonical limits above.

static void test_sqlite3RealToI64_belowLowerBound() {
    // Purpose: ensure the lower bound guard triggers and returns SMALLEST_INT64
    const long long EXPECTED = -9223372036854775807LL - 1; // SMALLEST_INT64
    double input = -1e100; // well below lower threshold
    long long actual = sqlite3RealToI64(input);
    if (actual == EXPECTED) {
        report_result("sqlite3RealToI64_belowLowerBound", true, nullptr);
    } else {
        char buf[256];
        std::snprintf(buf, sizeof(buf),
            "expected %lld, got %lld", EXPECTED, actual);
        report_result("sqlite3RealToI64_belowLowerBound", false, buf);
    }
}

static void test_sqlite3RealToI64_aboveUpperBound() {
    // Purpose: ensure the upper bound guard triggers and returns LARGEST_INT64
    const long long EXPECTED = 9223372036854775807LL; // LARGEST_INT64
    double input = 1e100; // well above upper threshold
    long long actual = sqlite3RealToI64(input);
    if (actual == EXPECTED) {
        report_result("sqlite3RealToI64_aboveUpperBound", true, nullptr);
    } else {
        char buf[256];
        std::snprintf(buf, sizeof(buf),
            "expected %lld, got %lld", EXPECTED, actual);
        report_result("sqlite3RealToI64_aboveUpperBound", false, buf);
    }
}

static void test_sqlite3RealToI64_withinPositive() {
    // Purpose: ensure proper casting path for a positive value within range
    const long long EXPECTED = 12345; // (i64)12345.678 -> 12345
    double input = 12345.678;
    long long actual = sqlite3RealToI64(input);
    if (actual == EXPECTED) {
        report_result("sqlite3RealToI64_withinPositive", true, nullptr);
    } else {
        char buf[256];
        std::snprintf(buf, sizeof(buf),
            "expected %lld, got %lld", EXPECTED, actual);
        report_result("sqlite3RealToI64_withinPositive", false, buf);
    }
}

static void test_sqlite3RealToI64_withinNegative() {
    // Purpose: ensure proper casting path for a negative value within range
    const long long EXPECTED = -98766; // (i64)-98766.09 -> -98766
    double input = -98766.09;
    long long actual = sqlite3RealToI64(input);
    if (actual == EXPECTED) {
        report_result("sqlite3RealToI64_withinNegative", true, nullptr);
    } else {
        char buf[256];
        std::snprintf(buf, sizeof(buf),
            "expected %lld, got %lld", EXPECTED, actual);
        report_result("sqlite3RealToI64_withinNegative", false, buf);
    }
}

int main() {
    // Run all tests
    test_sqlite3RealToI64_belowLowerBound();
    test_sqlite3RealToI64_aboveUpperBound();
    test_sqlite3RealToI64_withinPositive();
    test_sqlite3RealToI64_withinNegative();

    // Summary
    printf("Summary: %d/%d tests passed.\n", g_tests_passed, g_tests_total);
    // Return non-zero if any test failed (helps integrate with build systems)
    return (g_tests_total == g_tests_passed) ? 0 : 1;
}
// test_measure_time_stop.cpp
// Comprehensive unit tests for the focal method MeasureTimeStop in threaded_testbed.c
// This test uses function interposition to deterministically control the time values
// returned by timespec_get/clock_gettime, enabling deterministic verification
// of the elapsed calculation without modifying the production code.
//
// Notes:
// - We override: timespec_get and clock_gettime to supply controlled "finish" times.
// - We access the global time markers 'start' (extern "C" struct timespec start) to set the
//   starting point for elapsed calculation.
// - The test suite runs in a single executable without GoogleTest (as required).

#include <threaded_internal.h>
#include <iostream>
#include <time.h>
#include <memory.h>
#include <stdlib.h>
#include <cmath>
#include <ctime>


// Declarations of the focal function and its time markers
extern "C" double MeasureTimeStop(void);

// The production code uses a global 'start' timespec to compute elapsed time.
// We declare it here so tests can initialize it deterministically.
extern "C" struct timespec start;

// We will provide deterministic values for 'finish' by overriding the system time fetchers.
static time_t g_finish_sec = 0;
static long g_finish_nsec = 0;

// Interpose timespec_get to provide controlled 'finish' values.
// Signature matches the standard library function.
extern "C" int timespec_get(struct timespec* ts, int base)
{
    ts->tv_sec = g_finish_sec;
    ts->tv_nsec = g_finish_nsec;
    return 1;
}

// Interpose clock_gettime to provide controlled 'finish' values when the code path
// does not use timespec_get (i.e., when HAVE_TIMESPEC_GET is not defined).
extern "C" int clock_gettime(clockid_t clk_id, struct timespec* ts)
{
    (void)clk_id; // unused in test interposition
    ts->tv_sec = g_finish_sec;
    ts->tv_nsec = g_finish_nsec;
    return 0;
}

// Simple test harness state
static int g_failures = 0;

// Helper: compute elapsed expected value given start and finish, mirroring MeasureTimeStop
static double expected_elapsed(const struct timespec& st, time_t fin_sec, long fin_nsec)
{
    double seconds = static_cast<double>(fin_sec - st.tv_sec);
    double nanos   = static_cast<double>(fin_nsec - st.tv_nsec) / 1e9;
    return seconds + nanos;
}

// Test 1: Positive elapsed when start is in the past relative to finish
// We expect a positive elapsed value equal to (finish - start) with nanos precision.
static void test_positive_elapsed()
{
    // Arrange: set a known start in the past
    start.tv_sec = 10;
    start.tv_nsec = 123456789; // 0.123456789 s

    // Arrange: define a controlled finish in the future
    g_finish_sec = 13;
    g_finish_nsec = 987654321; // 0.987654321 s

    // Act
    double elapsed = MeasureTimeStop();

    // Expected: (13 - 10) + (987654321 - 123456789) / 1e9
    double expected = expected_elapsed(start, g_finish_sec, g_finish_nsec);

    // Assert: allow a tiny tolerance due to potential minor differences in timing logic
    const double tol = 1e-6;
    bool ok = std::fabs(elapsed - expected) <= tol;
    if (!ok) {
        std::cerr << "[FAIL] test_positive_elapsed: elapsed=" << elapsed
                  << " expected=" << expected
                  << " (start=" << start.tv_sec << "." << start.tv_nsec
                  << ", finish=" << g_finish_sec << "." << g_finish_nsec << ")\n";
        g_failures++;
    } else {
        std::cout << "[PASS] test_positive_elapsed\n";
    }
}

// Test 2: Negative elapsed when start is in the future relative to finish
// We expect a negative elapsed value equal to (finish - start).
static void test_negative_elapsed()
{
    // Arrange: set a future start
    start.tv_sec = 20;
    start.tv_nsec = 0;

    // Arrange: define finish earlier than start
    g_finish_sec = 15;
    g_finish_nsec = 0;

    // Act
    double elapsed = MeasureTimeStop();

    // Expected: (15 - 20) + (0 - 0) / 1e9 = -5.0
    double expected = -5.0;

    // Assert
    const double tol = 1e-6;
    bool ok = std::fabs(elapsed - expected) <= tol;
    if (!ok) {
        std::cerr << "[FAIL] test_negative_elapsed: elapsed=" << elapsed
                  << " expected=" << expected
                  << " (start=" << start.tv_sec << "." << start.tv_nsec
                  << ", finish=" << g_finish_sec << "." << g_finish_nsec << ")\n";
        g_failures++;
    } else {
        std::cout << "[PASS] test_negative_elapsed\n";
    }
}

// Main: run all tests in sequence
int main()
{
    std::cout << "Running tests for MeasureTimeStop...\n";

    test_positive_elapsed();
    test_negative_elapsed();

    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << g_failures << " test(s) failed.\n";
        return g_failures;
    }
}
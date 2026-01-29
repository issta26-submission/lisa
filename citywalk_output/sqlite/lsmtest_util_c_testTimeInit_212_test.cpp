/*
  TimeInit Unit Test Suite for testTimeInit (lsmtest_util.c)

  Purpose:
  - Validate the focal function testTimeInit, which initializes the global (presumably)
    time marker zero_time by invoking gettimeofday.
  - Since the implementation details of zero_time are not fully visible here, the tests
    rely on observable properties:
      1) After a call, zero_time should represent a non-negative wall-clock time.
      2) Consecutive invocations should yield non-decreasing time values (monotonicity).
  - This test suite is designed to be compiled and run in a C++11 environment without GTest.
  - It uses a lightweight, non-terminating assertion style to exercise code paths without aborting.

  Notes:
  - The test suite uses extern "C" declarations to access the C function under test
    and the time marker (zero_time) defined in the C source. If zero_time is not
    externally visible, linkage will fail; this test assumes zero_time is a global
    symbol accessible from other translation units.
  - The tests do not attempt to mock gettimeofday; instead, they observe the
    resulting value of zero_time and its monotonic progression after repeated calls.
*/

#include <string.h>
#include <lsmtest.h>
#include <cstdio>
#include <sys/time.h>
#include <stdio.h>
#include <stdarg.h>


// Declare the C function under test and the shared time marker used by testTimeInit.
// We assume zero_time is a global (non-static) variable defined in lsmtest_util.c.
extern "C" {
    void testTimeInit(void);
    extern struct timeval zero_time;
}

// Lightweight test harness (non-terminating, non-crashing assertions)
static int g_total = 0;
static int g_failed = 0;

#define TEST(name, cond) do { \
    g_total++; \
    if (!(cond)) { \
        g_failed++; \
        fprintf(stderr, "TEST FAILED: %s\n", name); \
    } else { \
        fprintf(stdout, "TEST PASSED: %s\n", name); \
    } \
} while (0)

/*
  Test 1: basic validity of zero_time after a single invocation
  - Reasoning: testTimeInit should populate zero_time with a valid current time value.
  - Observation: zero_time.tv_sec should be non-negative (time since epoch).
*/
static void test_timeInit_basic()
{
    // Trigger the initialization
    testTimeInit();

    // Inspect the observed time value after initialization
    struct timeval z = zero_time;
    bool is_valid = (z.tv_sec >= 0);
    // Also guard against impossible negative microseconds
    if (z.tv_usec < 0) {
        is_valid = false;
    }

    TEST("testTimeInit_basic: zero_time represents non-negative time after init", is_valid);
}

/*
  Test 2: monotonicity across successive invocations
  - Reasoning: Time should progress with real wall-clock time; hence a second call should
    yield a time value that is greater than or equal to the first.
*/
static void test_timeInit_monotonic()
{
    // First invocation and capture
    testTimeInit();
    struct timeval z1 = zero_time;

    // Second invocation
    testTimeInit();
    struct timeval z2 = zero_time;

    // Compare for non-decreasing time
    bool monotonic = false;
    if (z2.tv_sec > z1.tv_sec) {
        monotonic = true;
    } else if (z2.tv_sec == z1.tv_sec) {
        monotonic = (z2.tv_usec >= z1.tv_usec);
    }

    TEST("testTimeInit_monotonic: second init yields non-decreasing time", monotonic);
}

/*
  Entry point: run all tests and summarize results.
*/
int main(void)
{
    fprintf(stdout, "Running testTimeInit unit tests for lsmtest_util.c\n");

    test_timeInit_basic();
    test_timeInit_monotonic();

    fprintf(stdout, "\nTest Summary: Total=%d, Failed=%d\n", g_total, g_failed);
    return (g_failed == 0) ? 0 : 1;
}
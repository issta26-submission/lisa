/*
  Test suite for the focal method testTimeGet in lsmtest_util.c

  Strategy overview:
  - We treat testTimeGet as a C function (extern "C") and link against lsmtest_util.c.
  - We rely on the global symbol 'zero_time' (extern) to influence the outcome of testTimeGet.
  - We implement a lightweight, non-terminating test framework using EXPECT_TRUE-like assertions.
  - We cover several scenarios:
    1) Baseline: ensuring the function returns a non-negative value under normal conditions.
    2) Zero-time alignment: setting zero_time to the current time should yield a small non-negative duration.
    3) Future-zero_time: setting zero_time in the future should yield a non-positive value.
    4) Monotonic increase: after advancing time, the value should not decrease.

  Notes about dependencies:
  - Candidate Keywords inferred from the focal method and its dependencies:
    - gettimeofday, zero_time (global), struct timeval
    - time difference in milliseconds: (tv_sec difference * 1000) + (tv_usec difference)/1000
    - boundary handling via non-negative/negative values
    - static helpers in the test harness are present in lsmtest_util.c (stubs in <FOCAL_CLASS_DEP>)

  Important: This test is written in C++11, and avoids GTest. It calls the C function testTimeGet() directly and manipulates the extern/global zero_time when possible.
*/

#include <string.h>
#include <lsmtest.h>
#include <stdbool.h>
#include <cstdio>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>


// Linkers: declare the focal function with C linkage
extern "C" int testTimeGet(void);

// Access to the global zero_time used by testTimeGet
extern "C" struct timeval zero_time;

// Lightweight, non-terminating assertion mechanism
static int g_failures = 0;
#define EXPECT_TRUE(cond) \
  do { \
    if(!(cond)) { \
      fprintf(stderr, "EXPECT_TRUE failed: %s, in %s:%d\n", #cond, __FILE__, __LINE__); \
      ++g_failures; \
    } \
  } while(0)

/*
  Test 1: Baseline - ensure the function returns a non-negative value under normal conditions.
  Rationale: With zero_time defined (likely initialized elsewhere), testTimeGet should
  yield a non-negative duration since time since zero_time is non-decreasing.
*/
static void testTimeGet_Baseline(void) {
  long long val = testTimeGet();
  EXPECT_TRUE(val >= 0);
}

/*
  Test 2: Zero-time alignment - set zero_time to the current moment and verify a small non-negative duration.
  Rationale: By aligning zero_time with the current time, the elapsed time should be small and non-negative.
  Bound chosen conservatively to account for minor scheduling overheads.
*/
static void testTimeGet_SetZeroTime_NearNow(void) {
  struct timeval now;
  gettimeofday(&now, NULL);

  // Align zero_time to the exact current moment
  zero_time.tv_sec  = now.tv_sec;
  zero_time.tv_usec = now.tv_usec;

  long long val = testTimeGet();

  // Expect a small, non-negative duration
  EXPECT_TRUE(val >= 0);
  // Bound to catch gross anomalies; 2000 ms (2 seconds) is generous for a near-now alignment
  EXPECT_TRUE(val <= 2000);
}

/*
  Test 3: Future zero_time - set zero_time to a time in the future and verify non-positive duration.
  Rationale: If zero_time is in the future, the elapsed duration should be negative or zero.
  This covers the negative branch behavior of the calculation.
*/
static void testTimeGet_SetZeroTime_Future(void) {
  struct timeval now;
  gettimeofday(&now, NULL);

  // Place zero_time ~2 seconds in the future
  zero_time.tv_sec  = now.tv_sec + 2;
  zero_time.tv_usec = now.tv_usec;

  long long val = testTimeGet();

  // Expect non-positive value when zero_time is in the future
  EXPECT_TRUE(val <= 0);
  // Bound to guard against unexpectedly large negative values
  EXPECT_TRUE(val >= -3000);
}

/*
  Test 4: Monotonic increase - after advancing time, the duration should not decrease.
  Rationale: Since time progresses, and zero_time is fixed, subsequent calls should yield non-decreasing values.
*/
static void testTimeGet_MonotonicIncrease(void) {
  struct timeval base;
  gettimeofday(&base, NULL);

  // Fix zero_time to the current moment
  zero_time.tv_sec  = base.tv_sec;
  zero_time.tv_usec = base.tv_usec;

  long long v1 = testTimeGet();

  // Advance time by ~60ms
  usleep(60000);

  long long v2 = testTimeGet();

  // Expect non-decreasing sequence
  EXPECT_TRUE(v2 >= v1);
}

/*
  Main entry: run all tests and summarize results.
  If all expectations pass, print a success message; otherwise print the number of failures.
*/
int main(void) {
  printf("Starting testTimeGet suite\n");

  testTimeGet_Baseline();
  testTimeGet_SetZeroTime_NearNow();
  testTimeGet_SetZeroTime_Future();
  testTimeGet_MonotonicIncrease();

  if (g_failures == 0) {
    printf("ALL TESTS PASSED\n");
  } else {
    printf("TOTAL FAILURES: %d\n", g_failures);
  }

  // Non-zero exit code indicates failure for CI integration
  return (g_failures == 0) ? 0 : 1;
}
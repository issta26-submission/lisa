// Minimal C++11 test harness for sqlite3_hard_heap_limit64
// This test suite provides isolated, self-contained mocks to exercise
// the focal method's behavior without requiring the full SQLite project.
//
// Notes:
// - The test defines the necessary stubs for sqlite3_initialize, sqlite3_mutex_enter/leave,
//   and a minimal mem0 state to drive the focal function.
// - We cover: rc == 0 path, rc != 0 path, positive n updates hardLimit and alarmThreshold,
//   negative n leaves state unchanged, and the conditional alarmThreshold logic.

#include <cstdio>
#include <sqliteInt.h>
#include <cstdint>
#include <stdarg.h>


// Domain/types (mocked for test)
typedef long long sqlite3_int64;

// Lightweight dummy mutex type (no real locking needed for unit tests)
struct sqlite3_mutex { int dummy; };
typedef struct sqlite3_mutex sqlite3_mutex;

// Global mock to control sqlite3_initialize() return value
static int g_init_rc = 0;
int sqlite3_initialize() { return g_init_rc; }

// No-op mutex operations for testing
void sqlite3_mutex_enter(sqlite3_mutex *mx) { (void)mx; }
void sqlite3_mutex_leave(sqlite3_mutex *mx) { (void)mx; }

// Dependency: a minimal representation of the internal memory control block used by the focal function
struct Mem0 {
  sqlite3_mutex *mutex;
  sqlite3_int64 hardLimit;
  sqlite3_int64 alarmThreshold;
};

// Global instance used by the focal function
static Mem0 mem0 = { nullptr, 0, 0 };

// Focal method under test (reproduced in test to ensure self-contained compilation)
sqlite3_int64 sqlite3_hard_heap_limit64(sqlite3_int64 n){
  sqlite3_int64 priorLimit;
#ifndef SQLITE_OMIT_AUTOINIT
  int rc = sqlite3_initialize();
  if( rc ) return -1;
#endif
  sqlite3_mutex_enter(mem0.mutex);
  priorLimit = mem0.hardLimit;
  if( n>=0 ){
    mem0.hardLimit = n;
    if( n<mem0.alarmThreshold || mem0.alarmThreshold==0 ){
      mem0.alarmThreshold = n;
    }
  }
  sqlite3_mutex_leave(mem0.mutex);
  return priorLimit;
}

// Simple test assertions (non-terminating, to maximize coverage)
static int g_failures = 0;
#define ASSERT_EQ(expected, actual, msg) do { \
  if ((expected) != (actual)) { \
    printf("ASSERT FAILED: %s | expected: %lld, actual: %lld\n", (msg), (long long)(expected), (long long)(actual)); \
    ++g_failures; \
  } else { \
    printf("ASSERT PASSED: %s\n", (msg)); \
  } \
} while(0)

#define ASSERT_TRUE(cond, msg) do { \
  if (!(cond)) { \
    printf("ASSERT FAILED: %s\n", (msg)); \
    ++g_failures; \
  } else { \
    printf("ASSERT PASSED: %s\n", (msg)); \
  } \
} while(0)

// Helper to reset mem0 to known state
static void reset_mem0(sqlite3_int64 hard, sqlite3_int64 alarm)
{
  mem0.mutex = nullptr;
  mem0.hardLimit = hard;
  mem0.alarmThreshold = alarm;
}

/*
Test 1: rc == 0 path with n >= 0 and alarmThreshold == 0
- Prior limit should be returned as initial hardLimit.
- hardLimit should update to n.
- alarmThreshold should update to n (since previous alarmThreshold == 0).
*/
static void test_rc_zero_updates_all_when_alarm_zero()
{
  printf("Test 1: rc==0, n>=0, initial alarmThreshold==0\n");
  g_init_rc = 0;
  reset_mem0(100, 0); // prior limit = 100; alarmThreshold 0
  sqlite3_int64 ret = sqlite3_hard_heap_limit64(200); // n = 200
  ASSERT_EQ(100, ret, "Return priorLimit == 100");
  ASSERT_EQ(200, mem0.hardLimit, "hardLimit updated to 200");
  ASSERT_EQ(200, mem0.alarmThreshold, "alarmThreshold updated to 200");
  printf("\n");
}

/*
Test 2: rc == 0 path with n >= 0 and n < alarmThreshold (alarm should be lowered)
- Prior limit should be returned as initial hardLimit.
- hardLimit should update to n.
- alarmThreshold should update to n because n < previous alarmThreshold.
*/
static void test_rc_zero_lower_alarm_when_n_smaller()
{
  printf("Test 2: rc==0, n>=0, n < alarmThreshold\n");
  g_init_rc = 0;
  reset_mem0(100, 150); // prior limit = 100; alarmThreshold = 150
  sqlite3_int64 ret = sqlite3_hard_heap_limit64(120); // n = 120
  ASSERT_EQ(100, ret, "Return priorLimit == 100");
  ASSERT_EQ(120, mem0.hardLimit, "hardLimit updated to 120");
  ASSERT_EQ(120, mem0.alarmThreshold, "alarmThreshold lowered to 120");
  printf("\n");
}

/*
Test 3: rc == 0 path with n < 0 (no changes to mem0)
- priorLimit should be returned as current hardLimit.
- mem0.hardLimit and mem0.alarmThreshold should remain unchanged.
*/
static void test_rc_zero_negative_n_no_change()
{
  printf("Test 3: rc==0, n<0 (no state change)\n");
  g_init_rc = 0;
  reset_mem0(120, 60); // prior limit = 120; alarmThreshold = 60
  sqlite3_int64 ret = sqlite3_hard_heap_limit64(-5); // n = -5
  ASSERT_EQ(120, ret, "Return priorLimit == 120");
  ASSERT_EQ(120, mem0.hardLimit, "hardLimit remains 120");
  ASSERT_EQ(60, mem0.alarmThreshold, "alarmThreshold remains 60");
  printf("\n");
}

/*
Test 4: rc != 0 path (initialization failed)
- Should return -1 and not modify mem0.
*/
static void test_rc_non_zero_path_no_state_change()
{
  printf("Test 4: rc!=0 path (initialization failure)\n");
  g_init_rc = 1; // simulate sqlite3_initialize failure
  reset_mem0(42, 7); // current state
  sqlite3_int64 ret = sqlite3_hard_heap_limit64(1000);
  ASSERT_EQ(-1, ret, "Return -1 due to rc != 0");
  ASSERT_EQ(42, mem0.hardLimit, "hardLimit unchanged at 42");
  ASSERT_EQ(7, mem0.alarmThreshold, "alarmThreshold unchanged at 7");
  printf("\n");
}

int main()
{
  printf("Starting sqlite3_hard_heap_limit64 unit tests (self-contained C++11)\n\n");

  test_rc_zero_updates_all_when_alarm_zero();
  test_rc_zero_lower_alarm_when_n_smaller();
  test_rc_zero_negative_n_no_change();
  test_rc_non_zero_path_no_state_change();

  if (g_failures == 0) {
    printf("ALL TESTS PASSED\n");
  } else {
    printf("TOTAL FAILURES: %d\n", g_failures);
  }
  return (g_failures ? 1 : 0);
}
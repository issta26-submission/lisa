// Unit test suite for sqlite3_soft_heap_limit64
// Target environment: C++11, no GoogleTest, lightweight custom test framework.
// The test suite provides minimal stubs for the required SQLite-like runtime
// dependencies from malloc.c to exercise sqlite3_soft_heap_limit64 logic.

#include <iostream>
#include <sqliteInt.h>
#include <cstdint>
#include <stdarg.h>


// ------------------------------
// Domain-compatible types and stubs
// ------------------------------

// Minimal sqlite3_int64 type
typedef long long sqlite3_int64;

// Forward declaration of mutex type used by the focal code
// In the real project this is a platform-specific mutex type; here we keep it simple.
struct sqlite3_mutex {};

// Public-ish API surface used by sqlite3_soft_heap_limit64
int sqlite3_initialize(void);
void sqlite3_mutex_enter(sqlite3_mutex *mutex);
void sqlite3_mutex_leave(sqlite3_mutex *mutex);
sqlite3_int64 sqlite3StatusValue(int op);

// Operation code for sqlite3StatusValue
#define SQLITE_STATUS_MEMORY_USED 0

// Memory management hooks used by the focal function
sqlite3_int64 sqlite3_memory_used(void);
void sqlite3_release_memory(int n);

// Atomic store for boolean-like flag
void AtomicStore(int* addr, int value);

// ------------------------------
// Global/test harness state (mocks)
// ------------------------------

// Simple mutex object to be used by the focal function
static sqlite3_mutex testMutexObj;

// Simulated memory accounting
static sqlite3_int64 memoryUsed = 0;      // how much memory is currently "used"
static int lastReleaseValue = -1;         // last value passed to sqlite3_release_memory

// Simulated initialize outcome (0 = success)
static int simulateInitReturn = 0;

// The focal-class-like memory state (as in mem0 in the real code)
struct {
    sqlite3_mutex *mutex;
    sqlite3_int64 alarmThreshold;
    sqlite3_int64 hardLimit;
    int nearlyFull; // boolean flag stored as int
} mem0;

// Prototypes for the function under test (as in the real malloc.c)
sqlite3_int64 sqlite3_soft_heap_limit64(sqlite3_int64 n);

// ------------------------------
// Implementations (simple stubs/mocks)
// ------------------------------

int sqlite3_initialize(void) {
    return simulateInitReturn;
}

void sqlite3_mutex_enter(sqlite3_mutex *mutex) {
    // No real locking in this mock environment
    (void)mutex;
}

void sqlite3_mutex_leave(sqlite3_mutex *mutex) {
    (void)mutex;
}

sqlite3_int64 sqlite3StatusValue(int op) {
    if (op == SQLITE_STATUS_MEMORY_USED) {
        return memoryUsed;
    }
    return 0;
}

sqlite3_int64 sqlite3_memory_used(void) {
    return memoryUsed;
}

void sqlite3_release_memory(int n) {
    // Decrease simulated memory and record what was requested
    lastReleaseValue = n;
    if (n < 0) return;
    if (memoryUsed >= (sqlite3_int64)n) {
        memoryUsed -= (sqlite3_int64)n;
    } else {
        memoryUsed = 0;
    }
}

void AtomicStore(int* addr, int value) {
    *addr = value;
}

// ------------------------------
// The focal method implementation (copied/stubbed for unit tests)
// ------------------------------
sqlite3_int64 sqlite3_soft_heap_limit64(sqlite3_int64 n){
  sqlite3_int64 priorLimit;
  sqlite3_int64 excess;
  sqlite3_int64 nUsed;
#ifndef SQLITE_OMIT_AUTOINIT
  int rc = sqlite3_initialize();
  if( rc ) return -1;
#endif
  sqlite3_mutex_enter(mem0.mutex);
  priorLimit = mem0.alarmThreshold;
  if( n<0 ){
    sqlite3_mutex_leave(mem0.mutex);
    return priorLimit;
  }
  if( mem0.hardLimit>0 && (n>mem0.hardLimit || n==0) ){
    n = mem0.hardLimit;
  }
  mem0.alarmThreshold = n;
  nUsed = sqlite3StatusValue(SQLITE_STATUS_MEMORY_USED);
  AtomicStore(&mem0.nearlyFull, n>0 && n<=nUsed);
  sqlite3_mutex_leave(mem0.mutex);
  excess = sqlite3_memory_used() - n;
  if( excess>0 ) sqlite3_release_memory((int)(excess & 0x7fffffff));
  return priorLimit;
}

// ------------------------------
// Test framework (lightweight)
// ------------------------------
static int tests_run = 0;
static int tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++tests_run; \
    if(!(cond)){ \
        std::cerr << "TEST FAILURE: " << msg << " (line " << __LINE__ << ")" << std::endl; \
        ++tests_failed; \
    } \
} while(0)

#define EXPECT_EQ(a,b, msg) do { \
    ++tests_run; \
    if((a) != (b)){ \
        std::cerr << "TEST FAILURE: " << msg << " - expected: " << (b) << ", actual: " << (a) << " (line " << __LINE__ << ")" << std::endl; \
        ++tests_failed; \
    } \
} while(0)

#define RESET_ENV() do { \
    mem0.mutex = &testMutexObj; \
    mem0.alarmThreshold = 20; \
    mem0.hardLimit = 100; \
    mem0.nearlyFull = 0; \
    memoryUsed = 0; \
    lastReleaseValue = -1; \
    simulateInitReturn = 0; \
} while(0)

static void test_initialize_failure_path() {
    // Test that rc non-zero results in -1 return and no state change
    RESET_ENV();
    simulateInitReturn = 1; // simulate sqlite3_initialize failure
    sqlite3_int64 res = sqlite3_soft_heap_limit64(50);
    EXPECT_EQ(res, -1, "initialize failure should return -1");
    // No state change expected because we return early
    EXPECT_EQ(mem0.alarmThreshold, 20, "alarmThreshold should remain unchanged after init failure");
}

static void test_negative_n_returns_prior_limit() {
    RESET_ENV();
    mem0.alarmThreshold = 42;
    sqlite3_int64 res = sqlite3_soft_heap_limit64(-5);
    EXPECT_EQ(res, 42, "negative n should return priorLimit without changes");
    EXPECT_EQ(mem0.alarmThreshold, 42, "alarmThreshold should stay the same for negative n");
}

static void test_clamp_to_hard_limit_and_zero() {
    RESET_ENV();
    // Case a: n == 0 should be clamped to hardLimit
    mem0.hardLimit = 100;
    sqlite3_int64 resA = sqlite3_soft_heap_limit64(0);
    EXPECT_EQ(resA, 20, "priorLimit should be returned when clamping; initial priorLimit was 20");
    EXPECT_EQ(mem0.alarmThreshold, (sqlite3_int64)100, "n should be clamped to hardLimit (100)");

    // Case b: n > hardLimit should clamp to hardLimit as well
    RESET_ENV();
    mem0.hardLimit = 100;
    sqlite3_int64 resB = sqlite3_soft_heap_limit64(150);
    EXPECT_EQ(resB, 20, "priorLimit preserved when clamping to hardLimit");
    EXPECT_EQ(mem0.alarmThreshold, (sqlite3_int64)100, "n should be clamped to hardLimit (100) again");
}

static void test_nearly_full_flag_behavior() {
    RESET_ENV();
    mem0.hardLimit = 100;
    // Set memory used to a value that will influence nearlyFull flag
    memoryUsed = 60; // nUsed
    sqlite3_int64 res = sqlite3_soft_heap_limit64(50); // within hardLimit
    EXPECT_EQ(res, 20, "priorLimit returned after updating alarmThreshold");
    EXPECT_EQ(mem0.alarmThreshold, (sqlite3_int64)50, "alarmThreshold should be updated to 50");
    EXPECT_EQ(mem0.nearlyFull, 1, "nearlyFull should be true when n > 0 and n <= nUsed (50 <= 60)");
}

static void test_memory_excess_triggers_release() {
    RESET_ENV();
    mem0.hardLimit = 0; // free from hardLimit constraints
    memoryUsed = 200;   // nUsed
    sqlite3_int64 res = sqlite3_soft_heap_limit64(100); // n
    EXPECT_EQ(res, 20, "priorLimit returned when setting 100");
    EXPECT_EQ(mem0.alarmThreshold, (sqlite3_int64)100, "alarmThreshold should be updated to 100");
    EXPECT_EQ(lastReleaseValue, 100, "release_memory should be called with excess amount (200-100=100)");
}

static void test_no_release_when_excess_zero() {
    RESET_ENV();
    memoryUsed = 100;
    sqlite3_int64 res = sqlite3_soft_heap_limit64(100);
    EXPECT_EQ(res, 20, "priorLimit returned when n equals memoryUsed");
    EXPECT_EQ(mem0.alarmThreshold, (sqlite3_int64)100, "alarmThreshold updated to 100");
    EXPECT_EQ(lastReleaseValue, -1, "release_memory should not be called when excess == 0");
}

static void run_all_tests() {
    test_initialize_failure_path();
    test_negative_n_returns_prior_limit();
    test_clamp_to_hard_limit_and_zero();
    test_nearly_full_flag_behavior();
    test_memory_excess_triggers_release();
    test_no_release_when_excess_zero();
}

// ------------------------------
// Main entry
// ------------------------------
int main() {
    // Initialize environment
    mem0.mutex = &testMutexObj;
    RESET_ENV();

    run_all_tests();

    std::cout << "Tests run: " << tests_run << ", Failures: " << tests_failed << std::endl;
    return (tests_failed == 0) ? 0 : 1;
}
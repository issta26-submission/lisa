// A self-contained C++11 test harness for the focal method sqlite3_wal_hook.
// This test suite is designed to be compiled alongside the focal function
// in a simplified, self-contained environment (no GTest required).
// The goal is to validate the core behavioral branches of sqlite3_wal_hook
// (as described in the provided focal method) using runtime-style flags to
// emulate compile-time macro branches (OMIT_WAL and API_ARMOR).

#include <cstddef>
#include <cstdio>
#include <sqliteInt.h>
#include <cstdint>


// Domain-adapted lightweight test harness
static int g_test_failures = 0;
#define TEST_OK 1

#define EXPECT_TRUE(cond) \
  do { if (!(cond)) { \
      printf("TEST FAILURE: %s:%d: EXPECT_TRUE(%s) failed\n", __FILE__, __LINE__, #cond); \
      ++g_test_failures; \
    } \
  } while (0)

#define EXPECT_FALSE(cond) \
  do { if (cond) { \
      printf("TEST FAILURE: %s:%d: EXPECT_FALSE(%s) failed\n", __FILE__, __LINE__, #cond); \
      ++g_test_failures; \
    } \
  } while (0)

#define EXPECT_EQ(a,b) \
  do { if (!((a) == (b))) { \
      printf("TEST FAILURE: %s:%d: EXPECT_EQ(%p, %p) failed\n", __FILE__, __LINE__, (void*)(a), (void*)(b)); \
      ++g_test_failures; \
    } \
  } while (0)

#define EXPECT_NEQ(a,b) \
  do { if ((a) == (b)) { \
      printf("TEST FAILURE: %s:%d: EXPECT_NEQ(%p, %p) failed\n", __FILE__, __LINE__, (void*)(a), (void*)(b)); \
      ++g_test_failures; \
    } \
  } while (0)


// Minimal, self-contained simulation of sqlite3 types and accessors used by sqlite3_wal_hook

// Forward declare internal sqlite3 type (opaque in public API)
struct sqlite3;

// Minimal representation of sqlite3_mutex (opaque in public API)
typedef struct sqlite3_mutex sqlite3_mutex;

// Global counters to simulate mutex activity
static int g_mutex_enter_count = 0;
static int g_mutex_leave_count = 0;

// Runtime toggles to emulate different macro configurations
static int g_runtime_omit_wal = 0;       // Emulates SQLITE_OMIT_WAL (0 = enabled, 1 = omitted)
static int g_runtime_api_armor = 1;      // Emulates SQLITE_ENABLE_API_ARMOR (1 = enabled, 0 = disabled)
static int g_runtime_safety_ok = 1;      // Emulates sqlite3SafetyCheckOk(db) result

// A tiny sqlite3-like mutex object for testing (no real threading)
struct sqlite3_mutex {
  int dummy;
};

// The sqlite3 structure shape required by the focal method.
// We only model the members touched by sqlite3_wal_hook.
struct sqlite3 {
  sqlite3_mutex *mutex;             // Mutex used to guard WAL hook state
  void *pWalArg;                    // Argument passed to wal callback
  int (*xWalCallback)(void *, sqlite3*, const char*, int); // Callback function pointer
};

// Function prototypes that sqlite3_wal_hook depends on in the focal environment
static int sqlite3SafetyCheckOk(sqlite3 *db) {
  // Runtime-controlled safety check return value
  return g_runtime_safety_ok;
}
static void sqlite3_mutex_enter(sqlite3_mutex *mutex) {
  // Increment enter counter to verify mutex usage in tests
  (void)mutex; // suppress unused if optimized out
  ++g_mutex_enter_count;
}
static void sqlite3_mutex_leave(sqlite3_mutex *mutex) {
  (void)mutex;
  ++g_mutex_leave_count;
}

// A macro placeholder for the original MISUSE_BKPT symbol
#define SQLITE_MISUSE_BKPT 0

// The focal method under test (re-implemented here in a self-contained manner)
void *sqlite3_wal_hook(
  sqlite3 *db,                    /* Attach the hook to this db handle */
  int(*xCallback)(void *, sqlite3*, const char*, int),
  void *pArg                      /* First argument passed to xCallback() */
){
  // Baseline: emulate the behavior described in the focal method.
  // Note: This is a self-contained replica for unit testing in isolation.
#ifndef SQLITE_OMIT_WAL
  void *pRet;
#ifdef SQLITE_ENABLE_API_ARMOR
  if( !sqlite3SafetyCheckOk(db) ){
    (void)SQLITE_MISUSE_BKPT;
    return 0;
  }
#endif
  // Simulated mutex behavior around update
  sqlite3_mutex_enter(db->mutex);
  pRet = db->pWalArg;
  db->xWalCallback = xCallback;
  db->pWalArg = pArg;
  sqlite3_mutex_leave(db->mutex);
  return pRet;
#else
  return 0;
#endif
}

// Helper to create and initialize a fake sqlite3 database handle for tests
static sqlite3* create_fake_db(sqlite3_mutex* mtx, void* initialArg, int initialCallbackExists) {
  sqlite3* db = new sqlite3;
  db->mutex = mtx;
  db->pWalArg = initialArg;
  if (initialCallbackExists) {
    // For testing, assign a dummy non-null callback to demonstrate state changes
    db->xWalCallback = [](void*, sqlite3*, const char*, int)->int { return 0; };
  } else {
    db->xWalCallback = nullptr;
  }
  return db;
}

// Test 1: When WAL hook is omitted (emulated by runtime flag), the function should return 0
static void test_omit_wal_path() {
  // Arrange
  g_runtime_omit_wal = 1; // simulate SQLITE_OMIT_WAL
  g_runtime_api_armor = 1;
  g_runtime_safety_ok = 1;

  sqlite3_mutex mtx;
  sqlite3* db = create_fake_db(&mtx, nullptr, 0);
  void* prev = nullptr;

  // Act
  prev = sqlite3_wal_hook(db, nullptr, reinterpret_cast<void*>(0xdeadbeef));

  // Assert
  EXPECT_EQ(prev, nullptr);
  EXPECT_EQ(g_mutex_enter_count, 0);  // mutex should not be entered when WAL is omitted
  EXPECT_EQ(g_mutex_leave_count, 0);  // mutex should not be entered at all

  // Cleanup
  delete db;
  // Reset state for subsequent tests
  g_runtime_omit_wal = 0;
  g_mutex_enter_count = g_mutex_leave_count = 0;
}

// Test 2: When API_ARMOR is enabled, and safety check fails, the function should return 0
static void test_safety_check_fails() {
  // Arrange
  g_runtime_omit_wal = 0;
  g_runtime_api_armor = 1;
  g_runtime_safety_ok = 0; // simulate a failing safety check

  sqlite3_mutex mtx;
  sqlite3* db = create_fake_db(&mtx, reinterpret_cast<void*>(0x1234), 1);

  void* prev = nullptr;

  // Act
  prev = sqlite3_wal_hook(db, [](void*, sqlite3*, const char*, int)->int { return 0; }, reinterpret_cast<void*>(0xdeadbeef));

  // Assert
  EXPECT_EQ(prev, nullptr);
  // In this path, mutex should not be entered due to early return
  EXPECT_EQ(g_mutex_enter_count, 0);
  EXPECT_EQ(g_mutex_leave_count, 0);

  // Cleanup
  delete db;
  g_runtime_safety_ok = 1;
  g_mutex_enter_count = g_mutex_leave_count = 0;
}

// Test 3: Normal path: update the hook and return the previous pWalArg value
static void test_normal_path_updates_state() {
  // Arrange
  g_runtime_omit_wal = 0;
  g_runtime_api_armor = 1;
  g_runtime_safety_ok = 1;

  sqlite3_mutex mtx;
  void* initialArg = reinterpret_cast<void*>(0xA1A1);
  void* newArg = reinterpret_cast<void*>(0xB2B2);

  // Initial callback is NULL to ensure a well-defined previous value (NULL)
  sqlite3* db = create_fake_db(&mtx, initialArg, 0);

  // Define two distinct callback pointers to simulate different callbacks
  int dummy_cb1(void*, sqlite3*, const char*, int){ return 0; }
  int dummy_cb2(void*, sqlite3*, const char*, int){ return 0; }

  // Act
  void* prev = sqlite3_wal_hook(db, dummy_cb2, newArg);

  // Assert: previous argument should be initialArg
  EXPECT_EQ(prev, initialArg);
  // After call, internal state should reflect the updates
  EXPECT_TRUE(db->pWalArg == newArg);
  EXPECT_TRUE(db->xWalCallback == dummy_cb2);
  // Mutex should have been entered and left exactly once
  EXPECT_EQ(g_mutex_enter_count, 1);
  EXPECT_EQ(g_mutex_leave_count, 1);

  // Cleanup
  delete db;
  g_mutex_enter_count = g_mutex_leave_count = 0;
}

// Entry point to run tests
int main() {
  printf("Starting sqlite3_wal_hook unit tests (self-contained harness)\n");

  test_omit_wal_path();
  test_safety_check_fails();
  test_normal_path_updates_state();

  if (g_test_failures == 0) {
    printf("ALL tests PASSED\n");
    return 0;
  } else {
    printf("TOTAL TEST FAILURES: %d\n", g_test_failures);
    return 1;
  }
}
// Test suite for sqlite3NoopMutex (focal method) in mutex_noop.c
// Target: C++11, no GoogleTest, lightweight custom test harness with
// non-terminating assertions (logging) to maximize coverage.
// The tests exercise both builds with and without SQLITE_DEBUG, and, when
// SQLITE_MUTEX_NOOP is defined, test sqlite3DefaultMutex aliasing.
//
// Note: This test file assumes the mutex_noop.c translation unit is linked
// into the test binary. We intentionally avoid calling the actual
// initialization/exit functions to prevent undefined behavior from functions
// with missing return statements in the provided static implementations
// (e.g., noopMutexInit/End). Instead, we validate structural/behavioral
// contracts exposed via sqlite3_mutex_methods function pointers.

#include <cstddef>
#include <iostream>
#include <sqliteInt.h>
#include <cassert>


// Forward declare C interfaces/types from the focal file.
// We mirror the minimal subset needed to inspect the returned
// sqlite3_mutex_methods structure without pulling in the full SQLite headers.

extern "C" {

// Opaque struct used by sqlite3_mutex methods (only pointer types are used here)
struct sqlite3_mutex; // opaque

// Layout-representative of sqlite3_mutex_methods as used by sqlite3NoopMutex
typedef struct sqlite3_mutex_methods {
  int (*xInit)(void);
  int (*xEnd)(void);
  struct sqlite3_mutex *(*xAlloc)(int);
  void (*xFree)(struct sqlite3_mutex *);
  void (*xEnter)(struct sqlite3_mutex *);
  int (*xTry)(struct sqlite3_mutex *);
  void (*xLeave)(struct sqlite3_mutex *);
  int (*xHeld)(struct sqlite3_mutex *);
  int (*xNotheld)(struct sqlite3_mutex *);
} sqlite3_mutex_methods;

// Declare the focal function. The actual implementation lives in mutex_noop.c.
sqlite3_mutex_methods const *sqlite3NoopMutex(void);

// Optional alias if SQLITE_MUTEX_NOOP is defined
#ifdef SQLITE_MUTEX_NOOP
sqlite3_mutex_methods const *sqlite3DefaultMutex(void);
#endif

} // extern "C"

// Simple non-terminating test harness (EXPECT-like macros)
static int g_failures = 0;
#define LOG_FAIL(msg) do { std::cerr << "FAIL: " << (msg) << std::endl; } while(0)
#define EXPECT_TRUE(cond, msg) do { if(!(cond)) { ++g_failures; LOG_FAIL(msg); } } while(0)
#define EXPECT_FALSE(cond, msg) do { if(cond) { ++g_failures; LOG_FAIL(msg); } } while(0)
#define EXPECT_EQ(a, b, msg) do { if((a) != (b)) { ++g_failures; std::cerr << "FAIL: " << (msg)  \
    << " | Expected: " << (b) << " Got: " << (a) << std::endl; } } while(0)
#define EXPECT_SAME(a, b, msg) do { if((const void*)(a) != (const void*)(b)) { ++g_failures; \
    std::cerr << "FAIL: " << (msg) << " | Expected same address. Got: " << (void*)(a) \
    << " vs " << (void*)(b) << std::endl; } } while(0)

// Simple dummy structure to exercise debug mutex behavior.
// Must mirror the layout of sqlite3_debug_mutex { int id; int cnt; }
struct DummyDebugMutex {
  int id;
  int cnt;
};

int main() {
  // Candidate tests are split by compile-time macros to cover both configurations.

  // Test 1: Basic NOOP mutex structure (non-debug path)
  // This test is compiled when SQLITE_DEBUG is NOT defined.
#ifndef SQLITE_DEBUG
  {
    const sqlite3_mutex_methods* m1 = sqlite3NoopMutex();
    EXPECT_TRUE(m1 != nullptr, "sqlite3NoopMutex() should return a non-null pointer (NOOP).");
    const sqlite3_mutex_methods* m2 = sqlite3NoopMutex();
    EXPECT_SAME(m1, m2, "sqlite3NoopMutex() should return the same pointer on subsequent calls.");

    // All initialization-related hooks should be non-null in the NOOP variant
    EXPECT_TRUE(m1->xInit != nullptr, "xInit should be non-null in NOOP variant.");
    EXPECT_TRUE(m1->xEnd != nullptr, "xEnd should be non-null in NOOP variant.");
    EXPECT_TRUE(m1->xAlloc != nullptr, "xAlloc should be non-null in NOOP variant.");
    EXPECT_TRUE(m1->xFree != nullptr, "xFree should be non-null in NOOP variant.");
    EXPECT_TRUE(m1->xEnter != nullptr, "xEnter should be non-null in NOOP variant.");
    EXPECT_TRUE(m1->xTry != nullptr, "xTry should be non-null in NOOP variant.");
    EXPECT_TRUE(m1->xLeave != nullptr, "xLeave should be non-null in NOOP variant.");

    // In NOOP mode, the last two hooks (xHeld/xNotheld) are typically NULL
    EXPECT_TRUE(m1->xHeld == nullptr, "xHeld should be NULL in NOOP (non-debug) variant.");
    EXPECT_TRUE(m1->xNotheld == nullptr, "xNotheld should be NULL in NOOP (non-debug) variant.");
  }
#else
  // If SQLITE_DEBUG is defined, skip NOOP basic test in this block
  (void)0;
#endif // SQLITE_DEBUG

  // Test 2: Debug path behavior of sqlite3NoopMutex (when SQLITE_DEBUG is defined)
#ifndef NDEBUG
  // This test exercises the debug variant by checking non-null function pointers
  // and validating their runtime behavior through a crafted dummy mutex object.
#ifdef SQLITE_DEBUG
  {
    const sqlite3_mutex_methods* m = sqlite3NoopMutex();
    EXPECT_TRUE(m != nullptr, "sqlite3NoopMutex() should return a non-null pointer (DEBUG).");
    // In DEBUG path, xHeld and xNotheld are set to debug-specific functions.
    EXPECT_TRUE(m->xHeld != nullptr, "debug xHeld should be non-null in DEBUG variant.");
    EXPECT_TRUE(m->xNotheld != nullptr, "debug xNotheld should be non-null in DEBUG variant.");

    // The debug mutex helpers operate on a layout identical to sqlite3_debug_mutex
    // We pass a pointer to a DummyDebugMutex with different cnt values to exercise logic.
    DummyDebugMutex d;
    d.id = 0; // value doesn't affect logic for test
    d.cnt = 0;

    // When cnt == 0, debugMutexHeld should return 0 and debugMutexNotheld should return 1
    int held0 = m->xHeld(reinterpret_cast<sqlite3_mutex*>(&d));
    int notheld0 = m->xNotheld(reinterpret_cast<sqlite3_mutex*>(&d));
    EXPECT_EQ(held0, 0, "With cnt=0, xHeld() should return 0 (not held).");
    EXPECT_EQ(notheld0, 1, "With cnt=0, xNotheld() should return 1 (not held).");

    // Change state to simulate a held mutex (cnt > 0)
    d.cnt = 3;
    int held1 = m->xHeld(reinterpret_cast<sqlite3_mutex*>(&d));
    int notheld1 = m->xNotheld(reinterpret_cast<sqlite3_mutex*>(&d));
    EXPECT_EQ(held1, 1, "With cnt>0, xHeld() should return 1 (held).");
    EXPECT_EQ(notheld1, 0, "With cnt>0, xNotheld() should return 0 (not notheld).");
  }
#else
  // If SQLITE_DEBUG is not defined, skip this block
  (void)0;
#endif // SQLITE_DEBUG
#endif // NDEBUG

  // Test 3: sqlite3DefaultMutex aliasing (only when SQLITE_MUTEX_NOOP is defined)
#ifdef SQLITE_MUTEX_NOOP
  {
    const sqlite3_mutex_methods* a = sqlite3NoopMutex();
    const sqlite3_mutex_methods* b = sqlite3DefaultMutex();
    EXPECT_TRUE(a != nullptr, "sqlite3NoopMutex() must be non-null for alias test.");
    EXPECT_TRUE(b != nullptr, "sqlite3DefaultMutex() must be non-null for alias test.");
    EXPECT_SAME(a, b, "sqlite3DefaultMutex() should return the same pointer as sqlite3NoopMutex().");
  }
#else
  // If SQLITE_MUTEX_NOOP is not defined, this test is not applicable.
#endif

  // Summary of test results
  if (g_failures == 0) {
    std::cout << "All tests passed." << std::endl;
    return 0;
  } else {
    std::cerr << g_failures << " test(s) failed." << std::endl;
    return g_failures;
  }
}
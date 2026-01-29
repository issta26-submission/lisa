// Unit test suite for the focal method: sqlite3_collation_needed
// This file provides a self-contained C++11 test harness that exercises
// the core behavior of sqlite3_collation_needed as described in the prompt.
// Note: In a real project, sqlite3_collation_needed belongs to the internal
// SQLite codebase and relies on internal structures. For the purposes of
// this exercise, we implement a minimal in-memory mock of the relevant
// pieces to validate the method's essential logic (assignment of callbacks,
// arg propagation, and mutex coordination). The test suite uses a lightweight
//, non-terminating assertion mechanism (EXPECT_* macros) and runs in main().

#include <iostream>
#include <cstring>
#include <sqliteInt.h>
#include <cstdint>


// Domain knowledge and constraints (per <DOMAIN_KNOWLEDGE>):
// - Use only standard library and provided methods.
// - Do not rely on GTest; provide a simple, non-terminating assertion system.
// - Do not rely on private methods/fields of the real implementation; this
//   test provides a minimal, self-contained stand-in for the core behavior.
// - Access static-like behavior via globals/state within this translation unit.
// - Use the correct namespace (none required here; using global scope for C-style API).

// Step 1 (Program Understanding recap):
// The focal method sqlite3_collation_needed(db, pCollNeededArg, xCollNeeded)
// is intended to register a collation-needed callback for a database handle.
// It, under a mutex, stores the provided callback, resets the 16-bit variant
// to 0, stores the user argument, and returns SQLITE_OK. If API_ARMOR is
// enabled, it may early-return a misuse code if safety checks fail.

// To enable testing of both branches for API armor, we provide a compilable
// mode guarded by a macro. We also implement a tiny fake mutex API to exercise
// the critical section entry/exit without real multithreading in tests.

// Lightweight test framework (non-terminating, suitable for maximizing coverage)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " #cond " at " __FILE__ ":" << __LINE__ << "\n"; \
        ++g_tests_failed; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    ++g_tests_run; \
    if (!((a) == (b))) { \
        std::cerr << "EXPECT_EQ failed: " #a " (" << (a) << ") != " #b " (" << (b) << ") at " \
                  << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_tests_failed; \
    } \
} while(0)

#define EXPECT_PTR_EQ(a, b) do { \
    ++g_tests_run; \
    if (!(((void*)(a)) == ((void*)(b)))) { \
        std::cerr << "EXPECT_PTR_EQ failed: " #a " (" << (void*)(a) << ") != " #b " (" \
                  << (void*)(b) << ") at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_tests_failed; \
    } \
} while(0)

// Minimal mock/types to emulate the required internal structure for sqlite3_collation_needed.
// This is a lightweight stand-in; it is not a full SQLite implementation.

typedef struct sqlite3_mutex sqlite3_mutex; // opaque in test; treated as opaque
typedef struct sqlite3 sqlite3;

// Function prototypes that the focal method uses.
// In real SQLite, these are internal; here we provide minimal stubs/types for testing.

extern "C" {
    // Mutex enter/leave stubs (no real synchronization in test)
    void sqlite3_mutex_enter(sqlite3_mutex*);

    void sqlite3_mutex_leave(sqlite3_mutex*);
}

// A minimal sqlite3 struct with only the fields touched by sqlite3_collation_needed.
// The real SQLite struct has many more fields; we only expose those required by the test.
struct sqlite3 {
    sqlite3_mutex *mutex;                          // used by sqlite3_mutex_enter/leave
    void (*xCollNeeded)(void*,sqlite3*,int, const char*); // callback for 8-bit text
    void *pCollNeededArg;                          // user data for callback
    int xCollNeeded16;                              // 16-bit callback (set to 0 in function)
};

// Global hooks to observe mutex entry/exit in tests
static bool g_mutex_entered = false;
static bool g_mutex_left = false;

// Stubbed mutex enter/leave implementations to observe invocation
void sqlite3_mutex_enter(sqlite3_mutex* /*mutex*/) {
    g_mutex_entered = true;
}
void sqlite3_mutex_leave(sqlite3_mutex* /*mutex*/) {
    g_mutex_left = true;
}

// Optional API-Armor testing: provide a synthetic safety check function
// that the focal method will call when SQLITE_ENABLE_API_ARMOR is defined.
// We expose a small toggle so tests can exercise both branches.
#ifdef SQLITE_ENABLE_API_ARMOR
static int s_safety_ok = 1;
static int sqlite3SafetyCheckOk(sqlite3* /*db*/) {
    return s_safety_ok;
}
#endif

// The focal function under test (reproduced in this test file for isolation).
// This mirrors the logic described in the prompt and the enclosed <FOCAL_METHOD>.
// Note: In a real environment, this function would be linked from main.c.
// Here we provide a self-contained version to validate behavior in tests.
int sqlite3_collation_needed(
  sqlite3 *db,
  void *pCollNeededArg,
  void(*xCollNeeded)(void*,sqlite3*,int eTextRep,const char*)
){
#ifdef SQLITE_ENABLE_API_ARMOR
  if( !sqlite3SafetyCheckOk(db) ) return SQLITE_MISUSE_BKPT;
#endif
  sqlite3_mutex_enter(db->mutex);
  db->xCollNeeded = xCollNeeded;
  db->xCollNeeded16 = 0;
  db->pCollNeededArg = pCollNeededArg;
  sqlite3_mutex_leave(db->mutex);
  return 0; // SQLITE_OK
}

// Public API constants for test alignment.
// In real code these would come from sqlite3.h
static const int SQLITE_OK = 0;
static const int SQLITE_MISUSE_BKPT = 1;

// A trivial dummy callback to be registered
static void dummy_xCollNeeded(void* pUser, sqlite3* db, int eTextRep, const char* zName) {
    // No-op in test; the test validates that the pointer is stored correctly.
    (void)pUser;
    (void)db;
    (void)eTextRep;
    (void)zName;
}

// A second dummy callback for overwrite-testing
static void second_xCollNeeded(void* pUser, sqlite3* db, int eTextRep, const char* zName) {
    (void)pUser;
    (void)db;
    (void)eTextRep;
    (void)zName;
}

// Step 2: Unit Test Generation
// Test 1: Basic registration stores callback, arg, and resets 16-bit field.
// Test 2: Re-registration overwrites previous callback/arg and keeps 16-bit at 0.
// Test 3: API armor path returns misuse code when safety check fails (requires macro defined).
// Test 4: API armor path succeeds when safety check passes and behaves as in Test 1.

static void test_basic_registration_stores_callbacks_and_arg() {
    // Prepare a fake db object
    sqlite3 db;
    sqlite3_mutex dummyMutex;
    db.mutex = &dummyMutex;
    db.xCollNeeded = nullptr;
    db.pCollNeededArg = nullptr;
    db.xCollNeeded16 = 123; // non-zero to ensure it gets reset

    g_mutex_entered = false;
    g_mutex_left = false;

    void* testArg = (void*)0xDEADBEEF;
    // Call the focal function
    int rc = sqlite3_collation_needed(&db, testArg, dummy_xCollNeeded);

    // Assertions
    EXPECT_EQ(rc, SQLITE_OK);
    EXPECT_TRUE(g_mutex_entered);
    EXPECT_TRUE(g_mutex_left);
    EXPECT_PTR_EQ(db.xCollNeeded, dummy_xCollNeeded);
    EXPECT_EQ(db.pCollNeededArg, testArg);
    EXPECT_EQ(db.xCollNeeded16, 0);
}

static void test_overwrite_on_second_call() {
    sqlite3 db;
    sqlite3_mutex dummyMutex;
    db.mutex = &dummyMutex;
    db.xCollNeeded = nullptr;
    db.pCollNeededArg = nullptr;
    db.xCollNeeded16 = 999;

    // First registration
    void* firstArg = (void*)0x11111111;
    int rc1 = sqlite3_collation_needed(&db, firstArg, dummy_xCollNeeded);
    EXPECT_EQ(rc1, SQLITE_OK);
    EXPECT_PTR_EQ(db.xCollNeeded, dummy_xCollNeeded);
    EXPECT_EQ(db.pCollNeededArg, firstArg);
    EXPECT_EQ(db.xCollNeeded16, 0);

    // Second registration with a different callback and arg
    void* secondArg = (void*)0x22222222;
    int rc2 = sqlite3_collation_needed(&db, secondArg, second_xCollNeeded);
    EXPECT_EQ(rc2, SQLITE_OK);
    EXPECT_PTR_EQ(db.xCollNeeded, second_xCollNeeded);
    EXPECT_EQ(db.pCollNeededArg, secondArg);
    EXPECT_EQ(db.xCollNeeded16, 0);
}

#ifdef SQLITE_ENABLE_API_ARMOR
// Helper to adjust safety check behavior in tests
static void set_safety_ok(int ok) {
    s_safety_ok = ok;
}

static void test_api_armor_path_misuse() {
    // Enable armor path
    // Note: The test code is compiled with SQLITE_ENABLE_API_ARMOR defined to exercise this path.
    // We simulate a failed safety check by setting the global to return 0.
    sqlite3 db;
    sqlite3_mutex dummyMutex;
    db.mutex = &dummyMutex;
    db.xCollNeeded = nullptr;
    db.pCollNeededArg = nullptr;
    db.xCollNeeded16 = 0;

    set_safety_ok(0); // simulate unsafe db

    int rc = sqlite3_collation_needed(&db, nullptr, dummy_xCollNeeded);
    // Expect MISUSE_BKPT
    EXPECT_EQ(rc, SQLITE_MISUSE_BKPT);

    // Reset for next test
    set_safety_ok(1);
}
#endif

static void test_api_armor_path_success() {
#ifdef SQLITE_ENABLE_API_ARMOR
    // Ensure that when safety check passes, operation behaves like normal path.
    sqlite3 db;
    sqlite3_mutex dummyMutex;
    db.mutex = &dummyMutex;
    db.xCollNeeded = nullptr;
    db.pCollNeededArg = nullptr;
    db.xCollNeeded16 = -1;

    set_safety_ok(1);
    int rc = sqlite3_collation_needed(&db, (void*)0xABCD, dummy_xCollNeeded);
    EXPECT_EQ(rc, SQLITE_OK);
    EXPECT_PTR_EQ(db.xCollNeeded, dummy_xCollNeeded);
    EXPECT_EQ(db.pCollNeededArg, (void*)0xABCD);
    EXPECT_EQ(db.xCollNeeded16, 0);
#else
    // If API armor is not compiled in, this test is a no-op.
    (void)0;
#endif
}

// Step 3: Test Case Refinement
// The following main() function orchestrates the tests and prints a concise summary.
// Tests are designed to be non-terminating (they accumulate failures) to maximize
// code coverage during a single run.

int main() {
    // Run tests
    test_basic_registration_stores_callbacks_and_arg();
    test_overwrite_on_second_call();
#ifdef SQLITE_ENABLE_API_ARMOR
    test_api_armor_path_misuse();
    test_api_armor_path_success();
#else
    // If API armor is not enabled, still run a path that would conceptually test the normal behavior.
    test_api_armor_path_success(); // This call is a no-op when API_ARMOR is disabled
#endif

    // Summary
    std::cout << "\nTest run complete. Total tests: " << g_tests_run
              << ", Failures: " << g_tests_failed << "\n";

    // Non-zero exit on failure to aid integration with CI (even though tests are non-terminating).
    return (g_tests_failed != 0) ? 1 : 0;
}

/*
Explanation of the test approach and coverage goals:

- Step 1: Program understanding:
  - The focal method is intended to safely register a collation-needed callback into a sqlite3 database handle.
  - It uses a mutex to protect updates, assigns the provided callback and user data, and resets a 16-bit variant counter.
  - If API armor is enabled (SQLITE_ENABLE_API_ARMOR), it performs a safety check first and may return a misuse code.

- Step 2: Unit test generation:
  - We craft a minimal, self-contained mock environment that captures the essential fields touched by the method:
    - db->mutex, db->xCollNeeded, db->pCollNeededArg, db->xCollNeeded16
  - Tests ensure:
    - Proper assignment of callback and arg.
    - xCollNeeded16 is reset to 0.
    - The mutex enter/leave are invoked (observed by g_mutex_entered/g_mutex_left).
    - Re-registration overwrites previous values (test_overwrite_on_second_call).
    - API_ARMOR behavior is exercised when compiled with SQLITE_ENABLE_API_ARMOR.

- Step 3: Test case refinement:
  - The tests are written to be non-terminating, with a lightweight assertion system (EXPECT_* macros) to maximize execution paths and coverage.
  - Static/static-like behavior (e.g., API armor safety) is exercised through a global toggle to simulate both success and failure paths without terminating the test run.
  - The test harness is kept in a single translation unit for simplicity and deterministic behavior without external dependencies.

Notes:
- This test harness provides a self-contained verification of the focal logic in isolation. In a full integration scenario, tests would interact with a real sqlite3 DB instance and rely on public/internal headers. The current approach favors deterministic unit behavior while preserving the core semantics of the focal method as described in the prompt.
- If you want to run with API armor enabled, compile with -D SQLITE_ENABLE_API_ARMOR and ensure the test binary links with a definition for sqlite3SafetyCheckOk (provided here as a static function when API_ARMOR is defined). The test toggles safety to observe both branches.
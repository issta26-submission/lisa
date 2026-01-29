// Test suite for the focal method: sqlite3_auto_extension
// This test harness provides a self-contained environment to exercise
// the sqlite3_auto_extension logic by stubbing required dependencies.
// It is designed to compile with C++11 and does not depend on GTest.
// The tests intentionally avoid terminating on failure to maximize coverage.

#include <sqlite3ext.h>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Domain-specific constants (stand-in values for SQLite API macros)
#define SQLITE_OK 0
#define SQLITE_MISUSE_BKPT 1
#define SQLITE_NOMEM_BKPT 2
#define SQLITE_MUTEX_STATIC_MAIN 0
#define SQLITE_THREADSAFE 1

// We deliberately do not define SQLITE_ENABLE_API_ARMOR or SQLITE_OMIT_AUTOINIT
// to exercise the main path of sqlite3_auto_extension in this test harness.

// Forward declare the function under test (the real signature in C)
typedef void (*InitFunc)(void);
typedef InitFunc* InitFuncArray;

static int sqlite3_initialize(void);
static int sqlite3_initialize_rc = 0;       // 0 means success unless overridden
static int sqlite3_initialize_called = 0;

// Simple dummy mutex type to satisfy the test harness
struct sqlite3_mutex { int dummy; };
static sqlite3_mutex g_mainMutex;

// Prototypes for mutex helpers
static sqlite3_mutex* sqlite3MutexAlloc(int);
static void sqlite3_mutex_enter(sqlite3_mutex*);
static void sqlite3_mutex_leave(sqlite3_mutex*);

// A minimal in-process dynamic array to simulate wsdAutoext
static struct {
    unsigned int nExt;
    InitFunc *aExt;
} wsdAutoext = {0, nullptr};

// Macro to mirror the original code's wsdAutoextInit usage.
// It is a no-op in this test harness; real initialization is unnecessary here.
#define wsdAutoextInit do {} while(0)

// Small helper to emulate the behavior of realloc64 for function-pointer arrays.
// The signature matches InitFuncArray sqlite3_realloc64(InitFuncArray p, unsigned long long nByte);
static InitFunc *sqlite3_realloc64(InitFunc *p, unsigned long long nByte) {
    // If a global flag is set to fail realloc, simulate OOM.
    extern int simulate_realloc_fail;
    if (simulate_realloc_fail) return nullptr;

    // nByte is a byte count; we reinterpret as number of InitFunc pointers.
    size_t nelems = (size_t)nByte / sizeof(InitFunc);
    // realloc can handle null pointer (equivalent to malloc) when p==nullptr.
    InitFunc *q = (InitFunc*)std::realloc(p, nelems * sizeof(InitFunc));
    return q;
}

// Flag to simulate sqlite3_realloc64 failure (OOM)
int simulate_realloc_fail = 0;

// Minimal sqlite3_initialize wrapper used by the focal function
static int sqlite3_initialize(void) {
    sqlite3_initialize_called++;
    return sqlite3_initialize_rc;
}

// Minimal mutex helpers to satisfy sqlite3_auto_extension's usage
static sqlite3_mutex* sqlite3MutexAlloc(int /*type*/) {
#if 1
    (void)0;
    return &g_mainMutex;
#else
    return nullptr;
#endif
}
static void sqlite3_mutex_enter(sqlite3_mutex* /*mutex*/) { /* no-op for test */ }
static void sqlite3_mutex_leave(sqlite3_mutex* /*mutex*/) { /* no-op for test */ }

// The focal function under test (reproduced to operate against the test harness)
static int sqlite3_auto_extension(
  void (*xInit)(void)
){
#ifdef SQLITE_ENABLE_API_ARMOR
  if( xInit==0 ) return SQLITE_MISUSE_BKPT;
#endif
#ifndef SQLITE_OMIT_AUTOINIT
  int rc = SQLITE_OK;
  rc = sqlite3_initialize();
  if( rc ){
    return rc;
  }else
#else
  int rc = SQLITE_OK;
#endif
  {
    unsigned int i;
#if SQLITE_THREADSAFE
    sqlite3_mutex *mutex = sqlite3MutexAlloc(SQLITE_MUTEX_STATIC_MAIN);
#else
    sqlite3_mutex *mutex = nullptr;
#endif
    wsdAutoextInit;
    sqlite3_mutex_enter(mutex);
    for(i=0; i<wsdAutoext.nExt; i++){
      if( wsdAutoext.aExt[i]==xInit ) break;
    }
    if( i==wsdAutoext.nExt ){
      unsigned long long nByte = (wsdAutoext.nExt+1)*sizeof(wsdAutoext.aExt[0]);
      InitFunc *aNew;
      aNew = sqlite3_realloc64(wsdAutoext.aExt, nByte);
      if( aNew==0 ){
        rc = SQLITE_NOMEM_BKPT;
      }else{
        wsdAutoext.aExt = aNew;
        wsdAutoext.aExt[wsdAutoext.nExt] = xInit;
        wsdAutoext.nExt++;
      }
    }
    sqlite3_mutex_leave(mutex);
    assert( (rc&0xff)==rc );
    return rc;
  }
}

// Dummy external extension functions for testing
static void ext1(void) { /* do nothing */ }
static void ext2(void) { /* do nothing */ }

// Simple test harness asserting without aborting on failure
static int tests_run = 0;
static int tests_failed = 0;

static void test_fail(const char* msg) {
    std::cerr << "TEST FAILED: " << msg << std::endl;
    tests_failed++;
}
static void test_pass(const char* msg) {
    std::cout << "TEST PASSED: " << msg << std::endl;
}

#define ASSERT_TRUE(cond, msg) do { \
    tests_run++; \
    if(!(cond)) { test_fail(msg); } else { test_pass(msg); } \
} while(0)

// Helpers to reset the test environment between tests
static void reset_state() {
    // Reset the extension registry
    if (wsdAutoext.aExt) {
        std::free(wsdAutoext.aExt);
        wsdAutoext.aExt = nullptr;
    }
    wsdAutoext.nExt = 0;
    // Reset sqlite3 initialization simulate value
    sqlite3_initialize_rc = 0;
    sqlite3_initialize_called = 0;
    // Reset realloc behavior
    simulate_realloc_fail = 0;
}

// Test: first extension should be registered successfully
static void test_add_first_ext() {
    reset_state();
    int rc = sqlite3_auto_extension(ext1);
    ASSERT_TRUE(rc == SQLITE_OK, "First extension: rc should be SQLITE_OK");
    ASSERT_TRUE(wsdAutoext.nExt == 1, "First extension: nExt should be 1");
    ASSERT_TRUE(wsdAutoext.aExt[0] == ext1, "First extension: aExt[0] should be ext1");
}

// Test: adding the same extension again should not increase nExt
static void test_duplicate_ext_no_increment() {
    // assume previous test ran and ext1 is registered
    int rc = sqlite3_auto_extension(ext1);
    ASSERT_TRUE(rc == SQLITE_OK, "Duplicate ext: rc should remain SQLITE_OK");
    ASSERT_TRUE(wsdAutoext.nExt == 1, "Duplicate ext: nExt should still be 1");
}

// Test: adding a second distinct extension should succeed and grow the array
static void test_add_second_ext() {
    int rc = sqlite3_auto_extension(ext2);
    ASSERT_TRUE(rc == SQLITE_OK, "Second ext: rc should be SQLITE_OK");
    ASSERT_TRUE(wsdAutoext.nExt == 2, "Second ext: nExt should be 2");
    ASSERT_TRUE(wsdAutoext.aExt[1] == ext2, "Second ext: aExt[1] should be ext2");
}

// Test: initialization failure propagates rc and does not register new ext
static void test_init_failure_propagates() {
    reset_state();
    sqlite3_initialize_rc = 5; // non-zero simulate failure
    int rc = sqlite3_auto_extension(ext1);
    ASSERT_TRUE(rc == 5, "Init failure: rc should equal simulated value 5");
    ASSERT_TRUE(wsdAutoext.nExt == 0, "Init failure: no extension should be registered");
}

// Test: realloc64 failure should yield NOMEM_BKPT and not register new ext
static void test_realloc_failure_yields_nmem() {
    reset_state();
    sqlite3_initialize_rc = 0; // initialization success
    simulate_realloc_fail = 1;  // force realloc64 to fail
    int rc = sqlite3_auto_extension(ext1);
    ASSERT_TRUE(rc == SQLITE_NOMEM_BKPT, "Realloc failure: rc should be SQLITE_NOMEM_BKPT");
    ASSERT_TRUE(wsdAutoext.nExt == 0, "Realloc failure: no extension should be registered");
    simulate_realloc_fail = 0; // reset for other tests
}

// Clean up after tests
static void finalize_tests() {
    reset_state();
}

// Main function to run all tests
int main() {
    std::cout << "Starting sqlite3_auto_extension unit tests (standalone harness)..." << std::endl;

    test_add_first_ext();
    test_duplicate_ext_no_increment();
    test_add_second_ext();
    test_init_failure_propagates();
    test_realloc_failure_yields_nmem();

    // Finalize and report
    int total = tests_run;
    int failed = tests_failed;
    std::cout << "Tests run: " << total << ", Failures: " << failed << std::endl;

    finalize_tests();

    // Exit code 0 if all tests passed; otherwise return non-zero.
    return (failed == 0) ? 0 : 1;
}
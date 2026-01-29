/*
This test suite targets the focal function:
  char *sqlite3VMPrintf(sqlite3 *db, const char *zFormat, va_list ap)

Key goals:
- Validate the normal (non-OOM) path: formatting a simple string with arguments.
- Validate the OOM path by configuring SQLite to use a failing memory allocator
  (simulating acc.accError == SQLITE_NOMEM inside StrAccum) and ensuring the
  function returns NULL in that scenario.
- Use a lightweight, non-terminating test framework (no GTest) with
  minimal, explicit checks that continue on failure.
- Leverage public SQLite API (sqlite3_open, sqlite3_free, sqlite3_close) for
  creating a test database instance.
- Provide explanatory comments for each unit test.

Notes:
- To simulate OOM, the test configures a custom allocator via sqlite3_config(SQLITE_CONFIG_MALLOC)
  that begins to fail after a small number of allocations. This is intended to exercise
  the acc.accError == SQLITE_NOMEM branch inside sqlite3VMPrintf.
- The tests are written for C++11 but exercise a C API (SQLite). The test executable
  should link against the system or project SQLite3 library (e.g., -lsqlite3).

Build hints (not shown to the user): compile with a C++11 compiler and link against sqlite3.
Example (adjust to your build system):
  g++ -std=c++11 -Isrc/include -Lpath/to/sqlite -lsqlite3 test_printf_vm.cpp -o test_vmprintf
*/

#include <sqlite3.h>
#include <cstdarg>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>


// Ensure C linkage for SQLite headers when included in a C++ file
extern "C" {
}

// Lightweight test framework (non-terminating assertions)
static int g_failures = 0;

#define CHECK(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "Test failure: " << (msg) << " (" #cond ")" << std::endl; \
        ++g_failures; \
    } \
} while(0)

static void test_vmprintf_basic();
static void test_vmprintf_oom_simulation();
static int run_all_tests();

static char* vmprintf_wrapper(sqlite3* db, const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    char* res = sqlite3VMPrintf(db, fmt, ap);
    va_end(ap);
    return res;
}

// -------------- Test 1: Basic functionality of sqlite3VMPrintf --------------
/*
Test goal:
- Create an in-memory SQLite database.
- Use sqlite3VMPrintf to format a simple string with arguments.
- Verify the returned string content matches expectation.
- Free the allocated string using sqlite3_free (as per SQLite's memory management contract).
*/
static void test_vmprintf_basic() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    CHECK(db != nullptr, "Failed to allocate in-memory sqlite3 db");
    CHECK(rc == SQLITE_OK, "sqlite3_open should return SQLITE_OK for in-memory DB");

    if(db == nullptr || rc != SQLITE_OK) {
        // If DB couldn't be opened, skip further checks.
        if(db) sqlite3_close(db);
        return;
    }

    // Use a simple format with a single string argument
    char* z = vmprintf_wrapper(db, "Hello %s", "World");
    CHECK(z != nullptr, "sqlite3VMPrintf should return a non-NULL pointer on normal path");

    if(z != nullptr) {
        CHECK(std::strcmp(z, "Hello World") == 0, "Formatted string should be 'Hello World'");
        sqlite3_free(z);
    }

    sqlite3_close(db);
}

// -------------- Test 2: Simulate OOM to exercise acc.accError == SQLITE_NOMEM --------------
/*
Test goal:
- Configure a custom memory allocator that intentionally fails after a small
  number of allocations, simulating an out-of-memory (OOM) condition.
- Call sqlite3VMPrintf and verify that it returns NULL under OOM conditions.
- This exercises the branch:
    if( acc.accError==SQLITE_NOMEM ) { sqlite3OomFault(db); }
  inside sqlite3VMPrintf (the true branch).
Implementation notes:
- sqlite3_config(SQLITE_CONFIG_MALLOC, &mem_methods) is used to install a custom allocator.
- The custom allocator fail-after-N behavior is implemented for xMalloc and xRealloc.
- We perform the test after configuring the allocator, on a fresh in-memory DB.
*/
static int g_alloc_calls = 0;
static int g_fail_after_allocs = 1; // Fail after this many allocations

// Forward declarations for memory methods
static void* test_xMalloc(size_t n);
static void test_xFree(void* ptr);
static void* test_xRealloc(void* ptr, size_t n);
static int test_xSize(void* ptr);
static void test_xDestroy(void* ptr);

static sqlite3_mem_methods g_test_mem_methods;

static void simulate_oom_allocator_setup() {
    // Reset global counters
    g_alloc_calls = 0;
    // Prepare mem_methods struct
    g_test_mem_methods = {
        &test_xMalloc,
        &test_xFree,
        &test_xRealloc,
        &test_xSize,
        &test_xDestroy
    };
    // Install the allocator
    int rc = sqlite3_config(SQLITE_CONFIG_MALLOC, &g_test_mem_methods);
    CHECK(rc == SQLITE_OK, "sqlite3_config(SQLITE_CONFIG_MALLOC) should return SQLITE_OK");
}

// Custom allocator implementations
static void* test_xMalloc(size_t n) {
    if (g_alloc_calls >= g_fail_after_allocs) {
        return nullptr; // Simulate OOM
    }
    ++g_alloc_calls;
    return malloc(n);
}
static void test_xFree(void* ptr) {
    free(ptr);
}
static void* test_xRealloc(void* ptr, size_t n) {
    if (g_alloc_calls >= g_fail_after_allocs) {
        return nullptr; // Simulate OOM on realloc as well
    }
    ++g_alloc_calls;
    return realloc(ptr, n);
}
static int test_xSize(void* ptr) {
    // Not critical for this test; return a safe non-negative value.
    return (ptr != nullptr) ? 1 : 0;
}
static void test_xDestroy(void* ptr) {
    if (ptr) free(ptr);
}

static void test_vmprintf_oom_simulation() {
    // Install the failing allocator
    simulate_oom_allocator_setup();

    // Open a db after allocator is installed
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    CHECK(db != nullptr, "DB should be allocated even with failing allocator (OOM should be observed during formatting)");
    CHECK(rc == SQLITE_OK, "sqlite3_open should succeed with in-memory DB despite allocator override");

    if(db == nullptr || rc != SQLITE_OK) {
        if(db) sqlite3_close(db);
        return;
    }

    // Attempt to format a string; under OOM this should fail and return NULL
    char* z = vmprintf_wrapper(db, "Test OOM %d", 12345);
    // Depending on where OOM occurs, the result may be NULL or non-NULL.
    // We expect NULL on OOM to exercise the true branch in sqlite3VMPrintf.
    if (z != nullptr) {
        // If not NULL, still free to avoid leaks to keep test robust
        sqlite3_free(z);
        CHECK(false, "Expected NULL due to simulated OOM, but got a non-NULL pointer");
    } else {
        // NULL result is the expected OOM outcome
        CHECK(true, "sqlite3VMPrintf returned NULL under simulated OOM (as expected)");
    }

    sqlite3_close(db);
}

// -------------- Test runner --------------
static int run_all_tests() {
    std::cout << "Running sqlite3VMPrintf unit tests (C API tested from C++11 test runner)" << std::endl;

    test_vmprintf_basic();
    test_vmprintf_oom_simulation();

    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}

// -------------- Main --------------
int main() {
    return run_all_tests();
}
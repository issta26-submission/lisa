// Unit test suite for sqlite3_mutex_alloc (C linkage) using a lightweight handcrafted harness.
// This test targets the focal function in mutex.c without using GTest.
// It exercises the critical branches controlled by sqlite3_initialize and sqlite3MutexInit
// and verifies that the xMutexAlloc callback is invoked when appropriate.
//
// Important: This harness assumes the project is built with SQLITE_OMIT_AUTOINIT not defined
// (to exercise the initialization branches). The test relies on the common SQLite values
// where SQLITE_MUTEX_RECURSIVE is 0. The harness mocks the required global state and API
// surface used by sqlite3_mutex_alloc.

#include <vector>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>


// Declare C linkage for the symbols used by the focal function to enable linking with mutex.c.
extern "C" {

// Forward declare sqlite3_mutex type used by sqlite3_mutex_alloc return type.
// The real SQLite struct is opaque here; we only deal with it as a pointer.
typedef struct sqlite3_mutex sqlite3_mutex;

// Minimal chrome of the mutex.h surface used by sqlite3_mutex_alloc.
// We only need access to the xMutexAlloc callback nested under sqlite3GlobalConfig.mutex.
typedef struct {
    sqlite3_mutex* (*xMutexAlloc)(int);
} sqlite3MutexMethods;

// Global configuration surface that the focal function references.
typedef struct {
    sqlite3MutexMethods mutex;
} sqlite3GlobalConfigType;

// Extern declaration of the global config object used by sqlite3_mutex_alloc.
extern sqlite3GlobalConfigType sqlite3GlobalConfig;

// Prototypes of initialization hooks that sqlite3_mutex_alloc calls under AUTOINIT.
int sqlite3_initialize(void);
int sqlite3MutexInit(void);

// The focal function signature under test (already defined in mutex.c).
sqlite3_mutex *sqlite3_mutex_alloc(int id);
}

// -----------------------------------------------------------------------------
// Test doubles and state for the harness
// -----------------------------------------------------------------------------
// Provide a simple concrete definition for sqlite3_mutex to be allocated by the test hook.
// We keep it minimal since the tests only ensure a non-null pointer is returned or NULL in
// failure cases.
struct sqlite3_mutex {
    int id; // store the id used to create the mutex for verification purposes
};

// Real mutex_alloc will call the function pointer in sqlite3GlobalConfig.mutex.xMutexAlloc.
// We implement a test hook to simulate allocation.
extern "C" sqlite3_mutex* test_xMutexAlloc(int id);

// Helpers to track calls and control behavior
static int g_initialize_calls = 0;
static int g_initialize_return = 0; // 0 means success; non-zero simulates failure

static int g_mutexInit_calls = 0;
static int g_mutexInit_return = 0; // 0 means success; non-zero simulates failure

// Expose a per-test setter for the xMutexAlloc function pointer
static void set_xMutexAlloc(sqlite3_mutex* (*fn)(int)) {
    sqlite3GlobalConfig.mutex.xMutexAlloc = fn;
}

// Provide a concrete xMutexAlloc implementation used by tests.
// It allocates a small sqlite3_mutex object and encodes the provided id.
extern "C" sqlite3_mutex* test_xMutexAlloc(int id) {
    sqlite3_mutex* m = new sqlite3_mutex{ id };
    return m;
}

// Implementations of the initialization hooks to be used by the focal function.
// They increment call counters and return controlled values.
extern "C" int sqlite3_initialize(void) {
    ++g_initialize_calls;
    return g_initialize_return;
}

extern "C" int sqlite3MutexInit(void) {
    ++g_mutexInit_calls;
    return g_mutexInit_return;
}

// Define a minimal sqlite3GlobalConfig object to satisfy the focal function's access.
// The object layout mirrors the needs of sqlite3GlobalConfig.mutex.xMutexAlloc.
extern "C" sqlite3GlobalConfigType sqlite3GlobalConfig = { { nullptr } };

// Simple test harness state
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Lightweight assertion helper: non-terminating (logs outcomes but does not abort).
#define TEST_ASSERT(cond, msg)                                       \
    do {                                                             \
        ++g_total_tests;                                             \
        if (cond) {                                                  \
            std::cout << "[TEST PASS] " << msg << std::endl;        \
        } else {                                                     \
            ++g_failed_tests;                                      \
            std::cerr << "[TEST FAIL] " << msg << std::endl;       \
        }                                                            \
    } while (0)

static void run_case_A_case1() {
    // Case A: id <= SQLITE_MUTEX_RECURSIVE, sqlite3_initialize() returns non-zero -> should return NULL
    // We rely on threshold SQLITE_MUTEX_RECURSIVE == 0 (typical SQLite behavior).
    g_initialize_calls = 0;
    g_mutexInit_calls = 0;
    // Configure mocks
    g_initialize_return = 1;      // simulate failure to initialize
    g_mutexInit_return = 0;       // not used in this path, but keep sane

    // Install test xMutexAlloc
    set_xMutexAlloc(test_xMutexAlloc);

    // Call the focal function
    sqlite3_mutex* result = sqlite3_mutex_alloc(0); // id = 0 <= RECursive boundary

    // Expected: NULL due to initialize failure
    TEST_ASSERT(result == nullptr, "Case A: id<=RECURSIVE and sqlite3_initialize() non-zero should return NULL");
}

static void run_case_A_case2() {
    // Case B (continuation of A's path): id <= SQLITE_MUTEX_RECURSIVE and sqlite3_initialize() returns 0
    // Expect allocation via xMutexAlloc to be invoked and return non-null mutex.

    g_initialize_calls = 0;
    g_mutexInit_calls = 0;
    g_initialize_return = 0; // initialization succeeds
    g_mutexInit_return = 0;  // not used in this path

    // Install test xMutexAlloc
    set_xMutexAlloc(test_xMutexAlloc);

    // Call focal function
    sqlite3_mutex* result = sqlite3_mutex_alloc(0); // still id <= boundary

    // Expected: non-null pointer allocated via xMutexAlloc
    TEST_ASSERT(result != nullptr, "Case B: id<=RECURSIVE with successful sqlite3_initialize should allocate mutex");
    // Cleanup
    if (result) delete result;
}

static void run_case_C_case3() {
    // Case C: id > SQLITE_MUTEX_RECURSIVE and sqlite3MutexInit() returns non-zero -> should return NULL
    // We assume threshold 0, so id=1 is > boundary.
    g_initialize_calls = 0;
    g_mutexInit_calls = 0;
    g_initialize_return = 0; // won't be called in this path, but keep deterministic
    g_mutexInit_return = 1;  // simulate failure to initialize the mutex

    set_xMutexAlloc(test_xMutexAlloc);

    sqlite3_mutex* result = sqlite3_mutex_alloc(1); // id > boundary triggers second condition

    TEST_ASSERT(result == nullptr, "Case C: id>RECURSIVE and sqlite3MutexInit() non-zero should return NULL");
}

static void run_case_D_case4() {
    // Case D: id > SQLITE_MUTEX_RECURSIVE and sqlite3MutexInit() returns 0 -> should allocate
    g_initialize_calls = 0;
    g_mutexInit_calls = 0;
    g_initialize_return = 0;
    g_mutexInit_return = 0;

    set_xMutexAlloc(test_xMutexAlloc);

    sqlite3_mutex* result = sqlite3_mutex_alloc(1);

    TEST_ASSERT(result != nullptr, "Case D: id>RECURSIVE with successful sqlite3MutexInit should allocate mutex");
    if (result) {
        // Cleanup
        delete result;
    }
}

// Entry point
int main() {
    std::cout << "sqlite3_mutex_alloc unit test harness (non-GTest, single translation unit)\n";

    run_case_A_case1();
    run_case_A_case2();
    run_case_C_case3();
    run_case_D_case4();

    std::cout << "Tests completed. Total: " << g_total_tests
              << ", Passed: " << (g_total_tests - g_failed_tests)
              << ", Failed: " << g_failed_tests << "\n";

    // Return non-zero if any test failed
    return g_failed_tests == 0 ? 0 : 1;
}
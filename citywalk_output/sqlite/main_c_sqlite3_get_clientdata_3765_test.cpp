// Minimal C++11 test harness for the focal function:
// sqlite3_get_clientdata, with lightweight stubs to simulate the
// SQLite-like environment required by the function.
// This test is self-contained and does not rely on GTest.
// It focuses on exercising both true/false branches of the key predicate
// and ensures proper mutex unlocking across all paths.

#include <vector>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>


// Forward declare the types used by the focal function (C-style API)
struct sqlite3_mutex;  // opaque mutex type (stub)
struct DbClientData;

// Lightweight sqlite3 structure used by sqlite3_get_clientdata
struct sqlite3 {
    sqlite3_mutex *mutex;
    DbClientData *pDbData;
};

// Simple linked list node holding client data
struct DbClientData {
    const char *zName;
    void *pData;
    DbClientData *pNext;
};

// Global flag to simulate mutex state (true = locked, false = unlocked)
static bool g_mutex_locked = false;

// C linkage for the functions that the focal method relies on
extern "C" {

// Stub mutex type (no actual synchronization in tests)
struct sqlite3_mutex { int dummy; };

// Acquire the mutex (set flag to indicate "locked")
void sqlite3_mutex_enter(sqlite3_mutex *NotUsed) {
    (void)NotUsed; // suppress unused warning if optimized away
    g_mutex_locked = true;
}

// Release the mutex (set flag to indicate "unlocked")
void sqlite3_mutex_leave(sqlite3_mutex *NotUsed) {
    (void)NotUsed;
    g_mutex_locked = false;
}

// The focal method under test (reproduced here for the test harness)
void *sqlite3_get_clientdata(sqlite3 *db, const char *zName) {
    DbClientData *p;
    sqlite3_mutex_enter(db->mutex);
    for (p = db->pDbData; p; p = p->pNext) {
        if (strcmp(p->zName, zName) == 0) {
            void *pResult = p->pData;
            sqlite3_mutex_leave(db->mutex);
            return pResult;
        }
    }
    sqlite3_mutex_leave(db->mutex);
    return 0;
}

} // extern "C"

// Lightweight test framework (non-terminating, accumulates failures)
static std::vector<std::string> g_failures;

// Assertion helper: record failure without aborting tests
#define TEST_ASSERT(cond, msg) do { if(!(cond)) { g_failures.push_back(std::string(msg)); } } while(0)

// Convenience macro for test case delineation
#define RUN_TEST(name) void name(); static void name##_run() { name(); } \
    static bool name##_registered = [](){ name##_run(); return true; }();

// Helper to reset the global mutex state between tests
static void reset_mutex_state() {
    g_mutex_locked = false;
}

///////////////////////
// Test Case: Found second in list (middle element)
///////////////////////
static void test_get_clientdata_found_middle() {
    // Arrange: create a list of 3 entries: first -> second -> third
    DbClientData d3 = { "third",  (void*)0x3003, nullptr };
    DbClientData d2 = { "second", (void*)0x2002, &d3 };
    DbClientData d1 = { "first",  (void*)0x1001, &d2 };

    sqlite3 db;
    sqlite3_mutex m;
    db.mutex = &m;
    db.pDbData = &d1;

    // Act
    void *res = sqlite3_get_clientdata(&db, "second");

    // Assert: should return second's data and unlock mutex
    TEST_ASSERT(res == (void*)0x2002, "Expected to retrieve pData of 'second'");
    TEST_ASSERT(!g_mutex_locked, "Mutex should be released after lookup (middle)");
    reset_mutex_state();
    // Cleanup implicit via scope
}

// Expose a small runner to ensure this test gets executed when the program runs
static void run_test_get_clientdata_found_middle() {
    test_get_clientdata_found_middle();
}

///////////////////////
// Test Case: Not found (no matching name)
///////////////////////
static void test_get_clientdata_not_found() {
    // Arrange: single entry
    DbClientData d1 = { "only", (void*)0x1111, nullptr };

    sqlite3 db;
    sqlite3_mutex m;
    db.mutex = &m;
    db.pDbData = &d1;

    // Act
    void *res = sqlite3_get_clientdata(&db, "missing");

    // Assert: should return nullptr and unlock mutex
    TEST_ASSERT(res == nullptr, "Expected nullptr when no matching name");
    TEST_ASSERT(!g_mutex_locked, "Mutex should be released when not found");
    reset_mutex_state();
}

static void run_test_get_clientdata_not_found() {
    test_get_clientdata_not_found();
}

///////////////////////
// Test Case: Found first (head of list)
///////////////////////
static void test_get_clientdata_found_first() {
    DbClientData d2 = { "second", (void*)0x2002, nullptr };
    DbClientData d1 = { "first",  (void*)0x1001, &d2 };

    sqlite3 db;
    sqlite3_mutex m;
    db.mutex = &m;
    db.pDbData = &d1;

    void *res = sqlite3_get_clientdata(&db, "first");

    TEST_ASSERT(res == (void*)0x1001, "Expected to retrieve data of first element");
    TEST_ASSERT(!g_mutex_locked, "Mutex should be released after lookup (first)");
    reset_mutex_state();
}

static void run_test_get_clientdata_found_first() {
    test_get_clientdata_found_first();
}

///////////////////////
// Test Case: Empty list (null head)
///////////////////////
static void test_get_clientdata_empty_list() {
    sqlite3 db;
    sqlite3_mutex m;
    db.mutex = &m;
    db.pDbData = nullptr;

    void *res = sqlite3_get_clientdata(&db, "anything");

    TEST_ASSERT(res == nullptr, "Expected nullptr when list is empty");
    TEST_ASSERT(!g_mutex_locked, "Mutex should be released on empty list");
    reset_mutex_state();
}

static void run_test_get_clientdata_empty_list() {
    test_get_clientdata_empty_list();
}

///////////////////////
// Test Case: Duplicate names -> should return first occurrence
///////////////////////
static void test_get_clientdata_duplicate_names_returns_first() {
    DbClientData a = { "dup", (void*)0x1111, nullptr };
    DbClientData b = { "dup", (void*)0x2222, nullptr };
    a.pNext = &b;

    sqlite3 db;
    sqlite3_mutex m;
    db.mutex = &m;
    db.pDbData = &a;

    void *res = sqlite3_get_clientdata(&db, "dup");

    TEST_ASSERT(res == (void*)0x1111, "Expected first duplicate's data to be returned");
    TEST_ASSERT(!g_mutex_locked, "Mutex should be released after lookup (duplicate)");
    reset_mutex_state();
}

static void run_test_get_clientdata_duplicate_names_returns_first() {
    test_get_clientdata_duplicate_names_returns_first();
}

///////////////////////
// Main: run all tests and report results
///////////////////////
int main() {
    // Run each test (manually invoked to avoid a full test framework)
    run_test_get_clientdata_found_middle();
    run_test_get_clientdata_not_found();
    run_test_get_clientdata_found_first();
    run_test_get_clientdata_empty_list();
    run_test_get_clientdata_duplicate_names_returns_first();

    // Report results
    if (g_failures.empty()) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures.size() << " test(s) failed:" << std::endl;
        for (const auto &msg : g_failures) {
            std::cerr << " - " << msg << std::endl;
        }
        return 1;
    }
}
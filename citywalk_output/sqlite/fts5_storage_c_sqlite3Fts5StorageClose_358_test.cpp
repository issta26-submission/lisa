// Unit test suite for sqlite3Fts5StorageClose
// Target: fts5_storage.c - function sqlite3Fts5StorageClose
// Language: C++11 (no GoogleTest/GMock; simple custom test harness)

#include <sqlite3.h>
#include <iostream>
#include <cstring>
#include <fts5Int.h>


// Import necessary dependencies and correct paths.
// The focal method and its dependencies are C code; use extern "C" to link properly.
extern "C" {
}

// Lightweight test framework (no external libraries)
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[FAILED] " << (msg) << " (Line " << __LINE__ << ")" << std::endl; \
        return false; \
    } \
} while(0)

#define TEST_PASS(msg) do { \
    std::cout << "[PASSED] " << (msg) << std::endl; \
} while(0)

// Helper to print test header
static void print_header(const char* title) {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "TEST: " << title << std::endl;
    std::cout << "----------------------------------------" << std::endl;
}

// Test 1: Behavior when p is NULL
// - The function should perform no actions and return SQLITE_OK.
static bool test_sqlite3Fts5StorageClose_nullPointer() {
    print_header("sqlite3Fts5StorageClose should handle NULL pointer safely");
    int rc = sqlite3Fts5StorageClose(nullptr);
    TEST_ASSERT(rc == SQLITE_OK, "sqlite3Fts5StorageClose(NULL) must return SQLITE_OK");
    TEST_PASS("NULL pointer path returns SQLITE_OK and does nothing");
    return true;
}

// Test 2: Behavior with a valid non-NULL pointer and an initialized storage
// - Allocate a storage object and ensure aStmt array elements are NULL.
// - This exercises the true-branch of the guard and exercises the for-loop
//   with ArraySize(p->aStmt) iterations, invoking sqlite3_finalize on NULL pointers.
// - sqlite3_finalize(NULL) is defined to be SQLITE_OK; thus, this should not crash.
// - Finally, verify memory is freed via sqlite3_free.
static bool test_sqlite3Fts5StorageClose_allocated_with_null_statements() {
    print_header("sqlite3Fts5StorageClose operates on non-NULL storage with NULL aStmt entries");
    // Allocate storage object via sqlite3_malloc to mirror expected allocation pattern
    // to work with sqlite3_free in the function under test.
    size_t sz = sizeof(Fts5Storage);
    Fts5Storage *p = (Fts5Storage*)sqlite3_malloc(sz);
    TEST_ASSERT(p != nullptr, "Failed to allocate Fts5Storage");
    // Initialize memory to zero to ensure all aStmt entries are NULL
    std::memset(p, 0, sz);

    // It is assumed that p->aStmt is an array member in Fts5Storage.
    // By zero-initializing, all elements are NULL; sqlite3_finalize(NULL) should be safe.

    int rc = sqlite3Fts5StorageClose(p);
    TEST_ASSERT(rc == SQLITE_OK, "sqlite3Fts5StorageClose with NULL aStmt entries should return SQLITE_OK");

    // Free the storage object
    sqlite3_free(p);

    TEST_PASS("Allocated storage with NULL aStmt entries handled correctly and freed");
    return true;
}

// Test 3: Behavior when p is NULL after multiple tests (extra safety check)
// - Re-run to ensure idempotence and consistent return value.
static bool test_sqlite3Fts5StorageClose_nullPointer_again() {
    print_header("sqlite3Fts5StorageClose NULL pointer consistency check");
    int rc = sqlite3Fts5StorageClose(nullptr);
    TEST_ASSERT(rc == SQLITE_OK, "Repeated NULL pointer path must return SQLITE_OK");
    TEST_PASS("NULL pointer path is consistently safe");
    return true;
}

// Entry point to run all tests
int main() {
    int failed = 0;

    // Run tests and accumulate results
    if(!test_sqlite3Fts5StorageClose_nullPointer()) failed++;
    if(!test_sqlite3Fts5StorageClose_allocated_with_null_statements()) failed++;
    if(!test_sqlite3Fts5StorageClose_nullPointer_again()) failed++;

    if(failed == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << failed << " test(s) failed." << std::endl;
        return 1;
    }
}
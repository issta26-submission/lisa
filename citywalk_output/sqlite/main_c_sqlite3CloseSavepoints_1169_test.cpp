// Lightweight C++11 unit tests for the C-style focal method:
// sqlite3CloseSavepoints(sqlite3 *db)
// The tests are self-contained: they provide minimal stubs for the sqlite3 runtime
// environment (sqlite3, Savepoint) and implement the focal function exactly as shown.
// No external test frameworks are used (GTest is avoided). A tiny in-file test runner
// is implemented that uses non-terminating assertions to maximize code coverage.

#include <iostream>
#include <cstdlib>
#include <sqliteInt.h>


// Domain Helper: lightweight assertion mechanism (non-terminating)
static int g_totalTests = 0;
static int g_failedTests = 0;
static void ASSERT(bool condition, const char* msg) {
    ++g_totalTests;
    if (!condition) {
        ++g_failedTests;
        std::cerr << "[ASSERT FAILED] " << msg << std::endl;
    } else {
        std::cout << "[ASSERT PASSED] " << msg << std::endl;
    }
}

// Forward declare and define minimal sqlite3 dependency surface to allow the focal method to compile/link
extern "C" {

// Forward declaration of Savepoint used by sqlite3CloseSavepoints
typedef struct Savepoint Savepoint;
struct Savepoint {
    Savepoint *pNext;
};

// Forward declaration of sqlite3 structure used by the focal method
typedef struct sqlite3 sqlite3;
struct sqlite3 {
    Savepoint *pSavepoint;
    int nSavepoint;
    int nStatement;
    int isTransactionSavepoint;
};

// The test harness will implement a custom sqlite3DbFree to track frees
void sqlite3DbFree(sqlite3 *db, void *p);

// Focal method under test (reproduced here for self-contained tests)
void sqlite3CloseSavepoints(sqlite3 *db) {
    while (db->pSavepoint) {
        Savepoint *pTmp = db->pSavepoint;
        db->pSavepoint = pTmp->pNext;
        sqlite3DbFree(db, pTmp);
    }
    db->nSavepoint = 0;
    db->nStatement = 0;
    db->isTransactionSavepoint = 0;
}
} // extern "C"

// Test harness state to track freed Savepoint pointers (for address-level verification)
static Savepoint *g_freedPointers[32];
static int g_freedCount = 0;

// Custom memory free function used by the focal method
extern "C" void sqlite3DbFree(sqlite3 *db, void *p) {
    // Track freed pointers for verification, then actually free the memory
    if (p != nullptr && g_freedCount < static_cast<int>(sizeof(g_freedPointers)/sizeof(g_freedPointers[0]))) {
        g_freedPointers[g_freedCount++] = reinterpret_cast<Savepoint*>(p);
    }
    free(p);
}

// Helper to reset test-state between tests
static void resetTestState() {
    g_freedCount = 0;
    for (int i = 0; i < 32; ++i) g_freedPointers[i] = nullptr;
}

// Test 1: When db->pSavepoint is NULL (empty list), the function should perform no frees
// and reset counters to zero.
static void test_CloseSavepoints_EmptyList() {
    resetTestState();
    sqlite3 db;
    db.pSavepoint = NULL;
    db.nSavepoint = 3;
    db.nStatement = 7;
    db.isTransactionSavepoint = 1;

    // Call the focal method
    sqlite3CloseSavepoints(&db);

    // Expectations:
    // - No frees performed
    // - pSavepoint remains NULL
    // - Counters reset to 0
    ASSERT(db.pSavepoint == NULL, "Empty list: pSavepoint should remain NULL after call");
    ASSERT(db.nSavepoint == 0, "Empty list: nSavepoint should be 0 after call");
    ASSERT(db.nStatement == 0, "Empty list: nStatement should be 0 after call");
    ASSERT(db.isTransactionSavepoint == 0, "Empty list: isTransactionSavepoint should be 0 after call");
    ASSERT(g_freedCount == 0, "Empty list: no Savepoint should be freed");
    std::cout << "Test 1 (Empty list) completed.\n" << std::endl;
}

// Test 2: Single Savepoint in the list is freed and list cleared
static void test_CloseSavepoints_SingleNode() {
    resetTestState();
    // Allocate a single Savepoint node
    Savepoint *s1 = reinterpret_cast<Savepoint*>(malloc(sizeof(Savepoint)));
    s1->pNext = NULL;

    sqlite3 db;
    db.pSavepoint = s1;
    db.nSavepoint = 1;
    db.nStatement = 2;
    db.isTransactionSavepoint = 1;

    // Call the focal method
    sqlite3CloseSavepoints(&db);

    // Expectations:
    // - s1 freed exactly once
    // - pSavepoint NULL afterwards
    // - Counters reset to 0
    ASSERT(db.pSavepoint == NULL, "Single node: pSavepoint should be NULL after call");
    ASSERT(db.nSavepoint == 0, "Single node: nSavepoint should be 0 after call");
    ASSERT(db.nStatement == 0, "Single node: nStatement should be 0 after call");
    ASSERT(db.isTransactionSavepoint == 0, "Single node: isTransactionSavepoint should be 0 after call");
    ASSERT(g_freedCount == 1, "Single node: exactly one Savepoint should be freed");
    ASSERT(s1 == g_freedPointers[0], "Single node: freed pointer should match original node");

    // No need to free s1 again; the test harness freed it in sqlite3DbFree
    std::cout << "Test 2 (Single node) completed.\n" << std::endl;
}

// Test 3: Multiple Savepoints in a chain are freed in order and list cleared
static void test_CloseSavepoints_MultipleNodes() {
    resetTestState();
    // Create two Savepoint nodes: s1 -> s2 -> NULL
    Savepoint *s2 = reinterpret_cast<Savepoint*>(malloc(sizeof(Savepoint)));
    s2->pNext = NULL;
    Savepoint *s1 = reinterpret_cast<Savepoint*>(malloc(sizeof(Savepoint)));
    s1->pNext = s2;

    sqlite3 db;
    db.pSavepoint = s1;
    db.nSavepoint = 2;
    db.nStatement = 5;
    db.isTransactionSavepoint = 1;

    // Call the focal method
    sqlite3CloseSavepoints(&db);

    // Expectations:
    // - s1 freed first, then s2
    // - pSavepoint NULL afterwards
    // - Counters reset to 0
    ASSERT(db.pSavepoint == NULL, "Multiple nodes: pSavepoint should be NULL after call");
    ASSERT(db.nSavepoint == 0, "Multiple nodes: nSavepoint should be 0 after call");
    ASSERT(db.nStatement == 0, "Multiple nodes: nStatement should be 0 after call");
    ASSERT(db.isTransactionSavepoint == 0, "Multiple nodes: isTransactionSavepoint should be 0 after call");
    ASSERT(g_freedCount == 2, "Multiple nodes: exactly two Savepoints should be freed");
    ASSERT(s1 == g_freedPointers[0], "Multiple nodes: first freed should be s1");
    ASSERT(s2 == g_freedPointers[1], "Multiple nodes: second freed should be s2");

    // No leaks left; memory freed by sqlite3DbFree
    std::cout << "Test 3 (Multiple nodes) completed.\n" << std::endl;
}

// Entry point: run all tests
int main() {
    std::cout << "Starting tests for sqlite3CloseSavepoints (self-contained harness)" << std::endl;

    test_CloseSavepoints_EmptyList();
    test_CloseSavepoints_SingleNode();
    test_CloseSavepoints_MultipleNodes();

    // Summary
    std::cout << "\nTest Summary: ";
    std::cout << g_totalTests << " tests run, " << g_failedTests << " failed." << std::endl;

    return (g_failedTests == 0) ? 0 : 1;
}
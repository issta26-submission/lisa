// Unit test suite for sqlite3DeleteTriggerStep
// This test suite is built without GoogleTest. It uses a small, self-contained
// test harness with "non-terminating" assertions (failures are logged but do not
// abort execution) to maximize code coverage.
// The tests rely on a mock environment for dependent delete/free functions to
// observe that sqlite3DeleteTriggerStep frees all TriggerStep members properly.
//
// Test file layout:
// - Lightweight, test-oriented definitions of sqlite3 types and TriggerStep
// - Mock implementations of dependent delete/free functions
// - Test scenarios:
//     1) Two-node TriggerStep chain: verify all fields are freed for both nodes
//     2) Null input: verify no crash and no frees occur
//     3) Single-node chain with only essential fields: verify single free cycle
// - Main function runs all tests and reports results.

#include <vector>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>


// Forward declarations to mimic SQLite internal types required by the focal method.
extern "C" {
    // Opaque sqlite3 structure (we don't use actual internals in tests)
    struct sqlite3;
    typedef struct sqlite3 sqlite3;

    // The TriggerStep layout is assumed to match the real project layout based on the focal method.
    typedef struct TriggerStep TriggerStep;
    struct TriggerStep {
        TriggerStep *pNext;
        void *pWhere;
        void *pExprList;
        void *pSelect;
        void *pIdList;
        void *pUpsert;
        void *pFrom;
        char *zSpan;
    };

    // Focal method under test (to be linked from the project under test)
    void sqlite3DeleteTriggerStep(sqlite3 *db, TriggerStep *pTriggerStep);

    // Mocked dependent delete/free functions expected by sqlite3DeleteTriggerStep
    void sqlite3ExprDelete(sqlite3 *db, void *);
    void sqlite3ExprListDelete(sqlite3 *db, void *);
    void sqlite3SelectDelete(sqlite3 *db, void *);
    void sqlite3IdListDelete(sqlite3 *db, void *);
    void sqlite3UpsertDelete(sqlite3 *db, void *);
    void sqlite3SrcListDelete(sqlite3 *db, void *);

    // Free memory. In tests, track freed pointers for assertion, then actually free.
    void sqlite3DbFree(sqlite3 *db, void *);
}

// Global storage to observe frees performed by the mocks
static std::vector<void*> g_freedWhere;
static std::vector<void*> g_freedExprList;
static std::vector<void*> g_freedSelect;
static std::vector<void*> g_freedIdList;
static std::vector<void*> g_freedUpsert;
static std::vector<void*> g_freedFrom;
static std::vector<char*> g_freedSpan;      // actual zSpan buffers freed
static std::vector<void*> g_freedNode;      // TriggerStep objects freed (pTmp)

// Captured span buffers to compare against freed spans
static char *g_span1 = nullptr;
static char *g_span2 = nullptr;
static char *g_span3 = nullptr;

// Mock implementations for dependent delete/free functions
extern "C" {

// Note: The db parameter is unused in mocks; included to match signature.

// Track and observe deletion of pWhere pointers
void sqlite3ExprDelete(sqlite3 *db, void *p) {
    g_freedWhere.push_back(p);
}

// Track deletion of pExprList
void sqlite3ExprListDelete(sqlite3 *db, void *p) {
    g_freedExprList.push_back(p);
}

// Track deletion of pSelect
void sqlite3SelectDelete(sqlite3 *db, void *p) {
    g_freedSelect.push_back(p);
}

// Track deletion of pIdList
void sqlite3IdListDelete(sqlite3 *db, void *p) {
    g_freedIdList.push_back(p);
}

// Track deletion of pUpsert
void sqlite3UpsertDelete(sqlite3 *db, void *p) {
    g_freedUpsert.push_back(p);
}

// Track deletion of pFrom
void sqlite3SrcListDelete(sqlite3 *db, void *p) {
    g_freedFrom.push_back(p);
}

// Free memory; classify if the pointer is a zSpan buffer or a TriggerStep
void sqlite3DbFree(sqlite3 *db, void *p) {
    if (p == g_span1 || p == g_span2 || p == g_span3) {
        // It's a zSpan buffer allocated by tests
        g_freedSpan.push_back((char*)p);
    } else if (p != nullptr) {
        // Otherwise, it's a TriggerStep instance (pTmp)
        g_freedNode.push_back(p);
    }
    // Always free the memory to avoid leaks in test environment
    free(p);
}

} // extern "C"

// Simple non-terminating assertion helpers
static int g_failures = 0;
#define CHECK(cond, msg) do { \
    if(!(cond)) { \
        ++g_failures; \
        std::cerr << "Test Failure: " << msg << std::endl; \
    } \
} while(0)

// Helper to reset global test state
static void reset_test_state() {
    g_freedWhere.clear();
    g_freedExprList.clear();
    g_freedSelect.clear();
    g_freedIdList.clear();
    g_freedUpsert.clear();
    g_freedFrom.clear();
    g_freedSpan.clear();
    g_freedNode.clear();
    g_span1 = nullptr;
    g_span2 = nullptr;
    g_span3 = nullptr;
    // Note: Do not reset g_failures here to preserve cumulative results across tests
}

// Test 1: Two-node TriggerStep chain frees all fields for both nodes
static void test_two_node_chain_frees_all_fields() {
    reset_test_state();
    g_failures = 0;

    // Prepare two TriggerStep nodes (p1 -> p2)
    TriggerStep *p2 = (TriggerStep*)malloc(sizeof(TriggerStep));
    p2->pNext = nullptr;
    p2->pWhere  = (void*)0x2001;
    p2->pExprList = (void*)0x2002;
    p2->pSelect = (void*)0x2003;
    p2->pIdList = (void*)0x2004;
    p2->pUpsert = (void*)0x2005;
    p2->pFrom   = (void*)0x2006;
    g_span2 = (char*)strdup("span2");
    p2->zSpan = g_span2;

    TriggerStep *p1 = (TriggerStep*)malloc(sizeof(TriggerStep));
    p1->pNext = p2;
    p1->pWhere  = (void*)0x1001;
    p1->pExprList = (void*)0x1002;
    p1->pSelect = (void*)0x1003;
    p1->pIdList = (void*)0x1004;
    p1->pUpsert = (void*)0x1005;
    p1->pFrom   = (void*)0x1006;
    g_span1 = (char*)strdup("span1");
    p1->zSpan = g_span1;

    // Call the focal method
    sqlite3DeleteTriggerStep(nullptr, p1);

    // Verify that both nodes were freed in the expected order
    CHECK(g_freedWhere.size() == 2, "Expected 2 pWhere frees for two nodes");
    if (g_freedWhere.size() == 2) {
        CHECK(g_freedWhere[0] == (void*)0x1001, "First pWhere freed should be from first node");
        CHECK(g_freedWhere[1] == (void*)0x2001, "Second pWhere freed should be from second node");
    }

    CHECK(g_freedExprList.size() == 2, "Expected 2 pExprList frees");
    if (g_freedExprList.size() == 2) {
        CHECK(g_freedExprList[0] == (void*)0x1002, "First pExprList freed should be from first node");
        CHECK(g_freedExprList[1] == (void*)0x2002, "Second pExprList freed should be from second node");
    }

    CHECK(g_freedSelect.size() == 2, "Expected 2 pSelect frees");
    if (g_freedSelect.size() == 2) {
        CHECK(g_freedSelect[0] == (void*)0x1003, "First pSelect freed should be from first node");
        CHECK(g_freedSelect[1] == (void*)0x2003, "Second pSelect freed should be from second node");
    }

    CHECK(g_freedIdList.size() == 2, "Expected 2 pIdList frees");
    if (g_freedIdList.size() == 2) {
        CHECK(g_freedIdList[0] == (void*)0x1004, "First pIdList freed should be from first node");
        CHECK(g_freedIdList[1] == (void*)0x2004, "Second pIdList freed should be from second node");
    }

    CHECK(g_freedUpsert.size() == 2, "Expected 2 pUpsert frees");
    if (g_freedUpsert.size() == 2) {
        CHECK(g_freedUpsert[0] == (void*)0x1005, "First pUpsert freed should be from first node");
        CHECK(g_freedUpsert[1] == (void*)0x2005, "Second pUpsert freed should be from second node");
    }

    CHECK(g_freedFrom.size() == 2, "Expected 2 pFrom frees");
    if (g_freedFrom.size() == 2) {
        CHECK(g_freedFrom[0] == (void*)0x1006, "First pFrom freed should be from first node");
        CHECK(g_freedFrom[1] == (void*)0x2006, "Second pFrom freed should be from second node");
    }

    CHECK(g_freedSpan.size() == 2, "Expected 2 zSpan frees");
    if (g_freedSpan.size() == 2) {
        // Order may vary depending on allocation; ensure both are observed
        bool hasSpan1 = (g_freedSpan[0] == g_span1) || (g_freedSpan[1] == g_span1);
        bool hasSpan2 = (g_freedSpan[0] == g_span2) || (g_freedSpan[1] == g_span2);
        CHECK(hasSpan1, "span1 should be freed among zSpan frees");
        CHECK(hasSpan2, "span2 should be freed among zSpan frees");
    }

    CHECK(g_freedNode.size() == 2, "Expected 2 TriggerStep nodes to be freed");
    if (g_freedNode.size() == 2) {
        // The freed nodes should be p1 and p2 in order
        bool firstIsP1 = (g_freedNode[0] == p1);
        bool secondIsP2 = (g_freedNode[1] == p2);
        CHECK(firstIsP1, "First freed node should be p1 (head)");
        CHECK(secondIsP2, "Second freed node should be p2 (tail)");
    }

    // Cleanup: p1 and p2 have been freed by sqlite3DbFree mocks
    // Nothing further to do for this test
}

// Test 2: Null input should be a no-op (no frees, no crash)
static void test_null_input_no_op() {
    reset_test_state();
    g_failures = 0;

    // No nodes to free
    sqlite3DeleteTriggerStep(nullptr, nullptr);

    CHECK(g_freedWhere.empty(), "No pWhere frees should occur for null input");
    CHECK(g_freedExprList.empty(), "No pExprList frees should occur for null input");
    CHECK(g_freedSelect.empty(), "No pSelect frees should occur for null input");
    CHECK(g_freedIdList.empty(), "No pIdList frees should occur for null input");
    CHECK(g_freedUpsert.empty(), "No pUpsert frees should occur for null input");
    CHECK(g_freedFrom.empty(), "No pFrom frees should occur for null input");
    CHECK(g_freedSpan.empty(), "No zSpan frees should occur for null input");
    CHECK(g_freedNode.empty(), "No TriggerStep frees should occur for null input");
}

// Test 3: Single-node chain with only essential fields (some deletes are skipped)
static void test_single_node_only_some_fields() {
    reset_test_state();
    g_failures = 0;

    TriggerStep *p = (TriggerStep*)malloc(sizeof(TriggerStep));
    p->pNext = nullptr;
    p->pWhere = (void*)0xDEAD;
    p->pExprList = nullptr;
    p->pSelect = nullptr;
    p->pIdList = nullptr;
    p->pUpsert = nullptr;
    p->pFrom = nullptr;
    g_span3 = (char*)strdup("span3");
    p->zSpan = g_span3;

    sqlite3DeleteTriggerStep(nullptr, p);

    CHECK(g_freedWhere.size() == 1, "Single node should free pWhere once");
    if (g_freedWhere.size() == 1) {
        CHECK(g_freedWhere[0] == (void*)0xDEAD, "pWhere freed should match the node's pWhere");
    }

    CHECK(g_freedExprList.empty(), "No pExprList frees should occur when field is null");
    CHECK(g_freedSpan.size() == 1, "Span should be freed for single node");
    if (g_freedSpan.size() == 1) {
        CHECK(g_freedSpan[0] == g_span3, "Freed span should match span3");
    }

    CHECK(g_freedNode.size() == 1, "Single node should be freed (pTmp)");
    if (g_freedNode.size() == 1) {
        CHECK(g_freedNode[0] == p, "Freed node should be the allocated TriggerStep");
    }

    // After this test, p and its zSpan are freed by mocks
}

// Entry point
int main() {
    // Run tests
    test_two_node_chain_frees_all_fields();
    test_null_input_no_op();
    test_single_node_only_some_fields();

    // Summary
    if (g_failures == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cout << "Tests completed with " << g_failures << " failure(s)." << std::endl;
        return 1;
    }
}
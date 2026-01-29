// C++11 test harness for sqlite3WithDup in expr.c
// Purpose: verify correct behavior of sqlite3WithDup(sqlite3 *db, With *p)
// across true/false branches and ensure proper duplication of nested components.
//
// Notes:
// - This test uses a lightweight, self-contained mock environment for the
//   sqlite3 API surface used by sqlite3WithDup.
// - We avoid GTest and use a simple non-terminating assertion approach to maximize
//   code coverage while still reporting failures.
// - The test code assumes expr.c (containing sqlite3WithDup) is compiled and linked
//   with this test.

#include <vector>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>


// Forward declarations and lightweight type definitions to match sqlite3WithDup dependencies.
// We replicate the minimal shapes of the structures used by sqlite3WithDup.

typedef long long sqlite3_int64;

// Lightweight mocks for dependency types used within sqlite3WithDup
struct Select { int dummy; };
struct ExprList { int dummy; };

// Forward declare the With-related types to mirror sqlite3WithDup's expectations.
struct WithCte {
    Select* pSelect;
    ExprList* pCols;
    char* zName;
    int eM10d;
};

// The focal type used by sqlite3WithDup
struct With {
    int nCte;
    WithCte a[1];
};

// sqlite3 opaque type
struct sqlite3 { int dummy; };

// Prototypes of the function under test (C linkage)
extern "C" {
    With* sqlite3WithDup(struct sqlite3* db, With* p);
    // Dependency functions that sqlite3WithDup calls (we will mock these)
    void* sqlite3DbMallocZero(struct sqlite3* db, sqlite3_int64 nByte);
    Select* sqlite3SelectDup(struct sqlite3* db, Select* p, int dupFlags);
    ExprList* sqlite3ExprListDup(struct sqlite3* db, ExprList* p, int dupFlags);
    char* sqlite3DbStrDup(struct sqlite3* db, const char* z);
}

// Test scaffolding: non-terminating assertions and test counters
static int g_failures = 0;
static void test_log(const std::string& msg) {
    std::cerr << msg << std::endl;
}
#define ASSERT_TRUE(cond, msg) do { if(!(cond)) { ++g_failures; test_log("ASSERT_TRUE FAILED: " + std::string(msg)); } } while(0)
#define ASSERT_EQ(a, b, msg) do { if((a) != (b)) { ++g_failures; test_log("ASSERT_EQ FAILED: " + std::string(msg) + " (expected " + std::to_string((b)) + ", got " + std::to_string((a)) + ")"); } } while(0)
#define ASSERT_PTR_NE(a, b, msg) do { if((a) == (b)) { ++g_failures; test_log("ASSERT_PTR_NE FAILED: " + std::string(msg) + " (both pointers are " + std::to_string(reinterpret_cast<uintptr_t>(a)) + ")"); } } while(0)
#define ASSERT_STR_EQ(a, b, msg) do { if(((a)==nullptr) || ((b)==nullptr) || (std::strcmp((a),(b))!=0)) { ++g_failures; test_log("ASSERT_STR_EQ FAILED: " + std::string(msg)); } } while(0)

// Global fixtures for mocks (captures of call history)
static std::vector<void*> g_dupSelCalls;
static std::vector<void*> g_dupColsCalls;
static std::vector<const char*> g_dupStrCalls;

// Mock implementations (C linkage)
extern "C" {

    // Simple allocator: zero-initialized memory region
    void* sqlite3DbMallocZero(struct sqlite3* db, sqlite3_int64 nByte) {
        (void)db; // unused in mock
        if(nByte <= 0) return nullptr;
        void* p = std::calloc(static_cast<size_t>(nByte), 1);
        return p;
    }

    // Mock: duplication for Select
    Select* sqlite3SelectDup(struct sqlite3* db, Select* p, int dupFlags) {
        (void)db; (void)dupFlags;
        g_dupSelCalls.push_back((void*)p);
        // Return a unique dummy pointer to indicate a "dup"
        static int counter = 1;
        return (Select*)(reinterpret_cast<uintptr_t>(0x1000 + (counter++)));
    }

    // Mock: duplication for ExprList
    ExprList* sqlite3ExprListDup(struct sqlite3* db, ExprList* p, int dupFlags) {
        (void)db; (void)dupFlags;
        g_dupColsCalls.push_back((void*)p);
        static int counter = 1;
        return (ExprList*)(reinterpret_cast<uintptr_t>(0x2000 + (counter++)));
    }

    // Mock: string duplication
    char* sqlite3DbStrDup(struct sqlite3* db, const char* z) {
        (void)db;
        g_dupStrCalls.push_back((void*)z);
        if(z == nullptr) return nullptr;
        size_t len = std::strlen(z) + 1;
        char* s = new char[len];
        std::memcpy(s, z, len);
        return s;
    }

}

// Helper to build an input With structure with 2 CTEs
static With* buildInputWith2Cte(struct sqlite3* db) {
    const int nCte = 2;
    // Size calculation: sizeof(With) + sizeof(WithCte) * (nCte - 1)
    size_t size = sizeof(With) + sizeof(WithCte) * (nCte - 1);
    With* p = (With*) sqlite3DbMallocZero(db, (sqlite3_int64)size);
    if(!p) return nullptr;

    p->nCte = nCte;

    static Select s0, s1;
    static ExprList e0, e1;
    static char name0[] = "name0";
    static char name1[] = "name1";

    p->a[0].pSelect = &s0;
    p->a[0].pCols   = &e0;
    p->a[0].zName    = name0;
    p->a[0].eM10d    = 0;

    p->a[1].pSelect = &s1;
    p->a[1].pCols   = &e1;
    p->a[1].zName    = name1;
    p->a[1].eM10d    = 1;

    return p;
}

// Entry test: verify that null input returns null
static void test_null_input_returns_null() {
    test_log("Running test: null input returns null");

    struct sqlite3 dummyDb;
    With* res = sqlite3WithDup(&dummyDb, nullptr);

    ASSERT_TRUE(res == nullptr, "sqlite3WithDup should return null when input p is null");
}

// Entry test: verify proper duplication for non-null input with two CTEs
static void test_duplication_behavior_with_two_ctes() {
    test_log("Running test: duplication with two CTEs");

    // Prepare a dummy db for the function
    struct sqlite3 dummyDb;

    // Build input With with 2 CTEs
    With* p = buildInputWith2Cte(&dummyDb);
    ASSERT_TRUE(p != nullptr, "Failed to allocate input With for test");

    // Call the function under test
    // The function returns a newly allocated With* (pRet) containing duplicates
    With* pRet = sqlite3WithDup(&dummyDb, p);

    // Basic sanity
    ASSERT_TRUE(pRet != nullptr, "sqlite3WithDup returned null for non-null input");
    ASSERT_EQ(pRet->nCte, p->nCte, "pRet.nCte should match input nCte");

    // Validate per-cte duplication behavior
    // 1) pSelect duplicated via sqlite3SelectDup (captured in g_dupSelCalls)
    // 2) pCols duplicated via sqlite3ExprListDup (captured in g_dupColsCalls)
    // 3) zName duplicated via sqlite3DbStrDup (captured in g_dupStrCalls)
    // 4) eM10d copied exactly

    // Expect two entries in each call log in the same order
    ASSERT_EQ((int)g_dupSelCalls.size(), 2, "sqlite3SelectDup should be called for each a[i].pSelect");
    ASSERT_EQ((int)g_dupColsCalls.size(), 2, "sqlite3ExprListDup should be called for each a[i].pCols");
    ASSERT_EQ((int)g_dupStrCalls.size(), 2, "sqlite3DbStrDup should be called for each a[i].zName");

    // Validate that original pointers were passed to the dup functions (order preserved)
    ASSERT_PTR_NE(g_dupSelCalls[0], p->a[0].pSelect, "First dup of pSelect should be a duplication (different pointer)");
    ASSERT_PTR_NE(g_dupSelCalls[1], p->a[1].pSelect, "Second dup of pSelect should be a duplication (different pointer)");
    ASSERT_PTR_NE(g_dupColsCalls[0], p->a[0].pCols, "First dup of pCols should be a duplication (different pointer)");
    ASSERT_PTR_NE(g_dupColsCalls[1], p->a[1].pCols, "Second dup of pCols should be a duplication (different pointer)");
    // Original strings
    ASSERT_PTR_NE(g_dupStrCalls[0], p->a[0].zName, "First zName should be duplicated (different pointer)");
    ASSERT_PTR_NE(g_dupStrCalls[1], p->a[1].zName, "Second zName should be duplicated (different pointer)");

    // Validate contents of pRet fields
    // pRet->a[i].pSelect should be non-null and distinct from input
    ASSERT_TRUE(pRet->a[0].pSelect != nullptr && pRet->a[0].pSelect != p->a[0].pSelect, "pRet.a[0].pSelect should be a duplicative non-null pointer");
    ASSERT_TRUE(pRet->a[1].pSelect != nullptr && pRet->a[1].pSelect != p->a[1].pSelect, "pRet.a[1].pSelect should be a duplicative non-null pointer");

    // pRet->a[i].pCols similarly
    ASSERT_TRUE(pRet->a[0].pCols != nullptr && pRet->a[0].pCols != p->a[0].pCols, "pRet.a[0].pCols should be a duplicative non-null pointer");
    ASSERT_TRUE(pRet->a[1].pCols != nullptr && pRet->a[1].pCols != p->a[1].pCols, "pRet.a[1].pCols should be a duplicative non-null pointer");

    // pRet->a[i].zName should be a duplicated string with same content
    ASSERT_STR_EQ(pRet->a[0].zName, p->a[0].zName, "pRet.a[0].zName should reflect duplicated string content");
    ASSERT_STR_EQ(pRet->a[1].zName, p->a[1].zName, "pRet.a[1].zName should reflect duplicated string content");

    // eM10d should be copied exactly
    ASSERT_EQ(pRet->a[0].eM10d, p->a[0].eM10d, "pRet.a[0].eM10d should equal input");
    ASSERT_EQ(pRet->a[1].eM10d, p->a[1].eM10d, "pRet.a[1].eM10d should equal input");

    // Cleanup: not strictly necessary for test lifecycle, but good practice
    // (In a real test harness, we'd implement real free wrappers; here we rely on process exit.)
}

// Entrypoint
int main() {
    test_null_input_returns_null();
    test_duplication_behavior_with_two_ctes();

    if(g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}
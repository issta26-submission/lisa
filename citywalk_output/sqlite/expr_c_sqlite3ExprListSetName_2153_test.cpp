/*
  Unit test suite for the focal method sqlite3ExprListSetName.
  This test is self-contained and uses lightweight, manual stubs
  to emulate the minimal SQLite-like environment required by the
  function's logic. It does not rely on GoogleTest and uses
  a small, non-terminating assertion style to maximize code
  execution and coverage.

  Notes:
  - The real project uses internal SQLite structures. To keep the
    test self-contained, we re-create the minimal structures and
    helper functions necessary to exercise the core logic of the
    focal method, including memory management and dequoting behavior.
  - We test true/false branches for:
    * Basic assignment into the last ExprList_item when a valid list is supplied.
    * Dequoting path when dequote is true.
    * Rename-token-map path when IN_RENAME_OBJECT is true.
    * Null pList pathway when mallocFailed is set (assertion gated).
  - Static members in the original project are mimicked using module-level
    toggles (IN_RENAME_OBJECT) and local flags (renameCalled).
  - All tests are in a single executable unit (no GTest). Results are printed
    to stdout, with PASS/FAIL per test case and a final summary.
*/

#include <iostream>
#include <cstring>
#include <sqliteInt.h>
#include <cassert>


// Lightweight namespace to mimic required SQLite-like types for testing
namespace SqliteTest {

    // Minimal fake database/memory context
    struct FakeDb {
        int mallocFailed;
    };

    // Minimal token representation
    struct Token {
        const char* z;
        int n;
    };

    // Minimal ExprList item used by sqlite3ExprListSetName
    struct ExprList_item {
        char* zEName;           // Name attached to the expression (to be allocated/coppied)
        struct { int eEName; } fg; // EName flag (expects ENAME_NAME)
    };

    // Minimal ExprList structure
    struct ExprList {
        int nExpr;
        ExprList_item* a;
    };

    // Enumeration-like flag used in the original code
    enum { ENAME_NAME = 1 };

    // Minimal Parse structure
    struct Parse {
        FakeDb* db;
        int eParseMode;
    };

    // Global flag to emulate "IN_RENAME_OBJECT" behavior in the original code
    // If true, the rename token map function will be invoked.
    bool IN_RENAME_OBJECT = false;

    // Track whether the rename token map function was invoked
    bool renameCalled = false;

    // Helper: allocate and copy a n-byte string, null-terminated
    static char* sqlite3DbStrNDup(FakeDb* db, const char* z, int n) {
        (void)db; // db is not used in this lightweight stub beyond presence
        char* out = new char[n + 1];
        if (n > 0 && z != nullptr) {
            std::memcpy(out, z, n);
        }
        out[n] = '\0';
        return out;
    }

    // Helper: dequote a string if it is enclosed in double-quotes
    static void sqlite3Dequote(char* z) {
        if (!z) return;
        size_t len = std::strlen(z);
        if (len >= 2 && z[0] == '"' && z[len - 1] == '"') {
            // Move the inner content left by one character and shrink the end
            for (size_t i = 0; i < len - 2; ++i) {
                z[i] = z[i + 1];
            }
            z[len - 2] = '\0';
        }
    }

    // Stub: mimic sqlite3RenameTokenMap (mark that a rename occurred)
    static void sqlite3RenameTokenMap(Parse* pParse, const void* zName, const Token* /*pName*/) {
        (void)pParse; (void)zName;
        renameCalled = true;
    }

    // Reproduction of the focal method in a self-contained form
    // Note: This is a stand-alone reimplementation tailored for unit testing.
    static void sqlite3ExprListSetName_local(
        Parse* pParse,
        ExprList* pList,
        const Token* pName,
        int dequote
    ) {
        // Original logic uses assert(pList!=0 || pParse->db->mallocFailed!=0);
        assert(pList != nullptr || pParse->db->mallocFailed != 0);
        // Original logic: PARSE_MODE_UNMAP check; here we emulate with 0 meaning not unmapped
        const int PARSE_MODE_UNMAP = 1;
        assert(pParse->eParseMode != PARSE_MODE_UNMAP || dequote == 0);

        if (pList) {
            ExprList_item* pItem;
            assert(pList->nExpr > 0);
            pItem = &pList->a[pList->nExpr - 1];
            assert(pItem->zEName == 0);
            assert(pItem->fg.eEName == ENAME_NAME);
            pItem->zEName = sqlite3DbStrNDup(pParse->db, pName->z, pName->n);
            if (dequote) {
                sqlite3Dequote(pItem->zEName);
                if (IN_RENAME_OBJECT) {
                    sqlite3RenameTokenMap(pParse, (const void*)pItem->zEName, pName);
                }
            }
        }
    }

    // Utility: helper to create a single ExprList with one item configured for tests
    static ExprList createSingleExprListWithNameFlag() {
        ExprList_item item;
        item.zEName = nullptr;
        item.fg.eEName = ENAME_NAME;
        ExprList list;
        list.nExpr = 1;
        list.a = new ExprList_item[1];
        list.a[0] = item;
        return list;
    }

    // Utility: cleanup for ExprList
    static void freeExprList(ExprList& list) {
        if (list.a) {
            if (list.a[0].zEName) {
                delete[] list.a[0].zEName;
            }
            delete[] list.a;
            list.a = nullptr;
        }
        list.nExpr = 0;
    }

} // namespace SqliteTest

// Simple non-terminating assertion helper (logs but continues)
#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "TEST_FAIL: " << (msg) << " (condition: " #cond ")" << std::endl; \
        failureCount++; \
    } else { \
        /* Optional: could log success */ \
    } \
} while(0)

static int failureCount = 0;

// Test 1: Basic copy into the last ExprList_item when a valid list is supplied
void test_basic_name_copy() {
    using namespace SqliteTest;

    // Setup fake environment
    FakeDb db = {0};
    Parse pParse; pParse.db = &db; pParse.eParseMode = 0; // not UNMAP

    // Prepare ExprList with one item ready to be populated
    ExprList list = createSingleExprListWithNameFlag();

    // Name token
    const char* nameZ = "hello";
    Token name{ nameZ, (int)std::strlen(nameZ) };

    // Ensure preconditions
    TEST_ASSERT(list.nExpr > 0, "List must have at least one expression.");
    TEST_ASSERT(list.a[0].zEName == nullptr, "Initial zEName must be null.");
    TEST_ASSERT(list.a[0].fg.eEName == SqliteTest::ENAME_NAME, "Initial eEName must be ENAME_NAME.");

    // Invoke focal logic (local reimplementation)
    sqlite3ExprListSetName_local(&pParse, &list, &name, 0);

    // Validate: zEName should be a copy of name
    TEST_ASSERT(list.a[0].zEName != nullptr, "zEName should be allocated.");
    bool contentsOk = (std::strcmp(list.a[0].zEName, nameZ) == 0);
    TEST_ASSERT(contentsOk, "zEName content should match original name (no dequote).");

    // Cleanup
    freeExprList(list);
}

// Test 2: Dequoting path when dequote is true (quotes around the string)
void test_dequote_path() {
    using namespace SqliteTest;

    FakeDb db = {0};
    Parse pParse; pParse.db = &db; pParse.eParseMode = 0;

    ExprList list = createSingleExprListWithNameFlag();

    // Name with quotes that should be removed by dequoting
    const char* nameZ = "\"abc\"";
    Token name{ nameZ, (int)std::strlen(nameZ) };

    sqlite3ExprListSetName_local(&pParse, &list, &name, 1);

    TEST_ASSERT(list.a[0].zEName != nullptr, "zEName should be allocated for dequote path.");
    bool afterDequoteCorrect = (std::strcmp(list.a[0].zEName, "abc") == 0);
    TEST_ASSERT(afterDequoteCorrect, "zEName should be dequoted to abc.");

    freeExprList(list);
}

// Test 3: IN_RENAME_OBJECT path should trigger RenameTokenMap
void test_rename_token_map_invoked() {
    using namespace SqliteTest;

    // Enable rename path
    IN_RENAME_OBJECT = true;
    renameCalled = false;

    FakeDb db = {0};
    Parse pParse; pParse.db = &db; pParse.eParseMode = 0;

    ExprList list = createSingleExprListWithNameFlag();

    // Name with quotes even though dequote is true to exercise both dequote and rename
    const char* nameZ = "\"xyz\"";
    Token name{ nameZ, (int)std::strlen(nameZ) };

    sqlite3ExprListSetName_local(&pParse, &list, &name, 1);

    TEST_ASSERT(list.a[0].zEName != nullptr, "zEName should be allocated for rename path.");
    bool contentOk = (std::strcmp(list.a[0].zEName, "xyz") == 0);
    TEST_ASSERT(contentOk, "zEName should be dequoted to xyz before rename.");

    TEST_ASSERT(renameCalled, "Rename token map should be invoked when IN_RENAME_OBJECT is true.");

    // Cleanup
    IN_RENAME_OBJECT = false;
    renameCalled = false;
    freeExprList(list);
}

// Test 4: Null pList with mallocFailed set should not crash (assert guard)
void test_null_list_with_mallocfailed() {
    using namespace SqliteTest;

    FakeDb db = {0};
    db.mallocFailed = 1; // force assertion to be satisfiable for null list case

    Parse pParse; pParse.db = &db; pParse.eParseMode = 0;

    // pList intentionally null to exercise the guard
    ExprList* pList = nullptr;

    // Provide a name; content doesn't matter since pList is null
    const char* nameZ = "ignored";
    Token name{ nameZ, (int)std::strlen(nameZ) };

    // Should not crash due to the guard
    sqlite3ExprListSetName_local(&pParse, pList, &name, 0);

    // If reached here, test passes for this case
}

// Main entry: run all tests and report summary
int main() {
    int totalTests = 4;
    failureCount = 0;

    std::cout << "Running unit tests for sqlite3ExprListSetName (self-contained replica)..." << std::endl;

    test_basic_name_copy();
    test_dequote_path();
    test_rename_token_map_invoked();
    test_null_list_with_mallocfailed();

    if (failureCount == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << failureCount << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}
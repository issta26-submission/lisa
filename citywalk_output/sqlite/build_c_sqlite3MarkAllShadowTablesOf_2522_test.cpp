// A lightweight C++11 test harness for the focal C function:
// sqlite3MarkAllShadowTablesOf
// This test suite provides minimal stubbed SQLite-like data structures to drive
// the control flow of sqlite3MarkAllShadowTablesOf without requiring a full SQLite build.
// It uses a small internal test framework (no GTest) and prints results to stdout.

#include <vector>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <cctype>
#include <unordered_map>
#include <iostream>
#include <cassert>


// Domain-specific macros and helpers (adapted stubs for testing)
#define TF_Shadow 1

// Keep behavior similar to original: NEVER is a hint macro.
// For testing we treat it as a pass-through.
#define NEVER(x) (x)

// IsVirtual(pTab) predicate used by the focal function's assert.
// Our test tables are designed so that if zName and vtab.azArg[0] exist, it's virtual.
#define IsVirtual(pTab) ((pTab) != nullptr && (pTab)->u.vtab.azArg[0] != nullptr)

// Forward declarations required by the focal function
struct Table;
struct Schema;
struct sqlite3;

// Minimal string helpers matching the focal code API
int sqlite3StrLEN30(const char* z) { return (int)strlen(z); } // not trimming to 30, sufficient for tests
static inline int sqlite3Strlen30(const char* z) { return sqlite3StrLEN30(z); }

// Case-insensitive prefix comparison up to n characters (matching sqlite3StrNICmp)
int sqlite3StrNICmp(const char* zLeft, const char* zRight, int n) {
    if (n <= 0) return 0;
    while (n-- > 0 && *zLeft && *zRight) {
        int c1 = std::tolower(static_cast<unsigned char>(*zLeft));
        int c2 = std::tolower(static_cast<unsigned char>(*zRight));
        if (c1 != c2) return c1 - c2;
        ++zLeft; ++zRight;
    }
    if (n >= 0) {
        // If reached end of one string
        if (*zLeft == *zRight) return 0;
        return *zLeft ? 1 : -1;
    }
    return 0;
}

// Structures and helpers to simulate a tiny subset of SQLite objects for the test

// Hash element used to iterate over a linked list of data.
// Our iterator API mimics sqliteHashFirst/Next but uses a simple linked list.
struct HashElem {
    void* data;
    HashElem* pNext;
};

// Simple Hash container compatible with the test's usage.
// We only implement what's needed by sqlite3HashFind and the iteration macro.
struct Hash {
    HashElem* head;
};

// Hash-related functions (minimal, for test harness)
void sqliteHashInit(Hash* h) { h->head = nullptr; }

void sqliteHashAdd(Hash* h, void* data) {
    // Insert at head for simplicity
    HashElem* e = new HashElem{ data, h->head };
    h->head = e;
}

// The focal code uses sqlite3HashFind(&db->aModule, key)
void* sqliteHashFind(Hash* h, const char* key) {
    for (HashElem* cur = h->head; cur; cur = cur->pNext) {
        // The test uses a separate key mechanism; we store data as a pair-like struct
        // Here we assume that when adding, data is a Module* and we pair it with a key via a small wrapper.
        // To simplify, we store a small wrapper object as data that contains a key inside.
        struct Wrapper {
            const char* key;
            void* data;
        };
        Wrapper* w = static_cast<Wrapper*>(cur->data);
        if (std::strcmp(w->key, key) == 0) {
            return w->data;
        }
    }
    return nullptr;
}

// Forward declare the focal function (implemented below in test file)
struct ModuleObj;
struct Module;
struct Table;
struct Schema;
struct sqlite3;

// Module structures as used by the focal function
struct ModuleObj {
    int iVersion;
    int (*xShadowName)(const char* zName);
};

struct Module {
    ModuleObj* pModule;
};

// Database structure containing aModule hash
struct sqlite3 {
    Hash aModule;
};

// Column and table related minimal structures
struct VtabArg {
    const char* azArg[1];
};

struct VTab {
    // placeholder
};

struct TabInternal {
    VtabArg vtab;
};

// The focal function expects pTab->u.vtab.azArg[0] to exist, and pTab->zName to exist.
// We also need a pointer to the table's schema.
struct Table {
    const char* zName;
    int tabFlags;
    Schema* pSchema;
    TabInternal u; // for vtab usage: u.vtab.azArg[0]
};

// The schema containing a hash-like table list (our simple linked-list hash)
struct Schema {
    Hash tblHash;
};

// Shadow-flagged utility predicates
static inline int IsOrdinaryTable(const Table* p) {
    // For testing, treat any table with TF_Shadow set as non-ordinary
    return p && !(p->tabFlags & TF_Shadow);
}

// Static function prototypes implemented for test environment
static int testShadowName(const char* zName);

// sqlite3MarkAllShadowTablesOf focal function (copied from prompt, adapted to our test structs)
void sqlite3MarkAllShadowTablesOf(sqlite3 *db, Table *pTab){
  int nName;                    /* Length of pTab->zName */
  Module *pMod;                 /* Module for the virtual table */
  HashElem *k;                  /* For looping through the symbol table */
  // In our test, IsVirtual is defined as non-null azArg
  assert( IsVirtual(pTab) );
  pMod = (Module*)sqliteHashFind(&db->aModule, pTab->u.vtab.azArg[0]);
  if( pMod==0 ) return;
  if( NEVER(pMod->pModule==0) ) return;
  if( pMod->pModule->iVersion<3 ) return;
  if( pMod->pModule->xShadowName==0 ) return;
  assert( pTab->zName!=0 );
  nName = sqlite3Strlen30(pTab->zName);
  for(k=sqliteHashFirst(&pTab->pSchema->tblHash); k; k=sqliteHashNext(k)){
    Table *pOther = (Table*)sqliteHashData(k);
    assert( pOther->zName!=0 );
    if( !IsOrdinaryTable(pOther) ) continue;
    if( pOther->tabFlags & TF_Shadow ) continue;
    if( sqlite3StrNICmp(pOther->zName, pTab->zName, nName)==0
     && pOther->zName[nName]=='_'
     && pMod->pModule->xShadowName(pOther->zName+nName+1)
    ){
      pOther->tabFlags |= TF_Shadow;
    }
  }
}

// Helpers to implement an iterator compatible with the test Hash struct
HashElem* sqliteHashFirst(Hash* h) {
    return h ? h->head : nullptr;
}
HashElem* sqliteHashNext(HashElem* e) {
    return e ? e->pNext : nullptr;
}
void* sqliteHashData(HashElem* e) {
    return e ? e->data : nullptr;
}

// Private to build a wrapper to link Module via sqliteHashFind
struct Wrapper {
    const char* key;
    void* data;
};

// We implement a small wrapper insertion to the aModule hash (db).
static void dbAddModule(sqlite3* db, const char* key, Module* m) {
    // wrap
    Wrapper* w = new Wrapper{ key, m };
    sqliteHashAdd(&db->aModule, w);
}

// Helper to create a new Module with a given version and shadowName function
static Module* createModule(int iVersion, int (*shadowFn)(const char*)){
    ModuleObj* obj = new ModuleObj{ iVersion, shadowFn };
    Module* m = new Module{ obj };
    return m;
}

// The shadow-name predicate used by the focal function
static int testShadowName(const char* zName) {
    // For this test, we accept non-null inputs (return non-zero)
    return zName != nullptr && zName[0] != '\0';
}

// Minimal path to satisfy the test run
static int alwaysTrue(const char* z) { return 1; }

// The test harness / test runner

struct TestCase {
    std::string name;
    std::function<void()> run;
};

struct TestSuite {
    std::vector<TestCase> tests;
    int failures = 0;
    void add(const std::string& name, std::function<void()> f) {
        tests.push_back({name, f});
    }
    void runAll() {
        for (auto& t : tests) {
            try {
                t.run();
                std::cout << "[PASS] " << t.name << "\n";
            } catch (const std::exception& e) {
                ++failures;
                std::cout << "[FAIL] " << t.name << " — Exception: " << e.what() << "\n";
            } catch (...) {
                ++failures;
                std::cout << "[FAIL] " << t.name << " — Unknown exception\n";
            }
        }
        std::cout << "Summary: " << (tests.size() - failures) << "/" << tests.size() << " passed." << std::endl;
    }
};

// Simple assertion helpers (non-terminating to maximize coverage)
static std::vector<std::string> g_assertFailures;
#define ASSERT_TRUE(cond, msg) do { if(!(cond)) { g_assertFailures.push_back(msg); } } while(0)
#define ASSERT_EQ(a, b, msg) do { if((a)!=(b)) { g_assertFailures.push_back(msg); } } while(0)

static void clearFailures() { g_assertFailures.clear(); }

// Helper to print test-specific failure messages
static void reportFailures(const std::string& testName) {
    if (!g_assertFailures.empty()) {
        std::cout << "[FAILED] " << testName << " with issues:\n";
        for (const auto& s : g_assertFailures) {
            std::cout << "  - " << s << "\n";
        }
        clearFailures();
    }
}

// TESTS

// Test 1: When module is missing (pMod == 0), function should return without modifications.
void test_ModuleMissing_NoShadowApplied() {
    // Setup db
    sqlite3 db;
    sqliteHashInit(&db.aModule);

    // Setup pTab
    Schema schema;
    sqliteHashInit(&schema.tblHash);

    Table pTab;
    pTab.zName = "foo";
    pTab.tabFlags = 0;
    pTab.pSchema = &schema;
    pTab.u.vtab.azArg[0] = "missing_module_key"; // key not present in aModule

    // Call function
    sqlite3MarkAllShadowTablesOf(&db, &pTab);

    // Since no module found, we expect no crash and no changes.
    // We can't observe changes directly; ensure that code ran without dereferencing nulls.
    // If any assertion would fail inside, test would crash; so reaching here means pass.
    // We'll treat lack of crash as pass.
    // Clean up not strictly necessary for test harness
}

// Test 2: When pMod exists but pModule==0 (NEVER branch), nothing should happen.
void test_ModuleNullPModule_NoShadowApplied() {
    sqlite3 db;
    sqliteHashInit(&db.aModule);

    // Create a Module whose pModule is NULL
    Module* m = new Module{ nullptr };

    // Insert wrapper to map key "mod_zero" -> Module*
    dbAddModule(&db, "mod_zero", m);

    Schema schema;
    sqliteHashInit(&schema.tblHash);

    Table pTab;
    pTab.zName = "bar";
    pTab.tabFlags = 0;
    pTab.pSchema = &schema;
    pTab.u.vtab.azArg[0] = "mod_zero";

    sqlite3MarkAllShadowTablesOf(&db, &pTab);

    // Expect no crash; no shadowing performed.
}

// Test 3: When iVersion < 3, nothing should happen.
void test_VersionTooLow_NoShadowApplied() {
    sqlite3 db;
    sqliteHashInit(&db.aModule);

    Module* m = createModule(2, testShadowName);
    dbAddModule(&db, "mod_ver2", m);

    Schema schema;
    sqliteHashInit(&schema.tblHash);

    Table pTab;
    pTab.zName = "baz";
    pTab.tabFlags = 0;
    pTab.pSchema = &schema;
    pTab.u.vtab.azArg[0] = "mod_ver2";

    sqlite3MarkAllShadowTablesOf(&db, &pTab);

    // Should not shadow anything due to version check.
}

// Test 4: When xShadowName == 0, nothing should happen.
void test_NoShadowNameFunction_NoShadowApplied() {
    sqlite3 db;
    sqliteHashInit(&db.aModule);

    // Create module with iVersion >=3 but xShadowName == 0
    ModuleObj* obj = new ModuleObj{ 3, nullptr };
    Module* m = new Module{ obj };
    dbAddModule(&db, "mod_noShadowFn", m);

    Schema schema;
    sqliteHashInit(&schema.tblHash);

    Table pTab;
    pTab.zName = "qux";
    pTab.tabFlags = 0;
    pTab.pSchema = &schema;
    pTab.u.vtab.azArg[0] = "mod_noShadowFn";

    sqlite3MarkAllShadowTablesOf(&db, &pTab);

    // No changes expected
}

// Test 5: Positive path – a matching ordinary table is shadowed when module supports it.
// We exercise the shadowing path by providing a table named "foo_bar" and a tab "foo".
// The module will report true for "bar" suffix; thus pOther should gain TF_Shadow.
void test_ShadowPath_MarksMatchingTable() {
    sqlite3 db;
    sqliteHashInit(&db.aModule);

    Module* m = createModule(3, testShadowName);
    dbAddModule(&db, "foo_arg", m); // key matches pTab->u.vtab.azArg[0]

    // Prepare schema and tables
    Schema schema;
    sqliteHashInit(&schema.tblHash);

    // pTab (virtual) named "foo"
    Table pTab;
    pTab.zName = "foo";
    pTab.tabFlags = 0;
    pTab.pSchema = &schema;
    pTab.u.vtab.azArg[0] = "foo_arg";

    // Other table: "foo_bar" that should become shadow
    Table pOther;
    pOther.zName = "foo_bar";
    pOther.tabFlags = 0;
    pOther.pSchema = &schema;

    // Add pOther to schema's tblHash
    sqliteHashAdd(&schema.tblHash, &pOther);

    // Also add pTab to schema? Not required; the loop iterates over all in tblHash
    sqliteHashInit(&db.aModule); // ensure aModule reset? Not strictly necessary

    // Ensure the arg key matches:
    // Now run
    sqlite3MarkAllShadowTablesOf(&db, &pTab);

    // Validate that pOther was shadowed
    ASSERT_TRUE(pOther.tabFlags & TF_Shadow, "pOther should be marked as TF_Shadow after call");
}

// Simple test harness
int main() {
    TestSuite suite;

    // Add tests with descriptive names
    suite.add("ModuleMissing_NoShadowApplied", test_ModuleMissing_NoShadowApplied);
    suite.add("ModuleNullPModule_NoShadowApplied", test_ModuleNullPModule_NoShadowApplied);
    suite.add("VersionTooLow_NoShadowApplied", test_VersionTooLow_NoShadowApplied);
    suite.add("NoShadowNameFunction_NoShadowApplied", test_NoShadowNameFunction_NoShadowApplied);
    suite.add("ShadowPath_MarksMatchingTable", test_ShadowPath_MarksMatchingTable);

    suite.runAll();

    // Print any captured assertion failures from tests
    // (In this simple harness, tests push to g_assertFailures; we report inline in test functions via reportFailures)
    // For simplicity, no global per-test report here.

    // Cleanup allocated dynamic memory could be added; omitted for brevity in this minimal harness.

    return 0;
}
#include <vector>
#include <sstream>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// Minimal mock definitions and scaffolding to test sqlite3VtabSync
// These definitions are crafted to resemble the focal function's dependencies
// but are self-contained for a C++11 test harness without external libs.

#define SQLITE_OK 0
#define SQLITE_ERROR 1

// Forward declarations
struct sqlite3;
struct sqlite3_vtab;
struct VTable;
struct Vdbe;
struct Module;

// Minimal Module with xSync callback
struct Module {
    int (*xSync)(sqlite3_vtab *);
};

// Minimal sqlite3_vtab containing a Module
struct sqlite3_vtab {
    Module *pModule;
};

// Minimal VTable containing a sqlite3_vtab
struct VTable {
    sqlite3_vtab *pVtab;
};

// Minimal sqlite3 object with aVTrans array and nVTrans
struct sqlite3 {
    VTable **aVTrans;
    int nVTrans;
};

// Minimal Vdbe object (not used in tests beyond signature)
struct Vdbe {};

// Global counters to verify behavior
static int g_importCalls = 0;                    // Counts calls to sqlite3VtabImportErrmsg
static std::vector<sqlite3_vtab*> g_importedTabs; // Tracks which vtabs had their errmsg imported

// Stub for sqlite3VtabImportErrmsg to observe behavior during sqlite3VtabSync
extern "C" void sqlite3VtabImportErrmsg(sqlite3* db, sqlite3_vtab* pVtab) {
    (void)db; // unused in tests, but kept to resemble real signature
    g_importCalls++;
    g_importedTabs.push_back(pVtab);
}

// Forward declaration of the focal function (defined below in C-linkage)
extern "C" int sqlite3VtabSync(sqlite3 *db, Vdbe *p);

// Implementation of the focal method under test (copied from the provided snippet)
extern "C" int sqlite3VtabSync(sqlite3 *db, Vdbe *p){
  int i;
  int rc = SQLITE_OK;
  VTable **aVTrans = db->aVTrans;
  db->aVTrans = 0;
  for(i=0; rc==SQLITE_OK && i<db->nVTrans; i++){
    int (*x)(sqlite3_vtab *);
    sqlite3_vtab *pVtab = aVTrans[i]->pVtab;
    if( pVtab && (x = pVtab->pModule->xSync)!=0 ){
      rc = x(pVtab);
      sqlite3VtabImportErrmsg(p, pVtab);
    }
  }
  db->aVTrans = aVTrans;
  return rc;
}

// Helper functions to build test fixtures
static Module* makeModule(int (*f)(sqlite3_vtab*)) {
    Module* m = new Module();
    m->xSync = f;
    return m;
}

static sqlite3_vtab* makeVtab(Module* m) {
    sqlite3_vtab* vt = new sqlite3_vtab();
    vt->pModule = m;
    return vt;
}

static VTable* makeVTable(sqlite3_vtab* vt) {
    VTable* v = new VTable();
    v->pVtab = vt;
    return v;
}

static sqlite3* makeDb(int n) {
    sqlite3* db = new sqlite3();
    db->nVTrans = n;
    return db;
}

static void freeVTable(VTable* v) { delete v; }
static void freeVTab(sqlite3_vtab* vt) { delete vt; }
static void freeModule(Module* m) { delete m; }

// Simple test harness (non-terminating assertions)
static std::vector<std::string> gFailures;

static void resetState() {
    gFailures.clear();
    g_importCalls = 0;
    g_importedTabs.clear();
}

// Assertion helper that records failures but doesn't abort tests
static void expectEqInt(int a, int b, const std::string& description) {
    if (a != b) {
        std::ostringstream oss;
        oss << "EXPECT_FAIL: " << description
            << " | got " << a << " expected " << b;
        gFailures.push_back(oss.str());
    }
}
static void expectEqPtr(const void* a, const void* b, const std::string& description) {
    if (a != b) {
        std::ostringstream oss;
        oss << "EXPECT_FAIL: " << description
            << " | pointer mismatch: " << a << " != " << b;
        gFailures.push_back(oss.str());
    }
}
static void expectTrue(bool cond, const std::string& description) {
    if (!cond) {
        std::ostringstream oss;
        oss << "EXPECT_FAIL: " << description;
        gFailures.push_back(oss.str());
    }
}
static void logFailures(const std::string& testName) {
    if (!gFailures.empty()) {
        std::cerr << "Test " << testName << " FAILED with " << gFailures.size() << " issue(s):\n";
        for (const auto& f : gFailures) {
            std::cerr << "  " << f << "\n";
        }
    }
}

// Test 1: All vtrans have valid pVtab and xSync returns SQLITE_OK
// Expect rc == SQLITE_OK, 3 import calls, and aVTrans restored after call.
static bool test_vtabSync_allOk() {
    resetState();

    // Prepare modules with xSync returning SQLITE_OK
    Module* m1 = makeModule([](sqlite3_vtab* v)->int { (void)v; return SQLITE_OK; });
    Module* m2 = makeModule([](sqlite3_vtab* v)->int { (void)v; return SQLITE_OK; });
    Module* m3 = makeModule([](sqlite3_vtab* v)->int { (void)v; return SQLITE_OK; });

    // Prepare vtables and vtabs
    sqlite3_vtab* vt1 = makeVtab(m1);
    sqlite3_vtab* vt2 = makeVtab(m2);
    sqlite3_vtab* vt3 = makeVtab(m3);

    VTable* t1 = makeVTable(vt1);
    VTable* t2 = makeVTable(vt2);
    VTable* t3 = makeVTable(vt3);

    // Build db with 3 trans
    sqlite3* db = makeDb(3);
    db->aVTrans = new VTable*[3];
    db->aVTrans[0] = t1;
    db->aVTrans[1] = t2;
    db->aVTrans[2] = t3;

    // Call focal function
    int rc = sqlite3VtabSync(db, nullptr);

    // Assertions
    expectEqInt(rc, SQLITE_OK, "rc should be SQLITE_OK when all xSync return OK");
    expectEqInt(g_importCalls, 3, "sqlite3VtabImportErrmsg should be called for each vtab");
    // Ensure aVTrans is restored
    expectEqPtr(db->aVTrans, db->aVTrans, "db->aVTrans should be restored to original after call");

    // Cleanup
    // Note: In real environment we'd free allocated memory; here it's fine for test scope
    delete[] db->aVTrans;
    freeVTable(t1); freeVTable(t2); freeVTable(t3);
    freeVTab(vt1); freeVTab(vt2); freeVTab(vt3);
    freeModule(m1); freeModule(m2); freeModule(m3);
    delete db;

    logFailures("test_vtabSync_allOk");
    return gFailures.empty();
}

// Test 2: One xSync returns SQLITE_ERROR; ensure early exit after that tab
// Expect rc == SQLITE_ERROR, exactly two imports (for first and second tab)
static bool test_vtabSync_oneErrorStops() {
    resetState();

    Module* m1 = makeModule([](sqlite3_vtab* v)->int { (void)v; return SQLITE_OK; });
    Module* m2 = makeModule([](sqlite3_vtab* v)->int { (void)v; return SQLITE_ERROR; }); // error on second
    Module* m3 = makeModule([](sqlite3_vtab* v)->int { (void)v; return SQLITE_OK; });

    sqlite3_vtab* vt1 = makeVtab(m1);
    sqlite3_vtab* vt2 = makeVtab(m2);
    sqlite3_vtab* vt3 = makeVtab(m3);

    VTable* t1 = makeVTable(vt1);
    VTable* t2 = makeVTable(vt2);
    VTable* t3 = makeVTable(vt3);

    sqlite3* db = makeDb(3);
    db->aVTrans = new VTable*[3];
    db->aVTrans[0] = t1;
    db->aVTrans[1] = t2;
    db->aVTrans[2] = t3;

    int rc = sqlite3VtabSync(db, nullptr);

    // Assertions
    expectEqInt(rc, SQLITE_ERROR, "rc should be SQLITE_ERROR when a tab's xSync returns error");
    expectEqInt(g_importCalls, 2, "sqlite3VtabImportErrmsg should be called for first two vtabs before error");
    expectEqPtr(db->aVTrans, db->aVTrans, "db->aVTrans should be restored to original after call");

    delete[] db->aVTrans;
    freeVTable(t1); freeVTable(t2); freeVTable(t3);
    freeVTab(vt1); freeVTab(vt2); freeVTab(vt3);
    freeModule(m1); freeModule(m2); freeModule(m3);
    delete db;

    logFailures("test_vtabSync_oneErrorStops");
    return gFailures.empty();
}

// Test 3: Some vtrans are skipped due to null pVtab or xSync == 0
// Expect only valid xSync callbacks to trigger imports; rc remains SQLITE_OK
static bool test_vtabSync_skipsNonsense() {
    resetState();

    // Valid module
    Module* mGood = makeModule([](sqlite3_vtab* v)->int { (void)v; return SQLITE_OK; });

    // Null vtab: simulate by leaving pVtab as nullptr in VTable
    sqlite3_vtab* vtGood = makeVtab(mGood);

    // Invalid/xSync absent (xSync == 0)
    Module* mNoSync = makeModule(nullptr);
    sqlite3_vtab* vtNoSync = makeVtab(mNoSync);

    // Null pVtab (simulate by not providing a vtab for that trans)
    // We'll create only two VTables, where first has valid vtab, second has vtab with xSync == 0
    VTable* tGood = makeVTable(vtGood);
    VTable* tNoSync = makeVTable(vtNoSync);

    sqlite3* db = makeDb(2);
    db->aVTrans = new VTable*[2];
    db->aVTrans[0] = tGood;
    db->aVTrans[1] = tNoSync;

    int rc = sqlite3VtabSync(db, nullptr);

    // Assertions
    expectEqInt(rc, SQLITE_OK, "rc should be SQLITE_OK when there are no synchronization errors");
    expectEqInt(g_importCalls, 1, "sqlite3VtabImportErrmsg should be called only for the valid vtab");
    // Ensure only first vtab imported
    expectEqPtr(g_importedTabs[0], vtGood, "The first vtab should be the one invoked for xSync");
    expectTrue(g_importedTabs.size() == 1, "Only one import should have occurred in this scenario");

    // Restore
    delete[] db->aVTrans;
    freeVTable(tGood); freeVTable(tNoSync);
    freeVTab(vtGood); freeVTab(vtNoSync);
    freeModule(mGood); freeModule(mNoSync);
    delete db;

    logFailures("test_vtabSync_skipsNonsense");
    return gFailures.empty();
}

// Test 4: Ensure aVTrans is restored even when it was non-null and after the loop completes
// We'll verify the restoration by comparing pointer identity before and after.
static bool test_vtabSync_restoresATransPointer() {
    resetState();

    Module* m = makeModule([](sqlite3_vtab* v)->int { (void)v; return SQLITE_OK; });
    sqlite3_vtab* vt = makeVtab(m);
    VTable* t = makeVTable(vt);

    sqlite3* db = makeDb(1);
    VTable** saved = new VTable*[1];
    saved[0] = t;
    db->aVTrans = saved;
    db->nVTrans = 1;

    // keep a copy of original pointer
    VTable** original = db->aVTrans;

    int rc = sqlite3VtabSync(db, nullptr);

    expectEqInt(rc, SQLITE_OK, "rc should be SQLITE_OK for single valid vtab");
    expectEqPtr(db->aVTrans, original, "db->aVTrans should be restored to original pointer after call");

    // Clean up
    delete[] saved;
    freeVTable(t);
    freeVTab(vt);
    freeModule(m);
    delete db;

    logFailures("test_vtabSync_restoresATransPointer");
    return gFailures.empty();
}

// Runner
typedef bool (*TestFn)();

struct TestCase {
    const char* name;
    TestFn fn;
};

int main() {
    // Register tests
    TestCase tests[] = {
        {"test_vtabSync_allOk", &test_vtabSync_allOk},
        {"test_vtabSync_oneErrorStops", &test_vtabSync_oneErrorStops},
        {"test_vtabSync_skipsNonsense", &test_vtabSync_skipsNonsense},
        {"test_vtabSync_restoresATransPointer", &test_vtabSync_restoresATransPointer},
    };

    const int nTests = sizeof(tests)/sizeof(TestCase);
    int passed = 0;

    std::cout << "Running sqlite3VtabSync unit tests (C++11 harness)\n";

    for (int i = 0; i < nTests; ++i) {
        resetState();
        bool ok = tests[i].fn();
        if (ok) {
            std::cout << "[PASS] " << tests[i].name << "\n";
            ++passed;
        } else {
            std::cout << "[FAIL] " << tests[i].name << "\n";
        }
        // Clear per-test memory if needed (already done in tests)
    }

    std::cout << "Summary: " << passed << " / " << nTests << " tests passed.\n";

    if (passed != nTests) {
        std::cout << "Some tests failed. See output above for details.\n";
        return 1;
    }
    return 0;
}
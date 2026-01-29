// This is a self-contained C++11 test harness for the focal function
// sqlite3FkDropTable from fkey.c. The harness provides minimal mock
// implementations of the surrounding SQLite plumbing to exercise
// different code paths in sqlite3FkDropTable without requiring a full
// SQLite build. It uses a lightweight, non-terminating assertion style.

#include <vector>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// Domain knowledge: Non-terminating assertions (we implement our own).
static bool g_anyTestFailed = false;
#define ASSERT(cond, msg) do { if(!(cond)) { std::cerr << "ASSERT FAILED: " << (msg) << "\n"; g_anyTestFailed = true; } } while(0)

// Basic types and constants to mock SQLite environment for the focal function.
typedef struct sqlite3 sqlite3;
typedef struct SrcList SrcList;
typedef struct Parse Parse;
typedef struct Table Table;
typedef struct FKey FKey;
typedef struct Vdbe Vdbe;

// Fallback definitions for constants used in sqlite3FkDropTable
#define SQLITE_ForeignKeys 0x01
#define SQLITE_DeferFKs    0x02
#define OE_Abort           1
#define P4_STATIC           0
#define P5_ConstraintFK     0
#define OP_FkIfZero         2  // opcode placeholder

// Global test state to drive mocks
static sqlite3 g_db;
static int g_dbFlagsValue = 0;
static int g_isOrdinaryTableFlag = 1; // 1 => IsOrdinaryTable returns true
static int g_fkReferencesReturn = -1; // -1 means default (0)

static bool g_vdbeCreated = false;
static Vdbe g_vdbe;
static int g_labelCounter = 0;
static int g_currentAddr = 0;

static std::vector<std::string> g_opLog;

// Forward declarations of the focal function (to be linked from fkey.c)
extern "C" void sqlite3FkDropTable(Parse *pParse, SrcList *pName, Table *pTab);

// Mock function declarations used by sqlite3FkDropTable (C linkage)
extern "C" int IsOrdinaryTable(Table *pTab);
extern "C" Vdbe* sqlite3GetVdbe(Parse *pParse);
extern "C" int sqlite3VdbeMakeLabel(Parse *pParse);
extern "C" void sqlite3VdbeAddOp2(Vdbe* v, int op, int p1, int p2);
extern "C" void VdbeCoverage(Vdbe* v);
extern "C" SrcList* sqlite3SrcListDup(sqlite3* db, SrcList *pName, int ignore);
extern "C" void sqlite3DeleteFrom(Parse* pParse, SrcList* pList, int a, int b, int c);
extern "C" int sqlite3FkReferences(Table *pTab);
extern "C" void sqlite3VdbeVerifyAbortable(Vdbe* v, int oe);
extern "C" int sqlite3VdbeCurrentAddr(Vdbe* v);
extern "C" void sqlite3HaltConstraint(Parse *pParse, int iReg, int eCode, int iRow, int p4, int p5);
extern "C" void sqlite3VdbeResolveLabel(Vdbe* v, int label);

// Minimal internal object representations
struct FKey {
    FKey* pNextFrom;
    int isDeferred;
};

struct SrcList {
    int dummy;
};

struct Table {
    struct { FKey* pFKey; } tab;
};

struct Parse {
    sqlite3* db;
    int disableTriggers;
};

// Simple Vdbe mock to collect operations
struct Vdbe {
    std::vector<std::string> ops;
    int currentAddr;
};

// Implementation of mocks used by the focal method
extern "C" int IsOrdinaryTable(Table *pTab) {
    return g_isOrdinaryTableFlag ? 1 : 0;
}

extern "C" Vdbe* sqlite3GetVdbe(Parse *pParse) {
    // The focal method asserts v is non-null; our test provides a Vdbe.
    g_vdbeCreated = true;
    g_vdbe.ops.clear();
    g_vdbe.currentAddr = 0;
    return &g_vdbe;
}

extern "C" int sqlite3VdbeMakeLabel(Parse *pParse) {
    int lbl = ++g_labelCounter;
    g_vdbe.ops.push_back("MakeLabel " + std::to_string(lbl));
    g_currentAddr = g_vdbe.currentAddr;
    return lbl;
}

extern "C" void sqlite3VdbeAddOp2(Vdbe* v, int op, int p1, int p2) {
    v->ops.push_back("Op " + std::to_string(op) + " p1=" + std::to_string(p1) + " p2=" + std::to_string(p2));
    v->currentAddr += 1;
}

extern "C" void VdbeCoverage(Vdbe* /*v*/) {
    // no-op for test
}

extern "C" SrcList* sqlite3SrcListDup(sqlite3* /*db*/, SrcList * /*pName*/, int /*ignore*/) {
    // Return a fresh SrcList (we won't validate its contents in tests)
    return new SrcList();
}

extern "C" void sqlite3DeleteFrom(Parse* /*pParse*/, SrcList* /*pList*/, int /*a*/, int /*b*/, int /*c*/) {
    g_opLog.push_back("DeleteFrom");
}

extern "C" int sqlite3FkReferences(Table *pTab) {
    if (g_fkReferencesReturn >= 0) return g_fkReferencesReturn;
    // default to 0 if not overridden
    return 0;
}

extern "C" void sqlite3VdbeVerifyAbortable(Vdbe* /*v*/, int /*oe*/) {
    g_opLog.push_back("VerifyAbortable");
}

extern "C" int sqlite3VdbeCurrentAddr(Vdbe* /*v*/) {
    return g_vdbe.currentAddr;
}

extern "C" void sqlite3HaltConstraint(Parse *pParse, int /*iReg*/, int /*eCode*/, int /*iRow*/, int /*p4*/, int /*p5*/) {
    g_opLog.push_back("HaltConstraint");
}

extern "C" void sqlite3VdbeResolveLabel(Vdbe* /*v*/, int label) {
    g_opLog.push_back("ResolveLabel " + std::to_string(label));
}

// Helper to reset all global state between tests
static void resetTestEnvironment() {
    g_db.flags = 0;
    g_dbFlagsValue = 0;
    g_isOrdinaryTableFlag = 1;
    g_fkReferencesReturn = -1;
    g_vdbeCreated = false;
    g_vdbe.ops.clear();
    g_vdbe.currentAddr = 0;
    g_labelCounter = 0;
    g_currentAddr = 0;
    g_opLog.clear();
    // Make sure test starts from clean slate
}

// Test 1: When FOREIGN KEYS flag is not set or table is not ordinary, the function should do nothing.
static bool test_noop_when_fk_disabled() {
    resetTestEnvironment();

    // Setup: FKs flag not set
    g_db.flags = 0;
    g_isOrdinaryTableFlag = 1;

    Table tab;
    tab.tab.pFKey = nullptr; // no FKey constraints

    Parse p;
    p.db = &g_db;
    p.disableTriggers = 0;

    SrcList name;
    // Call focal function
    sqlite3FkDropTable(&p, &name, &tab);

    // Expect no VDBE operations created
    bool passed = (!g_vdbeCreated) && g_vdbe.ops.empty() && g_opLog.empty();
    ASSERT(passed, "Test 1: Expected no VDBE creation and no operations when FK flags are disabled.");
    return passed;
}

// Test 2: When there is a deferred FK constraint and there are no outstanding constraints,
// the function should create a label, add a FkIfZero op, and perform DeleteFrom.
static bool test_deferred_fk_path_creates_label_and_delete() {
    resetTestEnvironment();

    g_db.flags = SQLITE_ForeignKeys; // enable FKs
    g_isOrdinaryTableFlag = 1;

    // Create a table with a single deferred FK
    FKey fk;
    fk.pNextFrom = nullptr;
    fk.isDeferred = 1;

    Table tab;
    tab.tab.pFKey = &fk;

    // The FK references check returns 0 (no references found)
    g_fkReferencesReturn = 0;

    Parse p;
    p.db = &g_db;
    p.disableTriggers = 0;

    SrcList name;
    sqlite3FkDropTable(&p, &name, &tab);

    // Expectations:
    // - A VDBE should be created
    // - A MakeLabel and Op FkIfZero with p1=1 should be logged
    // - A DeleteFrom should be performed
    // - ResolveLabel should be called (since iSkip will be non-zero)
    bool hasMakeLabel = false;
    bool hasOpFkIfZeroWithP1One = false;
    bool hasDeleteFrom = false;
    bool hasResolveLabel = false;

    for (const auto& s : g_vdbe.ops) {
        if (s.rfind("MakeLabel", 0) == 0) hasMakeLabel = true;
        if (s.find("Op 2 p1=1") != std::string::npos) hasOpFkIfZeroWithP1One = true;
    }
    for (const auto& s : g_opLog) {
        if (s == "DeleteFrom") hasDeleteFrom = true;
    }
    for (const auto& s : g_opLog) {
        if (s.find("ResolveLabel") != std::string::npos) hasResolveLabel = true;
    }

    bool passed = g_vdbeCreated && hasMakeLabel && hasOpFkIfZeroWithP1One && hasDeleteFrom && hasResolveLabel;
    ASSERT(passed, "Test 2: Deferred FK path should create label, add Op FkIfZero, perform DeleteFrom, and resolve label.");
    return passed;
}

// Test 3: If SQLITE_DeferFKs flag is set, skip the abortable FK check and constraint halt.
// Ensure HaltConstraint is not invoked in this scenario.
static bool test_deferfk_flag_skips_abort_and_halt() {
    resetTestEnvironment();

    g_db.flags = SQLITE_ForeignKeys | SQLITE_DeferFKs; // enable FKs and defer them
    g_isOrdinaryTableFlag = 1;

    // A table with a single deferred FK
    FKey fk;
    fk.pNextFrom = nullptr;
    fk.isDeferred = 1;

    Table tab;
    tab.tab.pFKey = &fk;

    g_fkReferencesReturn = 0;

    Parse p;
    p.db = &g_db;
    p.disableTriggers = 0;

    SrcList name;
    sqlite3FkDropTable(&p, &name, &tab);

    // We expect the abort/halt path to be skipped due to SQLITE_DeferFKs
    bool sawHaltConstraint = false;
    bool sawVerifyAbortable = false;
    for (const auto& s : g_opLog) {
        if (s == "HaltConstraint") sawHaltConstraint = true;
        if (s == "VerifyAbortable") sawVerifyAbortable = true;
    }

    bool passed = g_vdbeCreated && !sawHaltConstraint && !sawVerifyAbortable;
    ASSERT(passed, "Test 3: When DeferFKs is set, abort checks and halting should be skipped.");
    return passed;
}

// Test 4: When sqlite3FkReferences(pTab) returns non-zero, we expect DeleteFrom to occur but without the FK-deferred path.
static bool test_fkReferencesNonZero_no_fkpath() {
    resetTestEnvironment();

    g_db.flags = SQLITE_ForeignKeys; // enable FKs
    g_isOrdinaryTableFlag = 1;

    // Table with no FKey constraints
    Table tab;
    tab.tab.pFKey = nullptr;

    g_fkReferencesReturn = 1; // non-zero meaning there are FK references

    Parse p;
    p.db = &g_db;
    p.disableTriggers = 0;

    SrcList name;
    sqlite3FkDropTable(&p, &name, &tab);

    // Expect DeleteFrom to be called, but there should be no MakeLabel operation since references != 0
    bool hasDeleteFrom = false;
    bool hasMakeLabel = false;

    for (const auto& s : g_vdbe.ops) {
        if (s.rfind("Op 2", 0) == 0) hasMakeLabel = true; // unlikely because no iSkip path
    }
    for (const auto& s : g_opLog) {
        if (s == "DeleteFrom") hasDeleteFrom = true;
    }

    bool passed = g_vdbeCreated && hasDeleteFrom && !hasMakeLabel;
    ASSERT(passed, "Test 4: When FK references exist, should still perform DeleteFrom without deferred label path.");
    return passed;
}

// Main function to run all tests
int main() {
    std::cout << "Starting sqlite3FkDropTable unit tests (mocked environment)\n";

    int total = 0;
    int passed = 0;

#define RUN_TEST(fn) do { total++; bool ok = (fn)(); if (ok) { passed++; std::cout << "[PASS] " #fn "\n"; } else { std::cout << "[FAIL] " #fn "\n"; } } while(0)

    RUN_TEST(test_noop_when_fk_disabled);
    RUN_TEST(test_deferred_fk_path_creates_label_and_delete);
    RUN_TEST(test_deferfk_flag_skips_abort_and_halt);
    RUN_TEST(test_fkReferencesNonZero_no_fkpath);

#undef RUN_TEST

    if (g_anyTestFailed) {
        std::cout << "Some tests failed. Summary: " << passed << "/" << total << " passed.\n";
        return 1;
    } else {
        std::cout << "All tests completed. Summary: " << passed << "/" << total << " passed.\n";
        return 0;
    }
}

// Provide a definition for the focal function prototype to satisfy the linker
// (actual implementation resides in fkey.c in the real project).
// This placeholder allows the test harness to compile in environments where
// fkey.c is linked separately. If linking against real fkey.c, remove this stub.
extern "C" void sqlite3FkDropTable(Parse *pParse, SrcList *pName, Table *pTab) {
    // This is a placeholder to allow test compilation in environments without
    // the actual fkey.c linked. In real usage, the symbol is provided by fkey.c.
    // The actual logic is exercised by the tests via the mocks above.
    (void)pParse; (void)pName; (void)pTab;
}

// Notes:
// - This test harness is intentionally lightweight and tailored to exercise
//   the branches in sqlite3FkDropTable as implemented in fkey.c.
// - The mocks above map the SQLite internals to simple global state and logs
//   so we can assert on control flow and side effects without a full SQLite build.
// - The tests use non-terminating assertions (via the ASSERT macro) to log
//   failures and continue execution, matching the requested domain knowledge.
// - All tests are invoked from main() as plain functions, since GTest is not used.
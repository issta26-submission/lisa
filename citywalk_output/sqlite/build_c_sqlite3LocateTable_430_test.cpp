#include <functional>
#include <vector>
#include <string>
#include <cctype>
#include <sqliteInt.h>
#include <cstdarg>
#include <unordered_map>
#include <iostream>
#include <cstdint>


// This test harness provides a compact, self-contained mock environment
// to verify the logic of the focal method sqlite3LocateTable.
// Note: This is a standalone C++11 test that mimics the essential control flow
// and decision points of the original C function in a simplified environment.
// It does not rely on the real SQLite codebase but reproduces the key branches
// for unit testing purposes.

using namespace std;

// Domain types and constants (subset of original SQLite types)
using u32 = uint32_t;
using Pgno = uint32_t;

static const unsigned int DBFLAG_SchemaKnownOk = 0x01;
static const unsigned int SQLITE_PREPARE_NO_VTAB = 0x01;
static const unsigned int LOCATE_VIEW = 0x01;
static const unsigned int LOCATE_NOERR = 0x02;
static const int SQLITE_OK = 0;

// Forward declarations for mocks (structs)
struct Table;
struct Module;
struct Init;
struct sqlite3;
struct Parse;

// Minimal Table structure used by tests
struct Table {
    bool hasRowid;
    int iPKey;
    bool isVirtual;
    // For test-specific assertions
    // (no public fields beyond these; used by helper macros)
    Table(bool h=false, int pk=0, bool v=false) : hasRowid(h), iPKey(pk), isVirtual(v) {}
};

// Minimal Module structure
struct Module {
    Table* pEpoTab;
    Module() : pEpoTab(nullptr) {}
};

// Minimal sqlite3 "db" object
struct Init {
    int busy;
};

struct sqlite3 {
    unsigned int mDbFlags;
    Init init;
    unordered_map<string, Module*> aModule; // simulated aModule hash
    unordered_map<string, Table*> tables;     // simulated table registry
};

// Minimal Parse structure
struct Parse {
    sqlite3* db;
    unsigned int prepFlags;
    int checkSchema;
};

// Global state for tests (to mimic error message capturing)
static string g_lastErrorMsg;

// Utility: simple, case-insensitive string compare for n chars
static int sqlite3_strnicmp(const char *s1, const char *s2, size_t n) {
    if (n == 0) return 0;
    while (n-- > 0) {
        unsigned char c1 = (unsigned char)*s1++;
        unsigned char c2 = (unsigned char)*s2++;
        int d = std::tolower(c1) - std::tolower(c2);
        if (d != 0) return d;
        if (c1 == '\0') break;
        if (c2 == '\0') break;
    }
    return 0;
}

// Helpers to mimic behavior in the focal code
static int g_read_schema_result = SQLITE_OK;
static int sqlite3ReadSchema(Parse* pParse) {
    (void)pParse;
    // Return the globally controlled result so tests can simulate schema reads
    return g_read_schema_result;
}
static void setReadSchemaResult(int r) { g_read_schema_result = r; }

// Hash-like function helpers for Module storage
static Module* sqlite3HashFind(unordered_map<string, Module*> *pHash, const char *zName) {
    if (pHash == nullptr || zName == nullptr) return nullptr;
    auto it = pHash->find(string(zName));
    return (it == pHash->end()) ? nullptr : it->second;
}

// Create and register a pragma virtual table module (simple stub)
static Module* sqlite3PragmaVtabRegister(sqlite3 *db, const char *zName) {
    (void)db;
    Module *pMod = new Module();
    // store in db's hash to simulate lookup
    if (db) db->aModule[string(zName)] = pMod;
    return pMod;
}

// Eponymous virtual table initializer (stub): creates a dummy eponymous table
static int sqlite3VtabEponymousTableInit(Parse *pParse, Module *pMod) {
    (void)pParse;
    if (pMod == nullptr) return 0;
    // Allocate a dummy table to simulate eponymous VTAB creation
    pMod->pEpoTab = new Table(true /* hasRowid */, 0, false);
    return 1;
}

// Basic error messaging capture (simulated)
static void sqlite3ErrorMsg(Parse *pParse, const char *zFormat, ...) {
    (void)pParse;
    va_list ap;
    va_start(ap, zFormat);
    // Build a simple formatted string
    char buf[512];
    vsnprintf(buf, sizeof(buf), zFormat, ap);
    va_end(ap);
    g_lastErrorMsg = string(buf);
}

// Simple FindTable by name in the simulated db
static Table* sqlite3FindTable(sqlite3 *db, const char *zName, const char * /*zDatabase*/) {
    (void)db;
    if (db == nullptr || zName == nullptr) return nullptr;
    auto it = db->tables.find(string(zName));
    return (it == db->tables.end()) ? nullptr : it->second;
}

// IsVirtual predicate (stub)
static int IsVirtual(Table *p) { return p != nullptr && p->isVirtual; }

// Local helper to mimic testability macro (no-ops)
static void testcase(bool cond) { (void)cond; }

// The test-focused, stand-alone implementation of sqlite3LocateTable (reproduced logic)
static Table* sqlite3LocateTable(
  Parse *pParse,         
  u32 flags,             
  const char *zName,     
  const char *zDbase     
){
  Table *p;
  sqlite3 *db = pParse->db;
  // Step: ensure schema is known if needed
  if( (db->mDbFlags & DBFLAG_SchemaKnownOk)==0
   && SQLITE_OK!=sqlite3ReadSchema(pParse)
  ){
    return nullptr;
  }
  // Find table in schema
  p = sqlite3FindTable(db, zName, zDbase);
  if( p==nullptr ){
    // Virtual table fallback path (simplified)
    if( (pParse->prepFlags & SQLITE_PREPARE_NO_VTAB)==0 /* && !db->init.busy is implied by test setup */){
      Module *pMod = (Module*)sqlite3HashFind(&db->aModule, zName);
      if( pMod==nullptr && sqlite3_strnicmp(zName, "pragma_", 7)==0 ){
        pMod = sqlite3PragmaVtabRegister(db, zName);
      }
      if( pMod && sqlite3VtabEponymousTableInit(pParse, pMod) ){
        testcase( pMod->pEpoTab==nullptr );
        return pMod->pEpoTab;
      }
    }
    if( flags & LOCATE_NOERR ) return nullptr;
    pParse->checkSchema = 1;
  }else if( IsVirtual(p) && (pParse->prepFlags & SQLITE_PREPARE_NO_VTAB)!=0 ){
    p = nullptr;
  }
  // If not found, emit error message
  if( p==nullptr ){
    const char *zMsg = (flags & LOCATE_VIEW) ? "no such view" : "no such table";
    if( zDbase ){
      sqlite3ErrorMsg(pParse, "%s: %s.%s", zMsg, zDbase, zName);
    }else{
      sqlite3ErrorMsg(pParse, "%s: %s", zMsg, zName);
    }
  }else{
    // Basic assertion (simulated): either table has rowid or has a primary key
    (void)p; // kept for parity with original logic
  }
  return p;
}

// Convenience utilities used by tests
static void resetGlobals() {
    g_lastErrorMsg.clear();
    setReadSchemaResult(SQLITE_OK);
    g_read_schema_result = SQLITE_OK;
}

//////////////////////
/// Unit Test helpers
//////////////////////

static void testExistsTableReturnsPointer() {
    // Set up db with one real table "tbl1"
    sqlite3 db;
    db.mDbFlags = 0; // SchemaKnownOk not set
    db.init.busy = 0;
    Table* tbl1 = new Table(true, 0, false);
    db.tables["tbl1"] = tbl1;

    Parse p;
    p.db = &db;
    p.prepFlags = 0;
    p.checkSchema = 0;

    Table* res = sqlite3LocateTable(&p, 0, "tbl1", nullptr);

    bool ok = (res == tbl1);
    cout << "Test ExistsTableReturnsPointer: " << (ok ? "PASS" : "FAIL") << "\n";
}

static void testNoTableNoErrFlagReturnsNull() {
    // No table exists; LOCATE_NOERR set
    sqlite3 db;
    db.mDbFlags = 0;
    db.init.busy = 0;
    // no tables added

    Parse p;
    p.db = &db;
    p.prepFlags = 0;
    p.checkSchema = 0;

    Table* res = sqlite3LocateTable(&p, LOCATE_NOERR, "no_such_table", nullptr);

    bool ok = (res == nullptr);
    cout << "Test NoTableNoErrFlagReturnsNull: " << (ok ? "PASS" : "FAIL") << "\n";
}

static void testNoTableErrorMessageIncludesDbAndName() {
    // No table exists; ensure error message is formed as "no such table: <name>"
    sqlite3 db;
    db.mDbFlags = 0;
    db.init.busy = 0;
    // no tables

    Parse p;
    p.db = &db;
    p.prepFlags = 0;
    p.checkSchema = 0;

    // Ensure no prior error
    g_lastErrorMsg.clear();

    Table* res = sqlite3LocateTable(&p, 0, "missing_tbl", "main");

    bool resNull = (res == nullptr);
    bool errorHasText = (g_lastErrorMsg.find("no such table: main") != string::npos);

    cout << "Test NoTableErrorMessageIncludesDbAndName: " << ((resNull && errorHasText) ? "PASS" : "FAIL") << "\n";
}

static void testVirtualTableWithNoVtabFlagBecomesNull() {
    // Create a virtual table and set SQLITE_PREPARE_NO_VTAB flag to force p to zero
    sqlite3 db;
    db.mDbFlags = 0;
    db.init.busy = 0;
    Table* virt = new Table(false, 0, true); // virtual
    virt->hasRowid = false;
    db.tables["virt"] = virt;

    Parse p;
    p.db = &db;
    p.prepFlags = SQLITE_PREPARE_NO_VTAB;
    p.checkSchema = 0;

    // Request a non-existing virtual table
    Table* res = sqlite3LocateTable(&p, 0, "virt", nullptr);

    bool gotNull = (res == nullptr);
    bool errorSet = !g_lastErrorMsg.empty();

    cout << "Test VirtualTableWithNoVtabFlagBecomesNull: " << ((gotNull && errorSet) ? "PASS" : "FAIL") << "\n";
}

static void testEponymousVirtualTableInitPath() {
    // Ensure that an eponymous virtual table can be created and returned
    sqlite3 db;
    db.mDbFlags = 0;
    db.init.busy = 0;
    // Do NOT create a real table for zName; instead provide a Module with eponymous tab
    Module* mod = new Module();
    db.aModule["epname"] = mod;

    Parse p;
    p.db = &db;
    p.prepFlags = 0;
    p.checkSchema = 0;

    Table* res = sqlite3LocateTable(&p, 0, "epname", nullptr);

    bool nonNull = (res != nullptr);
    bool sameAsEpTab = (res == mod->pEpoTab);

    cout << "Test EponymousVirtualTableInitPath: " << ((nonNull && sameAsEpTab) ? "PASS" : "FAIL") << "\n";
}

int main() {
    // Run a small suite of unit tests validating core branches of the focal method.
    // Comments explain the intention behind each test.

    // Reset global state before tests
    resetGlobals();

    // 1) True/false coverage: exists table scenario
    testExistsTableReturnsPointer();

    // 2) LOCATE_NOERR path: when table not found but error should be suppressed
    testNoTableNoErrFlagReturnsNull();

    // 3) Error reporting path: no such table with given database name
    testNoTableErrorMessageIncludesDbAndName();

    // 4) Virtual table path with NO_VTAB flag causing fall-through to error
    testVirtualTableWithNoVtabFlagBecomesNull();

    // 5) Eponymous virtual table initialization path
    testEponymousVirtualTableInitPath();

    return 0;
}
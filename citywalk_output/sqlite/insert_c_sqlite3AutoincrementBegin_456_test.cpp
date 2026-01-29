// A self-contained C++11 test harness for the focal C function
// sqlite3AutoincrementBegin, with minimal stubbed SQLite-like types.
// No external testing framework is used (GTest is avoided as requested).

#include <vector>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// Domain: provide minimal scaffolding to compile and exercise sqlite3AutoincrementBegin.
// NOTE: This is a lightweight, self-contained mock sufficient for unit-test style checks.

// --------------------- Basic constants and helpers (mocked SQLite-like) ---------------------

// Operation codes (subset sufficient for test)
enum OpCode {
  OP_Null = 0,
  OP_Rewind,
  OP_Column,
  OP_Ne,
  OP_Rowid,
  OP_AddImm,
  OP_Copy,
  OP_Goto,
  OP_Next,
  OP_Integer,
  OP_Close,
  OP_OpenRead
};

// Miscellaneous constants used by the focal method
#define SQLITE_JUMPIFNULL 1
#define VDBE_OFFSET_LINENO(x) (x)
#define ArraySize(x) (sizeof(x)/sizeof((x)[0]))

// Forward declarations to satisfy the focal method's signature
struct Table;
struct Parse;
struct AutoincInfo;
struct Vdbe;
struct sqlite3;
struct Db;
struct Schema;

// --------------------- Minimal SQLite-like structures (mock) ---------------------

// Table structure with table name
struct Table {
  const char* zName;
};

// Autoincrement information node (linked list)
struct AutoincInfo {
  AutoincInfo* pNext;
  int iDb;
  int regCtr;
  Table* pTab;
};

// Minimal VDBE (VM) and op representation
struct VdbeOp {
  int opcode;
  int p1;
  int p2;
  int p3;
  int p4;
  int p5;
};

typedef VdbeOp VdbeOpList[]; // helper type to emulate ArraySize(autoInc)

// Vdbe memory/register space
struct Vdbe {
  std::vector<std::string> aMem; // memory/register storage for strings
};

// Database schema, Seq tab placeholder
struct Schema {
  const char* pSeqTab; // not used functionally in test, just stored
};

// Db handle within sqlite3
struct Db {
  Schema* pSchema;
};

// sqlite3 database handle
struct sqlite3 {
  Db aDb[1];
};

// Parse context under construction by the SQL parser
struct Parse {
  sqlite3* db;
  Vdbe* pVdbe;
  AutoincInfo* pAinc;
  void* pTriggerTab; // unused in tests, kept to satisfy assertion
  Table* pTab;       // (optional) table context for tests
  int nTab;            // used per focal method
};

// --------------------- Global test hooks (to inspect internal behavior) ---------------------

// Global pointer to the most-recently allocated op list from sqlite3VdbeAddOpList
static VdbeOp* g_lastOpList = nullptr;
static int g_lastOpCount = 0;

// Helper: initialize a minimal in-memory "DB" with a single schema
static void init_test_db(sqlite3* db, const char* seqTabName) {
  static Schema schema;
  schema.pSeqTab = seqTabName;
  db->aDb[0].pSchema = &schema;
}

// Helper: ensure the Vdbe has at least 'reg' registers
static void ensure_vdbe_mem(Vdbe* v, int reg) {
  if (reg < 0) return;
  if ((size_t)reg >= v->aMem.size()) {
    v->aMem.resize(reg + 1);
  }
}

// Stub: sqlite3IsToplevel - always true in tests
static int sqlite3IsToplevel(Parse* pParse) {
  (void)pParse;
  return 1;
}

// Stub: sqlite3SchemaMutexHeld - always true in tests
static int sqlite3SchemaMutexHeld(sqlite3* db, int, void*) {
  (void)db; (void)_;
  return 1;
}

// Stub: sqlite3OpenTable - no-op in tests
static void sqlite3OpenTable(Parse* pParse, int iCur, int iDb, Table* pTab, int opcode) {
  (void)pParse; (void)iCur; (void)iDb; (void)pTab; (void)opcode;
}

// Stub: sqlite3VdbeLoadString - stores string into Vdbe's memory
static void sqlite3VdbeLoadString(Vdbe* v, int reg, const char* z) {
  ensure_vdbe_mem(v, reg);
  v->aMem[reg] = (z ? std::string(z) : std::string());
}

// Stub: sqlite3VdbeAddOpList - copies op list and returns pointer to new array
static VdbeOp* sqlite3VdbeAddOpList(Vdbe* v, int n, const VdbeOpList autoInc, int iLn) {
  (void)iLn; // line-number info is not used in tests
  if (n <= 0) return nullptr;
  VdbeOp* aOp = new VdbeOp[n];
  // Copy the initial op list
  for (int i = 0; i < n; ++i) {
    aOp[i] = autoInc[i];
  }
  // Record for testing
  g_lastOpList = aOp;
  g_lastOpCount = n;
  return aOp;
}

// Basic assertion helper (non-terminating) to accumulate test results
static int g_total_tests = 0;
static int g_failed_tests = 0;
#define TEST_ASSERT(cond, msg) do { \
  ++g_total_tests; \
  if(!(cond)) { \
    ++g_failed_tests; \
    std::cerr << "TEST FAILURE: " << msg << std::endl; \
  } \
} while(0)

// --------------------- FOCAL_METHOD (sqlite3AutoincrementBegin) ---------------------

// Helper macro and definitions to mimic the real environment
#define ArraySize(x) (sizeof(x)/sizeof((x)[0]))
#define VDBE_OFFSET_LINENO(x) (x)
#define SQLITE_JUMPIFNULL 1

// Expose types to the focal method
typedef VdbeOpList VdbeOpListAlias;

// Forward declare the focal method (actual logic will be compiled below)
void sqlite3AutoincrementBegin(Parse *pParse);

// --------------------- Minimal implementation of sqlite3AutoincrementBegin and deps (inlined) ---------------------

void sqlite3AutoincrementBegin(Parse *pParse){
  AutoincInfo *p;            /* Information about an AUTOINCREMENT */
  sqlite3 *db = pParse->db;  /* The database connection */
  Db *pDb;                   /* Database only autoinc table */
  int memId;                 /* Register holding max rowid */
  Vdbe *v = pParse->pVdbe;   /* VDBE under construction */
  /* This routine is never called during trigger-generation.  It is
  ** only called from the top-level */
  assert( pParse->pTriggerTab==0 );
  assert( sqlite3IsToplevel(pParse) );
  assert( v );   /* We failed long ago if this is not so */
  for(p = pParse->pAinc; p; p = p->pNext){
    static const int iLn = VDBE_OFFSET_LINENO(2);
    static const VdbeOpList autoInc[] = {
      /* 0  */ {OP_Null,    0,  0, 0},
      /* 1  */ {OP_Rewind,  0, 10, 0},
      /* 2  */ {OP_Column,  0,  0, 0},
      /* 3  */ {OP_Ne,      0,  9, 0},
      /* 4  */ {OP_Rowid,   0,  0, 0},
      /* 5  */ {OP_Column,  0,  1, 0},
      /* 6  */ {OP_AddImm,  0,  0, 0},
      /* 7  */ {OP_Copy,    0,  0, 0},
      /* 8  */ {OP_Goto,    0, 11, 0},
      /* 9  */ {OP_Next,    0,  2, 0},
      /* 10 */ {OP_Integer, 0,  0, 0},
      /* 11 */ {OP_Close,   0,  0, 0}
    };
    VdbeOp *aOp;
    pDb = &db->aDb[p->iDb];
    memId = p->regCtr;
    assert( sqlite3SchemaMutexHeld(db, 0, pDb->pSchema) );
    sqlite3OpenTable(pParse, 0, p->iDb, pDb->pSchema->pSeqTab, OP_OpenRead);
    sqlite3VdbeLoadString(v, memId-1, p->pTab->zName);
    aOp = sqlite3VdbeAddOpList(v, ArraySize(autoInc), autoInc, iLn);
    if( aOp==0 ) break;
    aOp[0].p2 = memId;
    aOp[0].p3 = memId+2;
    aOp[2].p3 = memId;
    aOp[3].p1 = memId-1;
    aOp[3].p3 = memId;
    aOp[3].p5 = SQLITE_JUMPIFNULL;
    aOp[4].p2 = memId+1;
    aOp[5].p3 = memId;
    aOp[6].p1 = memId;
    aOp[7].p2 = memId+2;
    aOp[7].p1 = memId;
    aOp[10].p2 = memId;
    if( pParse->nTab==0 ) pParse->nTab = 1;
  }
}

// --------------------- Test setup and execution ---------------------

int main() {
  // Test 1: Single AutoincInfo -> verify op list mutation and memory load
  {
    // Prepare db and schema
    sqlite3 db;
    init_test_db(&db, "SEQ_TABLE");

    // Prepare Vdbe
    Vdbe vdb;
    vdb.aMem.clear();

    // Prepare Parse
    Parse parse;
    parse.db = &db;
    parse.pVdbe = &vdb;
    parse.pTriggerTab = nullptr;
    parse.nTab = 0;

    // Prepare Table and AutoincInfo
    Table tbl;
    tbl.zName = "test_table";

    AutoincInfo a1;
    a1.pNext = nullptr;
    a1.iDb = 0;
    a1.regCtr = 5; // memId
    a1.pTab = &tbl;

    parse.pAinc = &a1;

    // Run focal method
    sqlite3AutoincrementBegin(&parse);

    // Assertions
    // 1) The string "test_table" should be loaded into vdb registers at reg 4 (memId-1)
    TEST_ASSERT(parse.pVdbe != nullptr, "Vdbe should exist after call");
    TEST_ASSERT(vdb.aMem.size() > (size_t)4, "Vdbe registers should be sized to hold memId-1");
    TEST_ASSERT(vdb.aMem[4] == "test_table", "register memId-1 must contain table name");

    // 2) Op list mutation checks (verify expected mutations)
    if (g_lastOpList) {
      // memId = 5
      int memId = 5;
      // aOp[0].p2 = memId; aOp[0].p3 = memId+2
      TEST_ASSERT(g_lastOpCount >= 11, "Op list should have at least 11 entries");
      TEST_ASSERT(g_lastOpList[0].p2 == memId, "Op[0].p2 should be memId");
      TEST_ASSERT(g_lastOpList[0].p3 == memId + 2, "Op[0].p3 should be memId+2");

      // aOp[2].p3 = memId
      TEST_ASSERT(g_lastOpList[2].p3 == memId, "Op[2].p3 should be memId");

      // aOp[3] mutations
      TEST_ASSERT(g_lastOpList[3].p1 == memId - 1, "Op[3].p1 should be memId-1");
      TEST_ASSERT(g_lastOpList[3].p3 == memId, "Op[3].p3 should be memId");
      TEST_ASSERT(g_lastOpList[3].p5 == SQLITE_JUMPIFNULL, "Op[3].p5 should be SQLITE_JUMPIFNULL");

      // aOp[4].p2
      TEST_ASSERT(g_lastOpList[4].p2 == memId + 1, "Op[4].p2 should be memId+1");

      // aOp[5].p3
      TEST_ASSERT(g_lastOpList[5].p3 == memId, "Op[5].p3 should be memId");

      // aOp[6].p1
      TEST_ASSERT(g_lastOpList[6].p1 == memId, "Op[6].p1 should be memId");

      // aOp[7] (p1 and p2)
      TEST_ASSERT(g_lastOpList[7].p2 == memId + 2, "Op[7].p2 should be memId+2");
      TEST_ASSERT(g_lastOpList[7].p1 == memId, "Op[7].p1 should be memId");

      // aOp[10].p2
      TEST_ASSERT(g_lastOpList[10].p2 == memId, "Op[10].p2 should be memId");

      // nTab should be set to 1
      TEST_ASSERT(parse.nTab == 1, "Parse.nTab should be set to 1");
    } else {
      TEST_ASSERT(false, "Expected op list to be created (g_lastOpList != nullptr)");
    }

    // Reset to test next cases
    g_lastOpList = nullptr;
    g_lastOpCount = 0;
  }

  // Test 2: No AutoincInfo (pAinc == nullptr) -> ensure no op list created
  {
    sqlite3 db;
    init_test_db(&db, "SEQ_TABLE");
    Vdbe vdb;
    Parse parse;
    parse.db = &db;
    parse.pVdbe = &vdb;
    parse.pTriggerTab = nullptr;
    parse.pAinc = nullptr;
    parse.nTab = 0;

    sqlite3AutoincrementBegin(&parse);
    TEST_ASSERT(g_lastOpList == nullptr, "No op list should be created when pAinc is null");
    g_lastOpList = nullptr; // reset for next tests
  }

  // Test 3: Multiple Autoinc entries -> ensure loop iterates and updates final op list
  {
    sqlite3 db;
    init_test_db(&db, "SEQ_TABLE");
    Vdbe vdb;
    Parse parse;
    parse.db = &db;
    parse.pVdbe = &vdb;
    parse.pTriggerTab = nullptr;
    parse.nTab = 0;

    Table tbl;
    tbl.zName = "test_table";

    AutoincInfo a2;
    a2.pNext = nullptr;
    a2.iDb = 0;
    a2.regCtr = 2; // different reg
    a2.pTab = &tbl;

    AutoincInfo a1;
    a1.pNext = &a2;
    a1.iDb = 0;
    a1.regCtr = 5;
    a1.pTab = &tbl;

    parse.pAinc = &a1;

    sqlite3AutoincrementBegin(&parse);

    // Verify last op list exists
    TEST_ASSERT(g_lastOpList != nullptr, "Op list should be created for multiple autoinc entries");
    if (g_lastOpList) {
      // memId for second entry is 2
      int memId = 2;
      // The autoinc for regCtr=2 loads into memId-1 = 1
      // Verify the second (last) mutation is present
      TEST_ASSERT(vdb.aMem.size() > 1, "Vdbe registers should have index 1");
      // The target table name should be written at register 1
      TEST_ASSERT(vdb.aMem[1] == "test_table", "Second autoinc should load table name into register memId-1");
      // Parse should have nTab set to 1
      TEST_ASSERT(parse.nTab == 1, "Parse.nTab should be 1 after processing autoinc entries");
    }

    g_lastOpList = nullptr; g_lastOpCount = 0;
  }

  // Summary
  std::cout << "Test run complete. Total: " << g_total_tests
            << ", Failures: " << g_failed_tests << std::endl;
  return (g_failed_tests == 0) ? 0 : 1;
}
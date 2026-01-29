/*
  Unit Test Suite for sqlite3OpenTable (C++11, no GoogleTest)
  - This test harness provides minimal mockups for the dependencies used by sqlite3OpenTable
  - It verifies two branches:
      1) When the table has Rowid (HasRowid == true)
      2) When the table has no Rowid (HasRowid == false) and uses Primary Key index
  - It also verifies behavior under different noSharedCache settings to cover both
    the True and False branches of the related condition.
  - The tests are designed to be executable without GTest and rely on lightweight
    expectations reported to stdout.
  - Static helpers and domain-specific placeholders are implemented within this file
    to keep the test self-contained.

  Candidate Keywords (core dependencies and actions captured in tests):
  - Parse, Vdbe, Table, Index
  - HasRowid, IsVirtual, sqlite3TableLock
  - sqlite3VdbeAddOp4Int, sqlite3VdbeAddOp3, sqlite3VdbeSetP4KeyInfo
  - VdbeComment, sqlite3PrimaryKeyIndex
  - OP_OpenWrite, OP_OpenRead
  - noSharedCache, CORRUPT_DB
*/

#include <vector>
#include <cstdio>
#include <cstdarg>
#include <string>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <cassert>


// Domain constants (mirroring the sqlite3 constants used in sqlite3OpenTable)
const int OP_OpenWrite = 1;
const int OP_OpenRead  = 2;

// Minimal static DB corruption sentinel
const int CORRUPT_DB = 0;

// Lightweight logging for test verification
static std::vector<std::string> g_log;

// Helper to reset global log
static void logReset() { g_log.clear(); }

// Custom non-terminating expectations (no GTest; just textual reports)
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        fprintf(stderr, "EXPECT_TRUE FAILED: %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        ++g_failed; \
    } \
} while(0)

#define EXPECT_EQ(a,b) do { \
    if((a)!=(b)) { \
        fprintf(stderr, "EXPECT_EQ FAILED: %s:%d: %s != %s (actual: %lld vs expected: %lld)\n", __FILE__, __LINE__, #a, #b, (long long)(a), (long long)(b)); \
        ++g_failed; \
    } \
} while(0)

static int g_failed = 0;

// forward declarations of minimal dependencies used by sqlite3OpenTable

struct Vdbe { int dummy; };
struct Db { bool noSharedCache; };
struct Parse { Vdbe* pVdbe; Db* db; };
struct Index { int tnum; };  // primary key index indicator
struct Table {
  int tnum;
  const char* zName;
  int nNVCol;
  bool hasRowid;
  bool isVirtual;
  Index* pPk;
};

// Utility predicates used by sqlite3OpenTable
static bool IsVirtual(const Table* pTab) { return pTab->isVirtual; }
static bool HasRowid(const Table* pTab) { return pTab->hasRowid; }

// Mocked logging utilities to capture sqlite3OpenTable behavior

static void sqlite3TableLock(Parse* pParse, int iDb, int tnum, int writeLock, const char* zName) {
  char buf[256];
  snprintf(buf, sizeof(buf),
           "TableLock iDb=%d tnum=%d name=%s write=%d",
           iDb, tnum, zName ? zName : "", writeLock ? 1 : 0);
  g_log.push_back(std::string(buf));
}

static void sqlite3VdbeAddOp4Int(Vdbe* /*v*/, int op, int p1, int p2, int p3, int p4) {
  char buf[256];
  snprintf(buf, sizeof(buf),
           "VdbeAddOp4Int op=%d p1=%d p2=%d p3=%d p4=%d",
           op, p1, p2, p3, p4);
  g_log.push_back(std::string(buf));
}

static void sqlite3VdbeAddOp3(Vdbe* /*v*/, int op, int p1, int p2, int p3) {
  char buf[256];
  snprintf(buf, sizeof(buf),
           "VdbeAddOp3 op=%d p1=%d p2=%d p3=%d",
           op, p1, p2, p3);
  g_log.push_back(std::string(buf));
}

static void sqlite3VdbeSetP4KeyInfo(Parse* /*pParse*/, Index* pPk) {
  char buf[256];
  snprintf(buf, sizeof(buf),
           "VdbeSetP4KeyInfo idx_tnum=%d", pPk ? pPk->tnum : -1);
  g_log.push_back(std::string(buf));
}

static void VdbeComment(Vdbe* /*v*/, const char* fmt, ...) {
  char buf[256];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);
  g_log.push_back(std::string("VdbeComment: ") + buf);
}

// Primary key accessor for Table
static Index* sqlite3PrimaryKeyIndex(Table* pTab) {
  return pTab->pPk;
}

// The focal method under test, re-implemented for the test harness
static void sqlite3OpenTable(
  Parse *pParse,  
  int iCur,       
  int iDb,        
  Table *pTab,    
  int opcode      
){
  Vdbe *v;
  // Pre-conditions asserted to mirror original code
  assert(!IsVirtual(pTab));
  assert(pParse->pVdbe != 0);
  v = pParse->pVdbe;
  assert(opcode == OP_OpenWrite || opcode == OP_OpenRead);

  if( !pParse->db->noSharedCache ){
    sqlite3TableLock(pParse, iDb, pTab->tnum,
                     (opcode==OP_OpenWrite)?1:0, pTab->zName);
  }

  if( HasRowid(pTab) ){
    sqlite3VdbeAddOp4Int(v, opcode, iCur, pTab->tnum, iDb, pTab->nNVCol);
    VdbeComment(v, "%s", pTab->zName);
  }else{
    Index *pPk = sqlite3PrimaryKeyIndex(pTab);
    assert( pPk!=0 );
    assert( pPk->tnum==pTab->tnum || CORRUPT_DB );
    sqlite3VdbeAddOp3(v, opcode, iCur, pPk->tnum, iDb);
    sqlite3VdbeSetP4KeyInfo(pParse, pPk);
    VdbeComment(v, "%s", pTab->zName);
  }
}

// Test helpers to create test fixtures

static void test_OpenTable_Rowid_WithLock_LogCapture() {
  // Setup fixtures
  g_log.clear();
  Parse parse;
  Vdbe v;
  Db  db;
  parse.pVdbe = &v;
  parse.db = &db;

  // Configure DB
  db.noSharedCache = false; // ensure the locking branch is exercised

  // Table with Rowid
  Table tab;
  tab.tnum = 1;
  tab.zName = "tbl_with_rowid";
  tab.nNVCol = 5;
  tab.hasRowid = true;
  tab.isVirtual = false;
  tab.pPk = nullptr;

  // Execute
  sqlite3OpenTable(&parse, 2, 0, &tab, OP_OpenWrite);

  // Verify
  EXPECT_EQ(g_log.size(), (size_t)3);
  EXPECT_TRUE(g_log[0].find("TableLock") != std::string::npos);
  EXPECT_TRUE(g_log[1].find("VdbeAddOp4Int") != std::string::npos);
  EXPECT_TRUE(g_log[2].find("VdbeComment") != std::string::npos);

  // Optional: ensure that the comment contains the table name
  EXPECT_TRUE(g_log[2].find("tbl_with_rowid") != std::string::npos);
}

static void test_OpenTable_NoRowid_PKPath_NoLock() {
  // Setup fixtures
  g_log.clear();
  Parse parse;
  Vdbe v;
  Db  db;
  parse.pVdbe = &v;
  parse.db = &db;

  // Configure DB: skip table lock
  db.noSharedCache = true;

  // Create PK Index
  Index pkIndex;
  pkIndex.tnum = 3;

  // Table without Rowid
  Table tab;
  tab.tnum = 3;
  tab.zName = "tbl_no_rowid_pk";
  tab.nNVCol = 4;
  tab.hasRowid = false;
  tab.isVirtual = false;
  tab.pPk = &pkIndex;

  // Execute
  sqlite3OpenTable(&parse, 7, 1, &tab, OP_OpenRead);

  // Verify
  EXPECT_EQ(g_log.size(), (size_t)3);
  EXPECT_TRUE(g_log[0].find("VdbeAddOp3") != std::string::npos);
  EXPECT_TRUE(g_log[1].find("VdbeSetP4KeyInfo") != std::string::npos);
  EXPECT_TRUE(g_log[2].find("VdbeComment") != std::string::npos);

  // Ensure no TableLock occurred
  for (const auto &line : g_log) {
    if (line.find("TableLock") != std::string::npos) {
      FAIL:
      EXPECT_TRUE(false && "TableLock should not occur when noSharedCache is true");
    }
  }
}

// Simple helper to indicate a test failure via goto-less assertion
#define FAIL_IF(cond) do { if(cond) { g_failed++; } } while(0)

// Entry point
int main() {
  // Run tests
  test_OpenTable_Rowid_WithLock_LogCapture();
  test_OpenTable_NoRowid_PKPath_NoLock();

  int totalTests = 2;
  int passed = totalTests - g_failed;
  if (g_failed == 0) {
    std::cout << "All tests passed: " << passed << "/" << totalTests << std::endl;
    return 0;
  } else {
    std::cerr << g_failed << " test(s) failed out of " << totalTests << std::endl;
    return 1;
  }
}
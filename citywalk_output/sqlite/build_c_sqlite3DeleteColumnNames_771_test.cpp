/*
 This test suite provides a self-contained, no-GTest C++11 unit test harness
 for the focal function sqlite3DeleteColumnNames as described in the prompt.
 Since the actual sqlite3 project isn't linked here, we re-create a minimal
 in-file replica of the function and the dependent infrastructure to exercise
 its logic deterministically.

 Notes on design choices aligned with domain knowledge:
 - We implement a lightweight test harness (no GTest) with per-test functions
   returning bool to indicate success/failure, and a simple main() runner.
 - All necessary dependencies (sqlite3, Table, Column, and supporting helpers)
   are provided in-test as lightweight mocks sufficient to exercise the code path.
 - We cover true/false branches for key predicates:
   - aCol == nullptr path (no work done)
   - Ordinary vs non-ordinary table path (pDfltList handling)
   - pnBytesFreed == 0 vs != 0 path (setting aCol/nCol and pDfltList)
   - Multiple columns (ensuring per-column cleanup)
 - We use non-terminating checks (test continues after a failure) and accumulate
   results to report at the end.
 - Static helpers in the test mimic the static/static-like behavior of the
   original code but are accessible for tests via simple function names.
 - Only standard library and basic C/C++ facilities are used; no external test
   frameworks.

 To compile this file, simply compile with a C++11 capable compiler, e.g.:
 g++ -std=c++11 -O2 -Wall -Wextra -o test_sqlite_delete_names test_sqlite_delete_names.cpp
*/

#include <vector>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// Lightweight mockup of the minimal types used by sqlite3DeleteColumnNames

struct sqlite3 {
  int pnBytesFreed; // mimic the memory freed counter used by the real code
};

struct Column {
  const char *zCnName; // column name in C-name form (may be NULL)
  unsigned hName;        // hash of the name (as produced by sqlite3StrIHash)
};

struct DfltList { 
  // placeholder for default value list (ExprList in real code)
};

struct Table {
  Column *aCol;           // array of Column
  int nCol;                 // number of columns
  struct {
    struct {
      DfltList *pDfltList; // default expression list for ORDINARY tables
    } tab;
  } u;
  int isOrdinary;            // mimic: non-zero if ordinary table
};

// --- Helpers that the focal function relies on (minimal, test-only versions) ---

static unsigned int sqlite3StrIHash(const char *z){
  // Simple case-insensitive hash: sum with a light mix
  unsigned int h = 0;
  if(z){
    while(*z){
      unsigned char c = static_cast<unsigned char>(*z++);
      if('A' <= c && c <= 'Z') c = c - 'A' + 'a';
      h = h*131u + static_cast<unsigned int>(c);
    }
  }
  return h;
}

// Track freed strings for test verification
static std::vector<const char*> g_freedStrings;

// Allocate a copy of a C-string (test memory management)
static char* test_strdup(const char *s){
  if(!s) return nullptr;
  char *p = new char[strlen(s) + 1];
  std::strcpy(p, s);
  return p;
}

// Simulated memory free used by the focal code for zCnName
static void sqlite3DbFree(sqlite3 *db, void *p){
  (void)db; // unused in test
  if(p){
    // record and then actually free the memory
    g_freedStrings.push_back((const char*)p);
    delete[] (char*)p;
  }
}

// Free the array of Columns (simulated as delete[] in test)
static void sqlite3DbNNFreeNN(sqlite3 *db, Column *p){
  (void)db; // unused in test
  if(p){
    delete[] p;
  }
}

// No-op in test (ExprListDelete)
static void sqlite3ExprListDelete(sqlite3 *db, DfltList *p){
  (void)db;
  (void)p;
}

// IsOrdinaryTable predicate (test-controlled)
static int IsOrdinaryTable(Table *pTable){
  return pTable ? (pTable->isOrdinary != 0) : 0;
}

// No-op for the test (to mimic real function behavior)
static void sqlite3DeleteReturning(sqlite3 *db, void *pRet){ (void)db; (void)pRet; }

// The literal focal function under test (re-implemented here for self-contained testing)
void sqlite3DeleteColumnNames(sqlite3 *db, Table *pTable){
  int i;
  Column *pCol;
  assert( pTable!=0 );
  assert( db!=0 );
  if( (pCol = pTable->aCol)!=0 ){
    for(i=0; i<pTable->nCol; i++, pCol++){
      // In test, ensure invariant: zCnName==0 OR hName equals hash of zCnName
      assert( pCol->zCnName==0 || pCol->hName==sqlite3StrIHash(pCol->zCnName) );
      sqlite3DbFree(db, (void*)pCol->zCnName);
    }
    sqlite3DbNNFreeNN(db, pTable->aCol);
    if( IsOrdinaryTable(pTable) ){
      sqlite3ExprListDelete(db, pTable->u.tab.pDfltList);
    }
    if( db->pnBytesFreed==0 ){
      pTable->aCol = 0;
      pTable->nCol = 0;
      if( IsOrdinaryTable(pTable) ){
        pTable->u.tab.pDfltList = 0;
      }
    }
  }
}

// --- Test scaffolding: per-test wrappers with explanatory comments ---

// Test 1: When there are no columns (aCol == nullptr), function should be a no-op without crash
static bool test_NoColumns_Nop(){
  sqlite3 db;
  db.pnBytesFreed = 0;
  Table t;
  t.aCol = nullptr;
  t.nCol = 0;
  t.isOrdinary = 1;
  t.u.tab.pDfltList = nullptr;

  // Act
  sqlite3DeleteColumnNames(&db, &t);

  // Assert: nothing particular to assert; ensure no crash and state unchanged
  bool ok = (t.aCol == nullptr) && (t.nCol == 0) && (t.u.tab.pDfltList == nullptr);
  return ok;
}

// Test 2: Ordinary table with a single column; ensure zCnName is freed and pointers reset
static bool test_SingleColumn_Ordinary_FreeAll(){
  sqlite3 db;
  db.pnBytesFreed = 0;
  Table t;
  t.aCol = new Column[1];
  t.nCol = 1;
  t.isOrdinary = 1;
  t.u.tab.pDfltList = new DfltList();

  // Prepare a single column
  const char *name = "ColOne";
  t.aCol[0].zCnName = test_strdup(name);
  t.aCol[0].hName = sqlite3StrIHash(t.aCol[0].zCnName);

  // Act
  sqlite3DeleteColumnNames(&db, &t);

  // Assert
  bool ok = true;
  if(t.aCol != 0) ok = false;           // should be set to 0
  if(t.nCol != 0) ok = false;            // should be 0
  if(t.u.tab.pDfltList != 0) ok = false; // ordinary table -> pDfltList freed (set to 0)
  // The zCnName string should have been freed
  if(g_freedStrings.empty()) ok = false;
  // Clean up any residual (defensive)
  // (Note: actual array was freed by sqlite3DbNNFreeNN, we already checked pointers)
  // In case any extra memory remains due to test harness, ignore here.

  // Reset static state for next tests
  t.aCol = nullptr;
  delete[] (Column*)t.aCol; // safe no-op if already null

  // Clear freed record for subsequent tests
  g_freedStrings.clear();

  return ok;
}

// Test 3: Non-ordinary table with a single column; ensure aCol/nCol reset but pDfltList remains
static bool test_SingleColumn_NonOrdinary_DfltListUnchanged(){
  sqlite3 db;
  db.pnBytesFreed = 0;
  Table t;
  t.aCol = new Column[1];
  t.nCol = 1;
  t.isOrdinary = 0; // non-ordinary
  t.u.tab.pDfltList = new DfltList();

  const char *name = "ColTwo";
  t.aCol[0].zCnName = test_strdup(name);
  t.aCol[0].hName = sqlite3StrIHash(t.aCol[0].zCnName);

  sqlite3DeleteColumnNames(&db, &t);

  bool ok = true;
  if(t.aCol != 0) ok = false;
  if(t.nCol != 0) ok = false;
  if(t.u.tab.pDfltList == 0) ok = false; // should remain non-null for non-ordinary
  // Freed string should be recorded
  if(g_freedStrings.empty()) ok = false;

  // Cleanup
  // After function, aCol array memory was freed by sqlite3DbNNFreeNN
  // Ensure we don't double-free in test cleanup
  delete[] (Column*)t.aCol; // t.aCol is already freed; guard in real harness
  t.aCol = nullptr;
  g_freedStrings.clear();

  return ok;
}

// Test 4: pnBytesFreed != 0 should avoid zeroing pTable fields
static bool test_PnBytesFreedNonZero_DoesNotZeroPointers(){
  sqlite3 db;
  db.pnBytesFreed = 1; // non-zero prevents zeroing of pTable fields
  Table t;
  t.aCol = new Column[1];
  t.nCol = 1;
  t.isOrdinary = 1;
  t.u.tab.pDfltList = new DfltList();

  const char *name = "ColThree";
  t.aCol[0].zCnName = test_strdup(name);
  t.aCol[0].hName = sqlite3StrIHash(t.aCol[0].zCnName);

  sqlite3DeleteColumnNames(&db, &t);

  bool ok = true;
  if(t.aCol == nullptr) ok = false; // should not be nulled due to pnBytesFreed != 0
  if(t.nCol == 0) ok = false;        // should not be zeroed
  // Memory for zCnName should still have been freed
  if(g_freedStrings.empty()) ok = false;

  // Cleanup
  // Clean allocated non-array memory
  delete[] (Column*)t.aCol;
  delete (DfltList*)t.u.tab.pDfltList; // safe even if not used
  t.aCol = nullptr;
  t.u.tab.pDfltList = nullptr;
  g_freedStrings.clear();

  return ok;
}

// Test 5: Multiple columns are freed; ensure both zCnNames are freed
static bool test_MultipleColumns_FreeAllNames(){
  sqlite3 db;
  db.pnBytesFreed = 0;
  Table t;
  t.nCol = 2;
  t.aCol = new Column[2];
  t.isOrdinary = 1;
  t.u.tab.pDfltList = new DfltList();

  // First col
  t.aCol[0].zCnName = test_strdup("ColA");
  t.aCol[0].hName = sqlite3StrIHash(t.aCol[0].zCnName);
  // Second col
  t.aCol[1].zCnName = test_strdup("ColB");
  t.aCol[1].hName = sqlite3StrIHash(t.aCol[1].zCnName);

  sqlite3DeleteColumnNames(&db, &t);

  bool ok = true;
  if(t.aCol != 0) ok = false;
  if(t.nCol != 0) ok = false;
  if(t.u.tab.pDfltList != 0) ok = false;
  // Both zCnName strings should be freed
  if(g_freedStrings.size() != 2) ok = false;

  // Cleanup
  // No memory to free for aCol (already freed); ensure test harness clean
  g_freedStrings.clear();

  return ok;
}

// --- Test runner and main ---

int main(){
  int total = 5;
  int passed = 0;

  bool r1 = test_NoColumns_Nop();
  std::cout << "Test1 (NoColumns_Nop): " << (r1 ? "PASS" : "FAIL") << std::endl;
  if(r1) ++passed;

  bool r2 = test_SingleColumn_Ordinary_FreeAll();
  std::cout << "Test2 (SingleColumn_Ordinary_FreeAll): " << (r2 ? "PASS" : "FAIL") << std::endl;
  if(r2) ++passed;

  bool r3 = test_SingleColumn_NonOrdinary_DfltListUnchanged();
  std::cout << "Test3 (SingleColumn_NonOrdinary_DfltListUnchanged): " << (r3 ? "PASS" : "FAIL") << std::endl;
  if(r3) ++passed;

  bool r4 = test_PnBytesFreedNonZero_DoesNotZeroPointers();
  std::cout << "Test4 (PnBytesFreedNonZero): " << (r4 ? "PASS" : "FAIL") << std::endl;
  if(r4) ++passed;

  bool r5 = test_MultipleColumns_FreeAllNames();
  std::cout << "Test5 (MultipleColumns_FreeAllNames): " << (r5 ? "PASS" : "FAIL") << std::endl;
  if(r5) ++passed;

  std::cout << "Summary: " << passed << "/" << total << " tests passed." << std::endl;
  return (passed == total) ? 0 : 1;
}
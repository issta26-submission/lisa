// C++11 lightweight unit tests for sqlite3SrcListAppend (no GTest/GMock used)

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <sqliteInt.h>


// Domain: Minimal in-file stubs to emulate the sqlite3 types and functions used by the focal method.
// This allows compiling and executing unit tests without linking against the real SQLite codebase.

// --------------------------------------------------------------------------------------
// Focal method and dependencies (reproduced/adapted in-test for compilation)

// Forward declarations of test scaffolding types
struct SrcItem;
struct SrcList;
struct Token;
struct Parse;
struct sqlite3;

// Token: minimal string holder
struct Token {
  const char *z;
};

// SrcItem: holds item data for SrcList
struct SrcItem {
  const char *zName;
  const char *zDatabase;
  int iCursor;
};

// SrcList: dynamic array with a small flexible array at end
struct SrcList {
  int nAlloc;
  int nSrc;
  SrcItem a[1]; // flexible array (simulated in tests)
};

// Parse: minimal parsing context with a database handle
struct sqlite3;
struct Parse {
  sqlite3 *db;
};

// Minimal sqlite3 handle
struct sqlite3 {
  int dummy;
};

// Forward declarations of dependencies used by sqlite3SrcListAppend
static char* sqlite3NameFromToken(sqlite3 *db, const Token *pName);
static void sqlite3SrcListDelete(sqlite3 *db, SrcList *pList);
static SrcList* sqlite3SrcListEnlarge(Parse *pParse, SrcList *pSrc, int nExtra, int iStart);
static void* sqlite3DbMallocRawNN(sqlite3 *db, size_t sz);

// The focal method under test (reproduced here for the test harness)
SrcList *sqlite3SrcListAppend(
  Parse *pParse,      /* Parsing context, in which errors are reported */
  SrcList *pList,     /* Append to this SrcList. NULL creates a new SrcList */
  Token *pTable,      /* Table to append */
  Token *pDatabase    /* Database of the table */
){
{
  SrcItem *pItem;
  sqlite3 *db;
  assert( pDatabase==0 || pTable!=0 );  /* Cannot have C without B */
  assert( pParse!=0 );
  assert( pParse->db!=0 );
  db = pParse->db;
  if( pList==0 ){
    pList = sqlite3DbMallocRawNN(pParse->db, sizeof(SrcList) );
    if( pList==0 ) return 0;
    pList->nAlloc = 1;
    pList->nSrc = 1;
    memset(&pList->a[0], 0, sizeof(pList->a[0]));
    pList->a[0].iCursor = -1;
  }else{
    SrcList *pNew = sqlite3SrcListEnlarge(pParse, pList, 1, pList->nSrc);
    if( pNew==0 ){
      sqlite3SrcListDelete(db, pList);
      return 0;
    }else{
      pList = pNew;
    }
  }
  pItem = &pList->a[pList->nSrc-1];
  if( pDatabase && pDatabase->z==0 ){
    pDatabase = 0;
  }
  if( pDatabase ){
    pItem->zName = sqlite3NameFromToken(db, pDatabase);
    pItem->zDatabase = sqlite3NameFromToken(db, pTable);
  }else{
    pItem->zName = sqlite3NameFromToken(db, pTable);
    pItem->zDatabase = 0;
  }
  return pList;
}
}

// --------------------------------------------------------------------------------------
// Simple test scaffolding state and helpers

// Flags to simulate allocation behavior
static bool g_allocFail = false;
static bool g_enlargeFail = false;
static int g_freedLists = 0;

// Implementations of dependencies used by sqlite3SrcListAppend

static char* sqlite3NameFromToken(sqlite3 *db, const Token *pName){
  (void)db; // unused in tests
  if( pName==0 || pName->z==0 ) return nullptr;
  size_t len = std::strlen(pName->z) + 1;
  char *z = (char*)std::malloc(len);
  if( z ) std::memcpy(z, pName->z, len);
  return z;
}

static void sqlite3SrcListDelete(sqlite3 *db, SrcList *pList){
  (void)db;
  if( pList==nullptr ) return;
  // Free string fields for existing items
  for(int i = 0; i < pList->nSrc; ++i){
    if( pList->a[i].zName ) std::free((void*)pList->a[i].zName);
    if( pList->a[i].zDatabase ) std::free((void*)pList->a[i].zDatabase);
  }
  std::free(pList);
  g_freedLists++;
}

static SrcList* sqlite3SrcListEnlarge(
  Parse *pParse,     
  SrcList *pSrc,     
  int nExtra,        
  int iStart         
){
  (void)pParse; (void)iStart;
  if( pSrc==nullptr ) return nullptr;
  if( g_enlargeFail ) return nullptr;
  int newAlloc = pSrc->nAlloc + nExtra;
  size_t oldSize = sizeof(SrcList) + (pSrc->nAlloc - 1) * sizeof(SrcItem);
  size_t newSize = sizeof(SrcList) + (newAlloc - 1) * sizeof(SrcItem);
  SrcList *pNew = (SrcList*)std::malloc(newSize);
  if( pNew==nullptr ) return nullptr;
  // Copy existing content (header + current items)
  std::memcpy(pNew, pSrc, oldSize);
  pNew->nAlloc = newAlloc;
  pNew->nSrc = pSrc->nSrc + nExtra;
  // Initialize new slots
  for(int i = pSrc->nSrc; i < pNew->nSrc; ++i){
    pNew->a[i].iCursor = 0;
    pNew->a[i].zName = nullptr;
    pNew->a[i].zDatabase = nullptr;
  }
  return pNew;
}

static void* sqlite3DbMallocRawNN(sqlite3 *db, size_t sz){
  (void)db;
  if( g_allocFail || sz == 0 ) return nullptr;
  void *p = std::malloc(sz);
  return p;
}

// --------------------------------------------------------------------------------------
// Test helpers

static SrcList* makeInitialSrcList(int nSrc){
  // Allocate minimal SrcList memory for one SrcItem (nAlloc >= 1, nSrc >= 1)
  SrcList* p = (SrcList*)std::malloc(sizeof(SrcList));
  if(!p) return nullptr;
  p->nAlloc = 1;
  p->nSrc = nSrc;
  // Initialize items
  for(int i=0; i<nSrc; ++i){
    p->a[i].iCursor = 0;
    p->a[i].zName = nullptr;
    p->a[i].zDatabase = nullptr;
  }
  // Ensure the first item is zeroed as per test expectations
  if(nSrc>0) p->a[0].iCursor = -1;
  return p;
}

// Simple string compare helper
static bool eq(const char* a, const char* b){
  if(a==nullptr || b==nullptr) return a==b;
  return std::strcmp(a,b)==0;
}

// --------------------------------------------------------------------------------------
// Tests

bool test_SrcListAppend_createsNewList_NoDatabase(){
  // Setup
  sqlite3 db;
  Parse p;
  p.db = &db;
  Token tTable;
  tTable.z = "tbl_A";

  // Call
  SrcList *res = sqlite3SrcListAppend(&p, nullptr, &tTable, nullptr);

  // Validate
  bool ok = true;
  if( res==nullptr ) ok = false;
  else {
    if( res->nAlloc != 1 || res->nSrc != 1 ) ok = false;
    if( res->a[0].iCursor != -1 ) ok = false;
    if( !eq(res->a[0].zName, "tbl_A") ) ok = false;
    if( res->a[0].zDatabase != nullptr ) ok = false;
  }

  // Cleanup
  sqlite3SrcListDelete(p.db, res);

  return ok;
}

bool test_SrcListAppend_enlargeToAddItem_NoDatabase(){
  // Setup
  sqlite3 db;
  Parse p;
  p.db = &db;

  SrcList* pList = makeInitialSrcList(1);
  if(!pList) return false;
  Token tTable;
  tTable.z = "tbl_B";

  // Call
  SrcList *res = sqlite3SrcListAppend(&p, pList, &tTable, nullptr);

  // Validate
  bool ok = true;
  if( res==nullptr ) ok = false;
  else {
    // New nSrc should be 2
    if( res->nSrc != 2 ) ok = false;
    // Old item should still be intact
    // New item should have zName "tbl_B" and zDatabase == nullptr
    if( res->a[1].zName==nullptr || !eq(res->a[1].zName, "tbl_B") ) ok = false;
    if( res->a[1].zDatabase != nullptr ) ok = false;
  }

  // Cleanup
  sqlite3SrcListDelete(p.db, res);

  return ok;
}

bool test_SrcListAppend_enlargeAddsDbAndTableNames(){
  // Setup
  sqlite3 db;
  Parse p;
  p.db = &db;

  SrcList* pList = makeInitialSrcList(1);
  if(!pList) return false;
  Token tTable;
  tTable.z = "tbl_C";
  Token tDb;
  tDb.z = "db_C"; // Database token

  // Call
  SrcList *res = sqlite3SrcListAppend(&p, pList, &tTable, &tDb);

  // Validate
  bool ok = true;
  if( res==nullptr ) ok = false;
  else {
    if( res->nSrc != 2 ) ok = false;
    // New item's zName should be pDb's token, and zDatabase should be pTable
    if( res->a[1].zName==nullptr || !eq(res->a[1].zName, "db_C") ) ok = false;
    if( res->a[1].zDatabase==nullptr || !eq(res->a[1].zDatabase, "tbl_C") ) ok = false;
  }

  // Cleanup
  sqlite3SrcListDelete(p.db, res);

  return ok;
}

bool test_SrcListAppend_dbTokenWithNullZ_IsTreatedAsNullDatabase(){
  // Setup
  sqlite3 db;
  Parse p;
  p.db = &db;

  SrcList* pList = makeInitialSrcList(1);
  if(!pList) return false;
  Token tTable;
  tTable.z = "tbl_D";
  Token tDb;
  tDb.z = nullptr; // z==0 -> treated as no database

  // Call
  SrcList *res = sqlite3SrcListAppend(&p, pList, &tTable, &tDb);

  // Validate
  bool ok = true;
  if( res==nullptr ) ok = false;
  else {
    if( res->nSrc != 2 ) ok = false;
    if( res->a[1].zName==nullptr || !eq(res->a[1].zName, "tbl_D") ) ok = false;
    if( res->a[1].zDatabase != nullptr ) ok = false;
  }

  // Cleanup
  sqlite3SrcListDelete(p.db, res);

  return ok;
}

bool test_SrcListAppend_allocationFailureReturnsNull(){
  // Setup: force allocation failure on first malloc
  g_allocFail = true;
  sqlite3 db;
  Parse p;
  p.db = &db;
  Token tTable;
  tTable.z = "tbl_E";

  SrcList *res = sqlite3SrcListAppend(&p, nullptr, &tTable, nullptr);

  // Validate: should be null due to allocation failure
  bool ok = (res == nullptr);

  // Cleanup state
  g_allocFail = false;
  return ok;
}

bool test_SrcListAppend_enlargeFailureCleansUpList(){
  // Setup: begin with a valid list; force enlarge to fail
  g_enlargeFail = true;

  sqlite3 db;
  Parse p;
  p.db = &db;

  SrcList* pList = makeInitialSrcList(1);
  if(!pList) { g_enlargeFail = false; return false; }

  Token tTable;
  tTable.z = "tbl_F";

  SrcList *res = sqlite3SrcListAppend(&p, pList, &tTable, nullptr);

  // Validate: should return null and the original pList should have been freed
  bool ok = (res == nullptr);
  ok = ok && (g_freedLists >= 1);

  // Cleanup: ensure no leaks remain
  g_enlargeFail = false;
  // If enlarge failed, the original pList should have been freed already by the focal code path.
  return ok;
}

// --------------------------------------------------------------------------------------
// Driver

int main(){
  int total = 0;
  int passed = 0;

  auto run = [&](const char* name, bool(*fn)())->bool{
    total++;
    bool r = fn();
    if(r){
      std::cout << "[PASS] " << name << "\n";
      passed++;
    }else{
      std::cout << "[FAIL] " << name << "\n";
    }
    return r;
  };

  // Run tests
  run("SrcListAppend_createsNewList_NoDatabase", test_SrcListAppend_createsNewList_NoDatabase);
  run("SrcListAppend_enlargeToAddItem_NoDatabase", test_SrcListAppend_enlargeToAddItem_NoDatabase);
  run("SrcListAppend_enlargeAddsDbAndTableNames", test_SrcListAppend_enlargeAddsDbAndTableNames);
  run("SrcListAppend_dbTokenWithNullZ_IsTreatedAsNullDatabase", test_SrcListAppend_dbTokenWithNullZ_IsTreatedAsNullDatabase);
  run("SrcListAppend_allocationFailureReturnsNull", test_SrcListAppend_allocationFailureReturnsNull);
  run("SrcListAppend_enlargeFailureCleansUpList", test_SrcListAppend_enlargeFailureCleansUpList);

  std::cout << "\nSummary: " << passed << " / " << total << " tests passed.\n";
  return (passed == total) ? 0 : 1;
}

// End of test suite
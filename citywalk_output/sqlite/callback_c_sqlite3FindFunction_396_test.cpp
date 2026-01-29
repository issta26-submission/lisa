// C++11 test suite for sqlite3FindFunction (reproduced minimal environment)
 // Note: This test suite uses a handcrafted mock environment to exercise the
 // focal function without relying on the full SQLite source tree.
 // The goal is to achieve high coverage by exercising true/false branches.

#include <vector>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <cctype>
#include <iostream>
#include <cstdlib>


// DOMAIN_KNOWLEDGE: Use only standard library, provide minimal mocks, and run tests from main.
// We implement a tiny stubbed environment to allow sqlite3FindFunction to operate.
// The test harness is self-contained and does not require GTest.

using namespace std;

// Forward declaration of the focal function's type to mimic the original signature
struct FuncDef;
typedef unsigned char u8;
typedef struct sqlite3 sqlite3;

// Basic function object (simplified)
struct FuncDef {
    FuncDef *pNext;
    const char *zName;
    unsigned short nArg;
    unsigned int funcFlags;
    void (*xSFunc)(void);
};

// Simple hash-like container to hold app-defined functions (mock)
struct Hash {
    FuncDef *pList;
};

// Minimal sqlite3 structure (mock)
struct sqlite3 {
    Hash aFunc;       // hash table of app-defined functions
    int mDbFlags;     // DB flags (e.g., PreferBuiltin)
};

// Constants and helpers to resemble SQLite behavior
const int DBFLAG_PreferBuiltin = 1 << 0;
const int FUNC_PERFECT_MATCH = 10; // chosen for test scoring

// Globals for test harness
static FuncDef* builtinHead = nullptr; // built-in functions list (mock)
static unsigned char sqlite3UpperToLower[256];

// Simple idempotent alloc/free wrappers for test
static void* sqlite3DbMallocZero(void* db, size_t n){
    (void)db; // unused in mock
    void* p = malloc(n);
    if(p) memset(p, 0, n);
    return p;
}
static void sqlite3DbFree(void* db, void* p){
    (void)db;
    free(p);
}
static void sqlite3OomFault(void* db){
    (void)db;
    // In test, we simply do nothing; we could set a flag if needed.
}

// Utility: case-insensitive string compare (ASCII)
static int stricmp_(const char* a, const char* b){
    for(;;){
        unsigned char ca = (unsigned char)tolower((unsigned char)*a);
        unsigned char cb = (unsigned char)tolower((unsigned char)*b);
        if(ca != cb) return (int)ca - (int)cb;
        if(*a == '\0') return 0;
        ++a; ++b;
    }
}

// Utility: length similar to sqlite3Strlen30
static int sqlite3Strlen30(const char* z){
    return (int)strlen(z);
}

// Hash operations (mock)

/* Find function by exact name in app-defined hash */
static FuncDef* sqlite3HashFind(Hash* h, const char* zName){
    if(!h) return nullptr;
    FuncDef* p = h->pList;
    while(p){
        if(strcmp(p->zName, zName)==0) return p;
        p = p->pNext;
    }
    return nullptr;
}

/* Insert function into app-defined hash.
   Returns NULL on success (insertion made); returns existing entry on collision. */
static FuncDef* sqlite3HashInsert(Hash* h, const char* zName, FuncDef* pNew){
    if(!h || !pNew) return nullptr;
    // If exists, return existing
    FuncDef* existing = sqlite3HashFind(h, zName);
    if(existing){
        return existing;
    }
    // Insert at head
    pNew->zName = zName; // assume caller allocated string for simplicity
    pNew->pNext = h->pList;
    h->pList = pNew;
    return nullptr;
}

// Built-in function search (mock)
static FuncDef* sqlite3FunctionSearch(int h, const char* zFunc){
    (void)h;
    if(!zFunc) return nullptr;
    // Linear scan for case-insensitive match
    FuncDef* p = builtinHead;
    while(p){
        if(stricmp_(p->zName, zFunc) == 0){
            return p;
        }
        p = p->pNext;
    }
    return nullptr;
}

// Add a built-in function (mock)
static void addBuiltinFunc(const char* name, int nArg, u8 enc, void (*xSFunc)(void)){
    // Allocate memory for FuncDef plus name string inline (as in sqlite's allocation style)
    size_t nameLen = strlen(name);
    FuncDef* p = (FuncDef*)malloc(sizeof(FuncDef) + nameLen + 1);
    p->pNext = builtinHead;
    p->zName = (const char*)&p[1];
    memcpy((char*)p->zName, name, nameLen+1);
    p->nArg = (unsigned short)nArg;
    p->funcFlags = (unsigned int)enc;
    p->xSFunc = xSFunc;
    builtinHead = p;
}

// Match quality heuristic used by sqlite3FindFunction (mock)
static int matchQuality(FuncDef* p, int nArg, u8 enc){
    int score = 0;
    if(p==nullptr) return 0;
    if(p->nArg == nArg || nArg == -1) score += 1;
    if((p->funcFlags & 0xFF) == enc) score += 1;
    // Also give extra weight if xSFunc exists
    if(p->xSFunc) score += 1;
    return score;
}

// Forward declaration of focal function (reproduced in test)
static FuncDef* sqlite3FindFunction(
  sqlite3 *db,       
  const char *zName, 
  int nArg,          
  u8 enc,            
  u8 createFlag      
);

// Implementation of focal function (as provided, adapted for mocks)
static FuncDef* sqlite3FindFunction(
  sqlite3 *db,       /* An open database */
  const char *zName, /* Name of the function.  zero-terminated */
  int nArg,          /* Number of arguments.  -1 means any number */
  u8 enc,            /* Preferred text encoding */
  u8 createFlag      /* Create new entry if true and does not otherwise exist */
){
  FuncDef *p;         /* Iterator variable */
  FuncDef *pBest = 0; /* Best match found so far */
  int bestScore = 0;  /* Score of best match */
  int h;              /* Hash value */
  int nName;          /* Length of the name */
  // assertion equivalents (simple runtime checks)
  if(nArg < -2) return 0;
  if(nArg < -1 || createFlag==0) { /* ok */ }
  nName = sqlite3Strlen30(zName);
  // First search for a match amongst the application-defined functions.
  p = sqlite3HashFind(&db->aFunc, zName);
  while( p ){
    int score = matchQuality(p, nArg, enc);
    if( score>bestScore ){
      pBest = p;
      bestScore = score;
    }
    p = p->pNext;
  }
  // If no match is found, search the built-in functions.
  //
  // If the DBFLAG_PreferBuiltin flag is set, then search the built-in
  // functions even if a prior app-defined function was found.  And give
  // priority to built-in functions.
  //
  // Except, if createFlag is true, that means that we are trying to
  // install a new function.  Whatever FuncDef structure is returned it will
  // have fields overwritten with new information appropriate for the
  // new function.  But the FuncDefs for built-in functions are read-only.
  // So we must not search for built-ins when creating a new function.
  if( !createFlag && (pBest==0 || (db->mDbFlags & DBFLAG_PreferBuiltin)!=0) ){
    bestScore = 0;
    // emulate hash for built-in search (we ignore h here for simplicity)
    p = sqlite3FunctionSearch(0, zName);
    while( p ){
      int score = matchQuality(p, nArg, enc);
      if( score>bestScore ){
        pBest = p;
        bestScore = score;
      }
      p = p->pNext;
    }
  }
  // If createFlag is true and the search did not reveal an
  // exact match for the name, number of arguments and encoding, then add a
  // new entry to the hash table and return it.
  if( createFlag && bestScore<FUNC_PERFECT_MATCH && 
      (pBest = (FuncDef*)sqlite3DbMallocZero(db, sizeof(*pBest)+nName+1))!=0 ){
    FuncDef *pOther;
    u8 *z;
    pBest->zName = (const char*)&pBest[1];
    pBest->nArg = (u16)nArg;
    pBest->funcFlags = enc;
    memcpy((char*)&pBest[1], zName, nName+1);
    for(z=(u8*)pBest->zName; *z; z++) *z = sqlite3UpperToLower[*z];
    pOther = (FuncDef*)sqlite3HashInsert(&db->aFunc, pBest->zName, pBest);
    if( pOther==pBest ){
      sqlite3DbFree(db, pBest);
      sqlite3OomFault(db);
      return 0;
    }else{
      pBest->pNext = pOther;
    }
  }
  if( pBest && (pBest->xSFunc || createFlag) ){
    return pBest;
  }
  return 0;
}

// Simple expectations/print helper for tests
static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) do { if(!(cond)) { ++g_failures; cerr << "EXPECT_TRUE failed: " << (msg) << "\n"; } } while(0)
#define EXPECT_NOT_NULL(p, msg) do { if((p)==nullptr){ ++g_failures; cerr << "EXPECT_NOT_NULL failed: " << (msg) << "\n"; } } while(0)
#define EXPECT_EQ(a,b,msg) do { if((a)!=(b)){ ++g_failures; cerr << "EXPECT_EQ failed: " << (msg) << " (" << (a) << " != " << (b) << ")\n"; } } while(0)

// Dummy xSFunc implementations
static void dummy_xFunc(void){}

// Helpers to reset environment between tests
static void reset_environment(){
    // Free built-ins
    while(builtinHead){
        FuncDef* t = builtinHead;
        builtinHead = builtinHead->pNext;
        free(t);
    }
    builtinHead = nullptr;
    // Clear sqlite3UpperToLower mask
    for(int i=0;i<256;i++) sqlite3UpperToLower[i] = (unsigned char)i;
}

// Test 1: App-defined function match should be returned when exact match and xSFunc present
static void test_app_defined_match(){
    reset_environment();

    // Prepare db
    sqlite3 db;
    db.aFunc.pList = nullptr;
    db.mDbFlags = 0;

    // Create an app-defined function "myFunc" with nArg=2 and enc=0, with an XFunc
    size_t nameLen = strlen("myFunc");
    FuncDef* app = (FuncDef*)malloc(sizeof(FuncDef) + nameLen + 1);
    app->pNext = nullptr;
    app->zName = (const char*)&app[1];
    memcpy((char*)app->zName, "myFunc", nameLen+1);
    app->nArg = 2;
    app->funcFlags = 0;
    app->xSFunc = dummy_xFunc;
    // insert into db
    sqlite3HashInsert(&db.aFunc, app->zName, app);

    // Prepare sqlite3UpperToLower (not strictly needed for this path, but keep coherent)
    for(int i=0;i<256;i++) sqlite3UpperToLower[i] = (unsigned char)i;

    FuncDef* result = sqlite3FindFunction(&db, "myFunc", 2, 0, 0);

    EXPECT_NOT_NULL(result, "Expected a function instance to be returned for app-defined function.");
    EXPECT_EQ(result, app, "Returned function should be the exact app-defined function.");
}

// Test 2: Built-in function should be found when prefBuiltin flag is set and app doesn't overshadow it
static void test_builtin_search_with_pref(){
    reset_environment();

    // Prepare db with preferBuiltin flag
    sqlite3 db;
    db.aFunc.pList = nullptr;
    db.mDbFlags = DBFLAG_PreferBuiltin;

    // Ensure built-in "toupper" exists
    addBuiltinFunc("toupper", 1, 0, dummy_xFunc); // built-in with 1 arg

    // Query for "toupper" (case-insensitive)
    FuncDef* result = sqlite3FindFunction(&db, "TOUPPER", 1, 0, 0);

    EXPECT_NOT_NULL(result, "Expected built-in function 'toupper' to be found.");
    EXPECT_NOT_NULL(result->xSFunc, "Built-in should have an implementation (xSFunc non-NULL).");
}

// Test 3: Create new entry path when createFlag is true and no good match exists
static void test_create_new_entry_path(){
    reset_environment();

    sqlite3 db;
    db.aFunc.pList = nullptr;
    db.mDbFlags = 0;

    const char* newName = "newFunc";
    // Ensure there is no existing entry
    FuncDef* pre = sqlite3HashFind(&db.aFunc, newName);
    EXPECT_TRUE(pre == nullptr, "Precondition: new function shouldn't exist yet.");

    FuncDef* result = sqlite3FindFunction(&db, newName, 1, 0, 1); // createFlag = 1

    EXPECT_NOT_NULL(result, "Expected new function entry to be created when createFlag is true.");
    EXPECT_TRUE(result->pNext == nullptr, "New entry should have pNext == NULL when first inserted.");

    // Ensure the function is stored in the hash for later calls
    FuncDef* check = sqlite3HashFind(&db.aFunc, newName);
    EXPECT_NOT_NULL(check, "Newly created function should be present in app hash.");
    EXPECT_EQ(check, result, "Hash should point to the newly created function.");
    // Also check that the stored name matches (case-insensitive normalization in real scenario; ours stores as given)
    if(check){
        EXPECT_TRUE(strcmp(check->zName, newName)==0, "Stored name should match the provided name (case-sensitive in mock).");
    }
}

// Entry point: run all tests
int main(void){
    // Initialize to-lower mapping (simulate real environment)
    for(int i=0;i<256;i++) sqlite3UpperToLower[i] = (unsigned char)i;
    g_failures = 0;

    test_app_defined_match();
    test_builtin_search_with_pref();
    test_create_new_entry_path();

    if(g_failures==0){
        cout << "All tests passed.\n";
        return 0;
    }else{
        cout << g_failures << " test(s) failed.\n";
        return 1;
    }
}
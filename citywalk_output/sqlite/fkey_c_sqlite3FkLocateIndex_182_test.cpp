// Unit test suite for sqlite3FkLocateIndex (fkey.c) using a lightweight
// C-like environment implemented in C++11 without any external test framework.
// The goal is to exercise true/false branches, cover single/composite FK logic,
// and verify basic memory/return behavior in a self-contained manner.

// Note: This test suite provides minimal stubs for the SQLite-internal types and
// routines required by sqlite3FkLocateIndex. It is not a full SQLite build; it
// focuses on exercising the focal method's logic paths.

#include <cctype>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Domain: Provide a lightweight C-compatible environment for sqlite3FkLocateIndex
// to operate on. We mirror just enough of the real data structures.

static const char* sqlite3StrBINARY = "BINARY"; // default collation sentinel

// Forward declarations (C-like interface)
extern "C" {
    // Memory management (mocked)
    void* sqlite3DbMallocRawNN(void* db, size_t n);
    void  sqlite3DbFree(void* db, void* p);

    // String utilities
    int sqlite3StrICmp(const char* zLeft, const char* zRight);

    // Column/collation
    const char* sqlite3ColumnColl(const struct Column* pCol);

    // Errors (no-op in tests)
    void sqlite3ErrorMsg(void* pParse, const char* zFormat, ...);
}

// Minimal mock: database handle (opaque)
struct Db {};

// Parse context (subset)
struct Parse {
    Db* db;
    int disableTriggers;
};

// Column description for a table
struct Column {
    const char* zCnName; // canonical name (column name used in SQL)
    const char* zCol;    // the actual name used for FK mapping (often same as zCnName)
    const char* zColl;     // collation for this column (NULL means BINARY)
};

// Index descriptor
struct Index {
    int nKeyCol;           // number of columns in index
    short* aiColumn;         // indices into Table.aCol for each key column
    const char** azColl;       // collation for each key column
    Index* pNext;            // next index in the table (linked list)
    int pPartIdxWhere;       // non-zero if partial index (we ignore)
    int isUnique;            // 1 if UNIQUE index, 0 otherwise
};

// Foreign key column descriptor
struct FKeyCol {
    const char* zCol; // name of the parent's column
    int iFrom;        // mapping for the column (arbitrary int used in tests)
};

// Foreign key descriptor
struct FKey {
    int nCol;           // number of columns in the FK
    FKeyCol* aCol;      // array of column mappings
    Table* pFrom;        // referencing (child) table
    const char* zTo;     // referenced table name
};

// Parent table descriptor
struct Table {
    const char* zName;
    int iPKey;            // index of the primary key column in aCol (or -1 if none)
    Column* aCol;           // array of columns
    int nCol;               // number of columns
    Index* pIndex;           // linked list of indexes
};

// Implementations of the required faux SQLite-like helpers

void* sqlite3DbMallocRawNN(void* db, size_t n) {
    (void)db; // unused in tests
    return malloc(n);
}
void sqlite3DbFree(void* db, void* p) {
    (void)db;
    free(p);
}
int sqlite3StrICmp(const char* zLeft, const char* zRight) {
    if (zLeft == zRight) return 0;
    if (!zLeft) return -1;
    if (!zRight) return 1;
    while (*zLeft && *zRight) {
        unsigned char a = (unsigned char)std::tolower((unsigned char)*zLeft);
        unsigned char b = (unsigned char)std::tolower((unsigned char)*zRight);
        if (a != b) return (int)(a - b);
        zLeft++; zRight++;
    }
    if (*zLeft == 0 && *zRight == 0) return 0;
    return (*zLeft == 0) ? -1 : 1;
}
const char* sqlite3ColumnColl(const Column* pCol) {
    if (!pCol) return NULL;
    return pCol->zColl;
}
void sqlite3ErrorMsg(void* pParse, const char* zFormat, ...) {
    (void)pParse;
    // Minimal/no-op: tests do not rely on message content, but we provide a hook.
    // It can be extended to collect error messages if needed.
}

// Forward declaration of the focal function (C linkage)
extern "C" int sqlite3FkLocateIndex(
  Parse *pParse,                  /* Parse context to store any error in */
  Table *pParent,                 /* Parent table of FK constraint pFKey */
  FKey *pFKey,                    /* Foreign key to find index for */
  Index **ppIdx,                  /* OUT: Unique index on parent table */
  int **paiCol                    /* OUT: Map of index columns in pFKey */
);

// Helper: create a simple single-column index for tests
Index* makeIndex(int nKeyCol, bool isUnique, short* aiColumn, const char** azColl) {
    Index* pIdx = (Index*)malloc(sizeof(Index));
    pIdx->nKeyCol = nKeyCol;
    pIdx->isUnique = isUnique ? 1 : 0;
    pIdx->aiColumn = (short*)malloc(nKeyCol * sizeof(short));
    for(int i=0;i<nKeyCol;i++) pIdx->aiColumn[i] = aiColumn[i];
    pIdx->azColl = (const char**)malloc(nKeyCol * sizeof(char*));
    for(int i=0;i<nKeyCol;i++) pIdx->azColl[i] = azColl ? azColl[i] : NULL;
    pIdx->pNext = NULL;
    pIdx->pPartIdxWhere = 0;
    return pIdx;
}

// The focal method copied (adapted to our test environment)
extern "C" int sqlite3FkLocateIndex(
  Parse *pParse,                  /* Parse context to store any error in */
  Table *pParent,                 /* Parent table of FK constraint pFKey */
  FKey *pFKey,                    /* Foreign key to find index for */
  Index **ppIdx,                  /* OUT: Unique index on parent table */
  int **paiCol                    /* OUT: Map of index columns in pFKey */
){
  Index *pIdx = 0;                    /* Value to return via *ppIdx */
  int *aiCol = 0;                     /* Value to return via *paiCol */
  int nCol = pFKey->nCol;             /* Number of columns in parent key */
  char *zKey = pFKey->aCol[0].zCol;   /* Name of left-most parent key column */
  /* The caller is responsible for zeroing output parameters. */
  assert( ppIdx && *ppIdx==0 );
  assert( !paiCol || *paiCol==0 );
  assert( pParse );
  /* If this is a non-composite (single column) foreign key, check if it 
  ** maps to the INTEGER PRIMARY KEY of table pParent. If so, leave *ppIdx 
  ** and *paiCol set to zero and return early. 
  **
  ** Otherwise, for a composite foreign key (more than one column), allocate
  ** space for the aiCol array (returned via output parameter *paiCol).
  ** Non-composite foreign keys do not require the aiCol array.
  */
  if( nCol==1 ){
    /* The FK maps to the IPK if any of the following are true:
    **
    **   1) There is an INTEGER PRIMARY KEY column and the FK is implicitly 
    **      mapped to the primary key of table pParent, or
    **   2) The FK is explicitly mapped to a column declared as INTEGER
    **      PRIMARY KEY.
    */
    if( pParent->iPKey>=0 ){
      if( !zKey ) return 0;
      if( !sqlite3StrICmp(pParent->aCol[pParent->iPKey].zCnName, zKey) ){
        return 0;
      }
    }
  }else if( paiCol ){
    assert( nCol>1 );
    aiCol = (int *)sqlite3DbMallocRawNN(pParse->db, nCol*sizeof(int));
    if( !aiCol ) return 1;
    *paiCol = aiCol;
  }
  for(pIdx=pParent->pIndex; pIdx; pIdx=pIdx->pNext){
    if( pIdx->nKeyCol==nCol && IsUniqueIndex(pIdx) && pIdx->pPartIdxWhere==0 ){ 
      /* pIdx is a UNIQUE index (or a PRIMARY KEY) and has the right number
      ** of columns. If each indexed column corresponds to a foreign key
      ** column of pFKey, then this index is a winner.  */
      if( zKey==0 ){
        /* If zKey is NULL, then this foreign key is implicitly mapped to 
        ** the PRIMARY KEY of table pParent. The PRIMARY KEY index may be 
        ** identified by the test.  */
        if( IsPrimaryKeyIndex(pIdx) ){
          if( aiCol ){
            int i;
            for(i=0; i<nCol; i++) aiCol[i] = pFKey->aCol[i].iFrom;
          }
          break;
        }
      }else{
        /* If zKey is non-NULL, then this foreign key was declared to
        ** map to an explicit list of columns in table pParent. Check if this
        ** index matches those columns. Also, check that the index uses
        ** the default collation sequences for each column. */
        int i, j;
        for(i=0; i<nCol; i++){
          i16 iCol = pIdx->aiColumn[i];     /* Index of column in parent tbl */
          const char *zDfltColl;            /* Def. collation for column */
          char *zIdxCol;                    /* Name of indexed column */
          if( iCol<0 ) break; /* No foreign keys against expression indexes */
          /* If the index uses a collation sequence that is different from
          ** the default collation sequence for the column, this index is
          ** unusable. Bail out early in this case.  */
          zDfltColl = sqlite3ColumnColl(&pParent->aCol[iCol]);
          if( !zDfltColl ) zDfltColl = sqlite3StrBINARY;
          if( sqlite3StrICmp(pIdx->azColl[i], zDfltColl) ) break;
          zIdxCol = pParent->aCol[iCol].zCnName;
          for(j=0; j<nCol; j++){
            if( sqlite3StrICmp(pFKey->aCol[j].zCol, zIdxCol)==0 ){
              if( aiCol ) aiCol[i] = pFKey->aCol[j].iFrom;
              break;
            }
          }
          if( j==nCol ) break;
        }
        if( i==nCol ) break;      /* pIdx is usable */
      }
    }
  }
  if( !pIdx ){
    if( !pParse->disableTriggers ){
      sqlite3ErrorMsg(pParse,
           "foreign key mismatch - \"%w\" referencing \"%w\"",
           pFKey->pFrom->zName, pFKey->zTo);
    }
    sqlite3DbFree(pParse->db, aiCol);
    return 1;
  }
  *ppIdx = pIdx;
  return 0;
}

// Test helpers

static void test_pass(const char* name) {
    std::cout << "[OK] " << name << "\n";
}

// Simple assertion helper (non-terminating)
static void expect_int_eq(const char* test, int a, int b) {
    if (a != b) {
        std::cerr << "[ERR] " << test << ": expected " << b << " but got " << a << "\n";
    }
}
static void expect_ptr_eq(const char* test, void* a, void* b) {
    if (a != b) {
        std::cerr << "[ERR] " << test << ": expected pointer " << b << " but got " << a << "\n";
    }
}
static void expect_not_null(const char* test, void* a) {
    if (a == nullptr) {
        std::cerr << "[ERR] " << test << ": expected non-null pointer\n";
    }
}

// Test 1: Single-column FK maps to IPK; expect early return (0) and no index chosen
void run_test_single_column_maps_to_ipk() {
    // Build parse
    Parse parse;
    Db db;
    parse.db = &db;
    parse.disableTriggers = 0;

    // Build parent table with a single PK column named "id"
    Table parent;
    parent.zName = "Parent";
    parent.iPKey = 0;
    parent.nCol = 1;
    Column cols[1];
    cols[0].zCnName = "id"; // PK canonical name
    cols[0].zCol = "id";    // alias
    cols[0].zColl = "BINARY";
    parent.aCol = cols;
    parent.pIndex = nullptr; // no index needed for this test

    // Build foreign key referencing "Parent" with single column "id"
    FKey fkey;
    fkey.nCol = 1;
    FKeyCol fkcol;
    fkcol.zCol = "id"; // maps to parent's PK
    fkcol.iFrom = 42;
    FKeyCol fkcols[1] = { fkcol };
    fkey.aCol = fkcols;
    fkey.pFrom = (Table*)malloc(sizeof(Table)); // dummy child
    fkey.pFrom->zName = "Child";
    fkey.zTo = "Parent";

    // Output pointers initialized to zero
    Index* outIdx = nullptr;
    int* outCol = nullptr;

    int ret = sqlite3FkLocateIndex(&parse, &parent, &fkey, &outIdx, &outCol);

    // Validate behavior
    expect_int_eq("Test1 return code", ret, 0);
    expect_ptr_eq("Test1 ppIdx unchanged", (void*)outIdx, (void*)nullptr);
    expect_ptr_eq("Test1 paiCol unchanged (NULL)", (void*)outCol, (void*)nullptr);

    // Cleanup
    free(fkey.pFrom);
}

// Test 2: Composite FK maps to a composite UNIQUE index with binary collations
void run_test_composite_fk_maps_to_unique_index() {
    // Build parse
    Parse parse;
    Db db;
    parse.db = &db;
    parse.disableTriggers = 0;

    // Parent table with two columns: "a" and "b"
    Table parent;
    parent.zName = "Parent";
    parent.iPKey = -1;
    parent.nCol = 2;
    Column cols[2];
    cols[0].zCnName = "a";
    cols[0].zCol = "a";
    cols[0].zColl = "BINARY";
    cols[1].zCnName = "b";
    cols[1].zCol = "b";
    cols[1].zColl = "BINARY";
    parent.aCol = cols;

    // Create a composite UNIQUE index on ("a","b")
    short aiColumn[2] = {0, 1}; // indices into aCol
    const char* azColl[2] = { "BINARY", "BINARY" };
    Index idx;
    idx.nKeyCol = 2;
    idx.aiColumn = (short*)malloc(2*sizeof(short));
    idx.azColl = (const char**)malloc(2*sizeof(char*));
    idx.pNext = nullptr;
    idx.pPartIdxWhere = 0;
    idx.isUnique = 1;
    idx.aiColumn[0] = aiColumn[0];
    idx.aiColumn[1] = aiColumn[1];
    idx.azColl[0] = azColl[0];
    idx.azColl[1] = azColl[1];
    parent.pIndex = &idx;

    // FKey mapping: (a -> a), (b -> b)
    FKey fkey;
    fkey.nCol = 2;
    FKeyCol fkcol0; fkcol0.zCol = "a"; fkcol0.iFrom = 10;
    FKeyCol fkcol1; fkcol1.zCol = "b"; fkcol1.iFrom = 11;
    FKeyCol fkcols[2] = { fkcol0, fkcol1 };
    fkey.aCol = fkcols;
    fkey.pFrom = (Table*)malloc(sizeof(Table)); // dummy child
    fkey.pFrom->zName = "Child";
    fkey.zTo = "Parent";

    Index* outIdx = nullptr;
    int* outCol = nullptr;

    int ret = sqlite3FkLocateIndex(&parse, &parent, &fkey, &outIdx, &outCol);

    // Validate
    expect_int_eq("Test2 return code", ret, 0);
    expect_ptr_eq("Test2 idx returned", (void*)outIdx, (void*)&idx);
    if (outCol) {
        expect_int_eq("Test2 aiCol[0]", outCol[0], 10);
        expect_int_eq("Test2 aiCol[1]", outCol[1], 11);
    } else {
        std::cerr << "[ERR] Test2: expected non-null aiCol\n";
    }

    // Cleanup
    free(fkey.pFrom);
    free(idx.aiColumn);
    free(idx.azColl);
}

// Test 3: Composite FK with an unusable index (first iCol < 0) should fail with error
void run_test_composite_unusable_index() {
    // Build parse
    Parse parse;
    Db db;
    parse.db = &db;
    parse.disableTriggers = 0;

    // Parent table with two columns
    Table parent;
    parent.zName = "Parent";
    parent.iPKey = -1;
    parent.nCol = 2;
    Column cols[2];
    cols[0].zCnName = "a";
    cols[0].zCol = "a";
    cols[0].zColl = "BINARY";
    cols[1].zCnName = "b";
    cols[1].zCol = "b";
    cols[1].zColl = "BINARY";
    parent.aCol = cols;
    parent.pIndex = nullptr;

    // Create a composite index but with first aiColumn = -1 to simulate unusable index
    short aiColumn[2] = {-1, 1};
    const char* azColl[2] = { "BINARY", "BINARY" };
    Index idx;
    idx.nKeyCol = 2;
    idx.aiColumn = (short*)malloc(2*sizeof(short));
    idx.azColl = (const char**)malloc(2*sizeof(char*));
    idx.pNext = nullptr;
    idx.pPartIdxWhere = 0;
    idx.isUnique = 1;
    idx.aiColumn[0] = aiColumn[0];
    idx.aiColumn[1] = aiColumn[1];
    idx.azColl[0] = azColl[0];
    idx.azColl[1] = azColl[1];
    parent.pIndex = &idx;

    // FKey: maps to a,b
    FKey fkey;
    fkey.nCol = 2;
    FKeyCol fkcol0; fkcol0.zCol = "a"; fkcol0.iFrom = 20;
    FKeyCol fkcol1; fkcol1.zCol = "b"; fkcol1.iFrom = 21;
    FKeyCol fkcols[2] = { fkcol0, fkcol1 };
    fkey.aCol = fkcols;
    fkey.pFrom = (Table*)malloc(sizeof(Table));
    fkey.pFrom->zName = "Child";
    fkey.zTo = "Parent";

    Index* outIdx = nullptr;
    int* outCol = nullptr;

    int ret = sqlite3FkLocateIndex(&parse, &parent, &fkey, &outIdx, &outCol);

    // Expect failure (1) due to unusable index
    expect_int_eq("Test3 return code", ret, 1);
    expect_ptr_eq("Test3 idx should be NULL", (void*)outIdx, (void*)nullptr);

    // Cleanup
    free(fkey.pFrom);
    free(idx.aiColumn);
    free(idx.azColl);
}

// Test 4: Single-column FK with non-matching PK should fail with error
void run_test_single_column_nonmatching_pk() {
    // Build parse
    Parse parse;
    Db db;
    parse.db = &db;
    parse.disableTriggers = 0;

    // Parent has PK on "id"
    Table parent;
    parent.zName = "Parent";
    parent.iPKey = 0;
    parent.nCol = 1;
    Column cols[1];
    cols[0].zCnName = "id";
    cols[0].zCol = "id";
    cols[0].zColl = "BINARY";
    parent.aCol = cols;
    parent.pIndex = nullptr;

    // FK maps to column "other" which does not exist in parent's PK
    FKey fkey;
    fkey.nCol = 1;
    FKeyCol fkcol;
    fkcol.zCol = "other";
    fkcol.iFrom = 7;
    FKeyCol fkcols[1] = { fkcol };
    fkey.aCol = fkcols;
    fkey.pFrom = (Table*)malloc(sizeof(Table));
    fkey.pFrom->zName = "Child";
    fkey.zTo = "Parent";

    Index* outIdx = nullptr;
    int* outCol = nullptr;

    int ret = sqlite3FkLocateIndex(&parse, &parent, &fkey, &outIdx, &outCol);

    // Expect failure (1) because no matching index for single-column non-IPK
    expect_int_eq("Test4 return code", ret, 1);
    expect_ptr_eq("Test4 idx should be NULL", (void*)outIdx, (void*)nullptr);

    // Cleanup
    free(fkey.pFrom);
}

int main() {
    std::cout << "Starting sqlite3FkLocateIndex unit tests (lightweight C++ harness)\n";

    run_test_single_column_maps_to_ipk();
    run_test_composite_fk_maps_to_unique_index();
    run_test_composite_unusable_index();
    run_test_single_column_nonmatching_pk();

    std::cout << "Unit tests completed.\n";
    return 0;
}
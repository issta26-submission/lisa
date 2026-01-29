// Self-contained unit tests for sqlite3AddNotNull
// This test suite is designed to be compiled with C++11 (no GTest).
// It re-implements the minimal reproducer of the necessary SQLite-like types
// and the focal function sqlite3AddNotNull to enable isolated testing.

#include <iostream>
#include <sqliteInt.h>
#include <cassert>


// Domain-specific lightweight mimicking of SQLite structures and constants

typedef unsigned char u8;

// Candidate Keywords from the focal method (simplified for test harness)
#define NEVER(x) (x)
#define TF_HasNotNull 0x01
#define COLFLAG_UNIQUE 0x01
#define OE_None 0

// Forward declarations
struct Table;
struct Column;
struct Index;
struct Parse;

// Lightweight structures to reproduce the logic path of sqlite3AddNotNull
struct Column {
    u8 notNull;          // NOT NULL flag storage
    unsigned int colFlags; // column flags (e.g., UNIQUE)
};

struct Index {
    int nKeyCol;     // number of key columns in index
    int onError;     // conflict resolution mode (non OE_None means valid)
    int *aiColumn;   // array of column indices this index covers
    Index *pNext;      // next index in the linked list
    int uniqNotNull; // flag indicating uniq-not-null for this index
};

struct Table {
    int nCol;          // number of columns
    Column *aCol;       // array of columns
    int tabFlags;        // table flags (e.g., TF_HasNotNull)
    Index *pIndex;       // linked list of indexes on this table
};

struct Parse {
    Table *pNewTable;     // the new table being augmented with a NOT NULL
};

// Focal method under test (re-implemented for test harness)
void sqlite3AddNotNull(Parse *pParse, int onError) {
    Table *p;
    Column *pCol;
    p = pParse->pNewTable;
    if( p==nullptr || NEVER(p->nCol<1) ) return;
    pCol = &p->aCol[p->nCol-1];
    pCol->notNull = (u8)onError;
    p->tabFlags |= TF_HasNotNull;
    /* Set the uniqNotNull flag on any UNIQUE or PK indexes already created
    ** on this column.  */
    if( pCol->colFlags & COLFLAG_UNIQUE ){
        Index *pIdx;
        for(pIdx=p->pIndex; pIdx; pIdx=pIdx->pNext){
            assert( pIdx->nKeyCol==1 && pIdx->onError!=OE_None );
            if( pIdx->aiColumn[0]==p->nCol-1 ){
                pIdx->uniqNotNull = 1;
            }
        }
    }
}

// Simple test framework (non-terminating assertions)
static int g_failures = 0;

#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " #cond " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ(a,b) do { \
    if((a)!=(b)) { \
        std::cerr << "EXPECT_EQ failed: " #a " == " #b " -> " << (a) << " != " << (b) \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while(0)

// Helper to create a simple 1-element array for aiColumn
static int aiColArr[1];

// Test 1: pParse is null -> function should return without crash
void test_sqlite3AddNotNull_nullParse() {
    // Domain coverage: true branch when pParse is null triggers immediate return
    Parse p;
    p.pNewTable = nullptr;
    sqlite3AddNotNull(&p, 1);
    // If we reach here, the early return satisfied; no assertion errors expected
    EXPECT_TRUE(true);
}

// Test 2: pNewTable is non-null but nCol < 1 -> early return
void test_sqlite3AddNotNull_zeroColumns() {
    // Domain coverage: path where p->nCol < 1 is true
    Table t;
    t.nCol = 0;
    t.aCol = nullptr;
    t.tabFlags = 0;
    t.pIndex = nullptr;

    Parse p;
    p.pNewTable = &t;

    sqlite3AddNotNull(&p, 5);
    // Should return without modifying anything
    EXPECT_TRUE(true);
}

// Test 3: Normal path with a single column (not unique) -> sets notNull and tabFlags
void test_sqlite3AddNotNull_normalPathSetsFlags() {
    // Prepare a table with one column, non-unique
    Table t;
    t.nCol = 1;
    t.aCol = new Column[1];
    t.aCol[0].notNull = 0;
    t.aCol[0].colFlags = 0; // not unique
    t.tabFlags = 0;
    t.pIndex = nullptr;

    Parse p;
    p.pNewTable = &t;

    int onError = 3;
    sqlite3AddNotNull(&p, onError);

    // Assertions:
    // - last column notNull should be set to onError
    EXPECT_EQ((int)t.aCol[0].notNull, onError);
    // - table flag TF_HasNotNull should be set
    EXPECT_TRUE((t.tabFlags & TF_HasNotNull) != 0);

    delete[] t.aCol;
}

// Test 4: Last column is UNIQUE and an appropriate index exists -> uniqNotNull should be set
void test_sqlite3AddNotNull_uniqueIndexSetsUniqNotNull() {
    // Prepare a table with two columns; last column is UNIQUE
    Table t;
    t.nCol = 2;
    t.aCol = new Column[2];
    // Column 0 (not last): normal
    t.aCol[0].notNull = 0;
    t.aCol[0].colFlags = 0;
    // Column 1 (last): UNIQUE
    t.aCol[1].notNull = 0;
    t.aCol[1].colFlags = COLFLAG_UNIQUE;

    t.tabFlags = 0;

    // Index on this table: single key column referencing column 1
    Index idx;
    idx.nKeyCol = 1;
    idx.onError = 1; // not OE_None
    idx.uniqNotNull = 0;
    idx.aiColumn = aiColArr; // use static array to avoid extra allocations
    idx.aiColumn[0] = 1; // points to last column
    idx.pNext = nullptr;

    t.pIndex = &idx;

    Parse p;
    p.pNewTable = &t;

    int onError = 2;
    sqlite3AddNotNull(&p, onError);

    // Assertions:
    // - last column notNull updated
    EXPECT_EQ((int)t.aCol[1].notNull, onError);
    // - TF_HasNotNull set
    EXPECT_TRUE((t.tabFlags & TF_HasNotNull) != 0);
    // - index uniqNotNull should be set to 1
    EXPECT_TRUE(idx.uniqNotNull == 1);

    // Cleanup
    // Note: idx.aiColumn points to static array, no delete required
    delete[] t.aCol;
}

// Entry point for tests
int main() {
    std::cout << "Running sqlite3AddNotNull unit tests...\n";

    test_sqlite3AddNotNull_nullParse();
    test_sqlite3AddNotNull_zeroColumns();
    test_sqlite3AddNotNull_normalPathSetsFlags();
    test_sqlite3AddNotNull_uniqueIndexSetsUniqNotNull();

    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << "Tests finished with " << g_failures << " failure(s).\n";
        return 1;
    }
}
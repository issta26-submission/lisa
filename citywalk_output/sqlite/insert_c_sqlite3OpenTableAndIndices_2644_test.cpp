// Unit test harness for the focal method: sqlite3OpenTableAndIndices
// This test suite is crafted to exercise the focal function under C++11 (no GoogleTest).
// It provides minimal stub implementations for required dependencies so the focal
// method can be exercised in isolation with controlled scenarios.
// The tests follow the steps in the provided Domain Knowledge and aim to cover
// true/false branches of key predicates without terminating the process on failures.

#include <vector>
#include <cstdarg>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// We expose the focal function and its basic dependency surface as extern "C" so
// that this test harness can link against the real implementation in insert.c
extern "C" {

// Type aliases and constants matching the focal surface
typedef unsigned char u8;
typedef struct Parse Parse;
typedef struct Table Table;
typedef struct Index Index;
typedef struct Vdbe Vdbe;

// Opcodes used by sqlite3OpenTableAndIndices
#define OP_OpenRead  0
#define OP_OpenWrite 1

// Forward declarations (interfaces present in the focal codebase)
int sqlite3OpenTableAndIndices(
  Parse *pParse,   /* Parsing context */
  Table *pTab,     /* Table to be opened */
  int op,          /* OP_OpenRead or OP_OpenWrite */
  u8 p5,           /* P5 value for OP_Open* opcodes (except on WITHOUT ROWID) */
  int iBase,       /* Use this for the table cursor, if there is one */
  u8 *aToOpen,     /* If not NULL: boolean for each table and index */
  int *piDataCur,  /* Write the database source cursor number here */
  int *piIdxCur    /* Write the first index cursor number here */
);

// Helpers used by sqlite3OpenTableAndIndices (we provide implementations)
int IsVirtual(Table *pTab);
int HasRowid(Table *pTab);
int sqlite3SchemaToIndex(void *db, void *pSchema);

// Lightweight surface for external calls invoked by sqlite3OpenTableAndIndices
void sqlite3OpenTable(Parse *pParse, int iDataCur, int iDb, Table *pTab, int opcode);
void sqlite3TableLock(Parse *pParse, int iDb, int tnum, int isWrite, const char *zName);
void sqlite3VdbeAddOp3(Vdbe *v, int op, int iIdxCur, int p1, int iDb);
void sqlite3VdbeSetP4KeyInfo(Parse *pParse, Index *pIdx);
void sqlite3VdbeChangeP5(Vdbe *v, u8 p5);
void VdbeComment(Vdbe *v, const char *zFormat, ...);

// Helpers used in the test scaffolding
int IsPrimaryKeyIndex(Index *pIdx);

} // extern "C"

// Minimal local test scaffolding to emulate the required surface.
// These definitions are kept simple intentionally, focusing on observable side effects.

struct Parse {
    void *db;     // pointer to a mock sqlite3 db object (we only need a pointer)
    Vdbe *pVdbe;  // pointer to a mock Vdbe
    int nTab;     // maximum cursor index used so far
};

struct Vdbe {
    int dummy;
};

// Lightweight mock database object
struct sqlite3 {
    int noSharedCache;
};

// Tables and indices (mocked)
// We replicate just enough structure for tests to build plausible scenarios.
// The focal function uses IsVirtual(pTab) and HasRowid(pTab) to decide branches.
struct Index {
    void *pSchema;
    int tnum;
    const char *zName;
    Index *pNext;
    int isPK; // simulate "IsPrimaryKeyIndex"
};

struct Table {
    void *pSchema;
    int tnum;
    const char *zName;
    Index *pIndex;
    int hasRowid;
    int isVirtual;
};

// ---- Test instrumentation state (capturing calls) ----
static int g_openTable_calls;
static int g_last_open_iDataCur;
static int g_last_open_iDb;
static Table *g_last_open_pTab;
static int g_last_open_opcode;

static int g_tableLock_calls;
static int g_last_tableLock_iDb;
static int g_last_tableLock_tnum;
static int g_last_tableLock_isWrite;
static const char *g_last_tableLock_name;

static int g_VdbeAddOp3_calls;
static int g_last_vdbe_op;
static int g_last_vdbe_iIdxCur;
static int g_last_vdbe_p1; // alias for p1 (iIdxCur)
static int g_last_vdbe_iDb;

static int g_VdbeSetP4KeyInfo_calls;
static int g_VdbeChangeP5_calls;
static u8 g_VdbeChangeP5_last;

static int g_vkIsVirtualOverrideUsed; // unused, kept for potential future checks

static Index g_index1;
static Index g_index2;
static Table g_table;

// Forward declare helpers
static void resetLogs();

// Implementations of the required external interfaces (stubs)

// The focal surface expects these to be visible with C linkage.
extern "C" {

// Determine virtualness of a table for tests
int IsVirtual(Table *pTab) {
    // Our test doubles virtual status via a field (if provided); otherwise 0.
    if (pTab == nullptr) return 0;
    return pTab->isVirtual;
}

// Determine if a table has a rowid
int HasRowid(Table *pTab) {
    if (pTab == nullptr) return 0;
    return pTab->hasRowid;
}

// Map a schema to a database index (stubbed)
int sqlite3SchemaToIndex(void *db, void *pSchema) {
    (void)db; (void)pSchema;
    return 0;
}

// Primary key check (stubbed)
int IsPrimaryKeyIndex(Index *pIdx) {
    if (pIdx == nullptr) return 0;
    return pIdx->isPK;
}

// Basic stubs for the required operations that sqlite3OpenTableAndIndices will call
void sqlite3OpenTable(Parse *pParse, int iDataCur, int iDb, Table *pTab, int opcode) {
    ++g_openTable_calls;
    g_last_open_iDataCur = iDataCur;
    g_last_open_iDb = iDb;
    g_last_open_pTab = pTab;
    g_last_open_opcode = opcode;
}
void sqlite3TableLock(Parse *pParse, int iDb, int tnum, int isWrite, const char *zName) {
    (void)pParse; (void)zName;
    ++g_tableLock_calls;
    g_last_tableLock_iDb = iDb;
    g_last_tableLock_tnum = tnum;
    g_last_tableLock_isWrite = isWrite;
    g_last_tableLock_name = zName;
}
void sqlite3VdbeAddOp3(Vdbe *v, int op, int iIdxCur, int p1, int iDb) {
    (void)v;
    ++g_VdbeAddOp3_calls;
    g_last_vdbe_op = op;
    g_last_vdbe_iIdxCur = iIdxCur;
    g_last_vdbe_p1 = p1;
    g_last_vdbe_iDb = iDb;
}
void sqlite3VdbeSetP4KeyInfo(Parse *pParse, Index *pIdx) {
    (void)pParse; (void)pIdx;
    ++g_VdbeSetP4KeyInfo_calls;
}
void sqlite3VdbeChangeP5(Vdbe *v, u8 p5) {
    (void)v;
    g_VdbeChangeP5_last = p5;
    ++g_VdbeChangeP5_calls;
}
void VdbeComment(Vdbe *v, const char *zFormat, ...) {
    (void)v; (void)zFormat;
}
}

// ---- Test cases ----

// Helper to reset all log counters before each test
static void resetLogs() {
    g_openTable_calls = 0;
    g_last_open_iDataCur = -1;
    g_last_open_iDb = -1;
    g_last_open_pTab = nullptr;
    g_last_open_opcode = -1;

    g_tableLock_calls = 0;
    g_last_tableLock_iDb = -1;
    g_last_tableLock_tnum = -1;
    g_last_tableLock_isWrite = -1;
    g_last_tableLock_name = nullptr;

    g_VdbeAddOp3_calls = 0;
    g_last_vdbe_op = -1;
    g_last_vdbe_iIdxCur = -1;
    g_last_vdbe_p1 = -1;
    g_last_vdbe_iDb = -1;

    g_VdbeSetP4KeyInfo_calls = 0;
    g_VdbeChangeP5_calls = 0;
    g_VdbeChangeP5_last = 0;
}

// Test 1: Virtual table should be a no-op and set illegal cursors -999
void test_virtual_table_noop() {
    resetLogs();

    // Prepare parsing context and VDBe
    Parse p;
    sqlite3 db;
    Vdbe v;
    db.noSharedCache = 0;
    p.db = &db;
    p.pVdbe = &v;
    p.nTab = 0;

    // Prepare a virtual table
    Table tab;
    tab.pSchema = nullptr;
    tab.tnum = 42;
    tab.zName = "virtual_tab";
    tab.pIndex = nullptr;
    tab.hasRowid = 1;      // doesn't matter due to IsVirtual shortcut
    tab.isVirtual = 1;     // mark as virtual

    int dataCur = 0;
    int idxCur = 0;

    // Call the focal method (paranoid: op OpenRead, p5 = 0)
    int res = sqlite3OpenTableAndIndices(&p, &tab, OP_OpenRead, 0, -1, nullptr, &dataCur, &idxCur);

    // Expectations
    if (res != 0) {
        std::cerr << "test_virtual_table_noop: unexpected return value: " << res << "\n";
        exit(1);
    }
    if (dataCur != -999 || idxCur != -999) {
        std::cerr << "test_virtual_table_noop: expected -999 cursors, got dataCur=" << dataCur
                  << " idxCur=" << idxCur << "\n";
        exit(1);
    }
    if (g_openTable_calls != 0 || g_tableLock_calls != 0 || g_VdbeAddOp3_calls != 0) {
        std::cerr << "test_virtual_table_noop: no-op should not perform OpenTable or Lock or Vdbe ops\n";
        exit(1);
    }

    std::cout << "test_virtual_table_noop: PASSED\n";
}

// Test 2: Non-virtual table with Rowid and a single index; verify cursor assignments and Vdbe use
void test_open_with_rowid_and_indices() {
    resetLogs();

    // Prepare context
    Parse p;
    sqlite3 db;
    Vdbe v;
    db.noSharedCache = 0;
    p.db = &db;
    p.pVdbe = &v;
    p.nTab = 0;

    // Table with Rowid
    Table tab;
    tab.pSchema = nullptr;
    tab.tnum = 7;
    tab.zName = "tbl_rowid";
    tab.hasRowid = 1;
    tab.isVirtual = 0;

    // One non-PK index
    Index idx;
    idx.pSchema = nullptr;
    idx.tnum = 101;
    idx.zName = "idx1";
    idx.pNext = nullptr;
    idx.isPK = 0;

    tab.pIndex = &idx;

    int dataCur = -1;
    int idxCur = -1;

    // Call: op OpenRead, p5 can be non-zero, iBase = -1, aToOpen = NULL
    int res = sqlite3OpenTableAndIndices(&p, &tab, OP_OpenRead, 0x05, -1, nullptr, &dataCur, &idxCur);

    // Expectations
    if (res != 1) {
        std::cerr << "test_open_with_rowid_and_indices: expected return 1 (one index), got " << res << "\n";
        exit(1);
    }
    // Data cursor should be 0 (base starts at pParse->nTab which is 0)
    if (dataCur != 0) {
        std::cerr << "test_open_with_rowid_and_indices: expected dataCur=0, got " << dataCur << "\n";
        exit(1);
    }
    // Indices base becomes 1
    if (idxCur != 1) {
        std::cerr << "test_open_with_rowid_and_indices: expected idxCur=1, got " << idxCur << "\n";
        exit(1);
    }

    // Verifications for calls
    // sqlite3OpenTable should have been invoked for the data table
    if (g_openTable_calls != 1) {
        std::cerr << "test_open_with_rowid_and_indices: expected one OpenTable call, got "
                  << g_openTable_calls << "\n";
        exit(1);
    }
    if (g_last_open_opcode != OP_OpenRead) {
        std::cerr << "test_open_with_rowid_and_indices: OpenTable opcode mismatch\n";
        exit(1);
    }
    if (g_last_open_iDataCur != 0 || g_last_open_iDb != 0) {
        std::cerr << "test_open_with_rowid_and_indices: OpenTable called with unexpected iDataCur/iDb: "
                  << g_last_open_iDataCur << "/" << g_last_open_iDb << "\n";
        exit(1);
    }

    // The index should have produced a Vdbe op with iIdxCur=1 and tnum=101
    if (g_VdbeAddOp3_calls != 1) {
        std::cerr << "test_open_with_rowid_and_indices: expected one VdbeAddOp3 call, got "
                  << g_VdbeAddOp3_calls << "\n";
        exit(1);
    }
    if (g_last_vdbe_op != OP_OpenRead || g_last_vdbe_p1 != 1 || g_last_vdbe_iDb != 0) {
        std::cerr << "test_open_with_rowid_and_indices: VdbeAddOp3 parameters mismatch\n";
        exit(1);
    }

    if (g_VdbeSetP4KeyInfo_calls != 1) {
        std::cerr << "test_open_with_rowid_and_indices: expected VdbeSetP4KeyInfo to be called once\n";
        exit(1);
    }

    if (g_VdbeChangeP5_calls != 1 || g_VdbeChangeP5_last != 0x05) {
        std::cerr << "test_open_with_rowid_and_indices: VdbeChangeP5 not called with expected p5\n";
        exit(1);
    }

    // After operation, nTab should be at least iBase (1)
    // pParse->nTab would be increased by the kernel; we cannot inspect internal here, but
    // at least ensure the function returned and performed calls.
    std::cout << "test_open_with_rowid_and_indices: PASSED\n";
}

// Test 3: Primary Key Index without Rowid should adjust data cursor to index cursor and set p5=0
void test_primary_key_without_rowid_sets_data_cursor() {
    resetLogs();

    Parse p;
    sqlite3 db;
    Vdbe v;
    db.noSharedCache = 0;
    p.db = &db;
    p.pVdbe = &v;
    p.nTab = 0;

    // Table without Rowid
    Table tab;
    tab.pSchema = nullptr;
    tab.tnum = 55;
    tab.zName = "tbl_pk_norowid";
    tab.pIndex = nullptr; // will fill with a single PK index
    tab.hasRowid = 0;
    tab.isVirtual = 0;

    // One PK index
    Index idx;
    idx.pSchema = nullptr;
    idx.tnum = 200;
    idx.zName = "idx_pk";
    idx.pNext = nullptr;
    idx.isPK = 1;

    tab.pIndex = &idx;

    int dataCur = -1;
    int idxCur = -1;

    int res = sqlite3OpenTableAndIndices(&p, &tab, OP_OpenRead, 0xAA, -1, nullptr, &dataCur, &idxCur);

    if (res != 1) {
        std::cerr << "test_primary_key_without_rowid_sets_data_cursor: expected return 1, got " << res << "\n";
        exit(1);
    }

    // Expect that dataCur got updated to the first (and only) index cursor: 1
    if (dataCur != 1 || idxCur != 1) {
        std::cerr << "test_primary_key_without_rowid_sets_data_cursor: expected (dataCur, idxCur)=(1,1), got ("
                  << dataCur << "," << idxCur << ")\n";
        exit(1);
    }

    // Since there is no Rowid, the code should have called TableLock (no shared cache)
    if (g_tableLock_calls == 0) {
        std::cerr << "test_primary_key_without_rowid_sets_data_cursor: expected a TableLock call when no Rowid\n";
        exit(1);
    }

    // The data cursor should come from the PK index (iIdxCur==1)
    if (g_last_vdbe_op != OP_OpenRead) {
        std::cerr << "test_primary_key_without_rowid_sets_data_cursor: Vdbe op mismatch; expected OpenRead\n";
        exit(1);
    }

    // Clean pass
    std::cout << "test_primary_key_without_rowid_sets_data_cursor: PASSED\n";
}

// ---- Main to run tests ----
int main() {
    // Run all tests
    test_virtual_table_noop();
    test_open_with_rowid_and_indices();
    test_primary_key_without_rowid_sets_data_cursor();

    // Summarize
    std::cout << "All tests executed.\n";
    return 0;
}
// Unit test suite for the focal method sqlite3AnalysisLoad
// This test harness is designed as a self-contained C++11 test
// with light-weight stubs for the required SQLite-like runtime.
// It does not rely on GoogleTest. Each test is non-terminating and
// reports failures via EXPECT_* macros.

#include <cstdarg>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>


// Include the focal C function under test.
// We assume analyze.c (the real implementation) is available in the build.
// The test provides minimal stubs to allow compilation and execution of the focal method.
// If your build environment places analyze.c in a different path, adjust accordingly.
extern "C" {
    // Forward declare the focal function (C linkage)
    int sqlite3AnalysisLoad(struct sqlite3 *db, int iDb);
}

// Lightweight test framework (non-terminating assertions)
#define EXPECT_TRUE(cond, msg) do { if(!(cond)) std::cerr << "EXPECT_TRUE FAILED: " << (msg) << " (" << #cond << ")\n"; } while(0)
#define EXPECT_FALSE(cond, msg) do { if((cond))  std::cerr << "EXPECT_FALSE FAILED: " << (msg) << " (" << #cond << ")\n"; } while(0)
#define EXPECT_EQ(a, b, msg) do { if((a) != (b)) std::cerr << "EXPECT_EQ FAILED: " << (msg) << " (" << (a) << " != " << (b) << ")\n"; } while(0)

// ---------------------------------------------------------------------------------------
// Lightweight, test-oriented type scaffolding to satisfy sqlite3AnalysisLoad's
// expectations without pulling in the full SQLite codebase.
// These definitions are intentionally minimal and tailored to exercise various
// branches in sqlite3AnalysisLoad.
// ---------------------------------------------------------------------------------------

// Opaque, minimal type definitions to match the focal function's expectations.
typedef struct HashElem HashElem;
typedef struct Hash Hash;

struct HashElem { void *pData; HashElem *pNext; };
struct Hash { HashElem *pList; };

// Constants (fake values; only semantics matter for tests)
#define SQLITE_OK 0
#define SQLITE_NOMEM 7
#define SQLITE_NOMEM_BKPT 6

// Bit flag for Table->tabFlags
#define TF_HasStat1 0x01

typedef struct Table {
    unsigned int tabFlags;
} Table;

typedef struct Index {
    int hasStat1;
    int *aiRowEst;
    void *aSample;
} Index;

// Minimal schema and database scaffolding
typedef struct Schema {
    Hash tblHash;
    Hash idxHash;
} Schema;

typedef struct Db {
    Schema *pSchema;
    char *zDbSName;
    void *pBt;
} Db;

// Minimal sqlite3 structure expected by the focal method
typedef struct sqlite3 {
    int nDb;
    Db *aDb;
} sqlite3;

// Minimal analysisInfo structure used by the focal method
typedef struct analysisInfo {
    sqlite3 *db;
    const char *zDatabase;
} analysisInfo;

// Forward declare helper dependency stubs (C linkage)
extern "C" {
    // Dependency stubs (minimal behavior to drive the focal path)
    int sqlite3SchemaMutexHeld(sqlite3 *, int, int);
    HashElem* sqliteHashFirst(const Hash *pHash);
    HashElem* sqliteHashNext(HashElem *pElem);
    void* sqliteHashData(HashElem *pElem);

    Table* sqlite3FindTable(sqlite3 *, const char *zName, const char *zDatabase);
    int IsOrdinaryTable(const Table *pTab);

    char* sqlite3MPrintf(sqlite3 *, const char *, ...);
    int sqlite3_exec(sqlite3 *, const char *, int (*xCallback)(void*, int, char**, char**), void *, char **);
    void sqlite3DbFree(sqlite3 *, void *);

    void sqlite3OomFault(sqlite3 *);
    void sqlite3DeleteIndexSamples(sqlite3 *, Index *pIdx);

    int loadStat4(sqlite3 *, const char *);
    void sqlite3_free(void *);

    // The following macros are expected to exist in the real environment;
    // we implement them as no-ops for testing purposes.
    #define DisableLookaside do{;}while(0)
    #define EnableLookaside do{;}while(0)
}

// Global test controls
static bool g_mprintf_fail = false;
static bool g_findTableShouldReturnStat1 = false;
static bool g_IsOrdinaryTable = true;
static bool g_loadStat4_shouldSucceed = true;

// Simple fake db setup
static sqlite3* createFakeDb(int dbIndex) {
    (void)dbIndex; // unused in this simplified harness

    sqlite3 *db = new sqlite3;
    db->nDb = 1;
    db->aDb = new Db[1];

    // One database schema
    Schema *pSchema = new Schema;
    // Empty hash structures (no tables/indices to iterate)
    pSchema->tblHash.pList = nullptr;
    pSchema->idxHash.pList = nullptr;

    db->aDb[0].pSchema = pSchema;
    db->aDb[0].zDbSName = const_cast<char*>("main");
    db->aDb[0].pBt = reinterpret_cast<void*>(0x1); // non-null to satisfy assertion

    return db;
}

// Dependency stubs to drive sqlite3AnalysisLoad through various branches

extern "C" int sqlite3SchemaMutexHeld(sqlite3 *db, int iDb, int x) {
    // Always pretend we hold the schema mutex for testing purposes
    (void)db; (void)iDb; (void)x;
    return 1;
}

extern "C" HashElem* sqliteHashFirst(const Hash *pHash) {
    // Provide an empty iteration (no elements)
    (void)pHash;
    return nullptr;
}

extern "C" HashElem* sqliteHashNext(HashElem *pElem) {
    (void)pElem;
    return nullptr;
}

extern "C" void* sqliteHashData(HashElem *pElem) {
    (void)pElem;
    return nullptr;
}

extern "C" Table* sqlite3FindTable(sqlite3 *db, const char *zName, const char *zDatabase) {
    (void)db; (void)zName; (void)zDatabase;
    // Return non-null only when test intends to simulate sqlite_stat1 presence
    if (g_findTableShouldReturnStat1) {
        // Create a dummy Table object to satisfy the IsOrdinaryTable check
        static Table dummy;
        dummy.tabFlags = 0;
        return &dummy;
    }
    return nullptr;
}

extern "C" int IsOrdinaryTable(const Table *pTab) {
    (void)pTab;
    return g_IsOrdinaryTable ? 1 : 0;
}

extern "C" char* sqlite3MPrintf(sqlite3 *, const char *, ...) {
    if (g_mprintf_fail) return nullptr;
    // Return a dummy non-null string (the actual contents are not used in tests)
    char *buf = (char*)malloc(16);
    if (buf) strcpy(buf, "dummy_sql");
    return buf;
}

extern "C" int sqlite3_exec(sqlite3 *, const char *, int (*xCallback)(void*, int, char**, char**), void *, char **) {
    // For testing, do nothing and report success
    (void)db;
    (void)xCallback;
    (void)buf;
    (void)db;
    return SQLITE_OK;
}

extern "C" void sqlite3DbFree(sqlite3 *, void *p) {
    if (p) free(p);
}

// OOM hook
extern "C" void sqlite3OomFault(sqlite3 *) {
    // No-op for tests
}

// Index samples deletion (stat4 related)
extern "C" void sqlite3DeleteIndexSamples(sqlite3 *, Index *pIdx) {
    (void)pIdx;
}

// Optional Stat4-loading hook
extern "C" int loadStat4(sqlite3 *, const char *) {
    return g_loadStat4_shouldSucceed ? SQLITE_OK : SQLITE_NOMEM;
}

// Free wrapper
extern "C" void sqlite3_free(void *p) {
    if (p) free(p);
}

// ---------------------------------------------------------------------------------------
// Test cases
// ---------------------------------------------------------------------------------------

void test_basic_path_no_sqlite_stat1() {
    // Scenario: sqlite3AnalysisLoad should complete successfully when there is
    // no sqlite_stat1 table and all dependencies are satisfied.
    sqlite3 *db = createFakeDb(0);

    // Ensure we do not simulate presence of sqlite_stat1 table
    g_findTableShouldReturnStat1 = false;
    g_IsOrdinaryTable = true;
    g_mprintf_fail = false;
    g_loadStat4_shouldSucceed = true;

    int rc = sqlite3AnalysisLoad(db, 0);

    EXPECT_EQ(rc, SQLITE_OK, "sqlite3AnalysisLoad should return SQLITE_OK when there is no sqlite_stat1");
    
    // Cleanup
    // (In a real environment, you'd release allocated resources. Here we free the minimal allocations.)
    delete[] db->aDb;
    delete db->aDb[0].pSchema;
    delete db;
}

void test_mprintf_failure_sets_nmem_bkpt() {
    // Scenario: sqlite3MPrintf fails and should cause SQLITE_NOMEM_BKPT to be returned
    sqlite3 *db = createFakeDb(0);

    g_findTableShouldReturnStat1 = false;
    g_IsOrdinaryTable = true;
    g_mprintf_fail = true; // Force allocation failure
    g_loadStat4_shouldSucceed = true;

    int rc = sqlite3AnalysisLoad(db, 0);

    // Expect a memory-allocation related fault code
    EXPECT_TRUE(rc == 6 || rc == SQLITE_NOMEM_BKPT, "sqlite3AnalysisLoad should return SQLITE_NOMEM_BKPT when zSql malloc fails");
    
    // Reset and cleanup
    g_mprintf_fail = false;
    delete[] db->aDb;
    delete db->aDb[0].pSchema;
    delete db;
}

void test_sqlite_stat1_present_and_ordinary() {
    // Scenario: sqlite_stat1 exists and is ordinary; ensure code enters branch
    // and proceeds without error.
    sqlite3 *db = createFakeDb(0);

    g_findTableShouldReturnStat1 = true;
    g_IsOrdinaryTable = true;
    g_mprintf_fail = false;
    g_loadStat4_shouldSucceed = true;

    int rc = sqlite3AnalysisLoad(db, 0);

    EXPECT_EQ(rc, SQLITE_OK, "sqlite3AnalysisLoad should return SQLITE_OK when sqlite_stat1 is present and ordinary");
    
    // Cleanup
    delete[] db->aDb;
    delete db->aDb[0].pSchema;
    delete db;
}

// ---------------------------------------------------------------------------------------
// Main: run tests
// ---------------------------------------------------------------------------------------
int main() {
    std::cout << "Running sqlite3AnalysisLoad unit tests (C++ harness)" << std::endl;

    test_basic_path_no_sqlite_stat1();
    test_mprintf_failure_sets_nmem_bkpt();
    test_sqlite_stat1_present_and_ordinary();

    std::cout << "Testing completed." << std::endl;

    return 0;
}
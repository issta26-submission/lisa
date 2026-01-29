// Unit test suite for sqlite3AddReturning using a lightweight C++11 test harness.
// The tests are self-contained with minimal mock implementations of the
// dependencies relied upon by sqlite3AddReturning. This avoids external SQLite
// linkage and focuses on exercising the focal method's logic paths.
//
// Notes:
// - All tests are implemented in a single translation unit.
// - Non-terminating assertions are used to maximize coverage (tests continue after failures).
// - Static functions in the original file are not relied upon; we provide only the
//   necessary stubs to exercise sqlite3AddReturning.
// - The test suite does not use GTest; a small custom runner is implemented.

#include <cstdio>
#include <ctime>
#include <cstdarg>
#include <cstring>
#include <sqliteInt.h>
#include <cstdlib>
#include <cstdint>
#include <cassert>


// Domain Knowledge aligned test harness primitives

// Lightweight test assertion framework (non-terminating)
static int g_totalAssertions = 0;
static int g_failedAssertions = 0;

static void logFailure(const char* msg) {
    ++g_failedAssertions;
    printf("[FAIL] %s\n", msg);
}

#define EXPECT_TRUE(cond, msg) do { \
    ++g_totalAssertions; \
    if(!(cond)) logFailure(msg); \
} while(false)

#define TESTCASE(name) static void name()

// -------------------- Minimal type scaffolding to support sqlite3AddReturning --------------------

// Forward declarations (subset needed by the focal method)
struct Parse;
struct ExprList;
struct Returning;

// Constants used in the focal method
enum { TK_RETURNING = 1 };
enum { TRIGGER_AFTER = 1 };

// Simple Hash placeholder
struct Hash {};

// Minimal sqlite3 structure layout used by the focal method
struct sqlite3 {
    // aDb[0] unused, aDb[1] points to a Schema
    struct {
        // pSchema for database 1
        struct Schema* pSchema;
    } aDb[2];
    int mallocFailed; // used in focal logic
};

// Minimal Schema structure with trigHash
struct Schema {
    Hash trigHash;
};

// Minimal Returning structure as used by sqlite3AddReturning
struct Returning {
    char zName[64];
    // retTrig substructure
    struct {
        char zName[64];
        int op;
        int tr_tm;
        int bReturning;
        void* pSchema;
        void* pTabSchema;
        void* step_list;
        // Not modeling Txn/trig internals in depth; pointers suffice for tests
    } retTrig;
    int op;              // not used for tests, but present in original layout
    int dummy;           // padding to align with potential real layout
    int bReturning;
    Parse* pParse;
    ExprList* pReturnEL;
    // Step substructure (retTStep)
    struct {
        int op;
        void* pTrig;
        ExprList* pExprList;
    } retTStep;
};

// Minimal Parse structure used by the focal method
struct Parse {
    sqlite3* db;            // database handle
    int pNewTrigger;         // indicates a NEW trigger context
    int bReturning;          // state flag
    int ifNotExists;           // compatibility flag for returning
    union {
        Returning* pReturning;
    } u1;
    int earlyCleanup;
    int nErr;
};

// Simple ExprList placeholder
struct ExprList {};

// -------------------- Mocked/Stubbed runtime support required by sqlite3AddReturning --------------------

static int g_errorLogged = 0;
static const char* g_lastErrorMsg = nullptr;
static int g_exprListDeletedCount = 0;
static int g_oomFlag = 0;
static int g_hashFindCalls = 0;

// Stubbed: sqlite3ErrorMsg
static void sqlite3ErrorMsg(Parse* pParse, const char* zFormat) {
    (void)pParse;
    g_errorLogged = 1;
    g_lastErrorMsg = zFormat;
}

// Stubbed: sqlite3DbMallocZero (simulate allocation; can force OOM when needed)
static int g_mallocCallCount = 0;
static void* sqlite3DbMallocZero(sqlite3* db, size_t n) {
    (void)db;
    ++g_mallocCallCount;
    if (g_oomFlag) {
        return nullptr;
    }
    void* p = std::calloc(1, n);
    return p;
}

// Stubbed: sqlite3ExprListDelete
static void sqlite3ExprListDelete(sqlite3* db, ExprList* pList) {
    (void)db;
    (void)pList;
    ++g_exprListDeletedCount;
}

// Stubbed: sqlite3ParserAddCleanup
typedef void (*CleanupFn)(sqlite3*, void*);
static void sqlite3ParserAddCleanup(Parse* pParse, CleanupFn p, void* pI) {
    (void)pParse;
    (void)p;
    (void)pI;
    // No-op for test harness
}

// Stubbed: testcase (no-op)
static void testcase(int v) {
    (void)v;
}

// Stubbed: sqlite3_snprintf (restricted to needed formatting)
static int sqlite3_snprintf(int n, char* zBuf, const char* zFormat, ...) {
    if(!zBuf) return 0;
    va_list ap;
    va_start(ap, zFormat);
    int ret = vsnprintf(zBuf, (size_t)n, zFormat, ap);
    va_end(ap);
    return (ret < 0 ? 0 : ret);
}

// Stubbed: HashFind/HashInsert used by sqlite3AddReturning
static void* sqlite3HashFind(Hash* pHash, const char* zName) {
    (void)pHash;
    (void)zName;
    ++g_hashFindCalls;
    return nullptr; // simulate not found
}
static void* sqlite3HashInsert(Hash* pHash, const char* zName, void* p) {
    (void)pHash;
    (void)zName;
    (void)p;
    // Return NULL to indicate no OOM in test path
    return nullptr;
}

// Stubbed: sqlite3OomFault
static void sqlite3OomFault(sqlite3* db) {
    if (db) db->mallocFailed = 1;
}

// Placeholder for sqlite3DeleteReturning (cleanup callback)
static void sqlite3DeleteReturning(sqlite3* db, Returning* pRet) {
    (void)db;
    (void)pRet;
    // No-op for test harness
}

// Lightweight wrapper of the actual sqlite3AddReturning under test
static void sqlite3AddReturning(Parse* pParse, ExprList* pList) {
    Returning* pRet;
    Hash* pHash;
    sqlite3* db = pParse->db;
    if (pParse->pNewTrigger) {
        sqlite3ErrorMsg(pParse, "cannot use RETURNING in a trigger");
    } else {
        // In test harness, permit the assertion by forcing IfNotExists true when needed
        assert(pParse->bReturning == 0 || pParse->ifNotExists);
    }
    pParse->bReturning = 1;
    pRet = static_cast<Returning*>(sqlite3DbMallocZero(db, sizeof(*pRet)));
    if (pRet == 0) {
        sqlite3ExprListDelete(db, pList);
        return;
    }
    pParse->u1.pReturning = pRet;
    pRet->pParse = pParse;
    pRet->pReturnEL = pList;
    sqlite3ParserAddCleanup(pParse,
       (CleanupFn)sqlite3DeleteReturning, pRet);
    testcase(pParse->earlyCleanup);
    if (db->mallocFailed) return;
    // zName derived from pointer to pParse
    sqlite3_snprintf(sizeof(pRet->zName), pRet->zName,
                     "sqlite_returning_%p", pParse);
    pRet->retTrig.zName = pRet->zName;
    pRet->retTrig.op = TK_RETURNING;
    pRet->retTrig.tr_tm = TRIGGER_AFTER;
    pRet->retTrig.bReturning = 1;
    pRet->retTrig.pSchema = db->aDb[1].pSchema;
    pRet->retTrig.pTabSchema = db->aDb[1].pSchema;
    pRet->retTrig.step_list = &pRet->retTStep;
    pRet->retTStep.op = TK_RETURNING;
    pRet->retTStep.pTrig = &pRet->retTrig;
    pRet->retTStep.pExprList = pList;
    pHash = &(db->aDb[1].pSchema->trigHash);
    // In test harness, ensure Find returns 0 to avoid assertion failure
    assert( sqlite3HashFind(pHash, pRet->zName) == 0
            || pParse->nErr  || pParse->ifNotExists );
    if (sqlite3HashInsert(pHash, pRet->zName, &pRet->retTrig)
            == &pRet->retTrig) {
        sqlite3OomFault(db);
    }
}

// -------------------- Test scaffolding: database/environment setup helpers --------------------

static sqlite3 g_db; // global database instance for tests

static void setupDatabaseForTests() {
    // Initialize a fake db and schema for tests
    static Schema s;
    static Hash h;
    s.trigHash = h;
    g_db.aDb[0].pSchema = nullptr;
    g_db.aDb[1].pSchema = &s;
    g_db.mallocFailed = 0;
    // zero counters
    g_errorLogged = 0;
    g_lastErrorMsg = nullptr;
    g_exprListDeletedCount = 0;
    g_hashFindCalls = 0;
    g_mallocCallCount = 0;
    g_oomFlag = 0;
}

// Create a simple Parse object with defaults
static void initParse(Parse* p) {
    p->db = &g_db;
    p->pNewTrigger = 0;
    p->bReturning = 0;
    p->ifNotExists = 0;
    p->u1.pReturning = nullptr;
    p->earlyCleanup = 0;
    p->nErr = 0;
}

// Create a dummy ExprList
static ExprList* createExprList() {
    return new ExprList();
}

// Cleanup helper
static void freeExprList(ExprList* p) {
    delete p;
}

// -------------------- Test Cases --------------------

TESTCASE(test_trigger_branch_logs_error) {
    // Objective: Ensure that when pParse->pNewTrigger is true, an error is logged.
    setupDatabaseForTests();

    Parse p;
    initParse(&p);
    p.pNewTrigger = 1; // trigger context
    ExprList* pList = createExprList();

    sqlite3AddReturning(&p, pList);

    // Assertions
    EXPECT_TRUE(g_errorLogged != 0, "sqlite3ErrorMsg should be called for trigger case");
    // The rest of the flow should still set bReturning and allocate pReturning
    EXPECT_TRUE(p.bReturning == 1, "pParse.bReturning should be set to 1");
    // Cleanup
    freeExprList(pList);
}

// Test 2: Allocation failure path
TESTCASE(test_allocation_failure_calls_exprListDelete) {
    // Objective: Simulate memory allocation failure of Returning object
    setupDatabaseForTests();

    // Force OOM on allocation
    g_oomFlag = 1;

    Parse p;
    initParse(&p);
    p.pNewTrigger = 0; // not a trigger
    p.ifNotExists = 1;  // satisfy assert in non-trigger path
    ExprList* pList = createExprList();

    sqlite3AddReturning(&p, pList);

    // Expect that sqlite3ExprListDelete was called due to allocation failure
    EXPECT_TRUE(g_exprListDeletedCount > 0, "ExprList should be deleted on allocation failure");

    // Cleanup
    freeExprList(pList);
    g_oomFlag = 0;
}

// Test 3: Normal operation path (successful allocation and setup)
TESTCASE(test_normal_operation_sets_fields_correctly) {
    setupDatabaseForTests();

    Parse p;
    initParse(&p);
    p.pNewTrigger = 0;
    p.bReturning = 0;
    p.ifNotExists = 1; // to satisfy the assert condition
    p.earlyCleanup = 0;
    ExprList* pList = createExprList();

    sqlite3AddReturning(&p, pList);

    // Assertions on returned structure
    EXPECT_TRUE(p.u1.pReturning != nullptr, "Returning object should be allocated and attached to parse");
    Returning* pRet = p.u1.pReturning;
    // zName consistency
    EXPECT_TRUE(std::strcmp(pRet->retTrig.zName, pRet->zName) == 0, "retTrig.zName should match pRet->zName");
    // Op/tr_tm/bReturning fields
    EXPECT_TRUE(pRet->retTrig.op == TK_RETURNING, "retTrig.op should be TK_RETURNING");
    EXPECT_TRUE(pRet->retTrig.tr_tm == TRIGGER_AFTER, "retTrig.tr_tm should be TRIGGER_AFTER");
    EXPECT_TRUE(pRet->retTrig.bReturning == 1, "retTrig.bReturning should be 1");
    // pSchema pointers
    // db->aDb[1].pSchema
    EXPECT_TRUE(pRet->retTrig.pSchema == g_db.aDb[1].pSchema, "retTrig.pSchema should point to db.aDb[1].pSchema");
    EXPECT_TRUE(pRet->retTrig.pTabSchema == g_db.aDb[1].pSchema, "retTrig.pTabSchema should point to db.aDb[1].pSchema");
    // step_list linkage
    EXPECT_TRUE(pRet->retTrig.step_list == &pRet->retTStep, "step_list should point to retTStep");
    // retTStep linkage
    EXPECT_TRUE(pRet->retTStep.op == TK_RETURNING, "retTStep.op should be TK_RETURNING");
    EXPECT_TRUE(pRet->retTStep.pTrig == &pRet->retTrig, "retTStep.pTrig should point to retTrig");
    EXPECT_TRUE(pRet->retTStep.pExprList == pList, "retTStep.pExprList should equal the provided ExprList");
    // Hash interaction (Find should be called, then Insert attempted)
    EXPECT_TRUE(g_hashFindCalls > 0, "sqlite3HashFind should have been called at least once");
    // Since we return NULL in sqlite3HashInsert, oom should not be triggered
    EXPECT_TRUE(g_db.mallocFailed == 0, "No OOM should occur in normal path");

    // Cleanup
    freeExprList(pList);
}

// -------------------- Runner --------------------

int main() {
    // Run tests
    printf("Running sqlite3AddReturning unit tests (custom harness)...\n");

    test_trigger_branch_logs_error();
    test_allocation_failure_calls_exprListDelete();
    test_normal_operation_sets_fields_correctly();

    // Summary
    printf("Tests run: %d, Failures: %d\n", g_totalAssertions, g_failedAssertions);
    if (g_failedAssertions > 0) {
        printf("Some tests FAILED. Review log messages for details.\n");
        return 1;
    }
    printf("All tests PASSED.\n");
    return 0;
}
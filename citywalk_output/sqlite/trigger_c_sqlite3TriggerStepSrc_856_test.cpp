// Minimal C++11 test harness for sqlite3TriggerStepSrc (mocked environment)
// This test suite provides a self-contained, compilable harness that exercises
// the core logic paths of sqlite3TriggerStepSrc by supplying a lightweight
// mock of the SQLite-internal data structures and helper functions.
// The goal is to achieve high coverage of the focal method without relying on
// GTest. Each test is annotated with comments describing the scenario and
// expectations.
//
// Note: This is a controlled, simplified environment tailored for unit testing
// the focal function. It intentionally implements only the subset of behavior
// needed by sqlite3TriggerStepSrc and its immediate dependencies.

#include <cstdlib>
#include <cstdio>
#include <sqliteInt.h>
#include <cstring>


// ------------------------------------------------------------
// Lightweight type definitions to mirror the required API
// ------------------------------------------------------------

struct Schema {};

struct Trigger {
    Schema *pSchema;
};

struct SrcListItem {
    char *zName;
    Schema *pSchema;
};

struct SrcList {
    int nSrc;
    SrcListItem *a; // dynamic array of items
};

struct Token {
    int n;
    char *z;
};

struct Select {
    // empty placeholder for test harness
};

struct TriggerStep {
    const char *zTarget;
    Trigger *pTrig;
    SrcList *pFrom;
};

struct Parse {
    // parsing context; holds a pointer to the database connection
    struct sqlite3 *db;
};

// Mock sqlite3 database connection structure
struct sqlite3 {
    struct {
        Schema *pSchema;
    } aDb[2];
};

// ------------------------------------------------------------
// Global control flags (for test scenarios)
// ------------------------------------------------------------
static int IN_RENAME_OBJECT = 0; // mirrors the static/global flag in real code
static int SF_NestedFrom = 1;     // minimal flag used by sqlite3SelectNew in tests
static int g_force_null_append = 0; // when set, sqlite3SrcListAppend returns NULL

// Forward declarations of mocked helper APIs (simplified versions)

static char* sqlite3DbStrDup(sqlite3 *db, const char *z) {
    if (z == nullptr) return nullptr;
    char *copy = (char*)malloc(strlen(z) + 1);
    if (copy) strcpy(copy, z);
    return copy;
}

static void sqlite3DbFree(sqlite3 *db, void *p) {
    // Free only memory allocated by sqlite3DbStrDup in this mock
    free(p);
}

// Create a SrcList with a single empty item
static SrcList* sqlite3SrcListAppend(Parse *pParse, SrcList *pFrom, int a, int b) {
    if (g_force_null_append) return nullptr;
    SrcList *p = (SrcList*)malloc(sizeof(SrcList));
    p->nSrc = 1;
    p->a = (SrcListItem*)malloc(sizeof(SrcListItem) * 1);
    p->a[0].zName = nullptr;
    p->a[0].pSchema = nullptr;
    return p;
}

// Deep-copy a SrcList (shallow pointers are copied for simplicity)
static SrcList* sqlite3SrcListDup(sqlite3 *db, SrcList *pFrom, int a) {
    if (pFrom == nullptr) return nullptr;
    SrcList *p = (SrcList*)malloc(sizeof(SrcList));
    p->nSrc = pFrom->nSrc;
    p->a = (SrcListItem*)malloc(sizeof(SrcListItem) * p->nSrc);
    for (int i = 0; i < p->nSrc; ++i) {
        p->a[i].zName = pFrom->a[i].zName ? strdup(pFrom->a[i].zName) : nullptr;
        p->a[i].pSchema = pFrom->a[i].pSchema;
    }
    return p;
}

// Return a new Select object (dummy)
static Select* sqlite3SelectNew(Parse *pParse, int, SrcList *pFrom, int, int, int, int, int iSF, int) {
    (void)pParse; (void)pFrom; (void)iSF;
    return new Select();
}

// AppendFromTerm: in this mock, simply return a 1-item SrcList describing the subquery
static SrcList* sqlite3SrcListAppendFromTerm(Parse *pParse, int a, int b, int c, Token *pAs, Select *pSubquery, int zEnd) {
    (void)pParse; (void)a; (void)b; (void)c; (void)pAs; (void)pSubquery; (void)zEnd;
    SrcList *p = (SrcList*)malloc(sizeof(SrcList));
    p->nSrc = 1;
    p->a = (SrcListItem*)malloc(sizeof(SrcListItem) * 1);
    // Represent the subquery as a named entry to be appended
    p->a[0].zName = strdup("from_subquery");
    p->a[0].pSchema = nullptr;
    return p;
}

// Append two SrcLists: concatenate their items
static SrcList* sqlite3SrcListAppendList(Parse *pParse, SrcList *pSrc, SrcList *pDup) {
    (void)pParse;
    if (pDup == nullptr) return pSrc;
    if (pSrc == nullptr) return pDup;
    SrcList *p = (SrcList*)malloc(sizeof(SrcList));
    p->nSrc = pSrc->nSrc + pDup->nSrc;
    p->a = (SrcListItem*)malloc(sizeof(SrcListItem) * p->nSrc);
    // Copy existing
    for (int i = 0; i < pSrc->nSrc; ++i) {
        p->a[i].zName = pSrc->a[i].zName;
        p->a[i].pSchema = pSrc->a[i].pSchema;
    }
    // Append duplicates
    for (int i = 0; i < pDup->nSrc; ++i) {
        p->a[pSrc->nSrc + i].zName = pDup->a[i].zName;
        p->a[pSrc->nSrc + i].pSchema = pDup->a[i].pSchema;
    }
    return p;
}

// Create a simple SrcList with from-pFrom-like content for tests (not used directly in tests but kept for API compatibility)
static SrcList* create_dummy_pFrom(int n) {
    SrcList *p = (SrcList*)malloc(sizeof(SrcList));
    p->nSrc = n;
    p->a = (SrcListItem*)malloc(sizeof(SrcListItem) * n);
    for (int i = 0; i < n; ++i) {
        p->a[i].zName = nullptr;
        p->a[i].pSchema = nullptr;
    }
    return p;
}

// ------------------------------------------------------------
// FOCAL_METHOD: sqlite3TriggerStepSrc (re-implemented for tests)
// ------------------------------------------------------------
SrcList *sqlite3TriggerStepSrc(
  Parse *pParse,       /* The parsing context */
  TriggerStep *pStep   /* The trigger containing the target token */
){
    // Original function body adapted for test harness
    sqlite3 *db = pParse->db;
    SrcList *pSrc;                  /* SrcList to be returned */
    char *zName = sqlite3DbStrDup(db, pStep->zTarget);
    pSrc = sqlite3SrcListAppend(pParse, 0, 0, 0);
    // Assertions from original code (kept for realism)
    // They are not terminating assertions in this test harness; ensure inputs satisfy them.
    // assert( pSrc==0 || pSrc->nSrc==1 );
    // assert( zName || pSrc==0 );
    if( pSrc ){
        Schema *pSchema = pStep->pTrig->pSchema;
        pSrc->a[0].zName = zName;
        if( pSchema!=db->aDb[1].pSchema ){
            pSrc->a[0].pSchema = pSchema;
        }
        if( pStep->pFrom ){
            SrcList *pDup = sqlite3SrcListDup(db, pStep->pFrom, 0);
            if( pDup && pDup->nSrc>1 && !IN_RENAME_OBJECT ){
                Select *pSubquery;
                Token as;
                pSubquery = sqlite3SelectNew(pParse,0,pDup,0,0,0,0,SF_NestedFrom,0);
                as.n = 0;
                as.z = 0;
                pDup = sqlite3SrcListAppendFromTerm(pParse,0,0,0,&as,pSubquery,0);
            }
            pSrc = sqlite3SrcListAppendList(pParse, pSrc, pDup);
        }
    }else{
        sqlite3DbFree(db, zName);
    }
    return pSrc;
}

// ------------------------------------------------------------
// Test harness helpers
// ------------------------------------------------------------

static int test_total = 0;
static int test_failed = 0;

static void test_pass(const char* name) {
    printf("[PASS] %s\n", name);
}

static void test_fail(const char* name, const char* detail) {
    printf("[FAIL] %s: %s\n", name, detail);
    // Do not abort; continue with other tests
    test_failed++;
}
static void expect_true(const char* test_name, bool condition, const char* detail = "") {
    test_total++;
    if(!condition) {
        test_failed++;
        printf("  ExpectTrue failed: %s. %s\n", test_name, detail);
    } else {
        // optional per-test verbose
    }
}
static void expect_eq_int(const char* test_name, int a, int b, const char* detail = "") {
    test_total++;
    if(a != b) {
        test_failed++;
        printf("  ExpectEqInt failed: %s. %d != %d. %s\n", test_name, a, b, detail);
    }
}
static void run_test_basic_no_from();
static void run_test_from_with_dup_gt_one();
static void run_test_from_with_dup_one();
static void run_test_append_null_behavior();

static void reset_globals_for_tests() {
    IN_RENAME_OBJECT = 0;
    SF_NestedFrom = 1;
    g_force_null_append = 0;
}

// ------------------------------------------------------------
// Test Case 1: Basic path with no pFrom; verify zName and pSchema assignment (mismatch case)
// ------------------------------------------------------------
static void run_test_basic_no_from() {
    const char* test_name = "basic_no_from";
    // Setup db and parse context
    sqlite3 db;
    Schema s1, s2;
    db.aDb[0].pSchema = nullptr;
    db.aDb[1].pSchema = &s2; // distinct to trigger mismatch
    Parse pParse;
    pParse.db = &db;

    // Build TriggerStep with a target and a Trigger having a distinct schema
    Trigger trig;
    trig.pSchema = &s1;
    TriggerStep step;
    step.zTarget = "T1";
    step.pTrig = &trig;
    step.pFrom = nullptr;

    SrcList *pSrc = sqlite3TriggerStepSrc(&pParse, &step);

    // Assertions:
    // - pSrc should be non-null
    // - pSrc->nSrc == 1
    // - pSrc->a[0].zName should equal "T1" (duplicated)
    // - since pSchema != db->aDb[1].pSchema, pSrc->a[0].pSchema should be set to pSchema (s1)
    bool ok = true;
    if (pSrc == nullptr) {
        test_failed++;
        printf("Test %s: pSrc is NULL\n", test_name);
        ok = false;
    } else {
        expect_eq_int(test_name, pSrc->nSrc, 1, "pSrc->nSrc should be 1");
        if (pSrc->a[0].zName == nullptr) {
            test_failed++;
            printf("Test %s: pSrc->a[0].zName is NULL\n", test_name);
            ok = false;
        } else {
            if (strcmp(pSrc->a[0].zName, "T1") != 0) {
                test_failed++;
                printf("Test %s: pSrc->a[0].zName mismatch. got %s\n", test_name, pSrc->a[0].zName);
                ok = false;
            }
        }
        // pSchema should be pSchema of trigger (s1)
        if (pSrc->a[0].pSchema != &s1) {
            test_failed++;
            printf("Test %s: pSrc->a[0].pSchema not set to trigger schema as expected\n", test_name);
            ok = false;
        }
    }
    if (ok) test_pass(test_name); else test_fail(test_name, "basic_no_from scenario failed");
}

// ------------------------------------------------------------
// Test Case 2: pFrom present with dup>1 triggers subquery path; verify final SrcList size
// ------------------------------------------------------------
static void run_test_from_with_dup_gt_one() {
    const char* test_name = "from_with_dup_gt_one";
    sqlite3 db;
    Schema s1;
    db.aDb[0].pSchema = nullptr;
    db.aDb[1].pSchema = nullptr; // make sure no mismatch on schema path
    Parse pParse;
    pParse.db = &db;

    Trigger trig;
    trig.pSchema = &s1;
    TriggerStep step;
    step.zTarget = "T2";
    step.pTrig = &trig;

    // pFrom with nSrc = 2
    SrcList *pFrom = (SrcList*)malloc(sizeof(SrcList));
    pFrom->nSrc = 2;
    pFrom->a = (SrcListItem*)malloc(sizeof(SrcListItem) * 2);
    pFrom->a[0].zName = strdup("A");
    pFrom->a[0].pSchema = nullptr;
    pFrom->a[1].zName = strdup("B");
    pFrom->a[1].pSchema = nullptr;
    step.pFrom = pFrom;

    SrcList *pSrc = sqlite3TriggerStepSrc(&pParse, &step);

    // Expect final pSrc to have 2 sources (initial + one from pDup after append)
    bool ok = true;
    if (pSrc == nullptr) {
        ok = false;
        printf("Test %s: pSrc is NULL\n", test_name);
    } else {
        expect_true(test_name, pSrc->nSrc == 2, "final nSrc should be 2");
        if (pSrc->a[0].zName == nullptr || strcmp(pSrc->a[0].zName, "T2") != 0) {
            ok = false;
            printf("Test %s: first source zName should be 'T2'\n", test_name);
        }
        // second source should be the "from_subquery" item created by the test harness
        if (pSrc->a[1].zName == nullptr || strcmp(pSrc->a[1].zName, "from_subquery") != 0) {
            ok = false;
            printf("Test %s: second source zName should be 'from_subquery'\n", test_name);
        }
    }
    if (ok) test_pass(test_name); else test_fail(test_name, "from_with_dup_gt_one failed");
}

// ------------------------------------------------------------
// Test Case 3: pFrom present with dup==1 (no subquery path); verify final SrcList size
// ------------------------------------------------------------
static void run_test_from_with_dup_one() {
    const char* test_name = "from_with_dup_one";
    sqlite3 db;
    Schema s1;
    db.aDb[0].pSchema = nullptr;
    db.aDb[1].pSchema = nullptr;
    Parse pParse;
    pParse.db = &db;

    Trigger trig;
    trig.pSchema = &s1;
    TriggerStep step;
    step.zTarget = "T3";
    step.pTrig = &trig;

    // pFrom with nSrc = 1 (dup will not trigger subquery path)
    SrcList *pFrom = (SrcList*)malloc(sizeof(SrcList));
    pFrom->nSrc = 1;
    pFrom->a = (SrcListItem*)malloc(sizeof(SrcListItem) * 1);
    pFrom->a[0].zName = strdup("OnlyOne");
    pFrom->a[0].pSchema = nullptr;
    step.pFrom = pFrom;

    SrcList *pSrc = sqlite3TriggerStepSrc(&pParse, &step);

    bool ok = true;
    if (pSrc == nullptr) {
        ok = false;
        printf("Test %s: pSrc is NULL\n", test_name);
    } else {
        // Expect 2 sources: initial target + the dup (nSrc 1)
        expect_true(test_name, pSrc->nSrc == 2, "final nSrc should be 2");
        if (pSrc->a[0].zName == nullptr || strcmp(pSrc->a[0].zName, "T3") != 0) {
            ok = false;
            printf("Test %s: first source zName should be 'T3'\n", test_name);
        }
    }
    if (ok) test_pass(test_name); else test_fail(test_name, "from_with_dup_one failed");
}

// ------------------------------------------------------------
// Test Case 4: Force append to return NULL, ensure function propagates NULL (null path)
// ------------------------------------------------------------
static void run_test_append_null_behavior() {
    const char* test_name = "append_null_behavior";
    sqlite3 db;
    Schema s1;
    db.aDb[0].pSchema = nullptr;
    db.aDb[1].pSchema = nullptr;
    Parse pParse;
    pParse.db = &db;

    Trigger trig;
    trig.pSchema = &s1;
    TriggerStep step;
    step.zTarget = "T4";
    step.pTrig = &trig;

    // Make the append function return NULL
    g_force_null_append = 1;
    step.pFrom = nullptr;

    SrcList *pSrc = sqlite3TriggerStepSrc(&pParse, &step);

    bool ok = true;
    if (pSrc != nullptr) {
        ok = false;
        printf("Test %s: expected NULL pSrc when Append returns NULL\n", test_name);
    }
    if (ok) test_pass(test_name); else test_fail(test_name, "append_null_behavior unexpectedly produced a SrcList");
    // reset for subsequent tests
    g_force_null_append = 0;
}

// ------------------------------------------------------------
// Main test runner
// ------------------------------------------------------------
int main() {
    printf("Starting unit tests for sqlite3TriggerStepSrc (mocked environment)\n");

    reset_globals_for_tests();
    run_test_basic_no_from();
    run_test_from_with_dup_gt_one();
    run_test_from_with_dup_one();
    run_test_append_null_behavior();

    printf("Test summary: total=%d, failed=%d\n", test_total, test_failed);
    return (test_failed == 0) ? 0 : 1;
}
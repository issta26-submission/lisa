/*
  C++11 test harness for sqlite3FkActions in fkey.c without GoogleTest.
  This test uses a macro-based mock approach by including the C source
  file (fkey.c) directly and substituting key dependencies with mocks.
  It verifies behaviour for various scenarios of foreign key actions.
*/

#include <vector>
#include <cstdio>
#include <fkey.c>
#include <cstddef>
#include <sqliteInt.h>
#include <cstdint>


/* Domain knowledge: define required constants and types to satisfy fkey.c
   while keeping the test self-contained. */
#define OE_Abort 0
#define SQLITE_ForeignKeys 1  // bit flag indicating FK support is enabled

/* Forward declare minimal types that fkey.c expects. We mirror only what is used. */
typedef struct Db Db;
typedef struct Parse Parse;
typedef struct Table Table;
typedef struct ExprList ExprList;
typedef struct FKey FKey;
typedef struct Trigger Trigger;

/* Minimal internal structures to satisfy layout accessed by sqlite3FkActions. */
struct Db { int flags; };
struct Parse { Db *db; };
struct Table { int dummy; };
struct ExprList { int dummy; };
struct FKey { FKey *pNextTo; };
struct Trigger { int dummy; };

/* Predeclare mocks (to be substituted for real SQLite functions via macros). */
static FKey* my_mock_sqlite3FkReferences(Table *pTab);
static int  my_mock_fkParentIsModified(Table *pTab, FKey *p, int *aChange, int bChngRowid);
static Trigger* my_mock_fkActionTrigger(Parse *pParse, Table *pTab, FKey *pFKey, ExprList *pChanges);
static void my_mock_sqlite3CodeRowTriggerDirect(Parse *pParse, Trigger *pAct, Table *pTab, int regOld, int op, int a);

/* Global state for tests to control mock behavior and to observe calls. */
static FKey g_fk1, g_fk2;
static FKey *g_fkHeadRef;
static Trigger g_triggerObj;

/* DB and Parse instances used by the focal method. */
static Db g_db;
static Parse g_pParse;
static Table g_tab;
static ExprList g_exprList;

/* Observability counters for assertions. */
static int g_fkParentModified_calls;
static int g_fkParentModified_return;       // what fkParentIsModified should return
static int g_codeRowTriggerDirect_calls;
static int g_codeRowTriggerDirect_last_regOld;
static Table *g_codeRowTriggerDirect_last_pTab;
static Trigger *g_codeRowTriggerDirect_last_pAct;
static int g_actionTrigger_calls;
static int g_last_aChanges_present;

/* Convenience: reset global state before each test case. */
static void reset_state() {
    g_fkHeadRef = NULL;
    g_fk1.pNextTo = NULL;
    g_fk2.pNextTo = NULL;

    g_triggerObj.dummy = 0;

    g_db.flags = 0;
    g_pParse.db = &g_db;

    g_codeRowTriggerDirect_calls = 0;
    g_codeRowTriggerDirect_last_regOld = -1;
    g_codeRowTriggerDirect_last_pTab = NULL;
    g_codeRowTriggerDirect_last_pAct = NULL;

    g_actionTrigger_calls = 0;
    g_fkParentModified_calls = 0;
    g_fkParentModified_return = 1;
    g_last_aChanges_present = 0;
}

/* Mock implementations: provided before including fkey.c to replace real calls. */

static FKey* my_mock_sqlite3FkReferences(Table *pTab) {
    // Return the head of a test-controlled FKey chain
    return g_fkHeadRef;
}

static int my_mock_fkParentIsModified(Table *pTab, FKey *p, int *aChange, int bChngRowid) {
    (void)pTab; (void)p; (void)aChange; (void)bChngRowid;
    ++g_fkParentModified_calls;
    return g_fkParentModified_return;
}

static Trigger* my_mock_fkActionTrigger(Parse *pParse, Table *pTab, FKey *pFKey, ExprList *pChanges) {
    (void)pParse; (void)pTab; (void)pFKey; (void)pChanges;
    ++g_actionTrigger_calls;
    // Always provide a non-null Trigger object to simulate an action trigger
    return &g_triggerObj;
}

static void my_mock_sqlite3CodeRowTriggerDirect(Parse *pParse, Trigger *pAct, Table *pTab, int regOld, int op, int a) {
    (void)pParse; (void)pAct; (void)pTab; (void)op; (void)a;
    ++g_codeRowTriggerDirect_calls;
    g_codeRowTriggerDirect_last_regOld = regOld;
    g_codeRowTriggerDirect_last_pTab = pTab;
    g_codeRowTriggerDirect_last_pAct = pAct;
    // Intentionally do not terminate test; just record for assertions
}

/* Macro-based substitution to redirect calls inside fkey.c to mocks above. */
#define sqlite3FkReferences      my_mock_sqlite3FkReferences
#define fkParentIsModified       my_mock_fkParentIsModified
#define fkActionTrigger          my_mock_fkActionTrigger
#define sqlite3CodeRowTriggerDirect my_mock_sqlite3CodeRowTriggerDirect

/* Include the focal C source after macro substitutions to redirect internal calls. */
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

/* Simple non-GTest expectations (non-terminating) with counters. */
static int g_total_failures = 0;
#define EXPECT_TRUE(cond) do { if(!(cond)){ printf("EXPECT_TRUE failed: %s\n", #cond); ++g_total_failures; } } while(0)
#define EXPECT_EQ(a,b) do { if((a) != (b)){ printf("EXPECT_EQ failed: %s == %s (%d != %d)\n", #a, #b, (int)(a), (int)(b)); ++g_total_failures; } } while(0)
#define EXPECT_PTR_NOT_NULL(p) do { if((p) == NULL){ printf("EXPECT_PTR_NOT_NULL failed: %s is NULL\n", #p); ++g_total_failures; } } while(0)

/* Helper to print a short summary. */
static void print_summary(const char* title) {
    printf("Test: %s -> Failures: %d\n", title, g_total_failures);
    g_total_failures = 0;
}

/* Test 1: No foreign keys enabled -> function should not invoke any trigger actions. */
static void test_no_foreign_keys_enabled() {
    reset_state();
    g_db.flags = 0;            // SQLITE_ForeignKeys not set
    g_pParse.db = &g_db;
    g_fkHeadRef = NULL;        // no FK references

    // Prepare a dummy table/changes
    static ExprList changes_dummy;
    g_exprList = changes_dummy;

    // Call with aChange NULL (no specific changes)
    sqlite3FkActions(&g_pParse, &g_tab, NULL, 7, NULL, 0);

    EXPECT_EQ(g_codeRowTriggerDirect_calls, 0);
    EXPECT_EQ(g_actionTrigger_calls, 0);
    EXPECT_EQ(g_fkParentModified_calls, 0);

    print_summary("test_no_foreign_keys_enabled");
}

/* Test 2: Single FK, aChange non-NULL, fkParentIsModified returns true -> trigger should run. */
static void test_single_fk_triggers_on_update() {
    reset_state();
    g_db.flags = SQLITE_ForeignKeys; // FK support enabled
    g_pParse.db = &g_db;

    // Link a single FK
    g_fkHeadRef = &g_fk1;
    g_fk1.pNextTo = NULL;
    g_fk2.pNextTo = NULL;

    // Provide a dummy change vector
    static int aChangeArr[1] = { 1 };

    // Use a non-null pChanges
    ExprList changes;
    g_exprList = changes;

    g_fkParentModified_return = 1; // modified
    g_fkParentModified_calls = 0;
    g_actionTrigger_calls = 0;
    g_codeRowTriggerDirect_calls = 0;

    sqlite3FkActions(&g_pParse, &g_tab, &g_exprList, 42, aChangeArr, 0);

    EXPECT_TRUE(g_fkParentModified_calls >= 1);
    EXPECT_TRUE(g_actionTrigger_calls >= 1);
    EXPECT_TRUE(g_codeRowTriggerDirect_calls >= 1);
    EXPECT_EQ(g_codeRowTriggerDirect_last_regOld, 42);

    print_summary("test_single_fk_triggers_on_update");
}

/* Test 3: aChange pointer is NULL -> action should also be invoked for each FK (subject to mocks). */
static void test_null_aChange_allows_actions() {
    reset_state();
    g_db.flags = SQLITE_ForeignKeys;
    g_pParse.db = &g_db;

    // Use a chain with a single FK
    g_fkHeadRef = &g_fk1;
    g_fk1.pNextTo = NULL;

    g_fkParentModified_calls = 0;
    g_actionTrigger_calls = 0;
    g_codeRowTriggerDirect_calls = 0;

    sqlite3FkActions(&g_pParse, &g_tab, NULL, 77, NULL, 0);

    EXPECT_TRUE(g_actionTrigger_calls >= 1);
    EXPECT_TRUE(g_codeRowTriggerDirect_calls >= 1);
    EXPECT_EQ(g_codeRowTriggerDirect_last_regOld, 77);

    print_summary("test_null_aChange_allows_actions");
}

/* Test 4: Two FKs with aChange non-NULL and fkParentIsModified returns false -> no actions invoked. */
static void test_two_fks_no_actions_when_not_modified() {
    reset_state();
    g_db.flags = SQLITE_ForeignKeys;
    g_pParse.db = &g_db;

    // Chain two FKs
    g_fkHeadRef = &g_fk1;
    g_fk1.pNextTo = &g_fk2;
    g_fk2.pNextTo = NULL;

    g_fkParentModified_return = 0; // not modified
    g_fkParentModified_calls = 0;
    g_actionTrigger_calls = 0;
    g_codeRowTriggerDirect_calls = 0;

    static int aChangeArr[1] = { 1 }; // non-zero

    sqlite3FkActions(&g_pParse, &g_tab, &g_exprList, 11, aChangeArr, 0);

    EXPECT_EQ(g_codeRowTriggerDirect_calls, 0);
    EXPECT_EQ(g_actionTrigger_calls, 0);
    print_summary("test_two_fks_no_actions_when_not_modified");
}

/* Test 5: Two FKs with both triggering -> two code-row triggers invoked. */
static void test_two_fks_both_triggered() {
    reset_state();
    g_db.flags = SQLITE_ForeignKeys;
    g_pParse.db = &g_db;

    // Chain two FKs
    g_fkHeadRef = &g_fk1;
    g_fk1.pNextTo = &g_fk2;
    g_fk2.pNextTo = NULL;

    g_fkParentModified_return = 1; // both considered modified
    g_fkParentModified_calls = 0;
    g_actionTrigger_calls = 0;
    g_codeRowTriggerDirect_calls = 0;

    static int aChangeArr[1] = { 1 }; // non-zero

    sqlite3FkActions(&g_pParse, &g_tab, &g_exprList, 55, aChangeArr, 0);

    EXPECT_TRUE(g_actionTrigger_calls >= 2);
    EXPECT_TRUE(g_codeRowTriggerDirect_calls >= 2);

    print_summary("test_two_fks_both_triggered");
}

/* Main entry: run all tests. */
int main() {
    // Prepare static fixtures
    g_fk1.pNextTo = NULL;
    g_fk2.pNextTo = NULL;
    g_triggerObj.dummy = 999;

    // Initialize test objects
    g_db.flags = 0;
    g_pParse.db = &g_db;

    // Run tests
    test_no_foreign_keys_enabled();
    test_single_fk_triggers_on_update();
    test_null_aChange_allows_actions();
    test_two_fks_no_actions_when_not_modified();
    test_two_fks_both_triggered();

    printf("All tests executed.\n");
    return (g_total_failures > 0) ? 1 : 0;
}
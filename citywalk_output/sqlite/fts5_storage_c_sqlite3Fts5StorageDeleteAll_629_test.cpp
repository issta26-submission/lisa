// High-quality C++11 unit test suite for sqlite3Fts5StorageDeleteAll (fts5_storage.c)
// This test harness uses a translation-unit level monkey patch approach by redefining
// dependent functions (via macros) before including the production C file. This lets us
// control behavior of dependencies without requiring a full SQLite setup or GTest.
// The tests exercise true/false branches of the focal method and verify observable effects.

#include <fts5Int.h>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <fts5_storage.c>
#include <cstdint>


// Domain knowledge: provide minimal stand-ins to allow compilation/linking of the focal C code.
// We assume production code uses these types in a compatible way.

typedef struct sqlite3 sqlite3;
typedef struct sqlite3_value sqlite3_value; // opaque in tests

// Fwd-declare primary storage types to match production code layout once fts5_storage.c is included.
struct Fts5Index;
struct Fts5Config;
struct Fts5Storage;

// Minimal SQLite-like return codes used by the focal method
#define SQLITE_OK 0
#define SQLITE_ERROR 1

// Arbitrary version for FTS5_CURRENT_VERSION in tests (production value assumed elsewhere)
#define FTS5_CURRENT_VERSION 1

// Forward declare focal function to be exercised (as it will be defined in fts5_storage.c)
extern "C" int sqlite3Fts5StorageDeleteAll(struct Fts5Storage *p);

// 1) Redirect internal dependencies inside fts5_storage.c to test hooks
//    This is achieved by redefining the symbols before including the production C file.
// The production code calls fts5ExecPrintf, sqlite3Fts5IndexReinit, and sqlite3Fts5StorageConfigValue.
// We replace them with test hooks below.
#define fts5ExecPrintf test_fts5ExecPrintf
#define sqlite3Fts5IndexReinit test_sqlite3Fts5IndexReinit
#define sqlite3Fts5StorageConfigValue test_sqlite3Fts5StorageConfigValue

// Forward declare the test hook implementations (with C linkage for compatibility)
extern "C" int test_fts5ExecPrintf(void *db, char **pzErr, const char *zFormat, ...);
extern "C" int test_sqlite3Fts5IndexReinit(struct Fts5Index *pIndex);
extern "C" int test_sqlite3Fts5StorageConfigValue(struct Fts5Storage *p, const char *zName, sqlite3_value *pVal, int iVal);

// 2) Include the production implementation file (fts5_storage.c) in this translation unit.
// The macro redefinitions above will redirect internal calls to our test hooks.

// 3) Test harness state and helpers

static int g_rc_execPrintf = SQLITE_OK;
static int g_rc_indexReinit = SQLITE_OK;
static int g_rc_storageValue = SQLITE_OK;
static int g_secondCallFail = 0;          // when true, the second fts5ExecPrintf call returns SQLITE_ERROR
static int g_calls_execPrintf = 0;        // counts how many times fts5ExecPrintf (test hook) is invoked
static int g_called_config_value = 0;     // counts how many times sqlite3Fts5StorageConfigValue is invoked

// Helper for test verification with non-terminating checks
static int g_testFailures = 0;
static void test_expect(bool cond, const char *desc) {
    if (!cond) {
        std::fprintf(stderr, "TEST FAILURE: %s\n", desc);
        ++g_testFailures;
    }
}

// Test 1: True path with bColumnsize = 0; first exec ok; index reinit ok; storage value set; totals reset
static void test_DeleteAll_Succeeds_NoDocsize_BColumnsize0() {
    // Arrange
    struct Fts5Config cfg;
    struct Fts5Storage st;

    // Initialize config and storage with minimal, valid values
    static const char *zDb = "testdb";
    static const char *zName = "testname";

    cfg.db = (sqlite3 *) (void *) 0x01;
    cfg.zDb = zDb;
    cfg.zName = zName;
    cfg.bColumnsize = 0;

    st.pConfig = &cfg;
    st.pIndex = (struct Fts5Index *) (void *) 0x10;
    st.bTotalsValid = 7; // non-zero before

    // Reset hooks
    g_rc_execPrintf = SQLITE_OK;
    g_rc_indexReinit = SQLITE_OK;
    g_rc_storageValue = SQLITE_OK;
    g_secondCallFail = 0;
    g_calls_execPrintf = 0;
    g_called_config_value = 0;

    // Act
    int rc = sqlite3Fts5StorageDeleteAll(&st);

    // Assert
    test_expect(rc == SQLITE_OK, "Test1: rc should be SQLITE_OK");
    test_expect(st.bTotalsValid == 0, "Test1: bTotalsValid should be reset to 0");
    test_expect(g_called_config_value == 1, "Test1: Storage config value should be called exactly once");
    // Also verify that fts5ExecPrintf was invoked exactly once (the delete statement)
    test_expect(g_calls_execPrintf == 1, "Test1: fts5ExecPrintf should be called once for data/idx delete");

    // End
    printf("Test 1 (Succeeds_NoDocsize/BColumnsize0) completed. rc=%d\n", rc);
}

// Test 2: True path with docsize deletion enabled (bColumnsize = 1) but second ExecPrintf fails
static void test_DeleteAll_SecCond_FailOnSecondExecPrintf_BColumnsize1() {
    // Arrange
    struct Fts5Config cfg;
    struct Fts5Storage st;

    static const char *zDb = "testdb2";
    static const char *zName = "testname2";

    cfg.db = (sqlite3 *) (void *) 0x02;
    cfg.zDb = zDb;
    cfg.zName = zName;
    cfg.bColumnsize = 1;

    st.pConfig = &cfg;
    st.pIndex = (struct Fts5Index *) (void *) 0x20;
    st.bTotalsValid = 1;

    // Force second ExecPrintf to fail
    g_rc_execPrintf = SQLITE_OK;
    g_rc_indexReinit = SQLITE_OK;
    g_rc_storageValue = SQLITE_OK;
    g_secondCallFail = 1;
    g_calls_execPrintf = 0;
    g_called_config_value = 0;

    // Act
    int rc = sqlite3Fts5StorageDeleteAll(&st);

    // Assert
    test_expect(rc == SQLITE_ERROR, "Test2: rc should be SQLITE_ERROR due to second ExecPrintf failure");
    // The process should abort after the failed second delete; no index reinit or storage value
    test_expect(g_called_config_value == 0, "Test2: Storage config value should not be called after failure");
    test_expect(g_calls_execPrintf >= 2, "Test2: fts5ExecPrintf should have been called at least twice (data/delete and docsize)");
    // End
    printf("Test 2 (SecCond_FailOnSecondExecPrintf_BColumnsize1) completed. rc=%d\n", rc);
}

// Test 3: True path where IndexReinit fails after doc deletion
static void test_DeleteAll_IndexReinit_Fails() {
    // Arrange
    struct Fts5Config cfg;
    struct Fts5Storage st;

    static const char *zDb = "testdb3";
    static const char *zName = "testname3";

    cfg.db = (sqlite3 *) (void *) 0x03;
    cfg.zDb = zDb;
    cfg.zName = zName;
    cfg.bColumnsize = 0; // only first delete occurs

    st.pConfig = &cfg;
    st.pIndex = (struct Fts5Index *) (void *) 0x30;
    st.bTotalsValid = 5;

    g_rc_execPrintf = SQLITE_OK;
    g_rc_indexReinit = SQLITE_ERROR; // force error on reinit
    g_rc_storageValue = SQLITE_OK;
    g_secondCallFail = 0;
    g_calls_execPrintf = 0;
    g_called_config_value = 0;

    int rc = sqlite3Fts5StorageDeleteAll(&st);

    // Assert
    test_expect(rc == SQLITE_ERROR, "Test3: rc should reflect IndexReinit failure");
    test_expect(g_called_config_value == 0, "Test3: Storage config value should not be called when reinit fails");
    test_expect(g_calls_execPrintf == 1, "Test3: fts5ExecPrintf should be called once for data/idx delete");
    printf("Test 3 (IndexReinit_Fails) completed. rc=%d\n", rc);
}

// Test 4: First ExecPrintf fails (delete queries not executed)
static void test_DeleteAll_FirstExecPrintf_Fails() {
    // Arrange
    struct Fts5Config cfg;
    struct Fts5Storage st;

    static const char *zDb = "testdb4";
    static const char *zName = "testname4";

    cfg.db = (sqlite3 *) (void *) 0x04;
    cfg.zDb = zDb;
    cfg.zName = zName;
    cfg.bColumnsize = 0;

    st.pConfig = &cfg;
    st.pIndex = (struct Fts5Index *) (void *) 0x40;
    st.bTotalsValid = 9;

    g_rc_execPrintf = SQLITE_ERROR; // simulate failure on first delete
    g_rc_indexReinit = SQLITE_OK;
    g_rc_storageValue = SQLITE_OK;
    g_secondCallFail = 0;
    g_calls_execPrintf = 0;
    g_called_config_value = 0;

    int rc = sqlite3Fts5StorageDeleteAll(&st);

    test_expect(rc == SQLITE_ERROR, "Test4: rc should reflect first ExecPrintf failure");
    test_expect(g_calls_execPrintf == 1, "Test4: fts5ExecPrintf should be called exactly once (first delete)");
    test_expect(g_called_config_value == 0, "Test4: Storage config value should not be called after first failure");
    printf("Test 4 (FirstExecPrintf_Fails) completed. rc=%d\n", rc);
}

// 4) Runner to execute all tests
int main() {
    printf("Starting sqlite3Fts5StorageDeleteAll unit tests (C++11 harness, no GTest).\n");

    test_DeleteAll_Succeeds_NoDocsize_BColumnsize0();
    test_DeleteAll_SecCond_FailOnSecondExecPrintf_BColumnsize1();
    test_DeleteAll_IndexReinit_Fails();
    test_DeleteAll_FirstExecPrintf_Fails();

    if (g_testFailures == 0) {
        printf("ALL TESTS PASSED.\n");
        return 0;
    } else {
        printf("SOME TESTS FAILED: %d failure(s).\n", g_testFailures);
        return 1;
    }
}
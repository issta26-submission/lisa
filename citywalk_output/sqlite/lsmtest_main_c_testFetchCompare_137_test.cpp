// C++11 test suite for the focal method testFetchCompare in lsmtest_main.c
// This test suite does not rely on GTest. It uses a lightweight harness and
// the existing lsmtest API to exercise testFetchCompare under controlled scenarios.
// The tests are designed to run in a normal environment where the lsm test
// infrastructure is available (lsmtest.h, tdb_fetch, tdb_open, etc.).

#include <lsmtest.h>
#include <cstdio>
#include <sys/time.h>
#include <sqlite3.h>
#include <cstring>
#include <sys/resource.h>
#include <cstdlib>


// Bring in C declarations from the project (lsmtest.h defines TestDb, etc.)
extern "C" {
}

// Declare the focal function from the C source so we can call it directly.
extern "C" void testFetchCompare(TestDb *pControl, TestDb *pDb, void *pKey, int nKey, int *pRc);

// Lightweight test harness
static int g_total = 0;
static int g_failed = 0;

// Utility macro: log a test result and continue (non-terminating)
#define TEST_LOG(msg, ...) do { \
    fprintf(stderr, "TEST: " msg "\n", ##__VA_ARGS__); \
} while(0)

// Helper: run a single-case assertion that does not abort the program
#define ASSERT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        fprintf(stderr, "ASSERTION FAILED: %s\n", msg); \
        g_failed++; \
    } \
} while(0)

// Helper: Open a new test database instance
static TestDb* open_db(const char* zSystem, int clear, int* pRc) {
    if(*pRc != 0) return nullptr;
    TestDb *pDb = testOpen(zSystem, clear ? 1 : 0, pRc);
    if(*pRc != 0 || !pDb){
        fprintf(stderr, "Failed to open DB system '%s' (clear=%d)\n", zSystem, (int)clear);
    }
    return pDb;
}

// Helper: Write a string key/value pair (ASCII) to the given DB
static void write_str(TestDb* pDb, const char* zKey, const char* zVal, int* pRc) {
    if(*pRc != 0) return;
    testWriteStr(pDb, zKey, zVal, pRc);
}

// Test Case 1: testFetchCompare with the same database instance for control and db
// Expectation: rc should remain 0 (no mismatch) since both fetch results come from the same DB state.
static void test_case_same_db_equal_values() {
    g_total++;
    int rc = 0;
    // Open a fresh database instance
    TestDb *pCtl = open_db("lsm", 1, &rc);
    TestDb *pDb  = pCtl; // Use the same DB instance to ensure identical data

    if(!pCtl || rc != 0) {
        ASSERT_TRUE(false, "Failed to open shared DB for test_case_same_db_equal_values");
        return;
    }

    // Write identical key/value to the same DB
    const char *key = "case1-key";
    const char *val = "shared-value";
    write_str(pCtl, key, val, &rc);
    // Since pCtl == pDb, both refer to the same data; no separate action needed
    // But call testFetchCompare with both pointers pointing to the same DB
    testFetchCompare(pCtl, pDb, (void*)key, (int)strlen(key), &rc);

    // Expect rc == 0 (no mismatch)
    ASSERT_TRUE(rc == 0, "testFetchCompare should not set rc when both DBs are identical");
    // Clean up
    testClose(&pCtl);
    // pDb is the same as pCtl; no separate close needed
}

// Test Case 2: testFetchCompare with two separate databases containing different values
// Expectation: rc should become non-zero due to mismatch in fetched values.
static void test_case_different_values() {
    g_total++;
    int rc = 0;

    // Open two independent databases (control and db)
    TestDb *pCtl = open_db("lsm", 1, &rc);
    TestDb *pDb  = open_db("lsm", 1, &rc);
    if(!pCtl || !pDb || rc != 0){
        ASSERT_TRUE(false, "Failed to open two independent DBs for test_case_different_values");
        if(pCtl) testClose(&pCtl);
        if(pDb)  testClose(&pDb);
        return;
    }

    // Write different values for the same key
    const char *key = "case2-key";
    write_str(pCtl, key, "value-from-control", &rc);
    write_str(pDb,  key, "value-from-db-different", &rc);

    // Fetch-comparison should detect the difference and set *pRc non-zero
    testFetchCompare(pCtl, pDb, (void*)key, (int)strlen(key), &rc);

    // Expect a non-zero rc to indicate mismatch
    ASSERT_TRUE(rc != 0, "testFetchCompare should detect mismatch between two DBs with different data");
    // Clean up
    testClose(&pCtl);
    testClose(&pDb);
}

// Test Case 3: testFetchCompare with same database pointer but no explicit data difference
// This ensures the "no error path" is exercised when both pControl and pDb point to the same state.
static void test_case_same_pointer_no_diff() {
    // Reuse Case 1 scenario but with a different key to ensure function handles multiple calls
    g_total++;
    int rc = 0;
    TestDb *pCtl = open_db("lsm", 1, &rc);
    TestDb *pDb  = pCtl;
    if(!pCtl || rc != 0){
        ASSERT_TRUE(false, "Failed to open DB for test_case_same_pointer_no_diff");
        return;
    }
    const char *key = "case3-key";
    write_str(pCtl, key, "val3", &rc);
    testFetchCompare(pCtl, pDb, (void*)key, (int)strlen(key), &rc);
    ASSERT_TRUE(rc == 0, "testFetchCompare should remain 0 when same-pointer DBs have identical data (case3)");
    testClose(&pCtl);
}

// Entry point
int main(int argc, char** argv) {
    (void)argc; (void)argv;

    g_total = 0;
    g_failed = 0;

    // Run test cases
    test_case_same_db_equal_values();
    test_case_different_values();
    test_case_same_pointer_no_diff();

    // Summary
    if(g_failed){
        fprintf(stderr, "UNIT TEST SUMMARY: FAILED %d/%d tests\n", g_failed, g_total);
    }else{
        fprintf(stderr, "UNIT TEST SUMMARY: ALL %d tests PASSED\n", g_total);
    }
    return (g_failed ? 1 : 0);
}
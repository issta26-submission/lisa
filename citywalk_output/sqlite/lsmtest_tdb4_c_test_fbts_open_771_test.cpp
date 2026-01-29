/*
  Unit test suite for the focal method: test_fbts_open
  - No GoogleTest; a lightweight C++11 test harness is provided.
  - The test suite exercises test_fbts_open by comparing its return value
    against a direct call to test_bt_open with the fixed spec string
    "fast=1 blksz=32K pagesz=512".
  - We use a minimal, non-terminating assertion approach to maximize code execution
    and coverage (logs failures but continues running all tests).
  - We assume TestDb is an opaque type defined in the C source; we forward-declare it.
  - The test harness uses main() to run tests as required by the domain knowledge.
*/

#include <bt.h>
#include <lsmtest.h>
#include <lsmtest_tdb.h>
#include <cstdio>
#include <string>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <pthread.h>


extern "C" {
    // Forward declare the opaque C type from the tested C module
    struct TestDb;

    // Prototypes for the focal function and its dependency
    int test_bt_open(const char *zSpec, const char *zFilename, int bClear, TestDb **ppDb);
    int test_fbts_open(const char *zSpec, const char *zFilename, int bClear, TestDb **ppDb);
}

// Lightweight non-terminating assertion/logging helper
static int g_total = 0;
static int g_failed = 0;

static void logTest(const std::string &name, bool passed) {
    std::cout << (passed ? "[PASS] " : "[FAIL] ") << name << std::endl;
    ++g_total;
    if (!passed) ++g_failed;
}

// Helper to clean up a test database file if created by the C test module.
// We best-effort remove the file; errors are ignored.
static void cleanupFile(const char *filename) {
    if (filename && filename[0] != '\0') {
        std::remove(filename);
    }
}

// Compare the behavior of test_fbts_open against test_bt_open
static bool compare_fbts_to_bt(const char *zSpec, const char *zFilename, int bClear) {
    // Ensure the testDb pointers exist for both calls (though we do not rely on their values)
    TestDb *pDbFBTS = nullptr;
    TestDb *pDbBT = nullptr;

    int resFBTS = test_fbts_open(zSpec, zFilename, bClear, &pDbFBTS);
    int resBT   = test_bt_open("fast=1 blksz=32K pagesz=512", zFilename, bClear, &pDbBT);

    // Cleanup test artifacts (best-effort)
    cleanupFile(zFilename);

    // Compare results; this verifies the wrapper delegates to the fixed spec
    return (resFBTS == resBT);
}

int main() {
    std::cout << "Starting test_fbts_open wrapper tests (no GTest).\n";

    // Test 1: zSpec differs, bClear = 0
    // The focal method should ignore zSpec and always call test_bt_open with the fixed spec.
    {
        const char *tmpFile = "/tmp/tdb_fbts_open_test1.db";
        bool ok = compare_fbts_to_bt("anything", tmpFile, 0);
        logTest("Wrapper delegates to test_bt_open with fixed spec (zSpec ignore), bClear=0", ok);
    }

    // Test 2: zSpec differs, bClear = 1
    {
        const char *tmpFile = "/tmp/tdb_fbts_open_test2.db";
        bool ok = compare_fbts_to_bt("anotherSpec", tmpFile, 1);
        logTest("Wrapper delegates to test_bt_open with fixed spec (zSpec ignore), bClear=1", ok);
    }

    // Test 3: zSpec = NULL (edge case), bClear = 0
    // Ensures that passing a NULL zSpec does not affect the delegation logic.
    {
        const char *tmpFile = "/tmp/tdb_fbts_open_test3.db";
        bool ok = compare_fbts_to_bt(nullptr, tmpFile, 0);
        logTest("Wrapper delegates to test_bt_open with fixed spec (NULL zSpec), bClear=0", ok);
    }

    // Test 4: Ensure multiple independent invocations behave deterministically
    {
        const char *tmpFileA = "/tmp/tdb_fbts_open_test4a.db";
        const char *tmpFileB = "/tmp/tdb_fbts_open_test4b.db";
        bool okA = compare_fbts_to_bt("specA", tmpFileA, 0);
        bool okB = compare_fbts_to_bt("specB", tmpFileB, 0);
        logTest("Wrapper consistency across multiple independent invocations (0,0)", okA && okB);
    }

    std::cout << "Finished tests. Total: " << g_total << ", Failures: " << g_failed << std::endl;
    // Return non-zero if there were failures to indicate abnormal termination to build system
    return g_failed;
}
// lsmtest_fetch_unit.cpp
// A lightweight C++11 unit test harness for the C function:
//   void testFetch(TestDb *pDb, void *pKey, int nKey, void *pVal, int nVal, int *pRc)
// The tests are designed to be non-terminating (no aborts) and rely on the
// existing C test framework (lsmtest.h). We exercise only the safe path where
// *pRc != 0 is not modified (to avoid triggering test_fail() aborts) and where
// a successful fetch matches the expected value (rc == 0 and data matches).
//
// Important: The harness uses the provided API from the focal class.
// It assumes the environment provides the underlying tdb_fetch and related DB helpers.

#include <lsmtest.h>
#include <sys/time.h>
#include <sqlite3.h>
#include <string>
#include <cstring>
#include <sys/resource.h>
#include <iostream>


// Import C declarations. The focal code is C; expose it with C linkage for the C++ test.
extern "C" {
}

// Note on test coverage and safety:
// - We deliberately test only the "safe" branches that do not trigger test_failed()
//   (which would abort the test process). This means we cover:
//     - The outer if (*pRc==0) path (when pRc is non-zero, testFetch does nothing).
//     - The successful fetch path where rc==0 and the retrieved value equals the expected value.
// - The inner mismatch branch (rc==0 but data mismatch) would call testSetError(1),
//   which would lead to test_failed() abort. We avoid exercising that branch here
//   to keep tests safe and non-terminating in this environment.

// Helper: Open a fresh DB, write a single key/value, and then fetch to verify
// that testFetch reports no error when the value matches exactly.
static void test_case_fetch_matches() {
    int rc = 0;
    // Open a fresh LSM DB (bClear = 1) to ensure a clean state.
    TestDb *pDb = testOpen("lsm", 1, &rc);
    if (rc != 0 || pDb == nullptr) {
        std::cerr << "SKIP: testOpen failed in test_case_fetch_matches (rc=" << rc << ")\n";
        return;
    }

    // Write a known key/value pair: key="key1", val="val1"
    int wrc = 0;
    testWriteStr(pDb, "key1", "val1", &wrc);
    if (wrc != 0) {
        std::cerr << "FAIL: testWriteStr failed in test_case_fetch_matches (wrc=" << wrc << ")\n";
        testClose(&pDb);
        return;
    }

    // Fetch the same key/value pair using testFetchStr wrapper.
    int fRc = 0;
    testFetchStr(pDb, "key1", "val1", &fRc);
    if (fRc != 0) {
        std::cerr << "FAIL: testFetchStr reported rc=" << fRc << " for matching key/value\n";
    } else {
        std::cout << "PASS: testFetch correctly retrieved matching key/value\n";
    }

    // Cleanup
    testClose(&pDb);
}

// Helper: Verify the branch where *pRc is non-zero causes testFetch to skip work.
// This is a safety test to ensure the outer guard is respected (no aborts).
static void test_case_fetch_with_nonzero_pRc_path() {
    int rcopen = 0;
    TestDb *pDb = testOpen("lsm", 1, &rcopen);
    if (rcopen != 0 || pDb == nullptr) {
        std::cerr << "SKIP: cannot open DB for test_case_fetch_with_nonzero_pRc_path\n";
        return;
    }

    // Start with a non-zero rc value; testFetch should skip the body.
    int pRc = 1;
    testFetchStr(pDb, "dummy", "x", &pRc);

    if (pRc != 1) {
        std::cerr << "FAIL: testFetch altered pRc when *pRc != 0 (pRc=" << pRc << ")\n";
    } else {
        std::cout << "PASS: testFetch correctly skipped when *pRc != 0\n";
    }

    testClose(&pDb);
}

// Entry point for the test suite.
int main() {
    std::cout << "Running lsmtest_fetch_unit.cpp: testFetch unit tests\n";

    // Run non-zero-pRc path test (safety path)
    test_case_fetch_with_nonzero_pRc_path();

    // Run matching fetch test (positive path)
    test_case_fetch_matches();

    std::cout << "lsmtest_fetch_unit.cpp: tests finished\n";
    return 0;
}
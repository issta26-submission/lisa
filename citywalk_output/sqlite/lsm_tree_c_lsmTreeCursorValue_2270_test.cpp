/***************************************
 * Unit Tests for lsmTreeCursorValue
 * - Language: C++11
 * - Approach: Lightweight in-process tests using the project public C API
 * - No GoogleTest or external frameworks used
 * - Tests are designed to be executable by the project's existing build
 *   (linking against the provided lsm_tree.c and related headers)
 * 
 * Notes:
 * - The tests rely on the public API exposed by the project (lsm_tree.h / lsmInt.h).
 * - Each test is annotated with comments explaining the intent and edge cases.
 * - Static helpers inside the test file are used to keep tests isolated.
 * - This file can be compiled as part of the test suite in a C++11 capable build.
 ****************************************/

#include <string.h>
#include <lsm_tree.h>
#include <lsmInt.h>
#include <string>
#include <cstring>
#include <iostream>


// Forward declare the project C API (as it would be exposed by the headers in the repo)
extern "C" {
}

/*
 * Lightweight test framework (non-terminating style)
 * - We accumulate a count of failed assertions
 * - Each test reports a descriptive message on failure
 * - The main function prints a final summary
 */

static int g_failCount = 0;

static void logFail(const std::string &testName, const std::string &reason) {
    std::cerr << "[FAIL] " << testName << ": " << reason << std::endl;
    ++g_failCount;
}

static void logPass(const std::string &testName) {
    std::cout << "[PASS] " << testName << std::endl;
}

/*
 * Utility helpers
 * - Initialize a fresh in-memory database context
 * - Create and destroy a cursor
 * - Helper to compare memory blocks safely
 */

// Helper to initialize a database instance (in-memory style) for tests
static bool initDb(lsm_db *&pDb) {
    // The real project exposes lsm_db as a structure. Here we allocate and zero it.
    pDb = new lsm_db;
    if (!pDb) return false;
    std::memset(pDb, 0, sizeof(lsm_db));

    // The project API provides lsmTreeInit to initialize tree state.
    // We assume it returns 0 on success (LSM_OK semantics).
    if (lsmTreeInit(pDb) != 0) {
        delete pDb;
        pDb = nullptr;
        return false;
    }
    return true;
}

// Helper to clean up database instance
static void destroyDb(lsm_db *pDb) {
    if (pDb) {
        // If there is a public transaction or cleanup function in the project,
        // it should be called here. We assume a best-effort destruction sequence.
        // Note: If the project provides a dedicated destructor/cleanup function,
        // it should be used. Otherwise releasing the DB object suffices for tests.
        delete pDb;
    }
}

// Helper to create a new cursor
static bool createCursor(lsm_db *pDb, TreeCursor **ppCsr) {
    if (!pDb || !ppCsr) return false;
    // bOld = 0 for a fresh cursor per typical API usage
    if (lsmTreeCursorNew(pDb, 0, ppCsr) != 0) {
        *ppCsr = nullptr;
        return false;
    }
    return true;
}

// Helper to safely release a cursor
static void destroyCursor(TreeCursor *pCsr) {
    if (pCsr) {
        lsmTreeCursorDestroy(pCsr);
    }
}

/*
 * Test Case 1: Value retrieval for an inserted key
 * - Intent: Exercise the true-branch where:
 *     res == 0 after treeCursorRestore, rc == LSM_OK after csrGetKey,
 *     and the key.flags has LSM_INSERT.
 * - Expected: lsmTreeCursorValue returns LSM_OK, pnVal equals key.nValue,
 *             and ppVal points to the stored value (TKV_VAL).
 */
static void test_value_retrieved_for_inserted_key() {
    const std::string testName = "test_value_retrieved_for_inserted_key";

    lsm_db *pDb = nullptr;
    if (!initDb(pDb)) {
        logFail(testName, "Failed to initialize database context");
        return;
    }

    TreeCursor *pCsr = nullptr;
    if (!createCursor(pDb, &pCsr)) {
        logFail(testName, "Failed to create TreeCursor");
        destroyDb(pDb);
        return;
    }

    // Prepare a simple key/value
    const char *key = "u_key_insert";
    const char *val = "inserted_value";

    // Insert the key-value pair
    // Assumes the API supports a basic binary-key insertion
    int rc = lsmTreeInsert(pDb, (void*)key, (int)std::strlen(key),
                           (void*)val, (int)std::strlen(val));

    if (rc != LSM_OK) {
        logFail(testName, "lsmTreeInsert() did not return LSM_OK");
        destroyCursor(pCsr);
        destroyDb(pDb);
        return;
    }

    // Seek to the key to position the cursor
    int res = 0;
    rc = lsmTreeCursorSeek(pCsr, (void*)key, (int)std::strlen(key), &res);
    if (rc != LSM_OK || res != 0) { // res==0 indicates found (depending on API)
        logFail(testName, "lsmTreeCursorSeek() did not locate the key as expected");
        destroyCursor(pCsr);
        destroyDb(pDb);
        return;
    }

    // Fetch the value via the focal function
    void *ppVal = nullptr;
    int pnVal = 0;
    rc = lsmTreeCursorValue(pCsr, &ppVal, &pnVal);

    if (rc != LSM_OK) {
        logFail(testName, "lsmTreeCursorValue() did not return LSM_OK");
        destroyCursor(pCsr);
        destroyDb(pDb);
        return;
    }

    // Validate content and address semantics
    if (ppVal == nullptr) {
        logFail(testName, "ppVal is null for a valid inserted key");
    } else if (pnVal != (int)std::strlen(val)) {
        logFail(testName, "pnVal does not match the length of the inserted value");
    } else if (std::strcmp((const char*)ppVal, val) != 0) {
        logFail(testName, "Value content retrieved does not match the inserted value");
    } else {
        logPass(testName);
    }

    destroyCursor(pCsr);
    destroyDb(pDb);
}

/*
 * Test Case 2: Cursor not positioned (res != 0 path)
 * - Intent: Exercise the else branch where treeCursorRestore indicates no valid position.
 * - Expected: ppVal == 0, pnVal == 0 and rc is whatever treeCursorRestore produced.
 *   Since the exact rc after restoration failure is implementation-dependent, we only
 *   assert that the output value/pointer are zeroed as per the focal method's contract.
 */
static void test_value_with_unrestored_cursor_results_in_zeroes() {
    const std::string testName = "test_value_with_unrestored_cursor_results_in_zeroes";

    lsm_db *pDb = nullptr;
    if (!initDb(pDb)) {
        logFail(testName, "Failed to initialize database context");
        return;
    }

    TreeCursor *pCsr = nullptr;
    if (!createCursor(pDb, &pCsr)) {
        logFail(testName, "Failed to create TreeCursor");
        destroyDb(pDb);
        return;
    }

    // Do NOT position the cursor (no lsmTreeCursorSeek or inserts)

    // Call the focal function directly
    void *ppVal = (void*)0xDEADBEEF; // non-null sentinel to verify zeroing
    int pnVal = -12345;              // non-zero to ensure it's overwritten

    int rc = lsmTreeCursorValue(pCsr, &ppVal, &pnVal);

    // Expect rc to be some value (implementation-defined), but pnVal/ppVal must be set to 0/NULL
    if (pnVal != 0 || ppVal != nullptr) {
        logFail(testName, "Expected zeroed output when cursor is not restored (res != 0)");
    } else {
        logPass(testName);
    }

    destroyCursor(pCsr);
    destroyDb(pDb);
}

/*
 * Test Case 3: Value retrieval for a second inserted key
 * - Intent: Exercise the second insertion path and ensure subsequent retrieval is correct.
 * - This mirrors Test Case 1 but with a different key/value and ensures independence.
 */
static void test_value_retrieved_for_second_insert() {
    const std::string testName = "test_value_retrieved_for_second_insert";

    lsm_db *pDb = nullptr;
    if (!initDb(pDb)) {
        logFail(testName, "Failed to initialize database context");
        return;
    }

    TreeCursor *pCsr = nullptr;
    if (!createCursor(pDb, &pCsr)) {
        logFail(testName, "Failed to create TreeCursor");
        destroyDb(pDb);
        return;
    }

    const char *key = "second_key";
    const char *val = "second_value";

    int rc = lsmTreeInsert(pDb, (void*)key, (int)std::strlen(key),
                           (void*)val, (int)std::strlen(val));

    if (rc != LSM_OK) {
        logFail(testName, "lsmTreeInsert() for second key did not return LSM_OK");
        destroyCursor(pCsr);
        destroyDb(pDb);
        return;
    }

    int res = 0;
    rc = lsmTreeCursorSeek(pCsr, (void*)key, (int)std::strlen(key), &res);
    if (rc != LSM_OK || res != 0) {
        logFail(testName, "lsmTreeCursorSeek() failed for second key");
        destroyCursor(pCsr);
        destroyDb(pDb);
        return;
    }

    void *ppVal = nullptr;
    int pnVal = 0;
    rc = lsmTreeCursorValue(pCsr, &ppVal, &pnVal);
    if (rc != LSM_OK) {
        logFail(testName, "lsmTreeCursorValue() failed for second key");
        destroyCursor(pCsr);
        destroyDb(pDb);
        return;
    }

    if (ppVal == nullptr || pnVal != (int)std::strlen(val) ||
        std::strcmp((const char*)ppVal, val) != 0) {
        logFail(testName, "Retrieved value for second key does not match expected");
    } else {
        logPass(testName);
    }

    destroyCursor(pCsr);
    destroyDb(pDb);
}

/*
 * Main: Run all tests
 */
int main() {
    std::cout << "Running unit tests for lsmTreeCursorValue..." << std::endl;

    test_value_retrieved_for_inserted_key();
    test_value_with_unrestored_cursor_results_in_zeroes();
    test_value_retrieved_for_second_insert();

    if (g_failCount == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failCount << " test(s) failed." << std::endl;
        return 1;
    }
}
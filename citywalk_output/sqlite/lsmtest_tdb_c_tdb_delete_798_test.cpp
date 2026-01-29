// test_tdb_delete.cpp
// Lightweight C++11 test suite for the focal C function: tdb_delete
// This test exercises tdb_delete(TestDb *pDb, void *pKey, int nKey)
// which simply forwards its arguments to pDb->pMethods->xDelete(pDb, pKey, nKey).

#include <stdlib.h>
#include <lsmtest.h>
#include <string.h>
#include <lsmtest_tdb.h>
#include <assert.h>
#include <cstdio>
#include <cstddef>
#include <stdio.h>
#include <leveldb/c.h>
#include <lsm.h>
#include <cstdint>


// Include the C headers that declare TestDb, TestDbMethods, and tdb_delete.
// Use C linkage for compatibility with the C code under test.
extern "C" {
}

// Simple, non-terminating assertion helpers.
// They print results but do not abort the test on failure.
static int g_test_failed = 0;

#define ASSERT_EQ_INT(actual, expected, msg) do {                    \
    if ((actual) != (expected)) {                                    \
        printf("ASSERT FAILED: %s | expected %d, got %d\n", (msg),  \
               (int)(expected), (int)(actual));                    \
        g_test_failed = 1;                                           \
    } else {                                                           \
        printf("ASSERT PASSED: %s\n", (msg));                        \
    }                                                                  \
} while (0)

#define ASSERT_PTR_EQ(actual, expected, msg) do {                    \
    if ((void*)(actual) != (void*)(expected)) {                     \
        printf("ASSERT FAILED: %s | pointers differ (actual=%p, expected=%p)\n", \
               (msg), (void*)(actual), (void*)(expected));          \
        g_test_failed = 1;                                           \
    } else {                                                           \
        printf("ASSERT PASSED: %s\n", (msg));                        \
    }                                                                  \
} while (0)


// Global capture for parameters passed to the fake xDelete function.
static TestDb *g_called_pDb = nullptr;
static void *g_called_pKey = nullptr;
static int   g_called_nKey = 0;

// First fake xDelete: returns a fixed value (42) and records parameters.
static int fake_delete_return_42(TestDb *pDb, void *pKey, int nKey) {
    g_called_pDb = pDb;
    g_called_pKey = pKey;
    g_called_nKey = nKey;
    return 42;
}

// Second fake xDelete: returns a different fixed value (-1) to test forwarding of result.
static int fake_delete_return_minus1(TestDb *pDb, void *pKey, int nKey) {
    g_called_pDb = pDb;
    g_called_pKey = pKey;
    g_called_nKey = nKey;
    return -1;
}

// Helper: reset capture state
static void reset_capture() {
    g_called_pDb = nullptr;
    g_called_pKey = nullptr;
    g_called_nKey = 0;
}

// Test 1: Ensure tdb_delete forwards the call to xDelete and returns its result.
// Also verify that all parameters are passed through correctly.
static void test_tdb_delete_forwards_call_and_params() {
    printf("Running test: tdb_delete forwards call and params (expected 42)\n");
    reset_capture();

    // Prepare a TestDb instance with a mock xDelete that returns 42.
    static TestDbMethods methods;      // Assumes existence from headers.
    static TestDb db;                  // The TestDb instance to pass to tdb_delete.

    methods.xDelete = fake_delete_return_42;
    db.pMethods = &methods;

    // Execute the focal method
    int result = tdb_delete(&db, (void*)0xDEADBEEF, 7);

    // Assertions
    ASSERT_EQ_INT(result, 42, "tdb_delete should return value of xDelete (42)");
    ASSERT_PTR_EQ(g_called_pDb, &db, "tdb_delete should pass the same TestDb pointer through");
    ASSERT_PTR_EQ(g_called_pKey, (void*)0xDEADBEEF, "tdb_delete should pass pKey through unchanged");
    ASSERT_EQ_INT(g_called_nKey, 7, "tdb_delete should pass nKey through unchanged");
}

// Test 2: Ensure tdb_delete forwards the call and the return value when xDelete returns -1.
// This validates handling of non-zero/negative return values from xDelete.
static void test_tdb_delete_forwards_return_minus1() {
    printf("Running test: tdb_delete forwards return -1 from xDelete\n");
    reset_capture();

    // Prepare a TestDb with fake xDelete returning -1
    static TestDbMethods methods;      // Assumes existence from headers.
    static TestDb db;
    methods.xDelete = fake_delete_return_minus1;
    db.pMethods = &methods;

    int result = tdb_delete(&db, (void*)0xABCDEF01, 123);

    // Assertions
    ASSERT_EQ_INT(result, -1, "tdb_delete should forward -1 returned by xDelete");
    ASSERT_PTR_EQ(g_called_pDb, &db, "tdb_delete should pass the same TestDb pointer through");
    ASSERT_PTR_EQ(g_called_pKey, (void*)0xABCDEF01, "tdb_delete should pass pKey through unchanged");
    ASSERT_EQ_INT(g_called_nKey, 123, "tdb_delete should pass nKey through unchanged");
}

// Main: Run tests from the main function as required by the domain knowledge.
// If any test failed, return non-zero to indicate failure.
int main() {
    printf("Starting tdb_delete unit tests (C++ harness, no GTest).\n\n");

    test_tdb_delete_forwards_call_and_params();
    printf("\n");

    test_tdb_delete_forwards_return_minus1();
    printf("\n");

    if (g_test_failed) {
        printf("TEST SUITE RESULT: FAILED\n");
        return 1;
    } else {
        printf("TEST SUITE RESULT: PASSED\n");
        return 0;
    }
}
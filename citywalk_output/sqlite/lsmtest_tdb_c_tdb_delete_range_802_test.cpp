// Test suite for the focal method: tdb_delete_range in lsmtest_tdb.c
// This C++11 test harness avoids Google Test and uses a lightweight, self-contained
// test runner. It mocks the xDeleteRange callback to verify that tdb_delete_range
// forwards calls correctly and propagates return values as-is.

#include <stdlib.h>
#include <lsmtest.h>
#include <string.h>
#include <lsmtest_tdb.h>
#include <assert.h>
#include <stdio.h>
#include <cstring>
#include <leveldb/c.h>
#include <iostream>
#include <lsm.h>
#include <cstdint>
#include <cassert>


// Include the C headers with C linkage to ensure correct name mangling
extern "C" {
}

// Global variables to capture arguments passed to the mocked xDeleteRange
static TestDb *g_last_pDb = nullptr;
static void *g_last_pKey1 = nullptr;
static int   g_last_nKey1 = 0;
static void *g_last_pKey2 = nullptr;
static int   g_last_nKey2 = 0;

// Control return value for the mock xDeleteRange
static int g_mock_return_value = 0;

// Mock implementation of xDeleteRange to intercept parameters and control return value
extern "C" int mock_xDeleteRange(TestDb *pDb, void *pKey1, int nKey1, void *pKey2, int nKey2) {
    g_last_pDb    = pDb;
    g_last_pKey1  = pKey1;
    g_last_nKey1  = nKey1;
    g_last_pKey2  = pKey2;
    g_last_nKey2  = nKey2;
    return g_mock_return_value;
}

// Helper to construct a minimal TestDb instance with its methods populated
static TestDb* make_test_db() {
    // Allocate TestDb and its method table
    TestDb* p = new TestDb;
    // The actual TestDb struct in the project typically has more fields,
    // but for the purposes of this test we only need pMethods to be non-null.
    // We assume TestDb has a member: TestDbMethods *pMethods;
    p->pMethods = new TestDbMethods;
    // Assign the mock function to xDeleteRange
    p->pMethods->xDeleteRange = mock_xDeleteRange;
    return p;
}

// Helper to clean up allocated objects
static void destroy_test_db(TestDb* p) {
    if (p) {
        delete p->pMethods;
        delete p;
    }
}

// Lightweight test harness
static int g_test_count = 0;
static int g_pass_count = 0;

#define RUN_TEST(NAME) \
    do { \
        std::cout << "Running: " << NAME << " ... "; \
        fflush(stdout); \
        if (NAME()) { \
            ++g_pass_count; \
            std::cout << "PASS\n"; \
        } else { \
            std::cout << "FAIL\n"; \
        } \
        ++g_test_count; \
    } while(0)

#define ASSERT_EQ(A, B) \
    if (!((A) == (B))) { \
        std::cerr << "Assertion failed: " << #A << " == " << #B \
                  << " (" << (A) << " != " << (B) << ")\n"; \
        return false; \
    }

#define ASSERT_PTR_EQ(A, B) \
    if ((A) != (B)) { \
        std::cerr << "Assertion failed: pointers " << #A << " == " << #B \
                  << " ( " << A << " != " << B << " )\n"; \
        return false; \
    }

// Test 1: Forwarding correctness and argument preservation (mock returns 0)
bool test_tdb_delete_range_forwards_arguments_and_returns_zero() {
    g_mock_return_value = 0;
    TestDb* pDb = make_test_db();

    void* key1 = reinterpret_cast<void*>(0x11110001);
    int nKey1 = 5;
    void* key2 = reinterpret_cast<void*>(0x22220002);
    int nKey2 = 9;

    // Reset captured values
    g_last_pDb = nullptr;
    g_last_pKey1 = nullptr;
    g_last_nKey1 = 0;
    g_last_pKey2 = nullptr;
    g_last_nKey2 = 0;

    int ret = tdb_delete_range(pDb, key1, nKey1, key2, nKey2);

    // Assertions
    ASSERT_EQ(ret, 0);
    ASSERT_PTR_EQ(g_last_pDb, pDb);
    ASSERT_PTR_EQ(g_last_pKey1, key1);
    ASSERT_EQ(g_last_nKey1, nKey1);
    ASSERT_PTR_EQ(g_last_pKey2, key2);
    ASSERT_EQ(g_last_nKey2, nKey2);

    destroy_test_db(pDb);
    return true;
}

// Test 2: Forwarding correctness when mock returns a negative value
bool test_tdb_delete_range_forwards_arguments_and_returns_negative() {
    g_mock_return_value = -7;
    TestDb* pDb = make_test_db();

    void* key1 = reinterpret_cast<void*>(0xDEADBEEF);
    int nKey1 = 3;
    void* key2 = reinterpret_cast<void*>(0xFEEDC0DE);
    int nKey2 = 12;

    g_last_pDb = nullptr;
    g_last_pKey1 = nullptr;
    g_last_nKey1 = 0;
    g_last_pKey2 = nullptr;
    g_last_nKey2 = 0;

    int ret = tdb_delete_range(pDb, key1, nKey1, key2, nKey2);

    // Assertions
    ASSERT_EQ(ret, -7);
    ASSERT_PTR_EQ(g_last_pDb, pDb);
    ASSERT_PTR_EQ(g_last_pKey1, key1);
    ASSERT_EQ(g_last_nKey1, nKey1);
    ASSERT_PTR_EQ(g_last_pKey2, key2);
    ASSERT_EQ(g_last_nKey2, nKey2);

    destroy_test_db(pDb);
    return true;
}

// Main function to run tests
int main() {
    std::cout << "Starting tdb_delete_range tests (C++11, no GTest)\n";

    RUN_TEST(test_tdb_delete_range_forwards_arguments_and_returns_zero);
    RUN_TEST(test_tdb_delete_range_forwards_arguments_and_returns_negative);

    std::cout << "Tests run: " << g_test_count << ", Passed: " << g_pass_count
              << ", Failed: " << (g_test_count - g_pass_count) << "\n";

    // If any test failed, return non-zero
    return (g_test_count == g_pass_count) ? 0 : 1;
}
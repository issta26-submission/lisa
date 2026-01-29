/*
  Test suite for sqlite3BtreeSetAutoVacuum (C function) using C++11 tests
  - Step 1 (Program Understanding) highlights:
    Core dependent components (Candidate Keywords):
      - Btree (structure, pBt member)
      - BtShared (structure, fields: btsFlags, autoVacuum, incrVacuum)
      - BTS_PAGESIZE_FIXED (flag bit in btsFlags)
      - SQLITE_OK, SQLITE_READONLY (return codes)
      - sqlite3BtreeEnter / sqlite3BtreeLeave (lock guards within btree.c)
    - The function behavior:
      - If SQLITE_OMIT_AUTOVACUUM is defined, returns SQLITE_READONLY
      - Otherwise, if (pBt->btsFlags & BTS_PAGESIZE_FIXED) != 0 and (av ? 1 : 0) != pBt->autoVacuum
        then return SQLITE_READONLY
      - Else update pBt->autoVacuum and pBt->incrVacuum based on provided autoVacuum value
  - Step 2/3 (Unit Test Generation and Refinement):
    Create minimal Btree/BtShared objects to exercise both branches:
      1) Normal path: not fixed pagesize, enabling autovacuum with various values (0,1,2)
      2) Fixed pagesize with mismatch: expect READONLY
      3) Fixed pagesize with match: expect OK and internal state unchanged accordingly
    Implementation notes:
      - No GTest; use a small handcrafted test harness with simple EXPECT_EQ/EXPECT_NOT equivalents
      - Tests use actual sqlite3BtreeSetAutoVacuum from the project; Btree/BtShared types are pulled from btreeInt.h
      - Tests dynamically allocate minimal Btree/BtShared to avoid needing full database context
  - All tests are self-contained in one file and can be compiled with a separate build step that links against the project sources.
  - This file uses only standard C++11 library plus the project's headers; it does not rely on GTest.
*/

#include <btreeInt.h>
#include <iostream>
#include <cstdlib>
#include <cstring>


// Step A: Bring in C project headers (Btree types and function under test)
extern "C" {
int sqlite3BtreeSetAutoVacuum(Btree *p, int autoVacuum);
}

// Test harness state
static int g_total_tests = 0;
static int g_passed_tests = 0;

// Lightweight assertion helpers (non-terminating)
#define EXPECT_EQ(expected, actual, desc) do { \
    ++g_total_tests; \
    if ((expected) == (actual)) { \
        ++g_passed_tests; \
    } else { \
        std::cerr << "FAIL: " << desc << " | expected: " << (expected) \
                  << ", actual: " << (actual) << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
    } \
} while(0)

#define EXPECT_NEQ(not_expected, actual, desc) do { \
    ++g_total_tests; \
    if ((not_expected) != (actual)) { \
        ++g_passed_tests; \
    } else { \
        std::cerr << "FAIL: " << desc << " | should not be: " << (not_expected) \
                  << " but got: " << (actual) << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
    } \
} while(0)

#define EXPECT_OK(rc, desc) do { \
    ++g_total_tests; \
    if ((rc) == 0) { \
        ++g_passed_tests; \
    } else { \
        std::cerr << "FAIL: " << desc << " | rc should be SQLITE_OK (0), got: " << (rc) \
                  << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
    } \
} while(0)

#define EXPECT_NOT_OK(rc, desc) do { \
    ++g_total_tests; \
    if ((rc) != 0) { \
        ++g_passed_tests; \
    } else { \
        std::cerr << "FAIL: " << desc << " | rc should be non-zero (readonly/error), got: " << (rc) \
                  << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
    } \
} while(0)

// Helper factory to create minimal Btree/BtShared objects for testing
static Btree* create_test_btree(int initialAutoVacuum, int flags) {
    Btree* p = (Btree*)calloc(1, sizeof(Btree));
    if (!p) return nullptr;
    BtShared* pBt = (BtShared*)calloc(1, sizeof(BtShared));
    if (!pBt) {
        free(p);
        return nullptr;
    }
    // Link structures
    p->pBt = pBt;

    // Initialize required fields used by sqlite3BtreeSetAutoVacuum
    pBt->btsFlags = flags;
    pBt->autoVacuum = initialAutoVacuum ? 1 : 0;
    pBt->incrVacuum = (initialAutoVacuum == 2) ? 1 : 0;

    return p;
}

static void destroy_test_btree(Btree* p) {
    if (!p) return;
    if (p->pBt) free(p->pBt);
    free(p);
}

// Step tests

// Test 1: Normal path - not fixed pagesize, enable autovacuum with value 1
// Expect: rc == SQLITE_OK (0), autoVacuum becomes 1, incrVacuum becomes 0
static void test_normal_enable_autovacuum_one() {
    // Purpose: cover the else branch when fixed flag is not set
    Btree* p = create_test_btree(0, 0); // not fixed, initial autoVacuum=0
    int rc = sqlite3BtreeSetAutoVacuum(p, 1); // enable autovacuum
    EXPECT_OK(rc, "test_normal_enable_autovacuum_one: rc should be SQLITE_OK");
    // Verify internal state updated accordingly
    EXPECT_EQ(1, p->pBt->autoVacuum, "test_normal_enable_autovacuum_one: autoVacuum should be 1");
    EXPECT_EQ(0, p->pBt->incrVacuum, "test_normal_enable_autovacuum_one: incrVacuum should be 0");
    destroy_test_btree(p);
}

// Test 2: Normal path - not fixed pagesize, enable autovacuum with value 2 (incrVacuum)
static void test_normal_enable_autovacuum_two() {
    Btree* p = create_test_btree(0, 0); // not fixed
    int rc = sqlite3BtreeSetAutoVacuum(p, 2); // enable incr vacuum mode
    EXPECT_OK(rc, "test_normal_enable_autovacuum_two: rc should be SQLITE_OK");
    EXPECT_EQ(1, p->pBt->autoVacuum, "test_normal_enable_autovacuum_two: autoVacuum should be 1");
    EXPECT_EQ(1, p->pBt->incrVacuum, "test_normal_enable_autovacuum_two: incrVacuum should be 1");
    destroy_test_btree(p);
}

// Test 3: Fixed pagesize, mismatch triggers readonly
// Set BTS_PAGESIZE_FIXED and initial autoVacuum != av (after conversion)
static void test_fixed_pagesize_mismatch_readonly() {
    Btree* p = create_test_btree(0, BTS_PAGESIZE_FIXED); // fixed pagesize
    int rc = sqlite3BtreeSetAutoVacuum(p, 1); // av becomes 1, but mismatch with initial 0
    EXPECT_NOT_OK(rc, "test_fixed_pagesize_mismatch_readonly: rc should be non-zero (readonly)");
    // Internal state should remain unchanged due to early return
    EXPECT_EQ(0, p->pBt->autoVacuum, "test_fixed_pagesize_mismatch_readonly: autoVacuum should remain 0");
    EXPECT_EQ(0, p->pBt->incrVacuum, "test_fixed_pagesize_mismatch_readonly: incrVacuum should remain 0");
    destroy_test_btree(p);
}

// Test 4: Fixed pagesize, match path (av equals current), should update to same value without error
static void test_fixed_pagesize_match_no_change() {
    Btree* p = create_test_btree(1, BTS_PAGESIZE_FIXED); // fixed pagesize, initial autoVacuum=1
    int rc = sqlite3BtreeSetAutoVacuum(p, 1); // av=1, matches current
    EXPECT_OK(rc, "test_fixed_pagesize_match_no_change: rc should be SQLITE_OK");
    // The values should be updated to the same logical values
    EXPECT_EQ(1, p->pBt->autoVacuum, "test_fixed_pagesize_match_no_change: autoVacuum should remain 1");
    EXPECT_EQ(0, p->pBt->incrVacuum, "test_fixed_pagesize_match_no_change: incrVacuum should remain 0");
    destroy_test_btree(p);
}

// Main: run all tests and print a summary
int main() {
    std::cout << "Starting sqlite3BtreeSetAutoVacuum test suite (C++11)\n";

    test_normal_enable_autovacuum_one();
    test_normal_enable_autovacuum_two();
    test_fixed_pagesize_mismatch_readonly();
    test_fixed_pagesize_match_no_change();

    std::cout << "Tests run: " << g_total_tests << ", Passed: " << g_passed_tests
              << ", Failed: " << (g_total_tests - g_passed_tests) << "\n";

    // If any test failed, return non-zero to indicate failure
    return (g_total_tests == g_passed_tests) ? 0 : 1;
}
// C++11 unit test suite for sqlite3BtreeGetAutoVacuum
// This test targets the focal method described in <FOCAL_METHOD>.
// It relies on the project-provided btreeInt.h and the real sqlite3BtreeGetAutoVacuum implementation.
// To exercise both code paths, compile this file twice with different compile-time flags:
// 1) Without defining SQLITE_OMIT_AUTOVACUUM (default): tests the normal path.
// 2) Defining SQLITE_OMIT_AUTOVACUUM (-DSQLITE_OMIT_AUTOVACUUM): tests the OMIT path (returns BTREE_AUTOVACUUM_NONE).
//
// Notes:
// - We use the existing Btree/BTShared structures from the project header to ensure layout compatibility.
// - We implement a lightweight test harness with non-terminating expectations (does not abort on failure).
// - Static helpers and dependencies are resolved via the provided headers. Only public interfaces are used.
//
// How to run (examples):
//  a) g++ -std=c++11 -Wall test.cpp -I<path_to_project_headers> -L<...> -lsqlite3 -o test_normal
//     ./test_normal
//  b) g++ -DSQLITE_OMIT_AUTOVACUUM -std=c++11 -Wall test.cpp -I<path_to_project_headers> -L<...> -lsqlite3 -o test_omit
//     ./test_omit

#include <btreeInt.h>
#include <iostream>
#include <string>


// Bow to C header layout used by the library
extern "C" {
}

// Ensure compatibility if the project headers already define these:
// Define defaults if not already present to avoid compile-time issues in this test harness.
#ifndef BTREE_AUTOVACUUM_NONE
#define BTREE_AUTOVACUUM_NONE 0
#endif
#ifndef BTREE_AUTOVACUUM_FULL
#define BTREE_AUTOVACUUM_FULL 1
#endif
#ifndef BTREE_AUTOVACUUM_INCR
#define BTREE_AUTOVACUUM_INCR 2
#endif

// Declarations for the SQLite B-tree API used by the focal method.
// We declare them extern "C" to match the project's linkage conventions.
extern "C" {
  // The actual implementation in btree.c provides sqlite3BtreeEnter/Leave hooks.
  // We declare them here to satisfy linkage; their definitions are provided by the library.
  void sqlite3BtreeEnter(Btree *p);
  void sqlite3BtreeLeave(Btree *p);

  // The focal function under test
  int sqlite3BtreeGetAutoVacuum(Btree *p);
}

// Simple non-terminating test harness (does not exit on failure).
static int g_total = 0;
static int g_passed = 0;

#define EXPECT_EQ(expected, actual, msg) do { \
    ++g_total; \
    if ((expected) == (actual)) { \
        ++g_passed; \
    } else { \
        std::cerr << "[FAIL] " << (msg) \
                  << " | expected: " << (expected) \
                  << " got: " << (actual) << "\n"; \
    } \
} while (0)


// Helper to construct a Btree instance with an associated BtShared state.
// We rely on the project-provided layout to avoid undefined behavior.
static Btree* makeBtreeWithAutoVacuum(int autoVacuum, int incrVacuum) {
    // BtShared holds the auto vacuum state flags used by sqlite3BtreeGetAutoVacuum
    BtShared *pBt = new BtShared;
    // Some SQLite builds may initialize to zero, but we explicitly set for clarity
    pBt->autoVacuum = autoVacuum;
    pBt->incrVacuum = incrVacuum;

    // Btree instance; the project defines Btree with a member BtShared *pBt
    Btree *p = new Btree;
    p->pBt = pBt;

    return p;
}

// Test 1: True/false branches for core predicate logic (normal compile with autovacuum enabled)
static void test_autovacuum_normal_paths() {
    // Case A: autoVacuum == false (0) -> BTREE_AUTOVACUUM_NONE
    {
        Btree *p = makeBtreeWithAutoVacuum(0, 0);
        int rc = sqlite3BtreeGetAutoVacuum(p);
        EXPECT_EQ(BTREE_AUTOVACUUM_NONE, rc, "autovacuum==0 should return BTREE_AUTOVACUUM_NONE");
        delete p->pBt;
        delete p;
    }

    // Case B: autoVacuum == true, incrVacuum == false -> BTREE_AUTOVACUUM_FULL
    {
        Btree *p = makeBtreeWithAutoVacuum(1, 0);
        int rc = sqlite3BtreeGetAutoVacuum(p);
        EXPECT_EQ(BTREE_AUTOVACUUM_FULL, rc, "autovacuum==1 && incrVacuum==0 should return BTREE_AUTOVACUUM_FULL");
        delete p->pBt;
        delete p;
    }

    // Case C: autoVacuum == true, incrVacuum == true -> BTREE_AUTOVACUUM_INCR
    {
        Btree *p = makeBtreeWithAutoVacuum(1, 1);
        int rc = sqlite3BtreeGetAutoVacuum(p);
        EXPECT_EQ(BTREE_AUTOVACUUM_INCR, rc, "autovacuum==1 && incrVacuum==1 should return BTREE_AUTOVACUUM_INCR");
        delete p->pBt;
        delete p;
    }
}

// Test 2: OMIT path (compile-time branch when SQLITE_OMIT_AUTOVACUUM is defined)
// This test is compiled only when the project defines SQLITE_OMIT_AUTOVACUUM.
// It asserts that the function returns BTREE_AUTOVACUUM_NONE regardless of internal state.
#ifdef SQLITE_OMIT_AUTOVACUUM
static void test_autovacuum_omit_path() {
    // If OMIT path is compiled, the function short-circuits to NONE
    Btree *p = makeBtreeWithAutoVacuum(1, 1);
    int rc = sqlite3BtreeGetAutoVacuum(p);
    EXPECT_EQ(BTREE_AUTOVACUUM_NONE, rc, "OMIT_AUTOVACUUM should always return BTREE_AUTOVACUUM_NONE");
    delete p->pBt;
    delete p;
}
#endif

// Entrypoint for test execution
int main() {
    std::cout << "sqlite3BtreeGetAutoVacuum unit tests (C++11 harness)\n";

    // Run normal-path tests (these execute when SQLITE_OMIT_AUTOVACUUM is not defined)
    test_autovacuum_normal_paths();

#ifdef SQLITE_OMIT_AUTOVACUUM
    // Run OMIT-path tests when the macro is defined
    test_autovacuum_omit_path();
#endif

    // Summary
    std::cout << "Test results: " << g_passed << " / " << g_total << " tests passed.\n";
    return (g_total == g_passed) ? 0 : 1;
}
// Unit tests for the focal method sqlite3GetVTable
// This test suite is crafted to work without GTest and relies on a small custom test harness.
// It provides minimal mockups of the necessary SQLite-style types to exercise the logic.

#include <iostream>
#include <sqliteInt.h>
#include <cassert>


// Simple test harness (non-terminating assertions)
static int g_failures = 0;
#define FAIL(msg) do { ++g_failures; std::cerr << "FAIL: " << msg << " (" << __FILE__ << ":" << __LINE__ << ")\n"; } while(0)
#define EXPECT_TRUE(cond, msg) do { if(!(cond)) { FAIL(msg); } } while(0)
#define EXPECT_PTR_EQ(a, b, msg) do { if((a) != (b)) { FAIL(msg); } } while(0)
#define EXPECT_NULL(a, msg) do { if((a) != nullptr) { FAIL(msg); } } while(0)

// Minimal mock definitions to simulate the environment for sqlite3GetVTable

// Forward declare sqlite3 type (opaque in this test)
struct sqlite3 {};

// VTable structure used by sqlite3GetVTable
struct VTable {
  sqlite3 *db;
  VTable *pNext;
  VTable(sqlite3 *d=nullptr, VTable *n=nullptr): db(d), pNext(n) {}
};

// Table structure with nested members to satisfy pTab->u.vtab.p access.
// We mirror the structure layout used in sqlite3GetVTable:
//   pTab->u.vtab.p
struct Table {
  struct {
    struct {
      VTable *p;
    } vtab;
  } u;
};

// IsVirtual predicate (in the real code, this would check if the table is a virtual table)
static bool IsVirtual(const Table *pTab) {
  // In tests we ensure virtual-ness is always true
  return true;
}

// The focal method under test (re-implemented here for the unit test environment)
VTable *sqlite3GetVTable(sqlite3 *db, Table *pTab){
  VTable *pVtab;
  assert( IsVirtual(pTab) );
  for(pVtab=pTab->u.vtab.p; pVtab && pVtab->db!=db; pVtab=pVtab->pNext);
  return pVtab;
}

// Test 1: No VTable chain present -> should return NULL
// Rationale: Ensure function handles null p pointer gracefully (no entries to traverse).
void test_no_vtab_returns_null() {
  // Prepare a tab with no virtual table
  Table tab;
  tab.u.vtab.p = nullptr;

  sqlite3 db;
  VTable *res = sqlite3GetVTable(&db, &tab);

  // Expect NULL when there is no VTable in the chain
  EXPECT_NULL(res, "Expected NULL when there is no VTable in the chain");
}

// Test 2: Single VTable in list that matches the database
// Rationale: The function should return the first (and only) matching VTable when db matches.
void test_single_matching_vtab_returns_head() {
  sqlite3 db;
  VTable *v = new VTable(&db, nullptr);

  Table tab;
  tab.u.vtab.p = v;

  VTable *res = sqlite3GetVTable(&db, &tab);

  EXPECT_PTR_EQ(res, v, "Expected to return the single VTable when db matches");
  delete v;
}

// Test 3: Multiple VTables with the matching one not at head
// Rationale: Ensure traversal stops at the first VTable whose db matches the provided db.
void test_traversal_to_matching_vtab() {
  sqlite3 dbA, dbB;
  VTable *v1 = new VTable(&dbA, nullptr); // not matching
  VTable *v2 = new VTable(&dbB, nullptr); // matching

  v1->pNext = v2;

  Table tab;
  tab.u.vtab.p = v1;

  VTable *res = sqlite3GetVTable(&dbB, &tab);

  EXPECT_PTR_EQ(res, v2, "Expected to traverse to and return the matching VTable");

  delete v1;
  delete v2;
}

// Test 4: No VTable matches the provided db -> should return NULL
// Rationale: Ensure function returns NULL when none of the VTables match.
void test_no_matching_vtab_returns_null() {
  sqlite3 dbTarget;
  sqlite3 db1, db2;
  VTable *v1 = new VTable(&db1, nullptr);
  VTable *v2 = new VTable(&db2, nullptr);
  v1->pNext = v2;

  Table tab;
  tab.u.vtab.p = v1;

  VTable *res = sqlite3GetVTable(&dbTarget, &tab);

  EXPECT_NULL(res, "Expected NULL when no VTable matches the target db");

  delete v1;
  delete v2;
}

// Test 5: pTab with a non-null head but first element matches
// Rationale: Ensure that when the head matches, the function returns immediately without traversing.
void test_head_matches_without_traversal() {
  sqlite3 db;
  VTable *v = new VTable(&db, nullptr);

  Table tab;
  tab.u.vtab.p = v;

  VTable *res = sqlite3GetVTable(&db, &tab);

  EXPECT_PTR_EQ(res, v, "Expected to return the head VTable when it matches immediately");

  delete v;
}

// Run all tests
int main() {
  test_no_vtab_returns_null();
  test_single_matching_vtab_returns_head();
  test_traversal_to_matching_vtab();
  test_no_matching_vtab_returns_null();
  test_head_matches_without_traversal();

  if (g_failures == 0) {
    std::cout << "All tests passed.\n";
  } else {
    std::cerr << g_failures << " test(s) failed.\n";
  }
  return g_failures;
}
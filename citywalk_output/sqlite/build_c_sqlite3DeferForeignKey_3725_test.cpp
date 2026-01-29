// High-coverage unit tests for sqlite3DeferForeignKey (reproduced in test harness)
// This test suite is designed to be run with a C++11 compiler without GoogleTest.
// It mocks the minimal environment required by sqlite3DeferForeignKey and verifies
// true/false branches and basic assignment behavior.

#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cassert>


// Typedefs and structures to mimic the minimal subset used by sqlite3DeferForeignKey
typedef uint8_t u8;

// Forward declaration of IsOrdinaryTable used by the focal method
struct Table;
bool IsOrdinaryTable(Table*);

// Macro to mimic SQLite's NEVER (for test simplicity we keep it a direct predicate)
#ifndef NEVER
#define NEVER(x) (x)
#endif

// The focal method under test (reproduced for unit testing)
void sqlite3DeferForeignKey(struct Parse *pParse, int isDeferred){
#ifndef SQLITE_OMIT_FOREIGN_KEY
  struct Table *pTab;
  struct FKey *pFKey;
  if( (pTab = pParse->pNewTable)==0 ) return;
  if( NEVER(!IsOrdinaryTable(pTab)) ) return;
  if( (pFKey = pTab->u.tab.pFKey)==0 ) return;
  assert( isDeferred==0 || isDeferred==1 ); /* EV: R-30323-21917 */
  pFKey->isDeferred = (u8)isDeferred;
#endif
}

// Minimal FKey structure
struct FKey {
  u8 isDeferred;
};

// Minimal Table structure to satisfy pTab->u.tab.pFKey usage in focal method
struct Table {
  // 'u' with a member 'tab' containing pFKey as in the original code path:
  struct {
    FKey* pFKey;
  } tab;
  struct {
    // Nested 'tab' to mirror pTab->u.tab.pFKey access
    struct {
      FKey* pFKey;
    } tab;
  } u;
};

// Minimal Parse structure with pNewTable
struct Parse {
  Table* pNewTable;
};

// Global flag to control IsOrdinaryTable behavior in tests
static bool g_IsOrdinaryTableReturnValue = true;

// Mocked IsOrdinaryTable function used by the focal method
bool IsOrdinaryTable(Table* pTab) {
  (void)pTab; // suppress unused warning in some test configurations
  return g_IsOrdinaryTableReturnValue;
}

// Simple test harness utilities
static int g_failures = 0;

static void expect_true(bool cond, const char* msg) {
  if(!cond){
    std::cerr << "EXPECT_TRUE failed: " << msg << "\n";
    ++g_failures;
  }
}
static void expect_eq(int a, int b, const char* msg) {
  if(a != b){
    std::cerr << "EXPECT_EQ failed: " << msg << " | expected " << a << " got " << b << "\n";
    ++g_failures;
  }
}

// Test 1: When pParse->pNewTable is null, the function should return without dereferencing any pointers.
void test_case1_no_pNewTable() {
  // Explanation: pParse.pNewTable == 0 should short-circuit immediately.
  Parse p;
  p.pNewTable = nullptr;

  sqlite3DeferForeignKey(&p, 1);

  // If we reached here, the function handled the null gracefully.
  expect_true(true, "Test 1: no crash when pNewTable is null");
}

// Test 2: When the object is not an ordinary table (IsOrdinaryTable returns false), function returns early.
void test_case2_not_ordinary_table() {
  // Arrange: force IsOrdinaryTable to return false
  g_IsOrdinaryTableReturnValue = false;

  FKey fk;
  fk.isDeferred = 0x5A; // some sentinel value to verify no modification
  Table tab;
  tab.u.tab.pFKey = &fk;        // pTab->u.tab.pFKey is non-null, but IsOrdinaryTable will say not ordinary
  Parse p;
  p.pNewTable = &tab;

  sqlite3DeferForeignKey(&p, 1);

  // Assert: fk should remain unchanged
  expect_eq((int)fk.isDeferred, 0x5A, "Test 2: isDeferred should not be modified when not ordinary");
  // Cleanup: restore behavior for subsequent tests
  g_IsOrdinaryTableReturnValue = true;
}

// Test 3: When the table is ordinary but has no foreign key (pFKey == nullptr), function returns early.
void test_case3_no_pFKey() {
  g_IsOrdinaryTableReturnValue = true;

  Table tab;
  tab.u.tab.pFKey = nullptr; // no FKey attached
  Parse p;
  p.pNewTable = &tab;

  sqlite3DeferForeignKey(&p, 0);

  // If function returns safely, nothing to assert for pFKey (it's null). We just ensure no crash.
  expect_true(true, "Test 3: no crash when pFKey is null");
}

// Test 4: When all prerequisites are met, isDeferred value should be set to 0.
void test_case4_set_deferred_zero() {
  g_IsOrdinaryTableReturnValue = true;

  FKey fk;
  fk.isDeferred = 0xAA; // initial value to ensure it changes
  Table tab;
  tab.u.tab.pFKey = &fk;

  Parse p;
  p.pNewTable = &tab;

  sqlite3DeferForeignKey(&p, 0);

  expect_eq((int)fk.isDeferred, 0, "Test 4: isDeferred should be set to 0");
}

// Test 5: When all prerequisites are met, isDeferred value should be set to 1.
void test_case5_set_deferred_one() {
  g_IsOrdinaryTableReturnValue = true;

  FKey fk;
  fk.isDeferred = 0;
  Table tab;
  tab.u.tab.pFKey = &fk;

  Parse p;
  p.pNewTable = &tab;

  sqlite3DeferForeignKey(&p, 1);

  expect_eq((int)fk.isDeferred, 1, "Test 5: isDeferred should be set to 1");
}

// Main: run all tests (as per domain knowledge, test methods are invoked from main)
int main() {
  // Run all defined test cases
  test_case1_no_pNewTable();
  test_case2_not_ordinary_table();
  test_case3_no_pFKey();
  test_case4_set_deferred_zero();
  test_case5_set_deferred_one();

  if(g_failures != 0){
    std::cerr << "Total failures: " << g_failures << "\n";
  } else {
    std::cout << "All tests passed.\n";
  }
  return g_failures ? 1 : 0;
}
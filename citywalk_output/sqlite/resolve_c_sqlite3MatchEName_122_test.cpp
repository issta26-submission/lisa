// Unit test suite for sqlite3MatchEName (resolve.c) using a lightweight C++11 harness.
// This suite exercises the focal function by leveraging the project's internal
// sqliteInt.h definitions (ExprList_item, ENAME_TAB, ENAME_ROWID, etc.).
// The tests avoid terminating on failure and provide explanatory comments for each case.

#include <iostream>
#include <sqliteInt.h>
#include <string>


// Import necessary SQLite internal definitions.
// The test environment is expected to provide sqliteInt.h in the include path.
extern "C" {
}

// Declaration of the focal function (C linkage).
extern "C" int sqlite3MatchEName(
  const struct ExprList_item *pItem,
  const char *zCol,
  const char *zTab,
  const char *zDb,
  int *pbRowid
);

static int g_failures = 0;

// Lightweight assertion helper: records failure but does not abort tests.
#define EXPECT_TRUE(cond, msg) \
  do { \
    if(!(cond)) { \
      std::cerr << "[FAIL] " << msg << "\n"; \
      ++g_failures; \
    } \
  } while(false)

// Helper to construct an ExprList_item used by sqlite3MatchEName tests.
// Assumes sqliteInt.h provides the exact layout with 'fg.eEName' and 'zEName'.
static ExprList_item makeExprListItem(int eName, const char* zEName) {
  ExprList_item item;
  item.fg.eEName = eName;
  item.zEName = zEName;
  return item;
}

// Test 1: True branch for ENAME_TAB with matching db, tab, and column.
// pbRowid is NULL to exercise the guard that pbRowid==0 is allowed via pointer NULL.
static void test_tab_match_basic() {
  ExprList_item item = makeExprListItem(ENAME_TAB, "db.tab.col");
  const char* zCol = "col";
  const char* zTab = "tab";
  const char* zDb  = "db";
  int* pbRowid = nullptr;

  int res = sqlite3MatchEName(&item, zCol, zTab, zDb, pbRowid);
  EXPECT_TRUE(res == 1, "test_tab_match_basic should return 1 for matching db.tab.col with ENAME_TAB");
}

// Test 2: ENAME_ROWID path sets pbRowid when zCol is a numeric rowid (digits only).
static void test_rowid_sets_flag() {
  ExprList_item item = makeExprListItem(ENAME_ROWID, "db.tab.col");
  const char* zCol = "123"; // digits -> valid rowid
  const char* zTab = "tab";
  const char* zDb  = "db";
  int rowid = 0;

  int res = sqlite3MatchEName(&item, zCol, zTab, zDb, &rowid);
  EXPECT_TRUE(res == 1, "test_rowid_sets_flag should return 1 for valid ENAME_ROWID with numeric zCol");
  EXPECT_TRUE(rowid == 1, "test_rowid_sets_flag should set pbRowid to 1 when ENAME_ROWID is used");
}

// Test 3: False branch when eEName is not ENAME_TAB and not ENAME_ROWID (non-matching enum).
// We use a clearly out-of-band value (eName = 9999) to force the early return path.
static void test_invalid_eName_paths() {
  ExprList_item item = makeExprListItem(9999, "db.tab.col");
  int rowid = 0;
  int res = sqlite3MatchEName(&item, "col", "tab", "db", &rowid);
  EXPECT_TRUE(res == 0, "test_invalid_eName_paths should return 0 for an unknown eEName value");
}

// Test 4: Mismatching database name should fail (db mismatch).
static void test_bad_db() {
  ExprList_item item = makeExprListItem(ENAME_TAB, "notdb.tab.col");
  int rowid = 0;
  int res = sqlite3MatchEName(&item, "col", "tab", "db", &rowid);
  EXPECT_TRUE(res == 0, "test_bad_db should return 0 when zDb does not match the prefix in zEName");
}

// Test 5: Mismatching table name should fail (tab mismatch).
static void test_bad_tab() {
  ExprList_item item = makeExprListItem(ENAME_TAB, "db.wrongtab.col");
  int rowid = 0;
  int res = sqlite3MatchEName(&item, "col", "tab", "db", &rowid);
  EXPECT_TRUE(res == 0, "test_bad_tab should return 0 when zTab does not match the second segment");
}

// Test 6: Mismatching column when ENAME_TAB (zCol provided) should fail.
static void test_bad_col_for_tab() {
  ExprList_item item = makeExprListItem(ENAME_TAB, "db.tab.col");
  int rowid = 0;
  int res = sqlite3MatchEName(&item, "othercol", "tab", "db", &rowid);
  EXPECT_TRUE(res == 0, "test_bad_col_for_tab should return 0 when zCol does not match final segment for ENAME_TAB");
}

// Run all tests and report overall status.
int main() {
  std::cout << "Starting sqlite3MatchEName unit tests (C++ harness)\n";

  test_tab_match_basic();
  test_rowid_sets_flag();
  test_invalid_eName_paths();
  test_bad_db();
  test_bad_tab();
  test_bad_col_for_tab();

  if(g_failures == 0) {
    std::cout << "ALL TESTS PASSED\n";
  } else {
    std::cout << g_failures << " TEST(S) FAILED\n";
  }

  return g_failures;
}
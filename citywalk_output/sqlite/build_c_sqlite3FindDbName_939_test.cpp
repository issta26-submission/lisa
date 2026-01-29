// High-quality C++11 unit tests for the focal method sqlite3FindDbName
// Notes:
// - The tests are designed to exercise the sqlite3FindDbName logic as implemented in the provided focal method.
// - We assume the project under test exposes sqlite3FindDbName with a compatible signature and internal types.
// - This test suite uses a lightweight, framework-free approach (no Google Test); it uses simple EXPECT-like macros
//   and a small test runner built into main().
// - We provide minimal stubs for sqlite3_stricmp so the focal function can be exercised in isolation when
//   linking against the code under test. The test definitions here are intended to be compiled and linked
//   with the project under test in the evaluator environment.

#include <iostream>
#include <cstring>
#include <sqliteInt.h>
#include <cctype>


// Declaration of the focal function under test (provided by the codebase under test)
extern "C" int sqlite3FindDbName(struct sqlite3 *db, const char *zName);

// Minimal type stubs matching the usage inside sqlite3FindDbName.
// Rely on the test environment to provide the real definition of sqlite3FindDbName; the layout here
// must be compatible enough to form valid db/aDb structures for test scaffolding.
struct Db {
  const char *zDbSName; // database name string (as used by sqlite3FindDbName)
};

struct sqlite3 {
  int nDb;     // number of databases
  Db *aDb;     // array of databases
};

// Minimal, self-contained, case-insensitive string compare used by sqlite3FindDbName.
// We provide a simple implementation here to satisfy the linker if the real symbol is not available.
extern "C" int sqlite3_stricmp(const char *zLeft, const char *zRight) {
  // Handle nulls conservatively: treat null as less-than non-null
  if (zLeft == nullptr && zRight == nullptr) return 0;
  if (zLeft == nullptr) return -1;
  if (zRight == nullptr) return 1;

  while (*zLeft && *zRight) {
    unsigned char a = static_cast<unsigned char>(std::tolower(static_cast<unsigned char>(*zLeft)));
    unsigned char b = static_cast<unsigned char>(std::tolower(static_cast<unsigned char>(*zRight)));
    if (a != b) return (int)a - (int)b;
    ++zLeft; ++zRight;
  }
  return (int)std::tolower(static_cast<unsigned char>(*zLeft)) - (int)std::tolower(static_cast<unsigned char>(*zRight));
}

// Lightweight test harness

static int g_total_failures = 0;

// Expectation macro: non-terminating (continues executing even on failure)
#define EXPECT_EQ_INT(expected, actual) do { \
    int _e = (int)(expected); \
    int _a = (int)(actual); \
    if (_e != _a) { \
        std::cerr << "EXPECT_EQ failed: expected " << _e << " but got " << _a \
                  << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_total_failures; \
    } \
} while(0)

// Helper: print test header
#define TEST_HEADER(name) std::cout << "Running test: " << #name << std::endl;

// Test 1: Last database in the array is found when zName matches exactly.
// This validates the main path where aDb[i].zDbSName matches zName at the greatest index.
bool test_sqlite3FindDbName_matches_last_index() {
  TEST_HEADER(test_sqlite3FindDbName_matches_last_index);

  // Prepare a db with 3 databases: ["alpha", "beta", "gamma"]
  Db dbEntries[3];
  dbEntries[0].zDbSName = "alpha";
  dbEntries[1].zDbSName = "beta";
  dbEntries[2].zDbSName = "gamma";

  sqlite3 db;
  db.nDb = 3;
  db.aDb = dbEntries;

  // zName matches the last entry "gamma" -> expect index 2
  int res = sqlite3FindDbName(&db, "gamma");
  EXPECT_EQ_INT(2, res);

  return g_total_failures == 0;
}

// Test 2: Case-insensitive match against an existing entry (e.g., "BETa" matches "beta").
// Validates the branch that uses sqlite3_stricmp for comparison.
bool test_sqlite3FindDbName_case_insensitive_match() {
  TEST_HEADER(test_sqlite3FindDbName_case_insensitive_match);

  // Prepare a db with 3 databases: ["alpha", "beta", "gamma"]
  Db dbEntries[3];
  dbEntries[0].zDbSName = "alpha";
  dbEntries[1].zDbSName = "beta";
  dbEntries[2].zDbSName = "gamma";

  sqlite3 db;
  db.nDb = 3;
  db.aDb = dbEntries;

  // zName matches "beta" case-insensitively -> expect index 1
  int res = sqlite3FindDbName(&db, "BETa");
  EXPECT_EQ_INT(1, res);

  return g_total_failures == 0;
}

// Test 3: zName does not exist and is non-null; should return -1 (not found).
bool test_sqlite3FindDbName_not_found() {
  TEST_HEADER(test_sqlite3FindDbName_not_found);

  // Prepare a db with 3 databases: ["alpha", "beta", "gamma"]
  Db dbEntries[3];
  dbEntries[0].zDbSName = "alpha";
  dbEntries[1].zDbSName = "beta";
  dbEntries[2].zDbSName = "gamma";

  sqlite3 db;
  db.nDb = 3;
  db.aDb = dbEntries;

  int res = sqlite3FindDbName(&db, "notexist");
  EXPECT_EQ_INT(-1, res);

  return g_total_failures == 0;
}

// Test 4: zName is NULL; should return -1 according to the function's guard.
bool test_sqlite3FindDbName_null_name() {
  TEST_HEADER(test_sqlite3FindDbName_null_name);

  // Prepare a db with 2 databases: ["alpha", "beta"]
  Db dbEntries[2];
  dbEntries[0].zDbSName = "alpha";
  dbEntries[1].zDbSName = "beta";

  sqlite3 db;
  db.nDb = 2;
  db.aDb = dbEntries;

  int res = sqlite3FindDbName(&db, nullptr);
  EXPECT_EQ_INT(-1, res);

  return g_total_failures == 0;
}

// Test 5: zName equals "main" and there is no "main" alias in the table.
// This exercises the special-case: if i==0 and zName equals "main", break and return 0.
bool test_sqlite3FindDbName_main_alias_when_no_match() {
  TEST_HEADER(test_sqlite3FindDbName_main_alias_when_no_match);

  // Prepare a db with 3 databases: ["alpha", "beta", "gamma"]
  Db dbEntries[3];
  dbEntries[0].zDbSName = "alpha";
  dbEntries[1].zDbSName = "beta";
  dbEntries[2].zDbSName = "gamma";

  sqlite3 db;
  db.nDb = 3;
  db.aDb = dbEntries;

  // zName is "main" and there's no "main" in the array -> should return 0
  int res = sqlite3FindDbName(&db, "main");
  EXPECT_EQ_INT(0, res);

  return g_total_failures == 0;
}

// Test 6: Mixed case: ensure "main" alias works when there is no exact match but i reaches 0.
// This is essentially a robustness check for the alias behavior.
bool test_sqlite3FindDbName_main_alias_at_zero() {
  TEST_HEADER(test_sqlite3FindDbName_main_alias_at_zero);

  // Prepare a db with 1 database: ["only"]
  Db dbEntries[1];
  dbEntries[0].zDbSName = "only";

  sqlite3 db;
  db.nDb = 1;
  db.aDb = dbEntries;

  // zName is "MAIN" (case-insensitive alias) -> should break at i==0 and return 0
  int res = sqlite3FindDbName(&db, "MAIN");
  EXPECT_EQ_INT(0, res);

  return g_total_failures == 0;
}

// Main: run all tests and report summary
int main() {
  // Reset global failure counter
  g_total_failures = 0;

  bool ok;

  ok = test_sqlite3FindDbName_matches_last_index();
  if (!ok) std::cerr << "Test 1 failed.\n";

  ok = test_sqlite3FindDbName_case_insensitive_match();
  if (!ok) std::cerr << "Test 2 failed.\n";

  ok = test_sqlite3FindDbName_not_found();
  if (!ok) std::cerr << "Test 3 failed.\n";

  ok = test_sqlite3FindDbName_null_name();
  if (!ok) std::cerr << "Test 4 failed.\n";

  ok = test_sqlite3FindDbName_main_alias_when_no_match();
  if (!ok) std::cerr << "Test 5 failed.\n";

  ok = test_sqlite3FindDbName_main_alias_at_zero();
  if (!ok) std::cerr << "Test 6 failed.\n";

  if (g_total_failures == 0) {
    std::cout << "All tests passed successfully." << std::endl;
    return 0;
  } else {
    std::cout << "Total failures: " << g_total_failures << std::endl;
    return 1;
  }
}
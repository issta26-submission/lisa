// Test suite for the focal method: sqlite3CommitInternalChanges
// This test is implemented in C++11 (no GTest) and targets the C function
// void sqlite3CommitInternalChanges(sqlite3 *db) which clears the
// DBFLAG_SchemaChange bit from db->mDbFlags.
// The tests rely on a minimal sqlite3 stub with an mDbFlags member.
// Explanatory notes are provided for each test case.

// Domain knowledge alignment:
// - Candidate Keywords/Dependencies: sqlite3CommitInternalChanges, sqlite3, mDbFlags, DBFLAG_SchemaChange
// - Test goals: verify bit clearing when flag is set, verify no change when flag not set,
//   ensure other bits remain unaffected, and exercise basic data-type integrity.
// - Constraints: non-GTest, non-terminating assertions, no private member access, use only standard C++ lib.

#include <iostream>
#include <sqliteInt.h>
#include <string>


// Step 1 (for test design context): declare the focal C function with C linkage.
// We provide a minimal sqlite3 type compatible with the function signature.
// Note: The actual project provides a fuller sqlite3 type; for testing, we assume
// the first (and only necessary) member used by sqlite3CommitInternalChanges is mDbFlags.
struct sqlite3_dummy {
  unsigned int mDbFlags; // This is the field manipulated by sqlite3CommitInternalChanges
};
typedef struct sqlite3_dummy sqlite3;

// Forward declaration of the focal function (as implemented in the project under test).
extern "C" void sqlite3CommitInternalChanges(sqlite3 *db);

// Utility: simple result recording (non-terminating assertion style)
struct TestResult {
  std::string name;
  bool passed;
  std::string message;
};

static bool assert_eq_unsigned(const char* testName, unsigned int expected, unsigned int actual, std::string &outMsg) {
  bool ok = (expected == actual);
  if (!ok) {
    outMsg = std::string("Assertion failed in ") + testName +
             std::string(": expected ") + std::to_string(expected) +
             std::string(", got ") + std::to_string(actual);
  }
  return ok;
}

// Test 1: When the schema change flag (DBFLAG_SchemaChange == 0x01 in SQLite) is set,
// sqlite3CommitInternalChanges should clear it (db->mDbFlags becomes 0).
bool test_commit_clears_flag_when_set() {
  // Domain-specific: initialize with only the schema-change flag set
  sqlite3 db;
  db.mDbFlags = 0x01; // assume DBFLAG_SchemaChange == 0x01
  // Act
  sqlite3CommitInternalChanges(&db);
  // Assert: the flag should be cleared; all other bits (none here) untouched
  unsigned int expected = 0x00;
  std::string msg;
  bool ok = assert_eq_unsigned("test_commit_clears_flag_when_set", expected, db.mDbFlags, msg);
  if (!ok) {
    std::cout << msg << std::endl;
  }
  return ok;
}

// Test 2: When the schema change flag is not set, the function should leave mDbFlags unchanged.
bool test_commit_no_change_when_flag_not_set() {
  sqlite3 db;
  db.mDbFlags = 0x00; // no schema change flag set
  sqlite3CommitInternalChanges(&db);
  unsigned int expected = 0x00;
  std::string msg;
  bool ok = assert_eq_unsigned("test_commit_no_change_when_flag_not_set", expected, db.mDbFlags, msg);
  if (!ok) {
    std::cout << msg << std::endl;
  }
  return ok;
}

// Test 3: Ensure that other unrelated bits remain unaffected by the operation.
// Set a non-schema-change bit (e.g., bit 1) and verify it remains after the call.
bool test_commit_keeps_other_bits_intact() {
  sqlite3 db;
  db.mDbFlags = 0x02; // some unrelated bit is set
  sqlite3CommitInternalChanges(&db);
  unsigned int expected = 0x02;
  std::string msg;
  bool ok = assert_eq_unsigned("test_commit_keeps_other_bits_intact", expected, db.mDbFlags, msg);
  if (!ok) {
    std::cout << msg << std::endl;
  }
  return ok;
}

// Main: run all tests and report summary.
// As per the domain knowledge guidance, we invoke test methods from main and print results.
int main() {
  int total = 0;
  int passed = 0;

  // Collect and run tests
  bool r1 = test_commit_clears_flag_when_set();
  total++; if (r1) passed++;

  bool r2 = test_commit_no_change_when_flag_not_set();
  total++; if (r2) passed++;

  bool r3 = test_commit_keeps_other_bits_intact();
  total++; if (r3) passed++;

  // Summary
  std::cout << "\nTest summary: " << passed << " / " << total << " tests passed." << std::endl;

  // Return non-zero if any test failed (but do not terminate abruptly)
  return (passed == total) ? 0 : 1;
}
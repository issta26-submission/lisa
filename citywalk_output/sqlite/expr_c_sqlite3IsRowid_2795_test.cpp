// Test suite for the focal method: sqlite3IsRowid
// This test suite is written for C++11, without any external testing framework (GTest not used).
// It relies on the project build to provide the actual implementation of sqlite3IsRowid
// and sqlite3StrICmp (as used inside sqlite3IsRowid). We declare them with C linkage.

#include <iostream>
#include <sqliteInt.h>
#include <string>


// Forward declarations of the focal method and its dependency (as provided by the project).
extern "C" int sqlite3IsRowid(const char *z);
// sqlite3StrICmp is used inside sqlite3IsRowid; we assume it is provided by the project.
// If linking to a separate SQLite library, this declaration ensures correct linkage.
extern "C" int sqlite3StrICmp(const char *zLeft, const char *zRight);

// Simple non-terminating assertion utilities.
// They print failures but do not abort the test suite, allowing full coverage.
static int g_tests = 0;
static int g_failures = 0;

#define ASSERT_EQ(expected, actual) do { \
  ++g_tests; \
  if ((expected) != (actual)) { \
    ++g_failures; \
    std::cerr << "Assertion failed at line " << __LINE__ << ": expected " \
              << (expected) << ", got " << (actual) << std::endl; \
  } \
} while (0)


// Test 1: Verify that exact core identifiers are recognized.
// This exercises the first three condition branches by providing exact matches.
static void test_exact_identifiers() {
  // "_ROWID_"
  ASSERT_EQ(1, sqlite3IsRowid("_ROWID_"));
  // "ROWID"
  ASSERT_EQ(1, sqlite3IsRowid("ROWID"));
  // "OID"
  ASSERT_EQ(1, sqlite3IsRowid("OID"));
  // Explanatory: These inputs should trigger the corresponding sqlite3StrICmp checks
  // and return 1 from sqlite3IsRowid.
}

// Test 2: Verify that identifiers are matched case-insensitively.
// This ensures the function is not case-sensitive as intended.
static void test_case_insensitive_matches() {
  // lowercase variants
  ASSERT_EQ(1, sqlite3IsRowid("_rowid_"));
  ASSERT_EQ(1, sqlite3IsRowid("rowid"));
  ASSERT_EQ(1, sqlite3IsRowid("oid"));

  // mixed-case variant for each identifier
  ASSERT_EQ(1, sqlite3IsRowid("ROWId")); // last character 'd' vs 'D' should still match
  ASSERT_EQ(1, sqlite3IsRowid("_ROWid_")); // mixed case with underscores
}

// Test 3: Verify false/negative cases (inputs that should not be recognized as rowid aliases).
static void test_false_negatives() {
  // Space and unusual characters
  ASSERT_EQ(0, sqlite3IsRowid("ROW ID"));
  // Non-identical character substitution
  ASSERT_EQ(0, sqlite3IsRowid("R0WID")); // zero instead of 'O'
  // Empty string should not be recognized
  ASSERT_EQ(0, sqlite3IsRowid(""));
  // Explanatory: These inputs exercise the false branches of the three if-statements.
}

// Test 4: Additional checks to ensure no accidental matches beyond the three core identifiers.
static void test_no_unexpected_matches() {
  // Similar but not identical identifiers
  ASSERT_EQ(0, sqlite3IsRowid("_ROWID"));     // missing trailing underscore
  ASSERT_EQ(0, sqlite3IsRowid("ROWID_"));     // extra trailing underscore
  ASSERT_EQ(0, sqlite3IsRowid("OIDX"));        // extra character
  // Explanatory: Ensures only exact matches to the three known identifiers yield 1.
}

// Runner to execute all tests and report summary
int main() {
  test_exact_identifiers();
  test_case_insensitive_matches();
  test_false_negatives();
  test_no_unexpected_matches();

  std::cout << "sqlite3IsRowid test results: "
            << g_tests << " tests, "
            << g_failures << " failures." << std::endl;

  // Non-zero exit code on any failure to indicate test suite failure.
  return (g_failures == 0) ? 0 : 1;
}
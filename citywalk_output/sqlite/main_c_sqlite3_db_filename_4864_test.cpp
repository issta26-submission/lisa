// Unit test suite for the focal method: sqlite3_db_filename
// Target: Test sqlite3_db_filename(sqlite3 *db, const char *zDbName) as implemented in main.c
// Assumptions:
// - The test binary will be linked with main.c (which contains sqlite3_db_filename).
// - The macro SQLITE_ENABLE_API_ARMOR may be defined to exercise the safety-check branch.
// - We provide lightweight C++ tests with no external testing framework (no GTest).
// - We mock the dependent symbols: sqlite3SafetyCheckOk, sqlite3DbNameToBtree, sqlite3BtreeGetFilename.
// - All mocks use C linkage (extern "C") to match the focal function expectations.

#include <cstddef>
#include <iostream>
#include <sqliteInt.h>


// Forward declare minimal types to mirror sqlite3's usage in sqlite3_db_filename.
extern "C" {
  // Opaque types used by the focal function and mocks.
  struct sqlite3;
  struct Btree;
  // The focal function under test (assumed to be provided by main.c during linking).
  const char *sqlite3_db_filename(sqlite3 *db, const char *zDbName);

  // Mocks for dependent symbols (to be provided below with C linkage).
  Btree *sqlite3DbNameToBtree(sqlite3 *db, const char *zDbName);
  const char *sqlite3BtreeGetFilename(Btree *pBt);
  int sqlite3SafetyCheckOk(sqlite3 *db);
}

// Lightweight C++ test harness (no GTest). Uses non-terminating assertions.
static int g_test_failures = 0;
#define TEST_FAIL(msg) do { std::cerr << "TEST FAILURE: " << (msg) << std::endl; ++g_test_failures; } while(0)
#define TEST_PASS(msg) do { std::cout << "TEST PASS: " << (msg) << std::endl; } while(0)
#define EXPECT_TRUE(cond, msg) do { if(!(cond)) { TEST_FAIL(msg); } else { /* optional verbose success */ } } while(0)
#define EXPECT_FALSE(cond, msg) do { if((cond)) { TEST_FAIL(msg); } } while(0)
#define EXPECT_PTR_EQ(actual, expected, msg) do { if((actual) != (expected)) { std::cerr << "EXPECTED POINTER EQUALMENT FAILED: " << (msg) 
                                                                                 << " | actual: " << static_cast<const void*>(actual)
                                                                                 << " != expected: " << static_cast<const void*>(expected) << "\n"; 
                                                                               ++g_test_failures; } } while(0)
#define EXPECT_STR_EQ(actual, expected, msg) do { if((actual) == nullptr || std::string(actual) != std::string(expected)) { 
                                                  std::cerr << "EXPECTED STRING EQUALMENT FAILED: " << (msg)
                                                            << " | actual: " << (actual)
                                                            << " != expected: " << (expected) << "\n"; 
                                                  ++g_test_failures; } } while(0)

// Minimal fake sqlite3 type (opaque to test).
struct sqlite3 { int dummy; };

// Mock state (shared across tests)
static int g_safety_ok = 1;
static Btree *g_fakeBtree = nullptr;
static const char *g_fakeFilename = nullptr;

// Mocked dependencies (C linkage to match focal function's expectations)
extern "C" {

  // Simulate safety check outcome. This will influence the early return path
  // when SQLITE_ENABLE_API_ARMOR is defined in the focal compilation unit.
  int sqlite3SafetyCheckOk(sqlite3 *db) {
    // The test can set g_safety_ok to 0 or 1 before invoking the focal function.
    (void)db; // unused in mock
    return g_safety_ok;
  }

  // Return the currently configured fake Btree pointer.
  Btree *sqlite3DbNameToBtree(sqlite3 *db, const char *zDbName) {
    (void)db; (void)zDbName;
    // In a real scenario, the mapping might depend on db/zDbName.
    return g_fakeBtree;
  }

  // Return the filename associated with the fake Btree (only if the pointer matches).
  const char *sqlite3BtreeGetFilename(Btree *pBt) {
    if (pBt == g_fakeBtree) {
      return g_fakeFilename;
    }
    return nullptr;
  }

} // extern "C"

// Helper to reset test mocks to a clean state
static void resetMocks() {
  g_safety_ok = 1;
  g_fakeBtree = nullptr;
  g_fakeFilename = nullptr;
}

// Test Case 1:
// When safety check passes and a valid Btree is returned for the given database name,
// sqlite3_db_filename should return the filename associated with that Btree.
static void test_case_1_filename_returned_when_safe_and_btree_present() {
  resetMocks();

  // Arrange
  g_safety_ok = 1;
  // Create a fake Btree and associated filename
  Btree fakeBt;
  g_fakeBtree = &fakeBt;
  g_fakeFilename = "filename_main.sqlite";

  // Act
  sqlite3 db; // dummy db instance
  const char *res = sqlite3_db_filename(&db, "main");

  // Assert
  EXPECT_TRUE(res != nullptr, "Expected non-null filename when safety OK and Btree present");
  EXPECT_PTR_EQ(reinterpret_cast<void*>(res), reinterpret_cast<void*>(g_fakeFilename),
                "Returned filename pointer should match mocked filename");
  EXPECT_STR_EQ(res, g_fakeFilename, "Returned filename content should match mocked filename");
  TEST_PASS("test_case_1_filename_returned_when_safe_and_btree_present");
}

// Test Case 2:
// When safety check passes but there is no Btree for the given name (sqlite3DbNameToBtree returns null),
// sqlite3_db_filename should return null.
static void test_case_2_null_when_btree_not_found() {
  resetMocks();

  // Arrange
  g_safety_ok = 1;
  g_fakeBtree = nullptr;
  g_fakeFilename = nullptr;

  // Act
  sqlite3 db;
  const char *res = sqlite3_db_filename(&db, "nonexistent_db");

  // Assert
  EXPECT_TRUE(res == nullptr, "Expected null filename when Btree is not found");
  TEST_PASS("test_case_2_null_when_btree_not_found");
}

// Test Case 3:
// When safety check fails (ARMOR enabled), the function should return null immediately
// regardless of the Btree lookup result.
static void test_case_3_null_when_safety_check_fails() {
  resetMocks();

  // Arrange
  g_safety_ok = 0;          // Simulate safety check failure
  Btree fakeBt;
  g_fakeBtree = &fakeBt;
  g_fakeFilename = "should_not_be_used.sqlite";

  // Act
  sqlite3 db;
  const char *res = sqlite3_db_filename(&db, "any_db");

  // Assert
  EXPECT_TRUE(res == nullptr, "Expected null filename when safety check fails (ARMOR path)");
  TEST_PASS("test_case_3_null_when_safety_check_fails");
}

// Entry point for running tests
static void runAllTests() {
  test_case_1_filename_returned_when_safe_and_btree_present();
  test_case_2_null_when_btree_not_found();
  test_case_3_null_when_safety_check_fails();
  if (g_test_failures == 0) {
    std::cout << "ALL TESTS PASSED\n";
  } else {
    std::cout << g_test_failures << " TEST(S) FAILED\n";
  }
}

// Main function: executes test suite from user-provided test runner
int main() {
  runAllTests();
  return (g_test_failures > 0) ? 1 : 0;
}
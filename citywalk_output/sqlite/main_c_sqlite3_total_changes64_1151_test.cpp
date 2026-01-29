// File: test_total_changes64_with_armor.cpp
// Purpose: Unit tests for sqlite3_total_changes64 under API_ARMOR enabled (SQLITE_ENABLE_API_ARMOR defined).
// Notes:
// - This test suite assumes the presence and linkage of the SQLite amalgamation headers (sqlite3.h) and
//   that sqlite3_total_changes64 is available to link against (as produced by main.c in the project).
// - We rely on a controlled stub for sqlite3SafetyCheckOk to drive true/false branches of the API armor.
// - The sqlite3 struct is instantiated using the real header definition to ensure field alignment (nTotalChange).
// - Non-terminating assertions are used to maximize test coverage without early exit.
//
// How to build (example):
//   g++ -std=c++11 test_total_changes64_with_armor.cpp -DSQLITE_ENABLE_API_ARMOR -I<path_to_sqlite_headers> -o test_with_armor
//   (Link against the project's compiled object that provides sqlite3_total_changes64, typically main.o or a lib)

#include <cmath>
#include <sqlite3.h>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>


// Include the SQLite public header to obtain the sqlite3 type and prototypes.
// The test assumes sqlite3_total_changes64 is provided by the project (e.g., compiled from main.c).
extern "C" {
}

// The project provides an API-armor safety check: sqlite3SafetyCheckOk.
// We need a deterministic control for test scenarios. 
// WARNING: If the project also defines sqlite3SafetyCheckOk in the linked objects, this
// override may cause a link-time conflict on some toolchains. If so, adapt by adjusting
// build flags or provide a weak alias in the library.
static bool g_safetyOk = true;

// Provide a test-controlled override for the safety check.
// Note: This symbol may collide with a real implementation if linked. In a controlled test
// environment, ensure the symbol is overridable (e.g., by linking order or weak symbols).
extern "C" int sqlite3SafetyCheckOk(sqlite3 *db) {
  (void)db; // unused param in this override; actual test controls via global flag
  return g_safetyOk ? 1 : 0;
}

// Forward declaration of the focal function under test.
// The function is defined in main.c (as per the project under test); we declare it here for linkage.
extern "C" sqlite3_int64 sqlite3_total_changes64(sqlite3 *db);

// Simple non-terminating assertion helper
static int gFailures = 0;
#define ASSERT_EQ(expected, actual) \
  do { \
    if ((expected) != (actual)) { \
      std::cerr << "Assertion failed: expected " << (expected) << ", got " << (actual) \
                << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
      ++gFailures; \
    } \
  } while (0)

// Test 1: API_ARMOR enabled, SafetyCheckOk returns true -> should return db->nTotalChange
static void test_total_changes64_armor_true() {
  // Allocate a sqlite3 object using the real header layout to ensure proper field access.
  sqlite3 *db = (sqlite3*)std::malloc(sizeof(sqlite3));
  if (!db) {
    std::cerr << "Failed to allocate sqlite3 object" << std::endl;
    ++gFailures;
    return;
  }

  // Initialize with a known value for nTotalChange
  db->nTotalChange = 0x0123456789ABCDEFLL; // large value to ensure full integrity
  g_safetyOk = true; // force SafetyCheckOk to return true

  sqlite3_int64 result = sqlite3_total_changes64(db);

  ASSERT_EQ(0x0123456789ABCDEFLL, result);

  std::free(db);
}

// Test 2: API_ARMOR enabled, SafetyCheckOk returns false -> should return 0
static void test_total_changes64_armor_false() {
  sqlite3 *db = (sqlite3*)std::malloc(sizeof(sqlite3));
  if (!db) {
    std::cerr << "Failed to allocate sqlite3 object" << std::endl;
    ++gFailures;
    return;
  }

  db->nTotalChange = 0xDEADBEEFCAFEBABELL;
  g_safetyOk = false; // force SafetyCheckOk to return false

  sqlite3_int64 result = sqlite3_total_changes64(db);

  ASSERT_EQ(0LL, result);

  std::free(db);
}

// Test runner for API_ARMOR tests
static void run_tests_with_armor() {
  test_total_changes64_armor_true();
  test_total_changes64_armor_false();
}

// Main entry for this test executable
int main() {
  run_tests_with_armor();

  if (gFailures == 0) {
    std::cout << "test_total_changes64_with_armor: all tests passed" << std::endl;
    return 0;
  } else {
    std::cerr << gFailures << " test(s) failed in test_total_changes64_with_armor" << std::endl;
    return 1;
  }
}


// File: test_total_changes64_without_armor.cpp
// Purpose: Unit tests for sqlite3_total_changes64 when API_ARMOR is not enabled.
// This validates the behavior of returning db->nTotalChange directly.
// Notes are similar to the armored test but compile without SQLITE_ENABLE_API_ARMOR.
// 
// How to build (example):
//   g++ -std=c++11 test_total_changes64_without_armor.cpp -I<path_to_sqlite_headers> -o test_without_armor
//   (Link against the project's compiled object that provides sqlite3_total_changes64, typically main.o or a lib)


extern "C" {
}

// The focal function is defined in the project and linked in; declare it here.
extern "C" sqlite3_int64 sqlite3_total_changes64(sqlite3 *db);

// Simple non-terminating assertion helper
static int gFailuresNoArmor = 0;
#define ASSERT_EQ_NOARMOR(expected, actual) \
  do { \
    if ((expected) != (actual)) { \
      std::cerr << "Assertion failed (no armor): expected " << (expected) << ", got " \
                << (actual) << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
      ++gFailuresNoArmor; \
    } \
  } while (0)

// Test: API_ARMOR not enabled, should simply return db->nTotalChange
static void test_total_changes64_no_armor_basic() {
  sqlite3 *db = (sqlite3*)std::malloc(sizeof(sqlite3));
  if (!db) {
    std::cerr << "Failed to allocate sqlite3 object" << std::endl;
    ++gFailuresNoArmor;
    return;
  }

  db->nTotalChange = 0x1234567890ABCDEFLL;

  sqlite3_int64 result = sqlite3_total_changes64(db);

  ASSERT_EQ_NOARMOR(0x1234567890ABCDEFLL, result);

  std::free(db);
}

// Test runner for no-armor tests
static void run_tests_without_armor() {
  test_total_changes64_no_armor_basic();
}

// Main entry for this test executable
int main_no_armor() {
  // This main is separate to avoid name clashes with the armored test's main.
  run_tests_without_armor();

  if (gFailuresNoArmor == 0) {
    std::cout << "test_total_changes64_without_armor: all tests passed" << std::endl;
    return 0;
  } else {
    std::cerr << gFailuresNoArmor << " test(s) failed in test_total_changes64_without_armor" << std::endl;
    return 1;
  }
}

// Note: In most build setups, you would compile this file as a separate executable.
// If your build system requires a single-file test driver, you can merge the two mains
// under distinct namespaces or by using function pointers to invoke test suites.
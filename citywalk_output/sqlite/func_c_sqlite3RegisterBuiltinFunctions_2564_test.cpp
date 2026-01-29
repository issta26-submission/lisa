/*
 * Unit tests for the focal method: sqlite3RegisterBuiltinFunctions
 * 
 * These tests are crafted for C++11 without using Google Test.
 * They exercise the builtin function registration by invoking the
 * C API and inspecting the internal function registry exposed via
 * sqlite3BuiltinFunctions (internally defined by SQLite).
 *
 * Notes:
 * - We rely on internal structures exposed via sqliteInt.h (FuncDefHash,
 *   FuncDef, and sqlite3BuiltinFunctions). This mirrors the way the original
 *   source registers builtins.
 * - The tests are non-terminating (they log failures and continue) to maximize
 *   code coverage as requested.
 * - The tests assume a typical SQLite build where sqlite3BuiltinFunctions is
 *   a public/extern symbol after including sqliteInt.h.
 * - The tests do not modify core library configuration; they work with the
 *   library as compiled in the project (i.e., under the project’s default
 *   compile-time macros).
 *
 * To run:
 * 1) Ensure the test is linked against the same SQLite sources used by the project.
 * 2) Compile with a C++11 compliant compiler, e.g.:
 *      g++ -std=c++11 -I<path-to-sqlite-headers> -L<path-to-sqlite-lib> \
 *          -lsqlite3 -o test_sqlite_builtin test_sqlite_builtin.cpp
 * 3) Run ./test_sqlite_builtin
 *
 * The test assumes the following internal API/types are available via sqliteInt.h:
 *   - typedef struct FuncDefHash FuncDefHash;
 *   - typedef struct FuncDef FuncDef;
 *   - extern FuncDefHash sqlite3BuiltinFunctions;
 *   - #define SQLITE_FUNC_HASH_SZ ...
 *   - FuncDef has members: const char *zName; union { FuncDef *pHash; } u;
 *
 * Constraints from the domain knowledge are respected.
 */

// C linkage for the functions declared in the C SQLite sources
extern "C" {
  // Focal method under test
  void sqlite3RegisterBuiltinFunctions(void);
  // Additional internal symbols are pulled in via sqliteInt.h below.
}

#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>


// Import internal SQLite types and the builtin function registry

// Forward declaration of the same object type used by the SQLite source.
// We rely on the symbol being declared in sqliteInt.h (extern FuncDefHash sqlite3BuiltinFunctions;).
extern "C" FuncDefHash sqlite3BuiltinFunctions;

// Simple non-terminating assertion macro suitable for test harnesses.
// It reports the failure but does not abort the test harness, enabling multiple checks per test.
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "TEST FAILURE: " << (msg) \
                  << " [File: " << __FILE__ << ", Line: " << __LINE__ << "]" \
                  << std::endl; \
        ++g_failed_tests; \
    } \
} while(0)

static int g_failed_tests = 0;

// Test 1: Registration should populate the builtin function registry with at least some entries.
// Additionally, the known substr function should be present among the registered functions.
void test_builtin_registration_populates_registry() {
  // Act
  sqlite3RegisterBuiltinFunctions();

  // Expose internal registry state
  int total_registered = 0;
  bool substr_present = false;

  // Iterate over the hash table to count registered functions and look for "substr"
  for (int i = 0; i < SQLITE_FUNC_HASH_SZ; ++i) {
    FuncDef *p = sqlite3BuiltinFunctions.a[i];
    while (p != nullptr) {
      ++total_registered;
      if (p->zName && std::strcmp(p->zName, "substr") == 0) {
        substr_present = true;
      }
      p = p->u.pHash;
    }
  }

  // Assert expectations
  TEST_ASSERT(total_registered > 0, "Builtin function registry should contain entries after registration");
  TEST_ASSERT(substr_present, "Builtin function 'substr' should be present in the registry after registration");
  // Comment: This test relies on the default build including a substantial builtin set.
}

// Test 2: Presence of a representative built-in function (e.g., length) to confirm
// that the function list contains expected names after registration.
void test_builtin_presence_of_length() {
  // Act
  sqlite3RegisterBuiltinFunctions();

  bool length_present = false;

  // Look for the function "length" in the registry
  for (int i = 0; i < SQLITE_FUNC_HASH_SZ; ++i) {
    FuncDef *p = sqlite3BuiltinFunctions.a[i];
    while (p != nullptr) {
      if (p->zName && std::strcmp(p->zName, "length") == 0) {
        length_present = true;
        break;
      }
      p = p->u.pHash;
    }
    if (length_present) break;
  }

  // Assert expectations
  TEST_ASSERT(length_present, "Builtin function 'length' should be registered after sqlite3RegisterBuiltinFunctions");
}

// Entry point for running all tests
int main() {
  // Run tests
  test_builtin_registration_populates_registry();
  test_builtin_presence_of_length();

  // Summary
  if (g_failed_tests == 0) {
    std::cout << "All tests passed.\n";
    return 0;
  } else {
    std::cerr << g_failed_tests << " test(s) FAILED.\n";
    return 1;
  }
}
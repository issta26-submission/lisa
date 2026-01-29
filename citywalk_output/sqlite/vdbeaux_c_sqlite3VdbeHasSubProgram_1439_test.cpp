/*
  Unit Test Suite for sqlite3VdbeHasSubProgram (vdbeaux.c)

  This test suite is designed to run under a C++11 compiler without using GTest.
  It relies on the real SQLite internal headers to access the Vdbe structure and
  the sqlite3VdbeHasSubProgram function exactly as compiled in the project.

  Approach:
  - Test both branches of the predicate:
    1) pVdbe->pProgram is non-null -> function returns non-zero (true)
    2) pVdbe->pProgram is null     -> function returns 0 (false)
  - Use direct, non-terminating expectations to maximize code coverage (do not abort on failure).
  - Use extern "C" linkage for C functions to avoid name mangling when calling into the C codebase.

  Notes:
  - The test uses the actual Vdbe structure and SubProgram type as provided by
    sqliteInt.h and vdbeInt.h (included below). It assumes pProgram exists on Vdbe.
  - We only manipulate the pProgram pointer; no other private behavior is exercised.
  - Static members: If there are static/internal components in the real code,
    this test exercises only the public function and its observable behavior.
*/

#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdint>


extern "C" {
  // Import necessary internal dependencies from the project.
  // These headers define Vdbe, SubProgram, sqlite3VdbeHasSubProgram, etc.
  #include "sqliteInt.h"
  #include "vdbeInt.h"
}

// Simple, non-fatal test harness
static int g_tests_run = 0;
static int g_tests_passed = 0;

#define EXPECT_TRUE(cond) do { \
  if(!(cond)) { \
    std::cerr << "EXPECT_TRUE failed: " #cond " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
  } else { \
    ++g_tests_passed; \
  } \
  ++g_tests_run; \
} while(0)

#define EXPECT_FALSE(cond) do { \
  if((cond)) { \
    std::cerr << "EXPECT_FALSE failed: " #cond " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
  } else { \
    ++g_tests_passed; \
  } \
  ++g_tests_run; \
} while(0)

#define RUN_TEST(test_fn) do { \
  bool ok = (test_fn)(); \
  if(ok) { \
    ++g_tests_passed; \
  } else { \
    std::cerr << "Test " #test_fn " failed\n"; \
  } \
  ++g_tests_run; \
} while(0)

/*
  Test 1: When pVdbe.pProgram is non-null, sqlite3VdbeHasSubProgram should return a non-zero value.
  - We create a Vdbe instance with pProgram pointing to a non-null dummy SubProgram pointer.
  - We expect a non-zero return value.
*/
static bool test_hasSubProgram_true() {
  // Construct a Vdbe with all-zero fields
  Vdbe v;
  std::memset(&v, 0, sizeof(Vdbe));

  // Create a non-null dummy pointer for pProgram
  // The exact type is defined in the headers; we cast an integer to the expected pointer type.
  // This is safe as sqlite3VdbeHasSubProgram only checks non-null and does not dereference.
  v.pProgram = reinterpret_cast<SubProgram*>(reinterpret_cast<uintptr_t>(1));

  int res = sqlite3VdbeHasSubProgram(&v);
  return res != 0;
}

/*
  Test 2: When pVdbe.pProgram is null, sqlite3VdbeHasSubProgram should return 0.
  - Vdbe instance with pProgram explicitly set to null.
  - We expect a zero return value.
*/
static bool test_hasSubProgram_false() {
  Vdbe v;
  std::memset(&v, 0, sizeof(Vdbe));

  v.pProgram = nullptr;

  int res = sqlite3VdbeHasSubProgram(&v);
  return res == 0;
}

int main() {
  // Run tests with non-terminating assertions
  // Two dedicated tests for the true/false branches
  RUN_TEST(test_hasSubProgram_true);
  RUN_TEST(test_hasSubProgram_false);

  // Summary
  std::cout << "Tests run: " << g_tests_run << ", Passed: " << g_tests_passed << "\n";

  // Return non-zero if any test failed
  return (g_tests_run == g_tests_passed) ? 0 : 1;
}
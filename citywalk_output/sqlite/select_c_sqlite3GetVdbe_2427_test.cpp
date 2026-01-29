// C++11 Unit Tests for sqlite3GetVdbe
// This test suite targets the focal method sqlite3GetVdbe defined in select.c.
// It relies on the SQLite internal types Parse and Vdbe as defined by the project.
// The tests are purposefully written using a lightweight, non-GTest framework style
// (manual test harness) with non-terminating EXPECT_*-style assertions.

#include <cstdio>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>


// Forward declare as C to match the project's linkage
extern "C" {
  // Public SQLite APIs (for database creation/destruction)
  // These headers are expected to be available in the project environment.
  #include "sqlite3.h"
  // Internal SQLite structures used by sqlite3GetVdbe:
  #include "parse.h"  // defines struct Parse
  #include "vdbe.h"   // defines struct Vdbe
  // Prototype for the focal function (as implemented in the project)
  Vdbe *sqlite3GetVdbe(Parse *pParse);
}

// Lightweight test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Non-terminating assertion macros (do not exit on failure; just record)
#define EXPECT_TRUE(cond, msg) do { \
  ++g_total_tests; \
  if(!(cond)) { \
    std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ << " - " << (msg) << std::endl; \
    ++g_failed_tests; \
  } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
  ++g_total_tests; \
  if((a) != (b)) { \
    std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ << " - " << (msg) \
              << " (expected: " << (void*)(b) << ", got: " << (void*)(a) << ")" << std::endl; \
    ++g_failed_tests; \
  } \
} while(0)

#define EXPECT_NOT_NULL(p, msg) do { \
  ++g_total_tests; \
  if((p) == nullptr) { \
    std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ << " - " \
              << (msg) << " - Pointer is NULL" << std::endl; \
    ++g_failed_tests; \
  } \
} while(0)


// Test 1: When pParse->pVdbe is non-null, sqlite3GetVdbe should return it directly.
void test_sqlite3GetVdbe_returns_existing_vdbe()
{
  // Open an in-memory database to provide a valid sqlite3* for the Parse
  sqlite3 *db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  EXPECT_TRUE(db != nullptr, "sqlite3_open should succeed");
  if(!db) return;

  // Prepare a Parse object with a non-null pVdbe
  Parse pParse;
  std::memset(&pParse, 0, sizeof(pParse));
  Vdbe dummyVdbe;
  // The concrete address is not important; it just needs to be non-null.
  pParse.pVdbe = &dummyVdbe;
  pParse.pToplevel = nullptr; // not a top-level parse
  pParse.db = db;
  pParse.okConstFactor = 0;

  Vdbe *ret = sqlite3GetVdbe(&pParse);

  EXPECT_NOT_NULL(ret, "sqlite3GetVdbe should return a non-null Vdbe* when pVdbe is set");
  EXPECT_EQ(ret, pParse.pVdbe, "sqlite3GetVdbe should return the existing pVdbe pointer");

  // Ensure no unintended modifications to okConstFactor when pVdbe is present
  EXPECT_EQ(pParse.okConstFactor, 0, "okConstFactor should remain unchanged when pVdbe is non-null");

  sqlite3_close(db);
}

// Test 2: When pParse->pVdbe is NULL, pParse->pToplevel==0 and optimization is enabled,
// sqlite3GetVdbe should set okConstFactor to 1 and create a new Vdbe via sqlite3VdbeCreate.
void test_sqlite3GetVdbe_sets_okConstFactor_and_creates_vdbe()
{
  sqlite3 *db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  EXPECT_TRUE(db != nullptr, "sqlite3_open should succeed for test 2");
  if(!db) return;

  Parse pParse;
  std::memset(&pParse, 0, sizeof(pParse));

  // Prepare state to trigger the branch:
  pParse.pVdbe = nullptr;
  pParse.pToplevel = nullptr; // top-level parse
  pParse.db = db;
  pParse.okConstFactor = 0;

  Vdbe *ret = sqlite3GetVdbe(&pParse);

  EXPECT_NOT_NULL(ret, "sqlite3GetVdbe should create a new Vdbe when pVdbe is NULL and conditions allow");
  // The hard requirement of this branch is that okConstFactor becomes 1
  EXPECT_TRUE(pParse.okConstFactor == 1, "okConstFactor should be set to 1 in this path");

  sqlite3_close(db);
}

// Test 3: When pParse->pVdbe is NULL and pParse->pToplevel != 0, the condition is false
// and the function should create a Vdbe without altering okConstFactor.
void test_sqlite3GetVdbe_no_const_factor_when_pToplevel_nonzero()
{
  sqlite3 *db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  EXPECT_TRUE(db != nullptr, "sqlite3_open should succeed for test 3");
  if(!db) return;

  Parse pParse;
  std::memset(&pParse, 0, sizeof(pParse));

  // State to ensure the first if condition is bypassed
  pParse.pVdbe = nullptr;
  pParse.pToplevel = reinterpret_cast<Parse*>(1); // non-null to simulate sub-parse
  pParse.db = db;
  pParse.okConstFactor = 0;

  Vdbe *ret = sqlite3GetVdbe(&pParse);

  EXPECT_NOT_NULL(ret, "sqlite3GetVdbe should create a Vdbe when pToplevel is non-null");
  // Ensure okConstFactor was not set to 1 since the condition should be skipped
  EXPECT_TRUE(pParse.okConstFactor == 0, "okConstFactor should remain 0 when pToplevel is non-null");

  sqlite3_close(db);
}


// Entry point for the test suite
int main()
{
  std::cout << "Starting sqlite3GetVdbe unit tests (non-GTest, C++11)..." << std::endl;

  test_sqlite3GetVdbe_returns_existing_vdbe();
  test_sqlite3GetVdbe_sets_okConstFactor_and_creates_vdbe();
  test_sqlite3GetVdbe_no_const_factor_when_pToplevel_nonzero();

  std::cout << "Tests completed. Total: " << g_total_tests
            << ", Failures: " << g_failed_tests << std::endl;

  return (g_failed_tests == 0) ? 0 : 1;
}
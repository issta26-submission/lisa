// Test suite for the focal method testCksumArrayNew and its related dependencies.
// This harness is designed to compile with C++11, without Google Test, and call
// the production function as declared in the project (extern "C").
// The environment is expected to provide the actual implementation of testCksumArrayNew
// along with its dependencies (tdb_open, testWriteDatasourceRange, testCksumDatabase, etc.).
//
// Notes:
// - We use the project's public interface where possible. Access to internal fields is via
//   the C struct definitions provided by the project headers (see lsmtest.h in the project).
// - We intentionally only check high-level invariants that the API guarantees (non-null pointers,
//   correct allocation of the azCksum array, etc.). We avoid asserting on internal memory layout beyond
//   what the public API exposes.
// - Since testCksumArrayNew contains an assertion that would abort on invalid input, we select
//   test cases that satisfy the preconditions (nLast >= nFirst and (nLast - nFirst) % nStep == 0).
// - The tests aim to maximize code coverage by exercising multiple valid inputs and exercising
//   allocation/cleanup paths.
//
// Domain knowledge: The public API provides CksumDb with fields nFirst, nLast, nStep and azCksum.
// The test harness relies on the header definitions available in the project (lsmtest.h) to access
// these fields. If your environment uses a different header, adjust include accordingly.

#include <lsmtest.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <cassert>


// The production code defines these types and the function:
// extern "C" CksumDb *testCksumArrayNew(Datasource *pData, int nFirst, int nLast, int nStep);
extern "C" {
  // Forward declarations matching the project interface.
  struct Datasource;
  struct CksumDb;
  typedef struct CksumDb CksumDb;
  // We rely on the actual implementation to define the exact layout of CksumDb.
  // If your environment provides a header, you should include it here instead of re-declaring.
  CksumDb *testCksumArrayNew(Datasource *pData, int nFirst, int nLast, int nStep);
}

// If the environment does not provide the above declarations (e.g., missing header), provide lightweight
// fallbacks that mirror the expected public API but avoid duplicating the real logic.
// This block is guarded to avoid conflicts when the real headers are present.
#ifndef __HAS_LSMTEST_FALLBACK__
#define __HAS_LSMTEST_FALLBACK__

// Lightweight fallback declarations to enable compilation in environments that do not
// ship with lsmtest.h. These definitions assume the same public API shape as the production code.
struct Datasource {
  // Intentionally empty: the production function uses pData only for its APIs;
  // the test harness does not rely on any concrete content here.
};

struct CksumDb {
  int nFirst;
  int nLast;
  int nStep;
  char **azCksum;
};

// Redefine the function prototype for the fallback scenario
extern "C" {
  CksumDb *testCksumArrayNew(Datasource *pData, int nFirst, int nLast, int nStep);
}

#endif // __HAS_LSMTEST_FALLBACK__

// Helper: free memory allocated by testCksumArrayNew (as produced by the production code)
static void freeCksumDb(CksumDb *p)
{
  if (!p) return;
  if (p->azCksum) {
    // The azCksum memory is allocated as a contiguous block that also contains
    // the pointers. Free the entire block at once.
    free(p->azCksum);
  }
  free(p);
}

// Simple test runner utility
static bool verifyPointerNotNull(void* p, const char* msg)
{
  if (p == nullptr) {
    std::cerr << "Test Failure: " << msg << " - pointer is NULL." << std::endl;
    return false;
  }
  return true;
}

// Test Case 1: Basic allocation and initialization for range 0..10 with step 2
static bool testCase_basicAllocation()
{
  Datasource ds = {}; // minimal datasource object
  int nFirst = 0;
  int nLast  = 10;
  int nStep  = 2;

  CksumDb *p = testCksumArrayNew(&ds, nFirst, nLast, nStep);
  if (!verifyPointerNotNull(p, "testCase_basicAllocation: testCksumArrayNew returned NULL")) {
    return false;
  }

  // Validate public fields (assumes access to struct fields as provided by project headers)
  bool ok = true;
  if (p->nFirst != nFirst) {
    std::cerr << "testCase_basicAllocation: nFirst mismatch. expected " << nFirst << ", got " << p->nFirst << std::endl;
    ok = false;
  }
  if (p->nLast != nLast) {
    std::cerr << "testCase_basicAllocation: nLast mismatch. expected " << nLast << ", got " << p->nLast << std::endl;
    ok = false;
  }
  if (p->nStep != nStep) {
    std::cerr << "testCase_basicAllocation: nStep mismatch. expected " << nStep << ", got " << p->nStep << std::endl;
    ok = false;
  }

  // nEntry = 1 + ((nLast - nFirst) / nStep)
  int nEntry = 1 + ((nLast - nFirst) / nStep);
  if (!p->azCksum && nEntry > 0) {
    std::cerr << "testCase_basicAllocation: azCksum pointer is NULL while nEntry=" << nEntry << std::endl;
    ok = false;
  } else {
    // Validate that each azCksum[i] pointer is non-NULL
    for (int i = 0; i < nEntry; ++i) {
      if (p->azCksum[i] == nullptr) {
        std::cerr << "testCase_basicAllocation: azCksum[" << i << "] is NULL" << std::endl;
        ok = false;
        break;
      }
    }
  }

  freeCksumDb(p);
  return ok;
}

// Test Case 2: Edge case where nFirst == nLast (single entry)
static bool testCase_singleEntry()
{
  Datasource ds = {};
  int nFirst = 5;
  int nLast  = 5;
  int nStep  = 1;

  CksumDb *p = testCksumArrayNew(&ds, nFirst, nLast, nStep);
  if (!verifyPointerNotNull(p, "testCase_singleEntry: testCksumArrayNew returned NULL")) {
    return false;
  }

  int nEntry = 1 + ((nLast - nFirst) / nStep);
  bool ok = true;
  if (p->nFirst != nFirst || p->nLast != nLast || p->nStep != nStep) {
    std::cerr << "testCase_singleEntry: field mismatch (nFirst/nLast/nStep)" << std::endl;
    ok = false;
  }
  if (!p->azCksum && nEntry > 0) {
    std::cerr << "testCase_singleEntry: azCksum pointer is NULL for single entry" << std::endl;
    ok = false;
  } else if (p->azCksum) {
    for (int i = 0; i < nEntry; ++i) {
      if (p->azCksum[i] == nullptr) {
        std::cerr << "testCase_singleEntry: azCksum[" << i << "] is NULL" << std::endl;
        ok = false;
        break;
      }
    }
  }

  freeCksumDb(p);
  return ok;
}

// Test Case 3: Another representative range (0..20 with step 5)
static bool testCase_anotherRange()
{
  Datasource ds = {};
  int nFirst = 0;
  int nLast  = 20;
  int nStep  = 5;

  CksumDb *p = testCksumArrayNew(&ds, nFirst, nLast, nStep);
  if (!verifyPointerNotNull(p, "testCase_anotherRange: testCksumArrayNew returned NULL")) {
    return false;
  }

  int nEntry = 1 + ((nLast - nFirst) / nStep);
  bool ok = true;
  if (p->nFirst != nFirst || p->nLast != nLast || p->nStep != nStep) {
    std::cerr << "testCase_anotherRange: field mismatch (nFirst/nLast/nStep)" << std::endl;
    ok = false;
  }
  if (!p->azCksum && nEntry > 0) {
    std::cerr << "testCase_anotherRange: azCksum pointer is NULL for range" << std::endl;
    ok = false;
  } else if (p->azCksum) {
    for (int i = 0; i < nEntry; ++i) {
      if (p->azCksum[i] == nullptr) {
        std::cerr << "testCase_anotherRange: azCksum[" << i << "] is NULL" << std::endl;
        ok = false;
        break;
      }
    }
  }

  freeCksumDb(p);
  return ok;
}

int main() {
  int overall_pass = 1;

  std::cout << "LSM CksumArrayNew test-suite (C++11 harness) starting..." << std::endl;

  // Run Test Case 1
  if (testCase_basicAllocation()) {
    std::cout << "[PASS] Test Case 1: Basic allocation and initialization (0..10 step 2)" << std::endl;
  } else {
    std::cout << "[FAIL] Test Case 1: Basic allocation and initialization" << std::endl;
    overall_pass = 0;
  }

  // Run Test Case 2
  if (testCase_singleEntry()) {
    std::cout << "[PASS] Test Case 2: Single-entry scenario (nFirst==nLast)" << std::endl;
  } else {
    std::cout << "[FAIL] Test Case 2: Single-entry scenario" << std::endl;
    overall_pass = 0;
  }

  // Run Test Case 3
  if (testCase_anotherRange()) {
    std::cout << "[PASS] Test Case 3: Another valid range (0..20 step 5)" << std::endl;
  } else {
    std::cout << "[FAIL] Test Case 3: Another valid range" << std::endl;
    overall_pass = 0;
  }

  if (overall_pass) {
    std::cout << "ALL TEST CASES PASSED." << std::endl;
  } else {
    std::cout << "SOME TEST CASES FAILED." << std::endl;
  }

  return overall_pass ? 0 : 1;
}
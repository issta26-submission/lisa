// Test harness for sqlite3PagerMemUsed with a lightweight, self-contained setup.
// This test is designed to run with C++11, without using GoogleTest.
// It relies on including the focal C file (pager.c) to access the real
// Pager type and the sqlite3PagerMemUsed implementation.
// The test also provides small stub implementations for external dependencies
// (sqlite3PcachePagecount and sqlite3MallocSize) so we can exercise the logic
// without requiring a full SQLite build.

// Note: This harness deliberately avoids terminating on first failure to maximize
// coverage across multiple test cases.

#include <wal.h>
#include <sqliteInt.h>
#include <pager.c>
#include <iostream>
#include <cstdlib>


// Bring in the focal method and its dependent types by including the C source.
// Wrapping the include in extern "C" ensures C linkage for the symbols defined in C.
// This also exposes the Pager type (needed to instantiate objects) and the
// PgHdr type (used in sizeof(PgHdr)).
extern "C" {
}

// Mock dependencies used by sqlite3PagerMemUsed (which in the real project
// would be provided by SQLite's PCache system and memory allocator).
extern "C" int sqlite3PcachePagecount(void *p){
  // Simple, deterministic behavior for tests:
  // - If a non-null PCache is provided, pretend there are 3 pages.
  // - If NULL is provided, report 0 pages.
  return (p != nullptr) ? 3 : 0;
}

// Mocked allocator size reporter used by sqlite3PagerMemUsed.
extern "C" int sqlite3MallocSize(void *p){
  // Return a fixed, deterministic size for testability.
  // The exact value is chosen to be stable across test scenarios.
  return 128;
}

// Helper: compute the expected value according to the focal method's formula.
// This mirrors the calculation performed inside sqlite3PagerMemUsed.
static int calc_expected(const Pager *p){
  // perPageSize = pageSize + nExtra + (int)(sizeof(PgHdr) + 5*sizeof(void*))
  int perPageSize = p->pageSize + p->nExtra + (int)(sizeof(PgHdr) + 5*sizeof(void*));
  // total = perPageSize * sqlite3PcachePagecount(pPCache) + sqlite3MallocSize(pPager) + pageSize
  int total = perPageSize * sqlite3PcachePagecount(p->pPCache) + sqlite3MallocSize((void*)p) + p->pageSize;
  return total;
}

// Simple test harness with multiple scenarios.
// Each test logs its result and a short description.
int main(){
  int failures = 0;

  // Test Case 1: Basic scenario with non-null PCache and typical sizes.
  // This validates the core calculation path when all inputs are present.
  {
    // Allocate a real Pager object (size known from the actual type in pager.c)
    Pager *pPager = (Pager *)malloc(sizeof(Pager));
    if(!pPager){
      std::cerr << "[ERR] Allocation for Pager failed in Test Case 1\n";
      ++failures;
    } else {
      pPager->pageSize = 4096;   // typical page size
      pPager->nExtra = 8;        // small extra
      pPager->pPCache = (void *)0x1234; // non-null PCache

      int used = sqlite3PagerMemUsed(pPager);
      int expected = calc_expected(pPager);

      if(used != expected){
        std::cerr << "[FAIL] Test Case 1: expected " << expected
                  << " but got " << used << "\n";
        ++failures;
      }else{
        std::cout << "[OK] Test Case 1: Basic non-null PCache calculation matches expectation.\n";
      }

      free(pPager);
    }
  }

  // Test Case 2: PCache is NULL.
  // This ensures the function handles a NULL PCache pointer gracefully
  // (i.e., sqlite3PcachePagecount(NULL) should yield 0 pages according to mock).
  {
    Pager *pPager = (Pager *)malloc(sizeof(Pager));
    if(!pPager){
      std::cerr << "[ERR] Allocation for Pager failed in Test Case 2\n";
      ++failures;
    } else {
      pPager->pageSize = 1024;
      pPager->nExtra = 4;
      pPager->pPCache = NULL; // NULL PCache

      int used = sqlite3PagerMemUsed(pPager);
      int expected = calc_expected(pPager);

      if(used != expected){
        std::cerr << "[FAIL] Test Case 2: expected " << expected
                  << " but got " << used << "\n";
        ++failures;
      }else{
        std::cout << "[OK] Test Case 2: NULL PCache handling matches expectation.\n";
      }

      free(pPager);
    }
  }

  // Test Case 3: Another variation to exercise different perPageSize and page count.
  // Ensures arithmetic with larger values remains correct.
  {
    Pager *pPager = (Pager *)malloc(sizeof(Pager));
    if(!pPager){
      std::cerr << "[ERR] Allocation for Pager failed in Test Case 3\n";
      ++failures;
    } else {
      pPager->pageSize = 8192;
      pPager->nExtra = 12;
      pPager->pPCache = (void *)0xABCD; // non-null PCache

      int used = sqlite3PagerMemUsed(pPager);
      int expected = calc_expected(pPager);

      if(used != expected){
        std::cerr << "[FAIL] Test Case 3: expected " << expected
                  << " but got " << used << "\n";
        ++failures;
      }else{
        std::cout << "[OK] Test Case 3: Large values calculation matches expectation.\n";
      }

      free(pPager);
    }
  }

  std::cout << "Total failures: " << failures << std::endl;
  return failures;
}
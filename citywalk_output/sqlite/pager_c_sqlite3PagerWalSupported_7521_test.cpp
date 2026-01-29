// Self-contained C++11 unit test harness for the focal method
// sqlite3PagerWalSupported replicated here for testing purposes.
// The tests cover true/false branches of the predicate logic.
//
// Notes:
// - This test is designed to be compiled and run without external test frameworks.
// - It creates minimal stand-ins for the dependent structures (Pager, PagerFD, sqlite3_io_methods).
// - We use simple non-terminating assertions to maximize code coverage.

#include <iostream>
#include <wal.h>
#include <sqliteInt.h>


// Forward declarations of minimal types to mirror the production layout
struct sqlite3_io_methods;
struct PagerFD;
struct Pager;

// Lightweight stub for the xShmMap callback (non-nullable in tests when needed)
static void* shmMapStub(void* p, long offset, int flags, void** pp) { (void)p; (void)offset; (void)flags; (void)pp; return nullptr; }

// Production-like structure layout (minimal for tests)
struct sqlite3_io_methods {
  int iVersion;
  void* (*xShmMap)(void*, long, int, void**);
};

struct PagerFD {
  sqlite3_io_methods *pMethods;
};

struct Pager {
  int noLock;
  int exclusiveMode;
  PagerFD *fd;
};

// Focal method under test (recreated here for isolated unit testing)
int sqlite3PagerWalSupported(Pager *pPager) {
  const sqlite3_io_methods *pMethods = pPager->fd->pMethods;
  if( pPager->noLock ) return 0;
  return pPager->exclusiveMode || (pMethods->iVersion>=2 && pMethods->xShmMap);
}

// Simple global failure counter for non-terminating tests
static int g_failures = 0;

// Helper macro for test assertions without aborting on failure
#define ASSERT_TRUE(cond, msg) do { \
  if(!(cond)) { \
    std::cerr << "FAILED: " << msg << " (LINE " << __LINE__ << ")\n"; \
    ++g_failures; \
  } \
} while(0)

// Test 1: When noLock is true, should return 0 regardless of other fields
void test_noLock_branch_returns_zero() {
  sqlite3_io_methods methods;
  methods.iVersion = 3;
  methods.xShmMap = shmMapStub;

  PagerFD fd;
  fd.pMethods = &methods;

  Pager pager;
  pager.noLock = 1;          // true -> should short-circuit to 0
  pager.exclusiveMode = 0;
  pager.fd = &fd;

  int res = sqlite3PagerWalSupported(&pager);
  ASSERT_TRUE(res == 0, "test_noLock_branch_returns_zero: expected 0 when noLock is true");
}

// Test 2: When exclusiveMode is true and noLock is false, should return true
void test_exclusiveMode_true_returns_true() {
  sqlite3_io_methods methods;
  methods.iVersion = 1;        // Version doesn't matter as exclusiveMode is true
  methods.xShmMap = shmMapStub;

  PagerFD fd;
  fd.pMethods = &methods;

  Pager pager;
  pager.noLock = 0;
  pager.exclusiveMode = 1;     // true -> should return non-zero
  pager.fd = &fd;

  int res = sqlite3PagerWalSupported(&pager);
  ASSERT_TRUE(res != 0, "test_exclusiveMode_true_returns_true: expected non-zero when exclusiveMode is true");
}

// Test 3: When noLock is false, exclusiveMode is false, iVersion>=2 and xShmMap non-null -> true
void test_version_and_shmMap_present_returns_true() {
  sqlite3_io_methods methods;
  methods.iVersion = 2;
  methods.xShmMap = shmMapStub;   // non-null -> considered available

  PagerFD fd;
  fd.pMethods = &methods;

  Pager pager;
  pager.noLock = 0;
  pager.exclusiveMode = 0;
  pager.fd = &fd;

  int res = sqlite3PagerWalSupported(&pager);
  ASSERT_TRUE(res != 0, "test_version_and_shmMap_present_returns_true: expected non-zero when iVersion>=2 and xShmMap present");
}

// Test 4: When noLock is false, exclusiveMode is false, iVersion<2 -> false
void test_version_too_low_returns_false() {
  sqlite3_io_methods methods;
  methods.iVersion = 1;               // less than 2
  methods.xShmMap = shmMapStub;

  PagerFD fd;
  fd.pMethods = &methods;

  Pager pager;
  pager.noLock = 0;
  pager.exclusiveMode = 0;
  pager.fd = &fd;

  int res = sqlite3PagerWalSupported(&pager);
  ASSERT_TRUE(res == 0, "test_version_too_low_returns_false: expected 0 when iVersion<2");
}

// Test 5: When iVersion>=2 but xShmMap is NULL -> false
void test_shmMap_null_returns_false() {
  sqlite3_io_methods methods;
  methods.iVersion = 2;
  methods.xShmMap = nullptr;            // effectively disabled

  PagerFD fd;
  fd.pMethods = &methods;

  Pager pager;
  pager.noLock = 0;
  pager.exclusiveMode = 0;
  pager.fd = &fd;

  int res = sqlite3PagerWalSupported(&pager);
  ASSERT_TRUE(res == 0, "test_shmMap_null_returns_false: expected 0 when xShmMap is NULL");
}

// Entry point to run all tests
int main() {
  std::cout << "Running sqlite3PagerWalSupported unit tests (self-contained, no GTest)...\n";

  test_noLock_branch_returns_zero();
  test_exclusiveMode_true_returns_true();
  test_version_and_shmMap_present_returns_true();
  test_version_too_low_returns_false();
  test_shmMap_null_returns_false();

  if(g_failures == 0) {
    std::cout << "ALL TESTS PASSED\n";
  } else {
    std::cout << g_failures << " TEST(S) FAILED\n";
  }
  return g_failures;
}
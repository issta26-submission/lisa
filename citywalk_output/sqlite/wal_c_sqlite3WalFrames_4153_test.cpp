// Lightweight C++11 unit tests for sqlite3WalFrames (no GoogleTest)
#include <Windows.h>
#include <iostream>
#include <wal.h>
#include <cstring>


// Forward declarations to avoid depending on sqlite3 internal headers.
// We assume the real wal.c provides non-static, externally linkable versions
// of these symbols in the test environment. If the real project uses static
// linkage for walFrames, these tests may not link; this file demonstrates
// a best-effort testing approach given the constraints.
extern "C" {
  // Forward declare the types used by sqlite3WalFrames.
  struct Wal;
  struct PgHdr;
  typedef unsigned int Pgno;

  // Declaration of the function under test (to be linked from wal.c)
  int sqlite3WalFrames(Wal *pWal, int szPage, PgHdr *pList, Pgno nTruncate, int isCommit, int sync_flags);

  // Optional helpers (we provide test-time overrides in the test suite).
  int walFrames(Wal *pWal, int szPage, PgHdr *pList, Pgno nTruncate, int isCommit, int sync_flags);
  int walHandleException(Wal *pWal);
}

// Test scaffolding: we provide test-time overrides for walFrames and walHandleException.
// These mocks are only effective if the real wal.c exposes walFrames and walHandleException
// with external linkage (i.e., not static). The mock is designed to capture the arguments
// passed to sqlite3WalFrames and to control the return values of walFrames.
namespace TestMocks {
  // Minimal opaque dummy types to satisfy the compiler; the real wal.c interprets these.
  struct Wal { int dummy; };
  struct PgHdr { int dummy; };
  using WalPtr = Wal*;
  using PgHdrPtr = PgHdr*;

  // Global state to capture parameters passed through sqlite3WalFrames
  static WalPtr g_last_pWal = nullptr;
  static int g_last_szPage = 0;
  static PgHdrPtr g_last_pList = nullptr;
  static Pgno g_last_nTruncate = 0;
  static int g_last_isCommit = 0;
  static int g_last_sync_flags = 0;

  // Control the return value of walFrames
  static int g_walFramesReturn = 0;
  // Control whether walHandleException was invoked
  static int g_walHandleExceptionCalled = 0;
  // Return value for walHandleException (not used in tests deeply, but kept for completeness)
  static int g_walHandleExceptionReturn = -1;

  // Provide a reset helper to isolate tests
  static void resetMocks() {
    g_last_pWal = nullptr;
    g_last_szPage = 0;
    g_last_pList = nullptr;
    g_last_nTruncate = 0;
    g_last_isCommit = 0;
    g_last_sync_flags = 0;
    g_walFramesReturn = 0;
    g_walHandleExceptionCalled = 0;
    g_walHandleExceptionReturn = -1;
  }

  // Mock implementation of walFrames (extern "C" to match expected linkage)
  extern "C" int walFrames(Wal *pWal, int szPage, PgHdr *pList, Pgno nTruncate, int isCommit, int sync_flags) {
    g_last_pWal = pWal;
    g_last_szPage = szPage;
    g_last_pList = pList;
    g_last_nTruncate = nTruncate;
    g_last_isCommit = isCommit;
    g_last_sync_flags = sync_flags;
    return g_walFramesReturn;
  }

  // Mock implementation of walHandleException (extern "C" to match expected linkage)
  extern "C" int walHandleException(Wal *pWal) {
    (void)pWal; // unused in tests
    g_walHandleExceptionCalled++;
    return g_walHandleExceptionReturn;
  }
}

// Simple test harness (no external test framework)
namespace TestHarness {
  using TestFn = bool(*)();

  // Simple assertion helper
  static bool fail(const char* msg) {
    std::cerr << "TEST FAILURE: " << msg << std::endl;
    return false;
  }

  // A tiny assertion macro with message
  #define ASSERT_EQ(expected, actual, msg) \
    do { if ((expected) != (actual)) { std::cerr << "Assertion failed: " << (msg) << " (expected=" << (expected) << ", actual=" << (actual) << ")" << std::endl; return false; } } while(0)

  // Test 1: Ensure sqlite3WalFrames forwards all parameters correctly to walFrames
  // and returns the exact value provided by walFrames when it succeeds.
  bool test_sqlite3WalFrames_ForwardsParameters_ReturnsWalFramesValue() {
    using namespace TestMocks;
    resetMocks();

    // Arrange: set a known return value from walFrames
    g_walFramesReturn = 0;
    g_walHandleExceptionReturn = -999; // should not be invoked in this test

    // Prepare a dummy Wal object
    Wal fakeWal;
    PgHdr dummyList;
    // Act
    int rc = sqlite3WalFrames(&fakeWal, 4096, &dummyList, 12345, 1, 0);

    // Assert: rc equals walFrames return value, and parameters were forwarded
    if (rc != 0) {
      return fail("sqlite3WalFrames did not propagate walFrames return value (0).");
    }
    ASSERT_EQ(&fakeWal, g_last_pWal, "pWal should be forwarded to walFrames");
    ASSERT_EQ(4096, g_last_szPage, "szPage should be forwarded to walFrames");
    ASSERT_EQ(&dummyList, g_last_pList, "pList should be forwarded to walFrames");
    ASSERT_EQ(12345u, g_last_nTruncate, "nTruncate should be forwarded to walFrames");
    ASSERT_EQ(1, g_last_isCommit, "isCommit should be forwarded to walFrames");
    ASSERT_EQ(0, g_last_sync_flags, "sync_flags should be forwarded to walFrames");

    // Also ensure walHandleException was not called in this scenario
    if (g_walHandleExceptionCalled != 0) {
      return fail("walHandleException should not be called when walFrames succeeds.");
    }

    return true;
  }

  // Test 2: Ensure sqlite3WalFrames forwards a non-zero return value correctly
  // This tests the non-error path where walFrames reports a failure code.
  bool test_sqlite3WalFrames_ReturnsNonZeroFromWalFrames() {
    using namespace TestMocks;
    resetMocks();

    // Arrange: set walFrames to return a non-zero code
    g_walFramesReturn = 5;
    g_walHandleExceptionReturn = -1;

    Wal fakeWal;
    PgHdr dummyList;
    int rc = sqlite3WalFrames(&fakeWal, 512, &dummyList, 99, 0, 2);

    // Assert: rc should equal 5, and parameters forwarded
    if (rc != 5) {
      return fail("sqlite3WalFrames did not forward a non-zero walFrames return value (5).");
    }
    ASSERT_EQ(&fakeWal, g_last_pWal, "pWal should be forwarded to walFrames");
    ASSERT_EQ(512, g_last_szPage, "szPage should be forwarded to walFrames");
    ASSERT_EQ(&dummyList, g_last_pList, "pList should be forwarded to walFrames");
    ASSERT_EQ(99u, g_last_nTruncate, "nTruncate should be forwarded to walFrames");
    ASSERT_EQ(0, g_last_isCommit, "isCommit should be forwarded to walFrames");
    ASSERT_EQ(2, g_last_sync_flags, "syncFlags should be forwarded to walFrames");

    // walHandleException should not be invoked in this path
    if (g_walHandleExceptionCalled != 0) {
      return fail("walHandleException should not be called when walFrames returns non-zero.");
    }

    return true;
  }

  // Optional: Test that both branches (true/false) of isCommit are exercised
  // by re-running the same test with isCommit toggled.
  bool test_sqlite3WalFrames_IsCommit_BranchesCovered() {
    using namespace TestMocks;
    resetMocks();

    // Case A: isCommit = 1
    g_walFramesReturn = 0;
    Wal fakeWalA;
    PgHdr listA;
    int rcA = sqlite3WalFrames(&fakeWalA, 1024, &listA, 555, 1, 0);
    if (rcA != 0) return fail("isCommit=1 path failed to return walFrames value 0.");

    // Validate parameters
    if (g_last_isCommit != 1) return fail("isCommit value not forwarded as 1 (Case A).");

    // Reset for Case B
    resetMocks();

    // Case B: isCommit = 0
    g_walFramesReturn = 0;
    Wal fakeWalB;
    PgHdr listB;
    int rcB = sqlite3WalFrames(&fakeWalB, 1024, &listB, 555, 0, 0);
    if (rcB != 0) return fail("isCommit=0 path failed to return walFrames value 0.");
    if (g_last_isCommit != 0) return fail("isCommit value not forwarded as 0 (Case B).");

    return true;
  }

  // A tiny runner
  void runAllTests() {
    int passed = 0;
    int total = 0;

    auto run = [&](const char* name, bool (*fn)()) {
      total++;
      bool ok = false;
      // Ensure mocks are isolated per test
      TestMocks::resetMocks();
      ok = fn();
      if (ok) {
        std::cout << "[PASS] " << name << std::endl;
        passed++;
      } else {
        std::cout << "[FAIL] " << name << std::endl;
      }
    };

    // Run tests
    run("test_sqlite3WalFrames_ForwardsParameters_ReturnsWalFramesValue",
        test_sqlite3WalFrames_ForwardsParameters_ReturnsWalFramesValue);
    run("test_sqlite3WalFrames_ReturnsNonZeroFromWalFrames",
        test_sqlite3WalFrames_ReturnsNonZeroFromWalFrames);
    run("test_sqlite3WalFrames_IsCommit_BranchesCovered",
        test_sqlite3WalFrames_IsCommit_BranchesCovered);

    std::cout << "Tests passed: " << passed << " / " << total << std::endl;
  }

} // namespace TestHarness

int main() {
  // Run all tests
  TestHarness::runAllTests();
  return 0;
}
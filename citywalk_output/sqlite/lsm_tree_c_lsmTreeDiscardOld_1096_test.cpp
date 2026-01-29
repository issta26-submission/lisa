/*
  Test suite for the focal method: lsmTreeDiscardOld located in lsm_tree.c

  This file provides a lightweight C++11-based test harness (no GTest) that
  targets the behavior of lsmTreeDiscardOld. It relies on the project’s
  existing headers and linkage so that the real types (e.g., lsm_db, treehdr)
  and the function under test are used.

  How this test works (high level):
  - Mocks the function lsmShmAssertLock to control the outcome of the
    assertion condition inside lsmTreeDiscardOld, enabling control over
    true/false evaluation of the two-part lock check.
  - Exercises two scenarios:
      1) First lock check returns true, triggering short-circuit (second
         check not invoked).
      2) First lock check returns false, second returns true (both checks
         invoked).
  - Verifies that after lsmTreeDiscardOld runs:
      - pDb->treehdr.iUsedShmid is updated to the previous iOldShmid.
      - pDb->treehdr.iOldShmid is reset to 0.
  - Tracks the number of times lsmShmAssertLock is called to validate
    branching behavior.

  Note:
  - The test uses the project’s lsmInt.h header (for lsm_db and required
    structures) and links against lsm_tree.c to exercise the real logic.
  - We provide an extern "C" mock for lsmShmAssertLock. In your build
    environment, ensure your linker resolves this symbol to the mock
    rather than a library implementation, or adapt the test harness to
    your mocking strategy (e.g., weak symbols, dependency injection, or
    a dedicated test binary build).
  - The tests rely on the presence of:
      - lsmTreeDiscardOld(lsm_db *pDb)
      - lsmShmAssertLock(lsm_db *pDb, int lock, int excl)

  The tests intentionally avoid terminating on assertion failures (we only
  exercise the true branch cases). For environments where asserts terminate
  the process, ensure the mock scenarios always yield true for the OR-ed
  condition as implemented here.
*/

/* Domain knowledge alignment and test plan:
   - Import dependencies with correct paths (lsmInt.h etc. expected in project).
   - Cover true/false branches of the assertion predicate via mock returns.
   - Access static-like behavior via the public interface; lsmTreeDiscardOld
     uses pDb and the global/mocked lsmShmAssertLock.
   - Use standard C++11 facilities only; no GMock; no GTest.
   - Tests are directly callable from main; no test framework required.
   - Include explanatory comments for each test case.
*/

#include <string.h>
#include <vector>
#include <lsmInt.h>
#include <cstring>
#include <iostream>
#include <cstdint>


// Include project headers to obtain real types (lsm_db, treehdr, etc.)
// Adjust the include path according to your build system.
extern "C" {
  // Prototype of function under test (provided by lsm_tree.c)
  void lsmTreeDiscardOld(lsm_db *pDb);

  // Our mock for the external dependency used inside lsmTreeDiscardOld
  // The actual implementation may be provided by the project; we override
  // with a test-specific mock in this translation unit.
  int lsmShmAssertLock(lsm_db *pDb, int lock, int excl);
}

// Forward declare to ensure linkage consistency if needed
// (In real project, keep the exact prototype from the headers)
  
// ---------------- Mocking infrastructure ----------------

static std::vector<int> g_lockReturns; // predefined sequence of boolean returns
static int g_lockCallCount = 0;         // number of times lsmShmAssertLock was called

// Mock implementation of lsmShmAssertLock
extern "C" int lsmShmAssertLock(lsm_db *pDb, int lock, int excl) {
  ++g_lockCallCount;
  if (!g_lockReturns.empty()) {
    int r = g_lockReturns.front();
    g_lockReturns.erase(g_lockReturns.begin());
    return r;
  }
  // Default to false if not configured (should not happen in our test cases)
  return 0;
}

// ---------------- Test harness ----------------

// Simple test harness (non-terminating): records results and prints a summary.
static int g_testsPassed = 0;
static int g_testsTotal  = 0;

#define RUN_TEST(name) do { \
  if (name()) { \
    ++g_testsPassed; \
  } else { \
    std::cerr << "Test failed: " #name "\n"; \
  } \
  ++g_testsTotal; \
} while(0)

static void printSummary() {
  std::cout << "Tests run: " << g_testsTotal
            << ", Passed: " << g_testsPassed
            << ", Failed: " << (g_testsTotal - g_testsPassed) << "\n";
}

// Utility to reset global mock state before each test
static void resetMockState() {
  g_lockReturns.clear();
  g_lockCallCount = 0;
}

// Helper to initialize a minimal lsm_db instance for tests
static void initDbForTest(lsm_db &db, int oldValue) {
  // Initialize the fields that lsmTreeDiscardOld touches.
  // The actual lsm_db in the project may contain many more members; only
  // the ones touched by the focal method are set here to avoid side effects.
  std::memset(reinterpret_cast<void*>(&db), 0, sizeof(db));
  // The focal method uses pDb->treehdr.iUsedShmid and pDb->treehdr.iOldShmid
  db.treehdr.iUsedShmid = 0;
  db.treehdr.iOldShmid  = oldValue;
}

// Test 1: First lock check returns true; second should not be called (short-circuit)
static bool test_DiscardOld_FirstLockTrue_ShortCircuit() {
  resetMockState();
  // Configure: first call returns true; second call should not be invoked
  g_lockReturns.push_back(1); // first call true
  // Prepare test DB
  lsm_db db;
  initDbForTest(db, 0x1234); // iOldShmid = 0x1234
  // Act
  lsmTreeDiscardOld(&db);
  // Assert: iUsedShmid updated to previous iOldShmid; iOldShmid reset to 0
  bool ok = (db.treehdr.iUsedShmid == 0x1234) && (db.treehdr.iOldShmid == 0);
  // Assert: second call was not performed
  bool shortCircuit = (g_lockCallCount == 1);
  if(!ok) {
    std::cerr << "Test 1 failed: incorrect iUsedShmid/iOldShmid after discard.\n";
  }
  if(!shortCircuit) {
    std::cerr << "Test 1 failed: expected short-circuit (only 1 lock check) but got "
              << g_lockCallCount << " calls.\n";
  }
  return ok && shortCircuit;
}

// Test 2: First lock check returns false; second returns true (both called)
static bool test_DiscardOld_FirstLockFalse_SecondLockTrue() {
  resetMockState();
  // Configure: first call false; second call true
  g_lockReturns.push_back(0); // first call false
  g_lockReturns.push_back(1); // second call true
  // Prepare test DB
  lsm_db db;
  initDbForTest(db, 0xDEAD); // iOldShmid = 0xDEAD
  // Act
  lsmTreeDiscardOld(&db);
  // Assert: iUsedShmid updated; iOldShmid reset
  bool ok = (db.treehdr.iUsedShmid == 0xDEAD) && (db.treehdr.iOldShmid == 0);
  // Assert: both calls were performed
  bool bothCalled = (g_lockCallCount == 2);
  if(!ok) {
    std::cerr << "Test 2 failed: incorrect iUsedShmid/iOldShmid after discard.\n";
  }
  if(!bothCalled) {
    std::cerr << "Test 2 failed: expected two lock checks but got "
              << g_lockCallCount << " calls.\n";
  }
  return ok && bothCalled;
}

// Entry point for the test suite
int main() {
  // Informational header
  std::cout << "Starting lsmTreeDiscardOld unit tests (non-GTest, C++11)..." << std::endl;

  // Run tests
  RUN_TEST(test_DiscardOld_FirstLockTrue_ShortCircuit);
  RUN_TEST(test_DiscardOld_FirstLockFalse_SecondLockTrue);

  // Summary
  printSummary();

  // Return 0 if all tests passed, non-zero otherwise
  return (g_testsPassed == g_testsTotal) ? 0 : 1;
}

/* Notes for test authors:
   - The mocks here replace the dependency that lsmTreeDiscardOld uses to check
     the writer locks. They are designed to let us test the two branches of the
     assert predicate in lsmTreeDiscardOld (short-circuit vs. full evaluation).
   - If your build system uses a different symbol resolution strategy for mocks
     (e.g., weak symbols, test builds, or a dedicated mock header), adapt the
     approach accordingly.
   - If the actual lsmInt.h or the project’s lsm_db type differs from what is
     assumed in this test, adjust the test DB struct initialization code
     accordingly to construct a valid lsm_db instance compatible with lsmTreeDiscardOld.
*/
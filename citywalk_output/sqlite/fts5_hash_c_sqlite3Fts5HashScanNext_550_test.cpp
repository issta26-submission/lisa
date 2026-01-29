/*
  FOCAL METHOD TEST SUITE
  Target: sqlite3Fts5HashScanNext(Fts5Hash *p)
  Context: Provided dependencies from fts5Int.h (Fts5Hash, Fts5HashScan, etc.)
  Environment: C++11, no GTest; simple runtime test harness using the project headers
  Notes:
   - We focus on the observable effects of sqlite3Fts5HashScanNext: advancing the scan pointer
   - We validate true-branch behavior (not at EOF) by performing successive advances
   - We validate EOF status after advancing to the end
   - We avoid terminating asserts by using a lightweight runtime checker (non-terminating assertions)
   - We rely on the project-provided API: sqlite3Fts5HashScanNext and sqlite3Fts5HashScanEof
   - We construct a small in-process chain (Fts5HashScan) to simulate navigation
   - All tests are executed from main() with per-test descriptive comments
*/

/* 
   Candidate Keywords (from Step 1 understanding):
   - p, pScan, pScanNext
   - Fts5Hash, Fts5HashScan
   - sqlite3Fts5HashScanNext, sqlite3Fts5HashScanEof
   - End-of-Scan (EOF) condition, pScan == NULL
   - Chain traversal, next pointer, in-place mutation
*/

#include <iomanip>
#include <iostream>
#include <fts5Int.h>


// Bring in the definitions from the project's C headers.
// These definitions provide the actual Fts5Hash and Fts5HashScan types used by the library.

// Expose the C-API of the focal method for C++ testing.
// The library is C, so we declare extern "C" to avoid name mangling when linking.
extern "C" {
  // Prototypes are provided in the actual library; we declare them here for testing.
  int sqlite3Fts5HashScanEof(Fts5Hash *p);
  void sqlite3Fts5HashScanNext(Fts5Hash *p);
}

/*
  Lightweight runtime assertion macro that does not terminate on failure.
  It records the number of failures and prints diagnostic messages to std::cerr.
*/
static int g_test_failures = 0;
#define TEST_CHECK(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ \
                  << " - " << (msg) << "\n"; \
        ++g_test_failures; \
    } else { \
        std::cout << "[PASS] " << (msg) << "\n"; \
    } \
} while(0)

/*
  Helper function: print a short summary at the end.
*/
static void print_summary() {
    std::cout << "\nTest Summary: ";
    if(g_test_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
    } else {
        std::cout << g_test_failures << " TEST(S) FAILED" << std::endl;
    }
}

/*
  Test 1: Basic traversal through a 3-element scan chain.
  - Create a chain: s1 -> s2 -> s3 -> NULL
  - Start with pScan = &s1
  - Verify:
    a) EOF is false at start
    b) After 1st Next, pScan moves to s2
    c) After 2nd Next, pScan moves to s3
    d) After 3rd Next, pScan becomes NULL (EOF)
  - Verify EOF status after each step accordingly.
  Notes:
   - We rely on the library's sqlite3Fts5HashScanEof to determine EOF and sqlite3Fts5HashScanNext to advance.
   - We avoid invoking Next when at EOF to prevent triggering the runtime assert (as per the implementation).
*/
static void test_hash_scan_next_basic_traversal() {
    std::cout << "\n[Test 1] Basic traversal of 3-element scan chain\n";

    // Build a simple chain of scan nodes using the library's types.
    Fts5HashScan s3;
    Fts5HashScan s2;
    Fts5HashScan s1;

    // Link chain: s1 -> s2 -> s3 -> NULL
    s3.pScanNext = nullptr; // last node
    s2.pScanNext = &s3;
    s1.pScanNext = &s2;

    // Build the hash object with initial scan pointer.
    Fts5Hash h;
    // Setting pScan to the head of the chain (s1)
    h.pScan = &s1;

    // Step 1: Eof should be false at the start (pScan is non-null)
    TEST_CHECK(sqlite3Fts5HashScanEof(&h) == 0, "EOF is false at start (not at end)");

    // Step 2: Advance to s2
    sqlite3Fts5HashScanNext(&h);
    TEST_CHECK(h.pScan == &s2, "After 1st Next, pScan should point to s2");

    // Step 3: Check EOF again (still not at end)
    TEST_CHECK(sqlite3Fts5HashScanEof(&h) == 0, "EOF remains false after moving to s2");

    // Step 4: Advance to s3
    sqlite3Fts5HashScanNext(&h);
    TEST_CHECK(h.pScan == &s3, "After 2nd Next, pScan should point to s3");

    // Step 5: Check EOF again (still not at end)
    TEST_CHECK(sqlite3Fts5HashScanEof(&h) == 0, "EOF remains false after moving to s3");

    // Step 6: Advance to end (NULL)
    sqlite3Fts5HashScanNext(&h);
    TEST_CHECK(h.pScan == nullptr, "After 3rd Next, pScan should be NULL (EOF)");
    TEST_CHECK(sqlite3Fts5HashScanEof(&h) != 0, "EOF is true after reaching end");
}

/*
  Test 2: EOF boundary behavior and consistency checks.
  - Ensure EOF semantics behave as expected when chain is entirely NULL.
  - Also verify that advancing from a non-NULL node to NULL works and EOF becomes true.
  Note:
   - This test focuses on EOF evaluation and state transitions without triggering the assert by avoiding Next at EOF.
*/
static void test_hash_scan_next_eof_boundary() {
    std::cout << "\n[Test 2] EOF boundary behavior\n";

    Fts5HashScan a;
    Fts5HashScan b;
    Fts5HashScan c;
    c.pScanNext = nullptr;
    b.pScanNext = &c;
    a.pScanNext = &b;

    Fts5Hash h;
    h.pScan = &a;

    // Initially not at EOF
    TEST_CHECK(sqlite3Fts5HashScanEof(&h) == 0, "EOF false when chain exists (start with a)");

    // Move to b
    sqlite3Fts5HashScanNext(&h);
    TEST_CHECK(h.pScan == &b, "pScan advanced to b");
    TEST_CHECK(sqlite3Fts5HashScanEof(&h) == 0, "EOF false after moving to b");

    // Move to c
    sqlite3Fts5HashScanNext(&h);
    TEST_CHECK(h.pScan == &c, "pScan advanced to c");
    TEST_CHECK(sqlite3Fts5HashScanEof(&h) == 0, "EOF false after moving to c");

    // Move from c to NULL (end)
    sqlite3Fts5HashScanNext(&h);
    TEST_CHECK(h.pScan == nullptr, "pScan moved to NULL (end)");
    TEST_CHECK(sqlite3Fts5HashScanEof(&h) != 0, "EOF true after end (NULL pScan)");
}

/*
  Main entry: runs all tests and reports a summary.
*/
int main() {
    std::cout << "Starting sqlite3Fts5HashScanNext unit tests (C++11 harness)\n";

    test_hash_scan_next_basic_traversal();
    test_hash_scan_next_eof_boundary();

    print_summary();
    return g_test_failures != 0 ? 1 : 0;
}
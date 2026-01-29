/*
  Unit test suite for sqlite3Fts5TermsetNew (focal method)

  Approach:
  - Re-create a minimal environment to exercise the focal function:
    int sqlite3Fts5TermsetNew(Fts5Termset **pp) which internally calls
    sqlite3Fts5MallocZero(&rc, sizeof(Fts5Termset)).
  - Provide a controllable mock for sqlite3Fts5MallocZero to cover both success
    and failure paths.
  - Verify:
    • rc == SQLITE_OK on success
    • pp is non-null and memory is zero-initialized (termset is zeroed)
    • on allocation failure, rc != SQLITE_OK and pp remains nullptr
  - No GTest; a lightweight in-file test harness is used with simple, non-terminating assertions.

  Notes:
  - This test file is self-contained and does not rely on external test frameworks.
  - If integrating into a larger test suite, you can adapt the assertion primitives.

  Core keywords for test focus: sqlite3Fts5TermsetNew, Fts5Termset, sqlite3Fts5MallocZero, zero-initialization, SQLITE_OK, allocation failure.
*/

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fts5Int.h>


// Lightweight, framework-free assertion helpers
static int g_tests_run = 0;
static int g_tests_failed = 0;

static void test_ok(const char* msg) {
    std::cout << "OK: " << msg << std::endl;
}

static void test_fail(const char* msg) {
    std::cerr << "FAIL: " << msg << std::endl;
}

static void assert_true(bool cond, const char* msg) {
    ++g_tests_run;
    if (cond) {
        test_ok(msg);
    } else {
        test_fail(msg);
        ++g_tests_failed;
    }
}

// Minimal types and constants to model the focal API surface
// In a real project these would come from the actual headers (fts5Int.h, sqlite3.h, etc.)
#define SQLITE_OK 0

typedef long long sqlite3_int64;

// Simple placeholder for the real Fts5Termset structure
struct Fts5Termset {
    int dummy; // used to verify zero-initialization
};

// Forward declarations for the focal API surface implemented in this test
extern "C" {
    void *sqlite3Fts5MallocZero(int *pRc, sqlite3_int64 nByte);
    int sqlite3Fts5TermsetNew(Fts5Termset **pp);
}

// Global switch to control malloc behavior (success vs. failure) for tests
static bool g_failMalloc = false;

/*
  Mock implementation of sqlite3Fts5MallocZero to drive test scenarios.

  - On success: sets *pRc = SQLITE_OK and returns a zero-initialized block
  - On failure: sets *pRc to a non-zero error code and returns nullptr
*/
extern "C" void *sqlite3Fts5MallocZero(int *pRc, sqlite3_int64 nByte) {
    if (g_failMalloc) {
        if (pRc) *pRc = 6; // arbitrary non-zero error code to indicate OOM/Fail
        return nullptr;
    }
    if (pRc) *pRc = SQLITE_OK;
    void *p = std::malloc((size_t)nByte);
    if (p) std::memset(p, 0, (size_t)nByte);
    return p;
}

/*
  Fts5TermsetNew implementation under test.

  This mirrors the focal method behavior:
  - Initialize rc = SQLITE_OK
  - Allocate and zero-initialize a Fts5Termset via sqlite3Fts5MallocZero
  - Return the rc value (potentially modified by the allocator)
*/
extern "C" int sqlite3Fts5TermsetNew(Fts5Termset **pp) {
    int rc = SQLITE_OK;
    *pp = sqlite3Fts5MallocZero(&rc, sizeof(Fts5Termset));
    return rc;
}

/* 
  Test 1: Success path
  - Expect rc == SQLITE_OK
  - Expect pp != nullptr
  - Expect allocated memory to be zero-initialized (Fts5Termset.dummy == 0)
*/
static void test_TermsetNew_Success() {
    g_failMalloc = false;
    Fts5Termset *p = nullptr;

    int rc = sqlite3Fts5TermsetNew(&p);
    assert_true(rc == SQLITE_OK, "sqlite3Fts5TermsetNew returns SQLITE_OK on success");
    assert_true(p != nullptr, "sqlite3Fts5TermsetNew should set pp to a non-null pointer on success");
    // Verify zero-initialization by inspecting the first member
    int firstMember = p ? p->dummy : -1;
    assert_true(p != nullptr && firstMember == 0, "Allocated Fts5Termset memory should be zero-initialized");

    // Cleanup
    std::free(p);
}

/* 
  Test 2: Failure path
  - Simulate allocation failure via g_failMalloc
  - Expect rc != SQLITE_OK
  - Expect pp to remain nullptr
*/
static void test_TermsetNew_Failure() {
    g_failMalloc = true;
    Fts5Termset *p = nullptr;

    int rc = sqlite3Fts5TermsetNew(&p);
    assert_true(rc != SQLITE_OK, "sqlite3Fts5TermsetNew returns non-zero on allocation failure");
    assert_true(p == nullptr, "pp should be nullptr when allocation fails");
}

// Entry point for running tests
int main() {
    std::cout << "Starting tests for sqlite3Fts5TermsetNew (focal method)" << std::endl;

    test_TermsetNew_Success();
    test_TermsetNew_Failure();

    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << std::endl;

    // Return non-zero if any test failed
    return g_tests_failed == 0 ? 0 : 1;
}
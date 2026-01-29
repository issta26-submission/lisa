/*
Unit test suite for the focal method:
  int sqlite3Fts5BufferSize(int *pRc, Fts5Buffer *pBuf, u32 nByte)

Context and approach:
- We compile with the actual fts5_buffer.c implementation available in the test environment.
- The tests focus on behavior of buffer resizing logic:
  - No reallocation when current space suffices.
  - Reallocation to a larger, computed size when needed (including edge cases like 0 initial space).
  - Failure path when realloc fails (setting the return code and leaving the buffer unchanged).
- We provide a lightweight, self-contained C++11 test harness (no GoogleTest). It uses a small framework of inline asserts that do not terminate on failure to maximize coverage.
- A minimal mock for sqlite3_realloc64 is provided to simulate allocation behavior and to inspect allocation size requested by the focal function.
- We declare a compatible Fts5Buffer struct and the needed types to align with the focal function signature.
- The tests are designed to be deterministic and cover true/false branches of key predicates.

Notes:
- The environment is assumed to compile fts5_buffer.c together with this test file.
- We avoid relying on private members or methods; tests interact through sqlite3Fts5BufferSize as the focal point.
- All test case explanations are added as inline comments next to each test section.

Implementation details:
- Lightweight test framework:
  - ASSERT_TRUE(cond, msg) records a non-fatal failure message when cond is false.
  - runTest(name, lambda) executes a test and reports PASS/FAIL without aborting on failures.
  - Global counters report overall results.
- Memory behavior:
  - A global flag g_realloc_fail controls whether sqlite3_realloc64 simulates success or failure.
  - sqlite3_realloc64 is implemented with C linkage to intercept calls from the focal function; it uses realloc to simulate behavior and records the last requested allocation size for verification.
- Coverage goals:
  - Case: No reallocation when pBuf->nSpace >= nByte.
  - Case: Reallocation with non-zero nSpace, requiring growth to a power-of-two boundary that exceeds nByte.
  - Case: Reallocation fails (pNew == NULL) sets *pRc to non-zero (SQLITE_NOMEM) and returns 1.
  - Case: Initial space is zero; first allocation uses 64 bytes as per the function logic.
*/

#include <functional>
#include <vector>
#include <fts5Int.h>
#include <string>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Domain-specific typedefs matching the focal code expectations
typedef unsigned int u32;
typedef unsigned char u8;
typedef unsigned long long u64;

// Forward declaration/definition of Fts5Buffer used by the focal function
// The real project defines this struct; we mirror the minimal layout used by sqlite3Fts5BufferSize.
struct Fts5Buffer {
    int nSpace;  // current allocated space (in bytes)
    void *p;     // allocated memory pointer
};

// Prototypes of the focal function (as seen in the provided code snippet)
extern "C" int sqlite3Fts5BufferSize(int *pRc, Fts5Buffer *pBuf, u32 nByte);

// We'll provide a small stub for sqlite3_realloc64 to control allocation behavior during tests.
// The focal function links against sqlite3_realloc64, so we expose it with C linkage.
static int g_realloc_fail = 0;                 // if non-zero, simulate realloc64 failure
static size_t g_last_alloc_size = 0;           // last allocation size requested by realloc64

extern "C" void* sqlite3_realloc64(void* p, unsigned long long n) {
    if (g_realloc_fail) {
        // Simulate allocation failure
        return nullptr;
    }
    // Simple realloc simulation using standard library; handle NULL as malloc
    void* q = nullptr;
    if (n == 0) {
        free(p);
        q = nullptr;
    } else {
        q = realloc(p, static_cast<size_t>(n));
    }
    if (q != nullptr) {
        g_last_alloc_size = static_cast<size_t>(n);
    }
    return q;
}

// Lightweight test harness
static std::vector<std::string> g_failures;  // per-test failure messages

#define ASSERT_TRUE(cond, msg) do { \
    if(!(cond)) { g_failures.push_back(std::string("Assertion failed: ") + (msg)); } \
} while(0)

static int g_total_tests = 0;
static int g_passed_tests = 0;

// Run a single test case (provided as a lambda)
static void runTest(const std::string& name, const std::function<void()> &testBody) {
    g_failures.clear();
    testBody();
    ++g_total_tests;
    if (g_failures.empty()) {
        std::cout << "PASS: " << name << std::endl;
        ++g_passed_tests;
    } else {
        std::cout << "FAIL: " << name << " (" << g_failures.size() << " failures)" << std::endl;
        for (const auto &f : g_failures) {
            std::cout << "  - " << f << std::endl;
        }
    }
    // Persist per-test messages are cleared by testBody for isolation
}

// Test 1: No reallocation is needed when pBuf->nSpace >= nByte
static void test_no_realloc_needed() {
    // Arrange
    g_realloc_fail = 0;
    g_last_alloc_size = 0;
    Fts5Buffer buf;
    buf.nSpace = 128;
    buf.p = malloc(128);
    void *originalPtr = buf.p;
    int rc = 0;

    // Act
    sqlite3Fts5BufferSize(&rc, &buf, 100);

    // Assert
    ASSERT_TRUE(rc == 0, "rc should be 0 when no reallocation occurs");
    ASSERT_TRUE(buf.p == originalPtr, "buffer pointer should remain unchanged when space suffices");
    ASSERT_TRUE(buf.nSpace == 128, "nSpace should remain unchanged when no reallocation occurs");
    // No realloc should have happened; last_alloc_size should stay 0
    ASSERT_TRUE(g_last_alloc_size == 0, "no allocation size should have been requested");
    free(originalPtr);
}

// Test 2: Reallocate when there isn't enough space and realloc succeeds
static void test_realloc_success() {
    // Arrange
    g_realloc_fail = 0;
    g_last_alloc_size = 0;
    Fts5Buffer buf;
    buf.nSpace = 16;
    buf.p = malloc(16);
    void *originalPtr = buf.p;
    int rc = 0;

    // Act
    sqlite3Fts5BufferSize(&rc, &buf, 70);

    // Assert
    ASSERT_TRUE(rc == 0, "rc should be 0 on successful reallocation");
    ASSERT_TRUE(buf.p != NULL, "buffer pointer should be non-null after successful realloc");
    ASSERT_TRUE(buf.p != originalPtr, "buffer pointer should change after realloc grows");
    ASSERT_TRUE(buf.nSpace == 128, "nSpace should be updated to 128 after doubling to meet 70 bytes");
    ASSERT_TRUE(g_last_alloc_size == 128, "allocation size reported should be 128 after reallocation");
    // Cleanup
    free(buf.p == originalPtr ? NULL : buf.p); // since p has changed, free current
}

// Test 3: Reallocate fails (simulate SQLITE_NOMEM path)
static void test_realloc_failure() {
    // Arrange
    g_realloc_fail = 1;  // force realloc64 to fail
    Fts5Buffer buf;
    buf.nSpace = 16;
    buf.p = malloc(16);
    void *originalPtr = buf.p;
    int rc = 0;

    // Act
    sqlite3Fts5BufferSize(&rc, &buf, 70);

    // Assert
    // On failure, function should return non-zero and not modify the buffer
    ASSERT_TRUE(rc != 0, "rc should be non-zero when realloc fails (OOM)");
    ASSERT_TRUE(buf.p == originalPtr, "buffer pointer should remain unchanged on realloc failure");
    ASSERT_TRUE(buf.nSpace == 16, "nSpace should remain unchanged on realloc failure");
    // Cleanup
    free(originalPtr);
}

// Test 4: Initial space is zero; first allocation should be 64 bytes
static void test_zero_initial_space_allocates_64() {
    // Arrange
    g_realloc_fail = 0;
    g_last_alloc_size = 0;
    Fts5Buffer buf;
    buf.nSpace = 0;
    buf.p = nullptr;
    int rc = 0;

    // Act
    sqlite3Fts5BufferSize(&rc, &buf, 50);

    // Assert
    ASSERT_TRUE(rc == 0, "rc should be 0 after initial allocation to accommodate nByte");
    ASSERT_TRUE(buf.p != NULL, "buffer pointer should be non-null after initial allocation");
    ASSERT_TRUE(buf.nSpace == 64, "nSpace should initialize to 64 when starting from 0");
    ASSERT_TRUE(g_last_alloc_size == 64, "allocation size should be 64 for initial allocation");
    // Cleanup
    free(buf.p);
}

// Entry point
int main() {
    // Run test suite
    runTest("No realloc needed", test_no_realloc_needed);
    runTest("Realloc succeeds when space is insufficient", test_realloc_success);
    runTest("Realloc fails gracefully (OOM path)", test_realloc_failure);
    runTest("Initial zero space allocates 64 bytes", test_zero_initial_space_allocates_64);

    // Summary
    std::cout << "Tests run: " << g_total_tests << ", Passed: " << g_passed_tests
              << ", Failed: " << (g_total_tests - g_passed_tests) << std::endl;

    return (g_total_tests == g_passed_tests) ? 0 : 1;
}
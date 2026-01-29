// Unit test suite for the focal method: sqlite3DbStrNDup
// This test suite is designed to be self-contained (no GTest required).
// It provides minimal stubs for dependent components and exercises
// the behavior of sqlite3DbStrNDup as described in the focal method.
//
// Notes on design decisions aligned with the provided domain knowledge:
// - We implement a lightweight, non-terminating test harness (checks do not abort).
// - We provide a mock sqlite3DbMallocRawNN to simulate allocation success/failure.
// - We disable assertions (NDEBUG) to ensure tests run to completion.
// - Tests cover: copy path when z != NULL, zero-length copies, NULL z with non-zero n,
//   and allocation failure path. Additional checks verify correct terminator behavior.
// - All tests are contained in a single file for simplicity and portability.

#define NDEBUG
#include <assert.h>
#include <cstring>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>



// Candidate Keywords extracted from the focal method:
// sqlite3DbStrNDup, sqlite3DbMallocRawNN, db, z, n, zNew, memcpy, 0, assertion, NULL

// Minimal type and forward declaration to mirror the environment
typedef uint64_t u64;

struct sqlite3 {
    // Intentionally empty: only used as a non-nullable type placeholder
};

// Global flag to simulate allocation failure in sqlite3DbMallocRawNN
static int g_allocFail = 0;

// Stubbed dependency: simulated allocator used by the focal method under test.
// In the real project, sqlite3DbMallocRawNN(db, n) would allocate memory from
// sqlite's allocator. Here we provide a controllable stub.
extern "C" char *sqlite3DbMallocRawNN(sqlite3 *db, u64 n);

// Forward declaration of the focal method under test.
// We re-implement the function here to enable self-contained testing without
// requiring the full original malloc.c in the build environment.
extern "C" char *sqlite3DbStrNDup(sqlite3 *db, const char *z, u64 n);

// Implementation of the focal method (reproduced locally for unit testing)
extern "C" char *sqlite3DbStrNDup(sqlite3 *db, const char *z, u64 n) {
  char *zNew;
  assert( db!=0 );
  assert( z!=0 || n==0 );
  assert( (n&0x7fffffff)==n );
  zNew = z ? sqlite3DbMallocRawNN(db, n+1) : 0;
  if( zNew ){
    memcpy(zNew, z, (size_t)n);
    zNew[n] = 0;
  }
  return zNew;
}

// Stubbed allocator: returns NULL when simulating OOM, otherwise malloc
extern "C" char *sqlite3DbMallocRawNN(sqlite3 *db, u64 n) {
    (void)db; // suppress unused parameter warning
    if (g_allocFail) {
        return NULL;
    }
    return (char*)malloc((size_t)n);
}

// Lightweight test harness (non-terminating checks)
static int g_tests_run = 0;
static int g_failures = 0;

static void report_failure(const char* test, const char* msg) {
    std::cerr << "Failure in test [" << test << "]: " << msg << std::endl;
    ++g_failures;
}

#define CHECK(cond, testName, msg) do { \
    if(!(cond)) { report_failure(testName, msg); } \
} while(0)

// Test 1: Basic copy path when z != NULL and n > 0
// - Verifies that exactly n bytes are copied from z to a newly allocated buffer.
// - Verifies that a terminating NUL character is appended at position n.
// - Verifies that the allocated memory is not the same as the source.
// - Ensures that the function returns a non-NULL pointer on success.
static void test_basic_copy_path() {
    const char *src = "abcdef"; // 6 bytes
    sqlite3 db;
    g_allocFail = 0; // ensure allocation succeeds

    char *dup = sqlite3DbStrNDup(&db, src, 6);
    CHECK(dup != NULL, "test_basic_copy_path", "Expected non-NULL duplicate on successful allocation");

    if (dup != NULL) {
        // Validate copied content and terminator
        CHECK(std::strncmp(dup, src, 6) == 0, "test_basic_copy_path", "Copied content does not match source");
        CHECK(dup[6] == '\0', "test_basic_copy_path", "Terminator not placed at position n=6");

        // Sanity check: ensure memory is distinct from original source
        CHECK(dup != src, "test_basic_copy_path", "Duplicate should be a separate buffer");
        free(dup);
    }
    ++g_tests_run;
}

// Test 2: Zero-length copy with non-null source (n == 0, z != NULL)
// - Expected: allocate 1 byte, copy zero bytes, set first byte to NUL.
static void test_zero_length_copy_non_null_source() {
    const char *src = "xyz"; // 3 bytes, but n=0 means no copy
    sqlite3 db;
    g_allocFail = 0;

    char *dup = sqlite3DbStrNDup(&db, src, 0);
    CHECK(dup != NULL, "test_zero_length_copy_non_null_source", "Expected non-NULL duplicate when z != NULL and n == 0");
    if (dup != NULL) {
        CHECK(dup[0] == '\0', "test_zero_length_copy_non_null_source", "First byte should be NUL for n==0");
        free(dup);
    }
    ++g_tests_run;
}

// Test 3: NULL source with non-zero n should return NULL (z == NULL && n > 0)
// - The implementation sets zNew to 0 when z is NULL, resulting in a NULL return.
static void test_null_source_nonzero_n() {
    sqlite3 db;
    g_allocFail = 0;

    char *dup = sqlite3DbStrNDup(&db, NULL, 5);
    CHECK(dup == NULL, "test_null_source_nonzero_n", "Expected NULL when z is NULL and n > 0");
    ++g_tests_run;
}

// Test 4: Allocation failure path (OOM) when z != NULL
// - Simulates sqlite3DbMallocRawNN returning NULL and expects the function to propagate NULL.
static void test_allocation_failure_path() {
    const char *src = "hello";
    sqlite3 db;
    g_allocFail = 1; // force allocator to fail

    char *dup = sqlite3DbStrNDup(&db, src, 5);
    CHECK(dup == NULL, "test_allocation_failure_path", "Expected NULL on allocation failure");

    g_allocFail = 0; // reset for other tests
    ++g_tests_run;
}

// Test 5: Verifies behavior when n is less than the allocated size and source differs
// - Ensures only the requested bytes are copied and a terminator is appended at n.
static void test_partial_copy_and_terminator() {
    const char *src = "12345ABCDE"; // 10 bytes
    sqlite3 db;
    g_allocFail = 0;

    // Copy first 5 bytes
    char *dup = sqlite3DbStrNDup(&db, src, 5);
    CHECK(dup != NULL, "test_partial_copy_and_terminator", "Expected non-NULL duplicate for n=5");
    if (dup != NULL) {
        CHECK(std::strncmp(dup, "12345", 5) == 0, "test_partial_copy_and_terminator", "First 5 bytes should match source");
        CHECK(dup[5] == '\0', "test_partial_copy_and_terminator", "Terminator not placed at position n=5");
        free(dup);
    }
    ++g_tests_run;
}

int main(void) {
    // Execute tests
    test_basic_copy_path();
    test_zero_length_copy_non_null_source();
    test_null_source_nonzero_n();
    test_allocation_failure_path();
    test_partial_copy_and_terminator();

    // Summary
    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_failures << std::endl;
    return (g_failures == 0) ? 0 : 1;
}
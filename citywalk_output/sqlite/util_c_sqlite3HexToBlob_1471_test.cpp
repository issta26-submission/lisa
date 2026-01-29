/*
  Unit Test Suite for sqlite3HexToBlob
  - Implements minimal stubs for dependencies used by sqlite3HexToBlob
  - Tests cover normal behavior and edge cases described by the focal method
  - No Google Test; a small in-file test harness with non-terminating assertions
  - All tests are self-contained in a single translation unit for simplicity

  Notes:
  - The focal function has a subtle behavior (and a potential off-by-one) that is exercised by tests.
  - We provide a controlled malloc allocator to simulate success/failure of sqlite3DbMallocRawNN.
  - Tests are annotated with explanations describing what each test verifies.
*/

#include <math.h>
#include <cstring>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>
#include <cstdlib>


// ---------------------------------------------------------------------------
// Minimal stubs to mimic sqlite3 internal API used by sqlite3HexToBlob
// These are provided solely for test compilation/execution in this environment.
// The real project would link against the actual sqlite3 implementation.
// ---------------------------------------------------------------------------

struct sqlite3 {
  int dummy; // placeholder to mimic opaque db handle
};

// Global flag to simulate allocation failure in sqlite3DbMallocRawNN
static bool g_malloc_fail = false;

// Set allocation failure mode for tests
static void sqlite3SetMallocFail(bool fail) {
  g_malloc_fail = fail;
}

// Minimal allocator used by sqlite3HexToBlob
static void* sqlite3DbMallocRawNN(sqlite3* /*db*/, int n) {
  if (g_malloc_fail) return NULL;
  if (n <= 0) return NULL;
  return std::malloc(static_cast<size_t>(n));
}

// Convert a single hex character to its integer value.
// This mirrors the sqlite3HexToInt logic used by the focal function.
static int sqlite3HexToInt(int h) {
  if (h >= '0' && h <= '9') return h - '0';
  if (h >= 'a' && h <= 'f') return 10 + (h - 'a');
  if (h >= 'A' && h <= 'F') return 10 + (h - 'A');
  return 0;
}

// The focal method under test (reproduced here for the test harness).
// It converts a hex-encoded string z (length n) into a blob, allocating with sqlite3DbMallocRawNN.
// Note: The implementation mirrors the provided source and includes its quirky behavior.
static void* sqlite3HexToBlob(sqlite3 *db, const char *z, int n){
  char *zBlob;
  int i;
  zBlob = (char *)sqlite3DbMallocRawNN(db, n/2 + 1);
  n--;
  if( zBlob ){
    for(i=0; i<n; i+=2){
      zBlob[i/2] = (sqlite3HexToInt(z[i])<<4) | sqlite3HexToInt(z[i+1]);
    }
    zBlob[i/2] = 0;
  }
  return zBlob;
}

// ---------------------------------------------------------------------------
// Simple non-terminating assertion framework (no abort on failure)
// to maximize code coverage across tests while reporting failures at end.
// ---------------------------------------------------------------------------

static int g_failures = 0;

#define TEST_LOG(msg) do { std::cerr << "LOG: " << msg << std::endl; } while(0)
#define CHECK(cond, msg) \
  do { \
    if(!(cond)) { \
      std::cerr << "FAIL: " << (msg) << " (" << __FUNCTION__ << ")" << std::endl; \
      ++g_failures; \
    } \
  } while(0)

// ---------------------------------------------------------------------------
// Test Suite
// Each test is designed to exercise particular branches and scenarios
// (normal operation, odd/even lengths, and allocation failure).
// ---------------------------------------------------------------------------

/*
  Test 1: Even-length hex string with 4 hex digits ("4142")
  Expected behavior:
    - blob[0] == 0x41
    - blob[1] == 0x00 (due to the post-loop write to zBlob[i/2] = 0)
    - blob is non-NULL
*/
static void test_hex_to_blob_even4() {
  sqlite3 db;
  const char *z = "4142";
  int n = 4;
  unsigned char *blob = (unsigned char*)sqlite3HexToBlob(&db, z, n);
  CHECK(blob != NULL, "blob should not be NULL for valid allocation");
  if (blob) {
    CHECK(blob[0] == 0x41, "First byte should be 0x41 (hex '41')");
    CHECK(blob[1] == 0x00, "Second byte should be 0x00 due to post-loop overwrite");
  }
  if (blob) std::free(blob);
}

/*
  Test 2: Even-length hex string with 6 hex digits ("414243")
  Expected behavior:
    - blob[0] == 0x41
    - blob[1] == 0x42
    - blob[2] == 0 (terminator-like behavior due to overwrite)
*/
static void test_hex_to_blob_even6() {
  sqlite3 db;
  const char *z = "414243";
  int n = 6;
  unsigned char *blob = (unsigned char*)sqlite3HexToBlob(&db, z, n);
  CHECK(blob != NULL, "blob should not be NULL for valid allocation");
  if (blob) {
    CHECK(blob[0] == 0x41, "First byte should be 0x41 (hex '41')");
    CHECK(blob[1] == 0x42, "Second byte should be 0x42 (hex '42')");
    CHECK(blob[2] == 0x00, "Third byte should be 0x00 due to overwrite in final step");
  }
  if (blob) std::free(blob);
}

/*
  Test 3: Odd-length hex string with 5 hex digits ("41424")
  Expected behavior:
    - blob[0] == 0x41
    - blob[1] == 0x00 (second byte overwritten by terminator)
*/
static void test_hex_to_blob_odd5() {
  sqlite3 db;
  const char *z = "41424";
  int n = 5;
  unsigned char *blob = (unsigned char*)sqlite3HexToBlob(&db, z, n);
  CHECK(blob != NULL, "blob should not be NULL for valid allocation");
  if (blob) {
    CHECK(blob[0] == 0x41, "First byte should be 0x41 (hex '41')");
    CHECK(blob[1] == 0x00, "Second byte should be 0x00 due to overwrite when n is odd");
  }
  if (blob) std::free(blob);
}

/*
  Test 4: Allocation failure scenario
  Simulate sqlite3DbMallocRawNN returning NULL and ensure function propagates NULL.
*/
static void test_allocation_failure_propagates_null() {
  sqlite3 db;
  const char *z = "414243";
  int n = 6;
  sqlite3SetMallocFail(true); // force allocator to fail
  unsigned char *blob = (unsigned char*)sqlite3HexToBlob(&db, z, n);
  CHECK(blob == NULL, "blob should be NULL when allocation fails");
  sqlite3SetMallocFail(false); // reset allocator for other tests
  // No deallocation needed as blob is NULL
}

/*
  Test 5: Lowercase hex input verification
  Input "616263" should decode to first two bytes 0x61, 0x62
*/
static void test_hex_to_blob_lowercase() {
  sqlite3 db;
  const char *z = "616263"; // hex for 'a','b','c'
  int n = 6;
  unsigned char *blob = (unsigned char*)sqlite3HexToBlob(&db, z, n);
  CHECK(blob != NULL, "blob should not be NULL for valid allocation");
  if (blob) {
    CHECK(blob[0] == 0x61, "First byte should be 0x61 ('a')");
    CHECK(blob[1] == 0x62, "Second byte should be 0x62 ('b')");
    CHECK(blob[2] == 0x00, "Third byte should be 0 due to overwrite behavior");
  }
  if (blob) std::free(blob);
}

// ---------------------------------------------------------------------------
// Main: Run all tests and report summary
// ---------------------------------------------------------------------------

int main() {
  TEST_LOG("Starting sqlite3HexToBlob unit tests");

  test_hex_to_blob_even4();
  test_hex_to_blob_even6();
  test_hex_to_blob_odd5();
  test_allocation_failure_propagates_null();
  test_hex_to_blob_lowercase();

  if (g_failures == 0) {
    std::cout << "ALL TESTS PASSED" << std::endl;
    return 0;
  } else {
    std::cerr << g_failures << " TEST(S) FAILED" << std::endl;
    return 1;
  }
}
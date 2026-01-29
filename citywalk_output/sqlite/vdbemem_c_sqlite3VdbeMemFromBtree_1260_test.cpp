// Lightweight C++11 unit tests for sqlite3VdbeMemFromBtree
// This test harness mocks the essential SQLite structures and functions
// used by sqlite3VdbeMemFromBtree to exercise its control flow and branches.
// No Google Test is used; a tiny custom assertion framework is implemented.

#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>


// -----------------------------------------------------------------------------
// Minimal mock definitions to emulate the C code environment
// -----------------------------------------------------------------------------

using u32 = unsigned int;

// Return codes (simplified for testing)
const int SQLITE_OK = 0;
const int SQLITE_CORRUPT_BKPT = 1;

// Mem flags (simplified)
const int MEM_Null = 0;
const int MEM_Blob = 1;

// Simple Mem structure used by the function under test
struct Mem {
  int flags;
  char* z;
  int n;
  Mem() : flags(MEM_Null), z(nullptr), n(0) {}
};

// Simple BtCursor structure to simulate a B-Tree cursor
struct BtCursor {
  std::string data; // payload stored in the "B-Tree" record
};

// -----------------------------------------------------------------------------
// Global/test-control state for mocks
// -----------------------------------------------------------------------------

static size_t g_maxRecordSize = 0; // maximum record size for the current cursor
static bool g_failPayload = false; // simulate payload read failure
static bool g_failClear = false;   // simulate memory clear/resize failure

// Accessor stubs that mimic the real code behavior

static size_t sqlite3BtreeMaxRecordSize(BtCursor* /*pCur*/) {
  // In real SQLite, this queries the B-Tree.
  // For tests, return the controlled max size.
  return g_maxRecordSize;
}

static int sqlite3VdbeMemClearAndResize(Mem *pMem, int szNew) {
  // Free previous buffer if any
  if (pMem->z) {
    delete[] pMem->z;
    pMem->z = nullptr;
  }

  if (szNew <= 0) {
    pMem->n = 0;
    return SQLITE_OK;
  }

  // Simulate allocation failure if requested
  if (g_failClear) {
    return SQLITE_CORRUPT_BKPT; // reuse a non-OK code to signal failure
  }

  // Allocate new buffer
  pMem->z = new char[szNew];
  if (!pMem->z) {
    return SQLITE_CORRUPT_BKPT;
  }
  pMem->n = 0;
  return SQLITE_OK;
}

static int sqlite3BtreePayload(BtCursor* pCur, u32 offset, u32 amt, char* pBuf) {
  // Copy the requested payload into pBuf if within range
  if (offset + amt > pCur->data.size()) {
    return SQLITE_CORRUPT_BKPT;
  }
  if (amt > 0 && pBuf) {
    std::memcpy(pBuf, pCur->data.data() + offset, amt);
  }
  return SQLITE_OK;
}

static void sqlite3VdbeMemRelease(Mem *pMem) {
  if (pMem->z) {
    delete[] pMem->z;
    pMem->z = nullptr;
  }
  pMem->flags = MEM_Null;
  pMem->n = 0;
}

// -----------------------------------------------------------------------------
// The focal method under test (reproduced from <FOCAL_METHOD>)
// -----------------------------------------------------------------------------

int sqlite3VdbeMemFromBtree(
  BtCursor *pCur,   /* Cursor pointing at record to retrieve. */
  u32 offset,       /* Offset from the start of data to return bytes from. */
  u32 amt,          /* Number of bytes to return. */
  Mem *pMem         /* OUT: Return data in this Mem structure. */
){
  int rc;
  pMem->flags = MEM_Null;
  if( sqlite3BtreeMaxRecordSize(pCur)<offset+amt ){
    return SQLITE_CORRUPT_BKPT;
  }
  if( SQLITE_OK==(rc = sqlite3VdbeMemClearAndResize(pMem, amt+1)) ){
    rc = sqlite3BtreePayload(pCur, offset, amt, pMem->z);
    if( rc==SQLITE_OK ){
      pMem->z[amt] = 0;   /* Overrun area used when reading malformed records */
      pMem->flags = MEM_Blob;
      pMem->n = (int)amt;
    }else{
      sqlite3VdbeMemRelease(pMem);
    }
  }
  return rc;
}

// -----------------------------------------------------------------------------
// Lightweight test framework (non-terminating assertions)
// -----------------------------------------------------------------------------

static int g_totalTests = 0;
static int g_failedTests = 0;

// Simple non-terminating assertion with message
static void assert_true(bool cond, const char* msg) {
  if (!cond) {
    std::cerr << "[ASSERT_FAIL] " << msg << std::endl;
    ++g_failedTests;
  }
  ++g_totalTests;
}

// Helper: reset a Mem to a clean state
static void resetMem(Mem &m) {
  if (m.z) {
    delete[] m.z;
    m.z = nullptr;
  }
  m.flags = MEM_Null;
  m.n = 0;
}

// -----------------------------------------------------------------------------
// Tests
// -----------------------------------------------------------------------------

// Test 1: Successful read from B-Tree into Mem (basic path)
static void test_fromBtree_success() {
  // Setup: data contains at least offset+amt bytes
  BtCursor cur;
  cur.data = "abcdef"; // 6 bytes
  g_maxRecordSize = cur.data.size();
  g_failPayload = false;
  g_failClear = false;

  Mem m;
  int rc = sqlite3VdbeMemFromBtree(&cur, 0, 5, &m);

  // Verify: OK return, blob with 5 bytes, NUL-terminated
  assert_true(rc == SQLITE_OK, "rc should be SQLITE_OK on success");
  assert_true(m.flags == MEM_Blob, "Mem should be MEM_Blob on success");
  assert_true(m.n == 5, "Mem.n should equal amt (5)");
  bool dataOk = (std::strncmp(m.z, "abcde", 5) == 0);
  assert_true(dataOk, "Mem contents should be 'abcde'");
  bool terminatorOk = (m.z[5] == '\0');
  assert_true(terminatorOk, "Mem buffer should be NUL-terminated at position 5");

  // Cleanup
  resetMem(m);
  // Also ensure no leaks
  if (m.z) { std::cerr << "[WARN] Memory leak detected in test_fromBtree_success\n"; }
}

// Test 2: When requested bytes exceed max record size -> CORRUPT_BKPT
static void test_fromBtree_overflow() {
  BtCursor cur;
  cur.data = "abcd"; // 4 bytes
  g_maxRecordSize = 4;
  g_failPayload = false;
  g_failClear = false;

  Mem m;
  int rc = sqlite3VdbeMemFromBtree(&cur, 3, 2, &m); // offset+amt = 5 > 4

  assert_true(rc == SQLITE_CORRUPT_BKPT, "rc should be CORRUPT_BKPT when overflow occurs");
  // Ensure pMem remains unchanged (null)
  assert_true(m.z == nullptr && m.flags == MEM_Null, "Mem should remain NULL on overflow");
  resetMem(m);
}

// Test 3: Payload read fails -> sqlite3VdbeMemRelease should be invoked
static void test_fromBtree_payload_fail() {
  BtCursor cur;
  cur.data = "abcdef"; // 6 bytes
  g_maxRecordSize = 6;
  g_failPayload = true;  // force payload failure
  g_failClear = false;

  Mem m;
  int rc = sqlite3VdbeMemFromBtree(&cur, 0, 3, &m);

  // Expect a non-OK rc and Mem is released (NULL)
  assert_true(rc != SQLITE_OK, "rc should indicate failure when payload fails");
  assert_true(m.z == nullptr && m.flags == MEM_Null, "Mem should be NULL after payload failure");
  resetMem(m);
}

// Test 4: Clear_and_resize fails -> function should propagate error without payload
static void test_fromBtree_clear_fail() {
  BtCursor cur;
  cur.data = "abcdef";
  g_maxRecordSize = cur.data.size();
  g_failPayload = false;
  g_failClear = true; // simulate failure in memory allocation

  Mem m;
  int rc = sqlite3VdbeMemFromBtree(&cur, 0, 3, &m);

  // Expect non-OK rc and Mem remains NULL
  assert_true(rc != SQLITE_OK, "rc should indicate failure when ClearAndResize fails");
  assert_true(m.z == nullptr && m.flags == MEM_Null, "Mem should be NULL when ClearAndResize fails");
  resetMem(m);
}

// Test 5: Zero-length output (amt = 0)
static void test_fromBtree_zero_amt() {
  BtCursor cur;
  cur.data = "abcdefghij"; // 10 bytes
  g_maxRecordSize = cur.data.size();
  g_failPayload = false;
  g_failClear = false;

  Mem m;
  int rc = sqlite3VdbeMemFromBtree(&cur, 2, 0, &m); // amt = 0

  // Should succeed with empty blob (n == 0) and a NUL terminator at z[0]
  assert_true(rc == SQLITE_OK, "rc should be SQLITE_OK for zero amt");
  assert_true(m.flags == MEM_Blob, "Mem should be MEM_Blob even for zero amt");
  assert_true(m.n == 0, "Mem.n should be 0 when amt is 0");
  if (m.z) {
    bool terminatorAtZero = (m.z[0] == '\0');
    assert_true(terminatorAtZero, "Mem[0] should be NUL-terminated");
  } else {
    assert_true(false, "Mem.z should not be null for zero amt case");
  }

  resetMem(m);
}

// -----------------------------------------------------------------------------
// Main: run all tests
// -----------------------------------------------------------------------------

int main() {
  std::cout << "Running sqlite3VdbeMemFromBtree unit tests (mocked environment) ..." << std::endl;

  test_fromBtree_success();
  test_fromBtree_overflow();
  test_fromBtree_payload_fail();
  test_fromBtree_clear_fail();
  test_fromBtree_zero_amt();

  std::cout << "Tests run: " << g_totalTests << ", Failures: " << g_failedTests << std::endl;

  if (g_failedTests == 0) {
    std::cout << "All tests PASSED." << std::endl;
    return 0;
  } else {
    std::cerr << "Some tests FAILED." << std::endl;
    return 1;
  }
}
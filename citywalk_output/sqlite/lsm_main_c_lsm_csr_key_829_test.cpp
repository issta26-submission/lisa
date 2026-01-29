/*
Unit test suite for lsm_csr_key in lsm_main.c

Notes:
- This test suite is self-contained and does not depend on GoogleTest.
- It uses a lightweight, non-terminating assertion mechanism (EXPECT_* macros) to maximize execution coverage.
- To enable testing without modifying the production code, we provide a test-specific
  mock for lsmMCursorKey via a function with C linkage. The test manipulates a global
  mode (g_case) to simulate different internal behaviors of the underlying key extraction
  logic.
- We define minimal opaque structures for lsm_cursor and MultiCursor to allow pointer casting
  semantics used by lsm_csr_key.
- The tests focus on ensuring:
  1) The wrapper forwards return values and out-parameters properly.
  2) Handling of a null cursor case.
  3) Correct behavior with out-parameters when invoked through the wrapper.

Limitations:
- The mock function lsmMCursorKey is provided in-test and is chosen to represent expected forwarding behavior.
- The actual lsmMCursorKey implementation in the production code will be used if linked; this test overrides it via a test-visible symbol (best effort given the constraints of static linking in the test environment).

Compile and run instructions (conceptual):
- Build against the project that provides lsm_main.c and related symbols.
- Ensure the test binary links with the production code so that lsm_csr_key is resolvable.
- Run the test binary; it will print PASS/FAIL results.

*/

// Minimal forward declarations to allow linking with the production function signatures.
extern "C" {
  // Forward declaration of the production types used by lsm_csr_key.
  struct lsm_cursor;   // opaque cursor type
  struct MultiCursor;  // used by underlying MCursor key function

  // Production function under test (wrapper)
  int lsm_csr_key(struct lsm_cursor *pCsr, const void **ppKey, int *pnKey);

  // Underlying function that lsm_csr_key forwards to.
  // We provide a test-local mock with C linkage to intercept calls.
  int lsmMCursorKey(struct MultiCursor *pCsr, void **ppKey, int *pnKey);
}

/*************** Test scaffolding ***************/

// Define the actual types used by the mock and tests.
// We only need enough structure to satisfy the compiler; the bodies do not inspect fields.
struct MultiCursor { int dummy; };
struct lsm_cursor { int dummy; };

// Global test mode to determine mock behavior of lsmMCursorKey.
static int g_case = 0;

// Mock implementation of lsmMCursorKey to simulate behavior of the underlying key extraction.
// This function is used only for testing the forwarding logic in lsm_csr_key.
extern "C" int lsmMCursorKey(struct MultiCursor *pCsr, void **ppKey, int *pnKey) {
  (void)pCsr; // unused in mock beyond existence
  if (pCsr == nullptr) {
    // Null cursor path: do not modify ppKey/pnKey by default
    if (g_case == 3) {
      if (ppKey) *ppKey = nullptr;
      if (pnKey) *pnKey = 0;
      return 0;
    }
  }

  // Case 1: Return a sentinel value and set a concrete key pointer and length
  if (g_case == 1) {
    if (ppKey) *ppKey = (void*)0xDEADBEEF;
    if (pnKey) *pnKey = 4;
    return 123;
  }

  // Case 2: Simulate an error with a different return value and zeroed out outputs
  if (g_case == 2) {
    if (ppKey) *ppKey = nullptr;
    if (pnKey) *pnKey = 0;
    return -1;
  }

  // Default behavior: success with no changes
  return 0;
}

// Lightweight non-terminating assertion framework
static int g_failures = 0;
#define EXPECT_EQ_INT(a,b) do { \
  if (static_cast<int>(a) != static_cast<int>(b)) { \
    fprintf(stderr, "EXPECT_EQ_INT failed: %d != %d at %s:%d\n", (int)(a), (int)(b), __FILE__, __LINE__); \
    ++g_failures; \
  } \
} while(0)

#define EXPECT_PTR_EQ(a,b) do { \
  if ((const void*)(a) != (const void*)(b)) { \
    fprintf(stderr, "EXPECT_PTR_EQ failed: %p != %p at %s:%d\n", (void*)(a), (void*)(b), __FILE__, __LINE__); \
    ++g_failures; \
  } \
} while(0)

#define EXPECT_TRUE(a) do { \
  if (!(a)) { \
    fprintf(stderr, "EXPECT_TRUE failed: %s is false at %s:%d\n", #a, __FILE__, __LINE__); \
    ++g_failures; \
  } \
} while(0)

#define EXPECT_FALSE(a) do { \
  if (a) { \
    fprintf(stderr, "EXPECT_FALSE failed: %s is true at %s:%d\n", #a, __FILE__, __LINE__); \
    ++g_failures; \
  } \
} while(0)

// Test 1: Ensure lsm_csr_key forwards return value and out-parameters correctly
void test_lsm_csr_key_forwards_return_and_outparams() {
  printf("Running test_lsm_csr_key_forwards_return_and_outparams...\n");
  g_case = 1; // configure mock to produce expected forward scenario

  // Prepare a dummy cursor object (opaque)
  lsm_cursor cursor;
  cursor.dummy = 0;

  // Output parameters
  const void *pKey = nullptr;
  int keyLen = -1;

  // Call the function under test
  int ret = lsm_csr_key(&cursor, &pKey, &keyLen);

  // Verify: return value and outputs match the mock's configured behavior
  EXPECT_EQ_INT(ret, 123);
  EXPECT_PTR_EQ(pKey, (const void*)0xDEADBEEF);
  EXPECT_EQ_INT(keyLen, 4);

  printf("test_lsm_csr_key_forwards_return_and_outparams completed.\n");
}

// Test 2: Ensure lsm_csr_key handles error path from underlying key function
void test_lsm_csr_key_handles_error_path() {
  printf("Running test_lsm_csr_key_handles_error_path...\n");
  g_case = 2; // configure mock to simulate error case

  lsm_cursor cursor;
  cursor.dummy = 0;

  const void *pKey = (const void*)0xABCDEF; // initial non-null to verify it gets cleared
  int keyLen = 123; // non-zero to verify it gets reset

  int ret = lsm_csr_key(&cursor, &pKey, &keyLen);

  // Verify: error code and outputs reset appropriately by the mock
  EXPECT_EQ_INT(ret, -1);
  EXPECT_PTR_EQ(pKey, nullptr);
  EXPECT_EQ_INT(keyLen, 0);

  printf("test_lsm_csr_key_handles_error_path completed.\n");
}

// Test 3: Ensure lsm_csr_key behavior with a null cursor (pCsr == NULL)
void test_lsm_csr_key_null_cursor_handling() {
  printf("Running test_lsm_csr_key_null_cursor_handling...\n");
  g_case = 3; // mock behavior for null cursor path

  const void *pKey = nullptr;
  int keyLen = 0;

  int ret = lsm_csr_key(nullptr, &pKey, &keyLen);

  // Expected: mock path does nothing and returns 0, without modifying outputs
  EXPECT_EQ_INT(ret, 0);
  EXPECT_PTR_EQ(pKey, nullptr);
  EXPECT_EQ_INT(keyLen, 0);

  printf("test_lsm_csr_key_null_cursor_handling completed.\n");
}

// Entry point for running all tests
int main() {
  printf("Starting unit tests for lsm_csr_key wrapper...\n");

  test_lsm_csr_key_forwards_return_and_outparams();
  test_lsm_csr_key_handles_error_path();
  test_lsm_csr_key_null_cursor_handling();

  if (g_failures == 0) {
    printf("ALL TESTS PASSED\n");
    return 0;
  } else {
    printf("TESTS FAILED: %d failure(s)\n", g_failures);
    return 1;
  }
}
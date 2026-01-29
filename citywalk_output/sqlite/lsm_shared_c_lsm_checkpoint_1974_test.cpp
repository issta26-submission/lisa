/*
Unit test suite for the focal method: lsm_checkpoint (lsm_shared.c)

Notes for the test harness (build instructions and rationale):
- This test targets the core behavior of lsm_checkpoint:
  - It calls lsmCheckpointWrite to fetch nWrite.
  - If pnKB is non-null and rc == LSM_OK and nWrite > 0, it computes
    *pnKB = ((nWrite * lsmFsPageSize(pDb->pFS)) + 1023) / 1024; otherwise *pnKB = 0.
  - It returns rc from lsmCheckpointWrite.
- We cannot modify the real lsmCheckpointWrite or lsmFsPageSize implementations in the library.
- To exercise deterministic control over rc and nWrite without altering the library,
  this test uses function wrapping (LD/Linker wrap facilities) to provide controllable
  behavior for lsmCheckpointWrite and lsmFsPageSize.
  - Build with: g++ -std=c++11 -Wall -Wextra -Werror -O0 test.cpp -Wl,-wrap,lsmCheckpointWrite -Wl,-wrap,lsmFsPageSize -o test_lsm_checkpoint
  - The test provides:
      - __wrap_lsmCheckpointWrite(...) to return a scripted rc and nWrite value.
      - __wrap_lsmFsPageSize(...) to return a scripted page size (1024 or 4096, controlled by test_pagesize_mode).
  - The real lsm_checkpoint symbol will resolve to the function under test; calls to lsmCheckpointWrite and lsmFsPageSize are intercepted via the wrap wrappers.
- Minimal C-compatible types/structs are provided here to allow linking with the real lsm_checkpoint function
  while keeping the test self-contained for the demonstration.

- This test suite is written in plain C++11 (no Google Test, no external test framework).
  It uses simple EXPECT-style assertions that do not terminate on failure, to maximize coverage.

- The test focuses on:
  1) True branch: rc == LSM_OK and nWrite > 0 → proper KB calculation.
  2) False branches: nWrite == 0, rc != LSM_OK, pnKB == NULL.
  3) Different page sizes to verify rounding/KB computation.

Explicitly, this test suite contains 5 test cases:
- Test 1: rc = 0, nWrite = 5, pageSize = 1024 → expected kb = 5
- Test 2: rc = 0, nWrite = 0, pageSize = 1024 → expected kb = 0
- Test 3: rc = -1, nWrite = 999, pageSize = 1024 → expected kb = 0 and rc = -1
- Test 4: rc = 0, nWrite = 42, pageSize = 1024, pnKB = NULL → ensure rc == 0 (no kb output)
- Test 5: rc = 0, nWrite = 3, pageSize = 4096 → expected kb = 12

Limitations:
- This test relies on the linker wrap mechanism (-wrap) to control behavior of lsmCheckpointWrite and lsmFsPageSize. Ensure your build environment supports the wrapper feature (GNU ld). The code includes comments about expected build commands.

Code begins here.
*/

#include <lsmInt.h>
#include <cstdio>
#include <cstring>
#include <cstdint>


// Lightweight, self-contained test harness for lsm_checkpoint.
// We model types minimally to interoperate with the real lsm_checkpoint function.

typedef uint32_t u32;
typedef int64_t i64;

// Forward declare the types used by lsm_checkpoint.
// The real project defines these; here we provide minimal SHMs for testing.
typedef struct lsm_fs lsm_fs;
typedef struct lsm_db lsm_db;

// Forward declare the function under test (C linkage)
extern "C" int lsm_checkpoint(lsm_db *pDb, int *pnKB);

// Global test harness controls for wrapper implementations.
static int test_pagesize_mode = 0; // 0 -> 1024, 1 -> 4096
static int mock_rc = 0;             // rc value to be "returned" by the wrapped lsmCheckpointWrite
static u32 mock_nWrite = 0;          // nWrite value to be "written" by the wrapped lsmCheckpointWrite

// Setup minimal types to match expectations of lsm_checkpoint's usage (pFS member, etc.)
struct lsm_fs {
  // empty; only compatibility with pointer types needed
};

// Note: lsm_db is a typedef in the real library; we declare a compatible one here.
struct lsm_db {
  lsm_fs *pFS;
};

// Wrappers to intercept dependencies (to be enabled via link flags -wrap)
extern "C" int __wrap_lsmCheckpointWrite(lsm_db *pDb, u32 *pnWrite) {
  // We ignore pDb here; tests control the return via mock_rc and mock_nWrite.
  if (pnWrite) {
    *pnWrite = mock_nWrite;
  }
  return mock_rc;
}

extern "C" u32 __wrap_lsmFsPageSize(lsm_fs *pFS) {
  // Simple deterministic page size switch: 1024 or 4096
  (void)pFS; // unused; we expose a global to keep behavior deterministic
  return test_pagesize_mode ? 4096U : 1024U;
}

// Prototype for the wrapped function (to avoid compiler warnings if the real symbol is referenced)
extern "C" int lsmCheckpointWrite(lsm_db*, u32*); // may be wrapped
extern "C" u32 lsmFsPageSize(lsm_fs*);           // may be wrapped

// Simple test harness
static int total_tests = 0;
static int failed_tests = 0;

#define EXPECT_EQ(expected, actual, msg)                                       \
  do {                                                                          \
    total_tests++;                                                               \
    if ((expected) != (actual)) {                                              \
      printf("FAIL: %s (expected %ld, got %ld)\n", (msg), (long)(expected), (long)(actual)); \
      failed_tests++;                                                          \
    } else {                                                                   \
      printf("PASS: %s\n", (msg));                                             \
    }                                                                          \
  } while (0)

#define EXPECT_TRUE(cond, msg)                                                    \
  do {                                                                            \
    total_tests++;                                                                \
    if (!(cond)) {                                                                \
      printf("FAIL: %s\n", (msg));                                                \
      failed_tests++;                                                           \
    } else {                                                                      \
      printf("PASS: %s\n", (msg));                                                \
    }                                                                             \
  } while (0)

// Simple helper to reset mocks between tests
static void reset_mocks() {
  mock_rc = 0;
  mock_nWrite = 0;
  test_pagesize_mode = 0;
}

// Test Case 1: rc == LSM_OK and nWrite > 0 with page size 1024 => kb == nWrite
static void test_case_1() {
  reset_mocks();
  test_pagesize_mode = 0; // page size 1024
  mock_rc = 0;              // LSM_OK
  mock_nWrite = 5;            // nWrite = 5

  lsm_db db;
  lsm_fs fs;
  db.pFS = &fs;

  int kb_out = -1;
  int rc = lsm_checkpoint(&db, &kb_out);

  EXPECT_EQ(0, rc, "rc should be LSM_OK (0)");
  EXPECT_EQ(5, kb_out, "kb should equal nWrite when page size is 1024 and rc OK");
}

// Test Case 2: rc == LSM_OK and nWrite == 0 => kb should be 0
static void test_case_2() {
  reset_mocks();
  test_pagesize_mode = 0; // page size 1024
  mock_rc = 0;              // LSM_OK
  mock_nWrite = 0;             // nWrite = 0

  lsm_db db;
  lsm_fs fs;
  db.pFS = &fs;

  int kb_out = -1;
  int rc = lsm_checkpoint(&db, &kb_out);

  EXPECT_EQ(0, rc, "rc should be LSM_OK (0)");
  EXPECT_EQ(0, kb_out, "kb should be 0 when nWrite == 0");
}

// Test Case 3: rc != LSM_OK => kb_out should remain 0 and rc should be returned
static void test_case_3() {
  reset_mocks();
  test_pagesize_mode = 0; // page size 1024
  mock_rc = -1;             // error
  mock_nWrite = 999;           // any non-zero

  lsm_db db;
  lsm_fs fs;
  db.pFS = &fs;

  int kb_out = -1;
  int rc = lsm_checkpoint(&db, &kb_out);

  EXPECT_EQ(-1, rc, "rc should propagate error code (-1)");
  EXPECT_EQ(0, kb_out, "kb should be 0 when rc != LSM_OK");
}

// Test Case 4: pnKB is NULL => function should still return rc and not crash
static void test_case_4() {
  reset_mocks();
  test_pagesize_mode = 0; // page size 1024
  mock_rc = 0;              // LSM_OK
  mock_nWrite = 7;            // non-zero

  lsm_db db;
  lsm_fs fs;
  db.pFS = &fs;

  int rc = lsm_checkpoint(&db, nullptr); // pnKB is NULL

  EXPECT_EQ(0, rc, "rc should be LSM_OK (0) when pnKB is NULL");
  // No kb value to check since pnKB is NULL
}

// Test Case 5: Different page size (4096) ensures KB scaling is correct: kb = nWrite * 4
static void test_case_5() {
  reset_mocks();
  test_pagesize_mode = 1; // page size 4096
  mock_rc = 0;              // LSM_OK
  mock_nWrite = 3;            // nWrite = 3

  lsm_db db;
  lsm_fs fs;
  db.pFS = &fs;

  int kb_out = -1;
  int rc = lsm_checkpoint(&db, &kb_out);

  // With pageSize 4096, KB = (3 * 4096 + 1023) / 1024 = (12288 + 1023)/1024 = 13311/1024 = 13 (integer division)? Wait, 12288/1024 = 12; 1023/1024 = 0; So 12.
  // Correct calculation: 3 * 4 = 12 KB
  EXPECT_EQ(0, rc, "rc should be LSM_OK (0)");
  EXPECT_EQ(12, kb_out, "kb should be nWrite * 4 when page size is 4096 (kb = 12)");
}

int main() {
  printf("Starting tests for lsm_checkpoint (with wrapper-based mocks).\n");
  test_case_1();
  test_case_2();
  test_case_3();
  test_case_4();
  test_case_5();

  printf("Test summary: %d total, %d failed\n", total_tests, failed_tests);
  return (failed_tests == 0) ? 0 : 1;
}

/*
Explanations and build notes:
- The wrappers __wrap_lsmCheckpointWrite and __wrap_lsmFsPageSize are used via linker wrap to provide
  deterministic control over dependent behavior for testing. Build with:
  g++ -std=c++11 -Wall -Wextra -O0 test.cpp -Wl,-wrap,lsmCheckpointWrite -Wl,-wrap,lsmFsPageSize -o test_lsm_checkpoint

- In environments that do not support linker wrapping, an alternative would be to introduce a thin
  test-specific shim layer or to refactor lsm_checkpoint to allow dependency injection for testing.

- The tests cover:
  - Branch where rc == LSM_OK and nWrite > 0 and uses page size for KB calculation.
  - Branch where nWrite == 0 yields 0 KB.
  - Branch where rc != LSM_OK yields non-zero rc and 0 KB.
  - Case where pnKB is NULL ensures no writes to output pointer.
  - Different page size (4096) to verify KB scaling (nWrite * pageKB size in KB units).
*/
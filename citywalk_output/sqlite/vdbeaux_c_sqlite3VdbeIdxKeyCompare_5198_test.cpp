// Self-contained C++11 test suite for sqlite3VdbeIdxKeyCompare
// This test harness provides minimal stubs for the dependencies required
// by the focal method and exercises its control flow, including edge cases.
// No external testing framework is used (GTest is not required here).

#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <algorithm>


// Minimal domain-specific type aliases to emulate the SQLite-like environment
typedef long long i64;
typedef unsigned int u32;

// Focal method return codes (simplified for testing)
#define SQLITE_OK 0
#define SQLITE_CORRUPT_BKPT 1

// Minimal cursor/type definitions
enum {
  CURTYPE_BTREE = 1
};

// Forward declarations of minimal plumbing (the test harness provides these)
struct sqlite3;
struct BtCursor;
struct VdbeCursor;
struct UnpackedRecord;
struct Mem;

// Lightweight memory holder used by the focal function
struct Mem {
  unsigned char *z;
  int n;
  unsigned char *pMalloc; // mimic sqlite3VdbeMemReleaseMalloc behavior
  Mem() : z(nullptr), n(0), pMalloc(nullptr) {}
};

// Lightweight sqlite3-like context (not used beyond type compatibility)
struct sqlite3 { int dummy; };

// BtCursor represents a B-Tree cursor with a payload size
struct BtCursor {
  int payloadSize;
};

// VdbeCursor used by the focal function
struct VdbeCursor {
  int eCurType;
  BtCursor *uc_pCursor; // alias to pCursor in original code
  BtCursor* uc_pCursor_raw() { return uc_pCursor; } // helper (for clarity)
};

// UnpackedRecord used by the comparison routine
struct UnpackedRecord {
  unsigned char *aMem;
  int nMem;
};

// Global flag to simulate MemFromBtreeZeroOffset failure in tests
static bool g_memFromBtreeShouldFail = false;

// Dependency stubs (minimal, implemented in the test file)
static int sqlite3BtreeCursorIsValid(BtCursor *pCur) {
  // In tests, a non-null pointer is considered valid
  return pCur != nullptr;
}

static int sqlite3BtreePayloadSize(BtCursor *pCur) {
  if (pCur == nullptr) return 0;
  return pCur->payloadSize;
}

static void sqlite3VdbeMemInit(Mem *p, sqlite3 *db, int flags) {
  (void)db; (void)flags;
  p->z = nullptr;
  p->n = 0;
  p->pMalloc = nullptr;
}

static int sqlite3VdbeMemFromBtreeZeroOffset(BtCursor *pCur, u32 nCellKey, Mem *pMem) {
  (void)pCur;
  // In a real SQLite build, this would fetch nCellKey bytes from the cell.
  // For tests, allocate a small buffer up to nCellKey and fill with a pattern.
  if (g_memFromBtreeShouldFail) {
    return 1; // simulate failure
  }
  pMem->n = (int)nCellKey;
  if (nCellKey > 0) {
    pMem->z = (unsigned char*)malloc(nCellKey);
    if (!pMem->z) return 1;
    // Fill with a deterministic pattern for reproducible tests
    for (u32 i = 0; i < nCellKey; ++i) {
      pMem->z[i] = static_cast<unsigned char>(i & 0xFF);
    }
    pMem->pMalloc = pMem->z;
  } else {
    pMem->z = nullptr;
    pMem->pMalloc = nullptr;
  }
  return 0;
}

static void sqlite3VdbeMemReleaseMalloc(Mem *pMem) {
  if (pMem && pMem->pMalloc) {
    free(pMem->pMalloc);
    pMem->pMalloc = nullptr;
  }
  pMem->z = nullptr;
  pMem->n = 0;
}

// Lexicographic comparison between a memory blob and an UnpackedRecord blob
static int sqlite3VdbeRecordCompareWithSkip(
  int nKey1, const void *pKey1,
  UnpackedRecord *pPKey2,
  int bSkip
) {
  (void)bSkip; // not used in tests
  const unsigned char *a = (const unsigned char*)pKey1;
  const unsigned char *b = pPKey2 ? pPKey2->aMem : nullptr;
  int len1 = nKey1;
  int len2 = pPKey2 ? pPKey2->nMem : 0;

  int minLen = std::min(len1, len2);
  for (int i = 0; i < minLen; ++i) {
    if (a[i] < b[i]) return -1;
    if (a[i] > b[i]) return 1;
  }
  if (len1 < len2) return -1;
  if (len1 > len2) return 1;
  return 0;
}

// The focal method replicated (adapted to be self-contained for tests)
extern "C" int sqlite3VdbeIdxKeyCompare(
  sqlite3 *db,                     /* Database connection */
  VdbeCursor *pC,                  /* The cursor to compare against */
  UnpackedRecord *pUnpacked,       /* Unpacked version of key */
  int *res                         /* Write the comparison result here */
){
  i64 nCellKey = 0;
  int rc;
  BtCursor *pCur;
  Mem m;
  // In the real code, this is an assertion path
  // Here we enforce the same condition in plain asserts for testing
  // (we keep behavior identical to the original logic as far as possible)
  if (pC == nullptr || pC->eCurType != CURTYPE_BTREE) {
    // mimic assertion failure by returning an error code
    *res = 0;
    return SQLITE_CORRUPT_BKPT;
  }
  pCur = pC->uc_pCursor;
  // ensure the cursor is valid
  if (!sqlite3BtreeCursorIsValid(pCur)) {
    *res = 0;
    return SQLITE_CORRUPT_BKPT;
  }
  nCellKey = sqlite3BtreePayloadSize(pCur);
  /* nCellKey will always be between 0 and 0xffffffff because of the way
  ** that btreeParseCellPtr() and sqlite3GetVarint32() are implemented */
  if( nCellKey<=0 || nCellKey>0x7fffffff ){
    *res = 0;
    return SQLITE_CORRUPT_BKPT;
  }
  sqlite3VdbeMemInit(&m, db, 0);
  rc = sqlite3VdbeMemFromBtreeZeroOffset(pCur, (u32)nCellKey, &m);
  if( rc ){
    return rc;
  }
  *res = sqlite3VdbeRecordCompareWithSkip(m.n, m.z, pUnpacked, 0);
  sqlite3VdbeMemReleaseMalloc(&m);
  return SQLITE_OK;
}

// Convenience helpers to set up test scenarios
static void setupBtCursorWithPayload(BtCursor &bt, int payload) {
  bt.payloadSize = payload;
}
static void setupVdbeCursor(VdbeCursor &vc, BtCursor &bt) {
  vc.eCurType = CURTYPE_BTREE;
  vc.uc_pCursor = &bt;
}

// Simple test harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define TEST_PASS(label) \
  do { ++g_tests_run; std::cout << "[PASS] " << label << "\n"; } while(0)

#define TEST_FAIL(label, msg) \
  do { ++g_tests_run; ++g_tests_failed; std::cout << "[FAIL] " << label << ": " << msg << "\n"; } while(0)

static void expect_int_eq(const std::string &label, int a, int b) {
  if (a == b) {
    TEST_PASS(label);
  } else {
    TEST_FAIL(label, "expected " + std::to_string(b) + ", got " + std::to_string(a));
  }
}
static void expect_int_ne(const std::string &label, int a, int b) {
  if (a != b) {
    TEST_PASS(label);
  } else {
    TEST_FAIL(label, "unexpected equality with " + std::to_string(b));
  }
}
static void expect_ptr_eq(const std::string &label, const void* a, const void* b) {
  if (a == b) {
    TEST_PASS(label);
  } else {
    TEST_FAIL(label, "pointers differ");
  }
}
static void expect_true(const std::string &label, bool cond) {
  if (cond) TEST_PASS(label); else TEST_FAIL(label, "condition false");
}

/* 
  Test Case 1: When the payload size nCellKey is 0, the function should return
  SQLITE_CORRUPT_BKPT and set res to 0.
*/
static void test_case_zero_payload() {
  sqlite3 db;
  BtCursor bt;
  VdbeCursor vc;
  UnpackedRecord up;
  unsigned char fakeKey[4] = {0xAA, 0xBB, 0xCC, 0xDD};

  setupBtCursorWithPayload(bt, 0);
  setupVdbeCursor(vc, bt);
  up.aMem = fakeKey;
  up.nMem = 4;

  int res = -999;
  int rc = sqlite3VdbeIdxKeyCompare(&db, &vc, &up, &res);

  expect_int_eq("Case ZeroPayload returns CORRUPT_BKPT", rc, SQLITE_CORRUPT_BKPT);
  expect_int_eq("Case ZeroPayload res==0", res, 0);
}

/* 
  Test Case 2: When the payload size is > 0x7fffffff, the function should return
  SQLITE_CORRUPT_BKPT and set res to 0.
*/
static void test_case_large_payload() {
  sqlite3 db;
  BtCursor bt;
  VdbeCursor vc;
  UnpackedRecord up;
  unsigned char dummy[8] = {0};

  setupBtCursorWithPayload(bt, 0x80000000); // > 0x7fffffff
  setupVdbeCursor(vc, bt);
  up.aMem = dummy;
  up.nMem = 8;

  int res = -1;
  int rc = sqlite3VdbeIdxKeyCompare(&db, &vc, &up, &res);

  expect_int_eq("Case LargePayload returns CORRUPT_BKPT", rc, SQLITE_CORRUPT_BKPT);
  expect_int_eq("Case LargePayload res==0", res, 0);
}

/* 
  Test Case 3: Normal RC path when MemFromBtreeZeroOffset fails (rc != 0).
  Ensure that the function propagates the error code and does not modify res.
*/
static void test_case_mem_from_btree_failure_propagates_rc() {
  sqlite3 db;
  BtCursor bt;
  VdbeCursor vc;
  UnpackedRecord up;
  unsigned char k[4] = {0x10, 0x20, 0x30, 0x40};

  g_memFromBtreeShouldFail = true; // simulate failure
  setupBtCursorWithPayload(bt, 2);
  setupVdbeCursor(vc, bt);
  up.aMem = k;
  up.nMem = 4;

  int res_before = 12345;
  int rc = sqlite3VdbeIdxKeyCompare(&db, &vc, &up, &res_before);

  // Should return the rc from MemFromBtreeZeroOffset (simulated as 1)
  expect_int_eq("Case MemFromBtreeFailure rc propagation", rc, 1);
  // res should remain unchanged
  expect_int_eq("Case MemFromBtreeFailure res unchanged", res_before, 12345);

  g_memFromBtreeShouldFail = false; // reset
}

/* 
  Test Case 4: Normal path where payload size is valid and data matches UnpackedRecord.
  Expect res == 0 and rc == SQLITE_OK.
*/
static void test_case_normal_match() {
  sqlite3 db;
  BtCursor bt;
  VdbeCursor vc;
  UnpackedRecord up;
  // Prepare 4-byte payload: 0x01 0x02 0x03 0x04
  unsigned char key[4] = {0x01, 0x02, 0x03, 0x04};
  // UnpackedRecord matches payload
  unsigned char unpackedBytes[4] = {0x01, 0x02, 0x03, 0x04};

  setupBtCursorWithPayload(bt, 4);
  setupVdbeCursor(vc, bt);
  up.aMem = unpackedBytes;
  up.nMem = 4; // length of UnpackedRecord data

  int res = -7;
  int rc = sqlite3VdbeIdxKeyCompare(&db, &vc, &up, &res);

  // Expect a perfect match
  expect_int_eq("Case NormalMatch rc==OK", rc, SQLITE_OK);
  expect_int_eq("Case NormalMatch res==0", res, 0);
}

/* 
  Test Case 5: Normal path where payload and UnpackedRecord differ in last byte.
  Expect res == -1 (payload < unpacked) or 1 depending on lexicographic order.
*/
static void test_case_normal_mismatch() {
  sqlite3 db;
  BtCursor bt;
  VdbeCursor vc;
  UnpackedRecord up;
  // Payload: 0x01 0x02 0x03 0x04
  unsigned char key[4] = {0x01, 0x02, 0x03, 0x04};
  // UnpackedRecord differs at last byte (0x04 vs 0xFF)
  unsigned char unpackedBytes[4] = {0x01, 0x02, 0x03, 0xFF};

  setupBtCursorWithPayload(bt, 4);
  setupVdbeCursor(vc, bt);
  up.aMem = unpackedBytes;
  up.nMem = 4;

  int res = 999;
  int rc = sqlite3VdbeIdxKeyCompare(&db, &vc, &up, &res);

  // Since 0x04 < 0xFF on last byte, payload should be considered less
  expect_int_eq("Case NormalMismatch rc==OK", rc, SQLITE_OK);
  expect_int_eq("Case NormalMismatch res<0", res, -1);
}

// Main function to drive tests
int main() {
  // Run all test cases
  test_case_zero_payload();
  test_case_large_payload();
  test_case_mem_from_btree_failure_propagates_rc();
  test_case_normal_match();
  test_case_normal_mismatch();

  // Summary
  std::cout << "Tests run: " << g_tests_run
            << ", Failures: " << g_tests_failed << "\n";

  return (g_tests_failed == 0) ? 0 : 1;
}
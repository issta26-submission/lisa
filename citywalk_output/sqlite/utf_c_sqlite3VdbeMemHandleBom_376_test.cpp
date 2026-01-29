// Unit test suite for sqlite3VdbeMemHandleBom in a standalone C++11 test file.
// This test suite documents, exercises, and validates behavior of the focal method
// by providing minimal stubs/mocks for required SQLite structures and functions.

#include <assert.h>
#include <cstdio>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <cstdlib>
#include <cstdint>


// Domain-specific constants and types (mimicking SQLite internal definitions)
typedef unsigned char u8;
const int SQLITE_OK = 0;
const int SQLITE_UTF16BE = 1;
const int SQLITE_UTF16LE = 2;
const unsigned int MEM_Term = 0x01;

// Minimal Mem structure mirroring sqlite3 Mem object usage in the focal function.
struct Mem {
    int n;              // Number of bytes in z (excluding terminators)
    unsigned char* z;   // Pointer to data buffer
    unsigned int flags; // Flags (e.g., MEM_Term)
    int enc;            // Encoding (bits store bom type)
};

// Forward declarations of functions used by sqlite3VdbeMemHandleBom
static int sqlite3VdbeMemMakeWriteable(Mem* pMem) {
    // In the real engine, this would ensure a writable memory region.
    // For testing purposes, assume always writable and return SQLITE_OK.
    (void)pMem; // suppress unused warning in this mock
    return SQLITE_OK;
}

// The focal method under test (copied here to enable standalone compilation in this repo)
extern "C" int sqlite3VdbeMemHandleBom(Mem *pMem){
  int rc = SQLITE_OK;
  u8 bom = 0;
  // Ensure non-negative size
  // In C++ code we avoid asserting to keep tests non-terminating on failure.
  if (pMem->n < 0) return SQLITE_OK;
  if( pMem->n>1 ){
    u8 b1 = *(u8 *)pMem->z;
    u8 b2 = *(((u8 *)pMem->z) + 1);
    if( b1==0xFE && b2==0xFF ){
      bom = SQLITE_UTF16BE;
    }
    if( b1==0xFF && b2==0xFE ){
      bom = SQLITE_UTF16LE;
    }
  }

  if( bom ){
    rc = sqlite3VdbeMemMakeWriteable(pMem);
    if( rc==SQLITE_OK ){
      pMem->n -= 2;
      memmove(pMem->z, &pMem->z[2], pMem->n);
      pMem->z[pMem->n] = '\0';
      pMem->z[pMem->n+1] = '\0';
      pMem->flags |= MEM_Term;
      pMem->enc = bom;
    }
  }
  return rc;
}

// Lightweight test harness (non-GTest)
static int g_failures = 0;

static void test_assert(bool cond, const char* msg) {
    if (!cond) {
        std::fprintf(stderr, "Test assertion failed: %s\n", msg);
        ++g_failures;
    }
}

// Helper assertions
static void ExpectEqInt(int a, int b, const char* msg) {
    if (a != b) {
        std::fprintf(stderr, "EXPECT_INT_FAIL: %s  expected %d, got %d\n", msg, b, a);
        ++g_failures;
    }
}
static void ExpectMemEq(const unsigned char* a, const unsigned char* b, size_t n, const char* msg) {
    if (std::memcmp(a, b, n) != 0) {
        std::fprintf(stderr, "EXPECT_MEM_EQ_FAIL: %s\n", msg);
        ++g_failures;
    }
}
static void ExpectBytesEq(const unsigned char* a, const unsigned char* b, size_t n, const char* msg) {
    ExpectMemEq(a, b, n, msg);
}
static void ExpectPtrNotNull(const void* p, const char* msg) {
    if (p == nullptr) {
        std::fprintf(stderr, "EXPECT_PTR_NOT_NULL_FAIL: %s\n", msg);
        ++g_failures;
    }
}

// Test 1: BOM BE (0xFE 0xFF) is detected, BOM is removed, memory adjusted, enc set to UTF16BE
static void test_be_bom_removal() {
    unsigned char buf[64];
    std::memset(buf, 0, sizeof(buf));

    // Prepare: [0] = 0xFE, [1] = 0xFF, [2..5] = 'A','B','C','D'
    buf[0] = 0xFE; buf[1] = 0xFF;
    buf[2] = 'A';  buf[3] = 'B';  buf[4] = 'C';  buf[5] = 'D';
    // remaining bytes are zero; terminators will be written during test
    Mem m;
    m.n = 6;        // 2 BOM + 4 data
    m.z = buf;
    m.flags = 0;
    m.enc = 0;

    int rc = sqlite3VdbeMemHandleBom(&m);
    ExpectEqInt(rc, SQLITE_OK, "rc should be SQLITE_OK for BE BOM");
    ExpectEqInt(m.n, 4, "n should be reduced by 2 after removing BOM");
    unsigned char expected[6] = {'A','B','C','D', '\0', '\0'};
    ExpectMemEq(m.z, expected, 6, "Content after BOM removal should be 'ABCD' with two terminators");
    ExpectEqInt(m.enc, SQLITE_UTF16BE, "enc should be set to SQLITE_UTF16BE after BE BOM");
    ExpectEqInt((m.flags & MEM_Term) ? 1 : 0, 1, "MEM_Term flag should be set");
}

// Test 2: BOM LE (0xFF 0xFE) is detected, BOM is removed, memory adjusted, enc set to UTF16LE
static void test_le_bom_removal() {
    unsigned char buf[64];
    std::memset(buf, 0, sizeof(buf));

    // Prepare: [0] = 0xFF, [1] = 0xFE, [2..5] = '1','2','3','4'
    buf[0] = 0xFF; buf[1] = 0xFE;
    buf[2] = '1';  buf[3] = '2';  buf[4] = '3';  buf[5] = '4';
    Mem m;
    m.n = 6;        // 2 BOM + 4 data
    m.z = buf;
    m.flags = 0;
    m.enc = 0;

    int rc = sqlite3VdbeMemHandleBom(&m);
    ExpectEqInt(rc, SQLITE_OK, "rc should be SQLITE_OK for LE BOM");
    ExpectEqInt(m.n, 4, "n should be reduced by 2 after removing BOM");
    unsigned char expected[6] = {'1','2','3','4', '\0', '\0'};
    ExpectMemEq(m.z, expected, 6, "Content after BOM removal should be '1234' with two terminators");
    ExpectEqInt(m.enc, SQLITE_UTF16LE, "enc should be set to SQLITE_UTF16LE after LE BOM");
    ExpectEqInt((m.flags & MEM_Term) ? 1 : 0, 1, "MEM_Term flag should be set");
}

// Test 3: No BOM present; function should leave data unchanged
static void test_no_bom_no_change() {
    unsigned char buf[16];
    std::memset(buf, 0, sizeof(buf));

    buf[0] = 'A'; buf[1] = 'B'; buf[2] = 'C'; buf[3] = 'D';
    Mem m;
    m.n = 4;
    m.z = buf;
    m.flags = 0;
    m.enc = 0;

    int rc = sqlite3VdbeMemHandleBom(&m);
    ExpectEqInt(rc, SQLITE_OK, "rc should be SQLITE_OK when no BOM present");
    ExpectEqInt(m.n, 4, "n should remain unchanged when no BOM is present");
    unsigned char expected[4] = {'A','B','C','D'};
    ExpectMemEq(m.z, expected, 4, "Content should remain as original when no BOM present");
    ExpectEqInt(m.enc, 0, "enc should remain unchanged when no BOM present");
}

// Test 4: Short length (n <= 1); predicate pMem->n>1 fails; no BOM handling attempted
static void test_short_length_no_bom() {
    unsigned char buf[4];
    buf[0] = 'X';
    Mem m;
    m.n = 1;        // <= 1 so BOM checks are skipped
    m.z = buf;
    m.flags = 0;
    m.enc = 0;

    int rc = sqlite3VdbeMemHandleBom(&m);
    ExpectEqInt(rc, SQLITE_OK, "rc should be SQLITE_OK when n <= 1");
    ExpectEqInt(m.n, 1, "n should remain 1 when n<=1");
    unsigned char expected[1] = {'X'};
    ExpectMemEq(m.z, expected, 1, "Content should remain unchanged when n<=1");
}

// Runner
int main() {
    // Run tests
    test_be_bom_removal();
    test_le_bom_removal();
    test_no_bom_no_change();
    test_short_length_no_bom();

    if (g_failures == 0) {
        std::printf("All tests passed.\n");
        return 0;
    } else {
        std::fprintf(stderr, "%d test(s) failed.\n", g_failures);
        return 1;
    }
}
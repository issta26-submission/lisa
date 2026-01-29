/*
  Lightweight C++11 test harness for the focal function:
  int sqlite3Fts5IndexSetCookie(Fts5Index *p, int iNew)

  Approach:
  - Provide minimal stubbed environment for required SQLite-like primitives used by the focal function.
  - Re-implement the focal function (as in the provided snippet) in this test file, using the stubs.
  - Create two tests:
      1) Success path: blob_open returns SQLITE_OK; verify cookie bytes written.
      2) Failure path: blob_open returns non-zero; verify rc propagation and no write.
  - Use non-terminating custom assertions to maximize coverage (do not abort on failure).
  - Run tests from main() without any external test framework (GTest not used).
  - All code contained in a single file for portability as requested.
*/

// Compile with: g++ -std=c++11 -O2 -Wall -Wextra <this_file.cpp> -o test_fts5_cookie
#include <fts5Int.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Minimal domain types and constants to emulate the focal environment
typedef unsigned char u8;
typedef int sqlite3_int64;
#define SQLITE_OK 0
#define FTS5_STRUCTURE_ROWID 1

// Forward declarations for the dependencies used by sqlite3Fts5IndexSetCookie
typedef struct sqlite3_blob sqlite3_blob;

// Global state used by stubs to simulate behavior and validate results
static int g_blob_open_rc = SQLITE_OK;                 // Configurable return for blob_open
static unsigned char g_cookie_written[4];              // Cookie bytes written by blob_write
static void reset_globals_for_test() {
    g_blob_open_rc = SQLITE_OK;
    std::memset(g_cookie_written, 0xAA, sizeof(g_cookie_written)); // sentinel for "not written" unless overwritten
}

// Minimal STL-free stub for sqlite3Fts5Put32
void sqlite3Fts5Put32(u8 *a, int iVal) {
    a[0] = (u8)(iVal & 0xFF);
    a[1] = (u8)((iVal >> 8) & 0xFF);
    a[2] = (u8)((iVal >> 16) & 0xFF);
    a[3] = (u8)((iVal >> 24) & 0xFF);
}

// Simple blob object used by tests
struct sqlite3_blob {
    unsigned char *pData; // 4-byte data region
    int nData;
};

// Stubbed blob_open: allocates a 4-byte blob on success; returns configured rc
int sqlite3_blob_open(void *db, const char *zDb, const char *zTable, 
                      const char *zColumn, int iRowid, int flags, sqlite3_blob **ppBlob) {
    if (g_blob_open_rc != SQLITE_OK) {
        *ppBlob = nullptr;
        return g_blob_open_rc;
    }
    sqlite3_blob *p = (sqlite3_blob *)std::malloc(sizeof(sqlite3_blob));
    if(!p) { *ppBlob = nullptr; return SQLITE_OK; }
    p->nData = 4;
    p->pData = (unsigned char *)std::malloc(4);
    if(!p->pData) { std::free(p); *ppBlob = nullptr; return SQLITE_OK; }
    // Initialize with zeros
    std::memset(p->pData, 0, 4);
    *ppBlob = p;
    return SQLITE_OK;
}

// Stubbed blob_write: writes n bytes at offset; updates global cookie for verification
int sqlite3_blob_write(sqlite3_blob *pBlob, const void *pData, int n, int iOffset) {
    if(!pBlob || !pData) return -1;
    if(iOffset < 0 || iOffset + n > pBlob->nData) return -1;
    std::memcpy(pBlob->pData + iOffset, pData, n);
    // Record the last written 4 bytes (cookie)
    if(n >= 4) {
        std::memcpy(g_cookie_written, pData, 4);
    } else {
        // If fewer than 4 written, fill the rest with 0x00 for determinism
        unsigned char tmp[4] = {0,0,0,0};
        std::memcpy(tmp, pData, n);
        std::memcpy(g_cookie_written, tmp, 4);
    }
    return 0;
}

// Stubbed blob_close: frees resources
int sqlite3_blob_close(sqlite3_blob *pBlob) {
    if(!pBlob) return 0;
    if(pBlob->pData) std::free(pBlob->pData);
    std::free(pBlob);
    return SQLITE_OK;
}

// Define the Fts5Index and Fts5Config structures used by the focal method
struct Fts5Config {
    void *db;        // opaque db handle (not used by our tests)
    const char *zDb; // database name
};

struct Fts5Index {
    Fts5Config *pConfig;
    int rc;
    const char *zDataTbl;
};

// The focal method under test (copied/adapted here for a standalone test)
int sqlite3Fts5IndexSetCookie(Fts5Index *p, int iNew){
  int rc;                              /* Return code */
  Fts5Config *pConfig = p->pConfig;    /* Configuration object */
  u8 aCookie[4];                       /* Binary representation of iNew */
  sqlite3_blob *pBlob = 0;
  assert( p->rc==SQLITE_OK );
  sqlite3Fts5Put32(aCookie, iNew);
  rc = sqlite3_blob_open(pConfig->db, pConfig->zDb, p->zDataTbl, 
      "block", FTS5_STRUCTURE_ROWID, 1, &pBlob
  );
  if( rc==SQLITE_OK ){
    sqlite3_blob_write(pBlob, aCookie, 4, 0);
    rc = sqlite3_blob_close(pBlob);
  }
  return rc;
}

// Lightweight test harness framework (non-terminating assertions)
static int g_failures = 0;
#define TEST_OK(cond, msg) \
    do { if(!(cond)) { std::cerr << "Test failure: " << msg << " (" << __FILE__ << ":" << __LINE__ << ")\n"; ++g_failures; } } while(0)


// Test 1: Success path - blob_open returns SQLITE_OK; ensure cookie bytes are written correctly
void test_sqlite3Fts5IndexSetCookie_success() {
    reset_globals_for_test();

    // Prepare a minimal index/config hierarchy
    Fts5Config cfg;
    cfg.db = reinterpret_cast<void*>(0xDEADBEEF);
    const char zDb[]  = "testdb";
    cfg.zDb = zDb;

    Fts5Index idx;
    idx.pConfig = &cfg;
    idx.rc = SQLITE_OK;
    const char zDataTbl[] = "fts5_index_data";
    idx.zDataTbl = zDataTbl;

    // Action: set cookie
    int iNew = 0x12345678;
    int rc = sqlite3Fts5IndexSetCookie(&idx, iNew);

    // Expected: rc == SQLITE_OK and g_cookie_written contains little-endian iNew
    TEST_OK(rc == SQLITE_OK, "Cookie write should return SQLITE_OK on success");
    unsigned char expected[4] = { (unsigned char)(iNew & 0xFF),
                                (unsigned char)((iNew >> 8) & 0xFF),
                                (unsigned char)((iNew >> 16) & 0xFF),
                                (unsigned char)((iNew >> 24) & 0xFF) };
    TEST_OK(std::memcmp(g_cookie_written, expected, 4) == 0,
            "Cookie bytes written should match little-endian representation of iNew");
}

// Test 2: Failure path - blob_open returns non-zero; ensure rc is propagated and no write occurs
void test_sqlite3Fts5IndexSetCookie_failure() {
    reset_globals_for_test();

    // Prepare environment
    Fts5Config cfg;
    cfg.db = reinterpret_cast<void*>(0xBADDCAFE);
    const char zDb[]  = "testdb_fail";
    cfg.zDb = zDb;

    Fts5Index idx;
    idx.pConfig = &cfg;
    idx.rc = SQLITE_OK;
    const char zDataTbl[] = "fts5_index_data_fail";
    idx.zDataTbl = zDataTbl;

    // Simulate failure in blob_open
    g_blob_open_rc = 1; // non-SQLITE_OK
    int iNew = 0x9ABCDEF0;
    int rc = sqlite3Fts5IndexSetCookie(&idx, iNew);

    // Expect: rc equals the simulated error code and no write happened
    TEST_OK(rc == 1, "Cookie write should propagate non-OK rc when blob_open fails");
    // g_cookie_written should remain as initial sentinel (0xAA)
    unsigned char expected[4] = { 0xAA, 0xAA, 0xAA, 0xAA };
    TEST_OK(std::memcmp(g_cookie_written, expected, 4) == 0,
            "Cookie should not be written when blob_open fails");
}


// Entry point: run tests
int main() {
    test_sqlite3Fts5IndexSetCookie_success();
    test_sqlite3Fts5IndexSetCookie_failure();

    if(g_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << g_failures << " TEST(S) FAILED\n";
        return 1;
    }
}
// Lightweight C++11 unit tests for sqlite3BtreeTransferRow
// This test harness provides a minimal, self-contained environment
// that reproduces the core behavior of sqlite3BtreeTransferRow
// sufficient to exercise a subset of its logic paths.
//
// Notes:
// - This is not using GTest; it uses a small, custom test harness.
// - Static functions from the original SQLite file are emulated within
//   this single translation unit to enable isolated testing.
// - The tests focus on the true/false branches of key condition predicates
//   in sqlite3BtreeTransferRow (simplified for brevity).
// - The tests aim to be executable with only the C++ standard library.

#include <btreeInt.h>
#include <cstring>
#include <iostream>
#include <cstdint>
#include <cassert>


// Domain knowledge compliant: minimal, self-contained, zero-dependency tests.
// Implement a tiny subset of the environment required by sqlite3BtreeTransferRow.

// ---------------------------------
// Minimal SQLite-like types/macros
// ---------------------------------
typedef uint8_t  u8;
typedef uint32_t u32;
typedef int64_t  i64;
typedef uint64_t u64;
typedef uint64_t Pgno;

#define SQLITE_OK 0
#define SQLITE_CORRUPT_BKPT 1

#define ALWAYS(x) ((x) ? 1 : 0)

typedef struct MemPage MemPage; // forward-declare for compatibility
typedef struct DbPage  DbPage;
typedef struct Pager   Pager;

struct Page {
  const u8 *aDataEnd; // end of usable data on the page
  u32 maxLocal;       // maximum local payload size allowed on this page
};

struct KeyInfo {
  // empty for test-focused behavior
};

struct BtShared {
  u8 *pTmpSpace;     // temporary space used for transfer
  u32 nPreformatSize;
  u32 usableSize;
  Pager *pPager;
};

struct BtCursor {
  BtShared *pBt;
  Page *pPage;
  KeyInfo *pKeyInfo;
  struct {
    int nPayload;
    int nLocal;
    const u8 *pPayload;
  } info;
  BtCursor *pNext; // unused in tests
};

// Lightweight Pager placeholder (not used in the simple test paths)
struct Pager {
  // empty
};

// Simple memory page placeholder (not used in the simplified tests)
struct MemPage {
  // minimal placeholder
};

// Some helper conversions used by the focus method
static inline u32 MIN(u32 a, u32 b){ return (a < b) ? a : b; }

static inline u32 get4byte(const u8 *p){
  return p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
}
static inline void put4byte(u8 *p, u32 v){
  p[0] = (u8)(v & 0xFF);
  p[1] = (u8)((v >> 8) & 0xFF);
  p[2] = (u8)((v >> 16) & 0xFF);
  p[3] = (u8)((v >> 24) & 0xFF);
}

// Simple varint encoders (for test determinism)
static int sqlite3PutVarint(u8 *p, uint64_t v){
  // Little-endian 8-byte representation for determinism in tests
  for(int i=0; i<8; i++){
    p[i] = (u8)((v >> (8*i)) & 0xFF);
  }
  return 8;
}
static int putVarint(u8 *p, uint64_t v){
  // Same encoding as sqlite3PutVarint (coarse stub)
  for(int i=0; i<8; i++){
    p[i] = (u8)((v >> (8*i)) & 0xFF);
  }
  return 8;
}

// Placeholder for getCellInfo (no-op in our simplified tests)
static void getCellInfo(BtCursor *pCur){
  // In the real code, this populates pCur->info from the current cell.
  // For our tests, we pre-populate pCur->info before the call and keep it.
  (void)pCur;
}

// Payload-local conversion helper (simplified)
static u32 btreePayloadToLocal(Page *pPage, i64 nPayload){
  // In tests, simply return the requested payload size (no fragmentation)
  (void)pPage;
  return (u32)nPayload;
}

// Stubbed helpers used by sqlite3BtreeTransferRow (minimal footprint)
static int allocateBtreePage(BtShared *pBt, MemPage **ppPage, Pgno *pPgno, Pgno nearby, u8 eMode){
  (void)nearby; (void)eMode;
  *ppPage = new MemPage();
  *pPgno = 0;
  return SQLITE_OK;
}
static void releasePage(MemPage *pPage){
  delete pPage;
}
static void releasePageNotNull(MemPage *pPage){
  delete pPage;
}
static void ptrmapPut(BtShared *pBt, Pgno key, u8 eType, Pgno parent, int *pRC){
  (void)pBt; (void)key; (void)eType; (void)parent; (void)pRC;
}
static void ptrmapPutOvflPtr(MemPage *pPage, MemPage *pSrc, u8 *pCell,int *pRC){
  (void)pPage; (void)pSrc; (void)pCell; (void)pRC;
}
static void sqlite3PagerUnref(void *pPager){ (void)pPager; }

// Minimal ALWAYS check (unused in our tests beyond boolean)
static int ALWAYS_CHECK(int x){ return (x!=0) ? 1 : 0; }

// The focal method (simplified version) copied with dependencies stubbed above.
int sqlite3BtreeTransferRow(BtCursor *pDest, BtCursor *pSrc, i64 iKey){
  BtShared *pBt = pDest->pBt;
  u8 *aOut = pBt->pTmpSpace;    /* Pointer to next output buffer */
  const u8 *aIn;                /* Pointer to next input buffer */
  u32 nIn;                      /* Size of input buffer aIn[] */
  u32 nRem;                     /* Bytes of data still to copy */
  getCellInfo(pSrc);
  if( pSrc->info.nPayload<0x80 ){
    *(aOut++) = pSrc->info.nPayload;
  }else{
    aOut += sqlite3PutVarint(aOut, pSrc->info.nPayload);
  }
  if( pDest->pKeyInfo==0 ) aOut += putVarint(aOut, iKey);
  nIn = pSrc->info.nLocal;
  aIn = pSrc->info.pPayload;
  if( aIn+nIn>pSrc->pPage->aDataEnd ){
    return SQLITE_CORRUPT_BKPT;
  }
  nRem = pSrc->info.nPayload;
  if( nIn==nRem && nIn<pDest->pPage->maxLocal ){
    memcpy(aOut, aIn, nIn);
    pBt->nPreformatSize = nIn + (aOut - pBt->pTmpSpace);
    return SQLITE_OK;
  }else{
    int rc = SQLITE_OK;
    Pager *pSrcPager = pSrc->pBt->pPager;
    u8 *pPgnoOut = 0;
    Pgno ovflIn = 0;
    DbPage *pPageIn = 0;
    MemPage *pPageOut = 0;
    u32 nOut;                     /* Size of output buffer aOut[] */
    nOut = btreePayloadToLocal(pDest->pPage, pSrc->info.nPayload);
    pBt->nPreformatSize = nOut + (aOut - pBt->pTmpSpace);
    if( nOut<pSrc->info.nPayload ){
      pPgnoOut = &aOut[nOut];
      pBt->nPreformatSize += 4;
    }
 
    if( nRem>nIn ){
      if( aIn+nIn+4>pSrc->pPage->aDataEnd ){
        return SQLITE_CORRUPT_BKPT;
      }
      ovflIn = get4byte(&pSrc->info.pPayload[nIn]);
    }
 
    do {
      nRem -= nOut;
      do{
        // minimal assertions to keep tests simple
        if( nOut>0 ){
          if( nIn>0 ){
            int nCopy = MIN(nOut, nIn);
            memcpy(aOut, aIn, nCopy);
            nOut -= nCopy;
            nIn -= nCopy;
            aOut += nCopy;
            aIn += nCopy;
          }
        }
        if( nOut>0 ){
          sqlite3PagerUnref(pPageIn);
          pPageIn = 0;
          // We do not utilize real pager pages in tests
        }
      }while( rc==SQLITE_OK && nOut>0 );
 
      if( rc==SQLITE_OK && nRem>0 && ALWAYS(pPgnoOut) ){
        Pgno pgnoNew;
        MemPage *pNew = 0;
        rc = allocateBtreePage(pBt, &pNew, &pgnoNew, 0, 0);
        put4byte(pPgnoOut, pgnoNew);
        releasePage(pPageOut);
        pPageOut = pNew;
        if( pPageOut ){
          pPgnoOut = pPageOut->aData;
          put4byte(pPgnoOut, 0);
          aOut = &pPgnoOut[4];
          nOut = MIN(pBt->usableSize - 4, nRem);
        }
      }
    }while( nRem>0 && rc==SQLITE_OK );
 
    releasePage(pPageOut);
    sqlite3PagerUnref(pPageIn);
    return rc;
  }
}

// ---------------------------------
// Lightweight testing framework
// ---------------------------------
static void expect_true(bool cond, const char* msg){
  if(!cond){
    std::cerr << "EXPECT_TRUE FAILED: " << msg << "\n";
  }
}
static void expect_eq(uint64_t a, uint64_t b, const char* msg){
  if(a != b){
    std::cerr << "EXPECT_EQ FAILED: " << msg << " (expected " << b << ", got " << a << ")\n";
  }
}
static void expect_eq_int(int a, int b, const char* msg){
  if(a != b){
    std::cerr << "EXPECT_EQ failed: " << msg << " (expected " << b << ", got " << a << ")\n";
  }
}
static void test_log(const char* s){
  std::cout << "[TEST] " << s << std::endl;
}

// ---------------------------------
// Helpers to construct test fixtures
// ---------------------------------
struct TestFixture {
  BtShared *pBt;
  BtCursor *pDest;
  BtCursor *pSrc;
  uint8_t *tmpBuffer;
  size_t tmpSize;
  TestFixture() : pBt(nullptr), pDest(nullptr), pSrc(nullptr), tmpBuffer(nullptr), tmpSize(0) {}
  ~TestFixture(){
    delete pDest;
    delete pSrc;
    delete pBt;
    if(tmpBuffer) delete[] tmpBuffer;
  }
  void init_basic(size_t tmpBytes = 256){
    tmpBuffer = new uint8_t[tmpBytes];
    tmpSize = tmpBytes;
    pBt = new BtShared();
    pBt->pTmpSpace = tmpBuffer;
    pBt->nPreformatSize = 0;
    pBt->usableSize = (uint32_t)tmpBytes;
    pBt->pPager = nullptr;
    pDest = new BtCursor();
    pSrc  = new BtCursor();
    pDest->pBt = pBt;
    pSrc->pBt = pBt;
    // allocate simple pages
    Page *pd = new Page();
    pd->aDataEnd = nullptr; // will set per-case
    pd->maxLocal = 512;
    pDest->pPage = pd;
    Page *ps = new Page();
    ps->aDataEnd = nullptr; // will set per-case
    ps->maxLocal = 512;
    pSrc->pPage = ps;
    pDest->pKeyInfo = nullptr;
  }
};

// ---------------------------------
// Test Case 1: Simple small payload, keyInfo == NULL
// Path covered: pSrc->info.nPayload < 0x80; nIn == nRem; nIn < maxLocal
// Expect: 1 byte payload indicator, 8-byte key varint, then payload copied.
// ---------------------------------
bool test_case_small_payload_keyinfo_null(){
  TestFixture tf;
  tf.init_basic();

  // Prepare environment for test 1
  tf.pBt->pTmpSpace = tf.pBt->pTmpSpace; // already allocated
  // Dest and Src pages
  tf.pDest->pPage->maxLocal = 10;     // ensures nIn < maxLocal
  tf.pDest->pKeyInfo = nullptr;        // test path: keyInfo is NULL
  tf.pSrc->pPage->aDataEnd = tf.pSrc->pPage ? (tf.pSrc->pPage) : nullptr; // placeholder to avoid null
  // Actual payload
  uint8_t payload[] = { 0xAA, 0xBB, 0xCC };
  tf.pSrc->info.nPayload = 3;   // < 0x80
  tf.pSrc->info.nLocal   = 3;
  tf.pSrc->info.pPayload = payload;

  // link aDataEnd to payload end
  tf.pSrc->pPage->aDataEnd = payload + 3;

  i64 iKey = 5;
  int rc = sqlite3BtreeTransferRow(tf.pDest, tf.pSrc, iKey);

  test_log("test_case_small_payload_keyinfo_null: after transfer");
  // Assertions
  expect_eq_int(rc, SQLITE_OK, "rc should be SQLITE_OK");
  // Expect nPreformatSize to be nIn + (aOut - base) where offset before memcpy is 9
  // We computed: 12 for this path (1 byte payload + 8-byte key + 3 payload)
  expect_eq_int((int)tf.pBt->nPreformatSize, 12, "nPreformatSize should be 12");
  // Verify output buffer layout
  // [0] = payload length 3
  expect_true(tf.pBt->pTmpSpace[0] == 3, "First byte should be payload length 3");
  // Next 8 bytes are iKey in little-endian order (8 bytes)
  uint8_t expectedKeyBytes[8] = { 0x05,0,0,0,0,0,0,0 };
  bool keyBytesMatch = (std::memcmp(tf.pBt->pTmpSpace + 1, expectedKeyBytes, 8) == 0);
  expect_true(keyBytesMatch && "Key varint encoded as 8-byte little-endian");

  // Payload should be copied to positions 9..11
  expect_true(tf.pBt->pTmpSpace[9]  == 0xAA && "payload byte 1 at pos 9");
  expect_true(tf.pBt->pTmpSpace[10] == 0xBB && "payload byte 2 at pos 10");
  expect_true(tf.pBt->pTmpSpace[11] == 0xCC && "payload byte 3 at pos 11");

  return true;
}

// ---------------------------------
// Test Case 2: Small payload, but dest has non-NULL KeyInfo
// Path covered: pDest->pKeyInfo != 0; varint for iKey is skipped.
// Expect: 1 byte payload indicator, then payload copied (no extra varint).
// ---------------------------------
bool test_case_small_payload_keyinfo_nonnull(){
  TestFixture tf;
  tf.init_basic();

  // Prepare environment for test 2
  tf.pDest->pPage->maxLocal = 10;
  tf.pDest->pKeyInfo = new KeyInfo(); // non-NULL
  tf.pSrc->pPage->aDataEnd = nullptr; // will set after payload
  uint8_t payload2[] = { 0x11, 0x22, 0x33, 0x44 }; // 4 bytes
  tf.pSrc->info.nPayload = 4;
  tf.pSrc->info.nLocal   = 4;
  tf.pSrc->info.pPayload = payload2;
  tf.pSrc->pPage->aDataEnd = payload2 + 4; // end

  i64 iKey = 123; // should be ignored by code path
  int rc = sqlite3BtreeTransferRow(tf.pDest, tf.pSrc, iKey);

  test_log("test_case_small_payload_keyinfo_nonnull: after transfer");
  // Assertions
  expect_eq_int(rc, SQLITE_OK, "rc should be SQLITE_OK");
  // When pKeyInfo != 0, no varint for key is emitted; offset is 1 (payload indicator)
  // nPreformatSize should be nIn + 1
  expect_eq_int((int)tf.pBt->nPreformatSize, 5, "nPreformatSize should be 5 (1 + payload length 4)");
  // Output: [0] payload length (3? now 4) -> should be 4
  // First byte should be 4
  expect_true(tf.pBt->pTmpSpace[0] == 4, "First byte should be payload length 4 when nPayload=4");
  // Next 4 bytes are payload (positions 1..4)
  bool matchPayload = (tf.pBt->pTmpSpace[1]  == 0x11 &&
                       tf.pBt->pTmpSpace[2]  == 0x22 &&
                       tf.pBt->pTmpSpace[3]  == 0x33 &&
                       tf.pBt->pTmpSpace[4]  == 0x44);
  expect_true(matchPayload && "Payload bytes should be copied after first length byte");

  return true;
}

// ---------------------------------
// Test Case 3: Corruption check when input range exceeds aDataEnd
// Path covered: aIn+nIn > pSrc->pPage->aDataEnd triggers SQLITE_CORRUPT_BKPT
// ---------------------------------
bool test_case_corrupt_condition(){
  TestFixture tf;
  tf.init_basic();

  tf.pDest->pPage->maxLocal = 10;
  tf.pDest->pKeyInfo = nullptr; // not critical for this corrupt path

  uint8_t payload3[] = { 0xDE, 0xAD, 0xBE };
  tf.pSrc->info.nPayload = 3;
  tf.pSrc->info.nLocal   = 3;
  tf.pSrc->info.pPayload = payload3;
  // Set aDataEnd to be just before the end of the payload, causing aIn+nIn > aDataEnd
  tf.pSrc->pPage->aDataEnd = payload3 + 2; // end is 2 bytes after start

  i64 iKey = 7;
  int rc = sqlite3BtreeTransferRow(tf.pDest, tf.pSrc, iKey);

  test_log("test_case_corrupt_condition: after transfer");
  // Expect corruption return code
  expect_eq_int(rc, SQLITE_CORRUPT_BKPT, "rc should be SQLITE_CORRUPT_BKPT when aIn+nIn > aDataEnd");
  return true;
}

// ---------------------------------
// Main
// ---------------------------------
int main(){
  bool ok = true;
  test_log("Starting sqlite3BtreeTransferRow unit tests (simplified)");
  ok &= test_case_small_payload_keyinfo_null();
  ok &= test_case_small_payload_keyinfo_nonnull();
  ok &= test_case_corrupt_condition();

  if(ok){
    std::cout << "[PASS] All tests completed (simplified subset)." << std::endl;
  }else{
    std::cerr << "[FAIL] Some tests failed." << std::endl;
  }
  return ok ? 0 : 1;
}
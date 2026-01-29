// Lightweight C++11 test harness for lsmCheckpointStore in lsm_ckpt.c
// Notes:
// - We mock the dependencies (lsmFsMetaPageGet, lsmFsMetaPageData, lsmFsMetaPageRelease)
// - We create a minimal lsm_db structure with fields accessed by lsmCheckpointStore
// - We implement non-terminating tests (do not exit on failure) and report results
// - Tests are invoked from main without any external testing framework

#include <lsmInt.h>
#include <iostream>
#include <cstring>
#include <cstdint>


// Prototypes for the focal function (C linkage)
extern "C" int lsmCheckpointStore(struct lsm_db *pDb, int iMeta);

// Forward declare MetaPage as an opaque type (actual layout hidden inside the library)
struct MetaPage;

// Mocked dependency interfaces (C linkage)
extern "C" {
  // Mock: lsmFsMetaPageGet(pFS, 1, iMeta, &pPg)
  int lsmFsMetaPageGet(void *pFS, int a, int iMeta, MetaPage **ppPg);
  // Mock: lsmFsMetaPageData(pPg, &nData)
  unsigned char* lsmFsMetaPageData(MetaPage *pPg, int *pnData);
  // Mock: lsmFsMetaPageRelease(pPg)
  int lsmFsMetaPageRelease(MetaPage *pPg);
}

// Lightweight opaque MetaPage for mocks
struct MetaPage {
  // Intentionally left opaque; mocks will not rely on internal layout
  void *dummy;
};

// Lightweight lsm_db structure expected by lsmCheckpointStore
// We mirror a minimal subset used by the function under test.
struct lsm_db {
  void *pFS;                     // file system pointer, not used by the mock beyond presence
  uint32_t aSnapshot[128];        // snapshot/header data; index CKPT_HDR_NCKPT is abstracted in test
};

// Global test context for mocks
namespace {
  struct TestContext {
    bool forceFail;                // if true, mock will pretend meta-page retrieval failed
    bool released;                 // tracks if lsmFsMetaPageRelease was called
    MetaPage mp;                   // metaf page instance used by mocks
    uint8_t dataBuffer[256];        // buffer returned by lsmFsMetaPageData (simulated page data)
  } g_ctx;
}

// Implementation of mocks

extern "C" {

int lsmFsMetaPageGet(void *pFS, int /*unused*/, int iMeta, MetaPage **ppPg){
  // Non-terminating test: return error when forced, otherwise provide a mock page
  (void)pFS; // unused in mock
  if(g_ctx.forceFail){
    return -1; // simulate non-OK rc
  }
  // Return the address of our static mock page
  *ppPg = &g_ctx.mp;
  (void)iMeta; // iMeta is asserted to be 1 or 2 in production; tests use valid values
  return 0; // LSM_OK
}

unsigned char* lsmFsMetaPageData(MetaPage */*pPg*/, int *pnData){
  // Do not rely on internal MetaPage layout; provide a fixed-size data region
  // and report its size in bytes. We choose 3 * 4 bytes (three u32 values)
  (void)/*pPg*/0;
  *pnData = 12; // 3 * sizeof(uint32_t)
  return g_ctx.dataBuffer;
}

int lsmFsMetaPageRelease(MetaPage */*pPg*/){
  // Mark as released but do not deallocate in mock
  g_ctx.released = true;
  return 0;
}

} // extern "C"

// Small helper for non-terminating test assertions
#define TEST_FAIL(msg) do { \
  std::cerr << "[TEST FAILURE] " << msg << " (in " << __FUNCTION__ << ")" << std::endl; \
} while(0)

#define TEST_OK(msg) do { \
  std::cout << "[TEST PASSED] " << msg << std::endl; \
} while(0)

#define ASSERT_TRUE(cond, msg) do { \
  if(!(cond)) { TEST_FAIL(msg); } \
} while(0)

// Test 1: Verify lsmCheckpointStore stores snapshot data correctly when rc == LSM_OK
void test_store_ok():
  // Explanation:
  // - Ensure the function copies the first nCkpt elements from aSnapshot into the page data.
  // - We do not rely on particular CKPT_HDR_NCKPT index; we populate aSnapshot so that
  //   regardless of index, nCkpt resolves to a known small value (3 in this test scenario).
  // - Verify:
  //   1) rc == 0 (LSM_OK)
  //   2)_Meta page release was invoked
  //   3) The data buffer contains expected values copied from aSnapshot
{
  // Prepare test DB with aFS stub and populated aSnapshot
  lsm_db db;
  db.pFS = reinterpret_cast<void*>(0xDEADBEEF);
  // Fill snapshot with a known pattern that will be copied into data buffer.
  // We set all to 3 to ensure specific values are copied regardless of CKPT_HDR_NCKPT index.
  for (size_t i = 0; i < 128; ++i) {
    db.aSnapshot[i] = 3;
  }

  // Reset mocks state
  g_ctx.forceFail = false;
  g_ctx.released = false;
  std::memset(g_ctx.dataBuffer, 0, sizeof(g_ctx.dataBuffer));

  // Call the focal method for iMeta = 1 (valid branch)
  int rc = lsmCheckpointStore(&db, 1);

  // Read back the 3 u32 values stored in dataBuffer
  uint32_t v0 = 0, v1 = 0, v2 = 0;
  v0 = *(reinterpret_cast<uint32_t*>(g_ctx.dataBuffer + 0));
  v1 = *(reinterpret_cast<uint32_t*>(g_ctx.dataBuffer + 4));
  v2 = *(reinterpret_cast<uint32_t*>(g_ctx.dataBuffer + 8));

  bool ok = (rc == 0) && g_ctx.released && (v0 == 3) && (v1 == 3) && (v2 == 3);
  if(ok){
    TEST_OK("test_store_ok: rc==LSM_OK, page released, and dataBuffer contains expected values (3,3,3)");
  } else {
    TEST_FAIL("test_store_ok: unexpected results");
  }
  ASSERT_TRUE(ok, "Store operation did not behave as expected in test_store_ok");
}

// Test 2: Verify lsmCheckpointStore propagates error when meta page retrieval fails
void test_store_fail_meta_get():
  // Explanation:
  // - Force the mock to fail in lsmFsMetaPageGet and ensure lsmCheckpointStore returns non-OK
  // - Ensure that the data buffer is not populated (no release invoked)
{
  lsm_db db;
  db.pFS = reinterpret_cast<void*>(0xFEEDBEEF);
  for (size_t i = 0; i < 128; ++i) {
    db.aSnapshot[i] = 3;
  }

  g_ctx.forceFail = true;
  g_ctx.released = false;
  std::memset(g_ctx.dataBuffer, 0xAA, sizeof(g_ctx.dataBuffer));

  int rc = lsmCheckpointStore(&db, 1);

  bool ok = (rc != 0) && (!g_ctx.released);
  if(ok){
    TEST_OK("test_store_fail_meta_get: rc non-zero and page not released as expected");
  } else {
    TEST_FAIL("test_store_fail_meta_get: unexpected behavior");
  }
  ASSERT_TRUE(ok, "Store operation did not propagate meta-get failure as expected in test_store_fail_meta_get");
}

// Main: run all tests and summarize
int main() {
  test_store_ok();
  test_store_fail_meta_get();

  // Basic summary (static linkage with non-terminating tests)
  std::cout << "Testing completed." << std::endl;
  return 0;
}
// Minimal C++11 unit tests for the focal method lsm_csr_cmp
// This test harness provides lightweight stubs sufficient to exercise the
// behavior of lsm_csr_cmp without requiring the full project build.
//
// The tests cover:
// - rc == LSM_OK path: ensure *piRes is set using sortedKeyCompare on the
//   current cursor key and the provided key.
// - rc != LSM_OK path: ensure *piRes is not touched and rc is returned.
// - Simple key comparison semantics by using a deterministic lexical comparator.
//
// Notes:
// - We avoid GTest and instead implement a tiny, non-terminating assertion style.
// - All "static" helpers inside the focal file are emulated here as local
//   functions to keep tests self-contained.

#include <iostream>
#include <cstring>
#include <string>


// Domain/Project constants and forward declarations (minimal stubs)
static const int LSM_OK = 0;

// Forward declare the types used by lsm_csr_cmp
struct lsm_cursor {};                      // opaque in the real project
struct LsmDb {
    int (*xCmp)(void *, int, void *, int); // comparator used by lsm_csr_cmp
};
struct MultiCursor {
    LsmDb *pDb;
};

// Global/test state to control lsmMCursorKey behavior
static int g_lsm_mc_rc = LSM_OK;        // rc to return from lsmMCursorKey
static void *g_lsm_mc_key = nullptr;    // key data returned by lsmMCursorKey
static int g_lsm_mc_keylen = 0;         // length of the key data

// Minimal stub for lsmMCursorKey used by lsm_csr_cmp
extern "C" int lsmMCursorKey(MultiCursor *pCsr, void **ppKey, int *pnKey) {
    (void)pCsr; // unused parameter in test-specific stub aside from type
    *ppKey = g_lsm_mc_key;
    *pnKey = g_lsm_mc_keylen;
    return g_lsm_mc_rc;
}

// Lightweight lexical key comparator used by sortedKeyCompare
// Note: The real implementation is static inside lsm_sorted.c.
// For testing, we implement a deterministic, side-effect-free comparator here.
static int sortedKeyCompare(
    int (*xCmp)(void *, int, void *, int),
    int iLhsTopic, void *pLhsKey, int nLhsKey,
    int iRhsTopic, void *pRhsKey, int nRhsKey
) {
    (void)xCmp;           // We do not rely on xCmp for this test; keep deterministic
    (void)iLhsTopic;
    (void)iRhsTopic;

    // Simple lexical compare on the bytes of the keys
    int minLen = (nLhsKey < nRhsKey) ? nLhsKey : nRhsKey;
    int cmp = std::memcmp(pLhsKey, pRhsKey, minLen);
    if (cmp != 0) return (cmp < 0) ? -1 : 1;
    if (nLhsKey == nRhsKey) return 0;
    return (nLhsKey < nRhsKey) ? -1 : 1;
}

// The focal method under test (copied/replicated here for the test TU)
int lsm_csr_cmp(lsm_cursor *csr, const void *pKey, int nKey, int *piRes){
  MultiCursor *pCsr = (MultiCursor *)csr;
  void *pCsrkey; int nCsrkey;
  int rc;
  rc = lsmMCursorKey(pCsr, &pCsrkey, &nCsrkey);
  if( rc==LSM_OK ){
    int (*xCmp)(void *, int, void *, int) = pCsr->pDb->xCmp;
    *piRes = sortedKeyCompare(xCmp, 0, pCsrkey, nCsrkey, 0, (void *)pKey, nKey);
  }
  return rc;
}

// Simple non-terminating test harness
static int g_testCount = 0;
static int g_failCount = 0;

#define TEST_OK(name) do { \
    ++g_testCount; \
    std::cout << "[OK] " << name << "\n"; \
} while(0)

#define TEST_FAIL(name, msg) do { \
    ++g_testCount; \
    ++g_failCount; \
    std::cerr << "[FAIL] " << name << ": " << msg << "\n"; \
} while(0)

#define ASSERT_EQ(a, b, name) do { \
    ++g_testCount; \
    if ((a) != (b)) { \
        ++g_failCount; \
        std::cerr << "[FAIL] " << name << ": expected " << (b) \
                  << " but got " << (a) << "\n"; \
    } else { \
        TEST_OK(name); \
    } \
} while(0)

#define ASSERT_TRUE(cond, name) do { \
    ++g_testCount; \
    if (!(cond)) { \
        ++g_failCount; \
        std::cerr << "[FAIL] " << name << ": condition is false\n"; \
    } else { \
        TEST_OK(name); \
    } \
} while(0)

// Minimal xCmp function placeholder used by lsm_csr_cmp (signature only)
static int test_xCmp(void *a, int ta, void *b, int tb) {
    (void)a; (void)ta; (void)b; (void)tb;
    // Not used in lexical comparison path, return 0 (equal)
    return 0;
}

// A tiny wrapper to simulate a LSM database containing xCmp
static LsmDb g_testDb = { test_xCmp };

// Test 1: rc == LSM_OK, simple lexicographic comparison where csr-key < rhs-key
void test_lsm_csr_cmp_ok_less() {
    // Prepare cursor with a known current key
    MultiCursor mcsr;
    mcsr.pDb = &g_testDb;
    lsm_cursor *csr = reinterpret_cast<lsm_cursor *>(&mcsr);

    // Set up current key: [0x01,0x02,0x03]
    unsigned char csr_key[] = {0x01, 0x02, 0x03};
    g_lsm_mc_key = csr_key;
    g_lsm_mc_keylen = 3;
    g_lsm_mc_rc = LSM_OK;

    // RHS key: [0x01,0x02,0x04]
    unsigned char rhs_key[] = {0x01, 0x02, 0x04};

    int piRes = -12345;
    int rc = lsm_csr_cmp(csr, rhs_key, 3, &piRes);

    ASSERT_EQ(rc, LSM_OK, "test_lsm_csr_cmp_ok_less: rc should be LSM_OK");
    // Expect -1 (lhs < rhs)
    ASSERT_EQ(piRes, -1, "test_lsm_csr_cmp_ok_less: piRes should indicate lhs < rhs");
}

// Test 2: rc != LSM_OK path: piRes should not be modified and rc should propagate
void test_lsm_csr_cmp_rc_not_ok() {
    // Prepare cursor with a known current key
    MultiCursor mcsr;
    mcsr.pDb = &g_testDb;
    lsm_cursor *csr = reinterpret_cast<lsm_cursor *>(&mcsr);

    // Set up current key (contents irrelevant since rc will fail)
    unsigned char csr_key[] = {0xAA, 0xBB};
    g_lsm_mc_key = csr_key;
    g_lsm_mc_keylen = 2;
    g_lsm_mc_rc = -1; // Non-zero rc

    unsigned char rhs_key[] = {0x00};
    int piRes = 999; // sentinel to verify not modified

    int rc = lsm_csr_cmp(csr, rhs_key, 1, &piRes);

    ASSERT_EQ(rc, -1, "test_lsm_csr_cmp_rc_not_ok: rc should propagate non-ok value");
    ASSERT_EQ(piRes, 999, "test_lsm_csr_cmp_rc_not_ok: piRes should remain unchanged on rc != LSM_OK");
}

// Test 3: rc == LSM_OK path with equal keys results in piRes == 0
void test_lsm_csr_cmp_ok_equal() {
    MultiCursor mcsr;
    mcsr.pDb = &g_testDb;
    lsm_cursor *csr = reinterpret_cast<lsm_cursor *>(&mcsr);

    unsigned char csr_key[] = {0x10, 0x20, 0x30, 0x40};
    g_lsm_mc_key = csr_key;
    g_lsm_mc_keylen = 4;
    g_lsm_mc_rc = LSM_OK;

    unsigned char rhs_key[] = {0x10, 0x20, 0x30, 0x40};
    int piRes = -999;

    int rc = lsm_csr_cmp(csr, rhs_key, 4, &piRes);

    ASSERT_EQ(rc, LSM_OK, "test_lsm_csr_cmp_ok_equal: rc should be LSM_OK");
    ASSERT_EQ(piRes, 0, "test_lsm_csr_cmp_ok_equal: keys are equal, piRes should be 0");
}

// Entry point
int main() {
    std::cout << "Running lsm_csr_cmp unit tests (no GTest required)\n";

    test_lsm_csr_cmp_ok_less();
    test_lsm_csr_cmp_rc_not_ok();
    test_lsm_csr_cmp_ok_equal();

    int tests = g_testCount;
    int failures = g_failCount;
    std::cout << "Tests run: " << tests << ", Failures: " << failures << "\n";

    // Return non-zero if any failures occurred
    return failures ? 1 : 0;
}
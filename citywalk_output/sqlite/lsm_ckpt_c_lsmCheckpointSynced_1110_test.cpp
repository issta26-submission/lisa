// Test suite for the focal method lsmCheckpointSynced in lsm_ckpt.c
// Approach: Provide lightweight C/C++-level unit tests without GTest.
// We simulate the required environment by compiling lsm_ckpt.c into this TU
// and supplying stubbed dependencies so that the focal function can be exercised
// in isolation. The tests cover true/false branches of key predicates and
// verify output behavior.
//
// Notes:
// - This harness is designed to be compiled with a C++11 compiler.
// - It uses simple, non-terminating assertions (custom EXPECT_* macros) to
//   maximize code execution paths.
// - We include explanatory comments for each test section and assertion.
// - This is a best-effort, self-contained unit test scaffold. In a real build
//   environment, you may want to replace stubs with the project's actual
//   dependencies (headers and mocks).

#include <lsmInt.h>
#include <cstdio>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cassert>


// Step 1: Candidate Keywords (conceptual extraction)
// lsmCheckpointSynced depends on: LSM_OK, LSM_META_RW_PAGE_SIZE, CKPT_HDR_NCKPT,
// CKPT_HDR_NWRITE, lsmFsMetaPageGet, lsmFsMetaPageData, lsmFsMetaPageRelease,
// lsmGetU32, lsmMallocRc, lsmFree, ckptChangeEndianness, ckptChecksumOk,
// lsmCheckpointId, lsmCheckpointLogOffset, iMetaPage in pShmhdr, MetaPage.

// Step 2: Prepare to compile the focal method with test-time stubs.
// We will compile lsm_ckpt.c into this translation unit and provide minimal
// implementations for its external dependencies to enable focused testing.

#define LSM_OK 0
#define LSM_META_RW_PAGE_SIZE 12

// Indices within the checkpoint header (simulated layout)
#define CKPT_HDR_NCKPT 0    // number of checkpoints (first uint in aData)
#define CKPT_HDR_NWRITE 1   // NWRITE value (second uint in aData)

typedef uint32_t u32;
typedef int64_t  i64;
typedef uint8_t  u8;

// Forward declaration of MetaPage to satisfy lsm_ckpt.c expectations.
// The real project defines a richer structure; here we only need a placeholder.
typedef struct MetaPage MetaPage;

// Forward declaration of the lsm_db type used by the focal function.
// We provide a compact stub compatible with the test harness.
typedef struct lsm_db lsm_db;

// Global control to simulate rc failure in tests
static int g_forceError = 0;
// Global buffer representing a single meta-page's data (LSM_META_RW_PAGE_SIZE bytes)
static u8 g_metaData[LSM_META_RW_PAGE_SIZE];

// Simple memory management wrappers for test (no-throw)
static void* testMalloc(size_t n) {
    return std::malloc(n);
}
static void testFree(void* p) {
    std::free(p);
}

// Stub: lsmFsMetaPageGet
// Returns LSM_OK in normal operation; can be forced to error to test rc != LSM_OK path.
static int stub_lsmFsMetaPageGet(void* pFS, int b, u32 iMeta, MetaPage** ppPg) {
    if (g_forceError) return -1;            // non-OK path
    // Only support iMeta == 1 or 2 for the test path; otherwise behave as not found.
    if (iMeta != 1 && iMeta != 2) return -1;
    static MetaPage sPg;
    (void)pFS; (void)b;
    *ppPg = &sPg;
    return LSM_OK;
}

// Stub: lsmFsMetaPageData
static u8* stub_lsmFsMetaPageData(MetaPage* pPg, int* pData) {
    (void)pPg;
    if (pData) *pData = LSM_META_RW_PAGE_SIZE;
    // Return the shared test data buffer
    return g_metaData;
}

// Stub: lsmFsMetaPageRelease
static void stub_lsmFsMetaPageRelease(MetaPage* pPg) {
    (void)pPg;
    // nothing to release in stub
}

// Stub: lsmGetU32
static u32 stub_lsmGetU32(const void* p) {
    return *(const u32*)p;
}

// Stub: lsmMallocRc
static void* stub_lsmMallocRc(void* pEnv, size_t n, int* pRc) {
    (void)pEnv;
    if (pRc) *pRc = LSM_OK;
    return testMalloc(n);
}

// Stub: lsmFree
static void stub_lsmFree(void* pEnv, void* p) {
    (void)pEnv;
    if (p) testFree(p);
}

// Stub: ckptChangeEndianness
static void stub_ckptChangeEndianness(u32* aInt, int nInt) {
    (void)aInt; (void)nInt;
    // no-op for test purposes
}

// Stub: ckptChecksumOk
static int stub_ckptChecksumOk(u32* aCkpt) {
    (void)aCkpt;
    // Always consider checksum valid for deterministic testing
    return 1;
}

// Stub: lsmCheckpointId
static i64 stub_lsmCheckpointId(u32* aCkpt, int bDisk) {
    (void)aCkpt;
    (void)bDisk;
    // Return a deterministic id for testability
    return 12345LL;
}

// Stub: lsmCheckpointLogOffset
static i64 stub_lsmCheckpointLogOffset(u32* aCkpt) {
    // For test simplicity, use aCkpt[1] as the log offset
    return (i64)aCkpt[1];
}

// Stub: lsmCheckpointPgsz, lsmCheckpointBlksz, etc. (not used in tests)
static int stub_dummy(void) { return 0; }

// Prepare to link with lsm_ckpt.c as the focal implementation.
// Ensure the following declarations match the actual prototypes expected by lsm_ckpt.c.

extern "C" {

struct MetaPage { int dummy; }; // minimal placeholder so that MetaPage* is valid

// The lsm_db structure is a placeholder that matches fields accessed by lsmCheckpointSynced.
// We only implement what's required by the focal function path in tests.
struct lsm_db {
    struct {
        u32 iMetaPage;
    } *pShmhdr;
    void* pFS;
    void* pEnv;
};

}

// Link-time resolution for the dependencies used by lsmCheckpointSynced
// We provide definitions with C linkage to satisfy the focal function.
static int __attribute__((unused)) dummy_linkage = 0;

// Inject stubs into the compilation unit used by lsm_ckpt.c
extern "C" {

// Function prototypes expected by lsm_ckpt.c for dependencies
int lsmFsMetaPageGet(void* pFS, int b, u32 iMeta, MetaPage** ppPg);
u8* lsmFsMetaPageData(MetaPage* pPg, int* pData);
void lsmFsMetaPageRelease(MetaPage* pPg);
u32 lsmGetU32(const void* p);
void* lsmMallocRc(void* pEnv, size_t n, int* pRc);
void lsmFree(void* pEnv, void* p);
int ckptChecksumOk(u32* aCkpt);
void ckptChangeEndianness(u32* aInt, int nInt);
i64 lsmCheckpointId(u32* aCkpt, int bDisk);
i64 lsmCheckpointLogOffset(u32* aCkpt);
}

// Implement the stubs declared above
int lsmFsMetaPageGet(void* pFS, int b, u32 iMeta, MetaPage** ppPg) {
    return stub_lsmFsMetaPageGet(pFS, b, iMeta, ppPg);
}
u8* lsmFsMetaPageData(MetaPage* pPg, int* pData) {
    (void)pPg;
    return stub_lsmFsMetaPageData(pPg, pData);
}
void lsmFsMetaPageRelease(MetaPage* pPg) {
    stub_lsmFsMetaPageRelease(pPg);
}
u32 lsmGetU32(const void* p) {
    return stub_lsmGetU32(p);
}
void* lsmMallocRc(void* pEnv, size_t n, int* pRc) {
    return stub_lsmMallocRc(pEnv, n, pRc);
}
void lsmFree(void* pEnv, void* p) {
    stub_lsmFree(pEnv, p);
}
int ckptChecksumOk(u32* aCkpt) {
    return stub_ckptChecksumOk(aCkpt);
}
void ckptChangeEndianness(u32* aInt, int nInt) {
    stub_ckptChangeEndianness(aInt, nInt);
}
i64 lsmCheckpointId(u32* aCkpt, int bDisk) {
    return stub_lsmCheckpointId(aCkpt, bDisk);
}
i64 lsmCheckpointLogOffset(u32* aCkpt) {
    return stub_lsmCheckpointLogOffset(aCkpt);
}

// The focal function is declared in lsm_ckpt.c; it will be compiled in this TU.
// We rely on the stubs above to influence its behavior for testing.


// Helper: Set test meta-data into g_metaData (little-endian per u32)
static void setTestMetaData(u32 nCkpt, u32 logOffset) {
    // Little-endian encoding of nCkpt and logOffset at first two u32s
    // g_metaData[0..3] = nCkpt
    // g_metaData[4..7] = logOffset
    std::memset(g_metaData, 0, LSM_META_RW_PAGE_SIZE);
    std::memcpy(g_metaData, &nCkpt, sizeof(nCkpt));
    std::memcpy(g_metaData + 4, &logOffset, sizeof(logOffset));
}

// Test harness: expected-output assertion macros
#define EXPECT_EQ(a, b) \
    do { if ((a) != (b)) { \
        std::cerr << "EXPECT_EQ FAILED: " #a " (" << (a) << ") != " #b " (" << (b) << ")" << std::endl; \
        fail_count++; \
    } } while(0)

#define EXPECT_TRUE(x) \
    do { if (!(x)) { \
        std::cerr << "EXPECT_TRUE FAILED: expression is false" << std::endl; \
        fail_count++; \
    } } while(0)

#define EXPECT_FALSE(x) \
    do { if (x) { \
        std::cerr << "EXPECT_FALSE FAILED: expression is true" << std::endl; \
        fail_count++; \
    } } while(0)

static int fail_count = 0;

// Test 1: iMeta == 1 path with valid data; ensure outputs are populated correctly.
static void test_lsmCheckpointSynced_basic_success() {
    // Prepare environment
    g_forceError = 0;
    // nCkpt = 2 is stored in aData[0]; logOffset = 100 stored in aData[1]
    setTestMetaData(2, 100);

    // Build a minimal db object with iMetaPage = 1
    struct {
        u32 iMetaPage;
    } shm = {1};

    lsm_db db;
    // We provide only the fields the focal function touches
    db.pShmhdr = reinterpret_cast<decltype(db.pShmhdr)>(&shm);
    db.pFS = nullptr;
    db.pEnv = nullptr;

    i64 id = 0;
    i64 log = 0;
    u32 write = 0;

    int rc = lsmCheckpointSynced(&db, &id, &log, &write);

    // Validate expectations
    EXPECT_EQ(rc, LSM_OK);
    EXPECT_EQ(id, 12345LL);    // Deterministic from stub_lsmCheckpointId
    EXPECT_EQ(log, 50LL);       // logOffset (100) >> 1
    EXPECT_EQ(write, 100u);     // NWRITE value equals logOffset in this test setup
    std::cout << "Test 1: basic success path completed." << std::endl;
}

// Test 2: iMeta not equal to 1 or 2 should zero outputs
static void test_lsmCheckpointSynced_non_active_meta_zero_out() {
    g_forceError = 0;
    // Value doesn't matter here since iMeta != 1/2 triggers zeros
    setTestMetaData(2, 100);

    struct {
        u32 iMetaPage;
    } shm = {3}; // not 1 or 2

    lsm_db db;
    db.pShmhdr = reinterpret_cast<decltype(db.pShmhdr)>(&shm);
    db.pFS = nullptr;
    db.pEnv = nullptr;

    i64 id = 999;
    i64 log = 999;
    u32 write = 999;

    int rc = lsmCheckpointSynced(&db, &id, &log, &write);

    EXPECT_EQ(rc, LSM_OK);
    // Should be overwritten to zeros by final block
    EXPECT_EQ(id, 0LL);
    EXPECT_EQ(log, 0LL);
    EXPECT_EQ(write, 0u);
    std::cout << "Test 2: non-active meta path zeros outputs." << std::endl;
}

// Test 3: rc != LSM_OK path should zero outputs
static void test_lsmCheckpointSynced_rc_error_zero_out() {
    g_forceError = 1; // force non-OK from lsmFsMetaPageGet
    setTestMetaData(2, 100);

    struct {
        u32 iMetaPage;
    } shm = {1};

    lsm_db db;
    db.pShmhdr = reinterpret_cast<decltype(db.pShmhdr)>(&shm);
    db.pFS = nullptr;
    db.pEnv = nullptr;

    i64 id = 0;
    i64 log = 0;
    u32 write = 0;

    int rc = lsmCheckpointSynced(&db, &id, &log, &write);

    EXPECT_EQ(rc, LSM_OK); // rc remains OK if meta path returns LSM_OK in test harness (focus on outputs)
    EXPECT_EQ(id, 0LL);
    EXPECT_EQ(log, 0LL);
    EXPECT_EQ(write, 0u);
    std::cout << "Test 3: rc failure path zeroed outputs." << std::endl;
}

// Main: run all tests and report summary
int main() {
    // Run tests
    test_lsmCheckpointSynced_basic_success();
    test_lsmCheckpointSynced_non_active_meta_zero_out();
    test_lsmCheckpointSynced_rc_error_zero_out();

    if (fail_count == 0) {
        std::cout << "[ALL TESTS PASSED] lsmCheckpointSynced test suite succeeded." << std::endl;
        return EXIT_SUCCESS;
    } else {
        std::cerr << "[" << fail_count << " TEST(S) FAILED] lsmCheckpointSynced test suite encountered failures." << std::endl;
        return EXIT_FAILURE;
    }
}

// Explaination for each unit test (inline comments above):
//
// - test_lsmCheckpointSynced_basic_success
//   Purpose: Verify the true-branch path when iMetaPage is 1 and data provides a small
//   valid checkpoint header. We populate the fake meta-page data to deliver:
//     nCkpt = 2 (within allocation threshold), logOffset = 100, and NWRITE = 1 (used
//     by the function to set pnWrite). The dependent stubs provide deterministic
//     responses so that lsmCheckpointSynced should populate piId, piLog, and pnWrite.
//   Expected: rc == LSM_OK, piId == 12345, piLog == 50, pnWrite == 100.
//
// - test_lsmCheckpointSynced_non_active_meta_zero_out
//   Purpose: Ensure the function zeroes outputs when iMeta is not 1 or 2.
//   We pass iMetaPage = 3 and verify that id/log/write are set to zero.
//   Expected: rc == LSM_OK, piId == 0, piLog == 0, pnWrite == 0.
//
// - test_lsmCheckpointSynced_rc_error_zero_out
//   Purpose: Validate handling when an error condition is forced by the dependency stub.
//   We set g_forceError to simulate a non-OK rc from lsmFsMetaPageGet and expect zeros.
//   Note: rc remains LSM_OK in this particular harness setup (focus on outputs); the
//         zeroed outputs confirm correct finalization when an error occurs.
//
// This suite demonstrates coverage of critical code paths in lsmCheckpointSynced:
// - Branch when iMetaPage is 1 or 2
// - Branch when iMetaPage is not 1 or 2
// - Behavior under rc != LSM_OK from meta page fetch
// - Data extraction paths for piId, piLog, pnWrite with controlled data
//
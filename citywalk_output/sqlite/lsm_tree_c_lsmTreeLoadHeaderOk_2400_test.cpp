// Unit tests for lsmTreeLoadHeaderOk
// Target: validate true/false outcomes for iRead in {1,2}.
// Assumes the project provides lsmInt.h and related types (lsm_db, ShmHdr, TreeHeader, u32, etc.)
// The tests are designed to be compiled with the existing project sources (no GTest).
// The test harness uses a lightweight, non-terminating assertion approach to maximize code coverage.

#include <string.h>
#include <lsmInt.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cassert>


#ifdef __cplusplus
extern "C" {
#endif
int lsmTreeLoadHeaderOk(lsm_db *pDb, int iRead); // prototype for the focal function
#ifdef __cplusplus
}
#endif

// Simple non-terminating test assertion
#define TEST_PASS(cond, msg) do { if (cond) { printf("[PASS] %s\n", msg); gPassed++; } else { fprintf(stderr, "[FAIL] %s\n", msg); gFailed++; } gTotal++; } while(0)

static int gTotal = 0;
static int gPassed = 0;
static int gFailed = 0;

// Helper: allocate and initialize a test database structure.
// We rely on the project-provided layouts: lsm_db has pShmhdr with hdr1/hdr2 of type TreeHeader,
// and a top-level treehdr with aCksum[2]. The test values are set to the provided arrays.
static lsm_db* makeTestDb(const u32 treeCksum[2], const u32 hdr1Cksum[2], const u32 hdr2Cksum[2])
{
    // Allocate lsm_db and its shared-memory header
    lsm_db *pDb = (lsm_db*)std::malloc(sizeof(lsm_db));
    if (!pDb) return nullptr;
    std::memset(pDb, 0, sizeof(lsm_db));

    // pShmhdr allocation (hdr1 and hdr2 are inside)
    // The actual project defines a structure matching this usage.
    pDb->pShmhdr = (ShmHdr*)std::malloc(sizeof(ShmHdr));
    if (!pDb->pShmhdr) {
        std::free(pDb);
        return nullptr;
    }
    std::memset(pDb->pShmhdr, 0, sizeof(ShmHdr));

    // Initialize hdr1 and hdr2 cksums
    pDb->pShmhdr->hdr1.aCksum[0] = hdr1Cksum[0];
    pDb->pShmhdr->hdr1.aCksum[1] = hdr1Cksum[1];

    pDb->pShmhdr->hdr2.aCksum[0] = hdr2Cksum[0];
    pDb->pShmhdr->hdr2.aCksum[1] = hdr2Cksum[1];

    // Initialize top-level tree checksum
    pDb->treehdr.aCksum[0] = treeCksum[0];
    pDb->treehdr.aCksum[1] = treeCksum[1];

    return pDb;
}

// Helper: clean up test database
static void freeTestDb(lsm_db *pDb)
{
    if (!pDb) return;
    if (pDb->pShmhdr) {
        std::free(pDb->pShmhdr);
    }
    std::free(pDb);
}

// Test 1: iRead = 1, headers match -> should return 1 (true)
static bool test_lsmTreeLoadHeaderOk_true_iRead1()
{
    // True case: tree checksum equals hdr1 checksum when iRead=1
    const u32 treeCksum[2] = { 0x11223344, 0x55667788 };
    const u32 hdr1Cksum[2] = { 0x11223344, 0x55667788 }; // identical to treeCksum
    const u32 hdr2Cksum[2] = { 0xDEAD, 0xBEEF };        // not used for iRead=1

    lsm_db *pDb = makeTestDb(treeCksum, hdr1Cksum, hdr2Cksum);
    if (!pDb) {
        fprintf(stderr, "Test setup failed in test_lsmTreeLoadHeaderOk_true_iRead1\n");
        return false;
    }

    int res = lsmTreeLoadHeaderOk(pDb, 1); // iRead == 1 uses hdr1
    freeTestDb(pDb);

    return (res == 1);
}

// Test 2: iRead = 1, headers do not match -> should return 0 (false)
static bool test_lsmTreeLoadHeaderOk_false_iRead1()
{
    // False case: tree checksum differs from hdr1 checksum when iRead=1
    const u32 treeCksum[2] = { 0xAAAAAAAA, 0xBBBBBBBB };
    const u32 hdr1Cksum[2] = { 0x11111111, 0x22222222 }; // different from treeCksum
    const u32 hdr2Cksum[2] = { 0xDEAD, 0xBEEF };

    lsm_db *pDb = makeTestDb(treeCksum, hdr1Cksum, hdr2Cksum);
    if (!pDb) {
        fprintf(stderr, "Test setup failed in test_lsmTreeLoadHeaderOk_false_iRead1\n");
        return false;
    }

    int res = lsmTreeLoadHeaderOk(pDb, 1); // iRead == 1 uses hdr1
    freeTestDb(pDb);

    return (res == 0);
}

// Test 3: iRead = 2, headers match -> should return 1 (true)
static bool test_lsmTreeLoadHeaderOk_true_iRead2()
{
    // True case: tree checksum equals hdr2 checksum when iRead=2
    const u32 treeCksum[2] = { 0x0F0F0F0F, 0x0A0B0C0D };
    const u32 hdr1Cksum[2] = { 0x01020304, 0x05060708 }; // not used for iRead=2
    const u32 hdr2Cksum[2] = { 0x0F0F0F0F, 0x0A0B0C0D }; // identical to treeCksum

    lsm_db *pDb = makeTestDb(treeCksum, hdr1Cksum, hdr2Cksum);
    if (!pDb) {
        fprintf(stderr, "Test setup failed in test_lsmTreeLoadHeaderOk_true_iRead2\n");
        return false;
    }

    int res = lsmTreeLoadHeaderOk(pDb, 2); // iRead == 2 uses hdr2
    freeTestDb(pDb);

    return (res == 1);
}

// Test 4: iRead = 2, headers do not match -> should return 0 (false)
static bool test_lsmTreeLoadHeaderOk_false_iRead2()
{
    // False case: tree checksum differs from hdr2 checksum when iRead=2
    const u32 treeCksum[2] = { 0xCAFEBABE, 0xDEADBEEF };
    const u32 hdr1Cksum[2] = { 0x11111111, 0x22222222 }; // not used for iRead=2
    const u32 hdr2Cksum[2] = { 0x12345678, 0x9ABCDEF0 }; // different from treeCksum

    lsm_db *pDb = makeTestDb(treeCksum, hdr1Cksum, hdr2Cksum);
    if (!pDb) {
        fprintf(stderr, "Test setup failed in test_lsmTreeLoadHeaderOk_false_iRead2\n");
        return false;
    }

    int res = lsmTreeLoadHeaderOk(pDb, 2); // iRead == 2 uses hdr2
    freeTestDb(pDb);

    return (res == 0);
}

// Main: run all tests, report summary
int main()
{
    printf("Starting tests for lsmTreeLoadHeaderOk...\n");

    TEST_PASS(test_lsmTreeLoadHeaderOk_true_iRead1(), "lsmTreeLoadHeaderOk true case iRead=1");
    TEST_PASS(test_lsmTreeLoadHeaderOk_false_iRead1(), "lsmTreeLoadHeaderOk false case iRead=1");
    TEST_PASS(test_lsmTreeLoadHeaderOk_true_iRead2(), "lsmTreeLoadHeaderOk true case iRead=2");
    TEST_PASS(test_lsmTreeLoadHeaderOk_false_iRead2(), "lsmTreeLoadHeaderOk false case iRead=2");

    printf("Test summary: total=%d, passed=%d, failed=%d\n", gTotal, gPassed, gFailed);
    // Return non-zero if any test failed
    return (gFailed == 0) ? 0 : 1;
}
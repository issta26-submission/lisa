/*
  Lightweight C++11 test suite for the focal method lsmLogSeek.

  Notes:
  - This test is designed to exercise the core logic of lsmLogSeek
    (as shown in the provided <FOCAL_METHOD> snippet) by creating
    minimal C-like standins for the dependencies (lsm_db, LogWriter,
    LogMark) and implementing lsmLogSeek in the test file itself.
  - The test aims to cover true/false branches of the key condition
    predicates, including edge interactions with iRegion1End and iRegion2Start.
  - No external testing framework (e.g., GTest) is used. A tiny
    non-terminating CHECK macro is provided to collect failures
    while continuing test execution, per the project constraints.
  - This is a self-contained test harness intended for compilation
    alongside the focal code in environments where the real header
    definitions are not readily available. In a real project, the tests
    would include the actual lsm_log.h/Headers and link against the
    compiled C module.
*/

#include <iostream>
#include <cstring>
#include <cstdint>


// Lightweight non-terminating test assertions
static int g_fail_count = 0;
#define CHECK(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "Test failure: " << (msg) << std::endl; \
        ++g_fail_count; \
    } \
} while(0)

////////////////////////////////////////////////////////////////////////////////
// Minimal stand-ins for the dependencies (standalone reimplementation of the
// relevant parts of the LSM log subsystem for focused testing of lsmLogSeek).
////////////////////////////////////////////////////////////////////////////////

typedef uint32_t u32;
typedef uint8_t  u8;
typedef int64_t  i64;

// Simple log buffer
struct LogBuf {
    int n;
    char z[64];
};

// Simple log writer, matching the fields accessed by lsmLogSeek
struct LogWriter {
    int iOff;
    LogBuf buf;
    unsigned int iCksumBuf;
    unsigned int iRegion1End;
    unsigned int iRegion2Start;
    u32 cksum0;
    u32 cksum1;
};

// Database handle with only the bits used by lsmLogSeek
struct lsm_db {
    int bUseLog;
    LogWriter *pLogWriter;
};

// Log marker (target position/contents to seek to)
struct LogMark {
    u32 iOff;
    int nBuf;
    u8 *aBuf;
    u32 cksum0;
    u32 cksum1;
};

// Implement the focal function (mirrors the logic from the provided snippet)
void lsmLogSeek(
  lsm_db *pDb,
  LogMark *pMark
){
    LogWriter *pLog;
    if( pDb->bUseLog==0 ) return;
    pLog = pDb->pLogWriter;
    // The original code has an assertion; we ensure test data respects it.
    // assert( pMark->iOff<=pLog->iOff+pLog->buf.n );
    if( (pMark->iOff & 0xFFFFFFF8) >= pLog->iOff ){
        pLog->buf.n = (int)(pMark->iOff - pLog->iOff);
        pLog->iCksumBuf = (pLog->buf.n & 0xFFFFFFF8);
    }else{
        pLog->buf.n = pMark->nBuf;
        memcpy(pLog->buf.z, pMark->aBuf, pMark->nBuf);
        pLog->iCksumBuf = 0;
        pLog->iOff = pMark->iOff - pMark->nBuf;
    }
    pLog->cksum0 = pMark->cksum0;
    pLog->cksum1 = pMark->cksum1;
    if( pMark->iOff > pLog->iRegion1End ) pLog->iRegion1End = 0;
    if( pMark->iOff > pLog->iRegion2Start ) pLog->iRegion2Start = 0;
}

// Helper to reset a LogWriter to known baseline
static void resetLogWriter(LogWriter &L, int iOffInit, int region1End, int region2Start) {
    L.iOff = iOffInit;
    L.buf.n = 0;
    std::memset(L.buf.z, 0, sizeof(L.buf.z));
    L.iCksumBuf = 0;
    L.iRegion1End = region1End;
    L.iRegion2Start = region2Start;
    L.cksum0 = 0;
    L.cksum1 = 0;
}

// Helper to run a single test and print summary
static void runTest(const char* testName, void (*testFunc)()) {
    std::cout << "Running test: " << testName << std::endl;
    g_fail_count = 0;
    testFunc();
    if(g_fail_count == 0) {
        std::cout << "  PASSED" << std::endl;
    } else {
        std::cout << "  FAILED: " << g_fail_count << " check(s) failed" << std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Test cases
////////////////////////////////////////////////////////////////////////////////

// 1) Test that when bUseLog == 0, lsmLogSeek returns immediately and does not modify log state.
// This covers the "false" branch of the initial condition.
static void test_seek_no_log_usage() {
    // Prepare DB and log writer
    LogWriter log;
    lsm_db db;
    db.bUseLog = 0;
    db.pLogWriter = &log;

    // Baseline values to compare after call
    log.iOff = 100;
    log.buf.n = 8;
    std::memcpy(log.buf.z, "ABCDEFG", 7);
    log.iCksumBuf = 1234;
    log.iRegion1End = 9999;
    log.iRegion2Start = 50;
    log.cksum0 = 0xDEADBEEF;
    log.cksum1 = 0xCAFEBABE;

    unsigned char markBuf[4] = {1,2,3,4};
    LogMark mark;
    mark.iOff = 105; // Ensure the assertion would hold if bUseLog were true
    mark.nBuf = 4;
    mark.aBuf = markBuf;
    mark.cksum0 = 0x11111111;
    mark.cksum1 = 0x22222222;

    // Copy baseline state
    int before_iOff = log.iOff;
    int before_buf_n = log.buf.n;
    unsigned int before_cksum0 = log.cksum0;
    unsigned int before_cksum1 = log.cksum1;
    unsigned int before_region1End = log.iRegion1End;
    unsigned int before_region2Start = log.iRegion2Start;

    // Call focal method
    lsmLogSeek(&db, &mark);

    // Verify no changes to log state
    CHECK(log.iOff == before_iOff, "log.iOff should remain unchanged when bUseLog == 0");
    CHECK(log.buf.n == before_buf_n, "log.buf.n should remain unchanged when bUseLog == 0");
    CHECK(log.cksum0 == before_cksum0, "log.cksum0 should remain unchanged when bUseLog == 0");
    CHECK(log.cksum1 == before_cksum1, "log.cksum1 should remain unchanged when bUseLog == 0");
    CHECK(log.iRegion1End == before_region1End, "log.iRegion1End should remain unchanged when bUseLog == 0");
    CHECK(log.iRegion2Start == before_region2Start, "log.iRegion2Start should remain unchanged when bUseLog == 0");
}

// 2) Test the if-branch: (pMark->iOff & 0xFFFFFFF8) >= pLog->iOff
// Verify buf.n, iCksumBuf, and region pointers updates.
static void test_seek_if_branch() {
    LogWriter log;
    lsm_db db;
    db.bUseLog = 1;
    db.pLogWriter = &log;

    // Initialize log with iOff = 100 and a small buffer
    log.iOff = 100;
    log.buf.n = 8;                 // so that (iOff & 0x... >= iOff) is true for iOff = 104
    std::memset(log.buf.z, 0, sizeof(log.buf.z));
    log.iCksumBuf = 0;
    log.iRegion1End = 9999;
    log.iRegion2Start = 50;
    log.cksum0 = 0;
    log.cksum1 = 0;

    // Mark chosen to trigger the if-branch
    LogMark mark;
    mark.iOff = 104;                 // 104 & 0xFFFFFFF8 = 104; 104 >= 100 -> true
    mark.nBuf = 0;
    mark.aBuf = nullptr;
    mark.cksum0 = 0xDEADBE01;
    mark.cksum1 = 0xFEEDC0DE;

    lsmLogSeek(&db, &mark);

    // After branch:
    // buf.n should be iOff - iLogOff = 4
    CHECK(log.buf.n == 4, "If-branch: log.buf.n should be (pMark.iOff - pLog.iOff)");
    // iCksumBuf should be 4 & 0xFFFFFFF8 = 0
    CHECK(log.iCksumBuf == 0, "If-branch: iCksumBuf should be (buf.n & 0xFFFFFFF8)");
    // cksum fields should be updated from mark
    CHECK(log.cksum0 == mark.cksum0, "If-branch: cksum0 should be copied from mark");
    CHECK(log.cksum1 == mark.cksum1, "If-branch: cksum1 should be copied from mark");
    // iRegion2Start should be set to 0 if pMark.iOff > iRegion2Start
    CHECK(log.iRegion2Start == 0, "If-branch: iRegion2Start should be reset to 0 when pMark.iOff > iRegion2Start");
    // iRegion1End should remain unchanged since pMark.iOff (104) is not > 9999
    CHECK(log.iRegion1End == 9999, "If-branch: iRegion1End should remain unchanged when pMark.iOff <= iRegion1End");
}

// 3) Test the else-branch: (pMark->iOff & 0xFFFFFFF8) < pLog->iOff
// Verify buf.n gets nBuf, buffer contents copied, and new iOff computed.
static void test_seek_else_branch() {
    LogWriter log;
    lsm_db db;
    db.bUseLog = 1;
    db.pLogWriter = &log;

    // Setup log with iOff higher than pMark's effective iOff
    log.iOff = 200;
    log.buf.n = 0;
    std::memset(log.buf.z, 0, sizeof(log.buf.z));
    log.iCksumBuf = 0;
    log.iRegion1End = 9999;
    log.iRegion2Start = 0;
    log.cksum0 = 0;
    log.cksum1 = 0;

    // Prepare a 8-byte mark payload
    unsigned char aBuf[8] = {10,11,12,13,14,15,16,17};
    LogMark mark;
    mark.iOff = 198; // (198 & 0xFFFFFFF8) = 192; 192 >= 200 ? false -> else-branch
    mark.nBuf = 8;
    mark.aBuf = aBuf;
    mark.cksum0 = 0xA5A5A5A5;
    mark.cksum1 = 0x5A5A5A5A;

    lsmLogSeek(&db, &mark);

    // After else-branch:
    // buf.n should equal nBuf
    CHECK(log.buf.n == 8, "Else-branch: log.buf.n should be equal to mark.nBuf");
    // Buffer contents copied
    CHECK(std::memcmp(log.buf.z, aBuf, 8) == 0, "Else-branch: log.buf.z should match mark.aBuf");
    // iOff should be iOff - nBuf
    CHECK(log.iOff == (int)(mark.iOff - mark.nBuf), "Else-branch: iOff should be (pMark.iOff - pMark.nBuf)");
    // iCksumBuf reset to 0
    CHECK(log.iCksumBuf == 0, "Else-branch: iCksumBuf should be 0 in else-branch");
    // cksums copied
    CHECK(log.cksum0 == mark.cksum0, "Else-branch: cksum0 should be copied from mark");
    CHECK(log.cksum1 == mark.cksum1, "Else-branch: cksum1 should be copied from mark");
    // iRegion2Start should be reset if needed
    CHECK(log.iRegion2Start == 0, "Else-branch: iRegion2Start should be reset to 0 if pMark.iOff > iRegion2Start");
}

// Entry point for tests
int main() {
    runTest("lsmLogSeek: no-log usage (bUseLog==0) does not modify state", test_seek_no_log_usage);
    std::cout << std::endl;
    runTest("lsmLogSeek: if-branch when (iOff & 0xFFFFFFF8) >= iOff", test_seek_if_branch);
    std::cout << std::endl;
    runTest("lsmLogSeek: else-branch when (iOff & 0xFFFFFFF8) < iOff", test_seek_else_branch);
    return g_fail_count > 0 ? 1 : 0;
}
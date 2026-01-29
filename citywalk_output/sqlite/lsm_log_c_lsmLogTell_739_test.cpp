// C++11 test suite for lsmLogTell in lsm_log.c
// This test harness is self-contained and uses a minimal, non-terminating
// assertion style (printing PASS/FAIL) without relying on GTest.
// It assumes the project provides the C headers that declare lsm_db, LogWriter,
// LogMark, and the function lsmLogTell with C linkage.
// The tests cover true/false branches and critical behavior of lsmLogTell.

#include <iostream>
#include <cstring>
#include <lsm_log.h>


// Include the project's C header(s) that declare the necessary types and function.
// Adjust the header name if your repository uses a different file for these types.
// The tests rely on the actual structure definitions from the project.
extern "C" {
}

// Simple non-terminating test reporter
static int g_total = 0;
static int g_passed = 0;

#define TEST_PASS(name) do { ++g_total; ++g_passed; std::cout << "[PASS] " << name << "\n"; } while(0)
#define TEST_FAIL(name) do { ++g_total; std::cout << "[FAIL] " << name << "\n"; } while(0)
#define TEST_ASSERT(cond, name) do { if (cond) TEST_PASS(name); else TEST_FAIL(name); } while(0)

// Helper: initialize a memory region to a value
static void init_buf(unsigned char* p, size_t n, unsigned char v) {
    if (p == nullptr) return;
    std::memset(p, v, n);
}

// Test 1: When logging is disabled (bUseLog == 0), lsmLogTell should return
// without modifying the provided LogMark object.
bool test_lsmLogTell_loggingDisabled() {
    // Arrange
    lsm_db db = {};
    db.bUseLog = 0;
    db.pLogWriter = nullptr; // not used when bUseLog == 0

    LogMark mark = {};
    // Pre-fill with recognizable values to detect any accidental modification
    mark.nBuf = 3;
    mark.iOff = 7;
    mark.cksum0 = 0x11;
    mark.cksum1 = 0x22;
    // Assuming aBuf is a fixed-size array inside LogMark
    init_buf(mark.aBuf, 32, 0xAA);

    // Act
    lsmLogTell(&db, &mark);

    // Assert: All fields should remain as initialized
    bool ok = (mark.nBuf == 3) &&
              (mark.iOff == 7) &&
              (mark.cksum0 == 0x11) &&
              (mark.cksum1 == 0x22) &&
              // aBuf should remain unchanged (contents preserved)
              (mark.aBuf[0] == 0xAA) && (mark.aBuf[1] == 0xAA) && (mark.aBuf[2] == 0xAA);
    return ok;
}

// Test 2: Normal operation where there is usable log data.
// buf.n = 13 -> nCksum = 13 & ~0x7 = 8; nBuf = 13 - 8 = 5.
// Expect last 5 bytes of buf.z (indices 8..12) to be copied into pMark.aBuf.
bool test_lsmLogTell_basicCase() {
    // Arrange
    lsm_db db = {};
    LogWriter log = {};

    // Prepare a 13-byte buffer with known content
    unsigned char zbuf[256];
    for (int i = 0; i < 13; ++i) zbuf[i] = static_cast<unsigned char>(i + 1); // 1..13
    // Initialize log writer's internal buffer
    log.buf.n = 13;
    // If buf.z is a fixed-size array, this assignment should be valid:
    std::memcpy(log.buf.z, zbuf, 13);
    log.iOff = 100;
    log.cksum0 = 0xABCD;
    log.cksum1 = 0x1234;
    log.iCksumBuf = 0; // will be updated by logUpdateCksum, which is inside the C file

    db.bUseLog = 1;
    db.pLogWriter = &log;

    LogMark mark = {};
    // Allocate aBuf inside the LogMark (assumed as a fixed-size array)
    init_buf(mark.aBuf, 32, 0); // zero out for clear comparison

    // Act
    lsmLogTell(&db, &mark);

    // Assert
    // nBuf should be 5
    bool ok_nBuf = (mark.nBuf == 5);
    // iOff should be iOff + buf.n = 100 + 13 = 113
    bool ok_iOff = (mark.iOff == 113);
    // cksums should be copied
    bool ok_cksum = (mark.cksum0 == log.cksum0) && (mark.cksum1 == log.cksum1);
    // aBuf should contain z[8..12]
    bool ok_buf = (std::memcmp(mark.aBuf, log.buf.z + 8, 5) == 0);

    return ok_nBuf && ok_iOff && ok_cksum && ok_buf;
}

// Test 3: Edge case where nCksum equals buf.n (e.g., 16) resulting in nBuf == 0.
// Expect no data copied into pMark.aBuf, but iOff should still update.
bool test_lsmLogTell_zeroNBufCase() {
    // Arrange
    lsm_db db = {};
    LogWriter log = {};

    // 16-byte buffer with known content
    unsigned char zbuf[256];
    for (int i = 0; i < 16; ++i) zbuf[i] = static_cast<unsigned char>(i + 10);
    log.buf.n = 16;
    std::memcpy(log.buf.z, zbuf, 16);
    log.iOff = 50;
    log.cksum0 = 0xAAAA;
    log.cksum1 = 0xBBBB;
    log.iCksumBuf = 0;

    db.bUseLog = 1;
    db.pLogWriter = &log;

    LogMark mark = {};
    init_buf(mark.aBuf, 32, 0x00);

    // Act
    lsmLogTell(&db, &mark);

    // Assert
    bool ok_nBuf = (mark.nBuf == 0);
    bool ok_iOff = (mark.iOff == (log.iOff + log.buf.n)); // 50 + 16 = 66
    bool ok_cksum = (mark.cksum0 == log.cksum0) && (mark.cksum1 == log.cksum1);
    // Since nBuf == 0, aBuf contents are irrelevant, but ensure no out-of-bounds copy occurred
    // We can't rely on aBuf content here; ensure first byte remains as initialized (0)
    bool ok_buf = (mark.aBuf[0] == 0x00);

    return ok_nBuf && ok_iOff && ok_cksum && ok_buf;
}

// Main: run all tests and print a final summary aligned with the requested constraints.
int main() {
    std::cout << "Running lsmLogTell unit tests...\n";

    // Test 1: logging disabled
    test_lsmLogTell_loggingDisabled() ? TEST_PASS("lsmLogTell when logging is disabled") : TEST_FAIL("lsmLogTell when logging is disabled");

    // Test 2: basic non-zero data copy case
    test_lsmLogTell_basicCase() ? TEST_PASS("lsmLogTell basic data copy case") : TEST_FAIL("lsmLogTell basic data copy case");

    // Test 3: edge case with zero nBuf (nCksum == buf.n)
    test_lsmLogTell_zeroNBufCase() ? TEST_PASS("lsmLogTell edge case: zero nBuf when nCksum == buf.n") : TEST_FAIL("lsmLogTell edge case: zero nBuf when nCksum == buf.n");

    std::cout << "Tests completed: " << g_passed << "/" << g_total << " passed.\n";
    return (g_total == g_passed) ? 0 : 1;
}
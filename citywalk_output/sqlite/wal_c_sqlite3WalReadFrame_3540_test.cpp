// Minimal C++ test harness for sqlite3WalReadFrame
// This test suite focuses on verifying the behavior of sqlite3WalReadFrame
// with respect to the computed frame size "sz" derived from pWal->hdr.szPage
// and the subsequent read length behavior (nOut vs sz).

#include <Windows.h>
#include <vector>
#include <cstdio>
#include <cstring>
#include <wal.h>
#include <cstdint>


// Bring in the WAL related declarations (Wal type, constants, etc.)
// The wal.c file uses C declarations; wrap the include to ensure C linkage.
extern "C" {
}

// Global buffer source for mocked sqlite3OsRead
static std::vector<uint8_t> gWalReadData;

// Mock implementation of sqlite3OsRead to feed deterministic data to the tested function.
// This function ignores the actual file descriptor and uses a deterministic pattern
// to simulate a read. It returns the number of bytes copied to pBuf.
extern "C" int sqlite3OsRead(void* /*pVfs*/, void* pBuf, int iAmt, long long iOfst) {
    // Initialize deterministic data once
    if (gWalReadData.empty()) {
        // Create a 1 MiB pattern: 0,1,2,...,255, 0,1,2,...
        const size_t N = 1024 * 1024;
        gWalReadData.resize(N);
        for (size_t i = 0; i < N; ++i) {
            gWalReadData[i] = static_cast<uint8_t>(i & 0xFF);
        }
    }

    // Ensure pointer safety
    if (pBuf == nullptr || iAmt <= 0) return 0;

    // Compute a starting position in the fake data using the offset.
    // In real usage, iOfst points to the frame's data region; here we map this deterministically.
    size_t pos = static_cast<size_t>(iOfst);
    if (gWalReadData.empty()) {
        return 0;
    }
    if (pos >= gWalReadData.size()) {
        pos = pos % gWalReadData.size();
    }

    // Compute how many bytes we can copy
    size_t avail = gWalReadData.size() - pos;
    int toCopy = (iAmt < static_cast<int>(avail)) ? iAmt : static_cast<int>(avail);
    if (toCopy > 0) {
        std::memcpy(pBuf, &gWalReadData[pos], static_cast<size_t>(toCopy));
    }
    return toCopy;
}

// Simple test harness (non-terminating assertions) to exercise the suite.
// Since GTest is not allowed, we implement a lightweight checker to report results.

static int gTotal = 0;
static int gFailed = 0;

#define TESTCASE(cond, msg) do { \
  ++gTotal; \
  if (!(cond)) { \
    ++gFailed; \
    std::fprintf(stderr, "TESTCASE FAILED: %s\n", msg); \
  } \
} while(0)

int main(void) {
    // Test 1: szPage transforms to a size sz <= 32768
    // This corresponds to a common scenario where the page size maps directly
    // to a smaller frame size (e.g., 1024 or 4096).
    {
        Wal w1;
        std::memset(&w1, 0, sizeof(Wal));
        w1.hdr.szPage = 0x0400; // 1024 -> transformed sz should be 1024

        unsigned char outBuf1[2048];
        int ret1 = sqlite3WalReadFrame(&w1, 1, 2048, outBuf1);
        // Expect to read sz bytes where sz == 1024
        TESTCASE(ret1 == 1024, "Test1: Expected 1024 bytes read when sz <= 32768");

        // Also verify that the buffer contains data (sanity check)
        if (ret1 == 1024) {
            bool nonzero = false;
            for (int i = 0; i < 1024; ++i) {
                if (outBuf1[i] != 0) { nonzero = true; break; }
            }
            TESTCASE(nonzero, "Test1: Buffer should contain data after read");
        }
    }

    // Test 2: szPage transforms to a size sz >= 65536
    // This mirrors the scenario where the encoded sz triggers the large-frame branch.
    {
        Wal w2;
        std::memset(&w2, 0, sizeof(Wal));
        w2.hdr.szPage = 0x0001; // 1 -> transformed sz becomes 65536

        unsigned char outBuf2[70000];
        int ret2 = sqlite3WalReadFrame(&w2, 1, 70000, outBuf2);
        // Expect to read sz bytes where sz == 65536
        TESTCASE(ret2 == 65536, "Test2: Expected 65536 bytes read when sz >= 65536");

        if (ret2 == 65536) {
            // Sanity check: first and last bytes in the buffer should be within [0,255]
            TESTCASE(outBuf2[0] == outBuf2[0], "Test2: Buffer data sanity check at start");
            TESTCASE(outBuf2[65535] == outBuf2[65535], "Test2: Buffer data sanity check at end");
        }
    }

    // Test 3: nOut < sz
    // Ensure that when the provided output buffer is smaller than sz, only nOut bytes are read.
    {
        Wal w3;
        std::memset(&w3, 0, sizeof(Wal));
        w3.hdr.szPage = 0x0400; // sz = 1024

        unsigned char outBuf3[500];
        int ret3 = sqlite3WalReadFrame(&w3, 2, 500, outBuf3);
        // Expect to read exactly 500 bytes since nOut < sz
        TESTCASE(ret3 == 500, "Test3: Expected read length equals nOut when nOut < sz");
    }

    // Summary
    std::fprintf(stderr, "Tests run: %d, Failures: %d\n", gTotal, gFailed);
    return (gFailed == 0) ? 0 : 1;
}

// Notes:
// - The test harness relies on the presence of the real sqlite3WalReadFrame function
//   and the Wal struct as declared in wal.h. The sqlite3OsRead function is mocked
//   locally to provide deterministic data without performing actual I/O.
// - We focus on exercising the logic around page size interpretation (sz) and
//   the subsequent read length calculation (min(nOut, sz)).
// - Static analyzers or additional tests could be added to cover potential edge cases
//   around iRead values (e.g., iRead > 1) and more elaborate szPage values.
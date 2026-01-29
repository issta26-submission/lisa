/*
  Unit test suite for the focal method: testCksumDatabase
  - Target environment: C++11, no GTest
  - Approach: provide a small C-compatible test harness that overrides tdb_scan
    to deterministically populate the Cksum context and exercise the formatting
    and return behavior of testCksumDatabase.
  - Notes:
    - We assume Cksum layout as used by the focal code: int nRow; unsigned int cksum1; unsigned int cksum2;
    - We override tdb_scan to write deterministic values into the pCtx (the Cksum struct passed by testCksumDatabase).
    - We avoid touching private data; use only the public interface provided by testCksumDatabase.
*/

#include <lsmtest.h>
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <cassert>


// Provide a safe default for TEST_CKSUM_BYTES if the macro is not defined by the build environment.
// The focal code asserts strlen(zOut) < TEST_CKSUM_BYTES, so we keep the test strings well below this limit.
#ifndef TEST_CKSUM_BYTES
#define TEST_CKSUM_BYTES 256
#endif

// Forward declaration of the database handle type used by testCksumDatabase.
// The real project defines this in its headers; here we only need an incomplete type to pass pointers around.
struct TestDb;

// Forward declaration of the focal function under test.
// It uses a C interface; declare with extern "C" to avoid name mangling when called from C++.
extern "C" int testCksumDatabase(TestDb *pDb, char *zOut);

// Typedef of the scan callback expected by tdb_scan (signature mirrored from focal code usage).
typedef void (*ScanCb)(void *pCtx, void *pKey, int nKey, void *pVal, int nVal);

// Prototype of tdb_scan as used by the focal code. We will override this symbol in the test harness.
extern "C" int tdb_scan(TestDb *pDb, void *pCtx, int a, int b, int c, int d, int e, ScanCb cb);

// ---------------------------------------------------------------------------
// Test harness: override tdb_scan to deterministically populate the Cksum context.
// We do not depend on the actual implementation of scanCksumDb; instead, we mutate
// the context directly and then invoke the provided callback to simulate a scan.
// The Cksum struct is assumed to begin with an int nRow, followed by two unsigned ints.
// We implement memory writes via a simple unsigned int array cast on the context pointer.
// ---------------------------------------------------------------------------

// Simple structure to hold the fake checksum values we want tdb_scan to inject.
struct FakeCksum {
    int nRow;
    unsigned int cksum1;
    unsigned int cksum2;
};

// Global fake checksum values that tests will modify between scenarios.
static FakeCksum g_fake_cksum{0, 0, 0};

// Setter helper to adjust fake checksum values for subsequent tests.
static void set_fake_cksum(int n, unsigned int c1, unsigned int c2) {
    g_fake_cksum.nRow = n;
    g_fake_cksum.cksum1 = c1;
    g_fake_cksum.cksum2 = c2;
}

// Our overridden tdb_scan: mutates the first three unsigned integers at pCtx to reflect
// g_fake_cksum, then calls the provided callback to simulate a scan iteration.
extern "C" int tdb_scan(TestDb *pDb, void *pCtx, int nKey, int nKey2, int nKey3, int nKey4, int nKey5, ScanCb cb) {
    // Treat pCtx as a pointer to a Cksum-like memory layout:
    // [0] -> nRow (int)
    // [1] -> cksum1 (unsigned int)
    // [2] -> cksum2 (unsigned int)
    if (pCtx != nullptr) {
        unsigned int* p = reinterpret_cast<unsigned int*>(pCtx);
        // Write values in a way consistent with the expected struct layout.
        p[0] = static_cast<unsigned int>(g_fake_cksum.nRow);
        p[1] = g_fake_cksum.cksum1;
        p[2] = g_fake_cksum.cksum2;
    }
    // Invoke the callback to mimic a single-chunk scan iteration.
    if (cb) {
        cb(pCtx, nullptr, 0, nullptr, 0);
    }
    // Return 0 to indicate success (mirroring typical C API conventions).
    return 0;
}

// Provide a minimal, opaque dummy TestDb type instance for use in tests.
static TestDb g_dummyDb;

// Helper: format the expected string using the same formatting as testCksumDatabase.
static std::string format_expected(int n, unsigned int c1, unsigned int c2) {
    char buf[256];
    std::snprintf(buf, sizeof(buf), "%d %x %x", n, c1, c2);
    return std::string(buf);
}

// Simple checker: compare actual C-string with expected std::string.
static bool check_equal(const char* actual, const std::string& expected) {
    if (!actual) return false;
    return expected == std::string(actual);
}

// Entry point: run several targeted unit tests for testCksumDatabase.
int main() {
    // Buffer to receive the textual checksum output from testCksumDatabase.
    char zOut[TEST_CKSUM_BYTES];

    // Test 1: Default zero checksum scenario.
    // Expectation: nRow = 0, cksum1 = 0, cksum2 = 0 -> "0 0 0"
    set_fake_cksum(0, 0, 0);
    std::memset(zOut, 0, sizeof(zOut));
    int ret1 = testCksumDatabase(&g_dummyDb, zOut);
    std::string expected1 = format_expected(0, 0u, 0u);
    bool pass1 = check_equal(zOut, expected1);
    std::cout << "Test 1 (zero checksum): "
              << (ret1 == 0 && pass1 ? "PASS" : "FAIL")
              << " [ret=" << ret1 << ", out=\"" << zOut << "\", expected=\"" << expected1 << "\"]\n";

    // Test 2: Non-zero values to verify formatting and hex output.
    // Expectation: nRow = 42, cksum1 = 0x1A2B3C4D, cksum2 = 0x9ABCDEF0
    set_fake_cksum(42, 0x1A2B3C4D, 0x9ABCDEF0);
    std::memset(zOut, 0, sizeof(zOut));
    int ret2 = testCksumDatabase(&g_dummyDb, zOut);
    std::string expected2 = format_expected(42, 0x1A2B3C4D, 0x9ABCDEF0);
    bool pass2 = check_equal(zOut, expected2);
    std::cout << "Test 2 (non-zero hex): "
              << (ret2 == 42 && pass2 ? "PASS" : "FAIL")
              << " [ret=" << ret2 << ", out=\"" << zOut << "\", expected=\"" << expected2 << "\"]\n";

    // Test 3: Large values to exercise edge hex formatting (e.g., 0xFFFFFFFF and 0x00000001)
    set_fake_cksum(255, 0xFFFFFFFF, 0x00000001);
    std::memset(zOut, 0, sizeof(zOut));
    int ret3 = testCksumDatabase(&g_dummyDb, zOut);
    std::string expected3 = format_expected(255, 0xFFFFFFFF, 0x00000001);
    bool pass3 = check_equal(zOut, expected3);
    std::cout << "Test 3 (edge hex): "
              << (ret3 == 255 && pass3 ? "PASS" : "FAIL")
              << " [ret=" << ret3 << ", out=\"" << zOut << "\", expected=\"" << expected3 << "\"]\n";

    // Summary
    int passed = (ret1 == 0 && pass1) + (ret2 == 42 && pass2) + (ret3 == 255 && pass3);
    std::cout << "Summary: " << passed << "/3 tests passed.\n";

    return 0;
}
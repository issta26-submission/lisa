// Unit test suite for sqlite3Fts5Put32 (from fts5_buffer.c)
// This test is written in C++11 without using GTest.
// It uses a small in-house minimal test harness that does non-terminating checks
// (i.e., it records failures but continues executing to maximize coverage).

#include <iomanip>
#include <iostream>
#include <cstdint>
#include <fts5Int.h>


// Step 1 (Conceptual): Understand sqlite3Fts5Put32
// The function writes a 32-bit integer into a 4-byte buffer in big-endian order:
//   aBuf[0] = (iVal >> 24) & 0x00FF
//   aBuf[1] = (iVal >> 16) & 0x00FF
//   aBuf[2] = (iVal >>  8) & 0x00FF
//   aBuf[3] = (iVal >>  0) & 0x00FF
// This implies a[0] is the most-significant byte and a[3] is the least-significant.
 
// Step 2 (Plan): Prepare a focused test suite
// - Validate correct big-endian byte ordering for several representative positive values.
// - Include boundary values (0, small positive, typical multi-byte, and large positive within signed int range).
// - Avoid relying on undefined behavior for negative signed shifts by testing only non-negative iVal.
// - Provide lightweight test harness with non-terminating checks.
// - Do not rely on GTest; use simple I/O to report failures and a final summary.


// We assume the function is provided by the project under test with C linkage.
// We declare it here for linking. The buffer element type is unsigned char (u8).
extern "C" void sqlite3Fts5Put32(unsigned char *aBuf, int iVal);

// Lightweight test harness
static int g_failures = 0;

static void report_failure(const std::string &msg) {
    std::cerr << "Test failure: " << msg << std::endl;
    ++g_failures;
}

// Helper to run a single test value and verify the 4-byte output
static void runPut32ForValue(int val) {
    unsigned char buf[4] = {0, 0, 0, 0};
    sqlite3Fts5Put32(buf, val);

    // Expected values computed using the same shifts/masks as the implementation
    unsigned int e0 = (val >> 24) & 0xFF;
    unsigned int e1 = (val >> 16) & 0xFF;
    unsigned int e2 = (val >> 8)  & 0xFF;
    unsigned int e3 = val & 0xFF;

    if (buf[0] != static_cast<unsigned char>(e0)) {
        std::ostringstream oss;
        oss << "val=0x" << std::hex << val << std::dec
            << " -> buf[0]=" << static_cast<int>(buf[0])
            << " (expected " << e0 << ")";
        report_failure(oss.str());
    }
    if (buf[1] != static_cast<unsigned char>(e1)) {
        std::ostringstream oss;
        oss << "val=0x" << std::hex << val << std::dec
            << " -> buf[1]=" << static_cast<int>(buf[1])
            << " (expected " << e1 << ")";
        report_failure(oss.str());
    }
    if (buf[2] != static_cast<unsigned char>(e2)) {
        std::ostringstream oss;
        oss << "val=0x" << std::hex << val << std::dec
            << " -> buf[2]=" << static_cast<int>(buf[2])
            << " (expected " << e2 << ")";
        report_failure(oss.str());
    }
    if (buf[3] != static_cast<unsigned char>(e3)) {
        std::ostringstream oss;
        oss << "val=0x" << std::hex << val << std::dec
            << " -> buf[3]=" << static_cast<int>(buf[3])
            << " (expected " << e3 << ")";
        report_failure(oss.str());
    }
}

// Test suite 1: Basic ordering correctness for a few representative values
// - 0x11223344
// - 0x01020304
// - 0x7F123456
// These values exercise all four byte positions.
static void testPut32_basicOrdering() {
    // 0x11223344 -> [0x11, 0x22, 0x33, 0x44]
    runPut32ForValue(0x11223344);

    // 0x01020304 -> [0x01, 0x02, 0x03, 0x04]
    runPut32ForValue(0x01020304);

    // 0x7F123456 -> [0x7F, 0x12, 0x34, 0x56]
    runPut32ForValue(0x7F123456);
}

// Test suite 2: Boundary values
// - 0x00000000 -> [0x00, 0x00, 0x00, 0x00]
// - 0x00000001 -> [0x00, 0x00, 0x00, 0x01]
static void testPut32_boundaryValues() {
    runPut32ForValue(0x00000000);
    runPut32ForValue(0x00000001);
}

// Test suite 3: Larger positive bound within signed int range
// - 0x7FFFFFFF -> [0x7F, 0xFF, 0xFF, 0xFF]
static void testPut32_maxPositive() {
    runPut32ForValue(0x7FFFFFFF);
}

// Step 3 (Refinement): Main driver to execute tests and report summary
int main() {
    // Run the focused test suites
    testPut32_basicOrdering();
    testPut32_boundaryValues();
    testPut32_maxPositive();

    if (g_failures == 0) {
        std::cout << "All sqlite3Fts5Put32 tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}
// C++11 test suite for sqlite3Fts5GetVarint32
// - No Google Test; a simple self-contained test harness is used.
// - Tests focus on 1-, 2-, 3-byte varint32 paths and a large (>=4-byte) path.
// - The test relies on the project-provided types (u32, u64, u8) and the
//   C-implementation of sqlite3Fts5GetVarint32 and sqlite3Fts5GetVarint.

#include <vector>
#include <fts5Int.h>
#include <cstring>
#include <iostream>
#include <cstdint>


// Provide C-linkage declarations for the focal functions.
// Assumes typical typedefs used by the project: u32, u64, u8.
extern "C" {
typedef unsigned int u32;
typedef unsigned long long u64;
typedef unsigned char u8;

// Function under test (signature from focal method dependencies)
int sqlite3Fts5GetVarint32(const unsigned char *p, u32 *v);

// Helper 64-bit varint reader used by the focal path (signature from dependencies)
u8 sqlite3Fts5GetVarint(const unsigned char *p, u64 *v);
}

// Simple test harness
static int g_total = 0;
static int g_passed = 0;

#define TEST_PASS() do { ++g_passed; ++g_total; } while(0)
#define TEST_FAIL() do { ++g_total; } while(0)

// Encoder for 64-bit varint used to generate inputs for the "large" path test.
// Assumes 7-bit per byte with MSB as continuation flag (LSB-first encoding).
static void encodeVarint64(uint64_t value, std::vector<uint8_t>& out) {
    // Emit least-significant 7 bits first; set MSB if more bytes follow.
    do {
        uint8_t b = value & 0x7F;
        value >>= 7;
        if (value) b |= 0x80;
        out.push_back(b);
    } while (value);
}

// Small utility: run all tests
static void runTests() {
    g_total = 0;
    g_passed = 0;

    // Test 1: 1-byte varint (values 0..127)
    {
        // Explanation:
        // p[0] = 0x7F (127) -> a = *p; !(a&0x80) => *v == 127; return 1
        unsigned char p[1];
        u32 v = 0;
        p[0] = 0x7F;
        int n = sqlite3Fts5GetVarint32(p, &v);
        if (n == 1 && v == 127) {
            TEST_PASS();
        } else {
            std::cerr << "Test1 (1-byte) failed: n=" << n << " v=" << v << "\n";
            TEST_FAIL();
        }
    }

    // Test 2: 2-byte varint path
    {
        // Explanation:
        // p0 = 0x81 (MSB set), p1 = 0x34 (MSB 0) -> value = ((0x01) << 7) | 0x34 = 180
        unsigned char p[2];
        u32 v = 0;
        p[0] = 0x81;
        p[1] = 0x34; // MSB clear
        int n = sqlite3Fts5GetVarint32(p, &v);
        if (n == 2 && v == 180) {
            TEST_PASS();
        } else {
            std::cerr << "Test2 (2-byte) failed: n=" << n << " v=" << v << "\n";
            TEST_FAIL();
        }
    }

    // Test 3: 3-byte varint path
    {
        // Explanation:
        // p0 = 0x81 (p0&0x7F = 1), p1 = 0xFF (MSB set, used by 2-byte path avoidance), p2 = 0x3F (MSB 0)
        // 3-byte path computes:
        // a = ((p0 & 0x7F) << 14) | p2  ->  (1<<14) | 0x3F = 0x4000 | 0x3F = 0x403F
        // a &= ((0x7F<<14) | 0x7F) -> remains 0x403F
        // b = (p1 & 0x7F) << 7 -> (0x7F) << 7 = 0x3F80
        // v = a | b = 0x403F | 0x3F80 = 0x7FBF = 32703
        unsigned char p[3];
        u32 v = 0;
        p[0] = 0x81;
        p[1] = 0xFF;
        p[2] = 0x3F;
        int n = sqlite3Fts5GetVarint32(p, &v);
        if (n == 3 && v == 32703) {
            TEST_PASS();
        } else {
            std::cerr << "Test3 (3-byte) failed: n=" << n << " v=" << v << "\n";
            TEST_FAIL();
        }
    }

    // Test 4: Large (4-9 byte) varint path
    {
        // We construct a 64-bit varint value that requires multiple bytes.
        // Use V = 0x123456789, which should encode to 5 bytes in the 64-bit varint format.
        uint64_t V = 0x123456789ULL;
        std::vector<uint8_t> enc;
        encodeVarint64(V, enc);

        // Build buffer such that p points to the start, but p-2 points to the encoding
        // According to sqlite3Fts5GetVarint32, when large path is taken, it calls
        // sqlite3Fts5GetVarint(p, &v64) with p-2 as the start.
        // So we need to pass p = buffer.data() + 2, and place the enc bytes at buffer[0..]
        std::vector<uint8_t> buffer(enc.size() + 2, 0);
        // Copy encoding into the start of buffer
        memcpy(buffer.data(), enc.data(), enc.size());

        u32 v32 = 0;
        // Call with p pointing two bytes into buffer so p-2==buffer.data()
        // This matches the code path: p -= 2; n = sqlite3Fts5GetVarint(p, &v64);
        int n = sqlite3Fts5GetVarint32(buffer.data() + 2, &v32);

        // Expected v32 is (V & 0x7FFFFFFF)
        uint32_t expected = static_cast<uint32_t>(V & 0x7FFFFFFFFULL); // lower 31 bits
        if (n == static_cast<int>(enc.size()) && v32 == expected) {
            TEST_PASS();
        } else {
            std::cerr << "Test4 (large path) failed: n=" << n
                      << " v32=" << v32 << " expected=" << expected << "\n";
            TEST_FAIL();
        }
    }

    // Summary
    std::cout << "Total tests: " << g_total << ", Passed: " << g_passed
              << ", Failed: " << (g_total - g_passed) << "\n";
}

int main() {
    runTests();
    // Return non-zero if there were failures
    // (g_passed equals number of tests passed)
    if (g_total == g_passed) {
        return 0;
    } else {
        return 1;
    }
}
/*
Step 1 candidate keywords (derived from the focal method and its class dependencies):
- sqlite3Fts5GetVarint
- u64 (uint64) and v (output value)
- input pointer p and decoding branches (1-byte to 9-byte varints)
- bitwise masks: 0x80, 0x7f, SLOT_2_0, SLOT_4_2_0 (external to test)
- path regressions: early return cases 1 and 2, and multi-byte decoding paths 3..9
- test harness for decoding verification
- C linkage (extern "C") for C function under test
- boundary coverage for 1..9-byte varints
- non-terminating assertion style in tests (pass/fail reporting)
- simple test runner (no gtest/GMock)
*/

#include <vector>
#include <fts5Int.h>
#include <string>
#include <iostream>
#include <cstdint>


// Domain knowledge notes:
// - We do not rely on Google Test. We implement a lightweight test harness.
// - The target function is C code; provide correct C linkage in C++ test.
// - We encode values using a generic LEB128-like varint encoder so that
//   the runtime function sqlite3Fts5GetVarint can decode them deterministically.
// - We verify both the number of bytes consumed and the decoded value.

// Define same-sized types as used by the focal C implementation
typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned char u8;

// Forward declaration of the function under test (C linkage)
extern "C" {
    // The real implementation lives in fts5_varint.c
    unsigned char sqlite3Fts5GetVarint(const unsigned char *p, u64 *v);
}

// Simple encoder: encodes a 64-bit value into 1..9 bytes using a
// little-endian 7-bit-per-byte scheme (LEB128-like). This mirrors the
// encoding used by typical "varint" readers in C libraries.
// Each byte uses the MSB as "continuation" flag except the last (MSB=0).
static std::vector<unsigned char> encodeVarintLEB128(u64 value) {
    std::vector<unsigned char> out;
    do {
        unsigned char byte = static_cast<unsigned char>(value & 0x7F);
        value >>= 7;
        if (value != 0) {
            byte |= 0x80; // more bytes follow
        }
        out.push_back(byte);
    } while (value != 0);
    return out;
}

// Simple test harness state
struct TestResult {
    std::string name;
    bool passed;
    std::string message;
};

// Run a single test: encode value, call the decoder, verify length and value
static TestResult runTestValue(u64 value) {
    // Determine expected length by encoder
    std::vector<unsigned char> encoded = encodeVarintLEB128(value);
    size_t expected_len = encoded.size();

    // Call the function under test
    u64 decoded = 0;
    unsigned char ret = sqlite3Fts5GetVarint(encoded.data(), &decoded);

    // Prepare result
    TestResult tr;
    tr.name = "Decode v=" + std::to_string(value) + " expect_len=" + std::to_string(expected_len);
    if (ret != static_cast<unsigned char>(expected_len)) {
        tr.passed = false;
        tr.message = "Return length mismatch: got " + std::to_string(ret) +
                     ", expected " + std::to_string(expected_len);
        return tr;
    }
    if (decoded != value) {
        tr.passed = false;
        tr.message = "Decoded value mismatch: got " + std::to_string(decoded) +
                     ", expected " + std::to_string(value);
        return tr;
    }
    tr.passed = true;
    tr.message = "OK";
    return tr;
}

// Entry point
int main() {
    // Candidate test values chosen to exercise varint lengths 1..9
    // These values map to 1..9-byte varints when encoded with the 7-bit-per-byte scheme.
    // v1 -> 1 byte, v2 -> 2 bytes, ..., v9 -> 9 bytes.
    const u64 testValues[9] = {
        0ULL,                         // 1-byte: 0
        128ULL,                        // 2-byte
        16384ULL,                      // 3-byte
        2097152ULL,                     // 4-byte
        268435456ULL,                   // 5-byte
        34359738368ULL,                 // 6-byte
        4398046511104ULL,               // 7-byte
        562949953421312ULL,             // 8-byte
        72057594037927936ULL             // 9-byte
    };

    std::vector<TestResult> results;

    // Run each test and collect results
    for (size_t i = 0; i < 9; ++i) {
        results.push_back(runTestValue(testValues[i]));
    }

    // Report
    int passed = 0;
    for (size_t i = 0; i < results.size(); ++i) {
        const TestResult &r = results[i];
        std::cout << "Test " << (i+1) << " (" << r.name << "): "
                  << (r.passed ? "PASSED" : "FAILED") << "\n";
        if (!r.passed) {
            std::cout << "  Reason: " << r.message << "\n";
        } else {
            ++passed;
        }
    }

    std::cout << "\nSummary: " << passed << " / " << results.size() << " tests PASSED.\n";

    // Return non-zero if any test failed to aid integration with scripts
    return (passed == static_cast<int>(results.size())) ? 0 : 1;
}
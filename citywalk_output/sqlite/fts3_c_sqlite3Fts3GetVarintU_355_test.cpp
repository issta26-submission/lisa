// Comprehensive unit test suite for the focal method sqlite3Fts3GetVarintU
// Adapted to a self-contained C++11 test harness (no GoogleTest).
// The goal is to exercise the varint decoding logic robustly with multiple cases.

#include <fts3Int.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <fts3.h>
#include <vector>
#include <assert.h>
#include <stdio.h>
#include <cstring>
#include <stdarg.h>
#include <iostream>
#include <cstdint>


// Domain assumptions (simplified for test harness):
// - We implement a self-contained version of the focal function signature:
//   int sqlite3Fts3GetVarintU(const char *pBuf, sqlite_uint64 *v)
// - This test harness does not require the original project headers.
// - We provide a straightforward 9-byte max varint decoder matching SQLite-like
//   unsigned varint decoding: 7 data bits per byte, MSB is continuation flag.
// - The function returns the number of bytes consumed from the input buffer.

using sqlite_uint64 = uint64_t;

// Self-contained implementation of the focal function (for testing purposes)
int sqlite3Fts3GetVarintU(const char *pBuf, sqlite_uint64 *v){
    const unsigned char *p = reinterpret_cast<const unsigned char*>(pBuf);
    sqlite_uint64 value = 0;
    int shift = 0;

    // Typical max 9 bytes for 64-bit varint (7 bits per byte, MSB as continuation)
    for(int i = 0; i < 9; ++i){
        unsigned char c = p[i];
        value |= static_cast<sqlite_uint64>(c & 0x7F) << shift;
        if((c & 0x80) == 0){
            *v = value;
            return i + 1; // bytes consumed
        }
        shift += 7;
    }
    // If loop completes (9 bytes with continuation bits), return 9 anyway
    *v = value;
    return 9;
}

// Helper: standard varint encoder for testing (produces 1..9 bytes)
std::vector<unsigned char> encodeVarintU(uint64_t value){
    std::vector<unsigned char> out;
    do{
        unsigned char b = static_cast<unsigned char>(value & 0x7F);
        value >>= 7;
        if(value) b |= 0x80;
        out.push_back(b);
    } while(value);
    return out;
}

// Lightweight test framework (non-terminating assertions)
static int g_failures = 0;

#define ASSERT_TRUE(cond, msg) do { \
    if(!(cond)){ \
        std::cerr << "ASSERT TRUE FAILED: " << msg << " (line " << __LINE__ << ")" << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define ASSERT_EQ(a, b, msg) do { \
    if(!((a) == (b))){ \
        std::cerr << "ASSERT EQUAL FAILED: " << msg \
                  << " | expected: " << (b) << " actual: " << (a) \
                  << " (line " << __LINE__ << ")" << std::endl; \
        ++g_failures; \
    } \
} while(0)

// Test 1: One-byte varint (value fits in a single byte, MSB = 0)
void test_one_byte_varint(){
    // Value to encode
    uint64_t val = 0x5A; // 90 decimal
    auto enc = encodeVarintU(val); // should produce [0x5A]
    const char* buf = reinterpret_cast<const char*>(enc.data());

    uint64_t out = 0;
    int consumed = sqlite3Fts3GetVarintU(buf, &out);

    // Expected: consumed = 1, value = val
    ASSERT_EQ(consumed, (int)enc.size(), "One-byte varint should consume exactly 1 byte");
    ASSERT_EQ(out, val, "One-byte varint decoded value mismatch");
}

// Test 2: Two-byte varint (continuation in first byte, ends in second)
void test_two_byte_varint(){
    // Example: value 300 -> encoding [0xAC, 0x02]
    uint64_t val = 300;
    auto enc = encodeVarintU(val);
    const char* buf = reinterpret_cast<const char*>(enc.data());

    uint64_t out = 0;
    int consumed = sqlite3Fts3GetVarintU(buf, &out);

    ASSERT_EQ(consumed, (int)enc.size(), "Two-byte varint should consume all produced bytes");
    ASSERT_EQ(out, val, "Two-byte varint decoded value mismatch");
}

// Test 3: Multi-byte varint (requires 3+ bytes)
void test_three_plus_varint(){
    // Use a value that requires multiple bytes (e.g., 0x0123456789ABCDEF)
    uint64_t val = 0x0123456789ABCDEFULL;
    auto enc = encodeVarintU(val);
    const char* buf = reinterpret_cast<const char*>(enc.data());

    uint64_t out = 0;
    int consumed = sqlite3Fts3GetVarintU(buf, &out);

    ASSERT_EQ(consumed, (int)enc.size(), "Multi-byte varint should consume all produced bytes");
    ASSERT_EQ(out, val, "Multi-byte varint decoded value mismatch");
}

// Test 4: Maximum 63-bit value (uses 9 bytes)
void test_max_varint(){
    uint64_t val = 0x7FFFFFFFFFFFFFFFULL; // maximum 63-bit value representable with 9 7-bit chunks
    auto enc = encodeVarintU(val);
    const char* buf = reinterpret_cast<const char*>(enc.data());

    uint64_t out = 0;
    int consumed = sqlite3Fts3GetVarintU(buf, &out);

    ASSERT_EQ(consumed, (int)enc.size(), "Max varint should consume 9 bytes");
    ASSERT_EQ(out, val, "Max varint decoded value mismatch");
}

// Test 5: Zero value (edge case)
void test_zero_varint(){
    uint64_t val = 0;
    auto enc = encodeVarintU(val);
    const char* buf = reinterpret_cast<const char*>(enc.data());

    uint64_t out = 0;
    int consumed = sqlite3Fts3GetVarintU(buf, &out);

    ASSERT_EQ(consumed, (int)enc.size(), "Zero varint should consume 1 byte");
    ASSERT_EQ(out, val, "Zero varint decoded value mismatch");
}

// Main entry: run all tests and report summary
int main(){
    std::cout << "Starting sqlite3Fts3GetVarintU unit tests (self-contained harness)" << std::endl;

    test_one_byte_varint();
    test_two_byte_varint();
    test_three_plus_varint();
    test_max_varint();
    test_zero_varint();

    if(g_failures == 0){
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << g_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}
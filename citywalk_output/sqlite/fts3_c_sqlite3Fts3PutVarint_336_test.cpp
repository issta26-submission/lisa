/*
Unit test suite for sqlite3Fts3PutVarint (C function) using C++11, without GTest.

What this test covers:
- Basic encoding correctness for small values (0, 1, 127)
- Multi-byte encoding for cross-boundary values (e.g., 128)
- Round-trip correctness for larger values via a decode helper
- Correct handling of negative values (ensuring bit-pattern preservation)
- A set of representative values to exercise multiple-byte encodings

Notes:
- The tests rely on the external C function sqlite3Fts3PutVarint, declared with C linkage.
- We implement a lightweight decodeVarint helper to verify that encoding is invertible for the given input.
- This test suite uses only the C++ Standard Library, no GTest, and runs from main().

Assumptions:
- The project provides the necessary headers for FTS3 (e.g., fts3Int.h / fts3.h) so that the linked function symbol and required macros (e.g., FTS3_VARINT_MAX) are available at link time.
- The underlying types sqlite_int64 and sqlite_uint64 map to standard 64-bit signed/unsigned integers (commonly int64_t / uint64_t). We provide aliases to match common definitions.
*/

#include <fts3Int.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <fts3.h>
#include <vector>
#include <assert.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <stdarg.h>
#include <string>
#include <iostream>
#include <cstdint>
#include <cassert>


// Candidate type aliases to align with the focal function's expected types.
// These typically map to SQLite internal types (signed/unsigned 64-bit integers).
using sqlite_int64 = int64_t;
using sqlite_uint64 = uint64_t;

// Declare the focal function with C linkage to enable linking against the C implementation.
extern "C" int sqlite3Fts3PutVarint(char *p, sqlite_int64 v);

// Decode helper: decodes a varint encoded in p with exactly len bytes.
// This is used to verify that the encoding produced by sqlite3Fts3PutVarint matches the input value.
static sqlite_int64 decodeVarint(const unsigned char* p, int len) {
    sqlite_uint64 val = 0;
    int shift = 0;
    for (int i = 0; i < len; ++i) {
        unsigned char c = p[i];
        val |= (static_cast<sqlite_uint64>(c & 0x7F) << shift);
        // If MSB is 0, this is the last byte; but we rely on the provided len.
        shift += 7;
    }
    return static_cast<sqlite_int64>(val);
}

// Simple test harness helpers
static bool runTest(const std::string& name, const std::function<bool()>& fn) {
    bool ok = false;
    try {
        ok = fn();
    } catch (...) {
        ok = false;
    }
    std::cout << (ok ? "[PASS] " : "[FAIL] ") << name << std::endl;
    return ok;
}

/*
Test 1: PutVarint with value 0
Expected: length 1, first byte equals 0x00.
*/
static bool test_putvarint_zero() {
    char buf[16] = {0};
    int len = sqlite3Fts3PutVarint(buf, 0);
    if (len != 1) {
        std::fprintf(stderr, "test_putvarint_zero: unexpected length %d\n", len);
        return false;
    }
    if (static_cast<unsigned char>(buf[0]) != 0x00) {
        std::fprintf(stderr, "test_putvarint_zero: unexpected byte 0x%02X\n", static_cast<unsigned char>(buf[0]));
        return false;
    }
    return true;
}

/*
Test 2: PutVarint with value 1
Expected: length 1, first byte equals 0x01.
*/
static bool test_putvarint_one() {
    char buf[16] = {0};
    int len = sqlite3Fts3PutVarint(buf, 1);
    if (len != 1) {
        std::fprintf(stderr, "test_putvarint_one: unexpected length %d\n", len);
        return false;
    }
    if (static_cast<unsigned char>(buf[0]) != 0x01) {
        std::fprintf(stderr, "test_putvarint_one: unexpected byte 0x%02X\n", static_cast<unsigned char>(buf[0]));
        return false;
    }
    return true;
}

/*
Test 3: PutVarint with value 127
Expected: length 1, final byte has high bit cleared to 0 (0x7F).
*/
static bool test_putvarint_127() {
    char buf[16] = {0};
    int len = sqlite3Fts3PutVarint(buf, 127);
    if (len != 1) {
        std::fprintf(stderr, "test_putvarint_127: unexpected length %d\n", len);
        return false;
    }
    if (static_cast<unsigned char>(buf[0]) != 0x7F) {
        std::fprintf(stderr, "test_putvarint_127: unexpected byte 0x%02X\n", static_cast<unsigned char>(buf[0]));
        return false;
    }
    return true;
}

/*
Test 4: PutVarint with value 128
Expected: length 2, bytes [0] = 0x80, [1] = 0x01
*/
static bool test_putvarint_128() {
    char buf[16] = {0};
    int len = sqlite3Fts3PutVarint(buf, 128);
    if (len != 2) {
        std::fprintf(stderr, "test_putvarint_128: unexpected length %d\n", len);
        return false;
    }
    if (static_cast<unsigned char>(buf[0]) != 0x80 || static_cast<unsigned char>(buf[1]) != 0x01) {
        std::fprintf(stderr, "test_putvarint_128: unexpected bytes: 0x%02X 0x%02X\n",
                     static_cast<unsigned char>(buf[0]), static_cast<unsigned char>(buf[1]));
        return false;
    }
    return true;
}

/*
Test 5: Round-trip correctness for a larger value
- Encode a large positive value, then decode and compare with the original.
*/
static bool test_putvarint_large_roundtrip() {
    char buf[32] = {0};
    sqlite_int64 v = 123456789012345LL;
    int len = sqlite3Fts3PutVarint(buf, v);
    if (len <= 0 || len > 32) {
        std::fprintf(stderr, "test_putvarint_large_roundtrip: invalid length %d\n", len);
        return false;
    }
    sqlite_int64 decoded = decodeVarint(reinterpret_cast<const unsigned char*>(buf), len);
    if (decoded != v) {
        std::fprintf(stderr, "test_putvarint_large_roundtrip: decoded 0x%llx != original 0x%llx\n",
                     static_cast<long long>(decoded), static_cast<long long>(v));
        return false;
    }
    return true;
}

/*
Test 6: Round-trip correctness for a negative value
- Encode -1 and ensure decoding yields -1 (bit-pattern preserved).
*/
static bool test_putvarint_negative_roundtrip() {
    char buf[32] = {0};
    sqlite_int64 v = -1;
    int len = sqlite3Fts3PutVarint(buf, v);
    if (len <= 0 || len > 32) {
        std::fprintf(stderr, "test_putvarint_negative_roundtrip: invalid length %d\n", len);
        return false;
    }
    sqlite_int64 decoded = decodeVarint(reinterpret_cast<const unsigned char*>(buf), len);
    if (decoded != v) {
        std::fprintf(stderr, "test_putvarint_negative_roundtrip: decoded 0x%llx != original 0x%llx\n",
                     static_cast<long long>(decoded), static_cast<long long>(v));
        return false;
    }
    return true;
}

/*
Test 7: A small suite of representative values to exercise multiple encodings
- 0, 1, 2, 127, 128, 3000, 1234567890123, -12345, INT64_MAX/4
*/
static bool test_putvarint_representative_set() {
    const std::vector<sqlite_int64> values = {
        0LL, 1LL, 2LL, 127LL, 128LL, 3000LL, 1234567890123LL, -12345LL,  (INT64_C(1) << 62)
    };
    for (size_t i = 0; i < values.size(); ++i) {
        char buf[64] = {0};
        sqlite_int64 v = values[i];
        int len = sqlite3Fts3PutVarint(buf, v);
        if (len <= 0 || len > 64) {
            std::fprintf(stderr, "test_putvarint_representative_set: value index %zu: invalid length %d\n", i, len);
            return false;
        }
        sqlite_int64 decoded = decodeVarint(reinterpret_cast<const unsigned char*>(buf), len);
        if (decoded != v) {
            std::fprintf(stderr, "test_putvarint_representative_set: index %zu: decoded 0x%llx != original 0x%llx\n",
                         i, static_cast<long long>(decoded), static_cast<long long>(v));
            return false;
        }
    }
    return true;
}

int main() {
    // Run all tests and collect results
    int overall_failures = 0;

    if (!runTest("test_putvarint_zero", test_putvarint_zero)) ++overall_failures;
    if (!runTest("test_putvarint_one", test_putvarint_one)) ++overall_failures;
    if (!runTest("test_putvarint_127", test_putvarint_127)) ++overall_failures;
    if (!runTest("test_putvarint_128", test_putvarint_128)) ++overall_failures;
    if (!runTest("test_putvarint_large_roundtrip", test_putvarint_large_roundtrip)) ++overall_failures;
    if (!runTest("test_putvarint_negative_roundtrip", test_putvarint_negative_roundtrip)) ++overall_failures;
    if (!runTest("test_putvarint_representative_set", test_putvarint_representative_set)) ++overall_failures;

    if (overall_failures == 0) {
        std::cout << "[ALL TESTS PASSED] sqlite3Fts3PutVarint unit tests completed successfully." << std::endl;
        return 0;
    } else {
        std::cerr << "[TEST FAILURES] " << overall_failures << " test(s) failed." << std::endl;
        return 1;
    }
}
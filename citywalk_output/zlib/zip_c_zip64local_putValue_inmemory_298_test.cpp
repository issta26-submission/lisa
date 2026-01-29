// Generated C++11 unit test suite for the focal method:
// zip64local_putValue_inmemory in the provided zip.c file.
// The test suite uses a small, self-contained test harness (no GTest).
// It exposes the target function by temporarily removing internal linkage
// through a macro trick: define 'local' as an empty macro before including zip.c.
// This allows calling zip64local_putValue_inmemory from the test harness.

#define local
// Include the implementation so the function zip64local_putValue_inmemory is visible.
// We rely on the implementation to define ZPOS64_T and related types via included headers.
#include <cstring>
#include <string.h>
#include <time.h>
#include <cstdint>
#include <stdio.h>
#include <stdint.h>
#include <crypt.h>
#include <zip.c>
#include <iostream>
#include <zlib.h>
#include <stdlib.h>
#include <zip.h>



// Simple non-terminating assertion helpers
// They print failure details but do not terminate the test suite execution.
static inline void assert_uint8_eq(uint8_t a, uint8_t b, const char* msg) {
    if (a != b) {
        std::cerr << "ASSERT FAIL: " << msg
                  << " | expected: " << +b << " , actual: " << +a << std::endl;
    }
}

static inline void assert_true(bool cond, const char* msg) {
    if (!cond) {
        std::cerr << "ASSERT FAIL: " << msg << std::endl;
    }
}

// Test 1: No overflow when nbByte bytes are enough to hold x
// x = 0x1234, nbByte = 2 -> dest[0] = 0x34, dest[1] = 0x12
static bool test_no_overflow_two_bytes() {
    unsigned char buf[2] = {0, 0};
    zip64local_putValue_inmemory((void*)buf, (ZPOS64_T)0x1234, 2);
    bool ok = (buf[0] == 0x34) && (buf[1] == 0x12);
    assert_true(ok, "test_no_overflow_two_bytes: bytes should be [0x34, 0x12]");
    return ok;
}

// Test 2: Overflow path active when x cannot fit in nbByte bytes
// x = 0x123456, nbByte = 2 -> after call, both bytes should be 0xFF
static bool test_overflow_two_bytes() {
    unsigned char buf[2] = {0, 0};
    zip64local_putValue_inmemory((void*)buf, (ZPOS64_T)0x123456, 2);
    bool ok = (buf[0] == 0xFF) && (buf[1] == 0xFF);
    assert_true(ok, "test_overflow_two_bytes: overflow should set both bytes to 0xFF");
    return ok;
}

// Test 3: Overflow path for a single-byte destination
// x = 0x1FF, nbByte = 1 -> after call, dest[0] should be 0xFF
static bool test_overflow_one_byte() {
    unsigned char buf[1] = {0};
    zip64local_putValue_inmemory((void*)buf, (ZPOS64_T)0x1FF, 1);
    bool ok = (buf[0] == 0xFF);
    assert_true(ok, "test_overflow_one_byte: overflow should set single byte to 0xFF");
    return ok;
}

// Test 4: No overflow with x == 0 and nbByte > 0
// dest should be all zeros
static bool test_zero_x_no_overflow() {
    unsigned char buf[4] = {0, 0, 0, 0};
    zip64local_putValue_inmemory((void*)buf, (ZPOS64_T)0x0, 4);
    bool ok = std::memcmp(buf, "\0\0\0\0", 4) == 0;
    assert_true(ok, "test_zero_x_no_overflow: all bytes should be 0 when x == 0");
    return ok;
}

// Test 5: Multi-byte case with no overflow and lower bytes filled correctly
// x = 0x00AABB, nbByte = 3 -> dest should be [0xBB, 0xAA, 0x00]
static bool test_no_overflow_multi_byte() {
    unsigned char buf[3] = {0, 0, 0};
    zip64local_putValue_inmemory((void*)buf, (ZPOS64_T)0x00AABB, 3);
    bool ok = (buf[0] == 0xBB) && (buf[1] == 0xAA) && (buf[2] == 0x00);
    assert_true(ok, "test_no_overflow_multi_byte: bytes should be [0xBB, 0xAA, 0x00]");
    return ok;
}

// Test 6: Overflow with nbByte less than required for x
// x = 0x01020304, nbByte = 3 -> lower 3 bytes are 0x04,0x03,0x02 and overflow -> all 0xFF
static bool test_overflow_with_larger_x() {
    unsigned char buf[3] = {0, 0, 0};
    zip64local_putValue_inmemory((void*)buf, (ZPOS64_T)0x01020304, 3);
    bool ok = buf[0] == 0xFF && buf[1] == 0xFF && buf[2] == 0xFF;
    assert_true(ok, "test_overflow_with_larger_x: overflow should set all 3 bytes to 0xFF");
    return ok;
}

// Runner to execute all tests and report summary
static void run_all_tests() {
    int passed = 0;
    int total = 0;

    auto run = [&](bool(*test_func)(), const char* name) {
        total++;
        bool ok = test_func();
        if (ok) {
            std::cout << "[PASS] " << name << std::endl;
            passed++;
        } else {
            std::cerr << "[FAIL] " << name << std::endl;
        }
    };

    run(test_no_overflow_two_bytes, "test_no_overflow_two_bytes");
    run(test_overflow_two_bytes, "test_overflow_two_bytes");
    run(test_overflow_one_byte, "test_overflow_one_byte");
    run(test_zero_x_no_overflow, "test_zero_x_no_overflow");
    run(test_no_overflow_multi_byte, "test_no_overflow_multi_byte");
    run(test_overflow_with_larger_x, "test_overflow_with_larger_x");

    std::cout << "Test results: " << passed << "/" << total << " passed." << std::endl;
}

// Main entry point for the test executable
int main() {
    run_all_tests();
    return 0;
}
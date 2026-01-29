// C++11 unit test suite for the focal method chunk_type_valid in pngfix.c
// This test suite does not rely on any external testing framework (e.g., GTest).
// It uses a lightweight, non-terminating assertion mechanism and runs all
// tests from main.
//
// Notes:
// - The tests assume the same per-byte 4-byte construction semantics as the
//   original library (i.e., a 32-bit value whose bytes correspond to the
//   four ASCII characters that form the chunk type).
// - We expose the C function chunk_type_valid via an extern "C" declaration and
//   provide a small helper to construct 32-bit values from four ASCII bytes.

#include <cstdint>
#include <setjmp.h>
#include <string.h>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


// The focal function is defined in the C source (pngfix.c). We declare it here
// with C linkage so the C++ test code can call it. The function signature is
// int chunk_type_valid(png_uint_32 c);
extern "C" int chunk_type_valid(uint32_t c);

// Helper to construct a 32-bit value from four bytes: [a, b, c, d] in
// big-endian byte order, matching typical PNG macro conventions.
// This ensures consistent byte placement across platforms.
static uint32_t PNG_U32_BYTES(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
    return (static_cast<uint32_t>(a) << 24) |
           (static_cast<uint32_t>(b) << 16) |
           (static_cast<uint32_t>(c) << 8)  |
           (static_cast<uint32_t>(d));
}

// Lightweight test result printer
static int g_total_tests = 0;
static int g_passed_tests = 0;

static void report_result(const char* test_name, bool passed) {
    ++g_total_tests;
    if (passed) {
        ++g_passed_tests;
        std::cout << "[PASS] " << test_name << "\n";
    } else {
        std::cout << "[FAIL] " << test_name << "\n";
    }
}

// Test 1: All four bytes are uppercase 'A' (65,65,65,65) -> expect true
bool test_chunk_type_all_AAAA() {
    // AAAA
    uint32_t val = PNG_U32_BYTES(65, 65, 65, 65);
    int r = chunk_type_valid(val);
    return (r != 0);
}

// Test 2: All uppercase (A,B,C,D) -> expect true
bool test_chunk_type_ABCD() {
    uint32_t val = PNG_U32_BYTES(65, 66, 67, 68); // 'A','B','C','D'
    int r = chunk_type_valid(val);
    return (r != 0);
}

// Test 3: One lowercase letter ('a' in the last position) -> expect false
bool test_chunk_type_last_lowercase() {
    uint32_t val = PNG_U32_BYTES(65, 66, 67, 97); // 'A','B','C','a'
    int r = chunk_type_valid(val);
    return (r == 0);
}

// Test 4: Contains a space (invalid) -> expect false
bool test_chunk_type_with_space() {
    uint32_t val = PNG_U32_BYTES(65, 32, 65, 65); // 'A',' ','A','A'
    int r = chunk_type_valid(val);
    return (r == 0);
}

// Test 5: Includes 'Z' among uppercase letters -> expect true
bool test_chunk_type_with_Z() {
    uint32_t val = PNG_U32_BYTES(90, 65, 65, 65); // 'Z','A','A','A'
    int r = chunk_type_valid(val);
    return (r != 0);
}

// Test 6: All lowercase letters -> expect false
bool test_chunk_type_all_lowercase() {
    uint32_t val = PNG_U32_BYTES(97, 98, 99, 100); // 'a','b','c','d'
    int r = chunk_type_valid(val);
    return (r == 0);
}

// Test 7: Mixed uppercase across all bytes -> expect true
bool test_chunk_type_mixed_upper_all_bytes() {
    uint32_t val = PNG_U32_BYTES(65, 90, 65, 90); // 'A','Z','A','Z'
    int r = chunk_type_valid(val);
    return (r != 0);
}

// Test 8: Zero value (all bytes zero) -> expect false
bool test_chunk_type_zero() {
    uint32_t val = 0;
    int r = chunk_type_valid(val);
    return (r == 0);
}

int main() {
    // Run all tests, reporting per-test results. This harness uses non-terminating
    // assertions by continuing to execute even when individual tests fail.
    std::cout << "Starting chunk_type_valid unit tests...\n";

    report_result("test_chunk_type_all_AAAA", test_chunk_type_all_AAAA());
    report_result("test_chunk_type_ABCD", test_chunk_type_ABCD());
    report_result("test_chunk_type_last_lowercase", test_chunk_type_last_lowercase());
    report_result("test_chunk_type_with_space", test_chunk_type_with_space());
    report_result("test_chunk_type_with_Z", test_chunk_type_with_Z());
    report_result("test_chunk_type_all_lowercase", test_chunk_type_all_lowercase());
    report_result("test_chunk_type_mixed_upper_all_bytes", test_chunk_type_mixed_upper_all_bytes());
    report_result("test_chunk_type_zero", test_chunk_type_zero());

    std::cout << "Chunk_type_valid tests completed. Passed "
              << g_passed_tests << " / " << g_total_tests << " tests.\n";

    // Return non-zero if any test failed (optional, but useful for CI)
    return (g_passed_tests == g_total_tests) ? 0 : 1;
}
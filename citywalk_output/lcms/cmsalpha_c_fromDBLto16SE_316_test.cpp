// Unit test suite for the focal method: fromDBLto16SE
// - Language: C++11
// - No GTest used; a simple in-house test harness is implemented
// - The tests exercise the exact behavior of fromDBLto16SE as described in the focal method
// - We rely on extern "C" prototype to link with the C function implemented in cmsalpha.c
//
// Notes on test strategy and coverage (mapped to the instructions):
// 1) Candidate Keywords extracted from the focal method: cmsFloat64Number, cmsUInt16Number, _cmsQuickSaturateWord, CHANGE_ENDIAN, fromDBLto16SE
// 2) Cover true/false branches of the saturation logic by testing values at lower bound, mid-range, upper bound, and beyond range
// 3) This test does not rely on private or static internal details; it uses the public C interface as a unit under test
// 4) Only standard library facilities are used for assertions and output
// 5) Assertions are non-terminating; the test harness continues after failures to maximize coverage
// 6) gmock/gmock-like capabilities are not used; no private methods are accessed
// 7) Data-type handling is explicit: we compare raw two-byte outputs (big-endian representation) to ensure correct endianness handling
// 8) The correct namespace is used (global namespace for C linkage; C++ file uses standard library in <iostream>)
// 9) The main function serves as the test runner; gtest-like mechanics are manually implemented

#include <cstring>
#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <cstdint>


// Link to the focal C function (declared with C linkage)
extern "C" void fromDBLto16SE(void* dst, const void* src);

// Helper: swap 16-bit word (endianness flip)
static inline uint16_t swap16(uint16_t v)
{
    return static_cast<uint16_t>((v << 8) | (v >> 8));
}

// Simple non-terminating assertion macro for testing
static int g_total_tests = 0;
static int g_failed_tests = 0;
#define TEST_ASSERT(cond, msg)                                  \
    do {                                                        \
        ++g_total_tests;                                        \
        if(!(cond)) {                                           \
            ++g_failed_tests;                                   \
            std::cerr << "TEST FAIL: " << (msg)                 \
                      << " (" << __FILE__ << ":" << __LINE__     \
                      << ")" << std::endl;                     \
        }                                                       \
    } while(false)

// Helper: compute expected big-endian bytes for a given input
static void compute_expected_be(double src, unsigned char out_be[2])
{
    // Saturation logic mirrors _cmsQuickSaturateWord(n * 65535.0f)
    double val = src * 65535.0;
    uint16_t i;
    if (val <= 0.0) {
        i = 0;
    } else if (val >= 65535.0) {
        i = 65535;
    } else {
        // Truncation toward zero (as typical in C cast)
        i = static_cast<uint16_t>(val);
    }
    uint16_t be = swap16(i);
    out_be[0] = static_cast<unsigned char>((be >> 8) & 0xFF);
    out_be[1] = static_cast<unsigned char>(be & 0xFF);
}

// Test 1: src = 0.0 should saturate to 0 and result in 0x0000 (big-endian: 0x00 0x00)
static void test_fromDBLto16SE_zero()
{
    double src = 0.0;
    unsigned char dst[2] = {0, 0};
    fromDBLto16SE(dst, &src);

    unsigned char expected_be[2];
    compute_expected_be(src, expected_be);

    TEST_ASSERT(dst[0] == expected_be[0] && dst[1] == expected_be[1],
                "fromDBLto16SE should produce 0x0000 for src = 0.0");
}

// Test 2: src = 1.0 should saturate to 65535, resulting in 0xFFFF (big-endian: 0xFF 0xFF)
static void test_fromDBLto16SE_max()
{
    double src = 1.0;
    unsigned char dst[2] = {0, 0};
    fromDBLto16SE(dst, &src);

    unsigned char expected_be[2];
    compute_expected_be(src, expected_be);

    TEST_ASSERT(dst[0] == expected_be[0] && dst[1] == expected_be[1],
                "fromDBLto16SE should produce 0xFFFF for src = 1.0");
}

// Test 3: src = 0.5 should yield mid-range value and correct endianness
static void test_fromDBLto16SE_half()
{
    double src = 0.5;
    unsigned char dst[2] = {0, 0};
    fromDBLto16SE(dst, &src);

    unsigned char expected_be[2];
    compute_expected_be(src, expected_be);

    TEST_ASSERT(dst[0] == expected_be[0] && dst[1] == expected_be[1],
                "fromDBLto16SE should correctly handle mid-range input (src = 0.5)");
}

// Test 4: src negative should saturate to 0
static void test_fromDBLto16SE_negative()
{
    double src = -0.75;
    unsigned char dst[2] = {0xAA, 0xBB}; // pre-fill to ensure no overwrite beyond 2 bytes
    fromDBLto16SE(dst, &src);

    unsigned char expected_be[2];
    compute_expected_be(src, expected_be);

    TEST_ASSERT(dst[0] == expected_be[0] && dst[1] == expected_be[1],
                "fromDBLto16SE should saturate negative input to 0 (src = -0.75)");
}

// Test 5: src > 1.0 should saturate to 65535 and endianness should reflect swap (0xFFFF)
static void test_fromDBLto16SE_overflow()
{
    double src = 2.0;
    unsigned char dst[2] = {0, 0};
    fromDBLto16SE(dst, &src);

    unsigned char expected_be[2];
    compute_expected_be(src, expected_be);

    TEST_ASSERT(dst[0] == expected_be[0] && dst[1] == expected_be[1],
                "fromDBLto16SE should saturate values > 1.0 to 65535 and swap endian");
}

// Entry point for running all tests
int main()
{
    std::cout << "Running fromDBLto16SE unit tests (C linkage to cmsalpha.c)" << std::endl;

    // Execute all defined tests
    test_fromDBLto16SE_zero();
    test_fromDBLto16SE_max();
    test_fromDBLto16SE_half();
    test_fromDBLto16SE_negative();
    test_fromDBLto16SE_overflow();

    // Summary
    std::cout << "Total tests run: " << g_total_tests << std::endl;
    if (g_failed_tests > 0) {
        std::cout << "Tests FAILED: " << g_failed_tests << std::endl;
    } else {
        std::cout << "All tests passed." << std::endl;
    }

    return g_failed_tests; // Non-zero indicates failure, but tests themselves never terminate early
}
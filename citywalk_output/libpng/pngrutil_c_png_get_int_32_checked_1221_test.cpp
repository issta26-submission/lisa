// Minimal C++11 test suite for png_get_int_32_checked
// This test suite provides a self-contained mock of the necessary pieces
// of the png library to exercise the focal function without GTest.
// It uses a lightweight, non-terminating test harness that reports
// failures but continues execution to maximize coverage.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>


// Lightweight mock/types to mimic the libpng signatures used by the focal method.
typedef uint8_t png_byte;
typedef const png_byte* png_const_bytep;
typedef png_byte* png_bytep;
typedef uint32_t png_uint_32;
typedef int32_t png_int_32;

// Forward declaration of the dependent function used by the focal method.
// In the real libpng, png_get_uint_32 reads a 32-bit unsigned value from 4 bytes
// in big-endian order. We implement a tiny version suitable for testing.
png_uint_32 png_get_uint_32(png_const_bytep buf);

// Implementation of the focal function under test.
// Mirroring the logic observed in the provided snippet.
png_int_32 png_get_int_32_checked(png_const_bytep buf, int *error)
{
    png_uint_32 uval = png_get_uint_32(buf);
    if ((uval & 0x80000000) == 0) // non-negative
        return (png_int_32)uval;

    uval = (uval ^ 0xffffffff) + 1; // 2's complement: -x = ~x+1
    if ((uval & 0x80000000) == 0) // no overflow
        return -(png_int_32)uval;

    // This version of png_get_int_32 has a way of returning the error to the
    // caller, so:
    *error = 1;
    return 0; // Safe
}

// Tiny, endian-correct implementation of png_get_uint_32 used by the focal method.
png_uint_32 png_get_uint_32(png_const_bytep buf)
{
    // Combine 4 bytes in big-endian order to form a 32-bit unsigned value.
    return ((png_uint_32)buf[0] << 24) |
           ((png_uint_32)buf[1] << 16) |
           ((png_uint_32)buf[2] << 8)  |
           ((png_uint_32)buf[3]);
}

// Lightweight test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Non-terminating assertion macro: counts a test as failed but continues.
#define EXPECT(cond, msg) \
    do { \
        ++g_total_tests; \
        if (!(cond)) { \
            ++g_failed_tests; \
            std::cerr << "Test failed: " << msg << std::endl; \
        } \
    } while(0)

// Test 1: Non-negative path
// - Input bytes yield a value with the most significant bit 0.
// - Expect the same value to be returned as a signed 32-bit integer.
// - Error should remain unchanged (default 0).
void test_non_negative_path()
{
    // 0x0000002A -> 42
    uint8_t buf[4] = {0x00, 0x00, 0x00, 0x2A};
    int error = 0;

    png_int_32 result = png_get_int_32_checked(buf, &error);

    EXPECT(result == 42, "Non-negative value should be 42");
    EXPECT(error == 0, "Error should remain 0 for non-negative value");
}

// Test 2: Negative value without overflow
// - Input value has sign bit set but after two's complement conversion does not overflow.
// - Expect a negative value to be returned.
void test_negative_no_overflow_path()
{
    // 0x80000001 -> after negation yields -2147483647
    uint8_t buf[4] = {0x80, 0x00, 0x00, 0x01};
    int error = 0;

    png_int_32 result = png_get_int_32_checked(buf, &error);

    EXPECT(result == -2147483647, "Negative value without overflow should be -2147483647");
    EXPECT(error == 0, "Error should remain 0 for this path");
}

// Test 3: Overflow path
// - Input value triggers overflow after two's complement conversion.
// - Expect 0 to be returned and error flag set to 1.
void test_overflow_path()
{
    // 0x80000000 -> after negation would be 0x80000000 (overflow)
    uint8_t buf[4] = {0x80, 0x00, 0x00, 0x00};
    int error = 0;

    png_int_32 result = png_get_int_32_checked(buf, &error);

    EXPECT(result == 0, "Overflow should return 0");
    EXPECT(error == 1, "Overflow should set error to 1");
}

// Test 4: Error value not modified when no overflow
// - Initialize error with a non-zero value and ensure it's preserved when there is no overflow.
void test_error_not_modified_on_no_overflow()
{
    // 0x00000002 -> 2 (non-negative)
    uint8_t buf[4] = {0x00, 0x00, 0x00, 0x02};
    int error = 12345;

    png_int_32 result = png_get_int_32_checked(buf, &error);

    EXPECT(result == 2, "Value should be 2 for non-negative input");
    EXPECT(error == 12345, "Error value should remain unchanged when no overflow");
}

// Test 5: All-ones input yields -1
// - Ensures that typical negative outputs (without overflow) work as expected.
void test_all_ones_negative()
{
    // 0xFFFFFFFF -> -1
    uint8_t buf[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    int error = 0;

    png_int_32 result = png_get_int_32_checked(buf, &error);

    EXPECT(result == -1, "All-ones input should yield -1");
    EXPECT(error == 0, "Error should remain 0 for this path");
}

int main()
{
    // Run tests with explanatory comments for each scenario
    test_non_negative_path();
    test_negative_no_overflow_path();
    test_overflow_path();
    test_error_not_modified_on_no_overflow();
    test_all_ones_negative();

    // Summary
    std::cout << "Total tests: " << g_total_tests << "\n";
    std::cout << "Failed tests: " << g_failed_tests << "\n";

    // Return non-zero if any test failed
    return (g_failed_tests != 0) ? 1 : 0;
}
// Test suite for the focal method: from8to16 (maps 8-bit to 16-bit using FROM_8_TO_16 macro)
// This test is implemented as a standalone translation unit to be compiled with C++11.
// It does not rely on GTest and uses a lightweight, non-terminating assertion mechanism.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>


// Minimal re-definition of the Cms types and macro to mirror the focal environment.
// These definitions allow compiling and testing the focal function behavior in isolation.
typedef uint8_t  cmsUInt8Number;
typedef uint16_t cmsUInt16Number;

// The macro FROM_8_TO_16(n) is typically implemented as (n * 257) or ((n << 8) | n),
// which maps 0..255 to 0..65535 by duplicating the byte into both high and low bytes.
// We implement it as (n * 257) to reflect the common 0x0101 approach.
#define FROM_8_TO_16(n) ((cmsUInt16Number)((n) * 257))

// Focal method under test (duplicated here for the unit test environment).
// In the real project, this function would be provided by cmsalpha.c.
// The test targets the exact behavior of this function: read a byte from src and write the
// corresponding 16-bit value to dst using FROM_8_TO_16.
void from8to16(void* dst, const void* src)
{
{
       cmsUInt8Number n = *(cmsUInt8Number*)src;
       *(cmsUInt16Number*) dst = (cmsUInt16Number) FROM_8_TO_16(n);
}
}

// Lightweight, non-terminating test framework (single-file, no external dependencies).
static int g_failures = 0;

#define ASSERT_EQ(a,b) do {                                                \
    auto va = (a);                                                           \
    auto vb = (b);                                                           \
    if (va != vb) {                                                          \
        std::cerr << "ASSERT_EQ failed: " #a " != " #b                          \
                  << " -> " << static_cast<unsigned long long>(va)          \
                  << " != " << static_cast<unsigned long long>(vb)        \
                  << "  [File: " << __FILE__ << ", Line: " << __LINE__ << "]" \
                  << std::endl;                                              \
        ++g_failures;                                                        \
    } } while (0)

#define RUN_TEST(test_func) do { test_func(); } while(0)

// Test 1: Zero input maps to zero output.
// This validates the lower boundary of the domain.
void test_from8to16_zero() {
    unsigned char src = 0;
    unsigned short dst = 0xFFFF; // initialize to a non-zero that should be overwritten
    from8to16(&dst, &src);
    ASSERT_EQ(dst, static_cast<unsigned short>(0));
}

// Test 2: Minimal non-zero input maps to FROM_8_TO_16(1) = 257.
// This validates a small non-zero mapping.
void test_from8to16_one() {
    unsigned char src = 1;
    unsigned short dst = 0;
    from8to16(&dst, &src);
    ASSERT_EQ(dst, static_cast<unsigned short>(257));
}

// Test 3: Mid-range input maps correctly (127 -> 32639 with 0x7F7F pattern).
void test_from8to16_mid() {
    unsigned char src = 127;
    unsigned short dst = 0;
    from8to16(&dst, &src);
    ASSERT_EQ(dst, static_cast<unsigned short>(32639)); // 127*257 = 32639
}

// Test 4: Boundary-like upper-mid input maps correctly (128 -> 32896).
void test_from8to16_mid_upper() {
    unsigned char src = 128;
    unsigned short dst = 0;
    from8to16(&dst, &src);
    ASSERT_EQ(dst, static_cast<unsigned short>(32896)); // 128*257 = 32896
}

// Test 5: Maximum input maps to maximum 16-bit value (255 -> 65535).
void test_from8to16_max() {
    unsigned char src = 255;
    unsigned short dst = 0;
    from8to16(&dst, &src);
    ASSERT_EQ(dst, static_cast<unsigned short>(65535)); // 255*257 = 65535
}

// Optional: run all tests and report summary
void run_all_tests() {
    test_from8to16_zero();
    test_from8to16_one();
    test_from8to16_mid();
    test_from8to16_mid_upper();
    test_from8to16_max();
}

// Entry point
int main() {
    run_all_tests();
    if (g_failures > 0) {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    } else {
        std::cout << "All tests passed." << std::endl;
        return 0;
    }
}
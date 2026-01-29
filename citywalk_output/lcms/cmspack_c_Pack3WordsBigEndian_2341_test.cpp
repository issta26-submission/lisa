// Test suite for Pack3WordsBigEndian in cmspack.c
// This test uses a lightweight, non-GTest approach with non-terminating
// assertions to maximize coverage across execution paths.
// It relies on the project's existing CMS types being defined by the
// included headers when built in the actual environment.

#include <cstring>
#include <iostream>
#include <iomanip>
#include <lcms2_internal.h>
#include <cstdint>


// Declaration of the focal function under test.
// We use a C-compatible prototype to avoid C++ name mangling.
// The actual project provides the precise typedefs (cmsUInt8Number, etc.).
// Here we assume common sizes that align with the real library.
// We deliberately use primitive types to maximize compatibility across
// different host configurations, since the function is not expected
// to dereference the 'info' parameter (it's marked unused in the body).
extern "C" unsigned char* Pack3WordsBigEndian(void* info,
                                            unsigned short wOut[],
                                            unsigned char* output,
                                            unsigned int Stride);

// Simple non-terminating test assertions.
// These print diagnostic information but do not abort the test run.
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "[EXPECT_TRUE FAILED] " #cond " at " __FILE__ ":" << __LINE__ << std::endl; \
        ++g_tests_failed; \
    } \
} while(0)

#define EXPECT_EQ(a,b) do { \
    if((a) != (b)) { \
        std::cerr << "[EXPECT_EQ FAILED] " #a " != " #b " (actual: " << +a << " vs " << +b \
                  << ") at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_tests_failed; \
    } \
} while(0)

// Helper to compute the portable expected big-endian byte sequence for three 16-bit words.
static void expected_big_endian_bytes(const unsigned short wOut[3], unsigned char exp[6])
{
    // Each wOut[i] -> high byte, then low byte (big-endian representation)
    exp[0] = static_cast<unsigned char>((wOut[0] >> 8) & 0xFF);
    exp[1] = static_cast<unsigned char>( wOut[0]       & 0xFF);
    exp[2] = static_cast<unsigned char>((wOut[1] >> 8) & 0xFF);
    exp[3] = static_cast<unsigned char>( wOut[1]       & 0xFF);
    exp[4] = static_cast<unsigned char>((wOut[2] >> 8) & 0xFF);
    exp[5] = static_cast<unsigned char>( wOut[2]       & 0xFF);
}

// Test 1: Basic correctness
// Verifies that three 16-bit words are packed into output in big-endian byte order
// and that the returned pointer advances by exactly 6 bytes.
static void test_Pack3WordsBigEndian_Basic()
{
    // Arrange
    const unsigned short wOut[3] = { 0x1234, 0xABCD, 0x0F0F };
    unsigned char buffer[16];
    std::memset(buffer, 0xAA, sizeof(buffer)); // fill with sentinel value
    unsigned char* start = buffer;

    // Act
    unsigned char* end = Pack3WordsBigEndian(nullptr, (unsigned short*)wOut, start, 0);

    // Assert
    EXPECT_TRUE(end == start + 6);

    unsigned char expected[6];
    expected_big_endian_bytes(wOut, expected);

    for (size_t i = 0; i < 6; ++i)
    {
        EXPECT_EQ(buffer[i], expected[i]);
    }

    // Check that bytes beyond the written region remain untouched (preserve sentinel)
    for (size_t i = 6; i < sizeof(buffer); ++i)
    {
        EXPECT_EQ(buffer[i], 0xAA);
    }
}

// Test 2: Consecutive calls
// Ensures the function can be used repeatedly in sequence and that
// outputs are placed contiguously without overlap.
static void test_Pack3WordsBigEndian_TwoConsecutiveCalls()
{
    // Arrange
    const unsigned short wOut1[3] = { 0x0123, 0x4567, 0x89AB };
    const unsigned short wOut2[3] = { 0xCDEF, 0x1357, 0x9BDF };
    unsigned char buffer[24];
    std::memset(buffer, 0x55, sizeof(buffer));
    unsigned char* start = buffer;

    // Act
    unsigned char* p1 = Pack3WordsBigEndian(nullptr, (unsigned short*)wOut1, start, 0);
    unsigned char* p2 = Pack3WordsBigEndian(nullptr, (unsigned short*)wOut2, p1, 0);

    // Expected little sections
    unsigned char exp1[6];
    unsigned char exp2[6];
    unsigned short tmp1[3] = { wOut1[0], wOut1[1], wOut1[2] };
    unsigned short tmp2[3] = { wOut2[0], wOut2[1], wOut2[2] };
    expected_big_endian_bytes(tmp1, exp1);
    expected_big_endian_bytes(tmp2, exp2);

    // Assert addresses
    EXPECT_TRUE(p1 == start + 6);
    EXPECT_TRUE(p2 == start + 12);

    // Assert first block
    for (size_t i = 0; i < 6; ++i) EXPECT_EQ(buffer[i], exp1[i]);
    // Assert second block
    for (size_t i = 0; i < 6; ++i) EXPECT_EQ(buffer[6 + i], exp2[i]);

    // Assert remaining region untouched
    for (size_t i = 12; i < 24; ++i) EXPECT_EQ(buffer[i], 0x55);
}

// Test 3: Buffer preservation and non-overwriting behavior
// Ensures that only the expected 6 bytes are modified and that
// prior content in the buffer beyond the written region remains intact.
static void test_Pack3WordsBigEndian_BufferPreservation()
{
    // Arrange
    const unsigned short wOut[3] = { 0x1A2B, 0x3C4D, 0x5E6F };
    unsigned char buffer[20];
    // Pre-fill with a pattern
    for (size_t i = 0; i < sizeof(buffer); ++i) buffer[i] = static_cast<unsigned char>(0xA5 + (i & 0x07));

    unsigned char* start = buffer;
    // Act
    unsigned char* end = Pack3WordsBigEndian(nullptr, (unsigned short*)wOut, start, 0);

    // Expected bytes
    unsigned char expected[6];
    expected_big_endian_bytes(wOut, expected);

    // Assert memory for first 6 bytes
    for (size_t i = 0; i < 6; ++i) EXPECT_EQ(buffer[i], expected[i]);

    // Assert the rest remains unchanged
    for (size_t i = 6; i < sizeof(buffer); ++i)
        EXPECT_EQ(buffer[i], static_cast<unsigned char>(0xA5 + (i & 0x07)));
}

// Entry point for test execution
int main()
{
    std::cout << "Starting Pack3WordsBigEndian test suite...\n";

    test_Pack3WordsBigEndian_Basic();
    test_Pack3WordsBigEndian_TwoConsecutiveCalls();
    test_Pack3WordsBigEndian_BufferPreservation();

    if (g_tests_failed == 0) {
        std::cout << "[ALL TESTS PASSED] Pack3WordsBigEndian\n";
        return 0;
    } else {
        std::cerr << "[TESTS FAILED] Pack3WordsBigEndian: " << g_tests_failed << " failure(s) detected\n";
        return 1;
    }
}
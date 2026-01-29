// Test suite for the focal method: adler32
// This test suite is designed to be compiled alongside the production code
// (adler32.c) without using Google Test. It provides a lightweight testing
// harness with non-terminating assertions to maximize code coverage.
//
// Assumptions and notes:
// - The adler32 function under test is declared as:
//     uLong ZEXPORT adler32(uLong adler, const Bytef *buf, uInt len);
// - Bytef is an unsigned byte type, uLong is an unsigned long, and uInt is an unsigned int.
// - The actual implementation of adler32_z (the internal helper) is expected to be
//   provided elsewhere in the project (e.g., by zlib/libc). The tests compare the
//   adler32 result against a reference Adler-32 calculation implemented inside the test.
//
// The test harness focuses on true/false branches through input variations, handling
// of zero-length buffers, and non-trivial seeds to ensure broad coverage.

#include <cstring>
#include <cstdint>
#include <iostream>
#include <vector>
#include <zutil.h>


// Bring in C declarations for the adler32 symbol.
// We declare the function with C linkage to ensure correct name mangling for linking.
extern "C" {
    // The production function under test
    // Note: The actual signature in the project uses types from zutil.h
    // We assume compatible sizes on common platforms (unsigned long, unsigned int, etc.)
    unsigned long adler32(unsigned long adler, const unsigned char *buf, unsigned int len);
}

// Lightweight, self-contained reference implementation of Adler-32 (zlib compatible)
static const unsigned int MOD_ADLER = 65521;

static inline unsigned long adler32_expected(unsigned long adler, const unsigned char *buf, unsigned int len)
{
    unsigned int a = static_cast<unsigned int>(adler & 0xFFFF);
    unsigned int b = static_cast<unsigned int>((adler >> 16) & 0xFFFF);

    for (unsigned int i = 0; i < len; ++i) {
        a += static_cast<unsigned int>(buf[i]);
        if (a >= MOD_ADLER) a -= MOD_ADLER;
        b += a;
        if (b >= MOD_ADLER) b -= MOD_ADLER;
    }
    return static_cast<unsigned long>((b << 16) | a);
}

// Simple non-terminating test assertion framework
static int g_failures = 0;

#define EXPECT_TRUE(cond) \
    do { if(!(cond)) { \
        ++g_failures; \
        std::cerr << "EXPECT_TRUE failed: " #cond " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    } } while(0)

#define EXPECT_EQ(a, b) \
    do { if(!((a) == (b))) { \
        ++g_failures; \
        std::cerr << "EXPECT_EQ failed: " #a " == " #b " -> " << (a) << " vs " << (b) \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    } } while(0)

#define ASSERT_EQ(a, b) EXPECT_EQ(a, b) // Alias for readability when a hard check is preferred

// Test 1: Zero-length buffer should leave the Adler value unchanged
void test_adler32_zero_length_buffer()
{
    // Given: initial adler value and empty buffer
    unsigned long initial = 1; // Standard initial value for Adler-32
    const unsigned char *buf = nullptr;
    unsigned int len = 0;

    // When: calling the function under test
    unsigned long result = adler32(initial, buf, len);

    // Then: result should equal the initial Adler value
    unsigned long expected = adler32_expected(initial, buf, len);

    ASSERT_EQ(result, expected);
    // Explanatory comment: verifies no modifications occur on zero-length input
}

// Test 2: Small known input against a reference Adler-32 computation
void test_adler32_known_abc()
{
    unsigned long initial = 1;
    const unsigned char data[] = { 'a', 'b', 'c' }; // 97, 98, 99
    unsigned int len = 3;

    unsigned long result = adler32(initial, data, len);
    unsigned long expected = adler32_expected(initial, data, len);

    ASSERT_EQ(result, expected);
    // Explanatory comment: validates basic correctness on a short input
}

// Test 3: Large buffer to exercise modulo wrap-around and performance path
void test_adler32_large_buffer()
{
    const size_t N = 70000; // large enough to cross modulus boundaries in typical runs
    std::vector<unsigned char> buffer(N, 0x7F); // fill with a non-zero byte (127)
    unsigned long initial = 1;

    unsigned long result = adler32(initial, buffer.data(), static_cast<unsigned int>(buffer.size()));
    unsigned long expected = adler32_expected(initial, buffer.data(), static_cast<unsigned int>(buffer.size()));

    ASSERT_EQ(result, expected);
    // Explanatory comment: ensures correctness under larger workloads and modulus interactions
}

// Test 4: Non-default seed value to verify proper handling of non-trivial initial adler
void test_adler32_non_default_seed()
{
    unsigned long initial = 0x12345678; // arbitrary non-default seed
    const unsigned char data[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
    unsigned int len = 5;

    unsigned long result = adler32(initial, data, len);
    unsigned long expected = adler32_expected(initial, data, len);

    ASSERT_EQ(result, expected);
    // Explanatory comment: ensures adler32 correctly incorporates a non-default initial state
}

// Runner: execute all tests and report summary
int main()
{
    test_adler32_zero_length_buffer();
    test_adler32_known_abc();
    test_adler32_large_buffer();
    test_adler32_non_default_seed();

    if (g_failures == 0) {
        std::cout << "All adler32 tests passed.\n";
        return 0;
    } else {
        std::cerr << g_failures << " tests failed.\n";
        return g_failures;
    }
}
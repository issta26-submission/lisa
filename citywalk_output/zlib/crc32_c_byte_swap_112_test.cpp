/*
Unit test suite for the focal method: byte_swap
Context:
- The method is defined in crc32.c under conditional compilation blocks depending on W (word width).
- It swaps bytes in a z_word_t word: 8-byte (W==8) and 4-byte (W==4) variants.
- Test targets: validate both branches (W==8 and W==4) where applicable, using the actual function exposed by the build.
- This test is built as a standalone C++11 program (no Google Test). It uses simple, non-terminating assertions to maximize code execution.
- We rely on crc32.h to provide z_word_t typedef and include crc32.c to link the function byte_swap.
- Build notes:
  - Compile the test with -DW=8 to exercise the 8-byte variant, and with -DW=4 to exercise the 4-byte variant.
  - If the environment defines ARMCRC32 or related gating differently, the test will gracefully skip the unavailable variant.
*/

/* Candidate Keywords extracted from the focal method and dependencies:
- z_word_t, word
- 8-byte (W == 8) vs 4-byte (W == 4) swap paths
- bit masks, shifts: >>, <<, &, |
- endian reversal, byte order, reverse bytes
- preprocessor branches (#if W == 8 / #else)
- crc32.c, crc32.h, zutil.h, DYNAMIC_CRC_TABLE
- type portability, unsigned integer behavior
- constexpr-like evaluation via preprocessing
*/

#include <cstdio>
#include <stdatomic.h>
#include <cstdint>
#include <iostream>
#include <zutil.h>
#include <crc32.h>


extern "C" {
// Declaration of the focal function (assumed to have external linkage in crc32.c)
z_word_t byte_swap(z_word_t word);
}

static int g_failures = 0;

// Lightweight non-terminating assertion helpers
#define LOG_FAIL(msg) do { \
    std::cerr << "TEST_FAIL: " << msg << " (line " << __LINE__ << ")\n"; \
} while (0)

#define EXPECT_EQ(actual, expected, msg) do { \
    if (static_cast<z_word_t>(actual) != static_cast<z_word_t>(expected)) { \
        std::cerr << "EXPECT_EQ failed: " << msg \
                  << " | actual=0x" << std::hex << (unsigned long long)(actual) \
                  << " vs expected=0x" << (unsigned long long)(expected) << std::dec \
                  << " (line " << __LINE__ << ")\n"; \
        ++g_failures; \
    } \
} while (0)

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << msg \
                  << " (line " << __LINE__ << ")\n"; \
        ++g_failures; \
    } \
} while (0)

#define EXPECT_FALSE(cond, msg) do { \
    if (cond) { \
        std::cerr << "EXPECT_FALSE failed: " << msg \
                  << " (line " << __LINE__ << ")\n"; \
        ++g_failures; \
    } \
} while (0)

// 8-byte variant tests (W == 8)
#if defined(W) && (W == 8)
static void test_byte_swap_8_basic_patterns() {
    // 1) General 8-byte swap: 0x0123456789ABCDEF -> 0xEFCDAB8967452301
    z_word_t w1 = (z_word_t)0x0123456789ABCDEFULL;
    z_word_t expected1 = (z_word_t)0xEFCDAB8967452301ULL;
    z_word_t res1 = byte_swap(w1);
    EXPECT_EQ(res1, expected1, "8-byte swap: 0x0123456789ABCDEF should become 0xEFCDAB8967452301");

    // 2) Edge: swap of 0x00000000000000FF -> 0xFF00000000000000
    z_word_t w2 = (z_word_t)0x00000000000000FFULL;
    z_word_t expected2 = (z_word_t)0xFF00000000000000ULL;
    z_word_t res2 = byte_swap(w2);
    EXPECT_EQ(res2, expected2, "8-byte swap: 0x00000000000000FF should become 0xFF00000000000000");

    // 3) All ones should be invariant under byte swap
    z_word_t w3 = (z_word_t)0xFFFFFFFFFFFFFFFFULL;
    z_word_t expected3 = (z_word_t)0xFFFFFFFFFFFFFFFFULL;
    z_word_t res3 = byte_swap(w3);
    EXPECT_EQ(res3, expected3, "8-byte swap: 0xFFFFFFFFFFFFFFFF should remain the same");

    // 4) Progressive pattern to verify byte order reversal
    z_word_t w4 = (z_word_t)0x0001020304050607ULL;
    z_word_t expected4 = (z_word_t)0x0706050403020100ULL;
    z_word_t res4 = byte_swap(w4);
    EXPECT_EQ(res4, expected4, "8-byte swap: 0x0001020304050607 should become 0x0706050403020100");

    // 5) Verify that applying swap twice returns the original
    z_word_t orig = (z_word_t)0x1122334455667788ULL;
    z_word_t swapped = byte_swap(orig);
    z_word_t swapped_twice = byte_swap(swapped);
    EXPECT_EQ(swapped_twice, orig, "8-byte swap: double application should return the original value");
}
#endif

// 4-byte variant tests (W == 4)
#if defined(W) && (W == 4)
static void test_byte_swap_4_basic_patterns() {
    // 1) General 4-byte swap: 0xAABBCCDD -> 0xDDCCBBAA
    z_word_t w1 = (z_word_t)0xAABBCCDD;
    z_word_t expected1 = (z_word_t)0xDDCCBBAA;
    z_word_t res1 = byte_swap(w1);
    EXPECT_EQ(res1, expected1, "4-byte swap: 0xAABBCCDD should become 0xDDCCBBAA");

    // 2) Edge: swap of 0x00000001 -> 0x01000000
    z_word_t w2 = (z_word_t)0x00000001;
    z_word_t expected2 = (z_word_t)0x01000000;
    z_word_t res2 = byte_swap(w2);
    EXPECT_EQ(res2, expected2, "4-byte swap: 0x00000001 should become 0x01000000");

    // 3) Edge: swap of 0xFFFFFFFF -> 0xFFFFFFFF
    z_word_t w3 = (z_word_t)0xFFFFFFFF;
    z_word_t expected3 = (z_word_t)0xFFFFFFFF;
    z_word_t res3 = byte_swap(w3);
    EXPECT_EQ(res3, expected3, "4-byte swap: 0xFFFFFFFF should remain 0xFFFFFFFF");

    // 4) Another pattern: 0x01020304 -> 0x04030201
    z_word_t w4 = (z_word_t)0x01020304;
    z_word_t expected4 = (z_word_t)0x04030201;
    z_word_t res4 = byte_swap(w4);
    EXPECT_EQ(res4, expected4, "4-byte swap: 0x01020304 should become 0x04030201");

    // 5) Verify swap twice returns original for 4-byte case
    z_word_t orig = (z_word_t)0x11223344;
    z_word_t swapped = byte_swap(orig);
    z_word_t swapped_twice = byte_swap(swapped);
    EXPECT_EQ(swapped_twice, orig, "4-byte swap: double application should return the original value");
}
#endif

static void run_all_tests() {
#if defined(W) && (W == 8)
    test_byte_swap_8_basic_patterns();
#elif defined(W) && (W == 4)
    test_byte_swap_4_basic_patterns();
#else
    std::cerr << "Note: W is not defined as 8 or 4. Skipping focused byte_swap tests.\n";
#endif
}

int main() {
    g_failures = 0;

    // Run tests conditioned on the active word width
    run_all_tests();

    if (g_failures == 0) {
        std::cout << "All byte_swap tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " byte_swap test(s) failed." << std::endl;
        return g_failures;
    }
}
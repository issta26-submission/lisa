// Test suite for the focal method crc_one_byte in pngfix.c
// This test harness is designed for C++11 without GTest and relies on the
// project's C API (png_uint_32) and the external crc_table used by crc_one_byte.
// The test compares crc_one_byte against a reference CRC-32 update implemented
// in pure C++ (bitwise, using polynomial 0xEDB88320) to ensure correctness.
//
// Important: This test assumes that the build environment provides png.h and that
// the function signature for crc_one_byte matches: png_uint_32 crc_one_byte(png_uint_32, int).
// If your environment uses a different typedef for png_uint_32, adjust the prototype
// accordingly (e.g., using uint32_t from <cstdint>).

#include <cstdint>
#include <setjmp.h>
#include <vector>
#include <string.h>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <png.h>
#include <ctype.h>


#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Declaration of the focal function under test.
// We declare it with C linkage to ensure correct name mangling when linking with pngfix.c
extern "C" png_uint_32 crc_one_byte(png_uint_32 crc, int b);

// Reference implementation: standard CRC-32 bitwise update with polynomial 0xEDB88320
static inline png_uint_32 ref_crc32_update(png_uint_32 crc, int b) {
    // Use only the least significant byte of b as per how crc_one_byte uses (crc ^ b) & 0xff
    png_uint_32 x = crc;
    unsigned int byte = static_cast<unsigned int>(static_cast<unsigned char>(b)); // 0..255
    x ^= byte;
    for (int i = 0; i < 8; ++i) {
        if (x & 1)
            x = (x >> 1) ^ 0xEDB88320u;
        else
            x >>= 1;
    }
    return x;
}

// Simple non-terminating test assertion macro (does not exit on failure)
static int g_failures = 0;
#define CHECK_EQ(actual, expected, msg) do {                               \
    if ((static_cast<png_uint_32>(actual)) != (static_cast<png_uint_32>(expected))) { \
        std::printf("FAIL: %s | expected 0x%08x, got 0x%08x\n", (msg),         \
                    (unsigned)(expected), (unsigned)(actual));            \
        ++g_failures;                                                      \
    }                                                                      \
} while (0)

int main() {
    // Step 2: Unit Test Generation for crc_one_byte
    // - Test across a variety of initial CRC values (crc)
    // - Test with a set of representative bytes (b), including edge cases
    //   such as 0, 1, 255, 128, and -1 to exercise the lower 8 bits masking.
    //
    // This approach validates that crc_one_byte produces the same result as a
    // known-good reference implementation (bitwise CRC-32 update).

    // Candidate initial CRC values to exercise different paths
    std::vector<png_uint_32> initial_crcs = {
        0x00000000u,
        0xFFFFFFFFu,
        0x12345678u,
        0x87654321u
    };

    // Representative bytes to feed into the CRC update, including negatives
    // which are masked to their lower 8 bits when used by crc_one_byte.
    std::vector<int> test_bytes = {
        0, 1, 2, 255, 128, -1  // -1 will be treated as 0xFFFFFFFF -> 0xFF after mask
    };

    // Run first set of exhaustive tests
    for (png_uint_32 init_crc : initial_crcs) {
        for (int b : test_bytes) {
            png_uint_32 expected = ref_crc32_update(init_crc, b);
            png_uint_32 actual   = crc_one_byte(init_crc, b);
            char msg[128];
            std::snprintf(msg, sizeof(msg),
                          "crc_one_byte(crc=0x%08x, b=%d)", (unsigned)init_crc, b);
            CHECK_EQ(actual, expected, msg);
        }
    }

    // Step 3: Test Refinement with additional coverage
    // - Additional random-like sequences to increase coverage on the function
    // - Small loop to vary initial CRC and b values further
    for (int i = 0; i < 8; ++i) {
        png_uint_32 init_crc = static_cast<png_uint_32>(i * 0x1234567u);
        int b = ((i * 37) & 0xFF);
        png_uint_32 expected = ref_crc32_update(init_crc, b);
        png_uint_32 actual   = crc_one_byte(init_crc, b);
        char msg[128];
        std::snprintf(msg, sizeof(msg),
                      "crc_one_byte(crc=0x%08x, b=%d)", (unsigned)init_crc, b);
        CHECK_EQ(actual, expected, msg);
    }

    // Summary
    if (g_failures == 0) {
        std::printf("All crc_one_byte tests passed.\n");
        return 0;
    } else {
        std::fprintf(stderr, "%d crc_one_byte tests failed.\n", g_failures);
        return 1;
    }
}
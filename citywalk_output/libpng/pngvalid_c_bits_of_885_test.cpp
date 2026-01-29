// C++11 unit test suite for the focal method bits_of from pngvalid.c
// This test assumes the environment provides the libpng type png_uint_32
// and the symbol bits_of with C linkage.
// The tests are self-contained and do not rely on Google Test; a simple
// in-house test harness is used.

#include <cstdint>
#include <signal.h>
#include <string.h>
#include <string>
#include <float.h>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <png.h>
#include <math.h>


// Include libpng's type to ensure correct prototype.
// If the environment provides libpng headers, this will resolve correctly.

// Declaration of the focal function under test.
// The function signature in the original C file is:
//   png_uint_32 bits_of(png_uint_32 num);
extern "C" png_uint_32 bits_of(png_uint_32 num);

// Helper: compute expected bit-length (0 for 0, else floor(log2(n)) + 1)
static png_uint_32 expected_bits(png_uint_32 n) {
    if (n == 0)
        return 0;
    png_uint_32 b = 0;
    while (n != 0) {
        ++b;
        n >>= 1;
    }
    return b;
}

// Simple test harness: run a single case and report pass/fail with a description.
// This mirrors non-terminating assertions (i.e., does not abort on failure).
static bool run_case(png_uint_32 input, const char* description) {
    png_uint_32 expected = expected_bits(input);
    png_uint_32 actual = bits_of(input);

    bool ok = (actual == expected);
    if (ok) {
        std::printf("PASS: %s (input=0x%08x, expected=%u, actual=%u)\n",
                    description,
                    static_cast<unsigned int>(input),
                    static_cast<unsigned int>(expected),
                    static_cast<unsigned int>(actual));
    } else {
        std::fprintf(stderr,
                     "FAIL: %s (input=0x%08x, expected=%u, actual=%u)\n",
                     description,
                     static_cast<unsigned int>(input),
                     static_cast<unsigned int>(expected),
                     static_cast<unsigned int>(actual));
    }
    return ok;
}

// Macro to simplify test descriptions and ensure coverage notes are clear.
#define TEST_DESC(s) (s)

int main(void) {
    // Coverage goals:
    // - Branch coverage for each if condition in bits_of:
    //   1) num & 0xffff0000U
    //   2) num & 0xff00U
    //   3) num & 0xf0U
    //   4) num & 0xcU
    //   5) num & 0x2U
    //   6) if (num)
    // The following test inputs are chosen to trigger true/false for each branch.

    int total = 0;
    int passed = 0;

    // 0: All branches are false; ensures 0 is returned and the final 'if (num)' is skipped.
    total += 1;
    if (run_case(0, TEST_DESC("Zero input: all branch predicates false, expect 0"))) ++passed;

    // 1: Only the final 'if (num)' is true; tests the last increment path when msb is at 0.
    total += 1;
    if (run_case(1, TEST_DESC("Input 1: only lowest bit set, expect 1"))) ++passed;

    // 0x10000 (2^16): triggers the first if (num & 0xffff0000U) and then falls through.
    // Tests path where b becomes 16 and num >>= 16.
    total += 1;
    if (run_case(0x00010000U, TEST_DESC("Input 0x00010000: first 16-bit block true, rest false"))) ++passed;

    // 0x00FF0000: first if true, second if false, third if true; exercises multiple branches.
    total += 1;
    if (run_case(0x00FF0000U, TEST_DESC("Input 0x00FF0000: first and third branches taken (second false)"))) ++passed;

    // 0xFFFF0000: all branches true; exercises full depth of the chain.
    total += 1;
    if (run_case(0xFFFF0000U, TEST_DESC("Input 0xFFFF0000: all branches true, expect 32"))) ++passed;

    // 0x0000FFFF: high bits zero, lower 16 bits set; tests second-level branch (num & 0xff00U) false, final true.
    total += 1;
    if (run_case(0x0000FFFFU, TEST_DESC("Input 0x0000FFFF: lower 16 bits set, expect 16"))) ++passed;

    // 0xFFFFFFFF: all 32 bits set; ensures 32 is returned.
    total += 1;
    if (run_case(0xFFFFFFFFU, TEST_DESC("Input 0xFFFFFFFF: all bits set, expect 32"))) ++passed;

    // 0x7FFFFFFF: top bit cleared (31 bits needed)
    total += 1;
    if (run_case(0x7FFFFFFFU, TEST_DESC("Input 0x7FFFFFFF: top bit not set, expect 31"))) ++passed;

    // 0x80000000: only the top bit set; expect 32
    total += 1;
    if (run_case(0x80000000U, TEST_DESC("Input 0x80000000: top bit set, expect 32"))) ++passed;

    // 0x00001000: small number with a bit in the middle; tests mid-range branches
    total += 1;
    if (run_case(0x00001000U, TEST_DESC("Input 0x00001000: mid-range bit, expect 13"))) ++passed;

    // 0x00000080: single bit in lower region; ensures branch coverage for lower 8 bits
    total += 1;
    if (run_case(0x00000080U, TEST_DESC("Input 0x00000080: single bit set at 7th position, expect 8"))) ++passed;

    // 0x00000001: smallest non-zero; verify returns 1
    total += 1;
    if (run_case(0x00000001U, TEST_DESC("Input 0x00000001: lowest bit only, expect 1"))) ++passed;

    // 0x80000001: highest and lowest bit set; tests combination
    total += 1;
    if (run_case(0x80000001U, TEST_DESC("Input 0x80000001: top and bottom bits, expect 32"))) ++passed;

    // 0x0000F000: high nibble in lower 16 bits; ensures branch in 0xf0U region
    total += 1;
    if (run_case(0x0000F000U, TEST_DESC("Input 0x0000F000: high nibble in lower 16, expect 16"))) ++passed;

    std::printf("Summary: %d tests total, %d passed.\n", total, passed);

    // Return non-zero if any test failed; exit code 0 means all tests passed.
    return (passed == total) ? 0 : 1;
}
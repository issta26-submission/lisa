/*
Unit test suite for the focal method:
  format_isset(format_list *pf, png_uint_32 format)

Context and rationale:
- The function checks if a given format index is represented in the bitfield pf->bits.
- Core logic: return format < FORMAT_COUNT && (pf->bits[format >> 5] & (1U << (format & 31))) != 0;
- We design tests to cover:
  1) True path when the bit is set for a valid format.
  2) False path when the bit is not set for a valid format.
  3) False path when the format index is out-of-range (format >= FORMAT_COUNT) to exercise short-circuit evaluation.
  4) Additional true path for a non-zero 32-block offset (format >= 32).
Note: We avoid GTest and implement a simple in-process test harness that uses non-terminating assertions to maximize code execution.

Candidate Keywords mapped to test goals:
- format_list, pf->bits, bit operations, 32-bit masks, FORMAT_COUNT, format indexes, short-circuit evaluation, boundary conditions.

This test suite is designed to be compiled and linked with the existing pngstest.c (or its accompanying headers) in a C++11-capable environment. It declares a compatible format_list struct layout sufficient to access pf->bits[...], and calls the C function format_isset with a C linkage.

*/

#include <cstdint>
#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <ctype.h>


// Provide a C-compatible declaration for the focal function.
// We declare a compatible (matching) struct type for format_list.
// In the real project, format_list is defined in the pngstest headers.
// Here we provide a compatible minimal layout sufficient for testing pf->bits access.
extern "C" {
    // The real declaration in pngstest.c is:
    // int format_isset(format_list *pf, png_uint_32 format);
    // We provide a prototype with C linkage.
    typedef struct format_list {
        unsigned int bits[4]; // Sufficient for test coverage (covers formats 0..127)
    } format_list;

    // Use a compatible typedef for png_uint_32 (assume 32-bit unsigned).
    typedef uint32_t png_uint_32;

    int format_isset(format_list *pf, png_uint_32 format);
}

// Simple non-terminating test harness (no exceptions, no aborts).
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define EXPECT_TRUE(cond, msg)                                  \
    do {                                                         \
        tests_run++;                                             \
        if (cond) { tests_passed++; }                          \
        else {                                                  \
            tests_failed++;                                     \
            fprintf(stderr, "EXPECT TRUE FAILED: %s\n", (msg)); \
        }                                                       \
    } while (0)

#define EXPECT_FALSE(cond, msg)                                 \
    do {                                                         \
        tests_run++;                                             \
        if (!(cond)) { tests_passed++; }                       \
        else {                                                   \
            tests_failed++;                                      \
            fprintf(stderr, "EXPECT FALSE FAILED: %s\n", (msg));\
        }                                                        \
    } while (0)

// Unit test 1: True path when the bit corresponding to format 0 is set.
void test_format_isset_true_case() {
    // Setup: pf.bits[0] has bit 0 set -> format 0 is represented.
    format_list pf;
    std::memset(pf.bits, 0, sizeof(pf.bits));
    pf.bits[0] = 0x00000001u; // bit 0 set

    png_uint_32 format = 0; // within range, bit set
    int result = format_isset(&pf, format);

    EXPECT_TRUE(result != 0, "format_isset should return true when bit 0 is set");
}

// Unit test 2: False path when the bit is not set for a valid format.
void test_format_isset_false_case_bit_not_set() {
    // Setup: pf.bits[0] has no bits set
    format_list pf;
    std::memset(pf.bits, 0, sizeof(pf.bits));

    png_uint_32 format = 0; // within range, bit not set
    int result = format_isset(&pf, format);

    EXPECT_FALSE(result != 0, "format_isset should return false when bit 0 is not set");
}

// Unit test 3: False path due to out-of-range format (short-circuit before pf->bits access).
void test_format_isset_false_case_out_of_range() {
    // pf contents are irrelevant because format >= FORMAT_COUNT triggers short-circuit.
    format_list pf;
    std::memset(pf.bits, 0xAA, sizeof(pf.bits)); // arbitrary content

    // Use a value extremely large to guarantee format < FORMAT_COUNT is false.
    png_uint_32 format = 0xFFFFFFFFu;
    int result = format_isset(&pf, format);

    EXPECT_FALSE(result != 0, "format_isset should return false for out-of-range format (short-circuit)");
}

// Unit test 4: True path for a non-zero 32-bit block (format = 32, uses pf.bits[1]).
void test_format_isset_true_case_format_32() {
    // Setup: pf.bits[1] has bit 0 set (format 32 corresponds to bits[1], bit 0)
    format_list pf;
    std::memset(pf.bits, 0, sizeof(pf.bits));
    pf.bits[1] = 0x00000001u; // bit 0 in second 32-bit block set

    png_uint_32 format = 32; // within range, bit set at bits[1]
    int result = format_isset(&pf, format);

    EXPECT_TRUE(result != 0, "format_isset should return true when format 32 is set (pf.bits[1] bit 0)");
}

// Simple test runner
int main(void) {
    // Run tests
    test_format_isset_true_case();
    test_format_isset_false_case_bit_not_set();
    test_format_isset_false_case_out_of_range();
    test_format_isset_true_case_format_32();

    // Summary
    const char *summary =
        "\nTest Summary:\n"
        "  Total tests   : %d\n"
        "  Passed        : %d\n"
        "  Failed        : %d\n";

    printf(summary, tests_run, tests_passed, tests_failed);

    // Return non-zero if any test failed to aid automation pipelines.
    return (tests_failed == 0) ? 0 : 1;
}
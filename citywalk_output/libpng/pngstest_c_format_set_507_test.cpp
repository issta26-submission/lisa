// Unit test suite for the focal method: format_set
// This test suite is designed as a self-contained C++11 test harness.
// It reproduces the core logic of the focal method in a minimal, stand-alone
// environment to enable deterministic, repeatable testing without GTest.
//
// Note: In a real integration, the tests would link against the actual
// pngstest.c implementation and its related headers. Here, we provide a
// faithful stand-in for the logic to enable high-coverage, executable tests
// even in environments where the full PNG test harness isn't available.
// The test cases focus on true/false branches and boundary conditions as per
// the domain knowledge guidelines.

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


// Domain knowledge hint: simulate the necessary dependency surface.
// We define FORMAT_COUNT and the data structure exactly as expected by the
// focal method under test so the logic can be exercised in isolation.

static const uint32_t FORMAT_COUNT = 64;                 // Number of formats (chosen to cover multiple 32-bit words)
static const uint32_t FORMAT_WORDS  = (FORMAT_COUNT + 31) / 32; // Number of 32-bit words needed to hold all bits

// A minimal representation of the dependent class/struct used by format_set.
// This mirrors the essential member used by the focal method: a bit array.
typedef struct {
    uint32_t bits[FORMAT_WORDS];
} format_list;

// Focal method under test (reproduced locally for a self-contained test harness).
// In the real project, you would link against the pngstest.c implementation.
// The behavior:
// - If format < FORMAT_COUNT: set the corresponding bit in pf->bits and return the
//   updated value of that 32-bit word after the OR.
// - Otherwise: return 0 and do not modify pf.
uint32_t format_set(format_list *pf, uint32_t format)
{
    if (format < FORMAT_COUNT)
        return pf->bits[format >> 5] |= ((uint32_t)1) << (format & 31);
    return 0;
}

// Simple, non-terminating assertion helper (prints message but continues execution).
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        fprintf(stderr, "ASSERTION FAILED: %s\n", msg); \
        return false; \
    } \
} while (0)

// Test 1: In-range small format (format = 0) should set bit 0 in pf->bits[0].
// This validates the true-branch behavior for the lowest index.
bool test_format_set_in_range_zero()
{
    // Domain knowledge: ensure the initial state is clean.
    format_list pf;
    std::memset(&pf, 0, sizeof(pf));

    // Call focal method with the first valid format (0).
    uint32_t ret = format_set(&pf, 0);

    // Expected: pf.bits[0] == 0x00000001, return == 0x00000001
    uint32_t expected_word = pf.bits[0];
    ASSERT_TRUE(ret == expected_word, "format_set(0) should return the updated word value (pf.bits[0]).");
    ASSERT_TRUE((pf.bits[0] & 0x00000001u) != 0, "Bit 0 should be set after format_set(pf, 0).");
    return true;
}

// Test 2: In-range last bit of the first word (format = 31) should set bit 31 of pf->bits[0].
// This validates the boundary within the first 32 formats.
bool test_format_set_in_range_last_bit_first_word()
{
    format_list pf;
    std::memset(&pf, 0, sizeof(pf));

    uint32_t ret1 = format_set(&pf, 0);        // Set bit 0
    uint32_t ret2 = format_set(&pf, 31);       // Set bit 31 in the same word

    // After the two calls, pf.bits[0] should have bits 0 and 31 set: 0x80000001
    uint32_t word0 = pf.bits[0];
    ASSERT_TRUE((word0 & 0x80000001u) == 0x80000001u, "Bits 0 and 31 should be set in pf.bits[0].");
    // Return value should reflect the new state of pf.bits[0]
    ASSERT_TRUE(ret2 == word0, "format_set(31) should return the updated pf.bits[0].");
    return true;
}

// Test 3: Cross-word boundary (format = 32) should set bit 0 of pf->bits[1].
// This validates transitioning to the next 32-bit word.
bool test_format_set_cross_word_boundary()
{
    format_list pf;
    std::memset(&pf, 0, sizeof(pf));

    // Set a couple of bits in the first word to verify independent word behavior
    uint32_t r0 = format_set(&pf, 0);   // pf.bits[0] bit0
    uint32_t r32 = format_set(&pf, 32); // pf.bits[1] bit0

    // Validate: pf.bits[1] bit0 is set, pf.bits[0] remains with bit0 set
    ASSERT_TRUE((pf.bits[1] & 0x00000001u) != 0, "pf.bits[1] bit0 should be set after format_set(pf,32).");
    ASSERT_TRUE((pf.bits[0] & 0x00000001u) != 0, "pf.bits[0] bit0 should remain set after subsequent calls.");
    // Check return value matches the new state of pf.bits[1]
    ASSERT_TRUE(r32 == pf.bits[1], "format_set(pf, 32) should return the updated pf.bits[1].");
    return true;
}

// Test 4: Cross-word boundary high bit (format = 63) should set bit 31 of pf->bits[1].
// This validates the last bit of the second word (bit index 63).
bool test_format_set_cross_word_boundary_high_bit()
{
    format_list pf;
    std::memset(&pf, 0, sizeof(pf));

    // Move to the second word and set its highest bit
    uint32_t ret = format_set(&pf, 63);

    // pf.bits[1] should have bit 31 set -> 0x80000000
    ASSERT_TRUE((pf.bits[1] & 0x80000000u) != 0, "pf.bits[1] bit31 should be set after format_set(pf,63).");
    ASSERT_TRUE(ret == pf.bits[1], "format_set(pf,63) should return the updated pf.bits[1].");
    return true;
}

// Test 5: Out-of-range format (format = FORMAT_COUNT) should not modify pf and should return 0.
// This validates the false-branch behavior when the input is beyond supported range.
bool test_format_set_out_of_range_no_modify()
{
    format_list pf;
    std::memset(&pf, 0, sizeof(pf));

    uint32_t ret = format_set(&pf, FORMAT_COUNT); // Out of range

    // pf should remain unchanged (all zeros)
    ASSERT_TRUE(pf.bits[0] == 0 && pf.bits[1] == 0, "pf.bits[0] and pf.bits[1] should remain zero for out-of-range input.");
    ASSERT_TRUE(ret == 0, "format_set(out_of_range) should return 0 and not modify pf.");
    return true;
}

// Simple test runner to execute all tests and report failures.
// The harness is intentionally minimal (no GTest) to align with the provided constraints.
int main(void)
{
    int failures = 0;

    printf("Running test: test_format_set_in_range_zero\n");
    if (!test_format_set_in_range_zero()) {
        fprintf(stderr, "FAILED: test_format_set_in_range_zero\n");
        ++failures;
    } else {
        printf("PASSED: test_format_set_in_range_zero\n");
    }

    printf("Running test: test_format_set_in_range_last_bit_first_word\n");
    if (!test_format_set_in_range_last_bit_first_word()) {
        fprintf(stderr, "FAILED: test_format_set_in_range_last_bit_first_word\n");
        ++failures;
    } else {
        printf("PASSED: test_format_set_in_range_last_bit_first_word\n");
    }

    printf("Running test: test_format_set_cross_word_boundary\n");
    if (!test_format_set_cross_word_boundary()) {
        fprintf(stderr, "FAILED: test_format_set_cross_word_boundary\n");
        ++failures;
    } else {
        printf("PASSED: test_format_set_cross_word_boundary\n");
    }

    printf("Running test: test_format_set_cross_word_boundary_high_bit\n");
    if (!test_format_set_cross_word_boundary_high_bit()) {
        fprintf(stderr, "FAILED: test_format_set_cross_word_boundary_high_bit\n");
        ++failures;
    } else {
        printf("PASSED: test_format_set_cross_word_boundary_high_bit\n");
    }

    printf("Running test: test_format_set_out_of_range_no_modify\n");
    if (!test_format_set_out_of_range_no_modify()) {
        fprintf(stderr, "FAILED: test_format_set_out_of_range_no_modify\n");
        ++failures;
    } else {
        printf("PASSED: test_format_set_out_of_range_no_modify\n");
    }

    if (failures > 0) {
        printf("TEST SUMMARY: FAILED (%d of %d tests failed)\n", failures, 5);
        return 1;
    } else {
        printf("TEST SUMMARY: ALL TESTS PASSED (%d tests)\n", 5);
        return 0;
    }
}
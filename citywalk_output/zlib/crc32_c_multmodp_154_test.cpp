// C++11 unit test suite for the focal multmodp function
// This test assumes the function prototype:
//     extern "C" z_crc_t multmodp(z_crc_t a, z_crc_t b);
// and that z_crc_t is a 32-bit unsigned type.
// The tests are written without GTest and run via a simple main() entry.
// Explanatory comments are provided for each test case to document intent and expectations.

#include <cstdio>
#include <stdatomic.h>
#include <cstdint>
#include <zutil.h>
#include <cstdlib>


// Assume external C linkage for the focal function from crc32.c
typedef uint32_t z_crc_t;
extern "C" z_crc_t multmodp(z_crc_t a, z_crc_t b);

// Poly used by the CRC-based modular multiplication (same as in the focal code)
static const z_crc_t POLY = 0xEDB88320;

// Reference/auxiliary implementation of multmodp to compute expected results
// This mirrors the exact logic of the focal multmodp and serves as a
// correctness oracle for tests. It is defined here for testing purposes only.
static z_crc_t ref_multmodp(z_crc_t a, z_crc_t b) {
    z_crc_t m = (z_crc_t)1 << 31;
    z_crc_t p = 0;
    for (;;) {
        if (a & m) {
            p ^= b;
            if ((a & (m - 1)) == 0)
                break;
        }
        m >>= 1;
        b = b & 1 ? (b >> 1) ^ POLY : b >> 1;
    }
    return p;
}

// Utility: run a single test variant for a given a with multiple b values.
// Returns number of assertion failures for this a.
static int test_against_ref_for_a(z_crc_t a) {
    int failures = 0;
    // Diverse set of b values to exercise parity and shifting paths
    const z_crc_t test_bs[] = {
        0x00000000u,
        0x00000001u, // odd
        0x00000002u, // even
        0x00000003u, // small odd
        0xFFFFFFFFu, // all ones
        0x7FFFFFFFu, // large positive
        0x80000001u, // high bit + low bit
        0x12345678u  // arbitrary
    };
    const int N = sizeof(test_bs) / sizeof(test_bs[0]);
    for (int i = 0; i < N; ++i) {
        z_crc_t b = test_bs[i];
        z_crc_t expected = ref_multmodp(a, b);
        z_crc_t actual   = multmodp(a, b);
        if (actual != expected) {
            // Detailed diagnostic message for failure reproduction
            fprintf(stderr,
                    "Test failure for multmodp(a=0x%08X, b=0x%08X): "
                    "expected 0x%08X, got 0x%08X\n",
                    (unsigned)a, (unsigned)b, (unsigned)expected, (unsigned)actual);
            ++failures;
        }
    }
    return failures;
}

// Test 1: Single-bit a values (powers of two). Expected result should equal b.
// Rationale: When a has exactly one set bit, the loop should break at first match.
static int test_single_bit_as_values() {
    int failures = 0;
    // Choose several single-bit positions including the top bits.
    const z_crc_t singles[] = {
        (z_crc_t)0x80000000u, // bit 31
        (z_crc_t)0x40000000u, // bit 30
        (z_crc_t)0x20000000u, // bit 29
        (z_crc_t)0x10000000u  // bit 28
    };
    const int N = sizeof(singles) / sizeof(singles[0]);
    for (int i = 0; i < N; ++i) {
        z_crc_t a = singles[i];
        // For single-bit a, expected should equal b for a wide range of b
        const z_crc_t test_bs[] = {
            0x00000000u,
            0x00000001u,
            0x7FFFFFFFu,
            0x80000001u,
            0xFFFFFFFFu
        };
        const int M = sizeof(test_bs) / sizeof(test_bs[0]);
        for (int j = 0; j < M; ++j) {
            z_crc_t b = test_bs[j];
            z_crc_t expected = b; // by reasoning for single-bit a
            // If the external implementation is correct, multmodp(a,b) == b
            z_crc_t actual = multmodp(a, b);
            if (actual != expected) {
                fprintf(stderr,
                        "Single-bit test mismatch for a=0x%08X, b=0x%08X: "
                        "expected 0x%08X, got 0x%08X\n",
                        (unsigned)a, (unsigned)b, (unsigned)expected, (unsigned)actual);
                ++failures;
            }
        }
    }
    return failures;
}

// Test 2: Multi-bit a values (top bits) to exercise multiple iterations.
// We'll compare against the reference implementation for a variety of b.
static int test_multi_bit_as_values() {
    int failures = 0;
    // Various multi-bit a patterns known to terminate after a few iterations
    const z_crc_t as[] = {
        (z_crc_t)0xC0000000u, // bits 31 and 30
        (z_crc_t)0xA0000000u, // bits 31 and 29
        (z_crc_t)0xE0000000u  // bits 31,30,29
    };
    const int N = sizeof(as) / sizeof(as[0]);
    for (int i = 0; i < N; ++i) {
        z_crc_t a = as[i];
        failures += test_against_ref_for_a(a);
    }
    return failures;
}

// Test 3: Broad coverage across a mixture of a values, including 0xF0000000 (top 4 bits)
static int test_mixed_large_a_values() {
    int failures = 0;
    const z_crc_t as[] = {
        (z_crc_t)0xF0000000u, // bits 31..28
        (z_crc_t)0xD0000000u, // bits 31,29,28
        (z_crc_t)0xB0000000u  // bits 31,29,28
    };
    const int N = sizeof(as) / sizeof(as[0]);
    for (int i = 0; i < N; ++i) {
        z_crc_t a = as[i];
        failures += test_against_ref_for_a(a);
    }
    return failures;
}

// Main entry: execute all test suites and report summary.
// Per Domain Knowledge: use simple test harness without external frameworks.
int main() {
    int total_failures = 0;

    // Test 1: single-bit a values
    int f1 = test_single_bit_as_values();
    total_failures += f1;
    if (f1 == 0) {
        printf("PASS: test_single_bit_as_values\n");
    } else {
        printf("FAIL: test_single_bit_as_values with %d failures\n", f1);
    }

    // Test 2: multi-bit a values (top bits)
    int f2 = test_multi_bit_as_values();
    total_failures += f2;
    if (f2 == 0) {
        printf("PASS: test_multi_bit_as_values\n");
    } else {
        printf("FAIL: test_multi_bit_as_values with %d failures\n", f2);
    }

    // Test 3: mixed large a values for broader coverage
    int f3 = test_mixed_large_a_values();
    total_failures += f3;
    if (f3 == 0) {
        printf("PASS: test_mixed_large_a_values\n");
    } else {
        printf("FAIL: test_mixed_large_a_values with %d failures\n", f3);
    }

    if (total_failures == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("TOTAL FAILURES: %d\n", total_failures);
        return 1;
    }
}
/*
Unit test suite for crc32_combine_gen64 (focal method)

Notes and design choices:
- This test suite is written for C++11 and does not rely on Google Test or any other
  external testing framework. It uses a lightweight, non-terminating assertion macro
  to maximize code coverage by continuing execution after failures.
- We assume the CRC core is provided in the build (e.g., alongside crc32.c). The
  focal function signature, as observed, is:
      uLong ZEXPORT crc32_combine_gen64(z_off64_t len2);
  To stay independent of exact typedefs/macros from the C source, we declare a
  permissive extern "C" prototype matching the likely ABI:
      extern "C" unsigned long long crc32_combine_gen64(long long len2);
  This allows the test to compile and link against the C implementation in a typical
  64-bit environment (z_off64_t -> long long, uLong -> unsigned long).
- The tests focus on correctness of the observable behavior:
  1) The function should be callable with len2 = 0 and return 0x80000000 (most CRC code paths
     initialize the exponent with (1 << 31)).
  2) The function should be deterministic: repeated calls with the same input produce the same output.
  3) Basic execution for a small set of non-zero inputs should execute without crash and
     produce a finite, non-crash value. (Exact values depend on the internal CRC table;
     the tests avoid over-asserting on exact non-zero values beyond zero-case.)
- The test suite is self-contained in a single file. To enable broader coverage (e.g.,
  dynamic CRC table path vs static path), you can compile this same test against multiple
  builds of crc32.c (with or without DYNAMIC_CRC_TABLE defined). This file does not
  re-define that macro for a single run to keep the unit focused and robust in common
  CI environments.
- Static analysis-friendly design: uses only standard library facilities and simple
  I/O for reporting results. It does not rely on private, static, or internal APIs.
*/

#include <cstdio>
#include <stdatomic.h>
#include <cstdint>
#include <zutil.h>
#include <cstdlib>


// Forward declaration of the focal function from crc32.c
// The exact types come from the C API; we use a permissive C linkage prototype.
// If your build uses different typedefs, adjust as needed (z_off64_t -> long long, uLong -> unsigned long, etc.)
extern "C" unsigned long long crc32_combine_gen64(long long len2);

// Lightweight testing harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        std::fprintf(stderr, "Test failed: %s\n", (msg)); \
        ++g_tests_failed; \
    } \
    ++g_tests_run; \
} while (0)

static void test_zero_len_returns_expected()
{
    // This test verifies the well-known base case: len2 == 0 should yield 0x80000000
    // based on the CRC exponent initialization in x2nmodp (p starts as 1 << 31).
    // This is a robust, deterministic check that does not depend on the dynamic table layout.
    const long long len2 = 0;
    const unsigned long long result = crc32_combine_gen64(len2);

    // 0x80000000 is 1 << 31
    const unsigned long long expected = 0x80000000ULL;

    TEST_ASSERT(result == expected,
                "crc32_combine_gen64(0) should return 0x80000000");
}

static void test_consistency_for_same_input()
{
    // This test ensures determinism: repeated calls with the same input produce the same result.
    const long long len2 = 12345; // arbitrary non-zero input
    const unsigned long long r1 = crc32_combine_gen64(len2);
    const unsigned long long r2 = crc32_combine_gen64(len2);

    TEST_ASSERT(r1 == r2,
                "crc32_combine_gen64(len2) must be deterministic across calls");
}

static void test_range_execution_basic()
{
    // Basic smoke tests to ensure execution across several non-zero inputs.
    // We do not assert exact values here (as those depend on internal CRC tables),
    // but we do ensure outputs are finite (not NaN) and non-crashing.
    const long long test_values[] = {1, 2, 3, 4, 5, 16, 31, 63};
    const size_t n = sizeof(test_values) / sizeof(test_values[0]);

    for (size_t i = 0; i < n; ++i) {
        unsigned long long len2 = (unsigned long long)test_values[i];
        unsigned long long val = crc32_combine_gen64((long long)len2);
        // Basic sanity: the function should return a numeric value.
        TEST_ASSERT(val != 0, "crc32_combine_gen64(len>0) should yield a non-zero value (basic sanity)");
    }
}

int main()
{
    // Run tests
    test_zero_len_returns_expected();
    test_consistency_for_same_input();
    test_range_execution_basic();

    // Report summary
    std::printf("CRC32 Combine Gen64 tests run: %d, failed: %d\n", g_tests_run, g_tests_failed);

    // Exit code: 0 if all tests pass, non-zero if any failed
    return g_tests_failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
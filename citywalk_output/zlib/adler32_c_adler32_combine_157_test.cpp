// Adler-32 Combine Unit Test Suite (C++11, no GTest)
// This test suite targets the focal function:
//     uLong adler32_combine(uLong adler1, uLong adler2, z_off_t len2);
// which is defined in adler32.c and delegates to an internal static helper.
// The tests rely on a self-contained reference implementation of Adler-32
// to verify the correctness of the combination logic by comparing against
// the Adler-32 of the concatenated data.
// Notes:
// - This test uses zlib's adler32_combine signature via z_off_t (zlib.h).
// - If zlib is not available in the environment, this test will not compile.
// - The test does not terminate on failure; it reports failures and continues.

#include <cstring>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <zutil.h>


// Include zlib to obtain the correct symbol prototype for adler32_combine
// and the z_off_t type used by the function.

// Ensure C linkage for the adler32_combine symbol when calling from C++
extern "C" {
    // Prototype exactly as provided by zlib for adler32_combine
    // (Some environments expose this via <zlib.h> already; we declare again with C linkage to be explicit.)
    uLong adler32_combine(uLong adler1, uLong adler2, z_off_t len2);
}

// Local helper: reference Adler-32 implementation (as used by zlib)
static uint32_t adler32_ref(const uint8_t* data, size_t len) {
    const uint32_t MOD_ADLER = 65521;
    uint32_t a = 1;
    uint32_t b = 0;
    for (size_t i = 0; i < len; ++i) {
        a += data[i];
        if (a >= MOD_ADLER) a -= MOD_ADLER;
        b += a;
        if (b >= MOD_ADLER) b -= MOD_ADLER;
    }
    return (b << 16) | a;
}

// Overload for std::vector<uint8_t>
static uint32_t adler32_ref(const std::vector<uint8_t>& data) {
    return adler32_ref(data.data(), data.size());
}

// Global test failure counter (non-terminating assertions)
static int g_test_failures = 0;

// Simple assertion helper: checks equality, reports but does not abort
#define TEST_ASSERT_EQ(actual, expected, msg) do { \
    if ((actual) != (expected)) { \
        ++g_test_failures; \
        std::cerr << "[FAIL] " << msg \
                  << " | Expected: " << (expected) \
                  << ", Actual: " << (actual) << std::endl; \
    } else { \
        std::cout << "[PASS] " << msg << std::endl; \
    } \
} while (0)

// Test 1: Empty first block and empty second block -> Adler of empty data (1)
static void test_adler32_combine_empty_empty() {
    // Data: both blocks empty
    std::vector<uint8_t> d1;
    std::vector<uint8_t> d2;

    uint32_t adler1 = adler32_ref(d1);
    uint32_t adler2 = adler32_ref(d2);
    z_off_t len2 = static_cast<z_off_t>(d2.size());

    uint32_t result = static_cast<uint32_t>(adler32_combine(adler1, adler2, len2));

    // Expected: Adler32 of concatenation (empty)
    std::vector<uint8_t> concat; // empty
    uint32_t expected = adler32_ref(concat);

    TEST_ASSERT_EQ(result, expected, "adler32_combine with two empty blocks equals Adler of empty");
}

// Test 2: Simple concatenation of two small blocks
// Validate that adler32_combine(a1, a2, len2) equals Adler32 of data1 + data2
static void test_adler32_combine_basic_concat() {
    // Data blocks
    const char *s1 = "Hello";
    const char *s2 = "World";
    std::vector<uint8_t> d1(reinterpret_cast<const uint8_t*>(s1),
                            reinterpret_cast<const uint8_t*>(s1) + std::strlen(s1));
    std::vector<uint8_t> d2(reinterpret_cast<const uint8_t*>(s2),
                            reinterpret_cast<const uint8_t*>(s2) + std::strlen(s2));

    uint32_t adler1 = adler32_ref(d1);
    uint32_t adler2 = adler32_ref(d2);
    z_off_t len2 = static_cast<z_off_t>(d2.size());

    uint32_t result = static_cast<uint32_t>(adler32_combine(adler1, adler2, len2));

    // Expected: Adler32 of "HelloWorld"
    std::vector<uint8_t> concat = d1;
    concat.insert(concat.end(), d2.begin(), d2.end());
    uint32_t expected = adler32_ref(concat);

    TEST_ASSERT_EQ(result, expected, "adler32_combine basic concatenation of 'Hello' + 'World'");
}

// Test 3: Larger data blocks to exercise the algorithm
// Use deterministic random data for reproducibility
static void test_adler32_combine_large_blocks() {
    // Generate 256 bytes for data1, 512 bytes for data2 with a simple LCG
    auto fill_blocks = [](size_t n, uint8_t seed, std::vector<uint8_t>& out) {
        out.resize(n);
        uint32_t x = static_cast<uint32_t>(seed);
        for (size_t i = 0; i < n; ++i) {
            x = (1664525u * x + 1013904223u); // LCG
            out[i] = static_cast<uint8_t>(x & 0xFF);
        }
    };

    std::vector<uint8_t> d1, d2;
    fill_blocks(256, 123, d1);
    fill_blocks(512, 45, d2);

    uint32_t adler1 = adler32_ref(d1);
    uint32_t adler2 = adler32_ref(d2);
    z_off_t len2 = static_cast<z_off_t>(d2.size());

    uint32_t result = static_cast<uint32_t>(adler32_combine(adler1, adler2, len2));

    // Expected: Adler32 of d1 concatenated with d2
    std::vector<uint8_t> concat = d1;
    concat.insert(concat.end(), d2.begin(), d2.end());
    uint32_t expected = adler32_ref(concat);

    TEST_ASSERT_EQ(result, expected, "adler32_combine with large blocks (256 + 512 bytes)");
}

// Test 4: Property: len2 == 0 should equal adler1 (second block length zero)
// In this case, the result should be independent of adler2 when len2 is zero
static void test_adler32_combine_len2_zero_property() {
    // Data blocks
    const char *s1 = "DataBlockOne";
    const char *s2 = "UnusedSecondBlock";
    std::vector<uint8_t> d1(reinterpret_cast<const uint8_t*>(s1),
                            reinterpret_cast<const uint8_t*>(s1) + std::strlen(s1));
    std::vector<uint8_t> d2(reinterpret_cast<const uint8_t*>(s2),
                            reinterpret_cast<const uint8_t*>(s2) + std::strlen(s2));

    uint32_t adler1 = adler32_ref(d1);
    uint32_t adler2 = adler32_ref(d2);
    z_off_t len2 = 0; // second block length is zero

    uint32_t result = static_cast<uint32_t>(adler32_combine(adler1, adler2, len2));

    // Expected: Adler32 of only d1
    uint32_t expected = adler32_ref(d1);

    TEST_ASSERT_EQ(result, expected, "adler32_combine with len2 == 0 should equal adler1 (ignore adler2)");
}

// Run all tests
static void run_all_tests() {
    std::cout << "Starting Adler-32 combine unit tests..." << std::endl;

    test_adler32_combine_empty_empty();
    test_adler32_combine_basic_concat();
    test_adler32_combine_large_blocks();
    test_adler32_combine_len2_zero_property();

    std::cout << "Testing complete. Failures: " << g_test_failures << std::endl;
}

int main() {
    run_all_tests();
    return g_test_failures == 0 ? 0 : 1;
}
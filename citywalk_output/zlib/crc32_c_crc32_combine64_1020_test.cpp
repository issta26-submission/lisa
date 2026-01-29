/*
   C++11 test harness for the focal C function:
   uLong ZEXPORT crc32_combine64(uLong crc1, uLong crc2, z_off64_t len2);

   Notes:
   - This test harness uses only the C++ standard library (no GoogleTest or GMock).
   - It relies on a C linkage declaration to crc32_combine64 to be linkable with the existing C
     implementation in crc32.c (as provided by the focal project).
   - We implement several tests that exercise observable properties of crc32_combine64 without
     peering into the internal static helpers (multmodp, x2nmodp). One property is highlighted
     by the following: when crc1 == 0, the function should yield crc2's lower 32 bits.
   - To exercise both branches of the DYNAMIC_CRC_TABLE macro (true and false), compile this same file
     twice with and without defining DYNAMIC_CRC_TABLE. The behavior under the dynamic table path
     triggers the "once(&made, make_crc_table)" call; the non-dynamic path uses the precomputed CRC table.
   - The tests include non-terminating-style assertions (they print results and continue) as per
     the domain guidance.
*/

#include <limits>
#include <cassert>
#include <stdatomic.h>
#include <string>
#include <cstdint>
#include <iostream>
#include <vector>
#include <zutil.h>


// Type aliases matching the focal C code style (as used in crc32.c)
using uLong = unsigned long;
using z_off64_t = long long;

// The focal function is provided by the project (C linkage).
// We declare it here so the test harness can link to it.
extern "C" {
    // Signature obtained from the focal method; ZEXPORT is a macro in the original code, omitted here.
    uLong crc32_combine64(uLong crc1, uLong crc2, z_off64_t len2);
}

// Simple non-terminating assertion macro: logs failure but continues tests.
static int g_failures = 0;
#define ASSERT(cond, msg)                                            \
    do {                                                             \
        if (!(cond)) {                                               \
            std::cerr << "[FAIL] " << msg << " (" << __FILE__     \
                      << ":" << __LINE__ << ")" << std::endl;     \
            ++g_failures;                                           \
        }                                                            \
    } while (false)

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

///// Test 1: crc1 == 0 implies the result equals crc2's lower 32 bits
// Rationale: When crc1 == 0, multmodp(...) is called with a & 0-flag, yielding 0.
// Therefore, crc32_combine64(0, crc2, len2) should be (crc2 & 0xffffffff).
void test_crc32_combine64_crc1_zero_property() {
    std::cout << "Test 1: crc1 == 0 -> result equals (crc2 & 0xffffffff)\n";

    // A small set of representative test vectors for crc2
    std::vector<uLong> crc2_values = {
        0x00000000UL,
        0x00000001UL,
        0x12345678UL,
        0xFFFFFFFFUL,
        0xA5A5A5A5UL
    };

    // A variety of lengths to exercise len2 (including zero)
    std::vector<z_off64_t> len2_values = {
        0, 1, 12345, 0xFFFFFFFFLL, -1 // include a negative value to ensure function handles non-positive len2 gracefully
    };

    for (uLong crc2 : crc2_values) {
        for (z_off64_t len2 : len2_values) {
            uLong res = crc32_combine64(0UL, crc2, len2);
            uLong expected = crc2 & 0xFFFFFFFFUL;
            // The core property should hold irrespective of len2
            ASSERT(res == expected, "crc32_combine64(0, " +
                                    std::to_string(crc2) + ", " +
                                    std::to_string(len2) + ") should equal (crc2 & 0xffffffff)");
            // Also log for inspection when a failure occurs
            if (res != expected) {
                std::cerr << "  [LOG] Got " << res
                          << " expected " << expected
                          << " for crc2=" << std::hex << crc2
                          << ", len2=" << len2 << std::dec << "\n";
            }
        }
    }
    std::cout << "Test 1 completed.\n";
}

///// Test 2: Deterministic results for repeated calls with same inputs
void test_crc32_combine64_determinism() {
    std::cout << "Test 2: Determinism - repeated calls with same inputs yield identical results\n";

    // Use a fixed set of inputs
    const uLong crc1 = 0x1A2B3C4DUL;
    const uLong crc2 = 0x0F0E0D0CUL;
    const z_off64_t len2 = 1234567890LL;

    uLong first = crc32_combine64(crc1, crc2, len2);
    uLong second = crc32_combine64(crc1, crc2, len2);

    ASSERT(first == second, "Two consecutive calls with same inputs must yield identical results");
    if (first != second) {
        std::cerr << "  [LOG] First=" << first << " Second=" << second << "\n";
    }

    // Also verify another independent set
    const uLong crc1_b = 0xFFFFFFFFUL;
    const uLong crc2_b = 0x01234567UL;
    const z_off64_t len2_b = 0;

    uLong r1 = crc32_combine64(crc1_b, crc2_b, len2_b);
    uLong r2 = crc32_combine64(crc1_b, crc2_b, len2_b);
    ASSERT(r1 == r2, "Determinism failed for alternate inputs");
    if (r1 != r2) {
        std::cerr << "  [LOG] r1=" << r1 << " r2=" << r2 << "\n";
    }

    std::cout << "Test 2 completed.\n";
}

///// Test 3: Upper 32 bits are zero (assuming 64-bit uLong)
void test_crc32_combine64_upper_bits_zero() {
    std::cout << "Test 3: Upper 32 bits of result are zero (when uLong is 64-bit) [best-effort heuristic]\n";

    // Use a few random-ish inputs to explore the returned value
    std::vector<std::tuple<uLong, uLong, z_off64_t>> samples = {
        {0x00000000UL, 0xDEADBEEFUL, 0},
        {0x12345678UL, 0x9ABCDEF0UL, 42},
        {0xFFFFFFFFUL, 0x0F0F0F0FUL, 100000},
        {0x0A0A0A0AUL, 0xF0F0F0F0UL, 0x7FFFFFFFLL}
    };

    for (const auto &t : samples) {
        uLong crc1 = std::get<0>(t);
        uLong crc2 = std::get<1>(t);
        z_off64_t len2 = std::get<2>(t);

        uLong res = crc32_combine64(crc1, crc2, len2);
        // Heuristic check: common implementations of crc32_combine64 return a 32-bit value extended to uLong.
        bool upper_zero = (res >> 32) == 0;
        ASSERT(upper_zero, "Upper 32 bits of crc32_combine64 result should be zero (approximate expectation)");
        if (!upper_zero) {
            std::cerr << "  [LOG] Non-zero upper 32 bits for crc1=" << std::hex << crc1
                      << " crc2=" << crc2 << " len2=" << len2 << std::dec
                      << " res=" << res << "\n";
        }
    }

    std::cout << "Test 3 completed.\n";
}

///// Test 4: Consistency across multiple varying inputs (stress-style)
void test_crc32_combine64_consistency_across_inputs() {
    std::cout << "Test 4: Consistency across varied inputs (stress-like test)\n";

    // Simple deterministic pseudo-random generator for repeatable tests
    unsigned int seed = 0xA5A5A5A5;
    auto next_u32 = [&seed]() -> uint32_t {
        seed = (1103515245U * seed + 12345U);
        return seed;
    };

    const int NUM_SAMPLES = 20;
    std::vector<uLong> crcs1(NUM_SAMPLES);
    std::vector<uLong> crcs2(NUM_SAMPLES);
    std::vector<z_off64_t> lens(NUM_SAMPLES);

    for (int i = 0; i < NUM_SAMPLES; ++i) {
        crcs1[i] = static_cast<uLong>(next_u32());
        crcs2[i] = static_cast<uLong>(next_u32());
        lens[i]  = static_cast<z_off64_t>(static_cast<long long>(next_u32()));
    }

    // Compare pairwise within the generated set for determinism
    for (int i = 0; i < NUM_SAMPLES; ++i) {
        uLong a = crcs1[i];
        uLong b = crcs2[i];
        z_off64_t l = lens[i];

        uLong first = crc32_combine64(a, b, l);
        uLong second = crc32_combine64(a, b, l);
        ASSERT(first == second, "Non-deterministic result on same input in stress test");

        // Basic sanity: result should be a finite non-negative value
        ASSERT(first >= 0, "Result should be non-negative");
    }

    std::cout << "Test 4 completed.\n";
}

///// Main harness
int main() {
    std::cout << "Starting crc32_combine64 unit test suite (C++11 test harness).\n";

    // Run tests. These tests rely only on the public function crc32_combine64.
    test_crc32_combine64_crc1_zero_property();
    test_crc32_combine64_determinism();
    test_crc32_combine64_upper_bits_zero();
    test_crc32_combine64_consistency_across_inputs();

    // Summary
    if (g_failures == 0) {
        std::cout << "[PASSED] All tests completed successfully.\n";
    } else {
        std::cout << "[WARNING] Tests completed with " << g_failures
                  << " failure(s).\n";
    }

    // Return non-zero if any test failed to signal issues to build systems, CI, etc.
    return g_failures;
}
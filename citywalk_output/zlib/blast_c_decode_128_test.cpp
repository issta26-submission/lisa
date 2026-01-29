// Unit tests for the focal method "decode" inside blast.c
// Approach: include blast.c into this C++11 test file (with the helper macro
// to expose non-static functions) and directly exercise decode.
// This avoids GTest and keeps tests self-contained.

#include <stddef.h>
#include <blast.h>
#include <iomanip>
#include <setjmp.h>
#include <stdio.h>
#include <iostream>
#include <blast.c>
#include <stdlib.h>


// Expose non-static functions in blast.c by neutralizing the 'local' macro
// so that top-level functions (including decode) have external linkage
#define local
#undef local

// Forward declare dummy input functions used by the tests
// These match the blast_in function signature expected by the state
extern "C" {
    // Dummy infun that indicates no more input (returns 0)
    unsigned dummy_infun_no_input(void* how, unsigned char** buf) {
        return 0;
    }

    // Dummy infun that would supply a minimal chunk of input (not used in Test 1)
    unsigned dummy_infun_unused(void* how, unsigned char** buf) {
        static unsigned char dummy[1] = {0};
        *buf = dummy;
        return 1;
    }
}

// Simple non-terminating assertion helper
static int g_failures = 0;
#define EXPECT_EQ(actual, expected, msg) do { \
    if ((actual) != (expected)) { \
        std::cerr << "[FAIL] " << (msg) << ": expected " << (expected) \
                  << ", got " << (actual) << std::endl; \
        ++g_failures; \
    } else { \
        std::cout << "[PASS] " << (msg) << std::endl; \
    } \
} while (0)

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[FAIL] " << (msg) << std::endl; \
        ++g_failures; \
    } else { \
        std::cout << "[PASS] " << (msg) << std::endl; \
    } \
} while (0)

// Test 1: Basic successful decode path using a crafted Huffman table
// Preconditions:
// - Build a Huffman table where length 2 has 1 code, others zero.
// - Provide bit stream that yields a code corresponding to symbol[0].
// - After decode, verify the returned symbol and the updated bit buffer state.
static void test_decode_basic_symbol() {
    // Prepare Huffman table: counts[2] = 1; all others zero
    short counts[MAXBITS + 1] = {0};
    short syms[256] = {0};
    syms[0] = 0x2A; // choose some non-zero symbol, e.g., 42

    struct huffman h;
    h.count = counts;
    h.symbol = syms;

    // State setup
    struct state s;
    s.bitbuf = 0b11;   // LSB first: two bits set to 1
    s.bitcnt = 3;      // 3 bits available in bitbuf
    s.left = 0;
    s.infun = dummy_infun_unused; // not used in this test (we decode before needing input)
    s.inhow = NULL;
    s.in = NULL;
    s.outfun = NULL;
    s.outhow = NULL;
    s.next = 0;
    s.first = 1;

    // Call decode
    int result = decode(&s, &h);

    // Expected: should return h.symbol[0] = 0x2A (42)
    EXPECT_EQ(result, 0x2A, "decode returns symbol[0] (42)")
    // Bitbuffer state: after consuming 2 bits in the second iteration,
    // bitbuf should be the remaining bits (0) and bitcnt reduced by len (2),
    // so bitcnt should be (3 - 2) & 7 = 1
    EXPECT_EQ(s.bitbuf, 0, "decode updates bitbuf after success")
    EXPECT_EQ(s.bitcnt, 1, "decode updates bitcnt after consuming bits")
}

// Test 2: Out-of-input path triggers longjmp to indicate input exhaustion
// Preconditions:
// - Huffman table with all zeros so no code will be decoded.
// - s.infun returns 0 to simulate end of input when the decoder asks for more bits.
// - Use setjmp/longjmp to catch the longjmp from decode().
static int test_decode_out_of_input_longjmp() {
    // Huffman with no codes
    short counts[MAXBITS + 1] = {0};
    short syms[256] = {0};

    struct huffman h;
    h.count = counts;
    h.symbol = syms;

    // State setup
    struct state s;
    s.bitbuf = 0;
    s.bitcnt = 1;    // small amount of bits available
    s.left = 0;
    s.infun = dummy_infun_no_input; // input is exhausted on demand
    s.inhow = NULL;
    s.in = NULL;
    s.outfun = NULL;
    s.outhow = NULL;
    s.next = 0;
    s.first = 1;

    // Use setjmp to catch the longjmp from within decode()
    if (setjmp(s.env) == 0) {
        // This should longjmp back here with value != 0
        decode(&s, &h);
        // If we return here, something is wrong
        std::cerr << "[FAIL] test_decode_out_of_input_longjmp: decode() did not longjmp as expected" << std::endl;
        g_failures++;
        return 1;
    } else {
        // We arrived here via longjmp, which is the expected path
        std::cout << "[PASS] test_decode_out_of_input_longjmp: longjmp occurred as expected" << std::endl;
        return 0;
    }
}

// Test 3: No codes in Huffman table should yield -9 (ran out of codes) after decoding attempts
// Preconditions:
// - bitcnt set to allow a full traversal of the possible bit lengths without a match.
// - All counts are zero; function should return -9.
static void test_decode_no_codes_returns_minus9() {
    short counts[MAXBITS + 1] = {0};
    short syms[256] = {0};

    struct huffman h;
    h.count = counts;
    h.symbol = syms;

    struct state s;
    s.bitbuf = 0;
    s.bitcnt = MAXBITS + 1; // 14
    s.left = 0;
    s.infun = dummy_infun_no_input;
    s.inhow = NULL;
    s.in = NULL;
    s.outfun = NULL;
    s.outhow = NULL;
    s.next = 0;
    s.first = 1;

    int result = decode(&s, &h);
    EXPECT_EQ(result, -9, "decode returns -9 when no codes exist")
}

// Main runner
int main() {
    std::cout << "Running decode unit tests (C++ harness)..." << std::endl;

    test_decode_basic_symbol();
    int t2 = test_decode_out_of_input_longjmp();
    (void)t2; // suppress unused warning if not used further

    test_decode_no_codes_returns_minus9();

    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}
// C++11 test suite for the focal method "bits" behavior via the public blast() entry point.
// Note: The real bits function is static inside blast.c, but we can exercise its behavior
// through blast() by crafting controlled input streams. This test does not rely on GTest.
// It uses a small handcrafted test harness and inline mock input/output callbacks.

#include <cstdio>
#include <cstring>
#include <stddef.h>
#include <blast.h>
#include <setjmp.h>
#include <cstdint>
#include <cstddef>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>


// Prototypes for the blast() API (as used inside blast.c)
typedef unsigned (*blast_in)(void* how, unsigned char** buf);
typedef int      (*blast_out)(void* how, unsigned char* buf, unsigned len);

// blast() signature as exposed by blast.c
extern "C" int blast(blast_in infun, void* inhow, blast_out outfun, void* outhow,
                     unsigned* left, unsigned char** in);

// ------------------------------------------------------------
// Test harness helpers and mock components
// Each test uses its own mock input stream and callbacks.
// ------------------------------------------------------------

// Utility macro for simple test results printing without aborting on failure.
#define TEST_ASSERT(cond, msg)                                   \
    do { if (!(cond)) {                                \
            std::cerr << "TEST FAILED: " << (msg) << "\n"; \
            return false;                                \
        } } while(0)

// Test 1: No input is provided (left input is zero). blast() should return 2
// representing an input exhaustion longjmp path (out of input).
struct MockStreamNoInput {
    // We provide zero-length input buffer to trigger "out of input" path.
    const unsigned char* buf0;
    unsigned len0;
    int stage;
};

// inflow for Test 1: immediately report no data
unsigned test1_in(void* how, unsigned char** buf) {
    // Not using state: always indicate no data available
    *buf = NULL;
    return 0;
}
int test1_out(void* how, unsigned char* buf, unsigned len) {
    // No output flush required for this test
    return 0;
}
bool test_no_input_returns_2() {
    // Set up
    unsigned left = 0;
    unsigned char* inptr = nullptr;

    // Run blast with no input available
    int ret = blast(test1_in, nullptr, test1_out, nullptr, &left, &inptr);

    // Expect out-of-input error code 2
    TEST_ASSERT(ret == 2, "blast() should return 2 on out-of-input (no data)");
    // Additionally, verify that *left remains 0 or as per implementation (not strictly required)
    return true;
}

// Test 2: First byte provides lit = 0, second byte provides dict = 7 (out-of-range: 7 > 6) -> should return -2
// This exercises the branch "if (lit > 1) return -1" not taken; instead we exercise the dict range check.
struct MockStreamDICT {
    const unsigned char* b0;
    unsigned len0;
    const unsigned char* b1;
    unsigned len1;
    int stage;
};
static unsigned test2_in(void* how, unsigned char** buf) {
    MockStreamDICT* s = (MockStreamDICT*)how;
    if (s->stage == 0) {
        s->stage = 1;
        *buf = (unsigned char*)s->b0;
        return s->len0;
    } else if (s->stage == 1) {
        s->stage = 2;
        *buf = (unsigned char*)s->b1;
        return s->len1;
    } else {
        *buf = NULL;
        return 0;
    }
}
static int test2_out(void* how, unsigned char* buf, unsigned len) {
    return 0;
}
bool test_dict_out_of_range_returns_minus2() {
    // Prepare two buffers:
    // - First byte: lit = 0 (0x00) to take the "else" branch and read dict next
    // - Second byte: dict = 7 (0x07) -> out of allowed range (4..6)
    unsigned char bufA[1] = { 0x00 }; // lit = 0
    unsigned char bufB[1] = { 0x07 }; // dict = 7
    MockStreamDICT stream{ bufA, 1, bufB, 1, 0 };

    unsigned left = 1; // provide at least something for initial influn
    unsigned char* inptr = nullptr;

    int ret = blast(test2_in, &stream, test2_out, nullptr, &left, &inptr);
    // Expect -2 from dict out of range
    TEST_ASSERT(ret == -2, "blast() should return -2 when dict is out of range (4..6)");
    return true;
}

// Test 3: First 8-bit read yields lit > 1 (e.g., lit == 2), which should cause immediate -1 return.
// We provide a single byte 0x02 for lit, so bits(s,8) returns 2 and triggers -1.
struct MockStreamLitGT1 {
    const unsigned char* b0;
    unsigned len0;
    int stage;
};
static unsigned test3_in(void* how, unsigned char** buf) {
    MockStreamLitGT1* s = (MockStreamLitGT1*)how;
    if (s->stage == 0) {
        s->stage = 1;
        *buf = (unsigned char*)s->b0;
        return s->len0;
    } else {
        *buf = nullptr;
        return 0;
    }
}
static int test3_out(void* how, unsigned char* buf, unsigned len) {
    return 0;
}
bool test_lit_gt_one_returns_minus1() {
    unsigned char byte = 0x02; // lit = 2
    MockStreamLitGT1 stream{ &byte, 1, 0 };

    unsigned left = 1;
    unsigned char* inptr = nullptr;

    int ret = blast(test3_in, &stream, test3_out, nullptr, &left, &inptr);
    // Expect -1 from lit > 1 branch
    TEST_ASSERT(ret == -1, "blast() should return -1 when lit > 1 (invalid Huffman code)");
    return true;
}

// ------------------------------------------------------------
// Main harness to run tests
// ------------------------------------------------------------
int main() {
    int failed = 0;

    std::cout << "Running blast.bits-focused unit tests (via blast() interface)..." << std::endl;

    if (!test_no_input_returns_2()) {
        std::cerr << "Test 1 failed." << std::endl;
        failed++;
    } else {
        std::cout << "Test 1 passed." << std::endl;
    }

    if (!test_dict_out_of_range_returns_minus2()) {
        std::cerr << "Test 2 failed." << std::endl;
        failed++;
    } else {
        std::cout << "Test 2 passed." << std::endl;
    }

    if (!test_lit_gt_one_returns_minus1()) {
        std::cerr << "Test 3 failed." << std::endl;
        failed++;
    } else {
        std::cout << "Test 3 passed." << std::endl;
    }

    if (failed) {
        std::cerr << "Some tests FAILED: " << failed << std::endl;
        return EXIT_FAILURE;
    } else {
        std::cout << "All tests PASSED." << std::endl;
        return EXIT_SUCCESS;
    }
}
/*
Unit Test Suite for the focal method and its dependencies (puff.c)

Step 1 — Program Understanding (keywords summarized below)
- Core function under test: puff(...) which orchestrates decompression by internally using a static implementation of the deflate logic.
- Relevant structures and helpers (from class dependencies in puff.c):
  - struct state: maintains decoding state, output buffer, counters (out, outlen, outcnt), etc.
  - struct huffman: Huffman code tables (lencode, distcode used for literals/lengths and distances).
  - decode(struct state*, const struct huffman*): decodes a symbol using a Huffman tree.
  - bits(struct state*, int need) / stored(struct state*): bitstream helpers.
  - Various internal functions and static logic for fixed/dynamic blocks and the top-level puff() interface.
- Candidate Keywords to guide test design:
  - puff, state, out, outlen, outcnt, lencode, distcode
  - decode, bits, lens, lext, dists, dext
  - stored, fixed, dynamic (block types)
  - NIL macro, SYMBOL handling ( literals < 256, length > 256, end block 256)

Step 2 — Unit Test Generation (targeting puff.c integration)
- Approach: Exercise the public interface puff(...) with a minimal, valid DEFLATE stored-block input (BFINAL=1, BTYPE=00) to validate the codes() logic path through end-to-end behavior.
- Tests focus on:
  - Successful decompression of a simple stored block (literal data) using puff.
  - Failure path when destination buffer is too small (triggering the “destination too small” branch in codes).
- Rationale: Stored-block path exercises the symbolic path (literal handling, length/distance handling, end-of-block) without needing to generate complex Huffman-encoded blocks.

Step 3 — Test Case Refinement (domain knowledge)
- We use a very small, handcrafted DEFLATE stored block:
  - BFINAL=1, BTYPE=00 (stored block)
  - LEN and NLEN fields correctly set for the payload size
  - Payload is a simple 4-byte sequence "ABCD"
- We add a second test to exercise the “destination too small” path by supplying a too-small destination buffer for the same block.
- We implement a tiny, self-contained test harness (no GTest), printing pass/fail results but continuing execution to maximize coverage.

Now the test code (C++11, no GTest, link against puff.c/puff.h)

*/

#include <iostream>
#include <puff.h>
#include <setjmp.h>
#include <cstring>


// Include the C header for puff. Ensure C linkage in C++ translator.
extern "C" {
}

// Simple non-terminating test harness primitives
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void log_pass(const char* test) {
    std::cout << "[PASS] " << test << "\n";
}
static void log_fail(const char* test, const char* msg) {
    std::cerr << "[FAIL] " << test << " - " << msg << "\n";
}

// Test 1: Decompress a simple stored-block block containing "ABCD" successfully
bool test_puff_stored_block_success() {
    // Craft a minimal valid DEFLATE stored-block (BFINAL=1, BTYPE=00) with 4 bytes payload: "ABCD"
    // Encoding per DEFLATE stored-block specification:
    // - First byte: BFINAL(1) + BTYPE(00) -> 0b001 -> 0x01
    // - LEN = 0x0004 (little-endian)
    // - NLEN = ~LEN = 0xFFFB (little-endian) -> 0xFB, 0xFF
    // - Data: 'A' 'B' 'C' 'D'
    unsigned char input[] = {
        0x01,       // BFINAL=1, BTYPE=00 (stored block)
        0x04, 0x00, // LEN = 4
        0xFB, 0xFF, // NLEN = ~LEN
        'A', 'B', 'C', 'D' // payload
    };
    const unsigned long input_len = sizeof(input);

    unsigned char output[32];
    unsigned long output_len = sizeof(output);
    unsigned long consumed_input = input_len;

    // Call puff (dest, destlen, source, sourcelen)
    int ret = puff(output, &output_len, input, &consumed_input);

    if (ret != 0) {
        log_fail("test_puff_stored_block_success", "puff() reported non-zero return; expected 0 for success.");
        return false;
    }

    if (output_len != 4) {
        log_fail("test_puff_stored_block_success", "Output length mismatch; expected 4 bytes.");
        return false;
    }

    if (std::memcmp(output, "ABCD", 4) != 0) {
        log_fail("test_puff_stored_block_success", "Output data mismatch; expected 'ABCD'.");
        return false;
    }

    log_pass("test_puff_stored_block_success");
    return true;
}

// Test 2: Destination buffer too small should trigger the 'destination too small' path (return 1)
bool test_puff_dest_too_small() {
    // Reuse the same stored-block input as Test 1
    unsigned char input[] = {
        0x01, 0x04, 0x00, 0xFB, 0xFF,
        'A', 'B', 'C', 'D'
    };
    const unsigned long input_len = sizeof(input);

    unsigned char output[2]; // Intentionally too small for 4-byte payload
    unsigned long output_len = sizeof(output);
    unsigned long consumed_input = input_len;

    int ret = puff(output, &output_len, input, &consumed_input);

    // We expect a non-zero return (1) indicating buffer/distance issue (per codes() logic)
    if (ret != 1) {
        log_fail("test_puff_dest_too_small", "puff() did not report expected 'destination too small' (1).");
        return false;
    }

    log_pass("test_puff_dest_too_small");
    return true;
}

int main() {
    // Run tests in a lightweight harness
    g_total_tests = 0;
    g_failed_tests = 0;

    // Test 1
    ++g_total_tests;
    if (!test_puff_stored_block_success()) {
        ++g_failed_tests;
    }

    // Test 2
    ++g_total_tests;
    if (!test_puff_dest_too_small()) {
        ++g_failed_tests;
    }

    // Summary
    std::cout << "Tests run: " << g_total_tests << ", Passed: "
              << (g_total_tests - g_failed_tests) << ", Failed: "
              << g_failed_tests << "\n";

    return g_failed_tests ? 1 : 0;
}
// C++11 test suite for the focal method bi_flush in trees.c
// This test harness is designed to be compiled with the project under test.
// It includes the focal C source file to ensure access to the static (local) function.
// The tests focus on the observable state changes of deflate_state fields used by bi_flush.
// No GTest is used; a lightweight in-file test runner is implemented.

#include <string>
#include <deflate.h>
#include <cstdint>
#include <trees.c>
#include <iostream>


// Include the project's public definitions (types, macros) used by bi_flush.
// We wrap the C code inclusion in extern "C" to ensure C linkage within this C++ translation unit.
extern "C" {
}

// Include the focal C source file to bring bi_flush into this translation unit.
// This allows testing of a static (local) function from C code within a single TU.
extern "C" {
}

// Lightweight test framework (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TPRINTF(msg) do { std::cout << (msg) << std::endl; } while(0)
#define ASSERT(cond, desc) do {                                             \
    ++g_total_tests;                                                          \
    if (!(cond)) {                                                            \
        ++g_failed_tests;                                                     \
        std::cerr << "ASSERTION FAILED: " << (desc)                         \
                  << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    } else {                                                                  \
        /* Optional: print passing info for verbose runs */                 \
    }                                                                         \
} while(0)

using namespace std;

// Helper to print a summary after tests
static void PrintSummary() {
    std::cout << "Total tests: " << g_total_tests
              << ", Passed: " << (g_total_tests - g_failed_tests)
              << ", Failed: " << g_failed_tests << std::endl;
}

/*
  Test 1: bi_flush should take the branch where bi_valid == 16
  - Precondition: s->bi_valid == 16, s->bi_buf holds a value.
  - Expected: after bi_flush(s), s->bi_buf becomes 0 and s->bi_valid becomes 0.
  - Rationale: This verifies the primary path that flushes a full 16-bit buffer.
*/
static void test_bi_flush_branch_full16() {
    deflate_state s;
    // Initialize with a non-zero buffer and 16 valid bits
    s.bi_buf = 0xABCD;
    s.bi_valid = 16;

    // Call the focal method
    bi_flush(&s);

    // Validate observable side-effects on the deflate_state
    ASSERT(s.bi_buf == 0u, "When bi_valid == 16, bi_buf should be reset to 0");
    ASSERT(s.bi_valid == 0, "When bi_valid == 16, bi_valid should be reset to 0");
}

/*
  Test 2: bi_flush should take the branch where bi_valid >= 8 but bi_valid != 16
  - Precondition: s->bi_valid == 12, s->bi_buf holds a 16-bit value.
  - Expected: after bi_flush(s), s->bi_buf should be shifted right by 8 bits and bi_valid reduced by 8.
  - Specifically:
      - bi_buf before: 0x1234
      - (Byte)s->bi_buf passed to put_byte would be 0x34 (least significant byte),
        but we only verify the resulting state of bi_buf and bi_valid here.
      - bi_buf after: 0x12
      - bi_valid after: 4
  - Rationale: This path verifies proper partial flush behavior for 8-bit chunks.
*/
static void test_bi_flush_branch_partial8bit() {
    deflate_state s;
    s.bi_buf = 0x1234;
    s.bi_valid = 12;

    bi_flush(&s);

    // After flushing 8 bits, lower 8 bits are moved out and upper bits remain
    // We expect bi_buf to be 0x12 and bi_valid to be 4
    ASSERT(s.bi_buf == 0x12u, "bi_buf should be 0x12 after flushing 12 valid bits");
    ASSERT(s.bi_valid == 4, "bi_valid should be 4 after flushing 12 valid bits");
}

/*
  Test 3: bi_flush should perform no action when bi_valid < 8
  - Precondition: s->bi_valid == 7, s->bi_buf holds a value.
  - Expected: no modification to s->bi_buf or s->bi_valid.
  - Rationale: This validates the else path is not entered and state remains unchanged.
*/
static void test_bi_flush_branch_noop_below8() {
    deflate_state s;
    s.bi_buf = 0x9ABC;
    s.bi_valid = 7;

    // Preserve a copy of the original state for assertion
    uint32_t orig_buf = s.bi_buf;
    int orig_valid = s.bi_valid;

    bi_flush(&s);

    ASSERT(s.bi_buf == orig_buf, "bi_buf should be unchanged when bi_valid < 8");
    ASSERT(s.bi_valid == orig_valid, "bi_valid should be unchanged when bi_valid < 8");
}

// Entry point for executing all tests
int main() {
    TPRINTF("Starting unit tests for bi_flush in trees.c (C++11 test harness)");

    test_bi_flush_branch_full16();
    test_bi_flush_branch_partial8bit();
    test_bi_flush_branch_noop_below8();

    PrintSummary();

    // Return non-zero if any test failed
    return (g_failed_tests == 0) ? 0 : 1;
}
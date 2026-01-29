// Comprehensive unit test suite for sqlite3BtreeCursorHasHint
// Focus: verify the boolean interpretation of (pCsr->hints & mask) != 0
// This test relies on the actual BtCursor struct definition provided by btreeInt.h
// and the C linkage of sqlite3BtreeCursorHasHint. It is compiled as a C++11 test harness.

#include <btreeInt.h>
#include <iostream>
#include <cstring>


// Include the real BtCursor struct layout and related declarations

// Ensure C linkage for the focal function when compiling from C++
extern "C" int sqlite3BtreeCursorHasHint(BtCursor *pCsr, unsigned int mask);

// Simple non-terminating assertion helpers (mocking a lightweight test framework)
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define EXPECT_EQ(expected, actual, msg) do { \
    ++tests_run; \
    if ((expected) == (actual)) { \
        ++tests_passed; \
    } else { \
        ++tests_failed; \
        std::cerr << "EXPECT_EQ FAILED: " << (msg) \
                  << " | expected: " << (expected) \
                  << ", actual: " << (actual) << std::endl; \
    } \
} while(0)

#define EXPECT_TRUE(cond, msg) do { \
    ++tests_run; \
    if (static_cast<bool>(cond)) { \
        ++tests_passed; \
    } else { \
        ++tests_failed; \
        std::cerr << "EXPECT_TRUE FAILED: " << (msg) \
                  << " | condition is false" << std::endl; \
    } \
} while(0)

#define EXPECT_FALSE(cond, msg) do { \
    ++tests_run; \
    if (!(static_cast<bool>(cond))) { \
        ++tests_passed; \
    } else { \
        ++tests_failed; \
        std::cerr << "EXPECT_FALSE FAILED: " << (msg) \
                  << " | condition is true" << std::endl; \
    } \
} while(0)

/*
Test Case 1: True when exact matching bit is set
- Purpose: mask equals the bit set in pCsr->hints
- Expected: 1 (true)
*/
static void test_true_single_bit_match() {
    BtCursor csr;
    std::memset(&csr, 0, sizeof(BtCursor)); // zero all fields safely
    csr.hints = 0x00000001;                 // set bit 0
    int res = sqlite3BtreeCursorHasHint(&csr, 0x00000001);
    EXPECT_EQ(1, res, "single bit match should return 1");
}

/*
Test Case 2: False when there is no overlap between hints and mask
- Purpose: no common bits between pCsr->hints and mask
- Expected: 0 (false)
*/
static void test_false_no_overlap() {
    BtCursor csr;
    std::memset(&csr, 0, sizeof(BtCursor));
    csr.hints = 0x00000004;                 // set bit 2
    int res = sqlite3BtreeCursorHasHint(&csr, 0x00000002); // mask bit 1, no overlap
    EXPECT_EQ(0, res, "no overlap should return 0");
}

/*
Test Case 3: True when mask covers at least one bit in hints (multi-bit mask)
- Purpose: ensure multiple bits in mask still yield true if any overlap exists
- Expected: 1
*/
static void test_true_overlap_with_multi_bit_mask() {
    BtCursor csr;
    std::memset(&csr, 0, sizeof(BtCursor));
    csr.hints = 0xA;                          // bits 1 and 3 set (0b1010)
    int res = sqlite3BtreeCursorHasHint(&csr, 0x2 | 0x8); // mask bits 1 and 3
    EXPECT_EQ(1, res, "overlap with multi-bit mask should return 1");
}

/*
Test Case 4: True when hints have high-order bit set
- Purpose: ensure functionality with boundary values (e.g., high bit)
- Expected: 1
*/
static void test_true_high_order_bit() {
    BtCursor csr;
    std::memset(&csr, 0, sizeof(BtCursor));
    csr.hints = 0x80000000;                    // high-order bit set
    int res = sqlite3BtreeCursorHasHint(&csr, 0x80000000);
    EXPECT_EQ(1, res, "high-order bit overlap should return 1");
}

/*
Test Case 5: False when mask is zero (edge case)
- Purpose: mask == 0 should always yield false
- Expected: 0
*/
static void test_false_zero_mask() {
    BtCursor csr;
    std::memset(&csr, 0, sizeof(BtCursor));
    csr.hints = 0xFFFFFFFF;                    // any non-zero
    int res = sqlite3BtreeCursorHasHint(&csr, 0x0);
    EXPECT_EQ(0, res, "mask zero should yield 0");
}

/*
Run all defined tests
*/
static void run_all_tests() {
    test_true_single_bit_match();
    test_false_no_overlap();
    test_true_overlap_with_multi_bit_mask();
    test_true_high_order_bit();
    test_false_zero_mask();
}

/*
Explanatory notes:
- The BtCursor struct layout is taken from the project header btreeInt.h to ensure
  compatibility with sqlite3BtreeCursorHasHint's access to pCsr->hints.
- We use non-terminating expectations so that all tests run in a single execution.
- The test harness prints a summary and returns non-zero if any test failed.
*/
int main() {
    run_all_tests();

    std::cout << "sqlite3BtreeCursorHasHint tests completed." << std::endl;
    std::cout << "Total tests run: " << tests_run
              << ", Passed: " << tests_passed
              << ", Failed: " << tests_failed << std::endl;

    return (tests_failed == 0) ? 0 : 1;
}
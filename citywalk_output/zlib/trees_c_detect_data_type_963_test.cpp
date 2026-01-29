// Test suite for the focal method detect_data_type located in trees.c
// This test suite is designed to be compiled with C++11 (no Google Test).
// It directly exercises detect_data_type by constructing a deflate_state
// and manipulating its dyn_ltree array (Freq field) to trigger different
// branches of the function.
//
// Step 1 (Program Understanding) and Step 2 (Unit Test Generation) summary:
// - The function detect_data_type inspects a block-list of bytes (block_mask),
//   then checks for allow-listed bytes (indices 9, 10, 13), then scans
//   further literals (32 .. LITERALS-1). It returns Z_BINARY or Z_TEXT
//   accordingly.
// - We create test cases that cover: (a) block-listed byte triggers binary,
//   (b) allow-listed byte triggers text, (c) text bytes beyond 31 trigger text,
//   (d) no bytes present -> binary (empty/gray-lit).
//
// Step 3 (Test Case Refinement) notes:
// - Use non-terminating assertions: print failures but continue running tests.
// - Use only standard library facilities plus provided deflate.h definitions.
// - Do not rely on private/internal test hooks beyond public declarations.
// - Access to detect_data_type is assumed via extern declaration; the header
//   deflate.h is expected to declare necessary types (deflate_state, ct_data, etc.)
// - Static linkage in the original file may exist; for test purposes we assume
//   detect_data_type is linkable from this translation unit.

#include <iostream>
#include <deflate.h>
#include <cstring>


// Import necessary dependencies with correct paths.
// The real project provides deflate.h; we assume its presence here.

// Ensure the function is visible to this test (C linkage if needed).
extern "C" int detect_data_type(deflate_state *s);

// Simple non-terminating assertion mechanism.
// Keeps track of total tests and passed tests; prints failures but doesn't exit.
static int g_total_tests = 0;
static int g_passed_tests = 0;

static void check_eq(int expected, int actual, const char* test_name) {
    g_total_tests++;
    if (expected == actual) {
        g_passed_tests++;
        std::cout << "[OK] " << test_name << std::endl;
    } else {
        std::cerr << "[FAIL] " << test_name
                  << " - expected: " << expected
                  << ", actual: " << actual << std::endl;
    }
}

// Test 1: block-listed bit corresponding to n=0 is set in block_mask and Freq != 0
// Expected: detect_data_type returns Z_BINARY.
static void test_block_list_triggers_binary_at_zero() {
    deflate_state s = {};
    // Ensure all frequencies start at 0
    std::memset(s.dyn_ltree, 0, sizeof(s.dyn_ltree));
    // Set a non-zero frequency for a block-listed byte (n = 0)
    s.dyn_ltree[0].Freq = 1;

    int result = detect_data_type(&s);
    check_eq(Z_BINARY, result, "Block-listed bit at n=0 triggers Z_BINARY");
}

// Test 2: allow-listed bytes (n = 9, 10, or 13) cause Z_TEXT when no block-listed bytes dominate
static void test_allow_list_triggers_text() {
    deflate_state s = {};
    std::memset(s.dyn_ltree, 0, sizeof(s.dyn_ltree));

    // Ensure block-listed bits do not trigger: pick an index not in 0..6 or 14..25/28..31
    // n = 9 is not block-listed per the mask described by the code.
    s.dyn_ltree[9].Freq = 1;

    int result = detect_data_type(&s);
    check_eq(Z_TEXT, result, "Allow-listed byte at n=9 triggers Z_TEXT");
}

// Test 3: textual bytes beyond the first 31 (n in [32, LITERALS)) trigger Z_TEXT
static void test_text_beyond_31_triggers_text() {
    deflate_state s = {};
    std::memset(s.dyn_ltree, 0, sizeof(s.dyn_ltree));

    // Set a text byte beyond the initial range to trigger the 32..LITERALS check
    s.dyn_ltree[32].Freq = 1;

    int result = detect_data_type(&s);
    check_eq(Z_TEXT, result, "Text byte at n=32 triggers Z_TEXT");
}

// Test 4: Empty/gray-listed stream (no block-listed or allow-listed bytes)
// Expected: Z_BINARY
static void test_empty_stream_means_binary() {
    deflate_state s = {};
    std::memset(s.dyn_ltree, 0, sizeof(s.dyn_ltree));

    int result = detect_data_type(&s);
    check_eq(Z_BINARY, result, "Empty/gray-listed stream yields Z_BINARY");
}

// Test 5: Block-listed and allow-listed bytes coexist; block-listed should take precedence
static void test_block_trumps_allow_list() {
    deflate_state s = {};
    std::memset(s.dyn_ltree, 0, sizeof(s.dyn_ltree));

    // Block-listed n=1 is within 0..6; set Freq non-zero to trigger first branch
    s.dyn_ltree[1].Freq = 5;
    // Also set allow-listed bytes to non-zero to ensure first condition wins
    s.dyn_ltree[9].Freq = 3;

    int result = detect_data_type(&s);
    check_eq(Z_BINARY, result, "Block-listed bytes take precedence over allow-listed");
}

int main() {
    // Run the test suite
    std::cout << "Starting detect_data_type test suite..." << std::endl;

    test_block_list_triggers_binary_at_zero();
    test_allow_list_triggers_text();
    test_text_beyond_31_triggers_text();
    test_empty_stream_means_binary();
    test_block_trumps_allow_list();

    std::cout << "Tests completed. Passed "
              << g_passed_tests << " / " << g_total_tests << " tests."
              << std::endl;

    return (g_total_tests == g_passed_tests) ? 0 : 1;
}
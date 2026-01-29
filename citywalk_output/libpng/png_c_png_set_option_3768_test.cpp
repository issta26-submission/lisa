#include <cstdint>
#include <iostream>
#include <pngpriv.h>


// Include the project dependencies that declare png_set_option and related types/defs.
// The test suite relies on the actual library's definitions (png_struct, PNG_OPTION_NEXT, etc.)
// If the environment provides these headers, this will wire up correctly.

// Lightweight non-terminating test harness (no GTest). 
// Keeps track of test counts and reports failures without aborting execution.
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_EQ(actual, expected, msg) \
    do { \
        ++g_tests_run; \
        if ((actual) != (expected)) { \
            std::cerr << "TEST FAIL: " << msg \
                      << " | Expected: " << (expected) \
                      << ", Got: " << (actual) << "\n"; \
            ++g_tests_failed; \
        } \
    } while (0)

#define EXPECT_NEQ(actual, not_expected, msg) \
    do { \
        ++g_tests_run; \
        if ((actual) == (not_expected)) { \
            std::cerr << "TEST FAIL: " << msg \
                      << " | Unexpected value: " << (actual) << "\n"; \
            ++g_tests_failed; \
        } \
    } while (0)

// Test 1: Basic behavior - option 0, onoff = 0
// Expected: returns 0 (previous value in mask is 0) and updates options to 2 (0b10).
static void test_png_set_option_basic_option0_onoff0() {
    // Explanation: Ensure the true-branch executes for a valid pointer and in-range option.
    png_struct png_ptr;
    png_ptr.options = 0;
    int ret = png_set_option((png_structrp)&png_ptr, 0, 0); // option 0, off
    EXPECT_EQ(ret, 0, "png_set_option should return previous 2-bit value for option 0 when current is 0 (should be 0).");
    EXPECT_EQ(png_ptr.options, 2u, "png_set_option should set the first two bits to '10' (2) when onoff is 0.");
}

// Test 2: Basic behavior - option 0, onoff = 1
// Expected: returns 2 (previous value in mask was 2) and updates options to 3 (0b11).
static void test_png_set_option_basic_option0_onoff1() {
    png_struct png_ptr;
    png_ptr.options = 2; // current value in bits 0-1 is 2
    int ret = png_set_option((png_structrp)&png_ptr, 0, 1); // option 0, on
    EXPECT_EQ(ret, 2, "png_set_option should return previous value 2 for option 0 when current is 2.");
    EXPECT_EQ(png_ptr.options, 3u, "png_ptr.options should become 0b11 (3) after setting option 0 to on.");
}

// Test 3: Basic behavior - option 2, onoff = 1
// Starting from 0, option 2 on should set bits 2-3 to 0b11 and update options accordingly.
static void test_png_set_option_basic_option2_onoff1() {
    png_struct png_ptr;
    png_ptr.options = 0;
    int ret = png_set_option((png_structrp)&png_ptr, 2, 1);
    EXPECT_EQ(ret, 0, "png_set_option should return 0 as previous value for option 2 when starting from 0.");
    EXPECT_EQ(png_ptr.options, 0x0C, "png_ptr.options should become 0b1100 (12) after setting option 2 to on.");
}

// Test 4: Invalid input - NULL pointer
// Expected: returns PNG_OPTION_INVALID without modifying any memory.
static void test_png_set_option_null_ptr_invalid() {
    int ret = png_set_option(nullptr, 0, 0);
    EXPECT_EQ(ret, PNG_OPTION_INVALID, "png_set_option should return PNG_OPTION_INVALID for NULL pointer.");
}

// Test 5: Invalid input - negative option
// Expected: returns PNG_OPTION_INVALID (predicate fails).
static void test_png_set_option_negative_option_invalid() {
    png_struct png_ptr;
    png_ptr.options = 0;
    int ret = png_set_option((png_structrp)&png_ptr, -1, 0);
    EXPECT_EQ(ret, PNG_OPTION_INVALID, "png_set_option should return PNG_OPTION_INVALID for negative option.");
}

// Test 6: Invalid input - odd option (bit 0 set)
// The function requires option to be even (option & 1 == 0). Use option 1 (0b01) which is invalid.
static void test_png_set_option_odd_option_invalid() {
    png_struct png_ptr;
    png_ptr.options = 0;
    int ret = png_set_option((png_structrp)&png_ptr, 1, 0);
    EXPECT_EQ(ret, PNG_OPTION_INVALID, "png_set_option should return PNG_OPTION_INVALID for odd option (option & 1) != 0.");
}

// Test 7: Out-of-range option - equals PNG_OPTION_NEXT
// Expected: returns PNG_OPTION_INVALID.
static void test_png_set_option_out_of_range_invalid() {
    png_struct png_ptr;
    png_ptr.options = 0;
    int ret = png_set_option((png_structrp)&png_ptr, PNG_OPTION_NEXT, 0);
    EXPECT_EQ(ret, PNG_OPTION_INVALID, "png_set_option should return PNG_OPTION_INVALID when option >= PNG_OPTION_NEXT.");
}

// Test 8: Multiple options interaction (compound updates)
// Start with 0. Set option 0 to on, then set option 2 to on. Validate resulting options and return values.
static void test_png_set_option_multiple_options_interaction() {
    png_struct png_ptr;
    png_ptr.options = 0;
    int r1 = png_set_option((png_structrp)&png_ptr, 0, 1); // set option 0 on
    EXPECT_EQ(r1, 0, "First call: previous value for option 0 should be 0.");
    EXPECT_EQ(png_ptr.options, 3u, "After first call, options should be 0b0011 (3).");

    int r2 = png_set_option((png_structrp)&png_ptr, 2, 1); // set option 2 on
    EXPECT_EQ(r2, 0, "Second call: previous value for option 2 should be 0 when starting from 0 in those bits.");
    EXPECT_EQ(png_ptr.options, 15u, "After second call, options should be 0b1111 (15).");
}

// Test 9: Multiple updates on same option to verify overwrite behavior (onoff toggling)
// Start with 0. Set option 0 to on, then set option 0 to off. Validate.
static void test_png_set_option_toggle_same_option() {
    png_struct png_ptr;
    png_ptr.options = 0;
    int r1 = png_set_option((png_structrp)&png_ptr, 0, 1);
    EXPECT_EQ(r1, 0, "First toggle: previous value 0.");
    int r2 = png_set_option((png_structrp)&png_ptr, 0, 0);
    EXPECT_EQ(r2, 2, "Second toggle: previous value should be 2 (bits before second write).");
    EXPECT_EQ(png_ptr.options, 2u, "After toggling option 0 off, only bit 1 is set (0b0010).");
}

// Run all tests and report summary
static void run_all_tests() {
    test_png_set_option_basic_option0_onoff0();
    test_png_set_option_basic_option0_onoff1();
    test_png_set_option_basic_option2_onoff1();
    test_png_set_option_null_ptr_invalid();
    test_png_set_option_negative_option_invalid();
    test_png_set_option_odd_option_invalid();
    test_png_set_option_out_of_range_invalid();
    test_png_set_option_multiple_options_interaction();
    test_png_set_option_multiple_options_interaction(); // additional call for coverage
    test_png_set_option_toggle_same_option();

    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << "\n";
}

// Main entry: run tests and return non-zero if any test failed.
int main() {
    run_all_tests();
    return (g_tests_failed != 0) ? 1 : 0;
}
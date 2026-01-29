// C++11 unit test suite for the focal C function: uarb_copy
// This test suite targets the behavior of uarb_copy as defined in pngfix.c.
// It relies on libpng's types (png_uint_16 and the uarb typedef) being
// available via <png.h>.
// The tests are written without a testing framework (GTest), using a small
// non-terminating assertion mechanism to exercise multiple scenarios.

#include <setjmp.h>
#include <vector>
#include <string.h>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include <ctype.h>


// Import C dependencies: libpng types and the focal function.
// We wrap the C includes and declarations in extern "C" to ensure correct linkage.
extern "C" {
}

// Prototype for the focal function under test.
// The signature must match the C definition in pngfix.c:
extern "C" int uarb_copy(uarb to, uarb from, int idigits);

static int g_failures = 0;

// Lightweight non-terminating assertion helpers
#define EXPECT_TRUE(cond, msg) \
    do { if(!(cond)) { std::cerr << "EXPECT_TRUE failed: " << (msg) \
        << " at " << __FILE__ << ":" << __LINE__ << std::endl; ++g_failures; } } while(0)

#define EXPECT_EQ(a,b,msg) \
    do { if(!((a) == (b))) { std::cerr << "EXPECT_EQ failed: " << (msg) \
        << " (expected " << (a) << ", got " << (b) << ")" << std::endl; ++g_failures; } } while(0)

// Helper to compare two digit arrays (png_uint_16 elements)
static void expect_array_eq(const png_uint_16* a, const png_uint_16* b, size_t n, const char* msg) {
    for (size_t i = 0; i < n; ++i) {
        if (a[i] != b[i]) {
            std::cerr << "EXPECT_EQ failed: " << msg
                      << " at index " << i << " (expected " << b[i]
                      << ", got " << a[i] << ")" << std::endl;
            ++g_failures;
            // Do not early return to maximize coverage across indices
        }
    }
}

// Test 1: All first idigits elements are non-zero -> odigits should be idigits
static void test_all_nonzero_digits() {
    // Domain knowledge: ensure to and from arrays are copied element-wise, and
    // odigits reflects the last non-zero digit index + 1.
    const int digits = 5;
    std::vector<png_uint_16> from_vec(digits);
    std::vector<png_uint_16> to_vec(digits, 0);

    for (int i = 0; i < digits; ++i) {
        from_vec[i] = static_cast<png_uint_16>(i + 1); // [1,2,3,4,5]
    }

    uarb from = from_vec.data();
    uarb to   = to_vec.data();

    int idigits = digits;
    int odigits = uarb_copy(to, from, idigits);

    // Verify all digits are copied correctly
    expect_array_eq(to_vec.data(), from_vec.data(), digits,
                    "test_all_nonzero_digits: to should equal from for all digits");

    // odigits should be equal to idigits when all first idigits digits are non-zero
    EXPECT_EQ(odigits, digits, "test_all_nonzero_digits: odigits should equal idigits when all copied digits are non-zero");
}

// Test 2: Some zeros within the first idigits should not affect higher non-zeros
static void test_mixed_zero_and_nonzero_digits() {
    const int digits = 5;
    std::vector<png_uint_16> from_vec(digits);
    std::vector<png_uint_16> to_vec(digits, 0);

    // from = [1, 0, 3, 0, 4] -> last non-zero at index 4 -> odigits = 5
    from_vec[0] = 1;
    from_vec[1] = 0;
    from_vec[2] = 3;
    from_vec[3] = 0;
    from_vec[4] = 4;

    uarb from = from_vec.data();
    uarb to   = to_vec.data();

    int idigits = digits;
    int odigits = uarb_copy(to, from, idigits);

    // Verify to equals from for all idigits
    expect_array_eq(to_vec.data(), from_vec.data(), digits,
                    "test_mixed_zero_and_nonzero_digits: to should copy from, including zeros");

    // odigits should be index of last non-zero + 1 => 5
    EXPECT_EQ(odigits, digits, "test_mixed_zero_and_nonzero_digits: odigits should be 5 for last non-zero at index 4");
}

// Test 3: idigits = 0 should perform no copying and return 0
static void test_zero_idigits_no_op() {
    const int digits = 3;
    std::vector<png_uint_16> arr_from(digits);
    std::vector<png_uint_16> arr_to(digits);

    // Initialize with distinguishable values
    for (int i = 0; i < digits; ++i) {
        arr_from[i] = static_cast<png_uint_16>(i + 7); // [7,8,9]
        arr_to[i] = static_cast<png_uint_16>(i + 1);   // [1,2,3] to verify unchanged
    }

    uarb from = arr_from.data();
    uarb to   = arr_to.data();

    int idigits = 0;
    int odigits = uarb_copy(to, from, idigits);

    // odigits should be 0
    EXPECT_EQ(odigits, 0, "test_zero_idigits_no_op: odigits should be 0 when idigits is 0");

    // to should remain unchanged
    expect_array_eq(arr_to.data(), (png_uint_16[]){1,2,3}, digits, // using temporary literal array
                    "test_zero_idigits_no_op: to should remain unchanged when idigits is 0");
}

// Test 4: idigits less than digits, ensuring early digits are copied
static void test_partial_copy() {
    const int digits = 5;
    const int idigits = 3;

    std::vector<png_uint_16> from_vec(digits);
    std::vector<png_uint_16> to_vec(digits, 0);

    // from = [0, 1, 2, 3, 4]
    from_vec[0] = 0;
    from_vec[1] = 1;
    from_vec[2] = 2;
    from_vec[3] = 3;
    from_vec[4] = 4;

    uarb from = from_vec.data();
    uarb to   = to_vec.data();

    int odigits = uarb_copy(to, from, idigits);

    // Only first idigits digits copied
    expect_array_eq(to_vec.data(), from_vec.data(), idigits,
                    "test_partial_copy: first idigits elements should be copied exactly");

    // The odigits should reflect the last non-zero among the copied portion.
    // Since from_vec[2] is 2 (non-zero) and there are no non-zeros beyond index 2 in first idigits,
    // odigits should be 3.
    EXPECT_EQ(odigits, idigits, "test_partial_copy: odigits should equal idigits when the last copied digit is non-zero");
}

// Test 5: In-place copy (to == from). Should be safe for overlapping memory (same pointer)
static void test_inplace_copy_overlap() {
    const int digits = 5;
    std::vector<png_uint_16> arr(digits);

    // Initialize with [1,2,0,4,0]
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 0;
    arr[3] = 4;
    arr[4] = 0;

    uarb from = arr.data();
    uarb to   = arr.data();

    int idigits = digits;
    int odigits = uarb_copy(to, from, idigits);

    // The array should be unchanged after in-place copy
    expect_array_eq(arr.data(), (png_uint_16[]){1,2,0,4,0}, digits,
                    "test_inplace_copy_overlap: array should remain unchanged after in-place copy");

    // odigits should reflect the last non-zero among all digits
    // Last non-zero at index 3 -> odigits = 4
    EXPECT_EQ(odigits, 4, "test_inplace_copy_overlap: odigits should be 4 for last non-zero at index 3");
}

// Run all tests and report summary
static void run_all_tests() {
    test_all_nonzero_digits();
    test_mixed_zero_and_nonzero_digits();
    test_zero_idigits_no_op();
    test_partial_copy();
    test_inplace_copy_overlap();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
    } else {
        std::cout << g_failures << " TEST(S) FAILED" << std::endl;
    }
}

int main() {
    // Explain test scope
    std::cout << "Running unit tests for uarb_copy (pngfix.c) in C++11 environment" << std::endl;

    run_all_tests();

    // Return non-zero if any test failed
    return (g_failures == 0) ? 0 : 1;
}
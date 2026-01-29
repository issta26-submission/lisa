#include <cstdint>
#include <iostream>
#include <pngpriv.h>


// Minimal substitution of libpng types and constants to test the focal function.

// Public constants used by png_get_valid
#define PNG_INFO_tRNS 0x01
#define PNG_READ_tRNS_SUPPORTED 1  // Enable the tRNS-specific branch for testing

// Forward-declare minimal structures to mimic libpng's types
struct png_struct;
struct png_info;

// Pseudo-typedefs to resemble libpng's naming (simplified for tests)
typedef const png_struct* png_const_structrp;
typedef png_struct*       png_structrp;

typedef const png_info* png_const_inforp;
typedef png_info*       png_inforp;

typedef unsigned int png_uint_32;

// Minimal definitions to support the focal function logic
struct png_struct {
    unsigned int num_trans; // number of tRNS transparency entries
};

struct png_info {
    unsigned int valid; // bitfield of valid information chunks
};

// The focal function under test (simplified, self-contained version)
png_uint_32 png_get_valid(png_const_structrp png_ptr, png_const_inforp info_ptr,
                          png_uint_32 flag)
{
    if (png_ptr != nullptr && info_ptr != nullptr)
    {
#ifdef PNG_READ_tRNS_SUPPORTED
        // If the caller asks for tRNS validity but there is no transparency data,
        // libpng reports it as not valid. This is the special-case branch we test.
        if (flag == PNG_INFO_tRNS && png_ptr->num_trans == 0)
            return 0;
#endif
        // Otherwise, return the intersection of the info's valid flags with the requested flag(s)
        return info_ptr->valid & flag;
    }
    return 0;
}

// Simple test harness (no GTest/GMock). Uses non-terminating assertions to maximize code execution.
// All tests are standalone functions; main() runs them and reports results.

static int g_failures = 0;
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "ASSERTION FAILED: " << (msg) << std::endl; \
        ++g_failures; \
    } \
} while (0)

#define ASSERT_EQ(expected, actual, msg) do { \
    if (static_cast<decltype(expected)>(actual) != (expected)) { \
        std::cerr << "ASSERT_EQ FAILED: " << (msg) \
                  << " | expected: " << (expected) \
                  << ", actual: " << (actual) << std::endl; \
        ++g_failures; \
    } \
} while (0)

// Test 1: Null pointers should result in zero (non-executable path)
void test_null_pointers_return_zero() {
    // Comments: Ensure that passing a null png_ptr or a null info_ptr yields 0.
    png_struct s;
    s.num_trans = 0;
    png_inforp info = new png_info;
    info->valid = 0xFF; // arbitrary non-zero
    png_ptr = nullptr; // explicitly null
    // Case 1: png_ptr is null
    png_uint_32 res1 = png_get_valid(nullptr, info, 0x01);
    ASSERT_EQ(0u, res1, "png_get_valid should return 0 when png_ptr is NULL");

    // Case 2: info_ptr is null
    png_uint_32 res2 = png_get_valid(reinterpret_cast<png_const_structrp>(&s), nullptr, 0x01);
    ASSERT_EQ(0u, res2, "png_get_valid should return 0 when info_ptr is NULL");

    delete info;
}

// Test 2: Basic bitwise behavior when both pointers are valid
void test_basic_bitwise_and_behavior() {
    // Comments: Verify that the function returns info_ptr->valid AND flag when both pointers are valid.
    png_struct s;
    s.num_trans = 5; // non-zero to avoid tRNS shortcut
    png_structrp png_ptr = &s;

    png_info info;
    // Choose a known valid bitmask; test various flags
    info.valid = 0x0F; // 0000 1111

    // Case: flag selects a subset of bits
    unsigned res = png_get_valid(png_ptr, &info, 0x05); // 0101
    ASSERT_EQ(0x01, res, "png_get_valid should return info.valid & flag (0x0F & 0x05 == 0x01)");
}

// Test 3: tRNS branch when num_trans == 0 should yield 0 (branch active)
void test_tRNS_branch_zero_num_trans() {
    // Comments: With tRNS support enabled, if flag == PNG_INFO_tRNS and num_trans == 0,
    // the function should return 0 irrespective of info_ptr->valid.
    png_struct s;
    s.num_trans = 0; // triggers the tRNS-specific early return
    png_structrp png_ptr = &s;

    png_info info;
    info.valid = 0x01; // some non-zero valid bit
    png_inforp info_ptr = &info;

    png_uint_32 res = png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS);
    ASSERT_EQ(0u, res, "png_get_valid should return 0 when PNG_INFO_tRNS is requested and num_trans == 0");
}

// Test 4: tRNS branch ignored when num_trans != 0 (normal bitwise result)
void test_tRNS_branch_nonzero_num_trans() {
    // Comments: If num_trans != 0, the tRNS branch should not short-circuit; normal
    // bitwise AND applies.
    png_struct s;
    s.num_trans = 3;
    png_structrp png_ptr = &s;

    png_info info;
    info.valid = 0x0A; // 1010

    unsigned res = png_get_valid(png_ptr, &info, PNG_INFO_tRNS); // should be 0x01 (since 0x0A & 0x01 = 0x01)
    // Note: Since flag is exactly PNG_INFO_tRNS (0x01), the result is 0x01
    ASSERT_EQ(0x01u, res, "png_get_valid should perform bitwise AND with PNG_INFO_tRNS when num_trans != 0");
}

int main() {
    // Run tests
    test_null_pointers_return_zero();
    test_basic_bitwise_and_behavior();
    test_tRNS_branch_zero_num_trans();
    test_tRNS_branch_nonzero_num_trans();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << g_failures << " TEST(S) FAILED\n";
        return 1;
    }
}
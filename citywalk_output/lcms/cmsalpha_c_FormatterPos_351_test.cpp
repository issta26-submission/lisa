// Lightweight C++11 test suite for the focal method FormatterPos in cmsalpha.c
// This test harness does not rely on GTest. It uses a small, non-terminating
// assertion mechanism to maximize code execution coverage.
// Note: This test relies on the notion that FormatterPos inspects the following
// pseudo-fields via macros T_BYTES(frm), T_FLOAT(frm), T_ENDIAN16(frm):
//   - b := T_BYTES(frm) in [0,1,2,4,...]
//   - T_FLOAT(frm) := whether the encoding represents a floating type
// The tests here construct "frm" values by packing these fields into a 32-bit
// value in a way that is meant to reflect the library's bit-field layout.
// If your library uses a different encoding, adjust makeFrm() accordingly.

#include <lcms2_internal.h>
#include <iostream>
#include <string>
#include <cstdint>


// Declare the focal function with C linkage for compatibility in C++ test harness.
// We assume cmsUInt32Number is a 32-bit unsigned type in the library, aliasing to uint32_t.
extern "C" int FormatterPos(std::uint32_t frm);

// Helper to pack the test "frm" value.
// This is a convenience that mirrors the intended bit-field extraction in FormatterPos.
// We place:
//   - lower 4 bits: b (bytes per sample, where 0,1,2,4 are used in the focal method)
//   - bit 4: T_FLOAT(frm) (1 means floating, 0 means integral)
//   - bit 5: T_ENDIAN16(frm) (1 means endian16, used for 16-bit cases)
static std::uint32_t makeFrm(unsigned int b, bool isFloat, bool endian16)
{
    // Boundaries: b is placed in lower 4 bits, as the function reads b = T_BYTES(frm)
    // The actual library might encode differently; this wrapper mirrors the intended test intent.
    return (static_cast<std::uint32_t>(b) & 0xF)
           | ((isFloat ? 1u : 0u) << 4)
           | ((endian16 ? 1u : 0u) << 5);
}

// Simple non-terminating assertion framework
static int g_total = 0;
static int g_failures = 0;

#define EXPECT_EQ(actual, expected, msg) do {                                 \
    ++g_total;                                                                  \
    if ((actual) != (expected)) {                                             \
        std::cerr << "EXPECT_FAIL: " << (msg)                                 \
                  << " | actual: " << (actual)                                 \
                  << " != expected: " << (expected) << "\n";                  \
        ++g_failures;                                                          \
    }                                                                          \
} while (false)

#define TEST_PASSED_MSG(msg) do {                                              \
    std::cout << "TEST_PASSED: " << (msg) << "\n";                              \
} while (false)

// Test 1: DBL case (b == 0 and T_FLOAT(frm) == true) -> expected result 5
// This validates the first true-branch of FormatterPos.
static void test_formatterpos_dbl_case()
{
    // b = 0, floating, endian irrelevant
    std::uint32_t frm = makeFrm(0, true, false);
    int res = FormatterPos(frm);
    EXPECT_EQ(res, 5, "DBL case: b=0, float");
    if (g_failures == 0) TEST_PASSED_MSG("test_formatterpos_dbl_case");
}

// Test 2: HLF case (b == 2 and T_FLOAT(frm) == true) -> expected result 3
// Only compiled if CMS_NO_HALF_SUPPORT is not defined.
#ifndef CMS_NO_HALF_SUPPORT
static void test_formatterpos_hlf_case()
{
    // b = 2, floating
    std::uint32_t frm = makeFrm(2, true, false);
    int res = FormatterPos(frm);
    EXPECT_EQ(res, 3, "HLF case: b=2, float (HALF)");
    if (g_failures == 0) TEST_PASSED_MSG("test_formatterpos_hlf_case");
}
#else
static void test_formatterpos_hlf_case()
{
    // Skipped when HALF support is disabled
    TEST_PASSED_MSG("test_formatterpos_hlf_case (skipped due to CMS_NO_HALF_SUPPORT)");
}
#endif

// Test 3: FLT case (b == 4 and T_FLOAT(frm) == true) -> expected result 4
static void test_formatterpos_flt_case()
{
    // b = 4, floating
    std::uint32_t frm = makeFrm(4, true, false);
    int res = FormatterPos(frm);
    EXPECT_EQ(res, 4, "FLT case: b=4, float");
    if (g_failures == 0) TEST_PASSED_MSG("test_formatterpos_flt_case");
}

// Test 4: 16SE case (b == 2 and !T_FLOAT(frm) and T_ENDIAN16(frm) == true) -> 2
static void test_formatterpos_16se_case()
{
    // b = 2, not float, endian16 = true
    std::uint32_t frm = makeFrm(2, false, true);
    int res = FormatterPos(frm);
    EXPECT_EQ(res, 2, "16SE case: b=2, not float, endian16=true");
    if (g_failures == 0) TEST_PASSED_MSG("test_formatterpos_16se_case");
}

// Test 5: 16 case (b == 2 and !T_FLOAT(frm) and T_ENDIAN16(frm) == false) -> 1
static void test_formatterpos_16_case()
{
    // b = 2, not float, endian16 = false
    std::uint32_t frm = makeFrm(2, false, false);
    int res = FormatterPos(frm);
    EXPECT_EQ(res, 1, "16 case: b=2, not float, endian16=false");
    if (g_failures == 0) TEST_PASSED_MSG("test_formatterpos_16_case");
}

// Test 6: 8-bit non-floating (b == 1 and !T_FLOAT(frm)) -> 0
static void test_formatterpos_8bit_case()
{
    // b = 1, not float
    std::uint32_t frm = makeFrm(1, false, false);
    int res = FormatterPos(frm);
    EXPECT_EQ(res, 0, "8-bit case: b=1, not float");
    if (g_failures == 0) TEST_PASSED_MSG("test_formatterpos_8bit_case");
}

// Test 7: Not recognized (e.g., b == 3 with any flags) -> -1
static void test_formatterpos_not_recognized_case()
{
    // b = 3, any flags
    std::uint32_t frm = makeFrm(3, false, false);
    int res = FormatterPos(frm);
    EXPECT_EQ(res, -1, "Not recognized case: b=3, not float");
    if (g_failures == 0) TEST_PASSED_MSG("test_formatterpos_not_recognized_case");
}

// Runner to execute all tests
static void run_all_tests()
{
    test_formatterpos_dbl_case();
    test_formatterpos_hlf_case();
    test_formatterpos_flt_case();
    test_formatterpos_16se_case();
    test_formatterpos_16_case();
    test_formatterpos_8bit_case();
    test_formatterpos_not_recognized_case();

    // Summary
    std::cout << "Test Summary: " << g_failures << " failures out of " << g_total << " tests.\n";
}

// Main entry point
int main()
{
    run_all_tests();
    // Return non-zero if any test failed, enabling integration with simple CI scripts.
    return g_failures ? 1 : 0;
}
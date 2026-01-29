/*
   Lightweight unit test suite for the focal function:
   cmsUInt32Number BaseToBase(cmsUInt32Number in, int BaseIn, int BaseOut)

   Notes:
   - No Google Test or similar frameworks used. A minimal in-process test harness is implemented.
   - Tests exercise multiple representative inputs to exercise both digit extraction (BaseIn) and reconstruction
     (BaseOut) paths.
   - All tests are non-terminating (they report pass/fail but do not abort on failure).
   - The tests rely on the project-provided type cmsUInt32Number and the function BaseToBase with C linkage.
*/

#include <lcms2_internal.h>
#include <iostream>


// Include dependency headers to bring in cmsUInt32Number and any needed declarations.
// The focal class dependencies reveal that cmsUInt32Number and BaseToBase are part of the project.
extern "C" {
}

// Forward declaration of the focal function with C linkage.
// If the actual header provides the prototype, this extern is optional; kept here for robustness.
extern "C" cmsUInt32Number BaseToBase(cmsUInt32Number in, int BaseIn, int BaseOut);

// Simple, non-terminating assertion helper for unsigned 32-bit results.
static int g_failures = 0;

static void assert_uint32_equal(const char* testName, cmsUInt32Number got, cmsUInt32Number expected) {
    if (got != expected) {
        std::cerr << "[FAIL] " << testName
                  << " - expected " << static_cast<uint64_t>(expected)
                  << ", got " << static_cast<uint64_t>(got) << std::endl;
        ++g_failures;
    } else {
        std::cout << "[PASS] " << testName << std::endl;
    }
}

/*
   Test 1: BaseToBase should handle zero input gracefully.
   - In = 0, BaseIn = 10, BaseOut = 2
   - Expected: 0
*/
static void test_BaseToBase_zero_input() {
    cmsUInt32Number in = 0;
    cmsUInt32Number result = BaseToBase(in, 10, 2);
    assert_uint32_equal("BaseToBase(0,10,2)", result, 0u);
}

/*
   Test 2: Convert decimal digits from base 10 to base 16.
   - In = 12345 (base 10)
   - BaseIn = 10, BaseOut = 16
   - The function should interpret digits [1,2,3,4,5] as a number in base 16 which equals
     1*16^4 + 2*16^3 + 3*16^2 + 4*16 + 5 = 74565
*/
static void test_BaseToBase_decimal10_to_base16() {
    cmsUInt32Number in = 12345;
    cmsUInt32Number result = BaseToBase(in, 10, 16);
    assert_uint32_equal("12345_10_to_16", result, 74565u);
}

/*
   Test 3: Convert a small binary value to decimal.
   - In = 5, BaseIn = 2 (binary 101)
   - BaseOut = 10
   - Expected: 1*(10^2) + 0*(10^1) + 1 = 101
*/
static void test_BaseToBase_binary5_to_decimal() {
    cmsUInt32Number in = 5;
    cmsUInt32Number result = BaseToBase(in, 2, 10);
    assert_uint32_equal("5_in_bin_to_decimal_base10", result, 101u);
}

/*
   Test 4: Single-digit preserve when BaseOut is 2.
   - In = 1, BaseIn = 10, BaseOut = 2
   - Digit '1' remains as 1 in the new base.
*/
static void test_BaseToBase_single_digit_preserved() {
    cmsUInt32Number in = 1;
    cmsUInt32Number result = BaseToBase(in, 10, 2);
    assert_uint32_equal("single_digit_preserved", result, 1u);
}

/*
   Test 5: Large value edge-case in/out in hex space.
   - In = 0xFFFFFFFF, BaseIn = 16, BaseOut = 16
   - Expect the same 32-bit value back: 0xFFFFFFFF (4294967295)
*/
static void test_BaseToBase_large_hex_roundtrip() {
    cmsUInt32Number in = 0xFFFFFFFFu;
    cmsUInt32Number result = BaseToBase(in, 16, 16);
    assert_uint32_equal("0xFFFFFFFF_roundtrip_base16", result, static_cast<cmsUInt32Number>(0xFFFFFFFFu));
}

/*
   Test 6: Non-trivial mixed bases to verify digit sequence handling.
   - In = 7, BaseIn = 3 (base-3 digits: 21), BaseOut = 9
   - Expected: digits reversed interpreted in base 9 -> 2*9 + 1 = 19
*/
static void test_BaseToBase_three_to_nine() {
    cmsUInt32Number in = 7;
    cmsUInt32Number result = BaseToBase(in, 3, 9);
    assert_uint32_equal("7_in_base3_to_base9", result, 19u);
}

int main() {
    // Run all tests
    test_BaseToBase_zero_input();
    test_BaseToBase_decimal10_to_base16();
    test_BaseToBase_binary5_to_decimal();
    test_BaseToBase_single_digit_preserved();
    test_BaseToBase_large_hex_roundtrip();
    test_BaseToBase_three_to_nine();

    if (g_failures != 0) {
        std::cerr << "Total failures: " << g_failures << std::endl;
        return 1;
    }

    std::cout << "All tests passed." << std::endl;
    return 0;
}
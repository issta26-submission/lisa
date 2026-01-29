/*****************************************************
 * Unit Test Suite for From15To16 (focal method)
 * 
 * Context:
 * - Function under test: From15To16
 * - Location in focal class: From15To16(cmsUInt16Number x15)
 * - Core computation (as in focal method):
 *     r64 = ((uint64_t) x15 * 0xFFFF + 0x4000) >> 15
 *     return (cmsUInt16Number) r64;
 * 
 * Notes:
 * - This test suite is self-contained and does not use GTest.
 * - Uses a lightweight test harness with non-terminating assertions
 *   to maximize code coverage.
 * - Types are defined to mirror the focal C/C++ types.
 * - Includes explanatory comments for each unit test.
 *****************************************************/

/* 
   The test is designed to be compiled with a C++11 compliant compiler.
   It mirrors the data types and arithmetic used by the focal method. 
*/
#include <iostream>
#include <cstdint>
#include <fast_float_internal.h>


// Domain-aligned typedefs (mirror cms* types in the focal code)
typedef uint16_t cmsUInt16Number;
typedef uint8_t  cmsUInt8Number;
typedef uint64_t cmsUInt64Number;

// Inline macro matching the focal code style
#define cmsINLINE inline

// Focal method under test
cmsINLINE cmsUInt16Number From15To16(cmsUInt16Number x15)
{
    cmsUInt64Number r64 = ((cmsUInt64Number) x15 * 0xFFFF + 0x4000L) >> 15;
    return (cmsUInt16Number)r64;
}

// Simple non-terminating test harness
static int g_failures = 0;
#define EXPECT_EQ(actual, expected, test_name)                                         \
    do {                                                                                 \
        if ((actual) != (expected)) {                                                  \
            std::cerr << "FAIL: " << (test_name) << " | Expected: " << (expected           \
                      << " | Actual: " << (actual) << std::endl;                       \
            ++g_failures;                                                               \
        } else {                                                                         \
            std::cout << "PASS: " << (test_name) << std::endl;                          \
        }                                                                                \
    } while (0)

// Test 1: Zero input mapping
// Scenario: x15 = 0 should map to 0 after the scale/rounding operation.
static void test_From15To16_zero_input()
{
    cmsUInt16Number input = 0;
    cmsUInt16Number actual = From15To16(input);
    cmsUInt16Number expected = 0; // (0 * 0xFFFF + 0x4000) >> 15 == 0
    EXPECT_EQ(actual, expected, "From15To16(0) should be 0");
}

// Test 2: Small input mapping with rounding behavior
// Scenario: x15 = 1 should yield a small non-zero value due to rounding.
static void test_From15To16_small_input()
{
    cmsUInt16Number input = 1;
    cmsUInt16Number actual = From15To16(input);
    cmsUInt16Number expected = 2; // (1*65535 + 16384) >> 15 == 2
    EXPECT_EQ(actual, expected, "From15To16(1) should be 2");
}

// Test 3: Small input mapping (x=2) to ensure linear portion
// Scenario: x15 = 2 should map to 4
static void test_From15To16_small_input2()
{
    cmsUInt16Number input = 2;
    cmsUInt16Number actual = From15To16(input);
    cmsUInt16Number expected = 4; // (2*65535 + 16384) >> 15 == 4
    EXPECT_EQ(actual, expected, "From15To16(2) should be 4");
}

// Test 4: Small input mapping (x=3) to ensure linear progression continues
// Scenario: x15 = 3 should map to 6
static void test_From15To16_small_input3()
{
    cmsUInt16Number input = 3;
    cmsUInt16Number actual = From15To16(input);
    cmsUInt16Number expected = 6; // (3*65535 + 16384) >> 15 == 6
    EXPECT_EQ(actual, expected, "From15To16(3) should be 6");
}

// Test 5: Mid-range input to verify correct scaling
// Scenario: x15 = 16384 (midpoint of 0..32767) should map to 32768
static void test_From15To16_midrange()
{
    cmsUInt16Number input = 16384;
    cmsUInt16Number actual = From15To16(input);
    cmsUInt16Number expected = 32768; // (16384*65535 + 16384) >> 15 == 32768
    EXPECT_EQ(actual, expected, "From15To16(16384) should be 32768");
}

// Test 6: Near-maximum input to validate handling close to upper bound
// Scenario: x15 = 32766 should map to 65532
static void test_From15To16_near_max()
{
    cmsUInt16Number input = 32766;
    cmsUInt16Number actual = From15To16(input);
    cmsUInt16Number expected = 65532; // precomputed via formula
    EXPECT_EQ(actual, expected, "From15To16(32766) should be 65532");
}

// Test 7: Maximum 15-bit input to ensure rounding behavior near edge
// Scenario: x15 = 32767 should map to 65534
static void test_From15To16_max_input()
{
    cmsUInt16Number input = 32767;
    cmsUInt16Number actual = From15To16(input);
    cmsUInt16Number expected = 65534; // precomputed via formula
    EXPECT_EQ(actual, expected, "From15To16(32767) should be 65534");
}

// Test 8: Monotonicity check to cover general behavior
// Scenario: Ensure that From15To16 is non-decreasing over range
static void test_From15To16_monotonic()
{
    cmsUInt16Number a = 0;
    cmsUInt16Number b = 32767;
    cmsUInt16Number va = From15To16(a);
    cmsUInt16Number vb = From15To16(b);
    bool ok = (va <= vb);
    if (ok) {
        std::cout << "PASS: From15To16 monotonicity (0 -> 32767)" << std::endl;
    } else {
        std::cerr << "FAIL: From15To16 monotonicity (0 -> 32767)" << std::endl;
        ++g_failures;
    }
}

// Runner to execute all tests
static void run_all_tests()
{
    test_From15To16_zero_input();
    test_From15To16_small_input();
    test_From15To16_small_input2();
    test_From15To16_small_input3();
    test_From15To16_midrange();
    test_From15To16_near_max();
    test_From15To16_max_input();
    test_From15To16_monotonic();
}

int main()
{
    run_all_tests();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " TEST(S) FAILED" << std::endl;
        return g_failures;
    }
}
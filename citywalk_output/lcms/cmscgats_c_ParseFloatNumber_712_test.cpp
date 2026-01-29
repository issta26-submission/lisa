// Unit test suite for the focal method: ParseFloatNumber in cmscgats.c
// This test suite is written in C++11 (no Google Test). It uses a lightweight
// test harness with non-terminating assertions to maximize coverage.

#include <cctype>
#include <lcms2_internal.h>
#include <iostream>
#include <cmath>


// Domain knowledge and assumptions:
// - cmsFloat64Number is equivalent to double in typical cms code bases.
// - The function ParseFloatNumber has C linkage and accepts a const char* Buffer.
// - We link against the cmscgats.c implementation during compilation.

// Provide a minimal, safe alias to match the expected external type without pulling a full CMS header.
typedef double cmsFloat64Number;

// Declare the focal function with C linkage to match the implementation in cmscgats.c
extern "C" cmsFloat64Number ParseFloatNumber(const char *Buffer);

// Lightweight test harness: non-terminating assertions to maximize code execution.
// Global counter for test failures.
static int gTestFailures = 0;

// Helpers: non-terminating assertions and numeric comparisons with tolerance.
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << #cond << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++gTestFailures; \
    } \
} while(0)

#define EXPECT_DOUBLE_EQ(a, b, tol) do { \
    double _a = (double)(a); \
    double _b = (double)(b); \
    if (std::fabs(_a - _b) > (tol)) { \
        std::cerr << "EXPECT_DOUBLE_EQ failed: " << _a << " != " << _b \
                  << " (tol=" << (tol) << ") at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++gTestFailures; \
    } \
} while(0)

// Individual test cases. Each test is prefixed with a descriptive comment explaining its purpose.

// Test 1: Buffer == NULL should safely return 0.0
static void test_ParseFloatNumber_NullBuffer() {
    // Purpose: Verify that a null input yields 0.0 and does not crash.
    cmsFloat64Number res = ParseFloatNumber(nullptr);
    EXPECT_DOUBLE_EQ(res, 0.0, 1e-12);
}

// Test 2: Simple integer parsing without decimal or exponent
static void test_ParseFloatNumber_IntegerOnly() {
    // Purpose: Ensure integer values are parsed correctly.
    cmsFloat64Number res = ParseFloatNumber("42");
    EXPECT_DOUBLE_EQ(res, 42.0, 1e-12);
}

// Test 3: Positive and negative sign handling with integer
static void test_ParseFloatNumber_SignedInteger() {
    // Purpose: Check sign handling for '+' and '-' with an integer part.
    cmsFloat64Number resPlus = ParseFloatNumber("+123");
    EXPECT_DOUBLE_EQ(resPlus, 123.0, 1e-12);

    cmsFloat64Number resMinus = ParseFloatNumber("-123");
    EXPECT_DOUBLE_EQ(resMinus, -123.0, 1e-12);
}

// Test 4: Decimal numbers without leading digits (e.g., ".5", "-.5")
static void test_ParseFloatNumber_LeadingDecimal() {
    // Purpose: Validate parsing when integer part is missing before the decimal point.
    cmsFloat64Number res1 = ParseFloatNumber(".5");
    EXPECT_DOUBLE_EQ(res1, 0.5, 1e-12);

    cmsFloat64Number res2 = ParseFloatNumber("-.5");
    EXPECT_DOUBLE_EQ(res2, -0.5, 1e-12);
}

// Test 5: Decimal numbers with integer part (e.g., "12.34")
static void test_ParseFloatNumber_DecimalPart() {
    // Purpose: Ensure correct handling of fractional part after the decimal point.
    cmsFloat64Number res = ParseFloatNumber("12.34");
    EXPECT_DOUBLE_EQ(res, 12.34, 1e-12);
}

// Test 6: Decimal with exponent (positive exponent)
static void test_ParseFloatNumber_DecimalWithExponent_PosExp() {
    // Purpose: Verify proper exponent application for positive exponents.
    cmsFloat64Number res = ParseFloatNumber("12.34E2"); // 12.34 * 100 = 1234
    EXPECT_DOUBLE_EQ(res, 1234.0, 1e-12);
}

// Test 7: Decimal with exponent (negative exponent)
static void test_ParseFloatNumber_DecimalWithExponent_NegExp() {
    // Purpose: Verify exponent handling with a negative exponent.
    cmsFloat64Number res = ParseFloatNumber("1.5E-2"); // 1.5 * 0.01 = 0.015
    EXPECT_DOUBLE_EQ(res, 0.015, 1e-12);
}

// Test 8: Exponent with lowercase 'e'
static void test_ParseFloatNumber_Exponent_LowercaseE() {
    // Purpose: Ensure that lowercase 'e' is treated the same as uppercase 'E'.
    cmsFloat64Number res = ParseFloatNumber("2.5e2"); // 2.5 * 100 = 250
    EXPECT_DOUBLE_EQ(res, 250.0, 1e-12);
}

// Test 9: Exponent with explicit plus sign
static void test_ParseFloatNumber_Exponent_PlusSign() {
    // Purpose: Check exponent with '+' sign.
    cmsFloat64Number res = ParseFloatNumber("3.0E+2"); // 3.0 * 100 = 300
    EXPECT_DOUBLE_EQ(res, 300.0, 1e-12);
}

// Test 10: No digits after sign (only '+' or '-')
static void test_ParseFloatNumber_SignOnlyNoDigits() {
    // Purpose: Ensure sign without digits returns 0 (not a crash); both '+' and '-' cases.
    cmsFloat64Number resPlus = ParseFloatNumber("+");
    EXPECT_DOUBLE_EQ(resPlus, 0.0, 1e-12);

    cmsFloat64Number resMinus = ParseFloatNumber("-");
    EXPECT_DOUBLE_EQ(resMinus, 0.0, 1e-12);
}

// Test 11: Combined case with decimal and exponent (e.g., "0.123E0" => 0.123)
static void test_ParseFloatNumber_DecimalWithZeroExponent() {
    cmsFloat64Number res = ParseFloatNumber("0.123E0");
    EXPECT_DOUBLE_EQ(res, 0.123, 1e-12);
}

// Test 12: Zero with decimal and no exponent to ensure zero handling
static void test_ParseFloatNumber_ZeroVariants() {
    cmsFloat64Number res1 = ParseFloatNumber("0.");
    EXPECT_DOUBLE_EQ(res1, 0.0, 1e-12);

    cmsFloat64Number res2 = ParseFloatNumber("0.0");
    EXPECT_DOUBLE_EQ(res2, 0.0, 1e-12);
}

// Test 13: Exponent limits sanity (small value with negative exponent)
static void test_ParseFloatNumber_ExponentSmallValue() {
    cmsFloat64Number res = ParseFloatNumber("9E-9"); // 9 * 1e-9
    EXPECT_DOUBLE_EQ(res, 9e-9, 1e-18);
}

// Run all tests
static void RunAllTests() {
    test_ParseFloatNumber_NullBuffer();
    test_ParseFloatNumber_IntegerOnly();
    test_ParseFloatNumber_SignedInteger();
    test_ParseFloatNumber_LeadingDecimal();
    test_ParseFloatNumber_DecimalPart();
    test_ParseFloatNumber_DecimalWithExponent_PosExp();
    test_ParseFloatNumber_DecimalWithExponent_NegExp();
    test_ParseFloatNumber_Exponent_LowercaseE();
    test_ParseFloatNumber_Exponent_PlusSign();
    test_ParseFloatNumber_SignOnlyNoDigits();
    test_ParseFloatNumber_DecimalWithZeroExponent();
    test_ParseFloatNumber_ZeroVariants();
    test_ParseFloatNumber_ExponentSmallValue();
}

// Entry point
int main() {
    RunAllTests();
    if (gTestFailures == 0) {
        std::cout << "All ParseFloatNumber tests passed.\n";
        return 0;
    } else {
        std::cerr << gTestFailures << " ParseFloatNumber test(s) failed.\n";
        return 1;
    }
}
// Unit test suite for Lab2ITU (located in itufax.c)
// This test suite uses a lightweight, non-terminating assertion strategy
// (no GTest). It targets the focal method Lab2ITU and validates output
// against exact mathematical expectations derived from the implementation.
//
// Important: This test relies on the project's external dependencies
// (lcms.h, cmsCIELab, WORD, LPcmsCIELab, etc.). It declares Lab2ITU
// with C linkage to ensure correct symbol resolution when linked with the C
// implementation. Compile with the corresponding source files (e.g., itufax.c).

#include <iostream>
#include <cstdint>
#include <cmath>
#include <lcms.h>


// Import the project-specific headers that define the CMS types.
// The test assumes the same definitions used by Lab2ITU (WORD, LPcmsCIELab, cmsCIELab, etc.).

// Ensure we have a kinematic prototype for Lab2ITU matching the focal code.
extern "C" void Lab2ITU(LPcmsCIELab Lab, WORD Out[3]);

// Global counters for a lightweight test harness (non-terminating assertions).
static int g_testCount = 0;
static int g_failures = 0;

// Lightweight assertion macro that does not terminate the program on failure
// and records the result for summary output.
#define TEST_ASSERT_EQ(expected, actual, message) do { \
    ++g_testCount; \
    if ((expected) != (actual)) { \
        ++g_failures; \
        std::cerr << "[FAIL] " << message \
                  << " | expected: " << (expected) \
                  << ", actual: " << (actual) << std::endl; \
    } else { \
        std::cout << "[PASS] " << message << std::endl; \
    } \
} while (0)

// Candidate keywords distilled from the focal method (Step 1):
// - Lab (L, a, b components of cmsCIELab)
// - Out[3] (3-channel LAB-to-ITU conversion output)
// - floor, 65535, 0.5 (rounding behavior)
// - Offsets: +32768.0 for 'a', +24576.0 for 'b'
// - Scaling denominators: 100 for L, 170 for a, 200 for b
// These guide the expected-value calculations for tests.

// Helper: expected Out[0] for L (0..100) -> 0..65535 with rounding
static inline unsigned short expected_Out0(double L)
{
    return static_cast<unsigned short>(std::floor((L / 100.0) * 65535.0 + 0.5));
}

// Helper: expected Out[1] for a with offset 32768
static inline unsigned short expected_Out1(int a)
{
    return static_cast<unsigned short>(std::floor((static_cast<double>(a) / 170.0) * 65535.0 + 32768.0 + 0.5));
}

// Helper: expected Out[2] for b with offset 24576
static inline unsigned short expected_Out2(int b)
{
    return static_cast<unsigned short>(std::floor((static_cast<double>(b) / 200.0) * 65535.0 + 24576.0 + 0.5));
}

// Test 1: L=0, a=0, b=0 -> Out should be [0, 32768, 24576]
static void test_Lab2ITU_zero_values()
{
    // Setup Lab values
    cmsCIELab lab;
    lab.L = 0.0;
    lab.a = 0.0;
    lab.b = 0.0;

    WORD Out[3] = {0, 0, 0};

    // Act
    Lab2ITU(&lab, Out);

    // Assert
    TEST_ASSERT_EQ(expected_Out0(lab.L), Out[0], "Lab2ITU: L channel at L=0 should be 0");
    TEST_ASSERT_EQ(expected_Out1(static_cast<int>(lab.a)), Out[1], "Lab2ITU: a channel at a=0 should be 32768");
    TEST_ASSERT_EQ(expected_Out2(static_cast<int>(lab.b)), Out[2], "Lab2ITU: b channel at b=0 should be 24576");
}

// Test 2: L=100, a=0, b=0 -> Out should be [65535, 32768, 24576]
static void test_Lab2ITU_max_L_values()
{
    cmsCIELab lab;
    lab.L = 100.0;
    lab.a = 0.0;
    lab.b = 0.0;

    WORD Out[3] = {0, 0, 0};
    Lab2ITU(&lab, Out);

    TEST_ASSERT_EQ(expected_Out0(lab.L), Out[0], "Lab2ITU: L channel at L=100 should be 65535");
    TEST_ASSERT_EQ(expected_Out1(static_cast<int>(lab.a)), Out[1], "Lab2ITU: a channel at a=0 should be 32768");
    TEST_ASSERT_EQ(expected_Out2(static_cast<int>(lab.b)), Out[2], "Lab2ITU: b channel at b=0 should be 24576");
}

// Test 3: L=50, a=0, b=0 -> Middle value for L channel (rounding behavior)
static void test_Lab2ITU_half_L_value()
{
    cmsCIELab lab;
    lab.L = 50.0;
    lab.a = 0.0;
    lab.b = 0.0;

    WORD Out[3] = {0, 0, 0};
    Lab2ITU(&lab, Out);

    TEST_ASSERT_EQ(expected_Out0(lab.L), Out[0], "Lab2ITU: L channel at L=50 should be rounded to ~32768");
    TEST_ASSERT_EQ(expected_Out1(static_cast<int>(lab.a)), Out[1], "Lab2ITU: a channel at a=0 should be 32768");
    TEST_ASSERT_EQ(expected_Out2(static_cast<int>(lab.b)), Out[2], "Lab2ITU: b channel at b=0 should be 24576");
}

// Test 4: Negative a value tests modular wrap-around behavior on unsigned Out[1]
static void test_Lab2ITU_negative_a_wrap()
{
    cmsCIELab lab;
    lab.L = 0.0;
    lab.a = -170;  // Causes negative intermediate value before casting to WORD
    lab.b = 0.0;

    WORD Out[3] = {0, 0, 0};
    Lab2ITU(&lab, Out);

    // Expected based on formula with wrap-around cast to unsigned 16-bit
    unsigned short expected1 = expected_Out1(static_cast<int>(lab.a));
    TEST_ASSERT_EQ(expected_Out0(lab.L), Out[0], "Lab2ITU: L=0 -> Out0 should be 0");
    TEST_ASSERT_EQ(expected1, Out[1], "Lab2ITU: a negative value wrap-around for a=-170");
    TEST_ASSERT_EQ(expected_Out2(static_cast<int>(lab.b)), Out[2], "Lab2ITU: b=0 should be 24576");
}

// Test 5: Negative b value tests wrap-around on Out[2]
static void test_Lab2ITU_negative_b_wrap()
{
    cmsCIELab lab;
    lab.L = 0.0;
    lab.a = 0.0;
    lab.b = -200;  // Causes negative intermediate value before casting to WORD

    WORD Out[3] = {0, 0, 0};
    Lab2ITU(&lab, Out);

    unsigned short expected2 = expected_Out2(static_cast<int>(lab.b));
    TEST_ASSERT_EQ(expected_Out0(lab.L), Out[0], "Lab2ITU: L=0 -> Out0 should be 0");
    TEST_ASSERT_EQ(expected_Out1(static_cast<int>(lab.a)), Out[1], "Lab2ITU: a=0 -> Out1 should be 32768");
    TEST_ASSERT_EQ(expected2, Out[2], "Lab2ITU: b negative wrap-around for b=-200");
}

// Main: Run all tests and report a summary
int main()
{
    std::cout << "Starting Lab2ITU unit tests (C++11, no GTest)..." << std::endl;

    test_Lab2ITU_zero_values();
    test_Lab2ITU_max_L_values();
    test_Lab2ITU_half_L_value();
    test_Lab2ITU_negative_a_wrap();
    test_Lab2ITU_negative_b_wrap();

    // Summary
    std::cout << "Tests executed: " << g_testCount
              << ", Failures: " << g_failures << std::endl;

    // Non-terminating behavior: return non-zero if there were failures
    return (g_failures != 0) ? 1 : 0;
}
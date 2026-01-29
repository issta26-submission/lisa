/*
  Unit test suite for the focal method:
    void from16SEtoFLT(void* dst, const void* src)

  Step 1 (Conceptual):
  - Core operation: take a 16-bit unsigned source, apply a endianness swap (CHANGE_ENDIAN),
    then scale to a float in [0, 1] by dividing by 65535.0f.
  - Key dependencies from the focal class section:
    cmsUInt16Number, cmsFloat32Number, CHANGE_ENDIAN macro.

  Step 2 (Test Plan derived from dependencies):
  - Validate boundary values: 0 -> 0.0f, 0xFFFF -> 1.0f
  - Validate endianness swap effect: some non-trivial value (e.g., 0x1234) should yield
    swapped value (0x3412) divided by 65535.0f.
  - Ensure the function writes exactly one cmsFloat32Number worth of data to dst and uses
    the provided src value as input.

  Step 3 (Refinement considerations):
  - Use a small tolerance for floating comparisons due to potential precision differences.
  - Use a lightweight, self-contained test harness (no GTest) with non-terminating checks.
  - Access the library’s types via lcms2_internal.h and call from16SEtoFLT directly.
  - Do not rely on private members or non-exported symbols.
*/

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cmath>


// Import library dependencies (as per provided class dependencies)

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void log_failure(const char* testName, const char* reason) {
    std::cerr << "Test failed: " << testName << " - " << reason << std::endl;
    ++g_failed_tests;
}

static void CHECK(bool condition, const char* testName, const char* reason) {
    ++g_total_tests;
    if (!condition) {
        log_failure(testName, reason);
    }
}

/*
  Test 1: Zero input should yield zero output.
  dst = from16SEtoFLT(&dst, &src) with src == 0
  Expect dst == 0.0f
*/
static void test_from16SEtoFLT_zero() {
    const char* testName = "test_from16SEtoFLT_zero";

    cmsUInt16Number src = 0x0000;
    cmsFloat32Number dst = 0.0f;

    from16SEtoFLT(&dst, &src);

    CHECK(dst == 0.0f, testName, "dst should be exactly 0.0f for input 0");
}

/*
  Test 2: Maximum input should yield 1.0f after endianness swap and scaling.
  src = 0xFFFF, swap -> 0xFFFF, 0xFFFF / 65535.0f == 1.0f
*/
static void test_from16SEtoFLT_max() {
    const char* testName = "test_from16SEtoFLT_max";

    cmsUInt16Number src = 0xFFFF;
    cmsFloat32Number dst = 0.0f;

    from16SEtoFLT(&dst, &src);

    CHECK(dst == 1.0f, testName, "dst should be exactly 1.0f for input 0xFFFF");
}

/*
  Test 3: Endianness swap effect with a non-trivial value.
  Example: src = 0x1234 -> swapped value = 0x3412 = 13330
  Expected output = 13330 / 65535.0f
*/
static void test_from16SEtoFLT_swap_effect() {
    const char* testName = "test_from16SEtoFLT_swap_effect";

    cmsUInt16Number src = 0x1234; // 0x3412 after swap -> 13330
    cmsFloat32Number dst = 0.0f;

    from16SEtoFLT(&dst, &src);

    const cmsFloat32Number expected = 13330.0f / 65535.0f;
    const cmsFloat32Number tol = 1e-6f;

    CHECK(std::fabs(dst - expected) <= tol,
          testName,
          "dst should equal swapped_value / 65535.0f (13330/65535) within tolerance");
}

/*
  Main function:
  - Run all tests
  - Print summary and return non-zero if any test failed
*/
int main() {
    // Run tests
    test_from16SEtoFLT_zero();
    test_from16SEtoFLT_max();
    test_from16SEtoFLT_swap_effect();

    // Summary
    std::cout << "Total tests run: " << g_total_tests << std::endl;
    std::cout << "Total failures:  " << g_failed_tests << std::endl;

    return (g_failed_tests == 0) ? 0 : 1;
}
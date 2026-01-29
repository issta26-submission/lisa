// Test suite for png_fp_sub in C++11 without using GTest.
// The focal function is assumed to be provided by the project (png.c) with C linkage.
// We declare its prototype here and exercise its various branches.
// Note: This test harness uses a lightweight, non-terminating assertion approach.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <limits>


// Type alias matching the library's expected type.
// The real project likely defines this in its headers; we mirror it here for test purposes.
using png_int_32 = int32_t;

// Declaration of the focal function (C linkage) from the project under test.
extern "C" int png_fp_sub(png_int_32 addend0, png_int_32 addend1, int *error);

// Lightweight test harness state
static int gTotalTests = 0;
static int gFailedTests = 0;

// Utility to report a failure without stopping test execution
static void report_failure(const char* testName, const char* message) {
    ++gFailedTests;
    std::cerr << "Test " << testName << " FAILED: " << message << std::endl;
}

// Test 1: addend1 > 0 and -0x7fffffff + addend1 <= addend0 holds -> should compute addend0 - addend1 without touching error
// Expected: ret == addend0 - addend1; error remains unchanged.
void test_png_fp_sub_case_positive_branch_condition_true() {
    const char* testName = __func__;
    ++gTotalTests;

    int error = 999999; // sentinel to detect any unintended modification
    png_int_32 addend0 = 100;
    png_int_32 addend1 = 50;

    int ret = png_fp_sub(addend0, addend1, &error);

    bool ok = (ret == (addend0 - addend1)) && (error == 999999);
    if (!ok) {
        report_failure(testName,
            "Expected return = addend0 - addend1 and error unchanged (early return path for addend1 > 0).");
    }
}

// Test 2: addend1 > 0 but condition false -> should hit overflow protection path
// Use minimum int to ensure the inner condition fails (-0x7fffffff + addend1 > addend0 is false for INT_MIN, 1)
void test_png_fp_sub_case_positive_branch_condition_false_overflow() {
    const char* testName = __func__;
    ++gTotalTests;

    int error = 0;
    png_int_32 addend0 = std::numeric_limits<png_int_32>::min(); // INT_MIN
    png_int_32 addend1 = 1;

    int ret = png_fp_sub(addend0, addend1, &error);

    // The expected value is PNG_FP_1/2. Assuming PNG_FP_1 == 1<<16 (65536), PNG_FP_1/2 == 32768.
    // We use the conventional LibPNG-inspired constant 32768 as the expected value.
    const int expected_fp_half = 32768;

    if (!(ret == expected_fp_half && error == 1)) {
        report_failure(testName,
            "Expected return = PNG_FP_1/2 and error set to 1 (overflow path for addend1 > 0).");
    }
}

// Test 3: addend1 < 0 and condition true -> should compute addend0 - addend1 (which increases value) without touching error
void test_png_fp_sub_case_negative_branch_condition_true() {
    const char* testName = __func__;
    ++gTotalTests;

    int error = -999;
    png_int_32 addend0 = 5;
    png_int_32 addend1 = -1; // negative addend

    int ret = png_fp_sub(addend0, addend1, &error);

    // addend0 - addend1 = 5 - (-1) = 6
    bool ok = (ret == 6) && (error == -999);
    if (!ok) {
        report_failure(testName, "Expected return = addend0 - addend1 and error unchanged (negative addend path).");
    }
}

// Test 4: addend1 < 0 and condition false -> should hit overflow-like path for negative addend
// Use maximum int to make the inner comparison fail: 0x7fffffff + addend1 >= addend0 is false for INT_MAX with -1
void test_png_fp_sub_case_negative_branch_condition_false_overflow() {
    const char* testName = __func__;
    ++gTotalTests;

    int error = 0;
    png_int_32 addend0 = std::numeric_limits<png_int_32>::max(); // INT_MAX
    png_int_32 addend1 = -1;

    int ret = png_fp_sub(addend0, addend1, &error);

    const int expected_fp_half = 32768;

    if (!(ret == expected_fp_half && error == 1)) {
        report_failure(testName,
            "Expected return = PNG_FP_1/2 and error set to 1 (overflow path for negative addend).");
    }
}

// Test 5: addend1 == 0 -> should simply return addend0 and not modify error
void test_png_fp_sub_case_zero_addend1() {
    const char* testName = __func__;
    ++gTotalTests;

    int error = 12345;
    png_int_32 addend0 = 98765;
    png_int_32 addend1 = 0;

    int ret = png_fp_sub(addend0, addend1, &error);

    bool ok = (ret == addend0) && (error == 12345);
    if (!ok) {
        report_failure(testName, "Expected return = addend0 and error unchanged when addend1 == 0.");
    }
}

// Test 6: addend1 > 0 with large values to ensure normal subtraction without touching error
void test_png_fp_sub_case_large_positive_subtraction() {
    const char* testName = __func__;
    ++gTotalTests;

    int error = 0;
    png_int_32 addend0 = std::numeric_limits<png_int_32>::max(); // INT_MAX
    png_int_32 addend1 = 1;

    int ret = png_fp_sub(addend0, addend1, &error);

    bool ok = (ret == (addend0 - addend1)) && (error == 0);
    if (!ok) {
        report_failure(testName, "Expected normal subtraction result with no error modification for large positive subtraction.");
    }
}

// Main test runner
int main() {
    // Execute all test cases
    test_png_fp_sub_case_positive_branch_condition_true();
    test_png_fp_sub_case_positive_branch_condition_false_overflow();
    test_png_fp_sub_case_negative_branch_condition_true();
    test_png_fp_sub_case_negative_branch_condition_false_overflow();
    test_png_fp_sub_case_zero_addend1();
    test_png_fp_sub_case_large_positive_subtraction();

    // Summary
    std::cout << "png_fp_sub test summary: "
              << gTotalTests << " tests run, "
              << gFailedTests << " failures." << std::endl;

    if (gFailedTests != 0) {
        return 1;
    }
    return 0;
}
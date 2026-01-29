// Test suite for InkLimitingSampler in cmsvirt.c
// This test does not rely on GTest. It uses a small, self-contained test harness
// with non-terminating assertions (test results are collected and reported in main).

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>
#include <cstring>


// Minimal type aliases to align with the focal function's signature.
// These definitions mirror typical CMS LCMS typedefs used in cmsvirt.c.
typedef unsigned short cmsUInt16Number;
typedef double cmsFloat64Number;
typedef int cmsBool;

// Forward declaration of the focal function.
// It uses C linkage; ensure the test links against the actual implementation.
extern "C" int InkLimitingSampler(const cmsUInt16Number In[], cmsUInt16Number Out[], void* Cargo);

// Utility: compare two 4-element cmsUInt16Number arrays for exact equality.
static bool arrays_equal4(const cmsUInt16Number a[4], const cmsUInt16Number b[4]) {
    return (std::memcmp(a, b, 4 * sizeof(cmsUInt16Number)) == 0);
}

// Simple non-terminating assertion helper.
// Returns true if condition holds, false otherwise, without terminating the program.
static bool EXPECT_TRUE(bool condition, const char* test_desc) {
    if (condition) {
        std::cout << "[PASS] " << test_desc << std::endl;
        return true;
    } else {
        std::cout << "[FAIL] " << test_desc << std::endl;
        return false;
    }
}

// Test 1: When SumCMYK <= InkLimit (after scaling), or SumCMY == 0 -> Ratio == 1
// In this scenario, the output should be identical to the input for CMY and K unchanged.
static bool test_no_change_due_to_large_inklimit() {
    cmsUInt16Number In[4]  = {100, 200, 50, 0}; // C, M, Y, K
    cmsUInt16Number Out[4] = {0, 0, 0, 0};
    double cargo = 2.0; // InkLimit scaled: 2.0 * 655.35 = 1310.7
    int ret = InkLimitingSampler(In, Out, &cargo);
    cmsUInt16Number Exp[4] = {100, 200, 50, 0};
    bool ok = (ret != 0) && arrays_equal4(Out, Exp);
    (void)ret; // suppress unused warning if needed
    return EXPECT_TRUE(ok, "InkLimitingSampler should keep values when SumCMYK <= InkLimit (Ratio = 1)");
}

// Test 2: Ratio is positive and less than 1 (branch where SumCMYK > InkLimit && SumCMY > 0)
// Confirm that C/M/Y channels are scaled by Ratio and K is untouched.
static bool test_ratio_positive_branch() {
    cmsUInt16Number In[4]  = {100, 200, 50, 10}; // C, M, Y, K
    cmsUInt16Number Out[4] = {0, 0, 0, 0};
    double cargo = 0.5; // InkLimit scaled: 0.5 * 655.35 = 327.675
    int ret = InkLimitingSampler(In, Out, &cargo);
    // Expected Ratio ~ 1 - ((SumCMYK - InkLimit) / SumCMY)
    // SumCMY = 350, SumCMYK = 360, InkLimit = 327.675
    // Ratio = 1 - ((360 - 327.675) / 350) = 1 - (32.325 / 350) ≈ 0.907636
    // Expected outputs (rounded/truncated to cmsUInt16Number):
    // Out[0] = floor(100 * 0.907636) = 90
    // Out[1] = floor(200 * 0.907636) = 181
    // Out[2] = floor(50  * 0.907636) = 45
    // Out[3] = In[3] = 10
    cmsUInt16Number Exp[4] = {90, 181, 45, 10};
    bool ok = (ret != 0) && arrays_equal4(Out, Exp);
    (void)ret;
    return EXPECT_TRUE(ok, "InkLimitingSampler should apply Ratio < 1 to C/M/Y and keep K untouched (positive branch)");
}

// Test 3: Ratio negative gets clamped to 0 (ensures Ratio < 0 is corrected to 0)
// Ensures first three outputs are zero and K remains unchanged.
static bool test_ratio_negative_clamped_to_zero() {
    cmsUInt16Number In[4]  = {10, 10, 10, 400}; // C, M, Y, K
    cmsUInt16Number Out[4] = {0, 0, 0, 0};
    double cargo = 0.1; // InkLimit scaled: 65.535
    int ret = InkLimitingSampler(In, Out, &cargo);
    cmsUInt16Number Exp[4] = {0, 0, 0, 400};
    bool ok = (ret != 0) && arrays_equal4(Out, Exp);
    (void)ret;
    return EXPECT_TRUE(ok, "InkLimitingSampler should clamp negative Ratio to 0, zeroing C/M/Y and preserving K");
}

// Test 4: SumCMY equals zero (SumCMY == 0) leads to Ratio = 1 even if SumCMYK > InkLimit
// Verifies that with no CMY contribution, the first three channels remain unchanged.
static bool test_sumCMY_zero_behavior() {
    cmsUInt16Number In[4]  = {0, 0, 0, 123}; // C, M, Y, K
    cmsUInt16Number Out[4] = {0, 0, 0, 0};
    double cargo = 1.0; // InkLimit scaled: 655.35
    int ret = InkLimitingSampler(In, Out, &cargo);
    cmsUInt16Number Exp[4] = {0, 0, 0, 123};
    bool ok = (ret != 0) && arrays_equal4(Out, Exp);
    (void)ret;
    return EXPECT_TRUE(ok, "InkLimitingSampler should preserve CMY zeros and pass through K when SumCMY == 0");
}

// Entry point: run all tests and report summary
int main() {
    std::cout << "Running InkLimitingSampler unit tests (C++11, no GTest)" << std::endl;

    int tests_run = 0;
    int tests_pass = 0;

    bool r1 = test_no_change_due_to_large_inklimit();
    tests_run++;
    tests_pass += r1;

    bool r2 = test_ratio_positive_branch();
    tests_run++;
    tests_pass += r2;

    bool r3 = test_ratio_negative_clamped_to_zero();
    tests_run++;
    tests_pass += r3;

    bool r4 = test_sumCMY_zero_behavior();
    tests_run++;
    tests_pass += r4;

    std::cout << "Tests run: " << tests_run << ", Passed: " << tests_pass
              << ", Failed: " << (tests_run - tests_pass) << std::endl;

    // Return non-zero if any test failed
    return (tests_run == tests_pass) ? 0 : 1;
}
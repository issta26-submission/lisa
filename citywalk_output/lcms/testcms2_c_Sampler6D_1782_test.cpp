// C++11 test-suite for Sampler6D (testcms2.c focal method)
// Note: This test uses C linkage to call the focal C functions Fn8D1/Fn8D2/Fn8D3 and Sampler6D.
// It does not rely on Google Test; it provides a lightweight, non-terminating assertion mechanism.

#include <cstring>
#include <iostream>
#include <cstdint>
#include <testcms2.h>


// Import C dependencies with correct linkage
extern "C" {
}

// Lightweight test harness
static int g_tests_run = 0;
static int g_tests_failed = 0;

static void log_failure(const char* msg) {
    std::cerr << "TEST FAILURE: " << msg << std::endl;
}

// Non-terminating assertion macro for scalar values
#define EXPECT_EQ(actual, expected, msg) do { \
    ++g_tests_run; \
    if ((actual) != (expected)) { \
        ++g_tests_failed; \
        std::cerr << "  " << (msg) \
                  << " | Expected: " << (expected) \
                  << ", Actual: " << (actual) << std::endl; \
    } \
} while(0)

// Helper to run a single Sampler6D test case given 6 input values
static void run_one_case(const cmsUInt16Number in0,
                         const cmsUInt16Number in1,
                         const cmsUInt16Number in2,
                         const cmsUInt16Number in3,
                         const cmsUInt16Number in4,
                         const cmsUInt16Number in5)
{
    cmsUInt16Number In[6] = { in0, in1, in2, in3, in4, in5 };
    cmsUInt16Number Out[3] = {0, 0, 0};

    // Call the function under test
    cmsInt32Number ret = Sampler6D(In, Out, nullptr); // Cargo is unused in the focal method

    // Compute expected values using the independent Fn8D1/2/3 functions with the same inputs
    cmsUInt16Number E0 = Fn8D1(In[0], In[1], In[2], In[3], In[4], In[5], 0, 0, 6);
    cmsUInt16Number E1 = Fn8D2(In[0], In[1], In[2], In[3], In[4], In[5], 0, 0, 6);
    cmsUInt16Number E2 = Fn8D3(In[0], In[1], In[2], In[3], In[4], In[5], 0, 0, 6);

    // Verifications
    EXPECT_EQ(ret, 1, "Sampler6D return value should be 1");
    EXPECT_EQ(Out[0], E0, "Sampler6D Out[0] mismatch with Fn8D1");
    EXPECT_EQ(Out[1], E1, "Sampler6D Out[1] mismatch with Fn8D2");
    EXPECT_EQ(Out[2], E2, "Sampler6D Out[2] mismatch with Fn8D3");
}

// Public test cases

// 1) Basic correctness with typical positive inputs
static bool test_Sampler6D_basic_correctness() {
    std::cout << "Test 1: Basic correctness with typical inputs" << std::endl;
    // Example inputs
    run_one_case(10, 20, 30, 40, 50, 60);

    // Check results by verifying macro counters (non-terminating): ensure at least one test ran and none failed
    if (g_tests_run == 0) {
        log_failure("No assertions were executed in Test 1.");
        ++g_tests_failed;
        return false;
    }
    return g_tests_failed == 0;
}

// 2) Edge values: maximum 16-bit, zero, and mixed
static bool test_Sampler6D_edge_values() {
    std::cout << "Test 2: Edge values (max, zero, mixed)" << std::endl;
    // Edge case 1: max values
    run_one_case(0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF);
    // Edge case 2: zeros
    run_one_case(0, 0, 0, 0, 0, 0);
    // Edge case 3: mixed
    run_one_case(0x0000, 0xFFFF, 0x00FF, 0xAA55, 0x1234, 0x8000);

    return g_tests_failed == 0;
}

// 3) All zeros inputs to ensure stable behavior
static bool test_Sampler6D_all_zeros() {
    std::cout << "Test 3: All zeros inputs" << std::endl;
    run_one_case(0, 0, 0, 0, 0, 0);
    return g_tests_failed == 0;
}

// 4) Cargo parameter non-null should not affect outputs (Cargo is unused)
static bool test_Sampler6D_with_non_null_cargo() {
    std::cout << "Test 4: Non-null Cargo parameter (should be unused)" << std::endl;
    cmsUInt8Number dummy[8] = {0}; // small non-null cargo
    void* cargo = dummy;
    // Use a distinct input to verify consistency with Fn8D1/2/3
    cmsUInt16Number In[6] = { 1, 2, 3, 4, 5, 6 };
    cmsUInt16Number Out[3] = {0, 0, 0};

    cmsInt32Number ret = Sampler6D(In, Out, cargo);

    cmsUInt16Number E0 = Fn8D1(In[0], In[1], In[2], In[3], In[4], In[5], 0, 0, 6);
    cmsUInt16Number E1 = Fn8D2(In[0], In[1], In[2], In[3], In[4], In[5], 0, 0, 6);
    cmsUInt16Number E2 = Fn8D3(In[0], In[1], In[2], In[3], In[4], In[5], 0, 0, 6);

    EXPECT_EQ(ret, 1, "Sampler6D return value with Cargo should be 1");
    EXPECT_EQ(Out[0], E0, "Out[0] mismatch with Fn8D1 when Cargo is non-null");
    EXPECT_EQ(Out[1], E1, "Out[1] mismatch with Fn8D2 when Cargo is non-null");
    EXPECT_EQ(Out[2], E2, "Out[2] mismatch with Fn8D3 when Cargo is non-null");

    return g_tests_failed == 0;
}

// 5) Randomized inputs to exercise multiple code paths
static bool test_Sampler6D_random_inputs() {
    std::cout << "Test 5: Random inputs" << std::endl;
    // A small set of random-looking inputs (deterministic for test stability)
    run_one_case(7, 13, 29, 37, 51, 89);
    run_one_case(123, 456, 789, 1011, 2023, 4095);
    return g_tests_failed == 0;
}

// Entry point
int main(void) {
    // Reset test counters
    g_tests_run = 0;
    g_tests_failed = 0;

    bool ok = true;
    ok &= test_Sampler6D_basic_correctness();
    ok &= test_Sampler6D_edge_values();
    ok &= test_Sampler6D_all_zeros();
    ok &= test_Sampler6D_with_non_null_cargo();
    ok &= test_Sampler6D_random_inputs();

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << std::endl;

    if (g_tests_failed == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << "Some tests failed." << std::endl;
        return 1;
    }
}
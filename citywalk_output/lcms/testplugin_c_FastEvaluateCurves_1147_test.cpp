// Test suite for FastEvaluateCurves in testplugin.c
// C++11, no GTest. A lightweight test harness is provided here.
// The test harness calls the focal function directly and verifies behavior.
//
// Assumptions:
// - The project provides testcms2.h that defines cmsUInt16Number, CMSREGISTER, etc.
// - FastEvaluateCurves is defined in a C source file and linked with this test.
// - We only exercise the observable behavior: Out[0] is set to In[0], Data is unused.

#include <iostream>
#include <cstdint>
#include <testcms2.h>


// Forward declaration of the focal function from C source.
// We declare it with C linkage to avoid C++ name mangling issues.
extern "C" void FastEvaluateCurves(CMSREGISTER const cmsUInt16Number In[],
                                   CMSREGISTER cmsUInt16Number Out[],
                                   CMSREGISTER const void* Data);

static int g_total_tests = 0;
static int g_failed_tests = 0;

// Simple non-terminating assertion-like helper: logs and continues.
static void log_result(const char* test_name, bool passed) {
    ++g_total_tests;
    if (passed) {
        std::cout << "[PASS] " << test_name << std::endl;
    } else {
        ++g_failed_tests;
        std::cout << "[FAIL] " << test_name << std::endl;
    }
}

// Test 1: Basic single-element copy - ensure Out[0] == In[0] when Data is null.
// This verifies the core functionality of the focal method.
static bool test_basic_assignment_null_data() {
    cmsUInt16Number In[1]  = { 12345 };
    cmsUInt16Number Out[1] = { 0 };
    FastEvaluateCurves(In, Out, nullptr);
    return Out[0] == In[0];
}

// Test 2: Non-null Data pointer should have no effect on the result.
// This ensures that the function does not depend on Data, aligning with its implementation.
// The Out value should still equal In[0].
static bool test_data_pointer_no_effect() {
    cmsUInt16Number In[1]  = { 7 };
    cmsUInt16Number Out[1] = { 0x1234 };
    int dummy = 99;
    FastEvaluateCurves(In, Out, &dummy);
    return Out[0] == In[0];
}

// Test 3: Boundary value handling for 16-bit unsigned input.
// Verifies the function handles maximum 16-bit unsigned value correctly.
static bool test_max_uint16_input() {
    cmsUInt16Number In[1]  = { 0xFFFF }; // 65535
    cmsUInt16Number Out[1] = { 0 };
    FastEvaluateCurves(In, Out, nullptr);
    return Out[0] == In[0];
}

// Test 4: Ensure the function only writes to Out[0] and does not mutate other Out elements.
// We provide a 2-element output array; only Out[0] should be overwritten.
static bool test_out_array_is_written_only_at_zero() {
    cmsUInt16Number In[2]  = { 11, 999 };
    cmsUInt16Number Out[2] = { 0xAAAA, 0xBBBB }; // pre-fill to detect unintended writes
    FastEvaluateCurves(In, Out, nullptr);
    bool firstOverwritten = (Out[0] == In[0]); // should copy first element
    bool secondUnchanged  = (Out[1] == 0xBBBB); // should remain unchanged
    return firstOverwritten && secondUnchanged;
}

// Test 5: In array with a non-zero first element and a distinct second value.
// Confirms deterministic behavior for a second input value without affecting Out[0].
static bool test_in_second_value_does_not_affect_out0() {
    cmsUInt16Number In[2]  = { 256, 512 };
    cmsUInt16Number Out[2] = { 1, 2 };
    FastEvaluateCurves(In, Out, nullptr);
    return (Out[0] == In[0]) && (Out[1] == 2); // Out[0] should be 256; Out[1] unchanged
}

// Simple test runner
int main() {
    // Run tests with explanatory comments above each test function.
    bool r1 = test_basic_assignment_null_data();
    log_result("test_basic_assignment_null_data", r1);

    bool r2 = test_data_pointer_no_effect();
    log_result("test_data_pointer_no_effect", r2);

    bool r3 = test_max_uint16_input();
    log_result("test_max_uint16_input", r3);

    bool r4 = test_out_array_is_written_only_at_zero();
    log_result("test_out_array_is_written_only_at_zero", r4);

    bool r5 = test_in_second_value_does_not_affect_out0();
    log_result("test_in_second_value_does_not_affect_out0", r5);

    // Summary
    std::cout << "Total tests: " << g_total_tests << ", Passed: " 
              << (g_total_tests - g_failed_tests) << ", Failed: " 
              << g_failed_tests << std::endl;

    return (g_failed_tests == 0) ? 0 : 1;
}
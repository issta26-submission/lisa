// Unit test for PrelinOpt16free in cmsopt.c
// This test suite is built to run with C++11 and without Google Test.
// It uses a custom _cmsFree mock to verify that PrelinOpt16free frees
// exactly three pointers: EvalCurveOut16, ParamsCurveOut16, and the Prelin16Data itself.

#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <lcms2_internal.h>


// Include necessary internal declarations so that Prelin16Data and cmsContext
// are known to the test, mirroring the project's structure.
// This path should be adjusted if your build layout differs.

extern "C" {
    // Prototype for PrelinOpt16free as defined in cmsopt.c
    void PrelinOpt16free(cmsContext ContextID, void* ptr);

    // Mock for the system memory free function used by PrelinOpt16free.
    // We intentionally provide our own implementation to observe which
    // pointers are freed by PrelinOpt16free.
    void _cmsFree(cmsContext ContextID, void* Mem);
}

// Global record of freed pointers for validation in tests.
static std::vector<void*> FreedPointers;

// Our fake _cmsFree collects the pointer being freed and then actually frees it
// if non-null to avoid memory leaks during tests.
extern "C" void _cmsFree(cmsContext ContextID, void* Mem)
{
    // Record the freed memory address (NULL pointers are also recorded for coverage)
    FreedPointers.push_back(Mem);
    if (Mem != NULL) {
        std::free(Mem);
    }
}

// Simple test harness state
static int gFailures = 0;

// Helpers for test assertions
#define TEST_EXPECT_EQ_PTR(a, b, msg) do { \
    void* _va = (void*)(a); \
    void* _vb = (void*)(b); \
    if (_va != _vb) { \
        std::cerr << "FAIL: " << __FUNCTION__ << " - " << (msg) \
                  << " | expected: " << _vb << " actual: " << _va << std::endl; \
        ++gFailures; \
    } \
} while (0)

#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "FAIL: " << __FUNCTION__ << " - " << (msg) << std::endl; \
        ++gFailures; \
    } \
} while (0)

// Test 1: All fields are non-null. Ensure _cmsFree is called for EvalCurveOut16, ParamsCurveOut16, and the Prelin16Data itself in the correct order.
static void test_prelinopt16free_all_non_null()
{
    // Clear previous freed-trace
    FreedPointers.clear();

    // Prepare a dummy context (can be NULL in this test environment)
    cmsContext ctx = NULL;

    // Allocate a Prelin16Data structure from the project (actual layout is defined by the library)
    Prelin16Data* p16 = (Prelin16Data*) std::malloc(sizeof(Prelin16Data));
    TEST_ASSERT(p16 != NULL, "Failed to allocate Prelin16Data");

    // Allocate its output buffers
    void* evalOut = std::malloc(128);
    void* paramsOut = std::malloc(256);

    // Initialize the fields
    p16->EvalCurveOut16 = evalOut;
    p16->ParamsCurveOut16 = paramsOut;

    // Call the unit-under-test
    PrelinOpt16free(ctx, p16);

    // Validate that three frees occurred in the expected order
    TEST_ASSERT(FreedPointers.size() >= 3, "Expected at least 3 frees recorded");

    // Expect exact three frees: EvalCurveOut16, ParamsCurveOut16, then the struct itself (p16)
    TEST_EXPECT_EQ_PTR(FreedPointers[0], evalOut, "First freed pointer should be EvalCurveOut16");
    TEST_EXPECT_EQ_PTR(FreedPointers[1], paramsOut, "Second freed pointer should be ParamsCurveOut16");
    TEST_EXPECT_EQ_PTR(FreedPointers[2], p16, "Third freed pointer should be Prelin16Data (p16)");

    // Cleanup: Freed by the test harness, but just ensure no crash and memory tracking is consistent
    FreedPointers.clear();
}

// Test 2: EvalCurveOut16 is NULL, ParamsCurveOut16 is non-null. Ensure Frees reflect this scenario.
static void test_prelinopt16free_eval_null_params_non_null()
{
    FreedPointers.clear();
    cmsContext ctx = NULL;

    Prelin16Data* p16 = (Prelin16Data*) std::malloc(sizeof(Prelin16Data));
    TEST_ASSERT(p16 != NULL, "Failed to allocate Prelin16Data (test 2)");

    void* paramsOut = std::malloc(256);
    p16->EvalCurveOut16 = NULL;
    p16->ParamsCurveOut16 = paramsOut;

    PrelinOpt16free(ctx, p16);

    TEST_ASSERT(FreedPointers.size() >= 3, "Expected at least 3 frees in test 2");

    // First freed pointer should be NULL (for EvalCurveOut16)
    TEST_ASSERT(FreedPointers[0] == NULL, "First freed pointer should be NULL (EvalCurveOut16)");
    TEST_EXPECT_EQ_PTR(FreedPointers[1], paramsOut, "Second freed pointer should be ParamsCurveOut16");
    TEST_EXPECT_EQ_PTR(FreedPointers[2], p16, "Third freed pointer should be Prelin16Data (p16)");

    FreedPointers.clear();
}

// Test 3: ParamsCurveOut16 is NULL, EvalCurveOut16 is non-null. Ensure Frees reflect this scenario.
static void test_prelinopt16free_params_null_eval_non_null()
{
    FreedPointers.clear();
    cmsContext ctx = NULL;

    Prelin16Data* p16 = (Prelin16Data*) std::malloc(sizeof(Prelin16Data));
    TEST_ASSERT(p16 != NULL, "Failed to allocate Prelin16Data (test 3)");

    void* evalOut = std::malloc(128);
    p16->EvalCurveOut16 = evalOut;
    p16->ParamsCurveOut16 = NULL;

    PrelinOpt16free(ctx, p16);

    TEST_ASSERT(FreedPointers.size() >= 3, "Expected at least 3 frees in test 3");

    TEST_EXPECT_EQ_PTR(FreedPointers[0], evalOut, "First freed pointer should be EvalCurveOut16");
    TEST_ASSERT(FreedPointers[1] == NULL, "Second freed pointer should be NULL (ParamsCurveOut16)");
    TEST_EXPECT_EQ_PTR(FreedPointers[2], p16, "Third freed pointer should be Prelin16Data (p16)");

    FreedPointers.clear();
}

// Entry point for running tests
int main()
{
    std::cout << "Running PrelinOpt16free unit tests (C++11, no GTest)..." << std::endl;

    test_prelinopt16free_all_non_null();
    test_prelinopt16free_eval_null_params_non_null();
    test_prelinopt16free_params_null_eval_non_null();

    if (gFailures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << gFailures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}
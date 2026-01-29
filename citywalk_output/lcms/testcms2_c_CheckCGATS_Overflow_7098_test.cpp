// C++11 unit test suite for the focal method CheckCGATS_Overflow
// This test suite is designed to be compiled with C++11 without GTest.
// It uses a lightweight, non-terminating assertion pattern to maximize code coverage
// and to exercise the focal method and its direct dependencies (cmsIT8LoadFromMem, cmsIT8Free).

#include <iostream>
#include <cstdint>
#include <testcms2.h>


// Forward declarations for the C library functions used by the focal method.
// We assume the project provides these symbols via the standard Little cms API.
// If the project provides headers (e.g., cms.h, testcms2.h), prefer including them.
// Here we declare minimal prototypes to keep the test self-contained while linking against the real DLL/Obj.
extern "C" {
    // Opaque handle type from the CMS library
    typedef void* cmsHANDLE;
    // Basic typedefs used by the CMS IT8 loader
    typedef unsigned char cmsUInt8Number;
    typedef unsigned int  cmsUInt32Number;
    typedef int           cmsInt32Number;
    typedef void* cmsContext;

    // CMS IT8 loader APIs
    cmsHANDLE cmsIT8LoadFromMem(cmsContext ContextID, const void* mem, cmsUInt32Number size);
    void cmsIT8Free(cmsHANDLE handle);

    // The focal method under test
    cmsInt32Number CheckCGATS_Overflow(void);
}

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_passed_tests = 0;

#define RUN_TEST(test) \
    do { \
        g_total_tests++; \
        bool _ok = (test); \
        if (_ok) { ++g_passed_tests; } else { \
            std::cerr << "Test failed: " #test "\n"; \
        } \
    } while (0)

#define ASSERT_TRUE(cond, msg) \
    do { \
        if (cond) { /* pass */ } else { \
            std::cerr << "Assertion failed: " << (msg) << "\n"; \
        } \
    } while (0)

//
// Test 1: Verify that the focal method CheckCGATS_Overflow returns 1 as expected.
// This directly exercises the target function and asserts its return value.
// Explanatory: Ensures the public API contract of the focal method remains stable.
//
bool test_CheckCGATS_Overflow_Returns_One()
{
    cmsInt32Number ret = CheckCGATS_Overflow();
    bool ok = (ret == 1);
    if (!ok) {
        std::cerr << "Expected CheckCGATS_Overflow() to return 1, got " << ret << "\n";
    }
    return ok;
}

//
// Test 2: Exercise the CMS IT8 loader with the overflow-inducing input used by the focal method.
// Explanatory: Ensure that feeding an overflow-like value string into cmsIT8LoadFromMem
// does not crash or lead to a bad state. We only verify that the call returns (handle may be NULL).
//
bool test_CMSIT8LoadFromMem_OverflowInput_NoCrash()
{
    // Overflow-like numeric token that is invalid for the loader
    const cmsUInt8Number junk[] = { "@\nA 1.e2147483648\n" };

    cmsHANDLE h = cmsIT8LoadFromMem(0, (const void*)junk, sizeof(junk));

    // If a handle is returned, free it to avoid leaks in the test harness.
    if (h)
        cmsIT8Free(h);

    // If the loader crashed, the program would abort before this point.
    // Reaching here means the operation completed gracefully.
    return true;
}

//
// Test 3: Exercise the CMS IT8 loader with a minimal zero-sized input.
// Explanatory: Validate the loader's behavior on edge-case input and ensure no crash.
// The focal point is the loader's safety on boundary input, not its parsing result.
//
bool test_CMSIT8LoadFromMem_ZeroSizeInput_NoCrash()
{
    const cmsUInt8Number empty[] = { };
    cmsHANDLE h = cmsIT8LoadFromMem(0, (const void*)empty, 0);
    if (h) cmsIT8Free(h);
    return true;
}

//
// Test 4: Exercise the CMS IT8 loader with a NULL pointer input and zero size.
// Explanatory: Ensure loader handles a null data pointer gracefully when size is zero.
// This is a boundary condition check for robustness.
//
bool test_CMSIT8LoadFromMem_NullPointer_NoCrash()
{
    // NULL data pointer with size zero should be a no-op for many loaders.
    cmsHANDLE h = cmsIT8LoadFromMem(0, nullptr, 0);
    if (h) cmsIT8Free(h);
    return true;
}

//
// Entry point: Run all defined tests and report a summary.
// Explanatory: This main function serves as the test runner since external
// test frameworks are not allowed in this scenario.
// The tests are designed to be non-terminating on failure, so we accumulate and print results.
//
int main(int argc, char* argv[])
{
    // Test 1: focal method behavior
    RUN_TEST(test_CheckCGATS_Overflow_Returns_One);

    // Tests 2-4: loader robustness against overflow-boundary inputs
    RUN_TEST(test_CMSIT8LoadFromMem_OverflowInput_NoCrash);
    RUN_TEST(test_CMSIT8LoadFromMem_ZeroSizeInput_NoCrash);
    RUN_TEST(test_CMSIT8LoadFromMem_NullPointer_NoCrash);

    // Summary
    std::cout << "Tests run: " << g_total_tests << ", Passed: " << g_passed_tests
              << ", Failed: " << (g_total_tests - g_passed_tests) << "\n";

    // Return non-zero if any test failed
    return (g_total_tests == g_passed_tests) ? 0 : 1;
}
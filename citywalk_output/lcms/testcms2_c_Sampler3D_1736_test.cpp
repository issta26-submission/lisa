// C++11 compliant test suite for the focal method Sampler3D from testcms2.c
// This file does not rely on GTest. It provides a lightweight, non-terminating
// assertion framework suitable for maximizing coverage.
// Explanatory comments are added to each unit test to describe intent.

// Note: This test suite directly links against the C functions Sampler3D and Fn8D1/Fn8D2/Fn8D3
// that are defined in testcms2.c. We declare them with C linkage to avoid name mangling.

#include <iostream>
#include <cstdint>
#include <testcms2.h>


// Lightweight type aliases (mirroring the CMS types used in testcms2.c)
typedef unsigned short cmsUInt16Number;
typedef unsigned int   cmsUInt32Number;
typedef int            cmsInt32Number;

// Ensure the CMSREGISTER macro resolves to nothing in C++
#ifndef CMSREGISTER
#define CMSREGISTER
#endif

// Prototypes for the focal method and its dependencies (C linkage)
extern "C" {
    cmsInt32Number Sampler3D(CMSREGISTER const cmsUInt16Number In[],
                           CMSREGISTER cmsUInt16Number Out[],
                           CMSREGISTER void * Cargo);

    cmsUInt16Number Fn8D1(cmsUInt16Number a1, cmsUInt16Number a2, cmsUInt16Number a3, cmsUInt16Number a4,
                          cmsUInt16Number a5, cmsUInt16Number a6, cmsUInt16Number a7, cmsUInt16Number a8,
                          cmsUInt32Number m);

    cmsUInt16Number Fn8D2(cmsUInt16Number a1, cmsUInt16Number a2, cmsUInt16Number a3, cmsUInt16Number a4,
                          cmsUInt16Number a5, cmsUInt16Number a6, cmsUInt16Number a7, cmsUInt16Number a8,
                          cmsUInt32Number m);

    cmsUInt16Number Fn8D3(cmsUInt16Number a1, cmsUInt16Number a2, cmsUInt16Number a3, cmsUInt16Number a4,
                          cmsUInt16Number a5, cmsUInt16Number a6, cmsUInt16Number a7, cmsUInt16Number a8,
                          cmsUInt32Number m);
}

// Global counter for test failures (non-terminating assertion style)
static int g_failures = 0;

// Simple helper to report equality results without terminating test execution
static void check_uint16_equal(const char* testName,
                               cmsUInt16Number expected,
                               cmsUInt16Number actual)
{
    if (expected == actual) {
        std::cout << "[PASS] " << testName << std::endl;
    } else {
        ++g_failures;
        std::cout << "[FAIL] " << testName
                  << " | expected: " << expected
                  << " got: " << actual << std::endl;
    }
}

// Simple helper to report integer equality results (for return codes)
static void check_int_equal(const char* testName,
                            cmsInt32Number expected,
                            cmsInt32Number actual)
{
    if (expected == actual) {
        std::cout << "[PASS] " << testName << std::endl;
    } else {
        ++g_failures;
        std::cout << "[FAIL] " << testName
                  << " | expected: " << expected
                  << " got: " << actual << std::endl;
    }
}

// Test 1: Validate Sampler3D outputs exactly what Fn8D1/Fn8D2/Fn8D3 produce for zero inputs.
// Intent: Ensure the wrapper correctly forwards In[] values to Fn8D1/2/3 and writes to Out[].
static void test_Sampler3D_ZeroInputs_OutputsMatchFnOutputs()
{
    // Arrange
    cmsUInt16Number In[3]  = {0, 0, 0};
    cmsUInt16Number Out[3] = {0, 0, 0};
    void* Cargo = nullptr;

    // Expected values computed by calling the underlying Fn8* directly with same arguments
    cmsUInt16Number expected0 = Fn8D1(In[0], In[1], In[2], 0, 0, 0, 0, 0, 3);
    cmsUInt16Number expected1 = Fn8D2(In[0], In[1], In[2], 0, 0, 0, 0, 0, 3);
    cmsUInt16Number expected2 = Fn8D3(In[0], In[1], In[2], 0, 0, 0, 0, 0, 3);

    // Act
    cmsInt32Number rc = Sampler3D(In, Out, Cargo);

    // Assert
    check_int_equal("Sampler3D return code (ZeroInputs)", 1, rc);
    check_uint16_equal("Sampler3D Out[0] equals Fn8D1(In0..In2, m=3)", expected0, Out[0]);
    check_uint16_equal("Sampler3D Out[1] equals Fn8D2(In0..In2, m=3)", expected1, Out[1]);
    check_uint16_equal("Sampler3D Out[2] equals Fn8D3(In0..In2, m=3)", expected2, Out[2]);
}

// Test 2: Validate Sampler3D with non-zero inputs matches Fn8* outputs accordingly.
// Intent: Ensure correctness across typical non-zero data and exercise non-trivial paths.
static void test_Sampler3D_NonZeroInputs_OutputsMatchFnOutputs()
{
    // Arrange
    cmsUInt16Number In[3]  = {123, 456, 789};
    cmsUInt16Number Out[3] = {0, 0, 0};
    void* Cargo = nullptr;

    cmsUInt16Number expected0 = Fn8D1(In[0], In[1], In[2], 0, 0, 0, 0, 0, 3);
    cmsUInt16Number expected1 = Fn8D2(In[0], In[1], In[2], 0, 0, 0, 0, 0, 3);
    cmsUInt16Number expected2 = Fn8D3(In[0], In[1], In[2], 0, 0, 0, 0, 0, 3);

    // Act
    cmsInt32Number rc = Sampler3D(In, Out, Cargo);

    // Assert
    check_int_equal("Sampler3D return code (NonZeroInputs)", 1, rc);
    check_uint16_equal("Sampler3D Out[0] equals Fn8D1(In=123,456,789)", expected0, Out[0]);
    check_uint16_equal("Sampler3D Out[1] equals Fn8D2(In=123,456,789)", expected1, Out[1]);
    check_uint16_equal("Sampler3D Out[2] equals Fn8D3(In=123,456,789)", expected2, Out[2]);
}

// Test 3: Cargo parameter should be unused by Sampler3D; verify outputs are independent of Cargo value.
// Intent: Confirm static analysis of unused parameter is not affecting behavior or results.
static void test_Sampler3D_Cargo_Independence()
{
    // Arrange
    cmsUInt16Number In[3]  = {321, 654, 987};
    cmsUInt16Number Out1[3] = {0, 0, 0};
    cmsUInt16Number Out2[3] = {0, 0, 0};
    cmsUInt16Number OutExpected[3] = {0, 0, 0};
    void* CargoA = reinterpret_cast<void*>(0xDEADBEEF);
    void* CargoB = reinterpret_cast<void*>(0xFEEDC0DE);

    // Expected values computed using Fn8D* with the same In
    OutExpected[0] = Fn8D1(In[0], In[1], In[2], 0, 0, 0, 0, 0, 3);
    OutExpected[1] = Fn8D2(In[0], In[1], In[2], 0, 0, 0, 0, 0, 3);
    OutExpected[2] = Fn8D3(In[0], In[1], In[2], 0, 0, 0, 0, 0, 3);

    // Act: first with CargoA
    cmsInt32Number rc1 = Sampler3D(In, Out1, CargoA);

    // Act: then with CargoB
    cmsInt32Number rc2 = Sampler3D(In, Out2, CargoB);

    // Assert: return codes should be 1 in both cases
    check_int_equal("Sampler3D CargoIndependence rc1", 1, rc1);
    check_int_equal("Sampler3D CargoIndependence rc2", 1, rc2);

    // Assert: outputs must be identical regardless of Cargo
    check_uint16_equal("Sampler3D CargoIndependence Out0", OutExpected[0], Out1[0]);
    check_uint16_equal("Sampler3D CargoIndependence Out1", OutExpected[1], Out1[1]);
    check_uint16_equal("Sampler3D CargoIndependence Out2", OutExpected[2], Out1[2]);
    check_uint16_equal("Sampler3D CargoIndependence Out0 (second run)", OutExpected[0], Out2[0]);
    check_uint16_equal("Sampler3D CargoIndependence Out1 (second run)", OutExpected[1], Out2[1]);
    check_uint16_equal("Sampler3D CargoIndependence Out2 (second run)", OutExpected[2], Out2[2]);
}

// Entry point to run all tests
int main(int argc, char* argv[])
{
    (void)argc; (void)argv; // suppress unused parameter warnings if not used

    std::cout << "Running Sampler3D unit tests (no GTest framework)..." << std::endl;

    test_Sampler3D_ZeroInputs_OutputsMatchFnOutputs();
    test_Sampler3D_NonZeroInputs_OutputsMatchFnOutputs();
    test_Sampler3D_Cargo_Independence();

    if (g_failures == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cout << g_failures << " test(s) FAILED." << std::endl;
        return 1;
    }
}
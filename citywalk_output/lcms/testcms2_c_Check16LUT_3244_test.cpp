// Unit test suite for the focal method Check16LUT (as defined in testcms2.c)
// This test suite is designed to be compiled with a C++11 compiler without GTest.
// It provides a minimal mock of the cmsPipeline and its Eval16 mechanism to drive
// Check16LUT through both an identity LUT (expected to return true) and a
// non-identity LUT (expected to return false).

#include <iostream>
#include <cstdint>
#include <testcms2.h>


// Domain/types alignment with the focal code
using cmsInt32Number = int;
using cmsBool      = int;
using cmsUInt16Number = uint16_t;

// Forward declaration of the focal function.
// The real project provides this in testcms2.c; we declare it here to link against
// the actual implementation when building the test suite alongside the source.
extern "C" cmsInt32Number Check16LUT(struct cmsPipeline* lut);

// Minimal mock of the cmsPipeline and the Eval16 callback mechanism.
// This is sufficient to drive the focal function Check16LUT without requiring the
// full upstream library. It mirrors the essential interface used by the focal code.

typedef void (*CmsEval16Fn)(const cmsUInt16Number In[3], cmsUInt16Number Out[3], void* Cargo);

struct cmsPipeline {
    CmsEval16Fn Eval16;
    void* Cargo;
};

// Lightweight wrapper that mimics the upstream behavior.
// If a LUT is provided, invoke its Eval16 callback; otherwise pass-through.
extern "C" void cmsPipelineEval16(const cmsUInt16Number In[3], cmsUInt16Number Out[3], const cmsPipeline* lut)
{
    if (lut && lut->Eval16) {
        lut->Eval16(In, Out, lut->Cargo);
    } else {
        // Pass-through in absence of a LUT
        Out[0] = In[0];
        Out[1] = In[1];
        Out[2] = In[2];
    }
}

// Identity mapping: outputs equal inputs
static void IdentityEval16(const cmsUInt16Number In[3], cmsUInt16Number Out[3], void* /*Cargo*/)
{
    Out[0] = In[0];
    Out[1] = In[1];
    Out[2] = In[2];
}

// Non-identity mapping: deliberately corrupts the output
static void InvertEval16(const cmsUInt16Number In[3], cmsUInt16Number Out[3], void* /*Cargo*/)
{
    Out[0] = static_cast<cmsUInt16Number>(~In[0]);
    Out[1] = static_cast<cmsUInt16Number>(~In[1]);
    Out[2] = static_cast<cmsUInt16Number>(~In[2]);
}

// Simple, non-terminating assertion helper to allow multiple tests to run.
static int g_failures = 0;
static int g_tests_run = 0;

#define ASSERT(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "Assertion failed: " msg << std::endl; \
            ++g_failures; \
        } else { \
            // Optional success message can be verbose; kept quiet to reduce noise. \
        } \
        ++g_tests_run; \
    } while(0)

// Test 1: Identity LUT should yield true
static bool test_Check16LUT_Identity(void)
{
    cmsPipeline lut;
    lut.Eval16 = IdentityEval16;
    lut.Cargo  = nullptr;

    cmsInt32Number result = Check16LUT(&lut);
    ASSERT(result == 1, "Check16LUT with identity LUT should return true (1).");
    return (result == 1);
}

// Test 2: Non-identity LUT should yield false
static bool test_Check16LUT_NonIdentity(void)
{
    cmsPipeline lut;
    lut.Eval16 = InvertEval16;
    lut.Cargo  = nullptr;

    cmsInt32Number result = Check16LUT(&lut);
    ASSERT(result == 0, "Check16LUT with non-identity LUT should return false (0).");
    return (result == 0);
}

// Driver: run the two tests and report summary
int main(void)
{
    std::cout << "Starting Check16LUT unit tests (no GTest)." << std::endl;

    bool t1 = test_Check16LUT_Identity();
    bool t2 = test_Check16LUT_NonIdentity();

    if (t1) std::cout << "[PASS] Identity LUT test" << std::endl;
    else     std::cout << "[FAIL] Identity LUT test" << std::endl;

    if (t2) std::cout << "[PASS] Non-Identity LUT test" << std::endl;
    else     std::cout << "[FAIL] Non-Identity LUT test" << std::endl;

    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_failures << std::endl;

    return (g_failures == 0) ? 0 : 1;
}
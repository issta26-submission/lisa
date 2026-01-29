// Minimal C++11 test harness for the focal function CheckOne7D.
// This test suite does not use GTest. It relies on a small, self-contained
// mock layer for the dependent C functions (cmsPipelineEval16, Sampler7D,
// IsGoodWordPrec) so that we can exercise the control flow of CheckOne7D
// (true/false branches) without requiring the full CMS/LittleCMS runtime.
//
// How this works:
// - We declare the external function CheckOne7D (as it would be linked from testcms2.c).
// - We provide C-compatible mocks for cmsPipelineEval16, Sampler7D, IsGoodWordPrec.
// - We provide a simple switchable mock mode SetMockMode(int) to drive different
//   behaviors of the mocks so we can trigger different branches inside CheckOne7D.
// - We run a few test cases that cover passing and failing scenarios, including
//   boundary inputs.
// - We accumulate and print a concise summary of failures (non-terminating assertions).

#include <iostream>
#include <testcms2.h>
#include <cstdlib>


// Basic type aliases to align with the focal function's expectations.
typedef int cmsBool;
typedef unsigned short cmsUInt16Number;
typedef void cmsPipeline;

// Boolean constants (keep compatibility with typical CMS code).
#define TRUE 1
#define FALSE 0

extern "C" {
    // Prototype of the focal method under test.
    cmsBool CheckOne7D(cmsPipeline* lut,
                      cmsUInt16Number a1, cmsUInt16Number a2,
                      cmsUInt16Number a3, cmsUInt16Number a4,
                      cmsUInt16Number a5, cmsUInt16Number a6,
                      cmsUInt16Number a7);

    // Mocks for the dependencies used inside CheckOne7D.
    void cmsPipelineEval16(const cmsUInt16Number In[7], cmsUInt16Number Out[3], const cmsPipeline* lut);
    void Sampler7D(const cmsUInt16Number In[7], cmsUInt16Number Out[3], void* Cargo);
    cmsBool IsGoodWordPrec(const char* Title, cmsUInt16Number in, cmsUInt16Number out, cmsUInt16Number maxErr);

    // Test harness hook to switch mock behavior.
    void SetMockMode(int mode);
}

//
// Mocks implementation
//
// We provide a simple mode-driven behavior to exercise different branches inside
// CheckOne7D. The real check compares Out1 (lut) with Out2 (Sampler7D) per channel.
// We emulate:
 // mode 0: Out1 = In (channel-wise equal to Out2) -> CheckOne7D returns TRUE
 // mode 1: Out1[0] differs by +3 from In[0] (Channel 1 mismatch) -> FALSE
 // mode 2: Out1[2] differs by +3 from In[2] (Channel 3 mismatch) -> FALSE
static int g_mockMode = 0;

extern "C" void SetMockMode(int mode) {
    g_mockMode = mode;
}

// cmsPipelineEval16: fills Out with a deterministic relation to In based on mode.
extern "C" void cmsPipelineEval16(const cmsUInt16Number In[7], cmsUInt16Number Out[3], const cmsPipeline* lut) {
    (void)lut; // unused in mock
    switch (g_mockMode) {
        case 0:
            // Pass-through: channel values identical to input for all three channels.
            Out[0] = In[0];
            Out[1] = In[1];
            Out[2] = In[2];
            break;
        case 1:
            // Introduce a deliberate mismatch on Channel 1 to trigger a failure.
            Out[0] = In[0] + 3; // diff = 3 > maxErr (2)
            Out[1] = In[1];
            Out[2] = In[2];
            break;
        case 2:
            // Introduce a deliberate mismatch on Channel 3 to trigger a failure.
            Out[0] = In[0];
            Out[1] = In[1];
            Out[2] = In[2] + 3; // diff = 3 > maxErr (2)
            break;
        default:
            // Safe default: identical to input
            Out[0] = In[0];
            Out[1] = In[1];
            Out[2] = In[2];
            break;
    }
}

// Sampler7D: returns a "real value" based on In. For simplicity, mirror In here.
extern "C" void Sampler7D(const cmsUInt16Number In[7], cmsUInt16Number Out[3], void* Cargo) {
    (void)Cargo;
    // Real implementation would compute a 7D sampler. Our test uses the first 3 inputs.
    Out[0] = In[0];
    Out[1] = In[1];
    Out[2] = In[2];
}

// IsGoodWordPrec: compare with a small tolerance. We support per-channel checks via the maxErr.
extern "C" cmsBool IsGoodWordPrec(const char* Title, cmsUInt16Number in, cmsUInt16Number out, cmsUInt16Number maxErr) {
    (void)Title;
    cmsUInt16Number diff = (in > out) ? (in - out) : (out - in);
    return (diff <= maxErr) ? TRUE : FALSE;
}

// Lightweight test harness (non-terminating assertions)
static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        ++g_failures; \
        std::cerr << "Failure: " << (msg) << " [in " __FILE__ ":" << __LINE__ << "]" << std::endl; \
    } \
} while(0)


// Simple wrapper around the focal function for readability in tests.
static void run_CheckOne7D_test_case(const char* name,
                                   cmsUInt16Number a1, cmsUInt16Number a2, cmsUInt16Number a3,
                                   cmsUInt16Number a4, cmsUInt16Number a5, cmsUInt16Number a6,
                                   cmsUInt16Number a7, int expected)
{
    cmsBool res = CheckOne7D(nullptr, a1, a2, a3, a4, a5, a6, a7);
    EXPECT_TRUE((res == (expected ? TRUE : FALSE)), name);
}


// Main: run a suite of tests designed to cover true/false branches of CheckOne7D.
int main() {
    // Test 1: Mode 0 (pass case). In values chosen to be identical in lut and sampler paths.
    SetMockMode(0);
    {
        const char* testname = "CheckOne7D_Pass_Mode0_AllIdenticalInputs";
        run_CheckOne7D_test_case(testname, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1);
        // Expected: TRUE
    }

    // Test 2: Mode 1 (fail on Channel 1). Out1[0] differs by 3 from In[0], causing Channel 1 to fail.
    SetMockMode(1);
    {
        const char* testname = "CheckOne7D_Fail_Mode1_Channel1";
        run_CheckOne7D_test_case(testname, 1000, 2000, 1000, 1000, 1000, 1000, 1000, 0);
        // Expected: FALSE
    }

    // Test 3: Mode 2 (fail on Channel 3). Out1[2] differs by 3 from In[2], causing Channel 3 to fail.
    SetMockMode(2);
    {
        const char* testname = "CheckOne7D_Fail_Mode2_Channel3";
        run_CheckOne7D_test_case(testname, 1000, 1000, 1500, 1000, 1000, 1000, 1000, 0);
        // Expected: FALSE
    }

    // Test 4: Boundary values with mode 0 (should pass since all paths mirror input).
    SetMockMode(0);
    {
        const char* testname = "CheckOne7D_Pass_Mode0_BoundaryValues";
        run_CheckOne7D_test_case(testname, 65535, 65534, 65533, 0, 0, 0, 0, 1);
        // Expected: TRUE
    }

    // Test 5: Randomized sanity checks (mode 0) to exercise more inputs without changing logic.
    SetMockMode(0);
    {
        const char* testname = "CheckOne7D_Pass_Mode0_Randomish";
        run_CheckOne7D_test_case(testname, 10, 500, 300, 700, 123, 999, 45, 1);
        // Expected: TRUE
    }

    // Summary
    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}
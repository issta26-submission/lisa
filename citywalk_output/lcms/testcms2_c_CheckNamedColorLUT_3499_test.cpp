/*
Step 0 (Contextual Prelude)
This file implements a small, self-contained C++11 test harness for the focal function:
    cmsInt32Number CheckNamedColorLUT(void)
from the provided code base.
The test harness does not rely on GTest. Instead, it provides lightweight, non-terminating
assertions and a simple runner to execute multiple test cases from main().

Step 1: Program Understanding and Candidate Keywords
The focal method CheckNamedColorLUT builds a 3x3 LUT, constructs a Named Color List (nc),
fills it with 256 named entries, inserts a stage that uses that color list, and then
evaluates the LUT to verify that the outputs match the inputs. Core dependent components
and "candidate keywords" identified for test design include:
- cmsPipelineAlloc / cmsPipelineFree
- cmsAllocNamedColorList
- cmsAppendNamedColor
- cmsPipelineInsertStage
- _cmsStageAllocNamedColor
- cmsFreeNamedColorList
- cmsPipelineEval16
- Inw / Outw buffers (input/output pixel data)
- 256-entry Named Color generation (Name = "#<index>")
- DbgThread() (cmsContext) as a context provider
- Parameterized color PCS and Colorant values per entry
- Final verification: n2 == 0 (all outputs match inputs)

Step 2: What this test suite targets
- The normal, successful path (expected return 1)
- Repeated invocations yield consistent results (idempotence)
- Non-terminating assertions that allow full test execution even if one test fails
- Minimalistic coverage of the public API path without relying on private internals

Notes about limitations
- Due to library internals and potential dependence on runtime memory, forcing the exact false-branch
  paths (nc == NULL or rc == 0) in a standalone unit test would require symbol interposition or
  controlled mocking of external C library internals, which is non-trivial in a single-file harness.
- The tests provided exercise the normal, intended successful path, and repeatedly invoking the
  focal function to catch obvious regressions (crashes, leaks, non-deterministic behavior).

Now, the test code:

*/

#include <cstring>
#include <iostream>
#include <cstdio>
#include <testcms2.h>


// Lightweight, non-terminating test framework
static int g_total_tests = 0;
static int g_failed_tests = 0;
static int g_assertions = 0;
static int g_failed_assertions = 0;

#define ASSERT_TRUE(cond, msg) do { \
    ++g_assertions; \
    if (!(cond)) { \
        ++g_failed_assertions; \
        std::cerr << "[ASSERT FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " - " << (msg) << " | Condition: " #cond << std::endl; \
    } \
} while (0)

#define ASSERT_EQ(a, b, msg) do { \
    ++g_assertions; \
    if (!((a) == (b))) { \
        ++g_failed_assertions; \
        std::cerr << "[ASSERT FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " - " << (msg) << " | Expected " << (a) << " == " << (b) \
                  << std::endl; \
    } \
} while (0)

// Extern C declaration for the focal function under test.
// The actual implementation is provided by the codebase linked during build.
extern "C" int CheckNamedColorLUT(void); // cmsInt32Number CheckNamedColorLUT(void)

// Optional: helper to print a short summary
void PrintSummary() {
    std::cout << "Tests run: " << g_total_tests
              << ", Failures: " << g_failed_assertions
              << "/" << g_assertions << " assertions."
              << std::endl;
}

// Test 1: Normal path - ensure the function returns 1 (true) on typical build
void Test_CheckNamedColorLUT_NormalPath() {
    const char* test_name = "Test_CheckNamedColorLUT_NormalPath";
    ++g_total_tests;

    int result = CheckNamedColorLUT();
    ASSERT_EQ(result, 1, "CheckNamedColorLUT should return 1 on normal operation path");

    // Additional sanity: call again to ensure consistency (non-crashing, deterministic)
    int result2 = CheckNamedColorLUT();
    ASSERT_EQ(result2, 1, "CheckNamedColorLUT should return 1 on subsequent call");
}

// Test 2: Idempotence and repeatability
void Test_CheckNamedColorLUT_Idempotence() {
    const char* test_name = "Test_CheckNamedColorLUT_Idempotence";
    ++g_total_tests;

    int r1 = CheckNamedColorLUT();
    int r2 = CheckNamedColorLUT();
    int r3 = CheckNamedColorLUT();

    // We expect all results to be 1 in a stable environment
    ASSERT_EQ(r1, 1, "First call should return 1 (idempotence test)");
    ASSERT_EQ(r2, 1, "Second call should return 1 (idempotence test)");
    ASSERT_EQ(r3, 1, "Third call should return 1 (idempotence test)");
}

// Test 3: Repeated execution to detect potential crashes or leaks
void Test_CheckNamedColorLUT_NoCrashOnRepeat() {
    const char* test_name = "Test_CheckNamedColorLUT_NoCrashOnRepeat";
    ++g_total_tests;

    for (int i = 0; i < 5; ++i) {
        int r = CheckNamedColorLUT();
        ASSERT_EQ(r, 1, "Repeated invocation should consistently return 1");
    }
}

// Entry point for running tests
int main() {
    // Run selected test cases
    Test_CheckNamedColorLUT_NormalPath();
    Test_CheckNamedColorLUT_Idempotence();
    Test_CheckNamedColorLUT_NoCrashOnRepeat();

    // Print summary
    PrintSummary();

    // Return non-zero if any assertion failed
    if (g_failed_assertions > 0) {
        return 1;
    }
    return 0;
}
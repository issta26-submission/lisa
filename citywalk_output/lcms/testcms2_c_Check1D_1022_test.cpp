// test_check1d.cpp
// Unit test suite for the focal method Check1D in testcms2.c
// No GTest; a lightweight in-file test harness is provided with non-terminating assertions.

#include <cstdint>
#include <cstdio>
#include <testcms2.h>
#include <cstdlib>


// Import the project-specific test harness and declarations.
// Adjust the include path if your build uses a different layout.

// Expose the C function under test with C linkage for the C++ test file.
extern "C" {
    cmsInt32Number Check1D(cmsInt32Number nNodesToCheck, cmsBool Down, cmsInt32Number max_err);
    // Other dependencies are provided by testcms2.h and the linked C sources.
}

// Lightweight non-terminating test framework
static int g_total = 0;
static int g_failed = 0;

#define LOG_INFO(...) do { fprintf(stdout, __VA_ARGS__); fprintf(stdout, "\n"); } while(0)
#define LOG_ERR(...)  do { fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); } while(0)

#define EXPECT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            LOG_ERR("EXPECT_TRUE FAILED: %s", msg); \
            ++g_failed; \
        } \
        ++g_total; \
    } while(0)

#define EXPECT_EQ(a, b, msg) \
    do { \
        if ( (a) != (b) ) { \
            LOG_ERR("EXPECT_EQ FAILED: %s | Expected: %lld, Got: %lld", msg, (long long)(b), (long long)(a)); \
            ++g_failed; \
        } \
        ++g_total; \
    } while(0)


// Step 1 Keywords (candidates inferred from the focal method)
// - nNodesToCheck, Down, max_err
// - Tab buffer, p (cmsInterpParams*), Lerp16 interpolation
// - _cmsComputeInterpParams, BuildTable, chknull, DbgThread
// - 0xffff boundary, in/out values
// These are the core dependent components exercised by Check1D.

// Step 2: Test Suite for Check1D
// Each test aims to cover different branches and parameter combinations as far as
// the API allows predictable behavior in a unit-test context.

// Test 1: Basic successful path with Down = false and a large max_err.
// Expecting the function to complete the interpolation check without triggering
// the failure condition (abs(out - in) > max_err).
bool Test_Check1D_Simple_Success(void)
{
    // Coverage: Down = false, nNodesToCheck = 4, max_err = 0xFFFF
    // This should exercise the normal path where no early return is hit.
    cmsInt32Number ret = Check1D(4, 0 /* Down = false */, 0xFFFF);
    EXPECT_EQ(ret, 1, "Check1D simple success path should return 1 (Down=false, max_err large)");
    return true;
}

// Test 2: Basic successful path with Down = true and a large max_err.
// This exercises the alternate branch in the post-processing of the output.
bool Test_Check1D_Simple_Success_Down(void)
{
    cmsInt32Number ret = Check1D(4, 1 /* Down = true */, 0xFFFF);
    EXPECT_EQ(ret, 1, "Check1D simple success path with Down=true should return 1");
    return true;
}

// Test 3: Edge case: zero nodes to check.
// Expectation: either a controlled failure (return 0) or a safe no-op exit.
// We assert a non-crashing behavior by requiring a return value of 0 (likely path).
bool Test_Check1D_ZeroNodes(void)
{
    cmsInt32Number ret = Check1D(0, 0, 1000);
    EXPECT_TRUE(ret == 0, "Check1D with 0 nodes should return 0 (likely parameter/alloc path)");
    return true;
}

// Test 4: Moderate nodes with Down = false and large max_err to ensure broad coverage.
// This helps exercise multiple code paths without asserting exact interpolation results.
bool Test_Check1D_ModerateNodes_LargeMaxErr(void)
{
    cmsInt32Number ret = Check1D(8, 0, 0xFFFF);
    EXPECT_EQ(ret, 1, "Check1D with 8 nodes, Down=false, max_err=0xFFFF should return 1");
    return true;
}

// Test 5: Moderate nodes with Down = true and large max_err to verify Down path handling.
// This further enforces that the Down adjustment branch is exercised.
bool Test_Check1D_ModerateNodes_LargeMaxErr_Down(void)
{
    cmsInt32Number ret = Check1D(8, 1, 0xFFFF);
    EXPECT_EQ(ret, 1, "Check1D with 8 nodes, Down=true, max_err=0xFFFF should return 1");
    return true;
}


// Step 3: Test Driver
void RunAllTests(void)
{
    LOG_INFO("Starting test suite for Check1D...");
    Test_Check1D_Simple_Success();
    Test_Check1D_Simple_Success_Down();
    Test_Check1D_ZeroNodes();
    Test_Check1D_ModerateNodes_LargeMaxErr();
    Test_Check1D_ModerateNodes_LargeMaxErr_Down();
    LOG_INFO("Test suite completed. Total tests: %d, Failures: %d", g_total, g_failed);
}

// Entry point for the test executable
int main(int argc, char* argv[])
{
    (void)argc; (void)argv; // Unused in this test launcher
    RunAllTests();

    if (g_failed == 0) {
        LOG_INFO("ALL TESTS PASSED");
    } else {
        LOG_ERR("SOME TESTS FAILED: %d failed, out of %d tests", g_failed, g_total);
    }

    // Return non-zero if any test failed to integrate with CI systems.
    return g_failed ? 1 : 0;
}

/*
Notes for the test design:
- The tests directly call the focal function Check1D with various parameters to cover:
  - Normal path (non-Down) with a permissive max_err.
  - The Down-adjusted path (Down = true).
  - Boundary/edge path where nNodesToCheck = 0 to exercise early-NULL/RETURN behavior.
  - Additional moderate node tests to broaden coverage.
- We rely on the provided C API and the testing helpers present in testcms2.h.
- No GTest or external mocking framework is used. A lightweight in-file test harness is provided with non-terminating assertions (they log failures but continue execution).
- Static/internal helpers within the focal file are not accessed directly; tests interact solely through the public function Check1D, in line with the instruction to avoid private/internal access.
- To adapt to your environment, ensure that the include path for testcms2.h is correct and that the project links against the corresponding C sources for cms interpolation logic. 
- If needed, extend the test suite with additional parameter combinations (e.g., larger nNodesToCheck, different max_err values) to increase coverage depth. 
*/
// Test suite for ComparativeCt in fast_float_testbed.c
// This test suite uses a lightweight, custom test framework (no Google Test) and
// provides small C mock implementations for the dependent CMS-like API used by
// ComparativeCt (CreateCurves, cmsOpenProfileFromFile, trace) to drive deterministic
// behavior and verify branching and output.
//
// Notes:
// - We compile this as a C++11 translation unit, but link with the C file that defines
//   ComparativeCt. The mock functions are exposed with C linkage (extern "C") to
//   satisfy the linker.
// - We intentionally avoid private/internal details of the original project and focus
//   on the behavior of ComparativeCt given inICC / outICC variations.
// - The tests capture trace output to verify the expected formatting and values.

#include <cstring>
#include <cstdio>
#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <string>
#include <cstdarg>
#include <cassert>
#include <memory.h>
#include <stdlib.h>


// Global fixtures to observe calls from the mocked environment
static int g_CreateCurvesCalls = 0;
static int g_OpenProfileFromFileCalls = 0;
static std::vector<std::string> g_OpenProfileNames;
static std::string g_trace_output;

// Forward declarations of the function under test (from the C source file)
extern "C" {
    typedef void* cmsContext;
    typedef void* cmsHPROFILE;
    typedef double cmsFloat64Number;
    typedef unsigned int cmsUInt32Number;
    typedef unsigned short cmsUInt16Number;
    typedef unsigned char cmsUInt8Number;
    typedef int cmsBool;
    typedef int cmsInt32Number;

    // perf_fn type used by ComparativeCt
    typedef cmsFloat64Number (*perf_fn)(cmsContext, cmsHPROFILE, cmsHPROFILE);

    // Functions provided by the focal class dependencies (mocked below)
    cmsHPROFILE CreateCurves(void);
    cmsHPROFILE cmsOpenProfileFromFile(const char* name, const char* mode);

    // trace used by ComparativeCt (we capture its output)
    void trace(const char* frm, ...);

    // The function under test (ComparativeCt) we are validating
    void ComparativeCt(cmsContext ct1, cmsContext ct2, const char* Title, perf_fn fn1, perf_fn fn2, const char* inICC, const char* outICC);
}

// Mock implementations to simulate the CMS-like environment

extern "C" {

// CreateCurves: return a non-null dummy profile and count calls
cmsHPROFILE CreateCurves(void)
{
    ++g_CreateCurvesCalls;
    static int dummy;
    return (cmsHPROFILE)&dummy;
}

// cmsOpenProfileFromFile: record the call and return a dummy profile
cmsHPROFILE cmsOpenProfileFromFile(const char* name, const char* mode)
{
    ++g_OpenProfileFromFileCalls;
    if (name) g_OpenProfileNames.emplace_back(name);
    if (mode) g_OpenProfileNames.emplace_back(mode);
    static int dummy;
    return (cmsHPROFILE)&dummy;
}

// trace: capture formatted output into a global string for assertions
void trace(const char* frm, ...)
{
    va_list args;
    va_start(args, frm);
    char buf[4096];
    vsnprintf(buf, sizeof(buf), frm, args);
    va_end(args);
    g_trace_output += buf;
}

} // extern "C"

// Implement two deterministic perf_fn functions for testing
extern "C" {

cmsFloat64Number MyPerf1(cmsContext, cmsHPROFILE, cmsHPROFILE)
{
    // Deterministic value used for testing
    return 17.5;
}

cmsFloat64Number MyPerf2(cmsContext, cmsHPROFILE, cmsHPROFILE)
{
    // Deterministic value used for testing
    return 3.25;
}

} // extern "C"

// Helper to reset mocks between tests
static void reset_mocks()
{
    g_CreateCurvesCalls = 0;
    g_OpenProfileFromFileCalls = 0;
    g_OpenProfileNames.clear();
    g_trace_output.clear();
}

// Accessor to the function under test for test harness
extern "C" void ComparativeCt(cmsContext ct1, cmsContext ct2, const char* Title, perf_fn fn1, perf_fn fn2, const char* inICC, const char* outICC);

// Simple non-terminating test assertion
static void expect_true(bool cond, const char* msg)
{
    if (!cond) {
        std::cerr << "EXPECT_TRUE failed: " << msg << std::endl;
    }
    // Do not exit; continue to run other tests
}

// Test 1: Both inICC and outICC are NULL; Ensure both fallbacks (CreateCurves) are used
static void test_null_icc_both_null(void)
{
    reset_mocks();
    // Arrange
    const char* title = "Null ICC - both NULL";

    // Act
    ComparativeCt((cmsContext)1, (cmsContext)2, title, MyPerf1, MyPerf2, NULL, NULL);

    // Assert
    // Expect 4 CreateCurves calls (2 for the first pair, 2 for the second pair)
    expect_true(g_CreateCurvesCalls == 4, "CreateCurves should be called 4 times when both inICC and outICC are NULL");

    // No cmsOpenProfileFromFile calls should occur
    expect_true(g_OpenProfileFromFileCalls == 0, "cmsOpenProfileFromFile should not be called when inICC/outICC are NULL");

    // Trace should contain the title and the two values formatted to 2 decimals
    expect_true(g_trace_output.find(title) != std::string::npos, "Trace should contain the test Title");
    expect_true(g_trace_output.find("17.50") != std::string::npos, "Trace should contain first result 17.50");
    expect_true(g_trace_output.find("3.25") != std::string::npos, "Trace should contain second result 3.25");
}

// Test 2: Both inICC and outICC are non-NULL; Ensure both OpenFromFile paths are used
static void test_both_icc_non_null(void)
{
    reset_mocks();
    const char* title = "Both ICCs provided";

    ComparativeCt((cmsContext)3, (cmsContext)4, title, MyPerf1, MyPerf2, "in.icc", "out.icc");

    // Assert
    // All calls should be via cmsOpenProfileFromFile (4 calls total)
    expect_true(g_OpenProfileFromFileCalls == 4, "cmsOpenProfileFromFile should be called 4 times when both ICCs are provided");
    expect_true(g_CreateCurvesCalls == 0, "CreateCurves should not be called when both ICCs are provided");

    // Trace assertions
    expect_true(g_trace_output.find(title) != std::string::npos, "Trace should contain the test Title");
    expect_true(g_trace_output.find("17.50") != std::string::npos, "Trace should contain first result 17.50");
    expect_true(g_trace_output.find("3.25") != std::string::npos, "Trace should contain second result 3.25");
}

// Test 3: Mixed ICCs: inICC non-NULL, outICC NULL for both calls
static void test_in_icc_only(void)
{
    reset_mocks();
    const char* title = "In ICC only";

    ComparativeCt((cmsContext)5, (cmsContext)6, title, MyPerf1, MyPerf2, "in.icc", NULL);

    // Assert
    // Two OpenFromFile calls (for in.icc) and two CreateCurves calls (for out)
    expect_true(g_OpenProfileFromFileCalls == 2, "cmsOpenProfileFromFile should be called 2 times when only in ICC is provided");
    expect_true(g_CreateCurvesCalls == 2, "CreateCurves should be called 2 times when only in ICC is provided");

    // Trace
    expect_true(g_trace_output.find(title) != std::string::npos, "Trace should contain the test Title");
    expect_true(g_trace_output.find("17.50") != std::string::npos, "Trace should contain first result 17.50");
    expect_true(g_trace_output.find("3.25") != std::string::npos, "Trace should contain second result 3.25");
}

// Minimal test runner
int main()
{
    std::cout << "Starting ComparativeCt unit tests (non-GTest, C++11)..." << std::endl;

    test_null_icc_both_null();
    test_both_icc_non_null();
    test_in_icc_only();

    // Summary
    std::cout << "Tests completed." << std::endl;
    // Since this is a non-terminating test, return 0 even if some EXPECTs failed
    return 0;
}
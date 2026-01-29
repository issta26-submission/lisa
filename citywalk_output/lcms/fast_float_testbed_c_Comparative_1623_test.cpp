// Lightweight C++11 unit test harness for the focal method: Comparative
// This test suite targets the function:
//     void Comparative(const char* Title, perf_fn fn1, perf_fn fn2, const char* inICC, const char* outICC)
// which, per its definition, delegates to ComparativeCt(0, 0, ...).
//
// Approach:
// - Provide two test perf_fn implementations that increment global counters when invoked.
// - Call Comparative with those perf_fn implementations and verify that both functions
//   were invoked (to exercise the true/false branches indirectly via the function pointers).
// - Include 3 test cases to increase coverage: basic invocation, swapped function pointers,
//   and invocation with empty ICC strings.
// - Use a minimal, non-terminating assertion framework and print a summary at the end.

#include <fast_float_internal.h>
#include <iostream>
#include <string>
#include <memory.h>
#include <stdlib.h>


// Bring in the C declarations from the project under test.
// We assume the project provides the necessary types (cmsFloat64Number, cmsContext, cmsHPROFILE)
// and the focal function Comparative with the signature using a perf_fn typedef.
extern "C" {
}

// The perf_fn type is a function pointer used by Comparative/Performance in the project.
// We declare it here to be compatible with the project signature.
// If the actual typedef in the project header differs, adapt accordingly.
typedef cmsFloat64Number (*PerfFn)(cmsContext, cmsHPROFILE, cmsHPROFILE);

// Forward declaration of Comparative so we can call it from C++ tests.
// If the project provides a different prototype, adjust this declaration to match.
// Signature assumed: void Comparative(const char* Title, perf_fn fn1, perf_fn fn2, const char* inICC, const char* outICC)
extern "C" void Comparative(const char* Title, PerfFn fn1, PerfFn fn2, const char* inICC, const char* outICC);

// Global counters to verify perf_fn invocations
static int g_perf1_calls = 0;
static int g_perf2_calls = 0;

// Dummy perf_fn implementations used by tests.
// They increment the respective global counters and return a deterministic value.
extern "C" cmsFloat64Number perfFn1(cmsContext /*ct*/, cmsHPROFILE /*inP*/, cmsHPROFILE /*outP*/)
{
    ++g_perf1_calls;
    return 1.0;  // arbitrary deterministic value
}

extern "C" cmsFloat64Number perfFn2(cmsContext /*ct*/, cmsHPROFILE /*inP*/, cmsHPROFILE /*outP*/)
{
    ++g_perf2_calls;
    return 2.0;  // arbitrary deterministic value
}

// Simple, non-terminating test assertion helpers
static int assert_true(bool condition, const std::string& msg)
{
    if (!condition)
    {
        std::cerr << "ASSERTION FAILED: " << msg << std::endl;
        return 0; // failure
    }
    return 1; // success
}

// Test 1: Basic invocation - ensure both perf functions are invoked by Comparative
int test_basic_invocation()
{
    g_perf1_calls = 0;
    g_perf2_calls = 0;

    Comparative("TestBasicInvocation", perfFn1, perfFn2, "in_basic.icc", "out_basic.icc");

    int ok = assert_true(g_perf1_calls > 0, "perfFn1 should be invoked by Comparative");
    ok &= assert_true(g_perf2_calls > 0, "perfFn2 should be invoked by Comparative");
    return ok;
}

// Test 2: Swapped function pointers - ensure Comparative uses provided function pointers
int test_swapped_functions()
{
    // Reset counters
    g_perf1_calls = 0;
    g_perf2_calls = 0;

    // Pass the functions in swapped order
    Comparative("TestSwappedFunctions", perfFn2, perfFn1, "in_swapped.icc", "out_swapped.icc");

    int ok = assert_true(g_perf1_calls > 0, "perfFn1 should be invoked even when swapped");
    ok &= assert_true(g_perf2_calls > 0, "perfFn2 should be invoked even when swapped");
    return ok;
}

// Test 3: Empty ICC strings - ensure function remains callable with empty ICC parameters
int test_empty_icc_strings()
{
    g_perf1_calls = 0;
    g_perf2_calls = 0;

    Comparative("TestEmptyICC", perfFn1, perfFn2, "", "");

    int ok = assert_true(g_perf1_calls > 0, "perfFn1 should be invoked with empty ICCs");
    ok &= assert_true(g_perf2_calls > 0, "perfFn2 should be invoked with empty ICCs");
    return ok;
}

// Minimal test harness
struct TestCase {
    const char* name;
    int (*func)();
};

int main()
{
    // List of tests to run
    TestCase tests[] = {
        { "Basic invocation", test_basic_invocation },
        { "Swapped function pointers", test_swapped_functions },
        { "Empty ICC strings", test_empty_icc_strings },
    };

    const int numTests = sizeof(tests) / sizeof(tests[0]);
    int total = 0;
    int passed = 0;

    std::cout << "Starting Comparative unit tests (C++11, non-GTest)..." << std::endl;

    for (int i = 0; i < numTests; ++i)
    {
        std::cout << "Running test: " << tests[i].name << std::endl;
        int result = tests[i].func();
        ++total;
        if (result)
        {
            ++passed;
            std::cout << "PASS: " << tests[i].name << std::endl;
        }
        else
        {
            std::cout << "FAIL: " << tests[i].name << std::endl;
        }
    }

    std::cout << "Tests passed: " << passed << " / " << total << std::endl;
    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}
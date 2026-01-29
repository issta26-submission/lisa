/*
Unit test suite for the focal method PerformanceHeader located in fast_float_testbed.c

Approach and rationale:
- Step 1 (Program Understanding): The focal method PerformanceHeader simply emits a header line using trace(...) and returns (void).
  The surrounding codebase provides many related functions and stubs (e.g., trace, other performance-related helpers), but PerformanceHeader itself has no branches or complex logic.
  Candidate Keywords derived from the focal file: PerformanceHeader, trace, MPixel/sec, MByte/sec, header formatting, void return, no-branch flow.

- Step 2 (Unit Test Generation): Since the project uses C-style code with a C function PerformanceHeader, the tests must be C++11 compliant but interface with the C function. We cannot rely on GTest (as requested). The tests focus on:
  1) Verifying the function signature via a compile-time static assertion.
  2) Verifying that calling PerformanceHeader does not crash and completes (multiple invocations to exercise the call path).

- Step 3 (Test Case Refinement): Tests are written to maximize code execution without terminating the process on assertion failures (non-terminating assertions). A small, self-contained test harness is provided with a minimal runtime assertion macro. The tests rely only on standard C++ library and the provided C function PerformanceHeader. Static checks ensure the correct function signature. Since PerformanceHeader delegates to trace, which is a no-op in the provided class dependencies, runtime verification focuses on crash-free execution and correct symbol signature instead of capturing printed output.

Notes for integration:
- The test suite assumes that fast_float_testbed.c (the file containing PerformanceHeader) is compiled and linked with this test file in the final build.
- Because trace is a stub in the provided dependencies, the test cannot capture the exact printed header content. Instead, we verify that the function exists with the correct signature and can be invoked without causing a crash.

Code (C++11 test file):

*/
#include <cstdio>
#include <fast_float_internal.h>
#include <iostream>
#include <type_traits>
#include <memory.h>
#include <stdlib.h>


// Step 2: Declaration of the focal function (C linkage)
extern "C" void PerformanceHeader(void);

// Compile-time signature check: ensure PerformanceHeader has signature void(void)
static_assert(std::is_same<decltype(&PerformanceHeader), void(*)()>::value,
              "PerformanceHeader must have signature void PerformanceHeader(void)");

// Simple, non-terminating assertion macro suitable for a lightweight test harness
static int g_failures = 0;
#define EXPECT(cond) do { \
    if(!(cond)) { \
        std::cerr << "Test failed: " << __FILE__ << ":" << __LINE__ \
                  << " - Condition: " #cond << std::endl; \
        ++g_failures; \
    } \
} while(0)

// Test 1: Signature verification (redundant at runtime due to static_assert but keeps the test explicit)
void Test_PerformanceHeader_Signature()
{
    // The static_assert already validates the signature at compile time.
    // We still invoke PerformanceHeader to ensure the call path is executable.
    PerformanceHeader();
    PerformanceHeader();
}

// Test 2: Runtime behavior verification (crash-free execution on multiple invocations)
void Test_PerformanceHeader_NoCrash()
{
    // Call PerformanceHeader multiple times to exercise the function path.
    // Note: The actual header content is emitted via trace, which is a stub in this test environment.
    PerformanceHeader();
    PerformanceHeader();
    PerformanceHeader();
    // A non-fatal assertion to mark test progress
    EXPECT(true);
}

// Entry point for running tests
int main()
{
    std::cout << "Running PerformanceHeader tests (C++11)..." << std::endl;

    // Run tests
    Test_PerformanceHeader_Signature();
    Test_PerformanceHeader_NoCrash();

    if (g_failures != 0) {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }

    std::cout << "All tests passed." << std::endl;
    return 0;
}
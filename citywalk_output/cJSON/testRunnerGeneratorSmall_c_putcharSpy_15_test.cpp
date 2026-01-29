/*
This file provides a minimal C++11 test harness for the focal method:
  void putcharSpy(int c) { (void)putchar(c); }

Rationale and approach:
- To observe the argument passed to the underlying putchar, we create a local
  mock_putchar function and redirect all calls from putcharSpy to this mock
  by redefining putchar via a macro before including the focal file.
- We include the focal source (testRunnerGeneratorSmall.c) in this translation unit
  so that the macro substitution affects the actual putchar call inside putcharSpy.
- We then run a small set of tests (in plain C++) that verify that putcharSpy forwards
  the input value to putchar by inspecting the captured argument (g_lastPutcharArg).
- This approach avoids requiring GTest/GMock and respects the constraint to not
  use private members or methods of the focal code directly.
- All tests are annotated with comments explaining their intent.

Notes:
- The environment must provide the Unity framework (unity.h) as specified by the
  <FOCAL_CLASS_DEP> block. However, this harness does not rely on Unity APIs.
- The test suite is self-contained: it defines a simple assertion mechanism and
  prints results to stderr.
- The macro overriding technique is scoped to this translation unit only by using
  #undef after including the focal source.

Usage:
- Compile this file together with testRunnerGeneratorSmall.c in a C++11 capable compiler.
  Example (conceptual): g++ -std=c++11 test_putcharSpy.cpp -o test_putcharSpy
  Ensure the build links with the standard C library (libc) as usual.

*/

// Candidate Keywords derived from the focal method analysis:
// - putcharSpy, putchar, passthrough, linking tests, passthrough for linker tests,
// - C linkage, mock, capture, forward, argument forwarding, domains: stdio, Defs.h

#include <unity.h>
#include <Defs.h>
#include <testRunnerGeneratorSmall.c>
#include <cstdio>
#include <cstdlib>
#include <stdio.h>


// Global to capture the last value forwarded to putchar by the spy
static int g_lastPutcharArg = -1;

// Mock implementation of putchar to capture the forwarded value.
// It must be C-callable for compatibility with the focal code.
// We use extern "C" only if compiling under C++, to ensure C linkage for the mock.
#ifdef __cplusplus
extern "C" {
#endif
static int mock_putchar(int c) {
    g_lastPutcharArg = c;
    // Return value is ignored by the spy (void cast in implementation)
    return 0;
}
#ifdef __cplusplus
}
#endif

// Redirect calls to putchar to our mock within this translation unit.
// This macro affects the subsequent inclusion of the focal source file,
// so that the call inside putcharSpy goes to mock_putchar.
#define putchar mock_putchar

// Include the focal method and its dependencies.
// The macro above ensures the inline passthrough becomes a passthrough to the mock.

#undef putchar  // Restore macro scope to avoid leaking into other code

// Lightweight test harness (no GTest/GMock). Outputs results to stderr.
static int tests_run = 0;
static int tests_failed = 0;

static void run_assert(int condition, const char* msg) {
    ++tests_run;
    if (!condition) {
        ++tests_failed;
        std::fprintf(stderr, "ASSERT FAILED: %s\n", msg);
    }
}

//
// Unit Test 1
// Description: Verify that putcharSpy forwards the value 0 to putchar.
// Rationale: Covers the base-case path where a legitimate character value is passed.
// Expected: g_lastPutcharArg == 0 after invocation.
//
static void test_putcharSpy_for_zero(void) {
    g_lastPutcharArg = -999;       // reset capture
    putcharSpy(0);                 // invoke focal method
    run_assert(g_lastPutcharArg == 0, "putcharSpy(0) should forward 0 to putchar");
}

//
// Unit Test 2
// Description: Verify that putcharSpy forwards the ASCII value for 'A' (65).
// Rationale: Checks typical positive, printable character value.
// Expected: g_lastPutcharArg == 65 after invocation.
//
static void test_putcharSpy_for_A(void) {
    g_lastPutcharArg = -999;
    putcharSpy(65);
    run_assert(g_lastPutcharArg == 65, "putcharSpy(65) should forward 65 to putchar");
}

//
// Unit Test 3
// Description: Verify forwarding of a large/sentinel value (INT_MAX for int).
// Rationale: Ensures the function handles boundary values without undefined behavior.
// Expected: g_lastPutcharArg == INT_MAX after invocation.
//
static void test_putcharSpy_for_INT_MAX(void) {
    const int val = 2147483647; // INT_MAX for 32-bit int
    g_lastPutcharArg = -999;
    putcharSpy(val);
    run_assert(g_lastPutcharArg == val, "putcharSpy(INT_MAX) should forward INT_MAX");
}

//
// Unit Test 4
// Description: Verify multiple consecutive invocations forward each value correctly.
// Rationale: Ensures internal state (captured argument) updates with successive calls.
// Expected: Each invocation updates g_lastPutcharArg accordingly.
// Values tested: 1, 2, -5, 0
//
static void test_putcharSpy_multiple_calls(void) {
    const int values[] = {1, 2, -5, 0};
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        g_lastPutcharArg = -999;
        putcharSpy(values[i]);
        char msg[64];
        std::snprintf(msg, sizeof(msg), "putcharSpy(%d) should forward %d", values[i], values[i]);
        run_assert(g_lastPutcharArg == values[i], msg);
    }
}

int main(void) {
    // Run tests
    test_putcharSpy_for_zero();
    test_putcharSpy_for_A();
    test_putcharSpy_for_INT_MAX();
    test_putcharSpy_multiple_calls();

    // Report summary
    std::fprintf(stderr, "Test Summary: total=%d, failed=%d\n", tests_run, tests_failed);

    // Return non-zero if any test failed
    return (tests_failed == 0) ? 0 : 1;
}
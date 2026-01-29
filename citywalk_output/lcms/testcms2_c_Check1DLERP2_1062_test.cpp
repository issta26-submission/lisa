/*
Unit Test Suite for Check1DLERP2 (C function wrapper around Check1D)

Context and approach:
- Focal method (from Step 1): Check1DLERP2 simply returns the result of Check1D(2, FALSE, 0).
  Focal keywords: Check1D, 2, FALSE, 0.
- We cannot rely on the internal implementation of Check1D from testcms2.c, but we can
  exercise the wrapper relationship by comparing Check1DLERP2 against a direct
  invocation of Check1D with the same arguments.
- Test harness is written in C++11 and uses a lightweight, non-terminating assertion
  mechanism (prints PASS/FAIL but continues).
- We avoid including testcms2.h to prevent potential multiple-definition conflicts
  with a main() entry in that header. Instead, we declare C-linkage prototypes for
  the needed functions and rely on linking against testcms2.c (or equivalent).
- Static members: since we do not modify or mock static state inside Check1D, tests
  focus on behavioral equivalence of the wrapper to the underlying function.

Candidate Keywords extracted from focal method:
- Check1D: the underlying computation invoked by Check1DLERP2
- 2: nNodesToCheck argument
- FALSE: Down flag (0)
- 0: max_err

Notes for integration:
- If your build system uses a separate test runner that already defines main(),
  you can adapt to export the test code accordingly. The current version provides its
  own main() for standalone execution.
- Ensure testcms2.c (or the library providing Check1D/Check1DLERP2) is compiled and linked
  with this test file.

Below is the standalone C++11 test file. It declares the necessary C interfaces,
executes a small set of tests, and prints results to stdout. It should be compiled
together with the C source containing the actual implementations of Check1D and
Check1DLERP2 (e.g., testcms2.c).

Usage example (assumes linking with testcms2.c or a compatible library):
  g++ -std=c++11 -I. test_Check1DLERP2.cpp testcms2.c -o test_Check1DLERP2

*/

#include <iostream>
#include <testcms2.h>
#include <iomanip>


// Declare the C-style interfaces with C linkage to avoid name mangling issues
extern "C" {
    // Return type assumed to be int (cmsInt32Number)
    int Check1D(int nNodesToCheck, int Down, int max_err);

    // Wrapper function under test; returns int as in cmsInt32Number
    int Check1DLERP2(void);
}

// Lightweight test framework (non-terminating; continues after failures)
static int g_failed = 0;

#define ASSERT_EQ(a, b, desc) do {                           \
    if ((a) != (b)) {                                         \
        ++g_failed;                                           \
        std::cerr << "[FAIL] " << desc                          \
                  << " | " << #a << " = " << (a)               \
                  << " , " << #b << " = " << (b) << std::endl; \
    } else {                                                  \
        std::cout << "[PASS] " << desc << std::endl;          \
    }                                                         \
} while (0)

namespace {

/*
Test 1: Check1DLERP2 should return the same value as direct Check1D(2, FALSE, 0)

Rationale:
- Since Check1DLERP2 is defined as a wrapper around Check1D with fixed parameters
  (nNodesToCheck=2, Down=FALSE, max_err=0), its return value should be identical to
  a direct call with those exact arguments.
*/
void test_Check1DLERP2_matches_Check1D_values() {
    int res_wrapper = Check1DLERP2();
    int res_direct  = Check1D(2, 0, 0);
    ASSERT_EQ(res_wrapper, res_direct, "Check1DLERP2 should equal Check1D(2,0,0)");
}

/*
Test 2: Idempotence-like check

Rationale:
- If Check1DLERP2 is pure with respect to its inputs (which are fixed constants),
  repeated calls should yield identical results assuming no internal state mutation
  occurs during a call. This is a light-weight property test for stability.
*/
void test_Check1DLERP2_idempotence() {
    int r1 = Check1DLERP2();
    int r2 = Check1DLERP2();
    ASSERT_EQ(r1, r2, "Check1DLERP2 should be idempotent for consecutive calls");
}

/*
Test 3: Cross-check with a dependent Check1D invocation (sanity against interface)

Rationale:
- Verify that the two-step usage (wrapper vs direct) remains consistent across
  multiple variations of the direct-Check1D path for the same fixed parameters.
  Since the parameters are fixed in Check1DLERP2, this test repeats the direct call
  and ensures consistency across several invocations.
*/
void test_Check1DLERP2_consistency_across_direct_invocations() {
    const int expected = Check1D(2, 0, 0);
    const int wrappers[3] = { Check1DLERP2(), Check1DLERP2(), Check1DLERP2() };
    for (int i = 0; i < 3; ++i) {
        ASSERT_EQ(wrappers[i], expected, "Check1DLERP2 consistency across repeated calls");
    }
}

} // anonymous namespace

int main() {
    std::cout << "Running unit tests for Check1DLERP2 (wrapper around Check1D(2, FALSE, 0))" << std::endl;

    test_Check1DLERP2_matches_Check1D_values();
    test_Check1DLERP2_idempotence();
    test_Check1DLERP2_consistency_across_direct_invocations();

    std::cout << "Tests completed. Total failures: " << g_failed << std::endl;
    return g_failed;
}
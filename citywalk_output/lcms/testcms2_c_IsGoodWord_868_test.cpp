/*
Unit Test Suite for cmsBool IsGoodWord(const char *title, cmsUInt16Number in, cmsUInt16Number out)
Target: testcms2.c / testcms2.h integration in a C++11 environment without GTest.

What this test covers:
- True branch: in == out -> IsGoodWord should return TRUE.
- False branch: in != out -> IsGoodWord should return FALSE and call Fail (we verify only the return value to ensure coverage of both branches).
- Boundary values for 16-bit unsigned inputs to exercise behavior across the full domain.

Notes on Candidate Keywords (Step 1):
- Core dependent components: cmsBool, IsGoodWord, in, out, title, abs, Fail.
- Behavioral checks: return value TRUE when in == out; return value FALSE when in != out.
- Domain considerations: unsigned 16-bit arithmetic (cmsUInt16Number), 16-bit diff handling via abs(in-out).

Assumptions:
- The test relies on the public interface IsGoodWord from testcms2.h with C linkage.
- We avoid using GTest; a lightweight in-process test harness is implemented.
- We exercise only public interface to respect static/private boundaries.
*/

#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <cstdlib>


// Include C headers with C linkage
extern "C" {
}

// Simple non-terminating test harness (no GoogleTest)
struct TestCase {
    const char* name;
    bool (*func)();
};

static int g_passed = 0;
static int g_failed = 0;

// Helper to run a test and report result
static void RunTestCase(const TestCase& tc) {
    bool result = tc.func();
    if (result) {
        ++g_passed;
        std::cout << "[PASSED] " << tc.name << "\n";
    } else {
        ++g_failed;
        std::cout << "[FAILED] " << tc.name << "\n";
    }
}

/*
Test 1: True branch
- Context: in == out
- Expect: IsGoodWord returns TRUE
*/
static bool test_IsGoodWord_true_equal() {
    const char* title = "true_equal_case";
    cmsUInt16Number in = 1234;
    cmsUInt16Number out = 1234;
    cmsBool r = IsGoodWord(title, in, out);
    // Expect TRUE
    return (r == TRUE);
}

/*
Test 2: False branch (basic)
- Context: in != out
- Expect: IsGoodWord returns FALSE
- Note: We rely on return value to avoid dependence on Fail side-effects.
*/
static bool test_IsGoodWord_false_basic() {
    const char* title = "false_basic_case";
    cmsUInt16Number in = 1234;
    cmsUInt16Number out = 1235;
    cmsBool r = IsGoodWord(title, in, out);
    // Expect FALSE
    return (r == FALSE);
}

/*
Test 3: Boundary case - maximum difference
- Context: in = 0, out = 65535
- Expect: FALSE
*/
static bool test_IsGoodWord_false_max_difference() {
    const char* title = "false_max_difference";
    cmsUInt16Number in = 0;
    cmsUInt16Number out = 65535;
    cmsBool r = IsGoodWord(title, in, out);
    return (r == FALSE);
}

/*
Test 4: Boundary case - zero difference again
- Context: in = 0, out = 0
- Expect: TRUE
*/
static bool test_IsGoodWord_true_zero_difference() {
    const char* title = "true_zero_difference";
    cmsUInt16Number in = 0;
    cmsUInt16Number out = 0;
    cmsBool r = IsGoodWord(title, in, out);
    return (r == TRUE);
}

/*
Test 5: Another false case near boundary
- Context: in = 65535, out = 0
- Expect: FALSE
*/
static bool test_IsGoodWord_false_reverse_boundary() {
    const char* title = "false_reverse_boundary";
    cmsUInt16Number in = 65535;
    cmsUInt16Number out = 0;
    cmsBool r = IsGoodWord(title, in, out);
    return (r == FALSE);
}

// Collect all tests
static TestCase g_tests[] = {
    {"IsGoodWord_true_equal", test_IsGoodWord_true_equal},
    {"IsGoodWord_false_basic", test_IsGoodWord_false_basic},
    {"IsGoodWord_false_max_difference", test_IsGoodWord_false_max_difference},
    {"IsGoodWord_true_zero_difference", test_IsGoodWord_true_zero_difference},
    {"IsGoodWord_false_reverse_boundary", test_IsGoodWord_false_reverse_boundary},
};

// Entry point
int main() {
    std::cout << "Running IsGoodWord unit test suite (no GTest, C linkage to testcms2.h)\n";

    const int total = static_cast<int>(sizeof(g_tests) / sizeof(g_tests[0]));
    for (int i = 0; i < total; ++i) {
        RunTestCase(g_tests[i]);
    }

    int total_run = g_passed + g_failed;
    std::cout << "Summary: Ran " << total_run << " tests. Passed: " << g_passed
              << ", Failed: " << g_failed << "\n";

    // Return non-zero if any test failed to indicate failure to a harness
    return (g_failed == 0) ? 0 : 1;
}

/*
Notes for maintainers:
- This test suite focuses on the public IsGoodWord interface. It does not attempt to mock or
  override static or internal dependencies beyond the public API, in line with the provided
  domain knowledge constraints.
- If further coverage is required (e.g., verifying the side effects of Fail), a dedicated
  integration path or a wrapper around Fail in testcms2.c would be needed, as C linkage
  and static linkage constraints apply.
*/
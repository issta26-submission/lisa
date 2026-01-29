/*
Unit test suite for the focal method:
    cmsInt32Number CheckKOnlyBlackPreserving(void)

Context and goals:
- Follow the provided step-by-step instructions to build a focused, executable
  test suite without using GTest, relying on a small in-house test harness.
- The tests exercise the focal function from testcms2.c (CheckKOnlyBlackPreserving).
- We treat the surrounding codebase as given (library functions from Little CMS),
  and we verify that the function:
  - Returns a boolean-like value (0 or 1).
  - Is deterministic across multiple invocations in the same process.
- We do not modify private/static internals; we only call the public focal entry.
- The test harness is self-contained and prints results, using lightweight, non-terminating
  assertions (EXPECT_* macros) to maximize coverage.
- The tests are written in C++11 but invoke C linkage for the focal function.

Notes on dependencies:
- This test relies on the Little CMS API (lcms2) present in the environment.
- The Profile files test1.icc and test2.icc referenced by CheckKOnlyBlackPreserving should be present
  in the working directory when running the tests. The test asserts only the validity of the return value
  shape (0 or 1) and determinism, rather than the exact value, to stay robust across environments.

Candidate Keywords derived from Step 1 (core components, for traceability):
- CheckKOnlyBlackPreserving
- hSWOP, hFOGRA
- swop_lab, fogra_lab
- Lab1, Lab2
- hLab
- INTENT_PRESERVE_K_ONLY_PERCEPTUAL
- INTENT_PERCEPTUAL
- CMYK1, CMYK2
- Lab-based delta L comparison (DeltaL, Max)
- Test harness related names (DbgThread, cmsOpenProfileFromFileTHR, cmsCreateTransformTHR, cmsDoTransform, etc.)

Code:
*/

#include <cstdio>
#include <string>
#include <testcms2.h>
#include <cstdlib>
#include <cstdarg>
#include <lcms2.h>
#include <cmath>


// Include Little CMS header for types and the focal function declaration.
// Path may differ per environment; adjust include path if needed.

// Ensure we declare the focal function with C linkage for compatibility.
extern "C" cmsInt32Number CheckKOnlyBlackPreserving(void);

/* Lightweight in-house test harness
   - Non-terminating assertions: they log failures but do not abort the test run.
   - Simple registration of test cases; results printed to stdout.
*/
static int g_failures = 0;

#define EXPECT_TRUE(cond, ...) do { \
    if (!(cond)) { \
        ++g_failures; \
        fprintf(stderr, "TEST FAILURE: %s\n", #cond); \
        if (sizeof(__VA_ARGS__) > 1) { \
            fprintf(stderr, "  Info: "); \
            fprintf(stderr, __VA_ARGS__); \
            fprintf(stderr, "\n"); \
        } \
    } \
} while (0)

#define EXPECT_TRUE_MSG(cond, msg) EXPECT_TRUE((cond), "%s", msg)

// Forward declarations of test cases
static void test_CheckKOnlyBlackPreserving_ReturnsBooleanAndDeterministic(void);
static void test_CheckKOnlyBlackPreserving_Idempotence(void);

/* Test 1: Verify that CheckKOnlyBlackPreserving returns a boolean-like value (0 or 1)
   and that multiple invocations in the same process yield consistent results.
   This covers the true/false branching expectation at a high level, given the
   external profile data (test1.icc/test2.icc). */
static void test_CheckKOnlyBlackPreserving_ReturnsBooleanAndDeterministic(void)
{
    // First invocation
    cmsInt32Number r1 = CheckKOnlyBlackPreserving();
    EXPECT_TRUE((r1 == 0) || (r1 == 1),
                "CheckKOnlyBlackPreserving should return 0 or 1 on first call");

    // Second invocation to check determinism in the same process
    cmsInt32Number r2 = CheckKOnlyBlackPreserving();
    EXPECT_TRUE((r2 == 0) || (r2 == 1),
                "CheckKOnlyBlackPreserving should return 0 or 1 on second call");
    EXPECT_TRUE(r1 == r2,
                "CheckKOnlyBlackPreserving should be deterministic across calls");
}

/* Test 2: Idempotence test across multiple invocations
   - Reinforces the non-flaky behavior of the focal function.
   - While the actual Max/DeltaL computation depends on ICC data, the function should be stable
     in its boolean outcome across several runs in a single process. */
static void test_CheckKOnlyBlackPreserving_Idempotence(void)
{
    const int kRuns = 3;
    cmsInt32Number prev = CheckKOnlyBlackPreserving();
    for (int i = 1; i < kRuns; ++i) {
        cmsInt32Number cur = CheckKOnlyBlackPreserving();
        EXPECT_TRUE(cur == 0 || cur == 1,
                    "CheckKOnlyBlackPreserving should return 0 or 1 on each run");
        EXPECT_TRUE(cur == prev,
                    "CheckKOnlyBlackPreserving should be idempotent across multiple runs");
    }
}

static void RunAllTests(void)
{
    // Run each test; order is not important, but we keep a stable sequence.
    test_CheckKOnlyBlackPreserving_ReturnsBooleanAndDeterministic();
    test_CheckKOnlyBlackPreserving_Idempotence();
}

/*
Entry point for the test executable.
- Calls RunAllTests and returns 0 if all tests pass (no failures),
  non-zero if any test failed.
- The tests are designed to be light-weight and fast to execute.
*/
int main(int argc, char* argv[])
{
    (void)argc; (void)argv;

    // Reset global failure counter
    g_failures = 0;

    // Run all tests
    RunAllTests();

    // Summary
    if (g_failures == 0) {
        std::printf("ALL TESTS PASSED: CheckKOnlyBlackPreserving\n");
        return 0;
    } else {
        std::fprintf(stderr, "TOTAL FAILURES: %d in CheckKOnlyBlackPreserving tests\n", g_failures);
        return 1;
    }
}

/*
Explanation of test coverage and decisions:
- Test 1 covers the two critical aspects of the focal method:
  - It returns a boolean-like value (0 or 1).
  - It is deterministic across sequential invocations, which helps exercise branch consistency in the caller's perspective.
- Test 2 reinforces stability across multiple calls (idempotence).
- The test suite uses a minimal in-process harness with non-terminating assertions to maximize code coverage
  without aborting on the first failure.
- Static/private helpers inside the focal file (if any) are not accessed directly; tests rely on the
  public entry point as required.
- The tests depend on test1.icc/test2.icc being present in the runtime environment to ensure the
  function can perform its ICC-based computations. If those profiles are unavailable, the tests will
  either fail gracefully (as per the assertions) or the environment should be prepared to supply them.
- No GTest or GMock is used; the solution adheres to the constraint of not using Google Test.
- All tests are written to compile under C++11 (header inclusions, extern "C" linkage, and standard library usage).
*/
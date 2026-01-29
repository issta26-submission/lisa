// This test suite targets the UnityTestRunner function defined in the focal method.
// It exercises the selection logic (testSelected/groupSelected) and the
// orchestration of setup/testBody/teardown within UnityTestRunner.
// Note: We rely on the existing Unity test framework (unity_fixture.c/.h) to
// provide the necessary global state (Unity, UnityFixture, malloc/pointer helpers, etc.).
// The tests are written in C++11 but interact with the C-style Unity code via
// the provided C-interface (unityfunction*).

#include <unity_internals.h>
#include <cstring>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <cstdint>
#include <unity_fixture.h>


// Step 1: Candidate Keywords mapping (for traceability with the focal method)
// - UnityTestRunner: central orchestrator for running a test case with setup, testBody, and teardown.
// - UnityFixture: global fixture state controlling verbosity, filters, and repeat behavior.
// - Unity: global test state (NumberOfTests, CurrentTestName, TestFile, CurrentTestLineNumber, etc.)
// - testSelected / groupSelected: predicates invoked to decide whether a test should run.
// - setup / testBody / teardown: the lifecycle hooks invoked around the testBody.
// - UnityMalloc_StartTest / UnityMalloc_EndTest / UnityPointer_Init / UnityPointer_UndoAllSets
//   : memory and pointer state management helpers.
// - UnityConcludeFixtureTest: finalization hook after a test finishes.
// - UnityPrint / UnityPrintNumberUnsigned / UNITY_OUTPUT_CHAR: output utilities; behavior varies with UnityFixture.Verbose.
// - TEST_PROTECT: guard macro for isolating test blocks.
// - UnityCurrentTestFailed: status flag to decide whether to end allocation safely.
// - File/Line: file and line information captured by the runner.
// These keywords will guide the coverage goals for the tests.

// Step 2: Unit Test Generation
// We implement two test scenarios:
//  - TestRunSelectedExecutes: verify that when filters allow the test, setup/testBody/teardown are invoked,
//    and Unity counters/state reflect one executed test.
//  - TestRunNotSelectedSkips: verify that when filters disallow the test, no hooks are invoked and Unity
//    counters remain unchanged.

// Externally provided Unity C interface (headers provide public declarations)
extern "C" {
}

// Expose UnityTestRunner prototype if not provided by the header (most likely provided)
extern "C" void UnityTestRunner(unityfunction* setup,
                              unityfunction* testBody,
                              unityfunction* teardown,
                              const char* printableName,
                              const char* group,
                              const char* name,
                              const char* file,
                              unsigned int line);
}

// We will define C-callable test hooks that increment internal counters.
// These hooks are passed to UnityTestRunner as the lifecycle callbacks.

static int g_setup_called;
static int g_body_called;
static int g_teardown_called;

// C linkage for the hooks
extern "C" void test_setup_hook(void)
{
    g_setup_called = 1;
}
extern "C" void test_body_hook(void)
{
    g_body_called = 1;
}
extern "C" void test_teardown_hook(void)
{
    g_teardown_called = 1;
}

// Helpers to reset per-test state
static void reset_hook_counters()
{
    g_setup_called = 0;
    g_body_called = 0;
    g_teardown_called = 0;
}

// Simple non-terminating assertion helpers to collect failures without aborting
static int g_test_failed = 0;
#define ASSERT_EQ_INT(a, b) do { \
    if ((int)(a) != (int)(b)) { \
        fprintf(stderr, "ASSERT_EQ_INT failed: %d != %d (%s:%d)\n", (int)(a), (int)(b), __FILE__, __LINE__); \
        g_test_failed = 1; \
    } \
} while (0)

#define ASSERT_EQ_PTR(a, b) do { \
    if ((void*)(a) != (void*)(b)) { \
        fprintf(stderr, "ASSERT_EQ_PTR failed: %p != %p (%s:%d)\n", (void*)(a), (void*)(b), __FILE__, __LINE__); \
        g_test_failed = 1; \
    } \
} while (0)

// Test 1: Ensure that when test is selected, UnityTestRunner executes setup/testBody/teardown
void test_RunSelectedExecutes()
{
    // Prepare: ensure the test is selected by setting both filters to NULL (which means "match everything")
    // and reset hooks/state
    reset_hook_counters();
    // UnityFixture is defined in the Unity framework; we adjust its selection fields
    extern "C" struct UNITY_FIXTURE_T UnityFixture;
    // If the header exposes these, we can access; otherwise rely on default NULL (no filter)
    UnityFixture.NameFilter = nullptr;
    UnityFixture.GroupFilter = nullptr;
    UnityFixture.Verbose = 0; // not critical for this test
    UnityFixture.RepeatCount = 1;

    // The test details
    const char* printableName = "MyTestName";
    const char* group = "GroupA";
    const char* name = "TestName";
    const char* file = "unit_test.cpp";
    unsigned int line = 123;

    // Call UnityTestRunner with our hooks
    UnityTestRunner(test_setup_hook, test_body_hook, test_teardown_hook,
                    printableName, group, name, file, line);

    // Validate: the test should have run exactly once
    ASSERT_EQ_INT(g_setup_called, 1);
    ASSERT_EQ_INT(g_body_called, 1);
    ASSERT_EQ_INT(g_teardown_called, 1);

    // Validate that Unity state reflects execution
    // Unity.NumberOfTests should have incremented
    extern "C" struct UNITY_FIXTURE_T UnityFixture; // ensure access
    // Unity struct is defined in the Unity framework; access through the symbol
    // We compare against expected line to ensure the runner wrote it
    extern "C" volatile unsigned int Unity_line_check_placeholder; // dummy to avoid unused warning
    // The actual field is Unity.NumberOfTests; fetch and compare
    ASSERT_EQ_INT((int)Unity.NumberOfTests, 1);
    // And the TestFile/LineNumber should have been assigned before execution
    ASSERT_EQ_INT((int)Unity.CurrentTestLineNumber, (int)line);
    ASSERT_EQ_PTR((void*)Unity.TestFile, (void*)file);
    // CurrentTestName may be reset after verbose path; we avoid asserting its value here to remain robust
}

// Test 2: Ensure that when test is not selected (name/group mismatch), hooks are not called
void test_RunNotSelectedSkips()
{
    reset_hook_counters();

    // Set filters to force non-selection
    extern "C" struct UNITY_FIXTURE_T UnityFixture;
    // Use a non-null filter that does not match the given name
    UnityFixture.NameFilter = "DOES_NOT_MATCH"; // will not be found in "TestName"
    UnityFixture.GroupFilter = "GROUP_DOES_NOT_MATCH";

    UnityFixture.Verbose = 0;
    UnityFixture.RepeatCount = 1;

    // Call with the same test details
    const char* printableName = "IgnoredTest";
    const char* group = "GroupZ";
    const char* name = "TestName";
    const char* file = "unit_test.cpp";
    unsigned int line = 456;

    UnityTestRunner(test_setup_hook, test_body_hook, test_teardown_hook,
                    printableName, group, name, file, line);

    // Validate: hooks should not have been called
    ASSERT_EQ_INT(g_setup_called, 0);
    ASSERT_EQ_INT(g_body_called, 0);
    ASSERT_EQ_INT(g_teardown_called, 0);

    // Validate that Unity.NumberOfTests has not incremented
    ASSERT_EQ_INT((int)Unity.NumberOfTests, 0);
}

// A very light-weight runner to execute the tests in this translation unit
int main(void)
{
    g_test_failed = 0;

    fprintf(stdout, "Running UnityTestRunner unit tests (no GTest).\\n");

    // Run Test 1
    fprintf(stdout, "Test 1: Run selected executes hooks and increments counters... ");
    test_RunSelectedExecutes();
    if (g_test_failed) {
        fprintf(stdout, "FAILED\\n");
        return 1;
    } else {
        fprintf(stdout, "PASSED\\n");
    }

    g_test_failed = 0;

    // Run Test 2
    fprintf(stdout, "Test 2: Run not selected skips hooks and leaves counters untouched... ");
    test_RunNotSelectedSkips();
    if (g_test_failed) {
        fprintf(stdout, "FAILED\\n");
        return 1;
    } else {
        fprintf(stdout, "PASSED\\n");
    }

    fprintf(stdout, "All UnityTestRunner unit tests completed.\\n");
    return 0;
}
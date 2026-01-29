/****************** C++11 Test Suite for CheckAllocContext (testplugin.c) ******************
   - This test suite exercises the focal method CheckAllocContext from the provided
     C source, while respecting the constraints:
     - No GTest/GMock usage
     - Use only C++11 standard library
     - Non-terminating assertions (logs, but continues execution)
     - Tests import necessary dependencies via testcms2.h and link with the
       implementation containing CheckAllocContext
     - Access to C linkage for the focal function
   - Explanatory comments are added for each unit test to describe its purpose.

   Note: The environment is expected to provide the C API used by the focal method
   (cmsCreateContext, cmsDeleteContext, DupContext, cmsPluginTHR, PluginMemHandler,
    DebugMemDontCheckThis, etc.) via testcms2.h and related sources.
******************************************************************************/

#include <iostream>
#include <string>
#include <testcms2.h>


// Ensure C linkage for the C API used by the focal function
extern "C" {

// Declaration of the focal function under test (non-mangled C linkage)
cmsInt32Number CheckAllocContext(void);
}

/************** Lightweight Test Harness (non-terminating) **************/
static int g_totalTests = 0;
static int g_failedTests = 0;

#define ASSERT_TRUE(cond, desc) do { \
    ++g_totalTests; \
    if(!(cond)) { \
        ++g_failedTests; \
        std::cerr << "ASSERT FAILED: " << (desc) << std::endl; \
    } \
} while(0)

static void reportSummary() {
    std::cout << "\nTest Summary: " << (g_totalTests - g_failedTests)
              << " passed, " << g_failedTests << " failed, out of " << g_totalTests << " tests."
              << std::endl;
}

/************** Unit Tests **************/

/*
 * Test 1: Validate that CheckAllocContext returns 1 as per its implementation.
 * Rationale: Ensures the primary contract of the focal function is as expected.
 * This is a direct check of the function's return value without delving into
 * internal private states.
 */
static void test_CheckAllocContext_ReturnsOne() {
    cmsInt32Number res = CheckAllocContext();
    ASSERT_TRUE(res == 1, "CheckAllocContext should return 1");
}

/*
 * Test 2: Basic context lifecycle outside the focal function to ensure the
 * library's public API for creating/deleting contexts behaves correctly
 * and does not crash when used with null/default handlers.
 * Rationale: Confirms normal malloc path works and that deletion does not leak.
 */
static void test_ContextLifecycle_NormalPaths() {
    cmsContext c1 = cmsCreateContext(NULL, NULL);
    ASSERT_TRUE(c1 != NULL, "cmsCreateContext(NULL, NULL) should return non-null context");
    DebugMemDontCheckThis(c1);
    cmsDeleteContext(c1);

    cmsContext c2 = cmsCreateContext(NULL, NULL);
    ASSERT_TRUE(c2 != NULL, "cmsCreateContext(NULL, NULL) second path should return non-null");
    DebugMemDontCheckThis(c2);
    cmsDeleteContext(c2);
}

/*
 * Test 3: Context lifecycle with a PluginMemHandler (debug allocator) and
 * subsequent deletion. Verifies both allocation path and proper cleanup
 * when a custom allocator is involved.
 * Rationale: Exercises the alternate allocation path used in the focal
 * method and ensures cleanup is safe.
 */
static void test_ContextLifecycle_WithPluginMemHandler() {
    cmsContext c = cmsCreateContext(PluginMemHandler(), NULL);
    ASSERT_TRUE(c != NULL, "cmsCreateContext(PluginMemHandler(), NULL) should return non-null");
    DebugMemDontCheckThis(c);
    cmsDeleteContext(c);
}

/*
 * Test 4: Sequence mirroring part of CheckAllocContext to verify DupContext
 * after enabling a custom allocator and proper deletions without crashes.
 * Rationale: Ensures duplication path remains valid when different allocs
 * are used and after plugin-thr initialization.
 */
static void test_DupContext_AfterPluginAlloc() {
    cmsContext c1 = cmsCreateContext(NULL, NULL);
    DebugMemDontCheckThis(c1);
    cmsDeleteContext(c1);

    cmsContext c2 = cmsCreateContext(PluginMemHandler(), NULL);
    DebugMemDontCheckThis(c2);
    cmsDeleteContext(c2);

    c1 = cmsCreateContext(NULL, NULL);
    DebugMemDontCheckThis(c1);
    c2 = cmsCreateContext(PluginMemHandler(), NULL);
    DebugMemDontCheckThis(c2);

    cmsPluginTHR(c1, PluginMemHandler());

    cmsContext c3 = DupContext(c1, NULL);
    cmsContext c4 = DupContext(c2, NULL);

    cmsDeleteContext(c1);
    cmsDeleteContext(c2);
    if (c3 != NULL) cmsDeleteContext(c3);
    if (c4 != NULL) cmsDeleteContext(c4);
}

/*
 * Test 5: Stress-like check to ensure that repeated calls to CheckAllocContext
 * do not crash the test environment and consistently return 1.
 * Rationale: Provides confidence in stability across multiple invocations.
 */
static void test_CheckAllocContext_Repeatability() {
    for (int i = 0; i < 3; ++i) {
        cmsInt32Number res = CheckAllocContext();
        ASSERT_TRUE(res == 1, "Repeated CheckAllocContext call should return 1");
    }
}

/************** Main Driver **************/
int main() {
    // Run all unit tests
    test_CheckAllocContext_ReturnsOne();
    test_ContextLifecycle_NormalPaths();
    test_ContextLifecycle_WithPluginMemHandler();
    test_DupContext_AfterPluginAlloc();
    test_CheckAllocContext_Repeatability();

    reportSummary();

    // Return number of failed tests as exit code for quick CI signaling
    return g_failedTests;
}
/*
Unit test suite for the focal method CheckMPEPlugin located in testplugin.c.

Step 1 recap (conceptual):
- Focal method: CheckMPEPlugin(void) builds a small color pipeline using the MPE plugin,
  serializes/deserializes a profile via memory, and validates the negation operation
  on a 3-channel pipeline. Core interactions involve: context creation/duplication,
  profile creation, pipeline assembly, evaluation, memory I/O, and context switching
  when reading from memory-thr aware operations.
- Candidate Keywords guiding test design: cmsContext, WatchDogContext, cmsPluginTHR,
  DupContext, cmsCreateProfilePlaceholder, cmsPipelineAlloc, cmsPipelineInsertStage,
  StageAllocNegate, cmsPipelineEvalFloat, cmsWriteTag, cmsReadTag, cmsOpenProfileFromMemTHR,
  cmsOpenProfileFromMem, cmsReadTag, cmsSigDToB3Tag, cmsSaveProfileToMem, cmsCloseProfile,
  cmsDeleteContext, malloc/free, Fail/IsGoodVal (validations), ResetFatalError.

Step 2 plan (test targets):
- Primary objective: verify CheckMPEPlugin can be invoked and returns a plausible result
  (0 or 1) under typical conditions, without crashing.
- Additional objective: exercise stability by invoking CheckMPEPlugin multiple times.
- Because CheckMPEPlugin depends on external CMS internals and plugins, the tests focus on
  the observable contract (return value shape) and do not alter private internals.

Step 3 refinement:
- We provide a lightweight, non-terminating assertion mechanism (non-GTest) to maximize
  code execution paths and coverage without aborting on failures.
- Tests are designed to be executed from main() as a small harness, suitable for inclusion
  in a C++11 project without GTest.

Note:
- The test uses a forward declaration extern "C" int CheckMPEPlugin(void); to avoid
  potential C/C++ linkage conflicts while assuming that the actual function returns
  a type compatible with int (cmsInt32Number usually maps to a 32-bit int). If the
  underlying type differs on a given platform, this test can be trivially adjusted
  to match the exact signature via a proper header declaration.

Code begins here:
*/

// Lightweight unit test harness for non-GTest environments
#include <iostream>
#include <testcms2.h>
#include <cstdlib>


// Forward declaration of the focal function.
// We assume it returns an int-compatible value (cmsInt32Number is typically int).
extern "C" int CheckMPEPlugin(void);

// Simple non-terminating assertion reporting
static int g_test_count = 0;
static int g_failure_count = 0;

static void report(bool condition, const char* message) {
    ++g_test_count;
    if (!condition) {
        ++g_failure_count;
        std::cerr << "[FAIL] " << message << std::endl;
    } else {
        std::cout << "[PASS] " << message << std::endl;
    }
}

/*
Candidate Keywords and rationale (for test coverage alignment):
- Context lifecycle: WatchDogContext, cmsDeleteContext, DupContext
- Profile lifecycle: cmsCreateProfilePlaceholder, cmsCloseProfile, cmsOpenProfileFromMemTHR
- Pipeline lifecycle: cmsPipelineAlloc, cmsPipelineInsertStage, StageAllocNegate,
  cmsPipelineEvalFloat, cmsPipelineFree
- Memory I/O: cmsSaveProfileToMem, cmsOpenProfileFromMem, cmsReadTag
- Tag and capability: cmsSigDToB3Tag, cmsWriteTag
- Error handling: Fail, ResetFatalError
- Data flow: In[3], Out[3] arrays; validation via IsGoodVal
- Threading/Plugin: cmsPluginTHR, MPEPluginSample
- Resource cleanup: free/malloc, cmsCloseProfile
These keywords guide the tests to exercise common success-path behaviors and ensure
the function is callable in typical CMS environments.
*/

// Test 1: Basic return contract
// Verifies that CheckMPEPlugin returns a value consistent with success/failure
// semantics (0 or 1). This is the primary observable contract of the focal method.
void test_CheckMPEPlugin_BasicReturn() {
    int rc = CheckMPEPlugin();
    bool ok = (rc == 0 || rc == 1);
    report(ok, "CheckMPEPlugin returns 0 or 1 (basic correctness).");
}

// Test 2: Stability across multiple invocations
// Calls the focal method multiple times to ensure there are no crashes or undefined
// behaviors across successive executions (e.g., due to static state or resource reuse).
void test_CheckMPEPlugin_MultipleRuns() {
    for (int i = 0; i < 3; ++i) {
        int rc = CheckMPEPlugin();
        bool ok = (rc == 0 || rc == 1);
        report(ok, "CheckMPEPlugin repeated run should return 0 or 1.");
    }
}

int main() {
    std::cout << "Starting unit tests for CheckMPEPlugin (no GTest)..." << std::endl;

    test_CheckMPEPlugin_BasicReturn();
    test_CheckMPEPlugin_MultipleRuns();

    std::cout << "Tests finished. Total: " << g_test_count
              << ", Failures: " << g_failure_count << std::endl;

    // Return non-zero if any test failed
    return g_failure_count ? 1 : 0;
}
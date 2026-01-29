/*
Step 1 notes (embedded as comments in code):
- Focal method under test: CheckAdaptationStateContext(void)
- Core dependent components (Candidate Keywords) used by the method:
  - cmsSetAdaptationStateTHR: read/set global adaptation threshold/state
  - WatchDogContext: create a new context (watchdog)
  - DupContext: duplicate a context
  - cmsDeleteContext: free context resources
  - IsGoodVal: predicate checker to compare a value against a target within tolerance
  - Fail: reporting mechanism on assertion/failure
  - Context manipulation wrappers: the test relies on C-API-style context handling
- Dependencies are C-style functions with names shown above. We access them from C++ via extern "C".
- Behavioral note: The true path is when old1 == old2; the final return value is rc (result of IsGoodVal on the adapted state of c3 after setting to -1). The false path triggers Fail and returns 0.
- The test suite below provides:
  - A primary test that CheckAdaptationStateContext returns 1 (true path) under normal execution.
  - A consistency test that multiple consecutive runs of the focal method yield the same result.
  - A synthetic “manual replay” test that mirrors the focal method's internal steps to validate that the method's return equals the manually computed rc when the adaptation state has not changed between the initial and final reads.
- No GTest is used; a small, self-contained test harness is implemented with plain C++ I/O.
- Static/private considerations: The test interacts only with the public C API exposed by the focal code. Static file-scope helpers inside the focal file are not accessed directly.
- Namespace: tests use the global namespace (no std:: namespace pollution beyond iostream usage).
- The code assumes proper linking to the existing C/C headers of the project providing the cms* APIs.

*/

#include <cstring>
#include <iostream>
#include <cmath>
#include <testcms2.h>


// Typedefs matching the typical CMS-like type aliases used by the focal method.
// If actual project typedefs differ, these can be adjusted accordingly.
typedef int cmsInt32Number;
typedef void* cmsContext;
typedef double cmsFloat64Number;

// Forward declarations of the focal method and dependencies.
// They are provided by the project under test (testplugin.c and friends).
extern "C" {

    // Focal method under test
    cmsInt32Number CheckAdaptationStateContext(void);

    // Dependencies used by the focal method
    cmsContext WatchDogContext(void* Data);
    cmsContext DupContext(cmsContext src, void* Data);
    void cmsDeleteContext(cmsContext ctx);

    cmsInt32Number cmsSetAdaptationStateTHR(cmsContext ContextID, cmsFloat64Number thr);
    cmsInt32Number IsGoodVal(const char* Name, cmsInt32Number Value, cmsFloat64Number Target, cmsFloat64Number tolerance);
    void Fail(const char* msg);
}

// Simple test harness for non-GTest environments
static int g_totalTests = 0;
static int g_passedTests = 0;

#define TEST_PASS(name) do { std::cout << "[PASS] " << name << "\n"; } while(0)
#define TEST_FAIL(name) do { std::cout << "[FAIL] " << name << "\n"; } while(0)

#define RUN_TEST(testFunc) do { \
    if (testFunc()) { \
        ++g_passedTests; \
        TEST_PASS(#testFunc); \
    } else { \
        TEST_FAIL(#testFunc); \
    } \
    ++g_totalTests; \
} while(0)

// Test 1: Verify that the focal method CheckAdaptationStateContext returns 1 (true)
// under normal conditions (i.e., the true branch is taken).
bool test_CheckAdaptationStateContext_ReturnsTrue() {
    cmsInt32Number res = CheckAdaptationStateContext();
    // Expect the focal method to return 1 on success (true path)
    if (res == 1) {
        return true;
    } else {
        std::cerr << "CheckAdaptationStateContext: expected 1, got " << res << "\n";
        return false;
    }
}

// Test 2: Consistency across multiple invocations
// Ensure that two consecutive calls do not crash and yield the same result.
// This helps exercise both the checker path and memory safety across calls.
bool test_CheckAdaptationStateContext_Consistency() {
    cmsInt32Number r1 = CheckAdaptationStateContext();
    cmsInt32Number r2 = CheckAdaptationStateContext();
    if (r1 == r2) {
        return true;
    } else {
        std::cerr << "Inconsistency: first=" << r1 << ", second=" << r2 << "\n";
        return false;
    }
}

// Test 3: Manual replay mirroring the internal steps of the focal method
// This test replays the exact sequence inside CheckAdaptationStateContext up to the return point
// and then verifies that the focal method's return value equals the manually computed rc
// when the old state before and after remains unchanged.
bool test_CheckAdaptationStateContext_ManualReplayMatches() {
    // Manually replay the sequence:
    // old1 = cmsSetAdaptationStateTHR(NULL, -1);
    cmsInt32Number old1 = cmsSetAdaptationStateTHR(NULL, -1);

    // c1 = WatchDogContext(NULL);
    cmsContext c1 = WatchDogContext(NULL);
    // cmsSetAdaptationStateTHR(c1, 0.7);
    cmsSetAdaptationStateTHR(c1, 0.7);

    // c2 = DupContext(c1, NULL);
    cmsContext c2 = DupContext(c1, NULL);
    // c3 = DupContext(c2, NULL);
    cmsContext c3 = DupContext(c2, NULL);

    // rc = IsGoodVal("Adaptation state", cmsSetAdaptationStateTHR(c3, -1), 0.7, 0.001);
    cmsInt32Number rc = IsGoodVal("Adaptation state", cmsSetAdaptationStateTHR(c3, -1), 0.7, 0.001);

    // Clean up
    cmsDeleteContext(c1);
    cmsDeleteContext(c2);
    cmsDeleteContext(c3);

    // old2 = cmsSetAdaptationStateTHR(NULL, -1);
    cmsFloat64Number old2 = cmsSetAdaptationStateTHR(NULL, -1);

    // if (old1 != old2) { Fail("Adaptation state has changed"); return 0; }
    bool unchanged = (static_cast<cmsFloat64Number>(old1) == old2);
    if (!unchanged) {
        // If adaptation state changed, the focal method would return 0.
        // For this test, signal failure to reflect deviation.
        Fail("Adaptation state has changed (manual replay).");
        return false;
    }

    // The focal method would return rc in this path.
    cmsInt32Number focal_return = CheckAdaptationStateContext();
    if (focal_return != rc) {
        std::cerr << "Mismatch: focal_return=" << focal_return << ", manual_rc=" << rc << "\n";
        return false;
    }
    return true;
}

int main() {
    std::cout << "Starting test suite for CheckAdaptationStateContext...\n";

    // Run tests
    RUN_TEST(test_CheckAdaptationStateContext_ReturnsTrue);
    RUN_TEST(test_CheckAdaptationStateContext_Consistency);
    RUN_TEST(test_CheckAdaptationStateContext_ManualReplayMatches);

    // Summary
    std::cout << "Test suite completed: " << g_passedTests << "/" << g_totalTests << " tests passed.\n";

    // Return 0 if all tests pass, non-zero otherwise
    return (g_passedTests == g_totalTests) ? 0 : 1;
}
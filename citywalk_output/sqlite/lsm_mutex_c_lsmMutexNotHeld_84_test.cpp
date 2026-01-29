// This test suite targets the focal function:
// int lsmMutexNotHeld(lsm_env *pEnv, lsm_mutex *pMutex) { return pEnv->xMutexNotHeld ? pEnv->xMutexNotHeld(pMutex) : 1; }
// It relies on the project-provided types (lsm_env, lsm_mutex) and the function
// lsmMutexNotHeld declared in the same project (likely in C code). The tests are
// written in C++11 (no GoogleTest) and use a lightweight, non-terminating
// assertion approach as requested in the domain knowledge.

// Include project header to get correct type definitions (lsm_env, lsm_mutex, etc.)
#include <lsmInt.h>
#include <iostream>
#include <string>


// The project should provide this header with the proper typedefs for lsm_env and lsm_mutex.
// If your project layout differs, adjust the include path accordingly.

// Ensure we can link with the C implementation of lsmMutexNotHeld
extern "C" int lsmMutexNotHeld(lsm_env *pEnv, lsm_mutex *pMutex);

// Forward-declare a test callback matching the expected signature
// (int (*)(lsm_mutex *)) used by lsmMutexNotHeld when xMutexNotHeld is non-null.
static int testMutexNotHeldCallback(lsm_mutex *pMutex);

// Global test state to verify callback behavior
static bool g_callbackInvoked = false;
static lsm_mutex *g_callbackMutexPtr = nullptr;

// Implementation of the test callback that lsmMutexNotHeld will call
// when pEnv->xMutexNotHeld is non-null. It returns a deterministic value (e.g., 7)
// to verify that the return value from lsmMutexNotHeld matches the callback's result
// and that the correct pointer was forwarded.
static int testMutexNotHeldCallback(lsm_mutex *pMutex) {
    g_callbackInvoked = true;
    g_callbackMutexPtr = pMutex;
    return 7;  // Chosen arbitrary value for testing
}

// Simple non-terminating test reporter
static int g_totalFailures = 0;

static void reportTestResult(const std::string &testName, bool passed, const std::string &details = "") {
    if (passed) {
        std::cout << "[PASSED] " << testName << std::endl;
    } else {
        std::cout << "[FAILED] " << testName;
        if (!details.empty()) {
            std::cout << " - " << details;
        }
        std::cout << std::endl;
        ++g_totalFailures;
    }
}

// Test 1: When xMutexNotHeld is null, lsmMutexNotHeld should return 1 (default behavior)
static void test_lsmMutexNotHeld_nullCallback() {
    const std::string testName = "lsmMutexNotHeld should return 1 when xMutexNotHeld is null";

    // Arrange
    lsm_env env;
    env.xMutexNotHeld = nullptr;  // No callback provided

    lsm_mutex m;
    // Act
    int result = lsmMutexNotHeld(&env, &m);

    // Assert
    bool passed = (result == 1);
    reportTestResult(testName, passed,
        passed ? "" : "Expected 1, got " + std::to_string(result));
}

// Test 2: When xMutexNotHeld is provided, the callback should be invoked and its return value forwarded
// Additionally, the lsm_mutex pointer passed to the callback should be the same as provided.
static void test_lsmMutexNotHeld_callbackInvocation() {
    const std::string testName = "lsmMutexNotHeld should invoke non-null callback and forward mutex pointer";

    // Arrange
    g_callbackInvoked = false;
    g_callbackMutexPtr = nullptr;

    lsm_env env;
    env.xMutexNotHeld = testMutexNotHeldCallback;  // Provide the callback

    lsm_mutex m;
    // Act
    int result = lsmMutexNotHeld(&env, &m);

    // Assert
    bool callbackWasInvoked = g_callbackInvoked;
    bool pointerForwardedCorrectly = (g_callbackMutexPtr == &m);
    bool returnValueForwarded = (result == 7); // As defined in testMutexNotHeldCallback

    std::string details;
    if (!callbackWasInvoked) details += "Callback not invoked. ";
    if (!pointerForwardedCorrectly) details += "Pointer not forwarded correctly. ";
    if (!returnValueForwarded) details += "Return value not forwarded correctly.";

    reportTestResult(testName, callbackWasInvoked && pointerForwardedCorrectly && returnValueForwarded, details);
}

// Test 3: Verify that multiple mutex instances correctly forward to the callback
// This ensures the function does not cache any state and always forwards the provided pointer.
static void test_lsmMutexNotHeld_callbackForMultipleMutexes() {
    const std::string testName = "lsmMutexNotHeld should correctly forward multiple mutex instances";

    // Arrange
    g_callbackInvoked = false;
    g_callbackMutexPtr = nullptr;

    lsm_env env;
    env.xMutexNotHeld = testMutexNotHeldCallback;

    lsm_mutex m1{};
    lsm_mutex m2{};

    // Act: first call with m1
    int res1 = lsmMutexNotHeld(&env, &m1);

    // Check first call
    bool firstCallOK =
        g_callbackInvoked && g_callbackMutexPtr == &m1 && res1 == 7;

    // Reset for second call
    g_callbackInvoked = false;
    g_callbackMutexPtr = nullptr;

    // Act: second call with m2
    int res2 = lsmMutexNotHeld(&env, &m2);

    // Assert second call
    bool secondCallOK =
        g_callbackInvoked && g_callbackMutexPtr == &m2 && res2 == 7;

    std::string details;
    if (!firstCallOK) details += "First call failed to forward correct mutex or return value. ";
    if (!secondCallOK) details += "Second call failed to forward correct mutex or return value.";

    reportTestResult(testName, firstCallOK && secondCallOK, details);
}

// Entry point to run all tests
int main() {
    std::cout << "Starting lsmMutexNotHeld unit tests (C++ harness, no GTest)\n";
    test_lsmMutexNotHeld_nullCallback();
    test_lsmMutexNotHeld_callbackInvocation();
    test_lsmMutexNotHeld_callbackForMultipleMutexes();

    if (g_totalFailures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << g_totalFailures << " test(s) FAILED\n";
        return 1;
    }
}
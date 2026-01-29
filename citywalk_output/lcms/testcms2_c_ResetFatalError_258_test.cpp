// test_ResetFatalError.cpp
// A self-contained unit-test harness (C++11) for the focal method ResetFatalError.
// This test does not rely on Google Test or any external testing framework.
// It uses simple assertions and prints PASS/FAIL to stdout.
// The test relies on the behavior that ResetFatalError sets the CMS log error
// handler to FatalErrorQuit. We provide lightweight mock implementations of
// the relevant C APIs to introspect the effect of ResetFatalError without
// triggering real fatal exits.

// Important: The real project (testcms2.c) defines ResetFatalError and a number of
// helper stubs. This test is written to be compatible with that environment by
// modeling the expected interaction:
//   - cmsSetLogErrorHandler takes a function pointer with a signature:
//       void (*)(cmsContext, cmsUInt32Number, const char*)
//   - FatalErrorQuit has the corresponding signature
//   - ResetFatalError() calls cmsSetLogErrorHandler(FatalErrorQuit)

#include <iostream>
#include <string>
#include <cassert>
#include <testcms2.h>


// Domain-specific types approximated for testing. These must match the real
// project typedefs used by the focal code. If the real project uses different
// typedefs, adjust accordingly.
typedef void* cmsContext;
typedef unsigned int cmsUInt32Number;

// Forward declaration of the focal method under test.
// In the actual build, link with the file that defines ResetFatalError (from testcms2.c).
extern "C" void ResetFatalError(void);

// Forward declarations for the CMS API used by the focal method.
// We provide lightweight mocks so we can introspect the behavior without
// triggering real fatal exits during tests.
extern "C" {
    // Pointer type for the log-error handler
    typedef void (* cmsLogErrorHandler)(cmsContext, cmsUInt32Number, const char*);

    // Mocked CMS API: stores the current error handler pointer
    void cmsSetLogErrorHandler(cmsLogErrorHandler Handler);

    // The FatalErrorQuit function used by the focal method.
    void FatalErrorQuit(cmsContext ContextID, cmsUInt32Number ErrorCode, const char *Text);

    // For the tests, we expose a way to read back the current handler pointer.
    // If the real library has cmsGetLogErrorHandler, you can replace this with
    // a direct call. Otherwise, rely on g_CurrentHandler in this test.
    cmsLogErrorHandler cmsGetCurrentLogErrorHandler(void);
}

// Global state kept by the mock to verify interactions
static cmsLogErrorHandler g_CurrentHandler = nullptr;

// Extra captured data from invoking the fatal-error path
static bool g_LastFatalInvoked = false;
static const char* g_LastFatalText = nullptr;
static cmsContext g_LastFatalContext = nullptr;
static cmsUInt32Number g_LastFatalCode = 0;

// Lightweight mock implementations

extern "C" {

void cmsSetLogErrorHandler(cmsLogErrorHandler Handler) {
    // Store the handler so tests can verify what was installed
    g_CurrentHandler = Handler;
}

// Simple deterministic implementation of the fatal error handler for tests.
// It does not terminate the process (as per the stub in the provided class
// dependency list). Instead, it records invocation data for assertions.
void FatalErrorQuit(cmsContext ContextID, cmsUInt32Number ErrorCode, const char *Text) {
    g_LastFatalInvoked = true;
    g_LastFatalContext = ContextID;
    g_LastFatalCode = ErrorCode;
    g_LastFatalText = Text;
}
}

// Accessor to query the installed handler (for test assertions)
extern "C" cmsLogErrorHandler cmsGetCurrentLogErrorHandler(void) {
    return g_CurrentHandler;
}

// Helper to reset test state between cases
static void ResetTestState() {
    g_CurrentHandler = nullptr;
    g_LastFatalInvoked = false;
    g_LastFatalText = nullptr;
    g_LastFatalContext = nullptr;
    g_LastFatalCode = 0;
}

// Test 1: Ensure ResetFatalError installs FatalErrorQuit as the log error handler
static void Test_ResetFatalError_InstallsFatalErrorQuit() {
    ResetTestState();

    // Call the function under test
    ResetFatalError();

    // The current handler should be the FatalErrorQuit function.
    cmsLogErrorHandler installed = cmsGetCurrentLogErrorHandler();
    if (installed != FatalErrorQuit) {
        std::cerr << "Test 1 FAILED: ResetFatalError did not install FatalErrorQuit as the log error handler.\n";
        std::cerr << "Expected: " << reinterpret_cast<void*>(reinterpret_cast<long long>(&FatalErrorQuit))
                  << " Got: " << reinterpret_cast<void*>(reinterpret_cast<long long>(installed)) << "\n";
        std::exit(EXIT_FAILURE);
    }

    // Clean state for next tests
    ResetTestState();

    std::cout << "Test 1 PASSED: ResetFatalError installs FatalErrorQuit as the log error handler.\n";
}

// Test 2: Ensure the installed handler is indeed invoked and carries correct data
// when called through the function pointer installed by ResetFatalError.
static void Test_ResetFatalError_HandlerInvocation() {
    ResetTestState();

    // Install handler via the function under test
    ResetFatalError();

    // If the handler is correctly installed, we should be able to invoke it
    // and have FatalErrorQuit record its parameters without terminating.
    cmsLogErrorHandler installed = cmsGetCurrentLogErrorHandler();
    if (installed == nullptr) {
        std::cerr << "Test 2 FAILED: No log error handler installed by ResetFatalError.\n";
        std::exit(EXIT_FAILURE);
    }

    // Prepare fake context and error data
    void* fakeContext = (void*)0xDEADBEEF;
    cmsUInt32Number fakeCode = 0xABCD;
    const char* fakeText = "Fatal error triggered by test";

    // Invoke the handler directly to simulate a fatal event.
    installed(fakeContext, fakeCode, fakeText);

    // Assertions on captured data by FatalErrorQuit
    if (!g_LastFatalInvoked) {
        std::cerr << "Test 2 FAILED: FatalErrorQuit was not invoked by the handler.\n";
        std::exit(EXIT_FAILURE);
    }
    if (g_LastFatalContext != fakeContext) {
        std::cerr << "Test 2 FAILED: FatalErrorQuit received incorrect ContextID.\n";
        std::exit(EXIT_FAILURE);
    }
    if (g_LastFatalCode != fakeCode) {
        std::cerr << "Test 2 FAILED: FatalErrorQuit received incorrect ErrorCode.\n";
        std::exit(EXIT_FAILURE);
    }
    if (g_LastFatalText == nullptr || std::string(g_LastFatalText) != std::string(fakeText)) {
        std::cerr << "Test 2 FAILED: FatalErrorQuit received incorrect Text.\n";
        std::exit(EXIT_FAILURE);
    }

    std::cout << "Test 2 PASSED: Invoking the installed handler calls FatalErrorQuit with correct data.\n";
}

// Main test runner
int main() {
    // Each test prints PASS/FAIL and exits on failure to provide clear CI feedback.
    Test_ResetFatalError_InstallsFatalErrorQuit();
    Test_ResetFatalError_HandlerInvocation();

    std::cout << "ALL TESTS PASSED: ResetFatalError unit tests completed successfully.\n";
    return 0;
}

/*
Notes for maintainers and reviewers:
- Step 1 (Program Understanding) yields that ResetFatalError executes:
    cmsSetLogErrorHandler(FatalErrorQuit);
  The core dependencies revolve around the CMS' logging/error handling mechanism
  (FatalErrorQuit is the handler function).
  Candidate Keywords identified: ResetFatalError, cmsSetLogErrorHandler, FatalErrorQuit,
  cmsContext, cmsUInt32Number.

- Step 2 (Unit Test Generation) focuses on testing that:
  - The handler is installed (true branch): cmsSetLogErrorHandler is called with FatalErrorQuit.
  - The installed handler, when invoked, routes through FatalErrorQuit with the right data
    (true path for parameters). A false/invalid path would be covered by separate tests if
    behavior differs (not applicable here since the helper is a simple assignment).

- Step 3 (Test Case Refinement) uses:
  - Bare-bones assertions (no GTest) to maximize executable path coverage.
  - Simple state capture of the handler and the FatalErrorQuit invocation to verify
    correctness of static behavior without coupling to library internals.
  - Ensures compatibility with C-style APIs and standard C++ library (string for text
    comparison, basic i/o for reporting).

- Static analysis considerations:
  - This test is designed to be compiled alongside the focal codebase. If the real
    environment provides a different signature for cmsSetLogErrorHandler, adjust
    the typedefs (cmsContext, cmsUInt32Number) and the function prototype in the
    extern "C" block accordingly.
  - If the project uses a real cmsGetCurrentLogErrorHandler() accessor, you can
    replace the mock with the real API to validate the same properties.
*/
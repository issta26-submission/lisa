// Test suite for ErrorReportingFunction in testcms2.c
// This test suite is written for C++11 compatibility without Google Test.
// It relies on the external C declarations provided by testcms2.h.
// The tests exercise the visible side effects of ErrorReportingFunction:
// - TrappedError is set to TRUE (non-zero) on call
// - SimultaneousErrors is incremented
// - ReasonToFailBuffer is populated with the provided Text (truncated to TEXT_ERROR_BUFFER_SIZE-1 by strncpy)

#include <cstring>
#include <iostream>
#include <string>
#include <testcms2.h>


// Bring in C declarations from the project under test
extern "C" {
    // Forward declaration of the focal function (C linkage)
    void ErrorReportingFunction(cmsContext ContextID, cmsUInt32Number ErrorCode, const char *Text);
}

// The project is expected to define these globals/types in testcms2.h
// We declare them here as extern so tests can inspect their side effects.
// We rely on the fact that in typical LittleCMS style cmsBool is typedef int and
// TRUE/FALSE are 1/0, which keeps extern int compatible.
extern int TrappedError;                   // cmsBool-like flag set to TRUE on error
extern int SimultaneousErrors;              // counter incremented per error
extern char ReasonToFailBuffer[];            // destination buffer for Text
// TEXT_ERROR_BUFFER_SIZE is typically a macro; use it directly in tests where needed
// We do not declare TEXT_ERROR_BUFFER_SIZE as a variable; it's assumed to be a macro.


// Lightweight test harness (non-GTest) with non-terminating checks
static int g_totalFailures = 0;

// Helper to report per-test failures without terminating execution
static void report_failure(const char* testName, const char* message) {
    std::cerr << "[FAIL] " << testName << ": " << message << std::endl;
    ++g_totalFailures;
}

// Test 1: Basic invocation should set TrappedError, increment SimultaneousErrors, and copy text
static void test_ErrorReportingFunction_BasicInvocation() {
    const char* testName = "test_ErrorReportingFunction_BasicInvocation";

    // Reset global state before test
    TrappedError = 0;                // assume 0 means FALSE
    SimultaneousErrors = 0;
    ReasonToFailBuffer[0] = '\0';

    const char* inputText = "HelloWorld";
    ErrorReportingFunction(0, 0, inputText);

    bool ok = true;
    if (TrappedError == 0) {
        report_failure(testName, "TrappedError was not set to TRUE after call.");
        ok = false;
    }
    if (SimultaneousErrors != 1) {
        report_failure(testName, "SimultaneousErrors was not incremented to 1 after first call.");
        ok = false;
    }
    if (std::strcmp(ReasonToFailBuffer, inputText) != 0) {
        report_failure(testName, "ReasonToFailBuffer does not contain the input text as expected.");
        ok = false;
    }

    if (ok) {
        std::cout << "[OK] " << testName << std::endl;
    }
}

// Test 2: Long text truncation behavior should copy only TEXT_ERROR_BUFFER_SIZE-1 characters
static void test_ErrorReportingFunction_TextTruncation() {
    const char* testName = "test_ErrorReportingFunction_TextTruncation";

    // Reset global state before test
    TrappedError = 0;
    SimultaneousErrors = 0;
    ReasonToFailBuffer[0] = '\0';

    // Build a long input longer than TEXT_ERROR_BUFFER_SIZE-1
    // TEXT_ERROR_BUFFER_SIZE is a macro defined in the C file; we rely on it directly.
    std::string longText(TEXT_ERROR_BUFFER_SIZE + 50, 'A'); // longer than buffer
    ErrorReportingFunction(0, 0, longText.c_str());

    bool ok = true;
    if (TrappedError == 0) {
        report_failure(testName, "TrappedError was not set to TRUE after long input.");
        ok = false;
    }
    if (SimultaneousErrors != 1) {
        report_failure(testName, "SimultaneousErrors should be 1 after a single call with long text.");
        ok = false;
    }

    // Since strncpy was used with size TEXT_ERROR_BUFFER_SIZE-1, there is no guaranteed null-termination.
    // We compare the first TEXT_ERROR_BUFFER_SIZE-1 bytes to the corresponding portion of longText.
    if (std::memcmp(ReasonToFailBuffer, longText.data(), TEXT_ERROR_BUFFER_SIZE - 1) != 0) {
        report_failure(testName, "ReasonToFailBuffer's initial bytes do not match the input truncation.");
        ok = false;
    }

    if (ok) {
        std::cout << "[OK] " << testName << std::endl;
    }
}

// Test 3: Multiple invocations should accumulate SimultaneousErrors accordingly
static void test_ErrorReportingFunction_MultipleInvocations() {
    const char* testName = "test_ErrorReportingFunction_MultipleInvocations";

    // Reset global state before test
    TrappedError = 0;
    SimultaneousErrors = 0;
    ReasonToFailBuffer[0] = '\0';

    // First call
    ErrorReportingFunction(0, 0, "First");
    // Second call
    ErrorReportingFunction(0, 0, "Second");
    // Third call
    ErrorReportingFunction(0, 0, "Third");

    bool ok = true;
    if (TrappedError == 0) {
        report_failure(testName, "TrappedError should remain TRUE (non-zero) after successive calls.");
        ok = false;
    }
    if (SimultaneousErrors != 3) {
        report_failure(testName, "SimultaneousErrors should be 3 after three calls.");
        ok = false;
    }

    if (ok) {
        std::cout << "[OK] " << testName << std::endl;
    }
}


// Main function: run all tests and report summary
int main() {
    std::cout << "Starting ErrorReportingFunction unit tests (CMS context)..." << std::endl;

    test_ErrorReportingFunction_BasicInvocation();
    test_ErrorReportingFunction_TextTruncation();
    test_ErrorReportingFunction_MultipleInvocations();

    if (g_totalFailures == 0) {
        std::cout << "[ALL TESTS PASSED] " << std::endl;
        return 0;
    } else {
        std::cerr << "[TESTS FAILED] Total failures: " << g_totalFailures << std::endl;
        return 1;
    }
}
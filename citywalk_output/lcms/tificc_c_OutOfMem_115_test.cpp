// Test suite for the focal method OutOfMem in tificc.c
// This test uses a simple in-process test harness (no GTest) and a
// function-interposition approach to capture FatalError calls.
// Important: To make these tests work in your environment, link order must
// ensure the test's FatalError definition is used (e.g., g++ test.o tificc.o -o test).

#include <cstdio>
#include <lcms2_plugin.h>
#include <iostream>
#include <string>
#include <stdint.h>
#include <cstdarg>
#include <utils.h>
#include <tiffio.h>


// Domain knowledge and environment notes:
// - The focal function is: void OutOfMem(cmsUInt32Number size)
// - It calls FatalError("Out of memory on allocating %d bytes.", size);
// - We test that FatalError gets invoked with the correctly formatted message.
// - We assume cmsUInt32Number is 32-bit unsigned; for the test we map it to unsigned int.

typedef unsigned int cmsUInt32Number;

// Forward declaration of the focal function from tificc.c
extern "C" void OutOfMem(cmsUInt32Number size);

// Global variable to capture the last FatalError message produced by our mock.
// This allows assertions to verify correct formatting without terminating the test.
static std::string g_lastFatalMessage;

// Mock implementation of FatalError to intercept the formatted message.
// This must match the signature used by the focal code: void FatalError(const char*, ...)
// We deliberately do not terminate the process; tests rely on message capture only.
extern "C" void FatalError(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, ap);
    va_end(ap);
    g_lastFatalMessage = buffer;
}

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_passed_tests = 0;

static void log_test_result(const char* testName, bool passed)
{
    ++g_total_tests;
    if (passed) {
        ++g_passed_tests;
        std::cout << "[PASSED] " << testName << "\n";
    } else {
        std::cerr << "[FAILED] " << testName << "\n";
    }
}

// Test 1
// Purpose: Ensure that OutOfMem formats the message for a small size correctly.
static bool test_OutOfMem_FormatsSmallSize()
{
    g_lastFatalMessage.clear();
    cmsUInt32Number testSize = 5;
    OutOfMem(testSize);
    std::string expected = "Out of memory on allocating " + std::to_string(testSize) + " bytes.";
    return g_lastFatalMessage == expected;
}

// Test 2
// Purpose: Ensure that OutOfMem formats the message for a larger size correctly.
static bool test_OutOfMem_FormatsLargeSize()
{
    g_lastFatalMessage.clear();
    cmsUInt32Number testSize = 123456;
    OutOfMem(testSize);
    std::string expected = "Out of memory on allocating " + std::to_string(testSize) + " bytes.";
    return g_lastFatalMessage == expected;
}

// Test 3
// Purpose: Validate that multiple consecutive calls still produce correctly formatted messages.
static bool test_OutOfMem_MultipleCalls_FormatConsistency()
{
    // First call
    g_lastFatalMessage.clear();
    OutOfMem(1);
    std::string expected1 = "Out of memory on allocating 1 bytes.";
    if (g_lastFatalMessage != expected1) return false;

    // Second call
    g_lastFatalMessage.clear();
    OutOfMem(999);
    std::string expected2 = "Out of memory on allocating 999 bytes.";
    if (g_lastFatalMessage != expected2) return false;

    return true;
}

int main()
{
    // Run tests (non-terminating)
    log_test_result("OutOfMem formats small size", test_OutOfMem_FormatsSmallSize());
    log_test_result("OutOfMem formats large size", test_OutOfMem_FormatsLargeSize());
    log_test_result("OutOfMem multiple calls formatting consistency", test_OutOfMem_MultipleCalls_FormatConsistency());

    // Summary
    std::cout << "Tests run: " << g_total_tests << ", Passed: " << g_passed_tests
              << ", Failed: " << (g_total_tests - g_passed_tests) << "\n";

    // Return non-zero if any test failed
    return (g_passed_tests == g_total_tests) ? 0 : 1;
}
// cmscgats_synerror_tests.cpp
// A self-contained unit test suite for the focal method SynError
// Note: This test is self-contained and does not rely on GTest.
// It mocks the minimal environment required by SynError and validates its behavior.
// The goal is to exercise basic formatting, error signaling, and side-effects
// (setting sy, and calling cmsSignalError with the constructed message).

#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <cstdarg>
#include <cassert>
#include <lcms2_internal.h>


// Domain-specific types and constants (mocked minimal subset for testing)
typedef int cmsBool;
#define FALSE 0
#define TRUE 1
#define SSYNERROR 1
#define cmsERROR_CORRUPTION_DETECTED 2

// Forward declaration of cmsSignalError to enable a mock (test-scoped)
extern "C" void cmsSignalError(void* ContextID, int ErrCode, const char* fmt, ...);

// Minimal cmsIT8 and FileStackEntry structures used by SynError
typedef struct FileStackEntry {
    char FileName[256];
    FILE* Stream;
} FileStackEntry;

typedef struct cmsIT8 {
    FileStackEntry** FileStack; // array of pointers to FileStackEntry
    int IncludeSP;
    int lineno;
    int sy;
    void* ContextID;
} cmsIT8;

// The focal method under test (copied here to enable isolated testing in this suite)
// This mirrors the implementation provided in the focal snippet.
cmsBool SynError(cmsIT8* it8, const char *Txt, ...)
{
    {
        char Buffer[256], ErrMsg[1024];
        va_list args;
        va_start(args, Txt);
        vsnprintf(Buffer, 255, Txt, args);
        Buffer[255] = 0;
        va_end(args);
        snprintf(ErrMsg, 1023, "%s: Line %d, %s", it8->FileStack[it8 ->IncludeSP]->FileName, it8->lineno, Buffer);
        ErrMsg[1023] = 0;
        it8->sy = SSYNERROR;
        cmsSignalError(it8 ->ContextID, cmsERROR_CORRUPTION_DETECTED, "%s", ErrMsg);
        return FALSE;
    }
}

// Global storage to capture the mock cmsSignalError invocation
static void* g_LastSignalContext = nullptr;
static int   g_LastSignalCode = -1;
static std::string g_LastSignalMessage;

// Mock implementation of cmsSignalError to capture parameters for assertions
extern "C" void cmsSignalError(void* ContextID, int ErrCode, const char* fmt, ...)
{
    g_LastSignalContext = ContextID;
    g_LastSignalCode = ErrCode;

    // Copy formatted message
    char buffer[2048];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, ap);
    va_end(ap);
    g_LastSignalMessage.assign(buffer);
}

// Helpers for test state
static void reset_signal_capture()
{
    g_LastSignalContext = nullptr;
    g_LastSignalCode = -1;
    g_LastSignalMessage.clear();
}

// Simple test harness
static int tests_run = 0;
static int tests_failed = 0;

#define RUN_TEST(name) do { \
    if (!(name())) { \
        std::cerr << "Test failed: " << #name << "\n"; \
        tests_failed++; \
    } else { \
        std::cout << "Test passed: " << #name << "\n"; \
    } \
    tests_run++; \
} while(0)

// Test 1: Basic formatting of Txt with single integer argument
static bool test_SynError_BasicFormatting()
{
    reset_signal_capture();

    // Prepare a minimal it8 structure
    cmsIT8 it8;
    FileStackEntry* entry = new FileStackEntry;
    std::strcpy(entry->FileName, "source.it8");
    entry->Stream = nullptr;

    FileStackEntry** stack = new FileStackEntry*[1];
    stack[0] = entry;

    it8.FileStack = stack;
    it8.IncludeSP = 0;
    it8.lineno = 123;
    it8.sy = 0;
    it8.ContextID = (void*)0x1234;

    const char* Txt = "Value is %d";

    cmsBool ret = SynError(&it8, Txt, 7);

    // Assertions
    bool ok = true;
    if (ret != FALSE) {
        ok = false;
        std::cerr << "SynError should return FALSE\n";
    }
    if (it8.sy != SSYNERROR) {
        ok = false;
        std::cerr << "it8->sy should be SSYNERROR\n";
    }
    if (g_LastSignalContext != it8.ContextID) {
        ok = false;
        std::cerr << "cmsSignalError ContextID mismatch\n";
    }
    if (g_LastSignalCode != cmsERROR_CORRUPTION_DETECTED) {
        ok = false;
        std::cerr << "cmsSignalError ErrCode mismatch\n";
    }
    // Expected message: "source.it8: Line 123, Value is 7"
    const std::string expectedMsg = std::string("source.it8: Line 123, Value is 7");
    if (g_LastSignalMessage != expectedMsg) {
        ok = false;
        std::cerr << "cmsSignalError message mismatch, expected: \"" << expectedMsg
                  << "\", got: \"" << g_LastSignalMessage << "\"\n";
    }

    // Cleanup
    delete entry;
    delete[] stack;

    return ok;
}

// Test 2: Ensure formatting handles multiple characters and different text
static bool test_SynError_FormattedText2()
{
    reset_signal_capture();

    cmsIT8 it8;
    FileStackEntry* entry = new FileStackEntry;
    std::strcpy(entry->FileName, "log.it8");
    entry->Stream = nullptr;

    FileStackEntry** stack = new FileStackEntry*[1];
    stack[0] = entry;

    it8.FileStack = stack;
    it8.IncludeSP = 0;
    it8.lineno = 999;
    it8.sy = 0;
    it8.ContextID = (void*)0xDEADBEEF;

    const char* Txt = "Status: %s, code=%d";

    cmsBool ret = SynError(&it8, Txt, "OK", 0);

    // Assertions
    bool ok = true;
    if (ret != FALSE) {
        ok = false;
        std::cerr << "SynError should return FALSE (test 2)\n";
    }
    if (it8.sy != SSYNERROR) {
        ok = false;
        std::cerr << "it8->sy should be SSYNERROR (test 2)\n";
    }
    if (g_LastSignalContext != it8.ContextID) {
        ok = false;
        std::cerr << "cmsSignalError ContextID mismatch (test 2)\n";
    }
    if (g_LastSignalCode != cmsERROR_CORRUPTION_DETECTED) {
        ok = false;
        std::cerr << "cmsSignalError ErrCode mismatch (test 2)\n";
    }
    // Expected message: "log.it8: Line 999, Status: OK, code=0"
    const std::string expectedMsg = std::string("log.it8: Line 999, Status: OK, code=0");
    if (g_LastSignalMessage != expectedMsg) {
        ok = false;
        std::cerr << "cmsSignalError message mismatch (test 2), expected: \"" << expectedMsg
                  << "\", got: \"" << g_LastSignalMessage << "\"\n";
    }

    // Cleanup
    delete entry;
    delete[] stack;

    return ok;
}

int main()
{
    // Run tests
    RUN_TEST(test_SynError_BasicFormatting);
    RUN_TEST(test_SynError_FormattedText2);

    // Summary
    std::cout << "Tests run: " << tests_run << ", Failures: " << tests_failed << "\n";
    return (tests_failed == 0) ? 0 : 1;
}
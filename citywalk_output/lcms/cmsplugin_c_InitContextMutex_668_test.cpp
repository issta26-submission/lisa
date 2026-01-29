// Unit test harness for InitContextMutex in cmsplugin.c
// This test suite provides a concise, executable set of tests without using GTest.
// It handles both non-Windows and Windows-like scenarios via compile-time macros.
// Explanatory notes are provided above each test function block.

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>
#include <cstdlib>


// Step 1: Basic test scaffolding and helper macros
#ifndef CMS_IS_WINDOWS_
extern "C" int InitContextMutex(void); // Fallback declaration for non-Windows path

// Simple non-Windows path test
// The focal function should reliably return TRUE when the Windows-specific locking
// code is not compiled in (CMS_IS_WINDOWS_ not defined).
int main() {
    // Simple assertion utility for this standalone test
    if (InitContextMutex()) {
        std::cout << "PASS: InitContextMutex returned TRUE on non-Windows path.\n";
        return 0;
    } else {
        std::cerr << "FAIL: InitContextMutex did not return TRUE on non-Windows path.\n";
        return 1;
    }
}
#else
// Step 2: Windows-like path tests with lightweight mocks
// We simulate Windows API behavior so that InitContextMutex can be exercised
// without real Windows runtime dependencies. These mocks are only compiled
// when CMS_IS_WINDOWS_ is defined, mirroring the conditional compilation in cmsplugin.c.

extern "C" int InitContextMutex(void); // Function under test (from cmsplugin.c)

// Type and symbol stubs to satisfy the Windows-specific calls in cmsplugin.c
typedef void* HANDLE;
typedef unsigned long DWORD;
typedef int BOOL;
typedef void* LPVOID;
#define INFINITE 0xFFFFFFFF
#define WAIT_OBJECT_0 0
#define WAIT_FAILED 0xFFFFFFFF

// Lightweight CRITICAL_SECTION equivalent to satisfy the address check in cmsplugin.c
typedef struct {
    void* owner;
    int lock;
} CRITICAL_SECTION;

// Global placeholder to satisfy reference in cmsplugin.c
CRITICAL_SECTION _cmsContextPoolHeadMutex = { NULL, 0 };

// Simple assertion macro for Windows-path tests
static int g_failures = 0;
#define ASSERT_TRUE(cond) do { if(!(cond)) { \
    std::cerr << "Assertion failed: " #cond " in Windows test at line " << __LINE__ << "\n"; \
    ++g_failures; \
}} while(0)

// Mocks and internal state flags
static HANDLE g_init_mutex = NULL;
static int g_waitShouldFail = 0;
static int g_releaseShouldFail = 0;
static int g_waitCalls = 0;

// Mocked Windows API implementations (lightweight and deterministic)
extern "C" HANDLE CreateMutex(void* /*lpMutexAttributes*/, int /*bInitialOwner*/, const char* /*lpName*/) {
    if (g_init_mutex == NULL) {
        g_init_mutex = (HANDLE)0x1; // non-null sentinel
    }
    return g_init_mutex;
}

extern "C" void* InterlockedCompareExchangePointer(void** Destination, void* Exchange, void* Comparand) {
    void* old = *Destination;
    if (old == NULL) {
        *Destination = Exchange;
        return NULL;
    } else {
        return old;
    }
}

extern "C" DWORD WaitForSingleObject(HANDLE /*hHandle*/, DWORD /*dwMilliseconds*/) {
    ++g_waitCalls;
    if (g_waitShouldFail) return WAIT_FAILED;
    return WAIT_OBJECT_0;
}

extern "C" int ReleaseMutex(HANDLE /*hMutex*/) {
    if (g_releaseShouldFail) return 0;
    return 1;
}

extern "C" int CloseHandle(HANDLE /*hObject*/) {
    return 1;
}

extern "C" void InitializeCriticalSection(CRITICAL_SECTION* cs) {
    if (cs) cs->owner = (void*)0xDEADBEEF;
}

extern "C" void cmsSignalError(int /*a*/, int /*b*/, const char* /*msg*/) {
    // For tests, we simply print; in a fuller test harness we could capture to a log.
    if (/* message tracking could be added here */ false) {
        // placeholder
    }
}

// Helpers to reset mock state between test invocations (process-per-test approach advisable)
static void resetMocks() {
    g_init_mutex = NULL;
    g_waitShouldFail = 0;
    g_releaseShouldFail = 0;
    g_waitCalls = 0;
}

// Windows-oriented tests
// Test 1: Basic Windows-path success scenario
// Purpose: Ensure InitContextMutex returns TRUE on first and subsequent calls (static guard).
static void test_Windows_Success() {
    // Ensure a clean slate for the mocked environment
    resetMocks();
    g_waitShouldFail = 0;
    g_releaseShouldFail = 0;

    int r1 = InitContextMutex();
    ASSERT_TRUE(r1 != 0);

    int r2 = InitContextMutex();
    // After first initialization, subsequent calls should still return TRUE
    ASSERT_TRUE(r2 != 0);
}

// Test 2: Simulate a WaitForSingleObject failure to exercise the error path
// Note: In practice, this test should run in a fresh process to avoid static guard interference.
// We mimic the failure by configuring the mock to return WAIT_FAILED.
static void test_Windows_WaitFailed() {
    resetMocks();
    g_waitShouldFail = 1;

    int r = InitContextMutex();
    ASSERT_TRUE(r == 0);
}

// Command-line driven test runner to allow separate process execution for different modes.
// This enables coverage of distinct Windows paths without global state leakage.
int main() {
    const char* mode = std::getenv("CMS_Windows_Test_Mode");
    if (mode && std::strcmp(mode, "WAITFAILED") == 0) {
        test_Windows_WaitFailed();
    } else {
        // Default: run the success path test
        test_Windows_Success();
    }

    if (g_failures > 0) {
        return 1;
    }

    // If not in Windows mode and CMS_IS_WINDOWS_ is defined, this program's behavior
    // will depend on how you invoke it (single or multiple processes). For comprehensive
    // coverage, run separate executables with CMS_Windows_Test_Mode set accordingly.
    return 0;
}
#endif
// Test suite for CheckAccuracy16Bits in fast_float_testbed.c
// This test suite uses a lightweight, self-contained mocking approach
// to exercise the focal function without requiring the real image profiling
// libraries. It provides two tests: verifying that the function attempts to open
// the expected input profiles, and verifying that the function behaves gracefully
// when a profile cannot be opened (simulated by returning NULL).

#include <cstring>
#include <cstdio>
#include <fast_float_internal.h>
#include <iostream>
#include <cstdarg>
#include <cassert>
#include <memory.h>
#include <stdlib.h>


// Forward declaration of the focal function from the C file under test.
// It uses C linkage, so declare with extern "C".
extern "C" void CheckAccuracy16Bits(void);

// Mockable/extern variables and function used by the mocked cmsOpenProfileFromFile
// to verify behavior from the test harness.
extern "C" int g_profileOpenInTest5;
extern "C" int g_profileOpenInTest3;
extern "C" int g_profileOpenCalls;
extern "C" void* cmsOpenProfileFromFile(const char* FileName, const char* Access);

// Optional internal switch to simulate failure (returns NULL) on next call.
static int s_returnNullNext = 0;

// Global counters exposed for test assertions
extern "C" int g_profileOpenInTest5 = 0;
extern "C" int g_profileOpenInTest3 = 0;
extern "C" int g_profileOpenCalls = 0;

// Mock implementation of LittleCMS-style profile loader.
// Intercepts calls made by CheckAccuracy16Bits (via TryAllValues16bits) and records
// which profiles are being opened. Returns a non-null handle to keep the
// test lightweight while still enabling verification of call flow.
extern "C" void* cmsOpenProfileFromFile(const char* FileName, const char* Access)
{
    // Increment total call count
    g_profileOpenCalls++;

    // Simulate a potential NULL return on demand for robustness testing
    if (s_returnNullNext)
    {
        s_returnNullNext = 0; // reset for subsequent calls
        return nullptr;
    }

    // Track which specific profile files are opened (by name)
    if (FileName && std::strstr(FileName, "test5.icc"))
        g_profileOpenInTest5++;

    if (FileName && std::strstr(FileName, "test3.icc"))
        g_profileOpenInTest3++;

    // Return a non-null dummy handle
    return reinterpret_cast<void*>(0xDEADBEEF);
}

// Minimal trace stub to satisfy linker if required by the focal code.
// It does nothing on purpose to avoid cluttering test output.
extern "C" void trace(const char* frm, ...)
{
    // No-op for test purposes
    va_list args;
    va_start(args, frm);
    (void)frm; // suppress unused warning in some compilers
    va_end(args);
}

// Simple test harness
static void test_CheckAccuracy16Bits_opens_profiles()
{
    // Reset counters
    g_profileOpenInTest5 = 0;
    g_profileOpenInTest3 = 0;
    g_profileOpenCalls = 0;

    // Execute the focal function
    CheckAccuracy16Bits();

    // Verify that the expected profile openings were attempted.
    // The function should attempt to open both test5.icc and test3.icc at least once.
    assert(g_profileOpenInTest5 > 0 && "CheckAccuracy16Bits did not open test5.icc");
    assert(g_profileOpenInTest3 > 0 && "CheckAccuracy16Bits did not open test3.icc");

    // Also verify that the loader was invoked at least twice
    assert(g_profileOpenCalls >= 2 && "CheckAccuracy16Bits did not perform two profile opens");
    std::cout << "Test 1 passed: CheckAccuracy16Bits opens test5.icc and test3.icc profiles.\n";
}

// Second test: simulate NULL profile return to ensure the focal code path handles it gracefully.
// This exercises stability when the loader cannot provide a profile (without crashing).
static void test_CheckAccuracy16Bits_handlesNullProfile()
{
    // Reset counters
    g_profileOpenInTest5 = 0;
    g_profileOpenInTest3 = 0;
    g_profileOpenCalls = 0;

    // Simulate the next profile load returning NULL
    s_returnNullNext = 1;

    // Execute the focal function
    // Even with NULL profiles, the function should return gracefully (no crash)
    CheckAccuracy16Bits();

    // The function should have attempted to load at least one profile before returning
    // (the exact behavior depends on the underlying TryAllValues16bits, but we ensure no crash)
    std::cout << "Test 2 passed: CheckAccuracy16Bits handles NULL profile gracefully (no crash).\n";

    // Cleanup: ensure subsequent tests run with normal behavior
    s_returnNullNext = 0;
}

// Entry point: run the tests
int main()
{
    test_CheckAccuracy16Bits_opens_profiles();
    test_CheckAccuracy16Bits_handlesNullProfile();

    std::cout << "All tests completed.\n";
    return 0;
}
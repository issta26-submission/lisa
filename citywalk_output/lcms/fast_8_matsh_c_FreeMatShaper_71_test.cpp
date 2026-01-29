/*
Unit Test Suite for FreeMatShaper
Target: Validate FreeMatShaper behavior for NULL and non-NULL Data inputs.

Notes:
- The focal function FreeMatShaper is implemented here with C linkage to mirror the original C-style API:
  void FreeMatShaper(cmsContext ContextID, void* Data)
  It ignores ContextID and calls free_aligned if Data is non-NULL.
- We provide a minimal stub of XMatShaper8Data and override free_aligned to observe invocations.
- Tests use non-terminating assertions (EXPECT_*) to exercise both true and false branches.
- A small test harness prints a summary and returns non-zero if failures occurred.
- This file is self-contained and uses only the C++11 standard library.

Implementation details:
- Data == NULL should not trigger free_aligned.
- Data != NULL should call free_aligned with the exact pointer passed in.
- Multiple non-NULL calls should increment the counter accordingly and track the last freed pointer.

Usage:
- Compile as a C++11 program and run the executable. It will print pass/fail details and exit with code 0 on success, 1 on failure.

*/
#include <cstddef>
#include <iostream>
#include <cstdio>
#include <fast_float_internal.h>


// Domain-specific placeholder types and macros to mirror dependencies
typedef int cmsContext;

// Minimal placeholder for the focal data structure
struct XMatShaper8Data {};

// Macro to acknowledge unused parameters without warnings
#define UNUSED_PARAMETER(x) (void)(x)

// Forward declarations with C linkage to resemble the original codebase
extern "C" void free_aligned(XMatShaper8Data* a);
extern "C" void FreeMatShaper(cmsContext ContextID, void* Data);

// Global state to observe calls to free_aligned
static int g_FreeAligned_CallCount = 0;
static XMatShaper8Data* g_LastFreedPtr = nullptr;

// Override of free_aligned to observe invocations and the data passed
extern "C" void free_aligned(XMatShaper8Data* a)
{
    ++g_FreeAligned_CallCount;
    g_LastFreedPtr = a;
}

// Implementation of the focal method under test
extern "C" void FreeMatShaper(cmsContext ContextID, void* Data)
{
    UNUSED_PARAMETER(ContextID);
    if (Data != NULL) free_aligned((XMatShaper8Data*) Data);
}

// --- Test Harness (non-terminating assertions) ---

// Simple test result tracking
static int g_TotalTests = 0;
static int g_Failures = 0;

void Fail(const char* msg)
{
    ++g_Failures;
    std::cerr << "FAIL: " << msg << std::endl;
}

#define EXPECT_TRUE(cond, msg) do { \
    ++g_TotalTests; \
    if(!(cond)) Fail(msg); \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    ++g_TotalTests; \
    if(!((a) == (b))) Fail(msg); \
} while(0)

// Test 1: Data is NULL -> free_aligned must not be called
void test_FreeMatShaper_DataNull()
{
    // Reset observed state
    g_FreeAligned_CallCount = 0;
    g_LastFreedPtr = nullptr;

    cmsContext ctx = 0;
    FreeMatShaper(ctx, nullptr);

    // Expectation: free_aligned should not be invoked
    EXPECT_EQ(g_FreeAligned_CallCount, 0, "free_aligned should not be called when Data is NULL");
    // Also ensure LastFreedPtr remains null
    EXPECT_TRUE(g_LastFreedPtr == nullptr, "Last freed pointer should remain nullptr when Data is NULL");
}

// Test 2: Data is non-NULL -> free_aligned must be called with the same pointer
void test_FreeMatShaper_DataNonNull()
{
    // Reset observed state
    g_FreeAligned_CallCount = 0;
    g_LastFreedPtr = nullptr;

    XMatShaper8Data* data = new XMatShaper8Data();
    FreeMatShaper(0, (void*)data);

    // Expectation: free_aligned should be invoked exactly once with the same pointer
    EXPECT_EQ(g_FreeAligned_CallCount, 1, "free_aligned should be called exactly once for non-NULL Data");
    EXPECT_EQ(g_LastFreedPtr, data, "free_aligned should be called with the same pointer passed to FreeMatShaper");

    delete data;
}

// Test 3: Multiple non-NULL calls -> counter increments and last pointer is the second one
void test_FreeMatShaper_MultipleNonNull()
{
    // Reset observed state
    g_FreeAligned_CallCount = 0;
    g_LastFreedPtr = nullptr;

    XMatShaper8Data* p1 = new XMatShaper8Data();
    XMatShaper8Data* p2 = new XMatShaper8Data();

    FreeMatShaper(0, (void*)p1);
    FreeMatShaper(0, (void*)p2);

    // Expectations
    EXPECT_EQ(g_FreeAligned_CallCount, 2, "free_aligned should be called twice for two non-NULL Data calls");
    EXPECT_EQ(g_LastFreedPtr, p2, "Last freed pointer should be the second pointer passed");

    delete p1;
    delete p2;
}

// Runner to execute all tests
void RunAllTests()
{
    // Clear any prior state
    g_TotalTests = 0;
    g_Failures = 0;

    test_FreeMatShaper_DataNull();
    test_FreeMatShaper_DataNonNull();
    test_FreeMatShaper_MultipleNonNull();

    // Summary
    std::cout << "Test Summary: Total=" << g_TotalTests
              << " Failures=" << g_Failures << std::endl;
}

// Main entry point
int main()
{
    RunAllTests();

    if (g_Failures != 0) {
        std::cerr << "Some tests FAILED." << std::endl;
        return 1;
    } else {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    }
}
// Test suite for FreeMatShaper in cmsopt.c
// Purpose: verify FreeMatShaper calls _cmsFree when Data is non-NULL and does not call it when Data is NULL.
// Approach: provide a mock _cmsFree in this test translation unit to capture invocation details.
// Note: Build assumes cmsopt.o (or cmsopt.c compiled with a C compiler) is linked with this test.
// The mock _cmsFree uses C linkage to override the library's symbol during linking.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstdlib>


// Forward declare the functions under test with C linkage
extern "C" void FreeMatShaper(cmsContext ContextID, void* Data);

// Provide a C linkage mock for _cmsFree to intercept calls from FreeMatShaper
extern "C" void _cmsFree(cmsContext ContextID, void* Data);

// Global variables to capture mock invocation details
static int g_cmsFreeCallCount = 0;
static cmsContext g_lastCmsContext = nullptr;
static void* g_lastCmsData = nullptr;

// Mock implementation of _cmsFree to observe interactions
extern "C" void _cmsFree(cmsContext ContextID, void* Data)
{
    ++g_cmsFreeCallCount;
    g_lastCmsContext = ContextID;
    g_lastCmsData = Data;
}

// Utility to reset mock state between tests
static void resetMockState()
{
    g_cmsFreeCallCount = 0;
    g_lastCmsContext = nullptr;
    g_lastCmsData = nullptr;
}

// Lightweight, non-terminating assertion helpers (no GTest)
static int g_totalFailures = 0;

#define EXPECT_EQ(a, b) do { auto _a = (a); auto _b = (b); if (!(_a == _b)) { ++g_totalFailures; std::cerr << "EXPECT_EQ failed: " << #a << " (" << _a << ") != " << #b << " (" << _b << ") at " << __FILE__ << ":" << __LINE__ << std::endl; } } while(0)
#define EXPECT_PTR_EQ(a, b) do { void* _a = (void*)(a); void* _b = (void*)(b); if (_a != _b) { ++g_totalFailures; std::cerr << "EXPECT_PTR_EQ failed: " << #a << " (" << _a << ") != " << #b << " (" << _b << ") at " << __FILE__ << ":" << __LINE__ << std::endl; } } while(0)


// Test 1: When Data is non-NULL, FreeMatShaper should call _cmsFree exactly once with the provided Data and ContextID
static void test_FreeMatShaper_calls_cmsFree_on_non_null_data()
{
    resetMockState();

    // Prepare a non-null data buffer
    void* buffer = std::malloc(64);
    if (!buffer) {
        std::cerr << "Memory allocation failed in test setup." << std::endl;
        ++g_totalFailures;
        return;
    }

    cmsContext ctx = reinterpret_cast<cmsContext>(0x1234); // arbitrary non-null context

    // Call the function under test
    FreeMatShaper(ctx, buffer);

    // Expectations
    EXPECT_EQ(g_cmsFreeCallCount, 1);
    EXPECT_PTR_EQ(g_lastCmsContext, ctx);
    EXPECT_PTR_EQ(g_lastCmsData, buffer);

    // Cleanup: free the buffer to avoid a real leak in the host test environment
    std::free(buffer);
}

// Test 2: When Data is NULL, FreeMatShaper should NOT call _cmsFree
static void test_FreeMatShaper_does_not_call_cmsFree_on_null_data()
{
    resetMockState();

    cmsContext ctx = reinterpret_cast<cmsContext>(0xDEAD); // arbitrary context

    // Call the function under test with NULL data
    FreeMatShaper(ctx, nullptr);

    // Expect no invocation of _cmsFree
    EXPECT_EQ(g_cmsFreeCallCount, 0);
    EXPECT_PTR_EQ(g_lastCmsContext, nullptr);
    EXPECT_PTR_EQ(g_lastCmsData, nullptr);
}

// Test 3: Repeated calls ensure consistent behavior across multiple invocations
static void test_FreeMatShaper_multiple_calls_consistency()
{
    resetMockState();

    // First non-null call
    void* a = std::malloc(16);
    cmsContext c1 = reinterpret_cast<cmsContext>(0xABCD);
    FreeMatShaper(c1, a);
    EXPECT_EQ(g_cmsFreeCallCount, 1);
    EXPECT_PTR_EQ(g_lastCmsContext, c1);
    EXPECT_PTR_EQ(g_lastCmsData, a);

    // Free the memory via mock and real free to avoid leak
    std::free(a);
    resetMockState();

    // Second non-null call with different context and data
    void* b = std::malloc(32);
    cmsContext c2 = reinterpret_cast<cmsContext>(0xBEEF);
    FreeMatShaper(c2, b);
    EXPECT_EQ(g_cmsFreeCallCount, 1);
    EXPECT_PTR_EQ(g_lastCmsContext, c2);
    EXPECT_PTR_EQ(g_lastCmsData, b);

    std::free(b);
    resetMockState();

    // Null data call
    FreeMatShaper(c2, nullptr);
    EXPECT_EQ(g_cmsFreeCallCount, 0);
    EXPECT_PTR_EQ(g_lastCmsData, nullptr);
}


// Minimal test runner
int main()
{
    std::cout << "Running FreeMatShaper unit tests (CMS internal shaper memory management)..." << std::endl;

    test_FreeMatShaper_calls_cmsFree_on_non_null_data();
    test_FreeMatShaper_does_not_call_cmsFree_on_null_data();
    test_FreeMatShaper_multiple_calls_consistency();

    if (g_totalFailures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_totalFailures << " test(s) failed." << std::endl;
        return 1;
    }
}
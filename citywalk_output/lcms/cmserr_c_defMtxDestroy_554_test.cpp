// Unit test suite for defMtxDestroy in cmserr.c
// This test suite is written for C++11, without GoogleTest, and uses a custom
// lightweight test harness with non-terminating EXPECT-style checks.
// The test stubs internal CMS mutex destruction and memory free functions to
// verify the exact interactions performed by defMtxDestroy.

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>
#include <cstdlib>


// Import necessary CMS internal types. This assumes the environment provides
// lcms2_internal.h in the include path as per the project's structure.

// Forward declare the opaque mutex type from CMS.
// The real definition lives in the CMS headers; we only need a pointer type here.
struct _cmsMutex;

// Declares the focal function from cmserr.c with C linkage.
extern "C" void defMtxDestroy(cmsContext id, void* mtx);

// Forward declarations for internal CMS helper functions (to be overridden by tests)
extern "C" void _cmsDestroyMutexPrimitive(struct _cmsMutex* mtx);
extern "C" void _cmsFree(cmsContext ContextID, void* Ptr);

// Our test state: records how many times internal functions were invoked and with which parameters.
struct TestState {
    int destroyCallCount = 0;
    void* destroyPtr = nullptr;

    int freeCallCount = 0;
    cmsContext freeContext = nullptr;
    void* freePtr = nullptr;
};

static TestState g_state;

// Reset the test state before each test case
static void resetState() {
    g_state.destroyCallCount = 0;
    g_state.destroyPtr = nullptr;
    g_state.freeCallCount = 0;
    g_state.freeContext = nullptr;
    g_state.freePtr = nullptr;
}

// Implement stubbed internal functions with C linkage so they can be linked with cmserr.c

extern "C" void _cmsDestroyMutexPrimitive(struct _cmsMutex* mtx) {
    // Record that the mutex destruction primitive was invoked and with which pointer
    g_state.destroyCallCount++;
    g_state.destroyPtr = (void*)mtx;
}

extern "C" void _cmsFree(cmsContext ContextID, void* Ptr) {
    // Record that the free function was invoked and with which context and pointer
    g_state.freeCallCount++;
    g_state.freeContext = ContextID;
    g_state.freePtr = Ptr;
}

// Lightweight, non-terminating assertion helper
static int g_totalFailures = 0;
static void expect(bool condition, const char* message) {
    if (!condition) {
        ++g_totalFailures;
        std::cerr << "EXPECT FAILED: " << message << std::endl;
    }
}

// Test 1: Basic destruction with a valid (non-null) mutex pointer
static void test_destroy_with_valid_mutex() {
    resetState();

    // Prepare a fake mutex object (pointer is enough; size is unknown here)
    void* mtx = std::malloc(1); // allocate minimal block to have a non-null pointer
    cmsContext testCtx = reinterpret_cast<cmsContext>(0x1234); // arbitrary context

    // Call the function under test
    defMtxDestroy(testCtx, mtx);

    // Verify interactions
    expect(g_state.destroyCallCount == 1, "defMtxDestroy should call _cmsDestroyMutexPrimitive exactly once for valid mtx");
    expect(g_state.destroyPtr == mtx, "destroy should be invoked with the original mtx pointer");

    expect(g_state.freeCallCount == 1, "defMtxDestroy should call _cmsFree exactly once for valid mtx");
    expect(g_state.freeContext == testCtx, "free should be invoked with the same cmsContext as provided");
    expect(g_state.freePtr == mtx, "free should be invoked with the same mtx pointer");

    // Cleanup
    std::free(mtx);
}

// Test 2: Destruction with a null mutex pointer
static void test_destroy_with_null_mutex() {
    resetState();

    void* mtx = nullptr;
    cmsContext testCtx = reinterpret_cast<cmsContext>(0xABCD);

    defMtxDestroy(testCtx, mtx);

    // Even for null, the function should attempt to destroy the primitive and free
    expect(g_state.destroyCallCount == 1, "defMtxDestroy should call _cmsDestroyMutexPrimitive even for null mtx");
    expect(g_state.destroyPtr == nullptr, "destroy should be invoked with nullptr when mtx is null");

    expect(g_state.freeCallCount == 1, "defMtxDestroy should call _cmsFree even for null mtx");
    expect(g_state.freeContext == testCtx, "free should be invoked with the provided context even if mtx is null");
    expect(g_state.freePtr == nullptr, "free should be invoked with nullptr as mtx pointer when mtx is null");
}

// Test 3: Multiple consecutive destructions on the same mutex pointer
static void test_destroy_multiple_calls_on_same_mutex() {
    resetState();

    void* mtx = std::malloc(1);
    cmsContext testCtx = reinterpret_cast<cmsContext>(0xDEAD);

    // First destruction
    defMtxDestroy(testCtx, mtx);
    // Second destruction on the same mutex
    defMtxDestroy(testCtx, mtx);

    // Expect two invocations each
    expect(g_state.destroyCallCount == 2, "defMtxDestroy should invoke _cmsDestroyMutexPrimitive twice for consecutive destructions");
    expect(g_state.destroyPtr == mtx, "destroy should be called with the original mtx pointer each time");

    expect(g_state.freeCallCount == 2, "defMtxDestroy should invoke _cmsFree twice for consecutive destructions");
    expect(g_state.freeContext == testCtx, "free context should match for both destructions");
    expect(g_state.freePtr == mtx, "free should be invoked with the original mtx pointer each time");

    std::free(mtx);
}

// Test harness runner
static void runAllTests() {
    std::cout << "Starting unit tests for defMtxDestroy...\n";

    test_destroy_with_valid_mutex();
    test_destroy_with_null_mutex();
    test_destroy_multiple_calls_on_same_mutex();

    if (g_totalFailures > 0) {
        std::cout << "Unit tests completed with " << g_totalFailures << " failure(s).\n";
    } else {
        std::cout << "Unit tests completed successfully. All expectations met.\n";
    }
}

// Entry point
int main() {
    // Initialize the global state
    resetState();

    // Run all tests
    runAllTests();

    // Return non-zero if any expectations failed
    return (g_totalFailures > 0) ? 1 : 0;
}
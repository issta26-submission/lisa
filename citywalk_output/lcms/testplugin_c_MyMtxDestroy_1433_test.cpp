// Test suite for MyMtxDestroy in testplugin.c
// This test harness is written in C++11 and uses manual test macros
// (no Google Test). It provides lightweight mocks for external
// dependencies (Die and _cmsFree) to verify behavior of the focal method.

#include <cstdio>
#include <testcms2.h>
#include <cstdlib>


// Forward declarations of the focal method and its dependencies as seen by the linker.
// We do not rely on the actual struct layout here except to provide a compatible memory block
// that mimics at least the first member used by the focal method (nlocks).
extern "C" {
    // The real MyMtxDestroy is defined in testplugin.c (the focal method under test).
    void MyMtxDestroy(void* id, void* mtx); // cmsContext is treated as void* for test purposes.
    
    // Mocks for external dependencies used by the focal method.
    void _cmsFree(void* id, void* ptr);
    void Die(const char* msg);
}

// Simple domain types used by the interface (as expected by the focal method).
typedef void* cmsContext;

// A minimal representation of the mutex structure used by MyMtxDestroy.
// We only need the 'nlocks' member since that's what the focal method inspects.
struct MyMtxLayout {
    int nlocks;
};

// Global state for test harness
static int g_tests_run = 0;
static int g_failures  = 0;

// Mocks to intercept behavior of the focal method
// These must be defined with C linkage so that the linker resolves them
static void* g_lastFreedPtr = nullptr;
static cmsContext g_lastFreedId = nullptr;
static bool g_dieCalled = false;

// Mock implementation of Die: do not terminate, just record that it was invoked.
extern "C" void Die(const char* /*msg*/) {
    g_dieCalled = true;
}

// Mock implementation of _cmsFree: record the call and actually free the memory to avoid leaks.
extern "C" void _cmsFree(cmsContext id, void* ptr) {
    g_lastFreedId = id;
    g_lastFreedPtr = ptr;
    // Simulate real free behavior
    free(ptr);
}

// Helper assertions (non-terminating)
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        fprintf(stderr, "EXPECT_TRUE failed at %s:%d: %s\\n", __FILE__, __LINE__, #cond); \
        g_failures++; \
    } \
} while(0)

#define EXPECT_EQ_PTR(a,b) do { \
    if((void*)(a) != (void*)(b)) { \
        fprintf(stderr, "EXPECT_EQ_PTR failed at %s:%d: %p != %p\\n", __FILE__, __LINE__, (void*)(a), (void*)(b)); \
        g_failures++; \
    } \
} while(0)

#define EXPECT_EQ_INT(a,b) do { \
    if((int)(a) != (int)(b)) { \
        fprintf(stderr, "EXPECT_EQ_INT failed at %s:%d: %d != %d\\n", __FILE__, __LINE__, (int)(a), (int)(b)); \
        g_failures++; \
    } \
} while(0)

#define TEST(name) void name(); \
    void name()

// Test 1: When nlocks == 0, MyMtxDestroy should call _cmsFree with the correct id and the mutex pointer,
//         and should not call Die.
TEST(test_MyMtxDestroy_FreeWhenZeroLocks) {
    g_tests_run++;

    // Reset mocks
    g_dieCalled = false;
    g_lastFreedPtr = nullptr;
    g_lastFreedId = nullptr;

    // Prepare a dummy cmsContext and a mutex object with nlocks = 0
    cmsContext dummyId = reinterpret_cast<cmsContext>(0x1234);
    MyMtxLayout* mtx = (MyMtxLayout*) malloc(sizeof(MyMtxLayout));
    mtx->nlocks = 0;

    // Call the focal method
    MyMtxDestroy(dummyId, (void*)mtx);

    // Verifications
    EXPECT_TRUE(!g_dieCalled);                       // True branch should not call Die
    EXPECT_EQ_PTR(g_lastFreedPtr, (void*)mtx);        // _cmsFree should be invoked with the same mutex pointer
    EXPECT_EQ_PTR(g_lastFreedId, dummyId);            // and with the provided context

    // Do not free mtx here; _cmsFree mock already freed it
}

// Test 2: When nlocks != 0, MyMtxDestroy should invoke Die and should NOT call _cmsFree.
TEST(test_MyMtxDestroy_ErrorWhenNonZeroLocks) {
    g_tests_run++;

    // Reset mocks
    g_dieCalled = false;
    g_lastFreedPtr = nullptr;
    g_lastFreedId = nullptr;

    // Prepare a dummy cmsContext and a mutex object with nlocks != 0
    cmsContext dummyId = reinterpret_cast<cmsContext>(0x5678);
    MyMtxLayout* mtx = (MyMtxLayout*) malloc(sizeof(MyMtxLayout));
    mtx->nlocks = 5; // non-zero to trigger Die

    // Call the focal method
    MyMtxDestroy(dummyId, (void*)mtx);

    // Verifications
    EXPECT_TRUE(g_dieCalled);                          // Die should have been invoked
    // _cmsFree should not have been called; pointer should remain nullptr
    EXPECT_TRUE(g_lastFreedPtr == nullptr);
    EXPECT_TRUE(g_lastFreedId == nullptr);

    // Clean up: since _cmsFree was not called, free the allocated memory manually
    free(mtx);
}

// Entry point
int main() {
    // Run tests
    test_MyMtxDestroy_FreeWhenZeroLocks();
    test_MyMtxDestroy_ErrorWhenNonZeroLocks();

    // Report results
    if(g_failures == 0) {
        printf("All tests passed (%d tests).\n", g_tests_run);
    } else {
        printf("Tests completed with %d failure(s) out of %d test(s).\n", g_failures, g_tests_run);
    }

    // Return non-zero if any test failed
    return g_failures ? 1 : 0;
}
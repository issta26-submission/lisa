/*
Step 1 (Program Understanding) summary:
- Focal method: MyMtxCreate(cmsContext id)
- Behavior: Creates an unnamed Windows mutex via CreateMutex with default security attributes and not initially owned.
- Dependencies: cmContext type (from lcms2_plugin.h), CreateMutex (Windows API), potential interplay with a static plugin sample in the same translation unit (MutexPluginSample), and other thread-related helpers (not used in focused tests).
- Core candidate dependencies (Candidate Keywords): CreateMutex, cmsContext, MutexPluginSample, MyMtxDestroy, MyMtxLock, MyMtxUnlock, cmsContext id.

Step 2 (Unit Test Generation) plan:
- Write a small, self-contained test program (testthread.cpp) that exercises MyMtxCreate.
- Use a lightweight, non-terminating assertion mechanism (no GTest) per domain guidance.
- Validate that MyMtxCreate returns a non-null handle for:
  - a null cmsContext id (homepage of the function does not rely on the id)
  - subsequent calls (ensuring distinct handles are produced)
- Ensure proper resource cleanup with CloseHandle on all allocated mutex handles.
- Keep tests simple and compile under C++11 without GoogleTest.

Step 3 (Test Case Refinement) notes:
- Use only Windows API and standard library; do not rely on private/static internals beyond what the focal function exposes.
- Use a minimal test harness with non-terminating assertions (so tests still execute).
- Tests focus on functional behavior of MyMtxCreate (returning a valid HANDLE/void*).
- No mocks (GMock not allowed); only direct function calls to the focal method.
*/

#include <windows.h>
#include <iostream>
#include <lcms2_plugin.h>


// Extern declaration of the focal method under test.
// The exact signature uses cmsContext; we rely on the header to provide the correct type.
// If cmsContext is a typedef to a pointer type, nullptr will be a valid test input.
extern "C" void* MyMtxCreate(cmsContext id);

// Simple non-terminating assertion framework (no GTest).
static int test_failures = 0;

#define ASSERT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "Assertion failed: " << #cond << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++test_failures; \
    } \
} while(0)

#define ASSERT_NOT_NULL(ptr) ASSERT_TRUE((ptr) != nullptr)


// Test 1: MyMtxCreate should return a non-null handle when given a null cmsContext.
// The id parameter is not used by the function, so nullptr is a valid test input.
void test_MyMtxCreate_ReturnsNonNull_WithNullContext() {
    void* mtx = MyMtxCreate(nullptr);
    // Expect a valid (non-null) handle.
    ASSERT_NOT_NULL(mtx);
    if (mtx) {
        CloseHandle((HANDLE)mtx);
    }
    return;
}

// Test 2: Consecutive calls should produce distinct handles (two separate mutex objects).
void test_MyMtxCreate_DistinctHandles() {
    void* mtx1 = MyMtxCreate(nullptr);
    void* mtx2 = MyMtxCreate(nullptr);
    // Both should be valid and distinct.
    ASSERT_NOT_NULL(mtx1);
    ASSERT_NOT_NULL(mtx2);
    ASSERT_TRUE(mtx1 != mtx2);
    if (mtx1) CloseHandle((HANDLE)mtx1);
    if (mtx2) CloseHandle((HANDLE)mtx2);
    return;
}

// Test 3: Multiple quick successive calls should not crash or leak resources.
// Creates several mutexes and closes them, ensuring no nulls and proper cleanup.
void test_MyMtxCreate_MultipleCalls_NoLeaks() {
    const int N = 5;
    void* handles[N] = { nullptr };
    for (int i = 0; i < N; ++i) {
        handles[i] = MyMtxCreate(nullptr);
        ASSERT_NOT_NULL(handles[i]);
    }
    for (int i = 0; i < N; ++i) {
        if (handles[i]) CloseHandle((HANDLE)handles[i]);
        handles[i] = nullptr;
    }
    return;
}

int main() {
    std::cout << "Starting tests for MyMtxCreate (testthread.cpp)..." << std::endl;

    // Run tests
    test_MyMtxCreate_ReturnsNonNull_WithNullContext();
    test_MyMtxCreate_DistinctHandles();
    test_MyMtxCreate_MultipleCalls_NoLeaks();

    if (test_failures > 0) {
        std::cerr << "Total test failures: " << test_failures << std::endl;
        return 1;
    } else {
        std::cout << "All tests passed successfully." << std::endl;
        return 0;
    }
}
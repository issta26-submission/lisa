// Minimal C++11 test harness for the focal C function Type_ColorantTable_Free
// This test mocks the external dependency cmsFreeNamedColorList and verifies
// that Type_ColorantTable_Free correctly delegates freeing to the mock
// when Ptr is non-null and when Ptr is null.

// Note: This test is designed to be compiled together with the project's
// cmstypes.c file. It uses opaque forward declarations to avoid requiring
// full LCMS internal headers in the test translation unit.

#include <lcms2_internal.h>
#include <cstdio>
#include <cstdlib>


// Forward declarations to match the signatures used in cmstypes.c
extern "C" {
    // In the real project, cmsNAMEDCOLORLIST is a typedef'd struct.
    typedef struct cmsNAMEDCOLORLIST cmsNAMEDCOLORLIST;

    // The Type_ColorantTable_Free function to test
    struct _cms_typehandler_struct; // incomplete type since we don't need its layout here
    void Type_ColorantTable_Free(struct _cms_typehandler_struct* self, void* Ptr);

    // Mockable dependency: cmsFreeNamedColorList
    void cmsFreeNamedColorList(cmsNAMEDCOLORLIST* List);
}

// Global state used by the mock to verify correct behavior
static void* g_lastFreedPtr = nullptr; // last pointer passed to cmsFreeNamedColorList
static size_t g_freeCount = 0;         // how many times cmsFreeNamedColorList was invoked

// Expose a C linkage for the mock so cmstypes.c can link to it
extern "C" {

// Lightweight mock implementation of cmsFreeNamedColorList
// It records the pointer and increments a counter. If the pointer is non-null,
// it frees the memory to mimic real behavior.
void cmsFreeNamedColorList(cmsNAMEDCOLORLIST* List) {
    g_lastFreedPtr = (void*)List;
    ++g_freeCount;
    if (List != nullptr) {
        // Simulate real freeing behavior for the test's lifecycle
        free((void*)List);
    }
}
} // extern "C"

// Helper macro for non-terminating test assertions
// Logs a message and increments a failure counter if the condition is false.
static int g_total_failures = 0;
#define EXPECT(cond, msg) do { \
    if (!(cond)) { \
        fprintf(stderr, "EXPECT FAILED: %s\n", msg); \
        ++g_total_failures; \
    } \
} while (0)

// Test 1: Non-null Ptr should result in cmsFreeNamedColorList being called once
// and the exact pointer being passed through.
static void test_ColorantTable_Free_NonNullPtr() {
    g_total_failures = 0;
    g_lastFreedPtr = nullptr;
    g_freeCount = 0;

    void* data = malloc(16); // Allocate an opaque block (size irrelevant for the test)
    if (data == nullptr) {
        fprintf(stderr, "Test setup failed: malloc returned nullptr\n");
        ++g_total_failures;
        return;
    }

    // Call the focal function with a non-null Ptr
    Type_ColorantTable_Free(nullptr, data);

    // Validate that the mock was invoked exactly once and with the correct pointer
    EXPECT(g_freeCount == 1, "cmsFreeNamedColorList should be called exactly once for non-null Ptr");
    EXPECT(g_lastFreedPtr == data, "cmsFreeNamedColorList should receive the same Ptr value passed to Type_ColorantTable_Free");

    // Note: data is freed by the mock; no extra free here
    // Reset not strictly necessary since next test reinitializes state
}

// Test 2: Null Ptr should still call cmsFreeNamedColorList with nullptr
// and not crash the function.
static void test_ColorantTable_Free_NullPtr() {
    g_total_failures = 0;
    g_lastFreedPtr = (void*)0xDEADBEEF;
    g_freeCount = 0;

    // Call the focal function with a null Ptr
    Type_ColorantTable_Free(nullptr, nullptr);

    // Validate that the mock was invoked and received nullptr
    EXPECT(g_freeCount == 1, "cmsFreeNamedColorList should be called once for null Ptr");
    EXPECT(g_lastFreedPtr == nullptr, "cmsFreeNamedColorList should receive nullptr when Ptr is null");
}

// Test 3: Self parameter is non-null (edge case) to ensure no crash and proper delegation.
// The function should behave the same as Test 1, ignoring self and freeing Ptr.
static void test_ColorantTable_Free_NonNullSelf() {
    g_total_failures = 0;
    g_lastFreedPtr = nullptr;
    g_freeCount = 0;

    void* data = malloc(24);
    if (data == nullptr) {
        fprintf(stderr, "Test setup failed: malloc returned nullptr\n");
        ++g_total_failures;
        return;
    }

    // Pass a non-null self pointer to exercise the path where self is non-null
    Type_ColorantTable_Free(reinterpret_cast<struct _cms_typehandler_struct*>(0xCAFEBABE), data);

    // Validate that the mock was invoked and with the correct Ptr
    EXPECT(g_freeCount == 1, "cmsFreeNamedColorList should be called exactly once even when self is non-null");
    EXPECT(g_lastFreedPtr == data, "cmsFreeNamedColorList should receive the same Ptr value passed to Type_ColorantTable_Free");

    // Cleanup is performed by mock; don't free data here
}

// Simple runner that executes all tests and reports summary
int main() {
    printf("Running Type_ColorantTable_Free unit tests...\n");

    test_ColorantTable_Free_NonNullPtr();
    printf("Test 1 (Non-null Ptr) results: %s\n", (g_total_failures == 0 ? "PASSED" : "FAILED"));

    test_ColorantTable_Free_NullPtr();
    printf("Test 2 (Null Ptr) results: %s\n", (g_total_failures == 0 ? "PASSED" : "FAILED"));

    test_ColorantTable_Free_NonNullSelf();
    printf("Test 3 (Non-null self pointer) results: %s\n", (g_total_failures == 0 ? "PASSED" : "FAILED"));

    if (g_total_failures == 0) {
        printf("All tests PASSED\n");
        return 0;
    } else {
        printf("Some tests FAILED. Total failures: %d\n", g_total_failures);
        return 1;
    }
}
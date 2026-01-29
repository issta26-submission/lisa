/*
Unit test suite for the focal method WatchDogContext in testcms2.c

Step 1 – Program Understanding (summarized in code comments)
- Focal method watches memory context creation via cmsCreateContext(&DebugMemHandler, usr);
- If creation fails (ctx == NULL), it calls Die("Unable to create memory managed context");
- Otherwise it calls DebugMemDontCheckThis(ctx) and returns the context.
- Dependencies (from <FOCAL_CLASS_DEP>) include: cmsCreateContext, Die, DebugMemDontCheckThis, and the memory handler mechanism (DebugMemHandler).
- Core Candidate Keywords to guide test design:
  - cmsCreateContext
  - DebugMemHandler / DebugMalloc / DebugFree / DebugRealloc (memory subsystem)
  - Die (error handling path)
  - DebugMemDontCheckThis (post-creation hook)
  - cmsContext (type returned by WatchDogContext)
- Plan: build a small C++-based test harness (no GTest) that links against testcms2.c and testcms2.h, and overrides memory/Die behavior via test-provided stubs to deterministically exercise:
  - Positive path: successful cmsCreateContext, DebugMemDontCheckThis invoked, Die not invoked
  - Negative path: forced allocation failure causing cmsCreateContext to return NULL, leading to Die being invoked
  - Use setjmp/longjmp to safely capture Die invocation without terminating the test process
- Important constraints honored:
  - No GTest; use a lightweight, self-contained test harness in main
  - Use non-terminating assertions where possible; when exercising negative path we’ll use controlled longjmp to assert outcomes
  - Static/global test helpers implemented in test file (mimicking how static functions would be used within the focal file)
  - Import dependencies and use proper cross-language linking (C/C++) with extern "C"
*/

#include <cstring>
#include <cstdio>
#include <testcms2.h>
#include <setjmp.h>
#include <cstdlib>
#include <cstdarg>


// Domain knowledge: expose the necessary types to the C-based focal function.
// We assume cmsContext is a pointer-type; for the test harness we treat it as a void*.
typedef void* cmsContext;

// Forward declaration of the focal method under test.
// We assume the signature in C translates to a compatible C++ declaration.
extern "C" cmsContext WatchDogContext(void* usr);

// Global state used by test doubles to control behavior and verify interactions.
static int g_fail_alloc = 0;            // When non-zero, simulate allocation failure in DebugMalloc
static int g_die_called = 0;              // Tracks if Die() was invoked
static int g_memDontCheckCalled = 0;      // Tracks if DebugMemDontCheckThis() was invoked
static jmp_buf g_watchdog_jmp;              // Jump buffer to catch Die() longjmp

// Provide test doubles (memory management and error handling) with C linkage.
// These functions are expected to be resolved by the linker against testcms2.c's usage
// of the DebugMemHandler and its callbacks (DebugMalloc, DebugFree, etc.).

extern "C" {

    // Simulated DebugMalloc: either returns a valid block or NULL to simulate OOM.
    void* DebugMalloc(cmsContext ContextID, unsigned int size)
    {
        (void)ContextID;
        if (g_fail_alloc) {
            return NULL;
        }
        return malloc(size);
    }

    // Simulated DebugFree: straightforward wrapper
    void DebugFree(cmsContext ContextID, void* Ptr)
    {
        (void)ContextID;
        free(Ptr);
    }

    // Simulated DebugRealloc: simple wrapper around realloc
    void* DebugRealloc(cmsContext ContextID, void* Ptr, unsigned int NewSize)
    {
        (void)ContextID;
        if (Ptr == NULL) {
            return DebugMalloc(ContextID, NewSize);
        }
        return realloc(Ptr, NewSize);
    }

    // DebugMemDontCheckThis: record that the post-create hook was invoked
    void DebugMemDontCheckThis(void* Ptr)
    {
        (void)Ptr;
        g_memDontCheckCalled = 1;
    }

    // Die: intercept error path and longjmp back to test harness
    void Die(const char* Reason, ...)
    {
        (void)Reason;
        g_die_called = 1;
        // Shift control back to the test harness
        longjmp(g_watchdog_jmp, 1);
    }
}

// Helper test utilities
static void reset_test_state()
{
    g_fail_alloc = 0;
    g_die_called = 0;
    g_memDontCheckCalled = 0;
}

// Test 1: Positive path – ensure WatchDogContext returns a non-NULL context
// and that DebugMemDontCheckThis() is invoked, while Die() is not called.
static bool test_WatchDogContext_PositivePath()
{
    reset_test_state();
    // Call the focal method with a dummy user pointer
    cmsContext ctx = WatchDogContext((void*)0x1234);

    bool passed = (ctx != NULL) && (g_die_called == 0) && (g_memDontCheckCalled == 1);
    // In a real test, we'd also clean up the created context if required by the API.
    if (passed) {
        // If the real CMS allocates resources, consider freeing if safe.
        // Note: We do not know the exact deallocation function here; skip to avoid side effects.
    }

    if (!passed) {
        fprintf(stderr, "Test_WatchDogContext_PositivePath: FAILED\n");
        if (ctx == NULL) fprintf(stderr, "  Reason: cmsContext is NULL on success path.\n");
        if (g_die_called) fprintf(stderr, "  Reason: Die() was unexpectedly called on success path.\n");
        if (!g_memDontCheckCalled) fprintf(stderr, "  Reason: DebugMemDontCheckThis() was not invoked on success path.\n");
    } else {
        fprintf(stdout, "Test_WatchDogContext_PositivePath: PASSED\n");
    }
    return passed;
}

// Test 2: Negative path – simulate allocation failure to force cmsCreateContext to NULL
// and verify that Die() is invoked via longjmp control flow.
static bool test_WatchDogContext_NegativeAllocation()
{
    // Prepare to catch longjmp from Die()
    g_fail_alloc = 1;          // Force DebugMalloc to fail, causing cmsCreateContext to fail
    g_die_called = 0;
    g_memDontCheckCalled = 0;

    int jump = setjmp(g_watchdog_jmp);
    if (jump == 0) {
        // Attempt to create the context; this should fail and trigger Die()
        WatchDogContext((void*)0);
        // If we reach here, the code did not longjmp as expected; treat as failure
        fprintf(stderr, "Test_WatchDogContext_NegativeAllocation: Die() not triggered as expected\n");
        return false;
    } else {
        // Returned here via longjmp from Die()
        bool passed = (g_die_called == 1);
        if (passed) {
            fprintf(stdout, "Test_WatchDogContext_NegativeAllocation: PASSED\n");
        } else {
            fprintf(stderr, "Test_WatchDogContext_NegativeAllocation: FAILED (Die() not invoked as expected)\n");
        }
        return passed;
    }
}

// Public entrypoint (optional): run tests from main
int main(void)
{
    // Basic alignment with domain knowledge: run tests manually from main (no GTest)
    bool ok1 = test_WatchDogContext_PositivePath();
    bool ok2 = test_WatchDogContext_NegativeAllocation();

    if (ok1 && ok2) {
        printf("All tests PASSED.\n");
        return 0;
    } else {
        printf("Some tests FAILED. See above for details.\n");
        return 1;
    }
}
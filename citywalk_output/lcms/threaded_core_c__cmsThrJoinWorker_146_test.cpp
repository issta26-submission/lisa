// Threaded Core Test Suite for _cmsThrJoinWorker
// Description:
// This test suite exercises the focal function _cmsThrJoinWorker (Linux/non-Windows path)
// which uses POSIX pthread_join to join a worker thread created via _cmsThrCreateWorker.
//
// Assumptions:
// - The project provides the threaded_core.c implementation and the corresponding header(s)
//   exposing _cmsThrCreateWorker, _cmsThrJoinWorker, and related types.
// - The test is compiled with a C++11 compiler but does not use any external test framework.
// - We rely on non-terminating assertions to maximize code execution and coverage.
// - We focus on verifying a successful join path and a failure path by passing an invalid handle.
// - The test uses only standard C/C++ facilities for setup/verification.
//
// Note: If the actual project uses different type names or headers, adjust the extern "C" prototypes
// accordingly to match the real API.

#include <cstring>
#include <cstdio>
#include <pthread.h>
#include <threaded_internal.h>
#include <cstdlib>
#include <windows.h>
#include <unistd.h>
#include <cstdint>


// Lightweight test harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define TEST_PASSED(cond, msg) do { \
    ++g_tests_run; \
    if(!(cond)) { \
        fprintf(stderr, "Test failed: %s\n", msg); \
        ++g_tests_failed; \
    } \
} while(0)

extern "C" {

// The project headers are expected to declare the following types and functions.
// We provide minimal extern "C" prototypes here to compile the test against the real implementation.
// If your build uses different typedefs, adjust these accordingly.

typedef void* cmsContext;
typedef void* cmsHANDLE;

// Function pointer type used by the worker adaptor in the threaded core.
typedef void (*_cmsTransform2Fn)(void* CMMcargo, void* InputBuffer, void* OutputBuffer,
                                int PixelsPerLine, int LineCount, int Stride);

// Work slice structure used to pass parameters to the worker.
// The fields mirror usage in threaded_core.c (see thread_adaptor).
typedef struct _cmsWorkSlice {
    void* CMMcargo;
    void* InputBuffer;
    void* OutputBuffer;
    int   PixelsPerLine;
    int   LineCount;
    int   Stride;
} _cmsWorkSlice;

// Focal API
cmsHANDLE _cmsThrCreateWorker(cmsContext ContextID, _cmsTransform2Fn worker, _cmsWorkSlice* param);
void      _cmsThrJoinWorker(cmsContext ContextID, cmsHANDLE hWorker);

// Optional: signalling error (not used directly in tests, but declared for completeness)
void cmsSignalError(cmsContext ContextID, int cmsERROR_UNDEFINED, const char* fmt, ...);

}

// A simple worker function to be passed to _cmsThrCreateWorker.
// It writes a predefined value into the CMMcargo buffer to verify the worker ran.
static void testWorker(void* CMMcargo, void* InputBuffer, void* OutputBuffer,
                       int PixelsPerLine, int LineCount, int Stride)
{
    (void)InputBuffer; (void)OutputBuffer;
    (void)PixelsPerLine; (void)LineCount; (void)Stride;
    if (CMMcargo != nullptr) {
        // Write a recognizable value to confirm the worker executed.
        volatile int* target = (volatile int*)CMMcargo;
        *target = 0xDEADBEEF;
    }
}

// Test 1: Successful creation and joining of a worker thread.
// This verifies that the worker runs and _cmsThrJoinWorker returns without error.
static void test_join_worker_succeeds(void)
{
    // Explanation:
    // - Prepare a simple int to receive a result from the worker.
    // - Create a worker with the testWorker function and a pointer to the result.
    // - Join the worker and verify the result value was set by the worker.
    int result = 0;
    _cmsWorkSlice slice;
    slice.CMMcargo = &result;
    slice.InputBuffer = nullptr;
    slice.OutputBuffer = nullptr;
    slice.PixelsPerLine = 0;
    slice.LineCount = 1;
    slice.Stride = 0;

    cmsContext ctx = (cmsContext)0x1; // dummy context for testing

    cmsHANDLE h = _cmsThrCreateWorker(ctx, testWorker, &slice);
    TEST_PASSED(h != nullptr, "Worker handle should be non-null on successful creation");

    if (h != nullptr) {
        _cmsThrJoinWorker(ctx, h);
        // After join, the worker should have executed and set result to 0xDEADBEEF
        TEST_PASSED(result == 0xDEADBEEF, "Worker should have executed and written 0xDEADBEEF to result");
    } else {
        // If creation failed, report via test harness
        TEST_PASSED(false, "Worker creation failed; cannot perform join test");
    }
}

// Test 2: Joining with an invalid/garbage handle should not crash.
// This tests the error path in the non-Windows (POSIX) implementation where pthread_join
// would fail and cmsSignalError would be invoked. We can't observe the side-effect here easily,
// but we can ensure the call does not crash or terminate the test process.
static void test_join_invalid_handle(void)
{
    // Explanation:
    // - Call join with an obviously invalid handle to exercise the error path.
    // - We do not assert on side-effects of cmsSignalError here, as it is part of the library's error handling.
    // - The primary goal is to ensure the function handles invalid inputs gracefully.
    cmsContext ctx = (cmsContext)0x1;
    cmsHANDLE invalid = (cmsHANDLE)0xDEADBEEF;
    _cmsThrJoinWorker(ctx, invalid);

    // If we reach here, we consider the test "passed" for coverage purposes.
    TEST_PASSED(true, "Join with invalid handle did not crash");
}

// Entry point for the test suite.
int main(void)
{
    test_join_worker_succeeds();
    test_join_invalid_handle();

    // Summary
    printf("Test summary: total=%d, failed=%d\n", g_tests_run, g_tests_failed);

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}
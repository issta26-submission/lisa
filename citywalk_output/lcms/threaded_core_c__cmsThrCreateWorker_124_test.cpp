// Unit test suite for cmsThrCreateWorker (focal method) in threaded_core.c
// Target: C++11 environment, no GTest. The test harness is self-contained
// and uses a lightweight assertion mechanism. It mocks minimal external
// dependencies required by cmsThrCreateWorker (allocator, error signaling) at
// link time to enable deterministic tests.
//
// Notes:
// - Linux pthread path is exercised (CMS_IS_WINDOWS_ path is not compiled here).
// - We test the success path and the allocation-failure path.
// - The test uses a heap-allocated _cmsWorkSlice to ensure lifetime beyond
//   the thread creation call. The thread will access this slice via the adaptor
//   before returning, and we leak-free it after giving the worker time to run.
// - We do not rely on GMock or GoogleTest; we implement simple assertion helpers.

#include <cstring>
#include <thread>
#include <pthread.h>
#include <chrono>
#include <threaded_internal.h>
#include <iostream>
#include <stdarg.h>
#include <stdint.h>
#include <sys/types.h>
#include <cstdlib>
#include <windows.h>
#include <unistd.h>


// -------------------------------------------------------------------------------------
// Minimal C interface that cmsThrCreateWorker expects (mirroring the focal code's API).
// We declare these as extern "C" to avoid C++ name mangling during linkage with
// threaded_core.c, which is a C source file.
// -------------------------------------------------------------------------------------

extern "C" {

// Typedefs matching the focal file's expectations for Linux path.
typedef void (*_cmsTransform2Fn)(void* CMMcargo, void* InputBuffer, void* OutputBuffer,
                                int PixelsPerLine, int LineCount, int Stride);

typedef struct _cmsWorkSlice {
    void* CMMcargo;
    void* InputBuffer;
    void* OutputBuffer;
    int   PixelsPerLine;
    int   LineCount;
    int   Stride;
} _cmsWorkSlice;

typedef void* cmsHANDLE;
typedef void* cmsContext;

// External allocator/signalling used by cmsThrCreateWorker (to be mocked here).
void* _cmsMalloc(int pool, size_t size);
void  _cmsFree(int pool, void* ptr);
void  cmsSignalError(cmsContext ContextID, int ErrorCode, const char* fmt, ...);

cmsHANDLE _cmsThrCreateWorker(cmsContext ContextID, _cmsTransform2Fn worker, _cmsWorkSlice* param);

} // extern "C"

// -------------------------------------------------------------------------------------
// Mocked/Controlled environment to drive cmsThrCreateWorker behavior.
// We provide:
// - A malloc/freelike allocator that can be toggled to simulate allocation failure.
// - A simple error capture function to observe error signaling.
// - A test worker function that flips a flag when invoked.
// -------------------------------------------------------------------------------------

static int gSimulateMallocFailure = 0;
static int gLastErrorCode = 0;

// Simple cookie-based malloc replacement
extern "C" void* _cmsMalloc(int pool, size_t size) {
    (void)pool; // unused in tests
    if (gSimulateMallocFailure) {
        return NULL;
    }
    return malloc(size);
}

extern "C" void _cmsFree(int pool, void* ptr) {
    (void)pool; // unused in tests
    free(ptr);
}

// Capture error code when cmsSignalError is invoked
extern "C" void cmsSignalError(cmsContext ContextID, int ErrorCode, const char* fmt, ...) {
    (void)ContextID;
    (void)fmt;
    gLastErrorCode = ErrorCode;
}

// Accessor for tests to inspect last error
extern "C" int cmsGetLastErrorCode() {
    return gLastErrorCode;
}

// Test worker function: sets the provided flag via CMMcargo
extern "C" void testWorker(void* CMMcargo, void* InputBuffer, void* OutputBuffer,
                         int PixelsPerLine, int LineCount, int Stride) {
    (void)InputBuffer; (void)OutputBuffer;
    (void)PixelsPerLine; (void)LineCount; (void)Stride;
    int* flagPtr = (int*)CMMcargo;
    if (flagPtr) {
        *flagPtr = 1;
    }
}

// -------------------------------------------------------------------------------------
// Lightweight test framework (no GTest; minimal assertions with descriptive output).
// -------------------------------------------------------------------------------------

static int gTestsRun = 0;
static int gTestsFailed = 0;

#define ASSERT_TRUE(cond, msg) do { \
    gTestsRun++; \
    if (!(cond)) { \
        ++gTestsFailed; \
        std::cerr << "[FAIL] " << msg << " (line " << __LINE__ << ")" << std::endl; \
    } else { \
        std::cout << "[PASS] " << msg << std::endl; \
    } \
} while(0)

#define ASSERT_NOT_NULL(ptr, msg) ASSERT_TRUE((ptr) != nullptr, msg)

// -------------------------------------------------------------------------------------
// Test helpers
// -------------------------------------------------------------------------------------

// Helper to cleanly sleep for a short duration to allow the worker thread to run.
static void SleepMs(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// -------------------------------------------------------------------------------------
// Test 1: Success path - normal thread creation should return a non-null handle and
// the worker should be invoked, flipping the test flag.
// -------------------------------------------------------------------------------------
static void Test_CreateWorker_Success() {
    // Reset allocator/error state
    gSimulateMallocFailure = 0;
    gLastErrorCode = 0;

    // Allocate parameter on heap to ensure lifetime beyond cmsThrCreateWorker call
    _cmsWorkSlice* param = (_cmsWorkSlice*)malloc(sizeof(_cmsWorkSlice));
    ASSERT_NOT_NULL(param, "Allocate _cmsWorkSlice for test");

    int flag = 0;
    param->CMMcargo     = &flag;
    param->InputBuffer  = nullptr;
    param->OutputBuffer = nullptr;
    param->PixelsPerLine = 0;
    param->LineCount    = 0;
    param->Stride       = 0;

    cmsHANDLE h = _cmsThrCreateWorker(nullptr, testWorker, param);
    ASSERT_NOT_NULL(h, "cmsThrCreateWorker should return non-null handle on success");

    // Wait a short while for the worker to run
    SleepMs(200);

    // Verify the worker ran and modified the flag
    ASSERT_TRUE(flag == 1, "Worker should have been invoked and set flag to 1");

    // Clean up: param memory remains allocated by test (not freed by library in thread adaptor)
    free(param);
}

// -------------------------------------------------------------------------------------
// Test 2: Allocation failure path - simulate _cmsMalloc returning NULL for the adaptor
// memory; cmsThrCreateWorker should return NULL and not crash.
// -------------------------------------------------------------------------------------
static void Test_CreateWorker_AlloationFailure() {
    // Configure allocator to fail
    gSimulateMallocFailure = 1;
    gLastErrorCode = 0;

    _cmsWorkSlice* param = (_cmsWorkSlice*)malloc(sizeof(_cmsWorkSlice));
    ASSERT_NOT_NULL(param, "Allocate _cmsWorkSlice for allocation-failure test");

    int flag = 0;
    param->CMMcargo     = &flag;
    param->InputBuffer  = nullptr;
    param->OutputBuffer = nullptr;
    param->PixelsPerLine = 0;
    param->LineCount    = 0;
    param->Stride       = 0;

    cmsHANDLE h = _cmsThrCreateWorker(nullptr, testWorker, param);
    ASSERT_TRUE(h == nullptr, "cmsThrCreateWorker should return NULL when _cmsMalloc fails");

    // Cleanup (no thread should have been created, but param memory is still heap-allocated)
    free(param);

    // Reset allocator flag for subsequent tests
    gSimulateMallocFailure = 0;
}

// -------------------------------------------------------------------------------------
// Step 3: Test suite launcher
// -------------------------------------------------------------------------------------

static void RunAllTests() {
    std::cout << "Starting cmsThrCreateWorker unit tests (C++11)..." << std::endl;
    Test_CreateWorker_Success();
    Test_CreateWorker_AlloationFailure();
    std::cout << "Tests run: " << gTestsRun << ", Failures: " << gTestsFailed << std::endl;
}

// -------------------------------------------------------------------------------------
// Main
// -------------------------------------------------------------------------------------

int main() {
    RunAllTests();
    if (gTestsFailed > 0) {
        std::cerr << "Some tests failed." << std::endl;
        return 1;
    }
    std::cout << "All tests passed." << std::endl;
    return 0;
}
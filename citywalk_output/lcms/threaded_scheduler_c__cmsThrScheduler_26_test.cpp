// Test suite for the focal method _cmsThrScheduler in threaded_scheduler.c
// This test suite is written for C++11 without Google Test.
// It uses a small, self-contained test harness and mock implementations
// of the dependent functions used by _cmsThrScheduler to exercise true/false
// branches and single/multi-threaded behavior.
// The tests are designed to be non-terminating (no assertion aborts) and
// report results via a simple console output.

#include <cstring>
#include <thread>
#include <vector>
#include <threaded_internal.h>
#include <iostream>
#include <cstdlib>


// The focal function under test is declared with C linkage in the original code.
// We provide a forward declaration here to enable linking to threaded_scheduler.c.
extern "C" void _cmsThrScheduler(struct _cmstransform_struct* CMMcargo,
                       const void* InputBuffer,
                       void* OutputBuffer,
                       unsigned int PixelsPerLine,
                       unsigned int LineCount,
                       const struct cmsStride* Stride);

// Forward-declare the types used by the focal function.
// We keep these definitions intentionally minimal (opaque to the test) to
// avoid depending on the real library internals.
struct _cmstransform_struct { int dummy; };          // opaque transform struct
struct _cmsWorkSlice;                                 // forward declaration (real layout in library)
typedef void* cmsHANDLE;
typedef unsigned int cmsUInt32Number;
typedef int cmsInt32Number;
typedef bool cmsBool;

// Simple stride type (opaque in the test context)
typedef struct { int dummy; } cmsStride;

// A minimal, compatible function pointer type for the worker
typedef void (*_cmsTransform2Fn)(struct _cmstransform_struct*, const void*, void*, cmsUInt32Number, cmsUInt32Number, const cmsStride*);

// Global test hooks and state to drive mock behavior
static int g_test_nSlicesRequested = 1;          // number of slices that _cmsThrCountSlices should return
static bool g_splits_should_succeed = false;     // whether _cmsThrSplitWork should report success
static int  g_workerCalls = 0;                   // how many times the worker function is invoked
static int  g_thrCreateCalls = 0;                // how many times a worker thread is created
static int  g_thrJoinCalls = 0;                  // how many times a worker thread is joined
static cmsStride g_dummyStride = {0};            // dummy stride value (copied into FixedStride)

static cmsContext g_last_worker_context = 0;
static void*      g_last_worker_input = nullptr;
static void*      g_last_worker_output = nullptr;
static cmsUInt32Number g_last_worker_ppx = 0;
static cmsUInt32Number g_last_worker_linecount = 0;

// Forward declaration of the worker function used by the scheduler
static void test_worker(struct _cmstransform_struct* CMMcargo,
                        const void* InputBuffer,
                        void* OutputBuffer,
                        cmsUInt32Number PixelsPerLine,
                        cmsUInt32Number LineCount,
                        const cmsStride* Stride);

// Typedef of a generic worker function pointer
typedef void (*_cmsTransform2Fn)(struct _cmstransform_struct*, const void*, void*, cmsUInt32Number, cmsUInt32Number, const cmsStride*);

// Mocks for library functions used by _cmsThrScheduler

// cmsGetTransformContextID
extern "C" cmsContext cmsGetTransformContextID(struct _cmstransform_struct* /*CMMcargo*/) {
    return 1; // arbitrary non-zero context
}

// _cmsGetTransformWorker
extern "C" _cmsTransform2Fn _cmsGetTransformWorker(struct _cmstransform_struct* /*CMMcargo*/) {
    return &test_worker; // point to our test worker
}

// _cmsGetTransformMaxWorkers
extern "C" cmsInt32Number _cmsGetTransformMaxWorkers(struct _cmstransform_struct* /*CMMcargo*/) {
    return 4; // allow up to 4 workers; tests control via g_maxWorkers (implicitly via test_nSlices)
}

// _cmsThrCountSlices
extern "C" cmsUInt32Number _cmsThrCountSlices(struct _cmstransform_struct* /*CMMcargo*/,
                                              cmsInt32Number /*MaxWorkers*/,
                                              cmsUInt32Number /*PixelsPerLine*/,
                                              cmsUInt32Number /*LineCount*/,
                                              const cmsStride* /*FixedStride*/) {
    // Copy the test stride (to simulate FixedStride behavior) and return requested slices
    // (FixedStride is not used by the test worker, but we preserve interface realism)
    // If FixedStride is needed by the library in other tests, this can be extended.
    (void)&g_dummyStride; // silence unused warning if needed
    return (cmsUInt32Number)g_test_nSlicesRequested;
}

// _cmsCalloc
extern "C" void* _cmsCalloc(cmsContext /*ContextID*/, size_t n, size_t size) {
    return calloc(n, size);
}

// _cmsFree
extern "C" void _cmsFree(cmsContext /*ContextID*/, void* ptr) {
    free(ptr);
}

// _cmsThrSplitWork
extern "C" cmsBool _cmsThrSplitWork(_cmsWorkSlice* /*master*/, cmsUInt32Number /*nSlices*/, _cmsWorkSlice* /*slices*/) {
    // Do not rely on the real structure. Return the configured value.
    return (cmsBool)g_splits_should_succeed;
}

// _cmsThrCreateWorker
extern "C" cmsHANDLE _cmsThrCreateWorker(cmsContext /*ContextID*/, _cmsTransform2Fn worker, _cmsWorkSlice* /*slice*/) {
    (void)worker; // unused in this mock (test_worker is the actual worker)
    g_thrCreateCalls++;

    // Launch a dedicated thread that invokes the worker with minimal, null data.
    // The worker implementation is robust to null pointers and simply increments the count.
    cmsContext ctx = ContextID_dummy(); // placeholder; not actually used
    (void)ctx;
    std::thread* t = new std::thread([=]() {
        // Use null values; the test worker handles nulls gracefully
        worker(nullptr, nullptr, nullptr, 0, 0, nullptr);
    });
    return (cmsHANDLE)t;
}

// Helper to provide a compatible dummy context extraction (not used by the mock)
static cmsContext ContextID_dummy() { return 1; }

// _cmsThrJoinWorker
extern "C" void _cmsThrJoinWorker(cmsContext /*ContextID*/, cmsHANDLE thread) {
    g_thrJoinCalls++;
    if (thread) {
        std::thread* t = (std::thread*)thread;
        if (t->joinable()) t->join();
        delete t;
    }
}

// The worker function used by the scheduler in tests
static void test_worker(struct _cmstransform_struct* /*CMMcargo*/,
                        const void* InputBuffer,
                        void* OutputBuffer,
                        cmsUInt32Number /*PixelsPerLine*/,
                        cmsUInt32Number /*LineCount*/,
                        const cmsStride* /*Stride*/) {
    // Increment invocation count and record some info about the call
    g_workerCalls++;
    g_last_worker_input = (void*)InputBuffer;
    g_last_worker_output = OutputBuffer;
    // We deliberately do not dereference or inspect user buffers to keep this mock safe
    // If a real buffer is provided, we mark its first byte to verify the function was called.
    if (OutputBuffer) {
        char* p = (char*)OutputBuffer;
        p[0] = 'W'; // marker to indicate worker ran
    }
}

// Helper to reset test state between tests
static void reset_test_state() {
    g_test_nSlicesRequested = 1;
    g_splits_should_succeed = false;
    g_workerCalls = 0;
    g_thrCreateCalls = 0;
    g_thrJoinCalls = 0;
    std::memset(&g_dummyStride, 0, sizeof(g_dummyStride));
    g_last_worker_input = nullptr;
    g_last_worker_output = nullptr;
    g_last_worker_ppx = 0;
    g_last_worker_linecount = 0;
}

// Unit tests

// Test 1: Direct single-slice path (nSlices <= 1). Verifies that the worker is invoked exactly once
static bool test_direct_single_slice_path() {
    reset_test_state();
    g_test_nSlicesRequested = 1;          // Force single slice
    g_splits_should_succeed = false;      // Irrelevant for nSlices=1, but harmless
    const char inputBuf[] = "IN";
    char outputBuf[8];
    std::memset(outputBuf, 0, sizeof(outputBuf));

    struct _cmstransform_struct CMMcargo;
    cmsStride stride = {0};

    // Invoke the focal method
    _cmsThrScheduler(&CMMcargo,
                     (const void*)inputBuf,
                     (void*)outputBuf,
                     4,            // PixelsPerLine (arbitrary)
                     2,            // LineCount (arbitrary)
                     &stride);

    // Assertions
    bool passed = true;
    if (g_workerCalls != 1) {
        std::cerr << "Test 1 failed: expected 1 worker call, got " << g_workerCalls << "\n";
        passed = false;
    }
    // Worker should have been given a non-null OutputBuffer (to allow the 'W' marker)
    if (outputBuf[0] != 'W') {
        std::cerr << "Test 1 failed: worker did not write to OutputBuffer as expected.\n";
        passed = false;
    }
    return passed;
}

// Test 2: Multi-slice path with splitting enabled (nSlices > 1 and _cmsThrSplitWork returns true)
static bool test_multi_slice_path_with_split_true() {
    reset_test_state();
    g_test_nSlicesRequested = 3;         // Request 3 slices
    g_splits_should_succeed = true;       // Force splitting to succeed -> multi-thread path
    const char inputBuf[] = "IN";
    char outputBuf[8];
    std::memset(outputBuf, 0, sizeof(outputBuf));

    struct _cmstransform_struct CMMcargo;
    cmsStride stride = {0};

    _cmsThrScheduler(&CMMcargo,
                     (const void*)inputBuf,
                     (void*)outputBuf,
                     4,
                     2,
                     &stride);

    // Assertions
    bool passed = true;
    if (g_workerCalls != 3) {
        std::cerr << "Test 2 failed: expected 3 worker calls (1 main + 2 threads), got " << g_workerCalls << "\n";
        passed = false;
    }
    if (g_thrCreateCalls != 2) {
        std::cerr << "Test 2 failed: expected 2 thread creations, got " << g_thrCreateCalls << "\n";
        passed = false;
    }
    // We still test that the code executes; content of OutputBuffer from threads is not strictly required here
    return passed;
}

// Test 3: Multi-slice path with splitting disabled (nSlices > 1 but _cmsThrSplitWork returns false)
// Verifies that the code falls back to a single-threaded, non-split path and calls the worker exactly once
static bool test_multi_slice_path_without_split() {
    reset_test_state();
    g_test_nSlicesRequested = 3;        // Request 3 slices
    g_splits_should_succeed = false;     // Force split to fail
    const char inputBuf[] = "IN";
    char outputBuf[8];
    std::memset(outputBuf, 0, sizeof(outputBuf));

    struct _cmstransform_struct CMMcargo;
    cmsStride stride = {0};

    _cmsThrScheduler(&CMMcargo,
                     (const void*)inputBuf,
                     (void*)outputBuf,
                     4,
                     2,
                     &stride);

    // Assertions
    bool passed = true;
    if (g_workerCalls != 1) {
        std::cerr << "Test 3 failed: expected 1 worker call (no split), got " << g_workerCalls << "\n";
        passed = false;
    }
    if (g_thrCreateCalls != 0) {
        std::cerr << "Test 3 failed: expected 0 thread creations when split fails, got " << g_thrCreateCalls << "\n";
        passed = false;
    }
    // Even in this path, worker should have been invoked and may write to OutputBuffer
    // Since the test worker prints a marker to OutputBuffer, we can check it if non-null
    // (Some library paths may pass nullptr; we simply ensure no crash and one worker call.)
    return passed;
}

// Main function: Runs all tests and reports a summary.
// As per domain knowledge, test methods are invoked from main in lieu of a test framework.
int main() {
    int total = 3;
    int passed = 0;

    std::cout << "Starting tests for _cmsThrScheduler (threaded_scheduler.c) using a lightweight harness...\n";

    if (test_direct_single_slice_path()) {
        std::cout << "Test 1 (direct single-slice path) PASSED.\n";
        ++passed;
    } else {
        std::cout << "Test 1 (direct single-slice path) FAILED.\n";
    }

    if (test_multi_slice_path_with_split_true()) {
        std::cout << "Test 2 (multi-slice path with split=true) PASSED.\n";
        ++passed;
    } else {
        std::cout << "Test 2 (multi-slice path with split=true) FAILED.\n";
    }

    if (test_multi_slice_path_without_split()) {
        std::cout << "Test 3 (multi-slice path with split=false) PASSED.\n";
        ++passed;
    } else {
        std::cout << "Test 3 (multi-slice path with split=false) FAILED.\n";
    }

    std::cout << "Tests completed: " << passed << "/" << total << " passed.\n";
    return (passed == total) ? 0 : 1;
}
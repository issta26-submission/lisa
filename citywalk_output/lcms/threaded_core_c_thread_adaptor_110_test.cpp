// C++11 test suite for the focal method: thread_adaptor
// This test suite is designed to be compiled with a C++11 compiler.
// It includes the C source file that defines thread_adaptor (non-Windows path assumed).
// The test uses a minimal, self-contained harness (no Google Test) and prints PASS/FAIL.
// Explanatory comments are provided for each test case.

/*
Assumptions and testing strategy:
- We compile in a non-Windows environment, so the non-Windows version of thread_adaptor is used.
- We include the C source threaded_core.c in this translation unit with C linkage to ensure symbol compatibility.
- We provide a mock _cmsFree and a mock worker function to verify that thread_adaptor calls the worker
  with the correct parameters and frees the allocated parameter struct.
- We rely on the actual _cmsWorkSlice type being defined by threaded_core.c (via its headers).
- We implement two test scenarios to validate correct propagation of parameters and proper freeing of the param object.
*/

// Define Windows macro to force non-Windows path of the focal function when including the C source.
#define CMS_IS_WINDOWS_ 0

#include <cstring>
#include <pthread.h>
#include <threaded_internal.h>
#include <iostream>
#include <threaded_core.c>
#include <cstdlib>
#include <windows.h>
#include <unistd.h>


// Include the focal C source under C linkage to test the real function.
extern "C" {
}

// We also provide a test-scoped typedef matching the worker function type.
// The actual type is expected to be exposed by threaded_core.c via its headers.
typedef void (*_cmsTransform2Fn)(void* cargo, void* InputBuffer, void* OutputBuffer,
                                int PixelsPerLine, int LineCount, int Stride);

// Global test-state for verifying worker invocation and memory freeing.
static bool worker_called = false;
static void* observed_CMMcargo = nullptr;
static void* observed_InputBuffer = nullptr;
static void* observed_OutputBuffer = nullptr;
static int observed_PixelsPerLine = 0;
static int observed_LineCount = 0;
static int observed_Stride = 0;

static void* lastFreedPtr = nullptr;
static int freedCount = 0;

// Mock worker: records all parameters passed by thread_adaptor.
static void mock_worker(void* cargo, void* InputBuffer, void* OutputBuffer,
                        int PixelsPerLine, int LineCount, int Stride)
{
    worker_called = true;
    observed_CMMcargo = cargo;
    observed_InputBuffer = InputBuffer;
    observed_OutputBuffer = OutputBuffer;
    observed_PixelsPerLine = PixelsPerLine;
    observed_LineCount = LineCount;
    observed_Stride = Stride;
}

// Mock _cmsFree: records the pointer that was freed and increments a counter.
extern "C" void _cmsFree(void* ContextID, void* Ptr)
{
    (void)ContextID; // unused in test
    lastFreedPtr = Ptr;
    ++freedCount;
}

// Utility: reset all test-state before each test.
static void reset_state()
{
    worker_called = false;
    observed_CMMcargo = nullptr;
    observed_InputBuffer = nullptr;
    observed_OutputBuffer = nullptr;
    observed_OutputBuffer = nullptr;
    observed_PixelsPerLine = 0;
    observed_LineCount = 0;
    observed_Stride = 0;
    lastFreedPtr = nullptr;
    freedCount = 0;
}

// Test 1: Basic invocation should call worker with slice values and free the param struct
static bool test_thread_adaptor_basic_invocation()
{
    reset_state();

    // Prepare a sample _cmsWorkSlice
    _cmsWorkSlice s;
    s.CMMcargo = (void*)0xA1A1U;
    s.InputBuffer = (void*)0xB2B2U;
    s.OutputBuffer = (void*)0xC3C3U;
    s.PixelsPerLine = 11;
    s.LineCount = 2;
    s.Stride = 33;

    // Prepare thread_adaptor_param
    thread_adaptor_param* p = (thread_adaptor_param*)malloc(sizeof(thread_adaptor_param));
    if (p == nullptr) return false;
    p->worker = mock_worker;
    p->param = &s;

    // Call the focal function
    void* ret = thread_adaptor((void*)p);

    // Assertions (non-terminating, printable in test harness)
    bool ok = (ret == nullptr) &&
              worker_called &&
              lastFreedPtr == (void*)p &&
              observed_CMMcargo == s.CMMcargo &&
              observed_InputBuffer == s.InputBuffer &&
              observed_OutputBuffer == s.OutputBuffer &&
              observed_PixelsPerLine == s.PixelsPerLine &&
              observed_LineCount == s.LineCount &&
              observed_Stride == s.Stride &&
              freedCount == 1;

    // Cleanup: In normal flow, _cmsFree frees p; no additional delete needed here.
    if (!ok) {
        std::cerr << "test_thread_adaptor_basic_invocation: FAILED\n";
        // Debug prints to help diagnose
        std::cerr << "ret=" << ret
                  << " worker_called=" << worker_called
                  << " freedCount=" << freedCount
                  << " lastFreedPtr=" << lastFreedPtr
                  << " observed_PixelsPerLine=" << observed_PixelsPerLine
                  << " s.PixelsPerLine=" << s.PixelsPerLine
                  << std::endl;
    }

    return ok;
}

// Test 2: Ensure multiple distinct parameters are correctly passed to worker and freed
static bool test_thread_adaptor_multiple_params()
{
    reset_state();

    // First parameter instance
    _cmsWorkSlice s1;
    s1.CMMcargo = (void*)0x11111111;
    s1.InputBuffer = (void*)0x22222222;
    s1.OutputBuffer = (void*)0x33333333;
    s1.PixelsPerLine = 5;
    s1.LineCount = 10;
    s1.Stride = 15;

    thread_adaptor_param* p1 = (thread_adaptor_param*)malloc(sizeof(thread_adaptor_param));
    if (p1 == nullptr) return false;
    p1->worker = mock_worker;
    p1->param = &s1;

    // Call first time
    void* ret1 = thread_adaptor((void*)p1);

    // Validate first call
    bool ok1 = (ret1 == nullptr) &&
               worker_called &&
               lastFreedPtr == (void*)p1 &&
               observed_CMMcargo == s1.CMMcargo &&
               observed_InputBuffer == s1.InputBuffer &&
               observed_OutputBuffer == s1.OutputBuffer &&
               observed_PixelsPerLine == s1.PixelsPerLine &&
               observed_LineCount == s1.LineCount &&
               observed_Stride == s1.Stride &&
               freedCount == 1; // only one free call so far

    // Prepare second parameter instance
    reset_state();
    _cmsWorkSlice s2;
    s2.CMMcargo = (void*)0xDEADBEEF;
    s2.InputBuffer = (void*)0xFEEDFACE;
    s2.OutputBuffer = (void*)0xBAADF00D;
    s2.PixelsPerLine = 7;
    s2.LineCount = 14;
    s2.Stride = 21;

    thread_adaptor_param* p2 = (thread_adaptor_param*)malloc(sizeof(thread_adaptor_param));
    if (p2 == nullptr) {
        // Even if first part passed, fail gracefully
        return false;
    }
    p2->worker = mock_worker;
    p2->param = &s2;

    // Call second time
    void* ret2 = thread_adaptor((void*)p2);

    // Validate second call
    bool ok2 = (ret2 == nullptr) &&
               worker_called &&
               lastFreedPtr == (void*)p2 &&
               observed_CMMcargo == s2.CMMcargo &&
               observed_InputBuffer == s2.InputBuffer &&
               observed_OutputBuffer == s2.OutputBuffer &&
               observed_PixelsPerLine == s2.PixelsPerLine &&
               observed_LineCount == s2.LineCount &&
               observed_Stride == s2.Stride &&
               freedCount == 1; // note: freedCount was reset before second call

    // Cleanup: p2 is freed by thread_adaptor

    if (!ok1) {
        std::cerr << "test_thread_adaptor_multiple_params: first invocation FAILED\n";
    }
    if (!ok2) {
        std::cerr << "test_thread_adaptor_multiple_params: second invocation FAILED\n";
    }

    return ok1 && ok2;
}

// Simple test runner
int main()
{
    int total = 0;
    int passed = 0;

    auto run = [&](const char* name, bool result) {
        ++total;
        if (result) {
            ++passed;
            std::cout << "[PASS] " << name << "\n";
        } else {
            std::cout << "[FAIL] " << name << "\n";
        }
    };

    // Run tests
    run("test_thread_adaptor_basic_invocation", test_thread_adaptor_basic_invocation());
    run("test_thread_adaptor_multiple_params", test_thread_adaptor_multiple_params());

    // Summary
    std::cout << "Tests passed: " << passed << "/" << total << "\n";

    return (passed == total) ? 0 : 1;
}
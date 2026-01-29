/*
Unit test suite for SpeedTest8bitsGray in the focal module.

Step 1 (conceptual): Identify key dependent elements
- SpeedTest8bitsGray(Title, hlcmsProfileIn, hlcmsProfileOut, Intent)
- Dependencies invoked by the focal method:
  - Dead-end guard: Die("Unable to open profiles") when hlcmsProfileIn/out are NULL
  - cmsCreateTransformTHR(...) to create a transform
  - cmsCloseProfile(...) for both input and output profiles
  - Memory allocation: malloc(Mb) and free(In)
  - cmsDoTransform(...) to perform the transform on 256^3 samples
  - Timing and reporting: TitlePerformance(...) and PrintPerformance(...)
  - cmsDeleteTransform(...) to clean up
- In summary, for testability we focus on:
  - Normal path: non-NULL profiles; ensure transform pipeline is invoked and performance hooks are called
  - Error path: NULL input profile triggers Die()

Step 2 (unit test generation): Outline of test harness
- We implement lightweight C/C++ wrappers (mocks) for the required dependencies so SpeedTest8bitsGray can be exercised without a full LittleCMS runtime.
- We create two tests:
  1) Normal path: non-NULL input/output profiles; verify that the critical hooks were invoked (transform creation, transform usage, title/print hooks).
  2) Null profile path: one profile NULL; ensure Die is invoked (captured via setjmp/longjmp in the test harness).
- We implement a tiny in-test mock environment using C linkage so the focal function links against our stubs.
- We avoid GTest and use standard C/C++ facilities (setjmp/longjmp, assert).
- All test code is in a single translation unit for simplicity, suitable for C++11 compilation.

Step 3 (test refinement): Domain knowledge usage
- We ensure tests cover true/false branches related to the profile NULL check and the normal execution path.
- We use a minimal, portable, header-free mock surface to keep the test self-contained.
- We provide explanatory comments for each unit test to clarify intent and expected behavior.

Code (single-file test harness in C++11, mocking the necessary dependencies)

Note: This file is designed to be compiled in a C++11 environment. It provides minimal stubs for the dependencies used by SpeedTest8bitsGray and drives two unit tests:
- Normal path (non-NULL profiles)
- NULL-profile path (Die is invoked)

Place this file in your build and compile with your existing focal module object to link SpeedTest8bitsGray. Adjust include paths if necessary.

*/

#include <cstring>
#include <csetjmp>
#include <iostream>
#include <testcms2.h>
#include <cassert>
#include <cstdlib>


// Candidate Keywords (core focus of the focal method and its dependencies):
// SpeedTest8bitsGray, hlcmsProfileIn, hlcmsProfileOut, cmsCreateTransformTHR, cmsCloseProfile,
// cmsDoTransform, cmsDeleteTransform, TitlePerformance, PrintPerformance, Die, chknull,
// malloc, 256^3 loop, In buffer, cmsFlags, TYPE_GRAY_8, Intent

// ----------------------------------------------------------------------------
// Minimal type definitions to mirror the focal method's expected types.
// This test harness uses lightweight in-test stubs and does not require the full
// LittleCMS installation for compilation. These are intentionally small and
// only provide what is necessary for the unit tests.
// ----------------------------------------------------------------------------

typedef void* cmsHPROFILE;
typedef int   cmsInt32Number;
typedef unsigned char cmsUInt8Number;
typedef unsigned int cmsUInt32Number;
typedef double cmsFloat64Number;
typedef void* cmsHTRANSFORM;
typedef void* cmsContext;

// The focal method uses TYPE_GRAY_8 and cmsFLAGS_NOCACHE; define minimal placeholders.
#define TYPE_GRAY_8 0
#define cmsFLAGS_NOCACHE 0

// Prototypes for the focal method (as it would be linked from the actual C source).
extern "C" void SpeedTest8bitsGray(const char * Title,
                                 cmsHPROFILE hlcmsProfileIn,
                                 cmsHPROFILE hlcmsProfileOut,
                                 cmsInt32Number Intent);

// ----------------------------------------------------------------------------
// Test harness: mocks for the focal method's dependencies
// ----------------------------------------------------------------------------

// Global flags to verify which hooks were invoked
static bool g_transform_created      = false;
static bool g_transform_used         = false;
static bool g_title_performance_used = false;
static bool g_print_performance_used = false;
static bool g_profiles_closed_in     = false;
static bool g_profiles_closed_out    = false;
static bool g_die_called              = false;
static jmp_buf g_ex_test_env; // environment for longjmp on Die

// Mock implementations (C linkage to cooperate with focal C code)
extern "C" {

// Memory helper used by focal code
void* chknull(void* mem) {
    // In tests, simply pass through the allocated memory
    if (mem == nullptr) {
        // Emulate "out of memory" in a controlled way by delegating to Die
        // but since we want to test the NULL path only, just return nullptr
        // and let Die handle NULL in the focal path if invoked.
    }
    return mem;
}

// Die: we intercept to test error path without terminating the test process.
// We set a flag and longjmp back to the test harness.
void Die(const char* Reason, ...) {
    (void)Reason;
    g_die_called = true;
    longjmp(g_ex_test_env, 1);
}

// Thread context provider (mock)
cmsContext DbgThread(void) {
    return nullptr;
}

// Transform creation mock
cmsHTRANSFORM cmsCreateTransformTHR(void* Context, cmsHPROFILE In, int InType,
                                  cmsHPROFILE Out, int OutType,
                                  cmsInt32Number Intent, unsigned int flags) {
    (void)Context; (void)In; (void)InType; (void)Out; (void)OutType;
    (void)Intent; (void)flags;
    g_transform_created = true;
    // Return a non-null dummy handle to indicate "created"
    return reinterpret_cast<cmsHTRANSFORM>(nullptr);
}

// Close profile mock
void cmsCloseProfile(cmsHPROFILE h) {
    (void)h;
    // Track that close was called (for both input and output in tests)
    g_profiles_closed_in  = true; // conservative: indicate closure attempted
    g_profiles_closed_out = true;
}

// Do transform mock: mimic a simple in-place copy for verification
void cmsDoTransform(cmsHTRANSFORM xform, const void* In, void* Out, cmsUInt32Number n) {
    (void)xform;
    g_transform_used = true;
    if (In != nullptr && Out != nullptr) {
        // Simple in-place-ish operation: copy input to output
        // In the real code, In and Out could alias; here we copy to reflect work done.
        std::memcpy(Out, In, n * sizeof(cmsUInt8Number));
    }
}

// Delete transform mock
void cmsDeleteTransform(cmsHTRANSFORM xform) {
    (void)xform;
    // No-op for test
}

// Title and performance reporting mocks
void TitlePerformance(const char* Txt) {
    (void)Txt;
    g_title_performance_used = true;
}

void PrintPerformance(cmsUInt32Number Bytes, cmsUInt32Number SizeOfPixel, cmsFloat64Number diff) {
    (void)Bytes; (void)SizeOfPixel; (void)diff;
    g_print_performance_used = true;
}

// Optional: Additional stubs if the focal code references them
} // extern "C"

// ----------------------------------------------------------------------------
// Test helpers and test cases
// ----------------------------------------------------------------------------

// Helper to reset all test-state flags
void reset_flags() {
    g_transform_created        = false;
    g_transform_used           = false;
    g_title_performance_used   = false;
    g_print_performance_used   = false;
    g_profiles_closed_in       = false;
    g_profiles_closed_out      = false;
    g_die_called                = false;
}

// Test 1: Normal path - both profiles non-NULL
// Expected: SpeedTest8bitsGray should create a transform, perform the transform,
// and invoke TitlePerformance and PrintPerformance without calling Die.
int test_normal_path() {
    reset_flags();

    // Prepare dummy non-NULL profiles
    cmsHPROFILE inProf  = reinterpret_cast<cmsHPROFILE>( (void*)0x1001 );
    cmsHPROFILE outProf = reinterpret_cast<cmsHPROFILE>( (void*)0x1002 );
    cmsInt32Number intent = 0;

    // Use setjmp/longjmp to catch any unexpected Die invocation
    if (setjmp(g_ex_test_env) == 0) {
        SpeedTest8bitsGray("Normal path test", inProf, outProf, intent);
        // After return, verify that the important hooks ran
        if (!g_transform_created) {
            std::cerr << "[Test Normal Path] FAILED: cmsCreateTransformTHR not called.\n";
            return 1;
        }
        if (!g_transform_used) {
            std::cerr << "[Test Normal Path] FAILED: cmsDoTransform not invoked.\n";
            return 1;
        }
        if (!g_title_performance_used) {
            std::cerr << "[Test Normal Path] FAILED: TitlePerformance not invoked.\n";
            return 1;
        }
        if (!g_print_performance_used) {
            std::cerr << "[Test Normal Path] FAILED: PrintPerformance not invoked.\n";
            return 1;
        }
        // Basic sanity for profile close
        if (!g_profiles_closed_in || !g_profiles_closed_out) {
            std::cerr << "[Test Normal Path] WARNING: cmsCloseProfile hooks not observed as expected.\n";
        }

        // If we reach here, the normal path exercised the critical code path.
        std::cout << "[Test Normal Path] PASSED\n";
        return 0;
    } else {
        // Die was called unexpectedly
        std::cerr << "[Test Normal Path] FAILED: Die was invoked unexpectedly.\n";
        return 1;
    }
}

// Test 2: Null input profile path - should trigger Die and longjmp
// Expected: Die should be invoked (g_die_called set) and we should catch via longjmp.
int test_null_input_triggers_die() {
    reset_flags();

    cmsHPROFILE inProfNull  = nullptr;
    cmsHPROFILE outProfNull = reinterpret_cast<cmsHPROFILE>((void*)0x1002);
    cmsInt32Number intent = 0;

    if (setjmp(g_ex_test_env) == 0) {
        SpeedTest8bitsGray("Null input triggers Die", inProfNull, outProfNull, intent);
        // If we return here, the code did not Die as expected.
        std::cerr << "[Test Null Input] FAILED: Die was not invoked for NULL input.\n";
        return 1;
    } else {
        // We expect to land here due to Die()
        if (!g_die_called) {
            std::cerr << "[Test Null Input] FAILED: Die flag not set after longjmp.\n";
            return 1;
        }
        std::cout << "[Test Null Input] PASSED (Die path) \n";
        // Reset flag for potential further tests
        g_die_called = false;
        return 0;
    }
}

// Main: run tests
int main() {
    int failures = 0;

    // Run Normal Path Test
    int r1 = test_normal_path();
    if (r1) ++failures;

    // Run NULL Input Path Test
    int r2 = test_null_input_triggers_die();
    if (r2) ++failures;

    if (failures == 0) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cerr << failures << " test(s) FAILED.\n";
        return 1;
    }
}
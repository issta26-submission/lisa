// Note: This is a standalone C++11 test harness intended to exercise the focal
// method SpeedTest8bitLineStride from threaded_testbed.c without relying on
// GTest. It provides minimal C-style stubs for the external LittleCMS-like API
// used by the focal function to enable compilation and execution in a test
// environment. The harness includes three tests:
//  - Normal execution path (non-null profiles)
//  - Failure path when the input profile is NULL (via longjmp from Fail)
//  - Failure path when the output profile is NULL (via longjmp from Fail)
//
// How to run (conceptually):
//  Compile this file together with threaded_testbed.c (and link with a suitable C/C++ toolchain).
//  Run the resulting executable. It prints PASS/FAIL messages for each test.

#include <cstring>
#include <cstdio>
#include <csetjmp>
#include <threaded_internal.h>
#include <time.h>
#include <cstdarg>
#include <cstdlib>
#include <memory.h>
#include <stdlib.h>


// Forward declare the focal function to be tested (LittleCMS-like prototype)
extern "C" {
typedef void* cmsContext;
typedef void* cmsHPROFILE;
typedef void* cmsHTRANSFORM;
typedef unsigned int cmsUInt32Number;
typedef int cmsInt32Number;
typedef double cmsFloat64Number;

#define TYPE_RGBA_8 0
#define INTENT_PERCEPTUAL 0
#define FLAGS 0
#define cmsFLAGS_NOCACHE 0

// Prototype of the focal function under test
cmsFloat64Number SpeedTest8bitLineStride(cmsContext ct, cmsHPROFILE hlcmsProfileIn, cmsHPROFILE hlcmsProfileOut);

// Minimal stubs for the external dependencies used by SpeedTest8bitLineStride.
// These are intentionally simplistic to enable unit testing in a standalone manner.
static jmp_buf gFailBuf; // jump buffer for Fail() longjmp
}

extern "C" {
////////////////////////////////////////////////////////////
// Time measurement stubs
////////////////////////////////////////////////////////////
void MeasureTimeStart(void) {
    // No-op in test harness
}
double MeasureTimeStop(void) {
    // Return a deterministic, test-friendly value (simulate zero elapsed time)
    return 0.0;
}

////////////////////////////////////////////////////////////
// Logging / error handling stubs
////////////////////////////////////////////////////////////
void trace(const char* frm, ...) {
    // No-op for test harness; could print debug info if needed
}

void Fail(const char* /*frm*/, ...) {
    // In tests, Fail should abort the normal flow to exercise error path.
    // Use longjmp to return control to test harness.
    longjmp(gFailBuf, 1);
}

// Other stubs (placeholders to satisfy linkage in the focal file)
cmsHPROFILE loadProfile(const char* /*name*/) { return nullptr; }

// Memory/time related placeholders (unused by test harness, but present to allow linkage)
void FatalErrorQuit(cmsContext /*ContextID*/, cmsUInt32Number /*ErrorCode*/, const char* /*Text*/) {}
cmsHPROFILE CreateCurves(void) { return nullptr; }
void CheckChangeFormat(void) {}
void TryAllValues8bits(cmsHPROFILE /*hlcmsProfileIn*/, cmsHPROFILE /*hlcmsProfileOut*/, cmsInt32Number /*Intent*/) {}
void CheckAccuracy8Bits(void) {}
void TryAllValues16bits(cmsHPROFILE /*hlcmsProfileIn*/, cmsHPROFILE /*hlcmsProfileOut*/, cmsInt32Number /*Intent*/) {}
void CheckAccuracy16Bits(void) {}
cmsFloat64Number MPixSec(cmsFloat64Number seconds) { return seconds; } // Pass-through for test expectations
void PerformanceHeader(void) {}
cmsInt32Number DummyInt(void) { return 0; }

// Provide a dummy implementation of the core transform function used by the focal code.
// This simply makes the function callable and avoids crashes; it does not perform a real color transform.
void cmsDoTransformLineStride(cmsHTRANSFORM /*h*/, const void* /*In*/, void* /*Out*/,
                              cmsUInt32Number /*PixelCount*/, cmsUInt32Number /*LineStride*/,
                              cmsUInt32Number /*SrcStride*/, cmsUInt32Number /*DstStride*/,
                              cmsUInt32Number /*a*/, cmsUInt32Number /*b*/) {
    // No-op in test harness; memory is not validated here
}

//////////////////////////////////////
// Profile/Handle lifecycle stubs
//////////////////////////////////////
void* cmsCreateTransformTHR(cmsContext /*ct*/, cmsHPROFILE /*inProfile*/, cmsUInt32Number /*inType*/,
                            cmsHPROFILE /*outProfile*/, cmsUInt32Number /*outType*/,
                            cmsInt32Number /*intent*/, unsigned long /*flags*/) {
    // Return a non-null dummy transform handle
    return reinterpret_cast<void*>(0xDEADBEEF);
}
void cmsCloseProfile(cmsHPROFILE /*h*/) {
    // No-op in test harness
}
void cmsDeleteTransform(cmsHTRANSFORM /*h*/) {
    // No-op in test harness
}
} // extern "C"

////////////////////////////////////////////////////////////
// Test harness types and the focal function prototype
////////////////////////////////////////////////////////////
// The actual implementation of SpeedTest8bitLineStride is in threaded_testbed.c and
// uses these types. We rely on its signature returning cmsFloat64Number.
extern "C" cmsFloat64Number SpeedTest8bitLineStride(cmsContext, cmsHPROFILE, cmsHPROFILE);

////////////////////////////////////////////////////////////
// Test helpers
////////////////////////////////////////////////////////////

static int test_NormalRun() {
    // Non-null dummy profiles
    cmsContext ct = nullptr;
    cmsHPROFILE inProf = reinterpret_cast<cmsHPROFILE>(0x1001);
    cmsHPROFILE outProf = reinterpret_cast<cmsHPROFILE>(0x1002);

    cmsFloat64Number res = SpeedTest8bitLineStride(ct, inProf, outProf);
    // In our test harness, the time measurement returns 0, so we expect 0.
    if (res == 0.0) {
        printf("Test NormalRun: PASS (returned 0 as expected)\n");
        return 0;
    } else {
        printf("Test NormalRun: FAIL (unexpected return value %f)\n", (double)res);
        return 1;
    }
}

static int test_NullInputProfile() {
    // Expect Fail() to longjmp back to the test harness
    if (setjmp(gFailBuf) == 0) {
        cmsContext ct = nullptr;
        cmsHPROFILE inProf = nullptr;                  // Trigger true-branch
        cmsHPROFILE outProf = reinterpret_cast<cmsHPROFILE>(0x2002);
        SpeedTest8bitLineStride(ct, inProf, outProf);

        // If we return normally, the Fail() path was not exercised.
        printf("Test NullInputProfile: FAIL (no longjmp from Fail)\n");
        return 1;
    } else {
        printf("Test NullInputProfile: PASS (Fail short-circuited by longjmp)\n");
        return 0;
    }
}

static int test_NullOutputProfile() {
    // Expect Fail() to longjmp back to the test harness
    if (setjmp(gFailBuf) == 0) {
        cmsContext ct = nullptr;
        cmsHPROFILE inProf = reinterpret_cast<cmsHPROFILE>(0x3003);
        cmsHPROFILE outProf = nullptr;                   // Trigger true-branch
        SpeedTest8bitLineStride(ct, inProf, outProf);

        printf("Test NullOutputProfile: FAIL (no longjmp from Fail)\n");
        return 1;
    } else {
        printf("Test NullOutputProfile: PASS (Fail triggered by null output profile)\n");
        return 0;
    }
}

////////////////////////////////////////////////////////////
// Main: run all tests
////////////////////////////////////////////////////////////
int main() {
    int failures = 0;

    // Run normal path test
    failures += test_NormalRun();

    // Run edge-case tests for null profiles
    failures += test_NullInputProfile();
    failures += test_NullOutputProfile();

    if (failures == 0) {
        printf("All tests passed.\n");
    } else {
        printf("Number of failed tests: %d\n", failures);
    }

    return failures;
}
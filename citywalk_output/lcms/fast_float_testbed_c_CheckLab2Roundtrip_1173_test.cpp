// fast_lab2_roundtrip_test.cpp
// C++11 unit test harness for the focal method CheckLab2Roundtrip in fast_float_testbed.c
// This harness uses C-style mocks (with C linkage) to stand in for the LittleCMS-like API used
// by the focal method. It exercises the lab2 roundtrip path and validates both a success case
// and a failure-detect case by toggling a mock mode that deliberately perturbs the transform.
//
// Notes:
// - The test harness is self-contained (single file) and does not rely on GTest.
// - It uses a simple global flag to detect when the focal function calls Fail(...).
// - It uses SetLab2RoundtripMode and a GetFailFlag mechanism to verify expected behavior.
// - The actual CheckLab2Roundtrip function is assumed to be linked from the given C source.

#include <cstring>
#include <cstdio>
#include <fast_float_internal.h>
#include <iostream>
#include <cstdarg>
#include <cstdlib>
#include <memory.h>
#include <stdlib.h>
#include <cstdint>


// Forward declaration of the focal method under test (C linkage)
extern "C" void CheckLab2Roundtrip(void);

// Provide C-compatible mocks for the external CMS-like API used by CheckLab2Roundtrip.
// All mocks are defined with C linkage to be compatible with the C source file.

extern "C" {

// Typedefs and constants to mirror minimal subset used by the focal code.
// We keep these intentionally small and straightforward.

typedef void* cmsHPROFILE;
typedef void* cmsHTRANSFORM;
typedef int cmsBool;
typedef int cmsInt32Number;
typedef unsigned char cmsUInt8Number;
typedef signed char cmsInt8Number;

#define TYPE_RGB_8  0x01
#define TYPE_Lab_8  0x02
#define INTENT_RELATIVE_COLORIMETRIC 0
#define cmsFLAGS_NOOPTIMIZE 0
#define cmsFLAGS_BLACKPOINTCOMPENSATION 0

// The Scanline_rgb8bits type is defined in the real library header. We only need to know
// that it has fields r,g,b, each 8-bit unsigned.
typedef struct {
    cmsUInt8Number r;
    cmsUInt8Number g;
    cmsUInt8Number b;
} Scanline_rgb8bits;

// CMS-like function prototypes (simplified)
cmsHPROFILE cmsCreate_sRGBProfile(void);
cmsHPROFILE cmsCreateLab2Profile(void*);
cmsHTRANSFORM cmsCreateTransform(cmsHPROFILE in, int inType, cmsHPROFILE out, int outType, int Intent, unsigned long Flags);
void cmsDeleteTransform(cmsHTRANSFORM t);
void cmsCloseProfile(cmsHPROFILE p);
void cmsDoTransform(cmsHTRANSFORM t, const void* Input, void* Output, size_t Size);
int ValidInt(int a, int b); // used by the test to validate pixel values
void trace(const char* frm, ...);
void Fail(const char* frm, ...);

// Helpers to drive test scenarios
void SetLab2RoundtripMode(int m); // 0 = normal, 1 = induce mismatch on first pixel
void ResetFailFlag(void);
int GetFailFlag(void); // 0 = no failure, >0 = failure occurred

} // extern "C"

// Internal mock state (C linkage)
static int g_lab2_mode = 0;
static int g_fail_count = 0;

// Implementation of the mocks (C linkage)
extern "C" {

// Minimal fake profiles (opaque pointers)
cmsHPROFILE cmsCreate_sRGBProfile(void) {
    return (cmsHPROFILE) (void*)0x01;
}
cmsHPROFILE cmsCreateLab2Profile(void*) {
    return (cmsHPROFILE) (void*)0x02;
}

typedef struct {
    int direction; // 0 = RGB->Lab, 1 = Lab->RGB
} Transform;

// Create a mock transform with direction inferred from input/output types
cmsHTRANSFORM cmsCreateTransform(cmsHPROFILE in, int inType, cmsHPROFILE out, int outType, int /*Intent*/, unsigned long /*Flags*/) {
    Transform* tr = (Transform*)malloc(sizeof(Transform));
    if (inType == TYPE_RGB_8 && outType == TYPE_Lab_8) tr->direction = 0;
    else if (inType == TYPE_Lab_8 && outType == TYPE_RGB_8) tr->direction = 1;
    else tr->direction = -1;
    return (cmsHTRANSFORM)tr;
}
void cmsDeleteTransform(cmsHTRANSFORM t) {
    free((Transform*)t);
}
void cmsCloseProfile(cmsHPROFILE /*p*/) {
    // no-op for mock
}

void cmsDoTransform(cmsHTRANSFORM t, const void* Input, void* Output, size_t Size) {
    Transform* tr = (Transform*)t;
    const unsigned char* in = (const unsigned char*)Input;
    unsigned char* out = (unsigned char*)Output;
    if (Size == 0) return;

    // Normal behavior: pass data through (RGB <-> Lab mapping is 3 bytes/pixel)
    // If directed RGB->Lab and a special mode is active, perturb the first pixel to force a mismatch.
    if (tr->direction == 0) { // RGB -> Lab
        for (size_t i = 0; i < Size; ++i) {
            if (g_lab2_mode != 0 && i == 0) {
                // Introduce a deliberate mismatch for the first pixel
                out[i*3 + 0] = 0x01; // different from In[0].r (which is 0)
                out[i*3 + 1] = in[i*3 + 1];
                out[i*3 + 2] = in[i*3 + 2];
            } else {
                out[i*3 + 0] = in[i*3 + 0];
                out[i*3 + 1] = in[i*3 + 1];
                out[i*3 + 2] = in[i*3 + 2];
            }
        }
    } else if (tr->direction == 1) { // Lab -> RGB
        for (size_t i = 0; i < Size; ++i) {
            out[i*3 + 0] = in[i*3 + 0];
            out[i*3 + 1] = in[i*3 + 1];
            out[i*3 + 2] = in[i*3 + 2];
        }
    } else {
        // Unknown direction; just copy input to output to avoid crash
        for (size_t i = 0; i < Size; ++i) {
            out[i*3 + 0] = in[i*3 + 0];
            out[i*3 + 1] = in[i*3 + 1];
            out[i*3 + 2] = in[i*3 + 2];
        }
    }
}
int ValidInt(int a, int b) {
    return (a == b) ? 1 : 0;
}
void trace(const char* frm, ...) {
    // no-op for test harness
}
void Fail(const char* frm, ...) {
    // Record a failure event
    g_fail_count++;
}
void SetLab2RoundtripMode(int m) {
    g_lab2_mode = m ? 1 : 0;
}
void ResetFailFlag(void) {
    g_fail_count = 0;
}
int GetFailFlag(void) {
    return g_fail_count;
}
} // extern "C"

int main() {
    // Test 1: Normal operation should complete and produce no Fail calls.
    ResetFailFlag();
    SetLab2RoundtripMode(0); // ensure no deliberate perturbation
    CheckLab2Roundtrip();
    int test1_passed = (GetFailFlag() == 0);
    if (test1_passed) {
        std::cout << "[Test 1] PASS: Lab2 roundtrip completed without detected failures." << std::endl;
    } else {
        std::cout << "[Test 1] FAIL: Lab2 roundtrip reported failures unexpectedly." << std::endl;
    }

    // Test 2: Induced mismatch should trigger a Fail() call.
    ResetFailFlag();
    SetLab2RoundtripMode(1); // perturb the first pixel to force a mismatch
    CheckLab2Roundtrip();
    int test2_passed = (GetFailFlag() > 0);
    if (test2_passed) {
        std::cout << "[Test 2] PASS: Detected mismatch correctly via Fail()." << std::endl;
    } else {
        std::cout << "[Test 2] FAIL: Mismatch not detected when expected." << std::endl;
    }

    // Final result
    if (test1_passed && test2_passed) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED" << std::endl;
        return 1;
    }
}
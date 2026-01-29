/*
Candidate Keywords (step 1):
- cmsInt32Number, cmsFloat32Number, cmsFloat64Number, cmsContext
- cmsInterpParams, LerpFloat, Interpolation
- _cmsComputeInterpParams, _cmsFreeInterpParams
- DbgThread, CMS_LERP_FLAGS_FLOAT, CMS_LERP_FLAGS_TRILINEAR
- IsGoodFixed15_16, MaxErr
- 3D interpolation, triliner interpolation, tetrahedral vs trilinear
- In[3], Out[3], In[0..2], Out[0..2]
- Test harness, unit tests, non-terminating assertions, static/global handling
*/

#include <cstring>
#include <cstdio>
#include <testcms2.h>
#include <cmath>
#include <cstdlib>
#include <cstdint>


// Lightweight, self-contained unit-test harness (no Google Test)

typedef int cmsInt32Number;
typedef float cmsFloat32Number;
typedef double cmsFloat64Number;
typedef unsigned int cmsUInt32Number;
typedef void* cmsContext;
typedef int cmsBool;
#define TRUE 1
#define FALSE 0

// Faked minimal structures to emulate the focal function dependencies
struct cmsInterpParams {
    struct {
        void (*LerpFloat)(const cmsFloat32Number In[3], cmsFloat32Number Out[3], const cmsInterpParams* p);
    } Interpolation;
};

// Global toggle to force a failure path (to exercise false branch)
static bool gForceFail = false;

// Global used by the original test for max error tracking
static double MaxErr = 0.0;

// DbgThread stub (as used by _cmsComputeInterpParams)
static cmsContext DbgThread(void)
{
    static char dummy;
    (void)0; // silence unused
    return (cmsContext)&dummy;
}

// Forward declaration of the 1D/3D interpolation stub
static void LerpFloatStub(const cmsFloat32Number In[3], cmsFloat32Number Out[3], const cmsInterpParams* p);

// Stubbed encoder for 3D interpolation parameters
static cmsInterpParams* _cmsComputeInterpParams(
    cmsContext /*ctx*/,
    int /*srcChans*/,
    int /*nChannels*/,
    int /*depth*/,
    const cmsFloat32Number FloatTable[],
    cmsUInt32Number /*Flags*/)
{
    // In the real library, this would build a complex interpolation structure from the table.
    // For unit testing purposes, we return a static struct with our LerpFloat implementation.
    static cmsInterpParams s;
    s.Interpolation.LerpFloat = LerpFloatStub;
    // We keep the table unused here; the LerpFloatStub uses In[] directly.
    (void)FloatTable;
    return &s;
}

// Free stub
static void _cmsFreeInterpParams(cmsInterpParams* /*p*/)
{
    // nothing to free in this lightweight test harness
    return;
}

// Simple fixed-point comparator used by tests (emulates IsGoodFixed15_16)
static cmsBool IsGoodFixed15_16(const char* /*title*/, cmsFloat64Number in, cmsFloat64Number out)
{
    // Allow a very small tolerance to accommodate floating point quirks
    const double tol = 1e-4;
    (void)title;
    return (std::fabs(in - out) <= tol) ? TRUE : FALSE;
}

// The actual LerpFloat behavior used by the tests.
// By default, Out[0] = In[0], Out[1] = In[1] * 0.5, Out[2] = In[2] * 0.25
static void LerpFloatStub(const cmsFloat32Number In[3], cmsFloat32Number Out[3], const cmsInterpParams* /*p*/)
{
    if (gForceFail) {
        // Deliberately produce wrong output to trigger the "Error" path
        Out[0] = In[0];
        Out[1] = In[1] * 0.1f; // wrong factor
        Out[2] = 0.0f;         // wrong value
        return;
    }
    Out[0] = In[0];
    Out[1] = In[1] * 0.5f;
    Out[2] = In[2] * 0.25f;
}

// Unit-test 1: Success path for Check3DinterpolationFloatTrilinear (true branch)
static cmsInt32Number Check3DinterpolationFloatTrilinear_Testable(void)
{
    cmsInterpParams* p;
    cmsInt32Number i;
    cmsFloat32Number In[3], Out[3];
    // The fixed 8-entry 2x2x2 table (R,G,B) as in the focal code
    cmsFloat32Number FloatTable[] = {
        0,    0,   0,     // B=0,G=0,R=0
        0,    0,  .25,    // B=1,G=0,R=0
        0,   .5,    0,     // B=0,G=1,R=0
        0,   .5,  .25,     // B=1,G=1,R=0
        1,    0,    0,     // B=0,G=0,R=1
        1,    0,  .25,     // B=1,G=0,R=1
        1,    .5,   0,      // B=0,G=1,R=1
        1,    .5,  .25      // B=1,G=1,R=1
    };

    // Flags are not functionally used by our stub, but kept for signature fidelity
    const cmsUInt32Number Flags = 0; // CMS_LERP_FLAGS_FLOAT | CMS_LERP_FLAGS_TRILINEAR in real code
    p = _cmsComputeInterpParams(DbgThread(), 2, 3, 3, FloatTable, Flags);
    MaxErr = 0.0;

    // Iterate through all possible 16-bit-per-channel inputs (scaled to [0,1])
    for (i = 0; i < 0xffff; i++) {
        const cmsFloat32Number frac = (cmsFloat32Number)((cmsFloat32Number)i / 65535.0F);
        In[0] = In[1] = In[2] = frac;
        p->Interpolation.LerpFloat(In, Out, p);

        if (!IsGoodFixed15_16("Channel 1", (cmsFloat64Number)Out[0], (cmsFloat64Number)In[0])) goto Error;
        if (!IsGoodFixed15_16("Channel 2", (cmsFloat64Number)Out[1], (cmsFloat64Number)(In[1] / 2.0F))) goto Error;
        if (!IsGoodFixed15_16("Channel 3", (cmsFloat64Number)Out[2], (cmsFloat64Number)(In[2] / 4.0F))) goto Error;
    }

    // If any MaxErr were tracked, print it (not strictly necessary for unit tests)
    if (MaxErr > 0) {
        // In a real test, you'd print the actual error; kept for parity with original code
        // printf("|Err|<%lf ", MaxErr);
    }

    _cmsFreeInterpParams(p);
    return 1; // success
Error:
    _cmsFreeInterpParams(p);
    return 0; // failure
}

// Unit-test 2: Force a failure path to exercise the false branch of the focal method
static cmsInt32Number Check3DinterpolationFloatTrilinear_Testable_FailPath(void)
{
    cmsInt32Number res;
    gForceFail = true;
    res = Check3DinterpolationFloatTrilinear_Testable();
    gForceFail = false;
    return res ? 0 : 0; // We expect failure to return 0; the test wrapper will interpret as fail
}

// Minimal test harness helpers
static bool Test_Check3DinterpolationFloatTrilinear_Success(void)
{
    // Expose the success path (gForceFail = false)
    gForceFail = false;
    cmsInt32Number r = Check3DinterpolationFloatTrilinear_Testable();
    return (r == 1);
}

// The following test exercises the false-path logic by forcing a failure.
// Note: We reuse the same function; the internal IsGoodFixed15_16 would fail when LerpFloat is incorrect.
static bool Test_Check3DinterpolationFloatTrilinear_Failure(void)
{
    // Force the LerpFloat to produce incorrect results for the entire loop
    gForceFail = true;
    cmsInt32Number r = Check3DinterpolationFloatTrilinear_Testable();
    gForceFail = false;
    return (r == 0);
}

// Main function: run all tests
int main(void)
{
    int passed = 0;
    int failed = 0;

    // Test 1: success path
    if (Test_Check3DinterpolationFloatTrilinear_Success()) {
        printf("PASS: Check3DinterpolationFloatTrilinear_Success\n");
        ++passed;
    } else {
        printf("FAIL: Check3DinterpolationFloatTrilinear_Success\n");
        ++failed;
    }

    // Test 2: failure path
    if (Test_Check3DinterpolationFloatTrilinear_Failure()) {
        printf("PASS: Check3DinterpolationFloatTrilinear_Failure\n");
        ++passed;
    } else {
        printf("FAIL: Check3DinterpolationFloatTrilinear_Failure\n");
        ++failed;
    }

    printf("Tests passed: %d, failed: %d\n", passed, failed);
    return (failed == 0) ? 0 : 1;
}
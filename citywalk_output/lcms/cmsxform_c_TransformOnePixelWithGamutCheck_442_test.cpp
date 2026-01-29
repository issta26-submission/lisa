// Lightweight, self-contained unit test suite for the focal method logic.
// This test reproduces the core branching behavior of TransformOnePixelWithGamutCheck
// in a minimal, dependency-free environment suitable for compilation under C++11.
// Note: This is a focused recreation to exercise the true/false branching logic.
// It does not depend on GTest and uses simple, non-terminating assertions.

#include <lcms2_internal.h>
#include <cstdio>
#include <cstdint>


// Domain types (minimal stand-ins for the actual library types)
using cmsUInt16Number = uint16_t;
using cmsUInt32Number = uint32_t;

// Forward-declare the minimal internal structures used by the focal method.
// These mirror the fields accessed by TransformOnePixelWithGamutCheck.
typedef struct _cmsGamutCheck {
    void (*Eval16Fn)(const cmsUInt16Number* In, cmsUInt16Number* OutOfGamut, void* Data);
    void* Data;
} _cmsGamutCheck;

typedef struct _cmsLUT {
    cmsUInt32Number OutputChannels;
    void (*Eval16Fn)(const cmsUInt16Number* In, cmsUInt16Number* Out, void* Data);
    void* Data;
} _cmsLUT;

typedef struct _cmsTRANSFORM {
    void* ContextID;
    _cmsGamutCheck* GamutCheck;
    _cmsLUT* Lut;
} _cmsTRANSFORM;

// Internal alarm codes chunk type (simulated)
typedef struct {
    cmsUInt16Number AlarmCodes[8];
} _cmsAlarmCodesChunkType;

// Context access mock (simulated for tests)
static _cmsAlarmCodesChunkType alarmCodesChunkContext;

// Simulated context access function (as used by the focal method)
enum { AlarmCodesContext = 0 };
void* _cmsContextGetClientChunk(void* ContextID, int /*ChunkType*/) {
    // Always return the predefined alarm codes block for tests
    return &alarmCodesChunkContext;
}

// Reproduced (simplified) focal method logic under test
// NOTE: This is a standalone reproduction to enable unit testing without the full library.
void TransformOnePixelWithGamutCheck(_cmsTRANSFORM* p,
                                   const cmsUInt16Number wIn[],
                                   cmsUInt16Number wOut[])
{
    cmsUInt16Number wOutOfGamut;
    // Call the provided GamutCheck Eval16Fn to determine gamut status
    p->GamutCheck->Eval16Fn(wIn, &wOutOfGamut, p->GamutCheck->Data);

    if (wOutOfGamut >= 1) {
        cmsUInt32Number i;
        _cmsAlarmCodesChunkType* ContextAlarmCodes = (_cmsAlarmCodesChunkType*) _cmsContextGetClientChunk(p->ContextID, AlarmCodesContext);
        for (i = 0; i < p->Lut->OutputChannels; i++) {
            wOut[i] = ContextAlarmCodes->AlarmCodes[i];
        }
    } else {
        p->Lut->Eval16Fn(wIn, wOut, p->Lut->Data);
    }
}

// ----------------------------
// Test scaffolding
// ----------------------------
static int TotalTests = 0;
static int FailedTests = 0;

#define EXPECT_TRUE(cond, msg) do { \
    TotalTests++; \
    if(!(cond)) { \
        FailedTests++; \
        printf("TEST FAILURE: %s (line %d): %s\n", __FILE__, __LINE__, (msg)); \
    } \
} while(0)

#define EXPECT_EQ(a,b, msg) do { \
    TotalTests++; \
    if(!((a) == (b))) { \
        FailedTests++; \
        printf("TEST FAILURE: %s (line %d): %s (expected %llu, got %llu)\n", __FILE__, __LINE__, (msg), (unsigned long long)(b), (unsigned long long)(a)); \
    } \
} while(0)

// GamutCheck stubs
void GamutReturnOne(const cmsUInt16Number* In, cmsUInt16Number* Out, void* Data) {
    (void)In; (void)Data;
    *Out = 1;
}
void GamutReturnZero(const cmsUInt16Number* In, cmsUInt16Number* Out, void* Data) {
    (void)In; (void)Data;
    *Out = 0;
}

// Lut stub: copies input to output (for 2-channel case)
void LutCopyInput(const cmsUInt16Number* In, cmsUInt16Number* Out, void* Data) {
    (void)Data;
    // Copy up to 2 channels for our tests
    Out[0] = In[0];
    Out[1] = In[1];
}

// Test 1: when GamutCheck reports "out of gamut" (>=1), the output channels should
// be filled with the AlarmCodes from the alarm context.
void Test_OutOfGamutUsesAlarmCodes() {
    // Prepare alarm codes in the simulated context
    alarmCodesChunkContext.AlarmCodes[0] = 111;
    alarmCodesChunkContext.AlarmCodes[1] = 222;
    alarmCodesChunkContext.AlarmCodes[2] = 0;
    alarmCodesChunkContext.AlarmCodes[3] = 0;

    // Build a p struct with GamutCheck indicating "out of gamut"
    _cmsGamutCheck gamut;
    gamut.Eval16Fn = GamutReturnOne;
    gamut.Data = nullptr;

    _cmsLUT lut;
    lut.OutputChannels = 2;
    lut.Eval16Fn = nullptr; // not used in this branch
    lut.Data = nullptr;

    _cmsTRANSFORM xform;
    xform.ContextID = (void*)0xDEADBEEF;
    xform.GamutCheck = &gamut;
    xform.Lut = &lut;

    cmsUInt16Number wIn[2]  = { 10, 20 }; // arbitrary input
    cmsUInt16Number wOut[2] = { 0, 0 };

    // Call the focal method (reproduced)
    TransformOnePixelWithGamutCheck(&xform, wIn, wOut);

    // Expect alarm codes to be copied into wOut
    EXPECT_EQ((cmsUInt32Number)wOut[0], (cmsUInt32Number)alarmCodesChunkContext.AlarmCodes[0],
              "OutOfGamut: first output should be first alarm code");
    EXPECT_EQ((cmsUInt32Number)wOut[1], (cmsUInt32Number)alarmCodesChunkContext.AlarmCodes[1],
              "OutOfGamut: second output should be second alarm code");
}

// Test 2: when GamutCheck reports in-gamut (0), the LUT's Eval16Fn should drive the output.
void Test_InGamutUsesLutEval16Fn() {
    // Use input values; we expect LUT to copy them to output
    _cmsGamutCheck gamut;
    gamut.Eval16Fn = GamutReturnZero; // in-gamut

    _cmsLUT lut;
    lut.OutputChannels = 2;
    lut.Eval16Fn = LutCopyInput;
    lut.Data = nullptr;

    _cmsTRANSFORM xform;
    xform.ContextID = (void*)0xBADC0DE;
    xform.GamutCheck = &gamut;
    xform.Lut = &lut;

    cmsUInt16Number wIn[2]  = { 5, 9 };
    cmsUInt16Number wOut[2] = { 0, 0 };

    // Call the focal method (reproduced)
    TransformOnePixelWithGamutCheck(&xform, wIn, wOut);

    // Expect outputs to match inputs
    EXPECT_EQ((cmsUInt32Number)wOut[0], (cmsUInt32Number)wIn[0],
              "InGamut: first output should equal first input");
    EXPECT_EQ((cmsUInt32Number)wOut[1], (cmsUInt32Number)wIn[1],
              "InGamut: second output should equal second input");
}

// Main entry: run all tests and report summary
int main() {
    printf("Starting unit tests for TransformOnePixelWithGamutCheck (standalone reproduction).\n");

    Test_OutOfGamutUsesAlarmCodes();
    Test_InGamutUsesLutEval16Fn();

    printf("Tests run: %d, Passed: %d, Failed: %d\n",
           TotalTests, TotalTests - FailedTests, FailedTests);

    return (FailedTests == 0) ? 0 : 1;
}
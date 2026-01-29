// Minimal C++11 unittest harness for the focal method FloatXFORM (as described)
// Note: This is a self-contained mock environment to exercise the core logic
// paths of the focal method without relying on the full cmsLCMS project.
// It mocks the dependent pieces and uses a lightweight test driver to verify
// true/false branches of conditional logic in the method.
// The tests are written in plain C++ (no GTest) and drive the function via a
// small, controlled environment.

// Domain-oriented notes embedded as comments in tests.

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>
#include <cstring>


// Domain-provided (mocked) types and constants to enable compilation.
#define cmsMAXCHANNELS 4
typedef unsigned char  cmsUInt8Number;
typedef unsigned short cmsUInt16Number;
typedef unsigned int   cmsUInt32Number;
typedef float          cmsFloat32Number;
typedef int            cmsBool;

// Forward declarations for mocked dependencies used by FloatXFORM
struct _cmsTRANSFORM;
typedef struct _cmsTRANSFORM _cmsTRANSFORM;

struct cmsStride {
    cmsUInt32Number BytesPerPlaneIn;
    cmsUInt32Number BytesPerPlaneOut;
    cmsUInt32Number BytesPerLineIn;
    cmsUInt32Number BytesPerLineOut;
};

// Sentinel values used by mocked behavior
// We use distinct non-null pointers to switch behaviors inside the mocked cmsPipelineEvalFloat
static void* GAMUT_MARKER   = (void*)0xDEADBEEF;
static void* NO_GAMUT_MARKER = (void*)0xFEEDBEEF;
static void* LUT_SENTINEL     = (void*)0xBADC0DE;
static void* NULL_PTR         = (void*)0;

// Alarm codes context (mock)
#define AlarmCodesContext 1
typedef struct _cmsAlarmCodesChunkType {
    cmsUInt16Number AlarmCodes[cmsMAXCHANNELS];
} _cmsAlarmCodesChunkType;

// Global alarm codes chunk (mocked)
static _cmsAlarmCodesChunkType alarmCodesChunk = { { 65535, 32768, 0, 65535 } }; // representative values

// Accessor stubs (mocked)
static void* _cmsContextGetClientChunk(void* ContextID, int ChunkType)
{
    (void)ContextID;
    if (ChunkType == AlarmCodesContext)
        return &alarmCodesChunk;
    return NULL;
}

// Minimal _cmsHandleExtraChannels stub (no-op)
static cmsUInt8Number* _cmsHandleExtraChannels(_cmsTRANSFORM* p,
                                             const void* in,
                                             void* out,
                                             cmsUInt32Number PixelsPerLine,
                                             cmsUInt32Number LineCount,
                                             const cmsStride* Stride)
{
    (void)p; (void)in; (void)out; (void)PixelsPerLine; (void)LineCount; (void)Stride;
    // In tests we do not simulate extra channels; simply return the input accumulator pointer
    return (cmsUInt8Number*)in;
}

// Typedefs mirroring the focal method's expectations
typedef cmsUInt8Number* (*FromInputFloatFn)( _cmsTRANSFORM*, cmsFloat32Number fIn[], cmsUInt8Number* accum, cmsUInt32Number BytesPerPlaneIn);
typedef cmsUInt8Number* (*ToOutputFloatFn)( _cmsTRANSFORM*, cmsFloat32Number fOut[], cmsUInt8Number* output, cmsUInt32Number BytesPerPlaneOut);

// Forward declaration of the focal method (re-implemented here for test harness)
void FloatXFORM(_cmsTRANSFORM* p,
                const void* in,
                void* out, 
                cmsUInt32Number PixelsPerLine,
                cmsUInt32Number LineCount,
                const cmsStride* Stride);

// Mocked pipeline evaluation function
static void cmsPipelineEvalFloat(const cmsFloat32Number in[],
                                 cmsFloat32Number* out,
                                 void* pipeline)
{
    // Two sentinel behaviors to control tests:
    // - If pipeline == GAMUT_MARKER => simulate "Out of gamut" (OutOfGamut = 1.0)
    // - If pipeline == LUT_SENTINEL => simulate LUT path; set deterministic fOut
    // - Otherwise, default to no gamut (OutOfGamut = 0.0)
    if (pipeline == GAMUT_MARKER) {
        if (out != nullptr) *out = 1.0f;
    }
    else if (pipeline == LUT_SENTINEL) {
        // Fill fOut with a deterministic pattern for testing
        for (size_t c = 0; c < cmsMAXCHANNELS; ++c) {
            out[c] = 2.0f + (float)c; // e.g., [2.0, 3.0, 4.0, 5.0]
        }
    } else {
        // Default: zero-out (no-op path)
        if (out != nullptr) *out = 0.0f;
    }
}

// Minimal FromInputFloat implementation: do nothing (keep fIn initialized to 0 by memset in caller)
static cmsUInt8Number* mock_FromInputFloat(_cmsTRANSFORM* p,
                                           cmsFloat32Number fIn[],
                                           cmsUInt8Number* accum,
                                           cmsUInt32Number BytesPerPlaneIn)
{
    (void)p; (void)fIn; (void)accum; (void)BytesPerPlaneIn;
    return accum;
}

// Minimal ToOutputFloat implementation: copy fOut[] into the output buffer
static cmsUInt8Number* mock_ToOutputFloat(_cmsTRANSFORM* p,
                                         cmsFloat32Number fOut[],
                                         cmsUInt8Number* output,
                                         cmsUInt32Number BytesPerPlaneOut)
{
    (void)p; (void)BytesPerPlaneOut;
    // Write cmsMAXCHANNELS floats into the output buffer
    cmsFloat32Number* outF = reinterpret_cast<cmsFloat32Number*>(output);
    for (unsigned i = 0; i < cmsMAXCHANNELS; ++i) {
        outF[i] = fOut[i];
    }
    return output + cmsMAXCHANNELS * sizeof(cmsFloat32Number);
}

// Lightweight allocator for transform mock (not strictly needed by tests)
static _cmsTRANSFORM* AllocMockTransform()
{
    _cmsTRANSFORM* t = new _cmsTRANSFORM;
    t->ContextID = (void*)0xCAFEBABE;
    t->GamutCheck = NULL_PTR;
    t->FromInputFloat = mock_FromInputFloat;
    t->ToOutputFloat   = mock_ToOutputFloat;
    t->Lut               = LUT_SENTINEL;
    return t;
}

// The focal method copied and adapted for the test harness.
// It mirrors the logic from the provided code; a simplified environment is assumed here.
void FloatXFORM(_cmsTRANSFORM* p,
                const void* in,
                void* out, 
                cmsUInt32Number PixelsPerLine,
                cmsUInt32Number LineCount,
                const cmsStride* Stride)
{
    {
        cmsUInt8Number* accum;
        cmsUInt8Number* output;
        cmsFloat32Number fIn[cmsMAXCHANNELS], fOut[cmsMAXCHANNELS];
        cmsFloat32Number OutOfGamut;
        size_t i, j, c, strideIn, strideOut;

        _cmsHandleExtraChannels(p, in, out, PixelsPerLine, LineCount, Stride);
        strideIn = 0;
        strideOut = 0;
        memset(fIn, 0, sizeof(fIn));
        memset(fOut, 0, sizeof(fOut));

        for (i = 0; i < LineCount; i++) {
            accum = (cmsUInt8Number*)in + strideIn;
            output = (cmsUInt8Number*)out + strideOut;
            for (j = 0; j < PixelsPerLine; j++) {
                accum = p->FromInputFloat(p, fIn, accum, Stride->BytesPerPlaneIn);

                // Any gamut check to do?
                if (p->GamutCheck != NULL) {
                    // Evaluate gamut marker.
                    cmsPipelineEvalFloat(fIn, &OutOfGamut, p->GamutCheck);
                    // Is current color out of gamut?
                    if (OutOfGamut > 0.0) {
                        _cmsAlarmCodesChunkType* ContextAlarmCodes = (_cmsAlarmCodesChunkType*)_cmsContextGetClientChunk(p->ContextID, AlarmCodesContext);
                        // Certainly, out of gamut
                        for (c = 0; c < cmsMAXCHANNELS; c++)
                            fOut[c] = ContextAlarmCodes->AlarmCodes[c] / 65535.0F;
                    }
                    else {
                        // No, proceed normally
                        cmsPipelineEvalFloat(fIn, fOut, p->Lut);
                    }
                }
                else {
                    // No gamut check at all
                    cmsPipelineEvalFloat(fIn, fOut, p->Lut);
                }

                output = p->ToOutputFloat(p, fOut, output, Stride->BytesPerPlaneOut);
            }
            strideIn += Stride->BytesPerLineIn;
            strideOut += Stride->BytesPerLineOut;
        }
    }
}

// Simple non-terminating test assertion helpers
static int gTestFailures = 0;
static int gTestTotal    = 0;

#define EXPECT_TRUE(cond, desc) \
    do { \
        ++gTestTotal; \
        if(!(cond)) { \
            ++gTestFailures; \
            std::cerr << "EXPECT_TRUE failed: " << (desc) << "  at " << __FILE__ << ":" << __LINE__ << std::endl; \
        } \
    } while(0)

#define EXPECT_FLOAT_EQ(a, b, tol, desc) \
    do { \
        ++gTestTotal; \
        if(!(std::fabs((a) - (b)) <= (tol))) { \
            ++gTestFailures; \
            std::cerr << "EXPECT_FLOAT_EQ failed: " << (desc) << "  got " << (a) << " != " << (b) \
                      << "  at " << __FILE__ << ":" << __LINE__ << std::endl; \
        } \
    } while(0)

#define EXPECT_EQ_INT(a, b, desc) \
    do { \
        ++gTestTotal; \
        if((a) != (b)) { \
            ++gTestFailures; \
            std::cerr << "EXPECT_EQ_INT failed: " << (desc) << "  got " << (a) << " != " << (b) \
                      << "  at " << __FILE__ << ":" << __LINE__ << std::endl; \
        } \
    } while(0)

// Domain knowledge: Unit tests should cover true/false branches of predicates and
// ensure static-like path coverage via test harness. We implement 3 core tests plus a
// multi-pixel scenario to exercise loops.

// Test 1: GamutCheck is NULL -> No gamut path; must exercise LUT path
//         Expectation: out buffer contains the LUT-produced values (2.0, 3.0, 4.0, 5.0)
void test_FloatXFORM_NoGamutCheck_LutPath()
{
    std::cout << "Test 1: GamutCheck == NULL -> LUT path should drive fOut via LUT and write output.\n";

    _cmsTRANSFORM trans;
    trans.ContextID = (void*)0x1234;
    trans.GamutCheck = NULL;
    trans.FromInputFloat = mock_FromInputFloat;
    trans.ToOutputFloat = mock_ToOutputFloat;
    trans.Lut = LUT_SENTINEL;

    // Input/output buffers
    cmsFloat32Number inBuf[cmsMAXCHANNELS] = { 0.0f, 0.0f, 0.0f, 0.0f };
    cmsFloat32Number outBuf[cmsMAXCHANNELS] = { 0 };
    cmsStride Str { 0, 0, 0, 0 };

    // Call
    FloatXFORM(&trans, inBuf, outBuf, 1, 1, &Str);

    // Validate: The LUT path should have filled fOut with [2.0, 3.0, 4.0, 5.0]
    EXPECT_FLOAT_EQ(outBuf[0], 2.0f, 1e-6f, "Test1: pixel0 ch0");
    EXPECT_FLOAT_EQ(outBuf[1], 3.0f, 1e-6f, "Test1: pixel0 ch1");
    EXPECT_FLOAT_EQ(outBuf[2], 4.0f, 1e-6f, "Test1: pixel0 ch2");
    EXPECT_FLOAT_EQ(outBuf[3], 5.0f, 1e-6f, "Test1: pixel0 ch3");
}

// Test 2: GamutCheck non-NULL with OutOfGamut > 0 -> use AlarmCodesContext
//         Expectation: fOut filled with AlarmCodes / 65535.0F
void test_FloatXFORM_GamutOutOfGamut_AlarmCodes()
{
    std::cout << "Test 2: GamutCheck != NULL with OutOfGamut > 0 -> use AlarmCodes to fill fOut.\n";

    _cmsTRANSFORM trans;
    trans.ContextID = (void*)0x1234;
    trans.FromInputFloat = mock_FromInputFloat;
    trans.ToOutputFloat = mock_ToOutputFloat;
    trans.Lut = LUT_SENTINEL;

    // Point GamutCheck to GAMUT_MARKER => OutOfGamut will be set to 1.0 by cmsPipelineEvalFloat
    trans.GamutCheck = GAMUT_MARKER;

    cmsFloat32Number inBuf[cmsMAXCHANNELS] = { 0.0f };
    cmsFloat32Number outBuf[cmsMAXCHANNELS] = { 0.0f };

    cmsStride Str { 0, 0, 0, 0 };

    FloatXFORM(&trans, inBuf, outBuf, 1, 1, &Str);

    // AlarmCodesChunk: {65535, 32768, 0, 65535} -> normalized {1.0, 0.5, 0.0, 1.0}
    EXPECT_FLOAT_EQ(outBuf[0], 1.0f, 1e-6f, "Test2: ch0");
    EXPECT_FLOAT_EQ(outBuf[1], 0.5f, 1e-6f, "Test2: ch1");
    EXPECT_FLOAT_EQ(outBuf[2], 0.0f, 1e-6f, "Test2: ch2");
    EXPECT_FLOAT_EQ(outBuf[3], 1.0f, 1e-6f, "Test2: ch3");
}

// Test 3: GamutCheck non-NULL but OutOfGamut == 0 -> normal LUT path executed
//         Expectation: identical to LUT path as in Test 1
void test_FloatXFORM_GamutCheckNoOutOfGamut_LutPath()
{
    std::cout << "Test 3: GamutCheck != NULL with OutOfGamut == 0 -> fall back to LUT path.\n";

    _cmsTRANSFORM trans;
    trans.ContextID = (void*)0x1234;
    trans.FromInputFloat = mock_FromInputFloat;
    trans.ToOutputFloat = mock_ToOutputFloat;
    trans.Lut = LUT_SENTINEL;

    trans.GamutCheck = NO_GAMUT_MARKER;

    cmsFloat32Number inBuf[cmsMAXCHANNELS] = { 0.0f, 0.0f, 0.0f, 0.0f };
    cmsFloat32Number outBuf[cmsMAXCHANNELS] = { 0.0f };

    cmsStride Str { 0, 0, 0, 0 };

    FloatXFORM(&trans, inBuf, outBuf, 1, 1, &Str);

    // Should be identical to LUT path values
    EXPECT_FLOAT_EQ(outBuf[0], 2.0f, 1e-6f, "Test3: ch0");
    EXPECT_FLOAT_EQ(outBuf[1], 3.0f, 1e-6f, "Test3: ch1");
    EXPECT_FLOAT_EQ(outBuf[2], 4.0f, 1e-6f, "Test3: ch2");
    EXPECT_FLOAT_EQ(outBuf[3], 5.0f, 1e-6f, "Test3: ch3");
}

// Test 4: Multi-pixel case (PixelsPerLine > 1) with normal LUT path
//         We verify that the first pixel produces the expected LUT output and that
//         at least the function doesn't crash when looping over multiple pixels.
void test_FloatXFORM_MultiPixel_LutPath()
{
    std::cout << "Test 4: Multi-pixel case with LUT path (PixelsPerLine=2).\n";

    _cmsTRANSFORM trans;
    trans.ContextID = (void*)0x1234;
    trans.GamutCheck = NULL_PTR;
    trans.FromInputFloat = mock_FromInputFloat;
    trans.ToOutputFloat = mock_ToOutputFloat;
    trans.Lut = LUT_SENTINEL;

    cmsFloat32Number inBuf[cmsMAXCHANNELS * 2] = { 0.0f, 0.0f, 0.0f, 0.0f,   // first pixel
                                              0.0f, 0.0f, 0.0f, 0.0f }; // second pixel (unused in this simple mock)
    cmsFloat32Number outBuf[cmsMAXCHANNELS * 2] = { 0.0f };

    cmsStride Str { 0, 0, 0, 0 };

    // Call with 2 Pixels per line, 1 line
    FloatXFORM(&trans, inBuf, outBuf, 2, 1, &Str);

    // First pixel should be LUT-derived: [2.0, 3.0, 4.0, 5.0]
    EXPECT_FLOAT_EQ(outBuf[0], 2.0f, 1e-6f, "Test4: pixel0 ch0");
    EXPECT_FLOAT_EQ(outBuf[1], 3.0f, 1e-6f, "Test4: pixel0 ch1");
    EXPECT_FLOAT_EQ(outBuf[2], 4.0f, 1e-6f, "Test4: pixel0 ch2");
    EXPECT_FLOAT_EQ(outBuf[3], 5.0f, 1e-6f, "Test4: pixel0 ch3");
}

int main()
{
    // Run test cases
    test_FloatXFORM_NoGamutCheck_LutPath();
    test_FloatXFORM_GamutOutOfGamut_AlarmCodes();
    test_FloatXFORM_GamutCheckNoOutOfGamut_LutPath();
    test_FloatXFORM_MultiPixel_LutPath();

    // Summary
    std::cout << "\nTest Summary: " << gTestTotal << " tests executed, "
              << gTestFailures << " failures.\n";
    if (gTestFailures == 0) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cout << "Some tests FAILED.\n";
        return 1;
    }
}
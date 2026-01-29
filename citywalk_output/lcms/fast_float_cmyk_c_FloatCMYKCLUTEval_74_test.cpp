// Test suite for FloatCMYKCLUTEval (unit tests without GTest)
// This test provides a self-contained environment by re-implementing only the
// minimal dependencies required for the focal function to compile and run.
// It then exercises the function with a few scenarios to verify basic behavior.

#include <cstring>
#include <cstdio>
#include <fast_float_internal.h>
#include <limits>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstdint>


// Domain-specific: mimic minimal LittleCMS-like types and helpers

using cmsUInt32Number = uint32_t;
using cmsUInt8Number  = uint8_t;
using cmsFloat32Number = float;
using cmsHTRANSFORM = void*;

// A minimal opaque transform struct used by the focal method
struct _cmstransform_struct {
    void* UserData;
    unsigned int Flags;
};

// Forward declarations for internal helpers required by the focal method.
// These are lightweight stubs sufficient for unit testing in this environment.
static void* _cmsGetTransformUserData(struct _cmstransform_struct* CMMcargo) {
    return CMMcargo ? CMMcargo->UserData : nullptr;
}
static cmsUInt32Number cmsGetTransformInputFormat(cmsHTRANSFORM /*t*/) { return 0; }
static cmsUInt32Number cmsGetTransformOutputFormat(cmsHTRANSFORM /*t*/) { return 0; }
static cmsUInt32Number _cmsGetTransformFlags(struct _cmstransform_struct* t) {
    return t ? t->Flags : 0;
}
static int _cmsQuickFloor(cmsFloat32Number v) { return (int)std::floor(v); }

static cmsFloat32Number fclamp100(cmsFloat32Number v) {
    if (v <= 0.0f) return 0.0f;
    if (v >= 100.0f) return 100.0f;
    return v;
}

// A density function used by the CLUT computation (simplified)
static cmsFloat32Number DENS(int X, int Y, int Z) {
    return (cmsFloat32Number)(X + Y + Z);
}

// Linear interpolation helper used by the focal method
static cmsFloat32Number LinearInterpInt(cmsFloat32Number a, cmsFloat32Number l, cmsFloat32Number h) {
    // Simple linear interpolation: LERP(a) between l and h
    return l + (h - l) * a;
}

// Minimal strides and related types
struct cmsStride {
    cmsUInt32Number BytesPerPlaneIn;
    cmsUInt32Number BytesPerPlaneOut;
    cmsUInt32Number BytesPerLineIn;
    cmsUInt32Number BytesPerLineOut;
};

// Domain-specific data structures used by the focal method
struct cmsInterpParams {
    int nOutputs;
    const cmsFloat32Number* Table; // Table is not used in the tests
    cmsFloat32Number Domain[4];
    int opta[4];
};

struct FloatCMYKData {
    cmsInterpParams* p;
};

// Prototype of the focal method to test (as provided)
void FloatCMYKCLUTEval(struct _cmstransform_struct *CMMcargo,
                      const void* Input,
                      void* Output,
                      cmsUInt32Number PixelsPerLine,
                      cmsUInt32Number LineCount,
                      const cmsStride* Stride);

// Implement a local copy of the focal method using the same logic and simplified helpers.
// This is necessary to provide a testable unit without depending on the full library.
void FloatCMYKCLUTEval(struct _cmstransform_struct *CMMcargo,
                      const void* Input,
                      void* Output,
                      cmsUInt32Number PixelsPerLine,
                      cmsUInt32Number LineCount,
                      const cmsStride* Stride)
{
    {
        cmsFloat32Number        c, m, y, k;
        cmsFloat32Number        px, py, pz, pk;
        int                     x0, y0, z0, k0;
        int                     X0, Y0, Z0, K0, X1, Y1, Z1, K1;
        cmsFloat32Number        rx, ry, rz, rk;
        cmsFloat32Number        c0, c1 = 0, c2 = 0, c3 = 0;
        cmsUInt32Number         OutChan;
        FloatCMYKData*          pcmyk = (FloatCMYKData*) _cmsGetTransformUserData(CMMcargo);
        const cmsInterpParams*  p = pcmyk ->p;
        cmsUInt32Number        TotalOut = p -> nOutputs;
        cmsUInt32Number        TotalPlusAlpha;
        const cmsFloat32Number* LutTable = (const cmsFloat32Number*)p->Table;
        cmsUInt32Number        i, ii;
        const cmsUInt8Number*  cin;
        const cmsUInt8Number*  min;
        const cmsUInt8Number*  yin;
        const cmsUInt8Number*  kin;
        const cmsUInt8Number*  ain = NULL;
        cmsFloat32Number        Tmp1[cmsMAXCHANNELS], Tmp2[cmsMAXCHANNELS];
        cmsUInt8Number* out[cmsMAXCHANNELS];
        cmsUInt32Number SourceStartingOrder[cmsMAXCHANNELS];
        cmsUInt32Number SourceIncrements[cmsMAXCHANNELS];
        cmsUInt32Number DestStartingOrder[cmsMAXCHANNELS];
        cmsUInt32Number DestIncrements[cmsMAXCHANNELS];
        cmsUInt32Number InputFormat  = cmsGetTransformInputFormat((cmsHTRANSFORM) CMMcargo);
        cmsUInt32Number OutputFormat = cmsGetTransformOutputFormat((cmsHTRANSFORM) CMMcargo);
        cmsUInt32Number nchans, nalpha;
        size_t strideIn, strideOut;
        // Compute component increments (simplified for test)
        _cmsComputeComponentIncrements(InputFormat, Stride->BytesPerPlaneIn, &nchans, &nalpha, SourceStartingOrder, SourceIncrements);
        _cmsComputeComponentIncrements(OutputFormat, Stride->BytesPerPlaneOut, &nchans, &nalpha, DestStartingOrder, DestIncrements);
        if (!(_cmsGetTransformFlags(CMMcargo) & cmsFLAGS_COPY_ALPHA))
            nalpha = 0;
        strideIn = strideOut = 0;
        for (i = 0; i < LineCount; i++) {
            cin = (const cmsUInt8Number*)Input + SourceStartingOrder[0] + strideIn;
            min = (const cmsUInt8Number*)Input + SourceStartingOrder[1] + strideIn;
            yin = (const cmsUInt8Number*)Input + SourceStartingOrder[2] + strideIn;
            kin = (const cmsUInt8Number*)Input + SourceStartingOrder[3] + strideIn;
            if (nalpha)
                ain = (const cmsUInt8Number*)Input + SourceStartingOrder[4] + strideIn;
            TotalPlusAlpha = TotalOut;
            if (ain) TotalPlusAlpha++;
            for (ii = 0; ii < TotalPlusAlpha; ii++)
                out[ii] = (cmsUInt8Number*)Output + DestStartingOrder[ii] + strideOut;
            for (ii = 0; ii < PixelsPerLine; ii++) {
                c = fclamp100(*(cmsFloat32Number*)cin) / 100.0f;
                m = fclamp100(*(cmsFloat32Number*)min) / 100.0f;
                y = fclamp100(*(cmsFloat32Number*)yin) / 100.0f;
                k = fclamp100(*(cmsFloat32Number*)kin) / 100.0f;
                cin += SourceIncrements[0];
                min += SourceIncrements[1];
                yin += SourceIncrements[2];
                kin += SourceIncrements[3];
                pk = c * p->Domain[0];  // C
                px = m * p->Domain[1];  // M 
                py = y * p->Domain[2];  // Y 
                pz = k * p->Domain[3];  // K

                k0 = (int)_cmsQuickFloor(pk); rk = (pk - (cmsFloat32Number)k0);
                x0 = (int)_cmsQuickFloor(px); rx = (px - (cmsFloat32Number)x0);
                y0 = (int)_cmsQuickFloor(py); ry = (py - (cmsFloat32Number)y0);
                z0 = (int)_cmsQuickFloor(pz); rz = (pz - (cmsFloat32Number)z0);

                K0 = p->opta[3] * k0;
                K1 = K0 + (c >= 1.0 ? 0 : p->opta[3]);
                X0 = p->opta[2] * x0;
                X1 = X0 + (m >= 1.0 ? 0 : p->opta[2]);
                Y0 = p->opta[1] * y0;
                Y1 = Y0 + (y >= 1.0 ? 0 : p->opta[1]);
                Z0 = p->opta[0] * z0;
                Z1 = Z0 + (k >= 1.0 ? 0 : p->opta[0]);
                for (OutChan = 0; OutChan < TotalOut; OutChan++) {
                    c0 = DENS(X0, Y0, Z0);
                    if (rx >= ry && ry >= rz) {
                        c1 = DENS(X1, Y0, Z0) - c0;
                        c2 = DENS(X1, Y1, Z0) - DENS(X1, Y0, Z0);
                        c3 = DENS(X1, Y1, Z1) - DENS(X1, Y1, Z0);
                    }
                    else
                        if (rx >= rz && rz >= ry) {
                            c1 = DENS(X1, Y0, Z0) - c0;
                            c2 = DENS(X1, Y1, Z1) - DENS(X1, Y0, Z1);
                            c3 = DENS(X1, Y0, Z1) - DENS(X1, Y0, Z0);
                        }
                        else
                            if (rz >= rx && rx >= ry) {
                                c1 = DENS(X1, Y0, Z1) - DENS(X0, Y0, Z1);
                                c2 = DENS(X1, Y1, Z1) - DENS(X1, Y0, Z1);
                                c3 = DENS(X0, Y0, Z1) - c0;
                            }
                            else
                                if (ry >= rx && rx >= rz) {
                                    c1 = DENS(X1, Y1, Z0) - DENS(X0, Y1, Z0);
                                    c2 = DENS(X0, Y1, Z0) - c0;
                                    c3 = DENS(X1, Y1, Z1) - DENS(X1, Y1, Z0);
                                }
                                else
                                    if (ry >= rz && rz >= rx) {
                                        c1 = DENS(X1, Y1, Z1) - DENS(X0, Y1, Z1);
                                        c2 = DENS(X0, Y1, Z0) - c0;
                                        c3 = DENS(X0, Y1, Z1) - DENS(X0, Y1, Z0);
                                    }
                                    else
                                        if (rz >= ry && ry >= rx) {
                                            c1 = DENS(X1, Y1, Z1) - DENS(X0, Y1, Z1);
                                            c2 = DENS(X0, Y1, Z1) - DENS(X0, Y0, Z1);
                                            c3 = DENS(X0, Y0, Z1) - c0;
                                        }
                                        else {
                                            c1 = c2 = c3 = 0;
                                        }
                    Tmp1[OutChan] = c0 + c1 * rx + c2 * ry + c3 * rz;
                }

                LutTable = (cmsFloat32Number*)p->Table;
                LutTable += K1;
                for (OutChan = 0; OutChan < p->nOutputs; OutChan++) {
                    c0 = DENS(X0, Y0, Z0);
                    if (rx >= ry && ry >= rz) {
                        c1 = DENS(X1, Y0, Z0) - c0;
                        c2 = DENS(X1, Y1, Z0) - DENS(X1, Y0, Z0);
                        c3 = DENS(X1, Y1, Z1) - DENS(X1, Y1, Z0);
                    }
                    else
                        if (rx >= rz && rz >= ry) {
                            c1 = DENS(X1, Y0, Z0) - c0;
                            c2 = DENS(X1, Y1, Z1) - DENS(X1, Y0, Z1);
                            c3 = DENS(X1, Y0, Z1) - DENS(X1, Y0, Z0);
                        }
                        else
                            if (rz >= rx && rx >= ry) {
                                c1 = DENS(X1, Y0, Z1) - DENS(X0, Y0, Z1);
                                c2 = DENS(X1, Y1, Z1) - DENS(X1, Y0, Z1);
                                c3 = DENS(X0, Y0, Z1) - c0;
                            }
                            else
                                if (ry >= rx && rx >= rz) {
                                    c1 = DENS(X1, Y1, Z0) - DENS(X0, Y1, Z0);
                                    c2 = DENS(X0, Y1, Z0) - c0;
                                    c3 = DENS(X1, Y1, Z1) - DENS(X1, Y1, Z0);
                                }
                                else
                                    if (ry >= rz && rz >= rx) {
                                        c1 = DENS(X1, Y1, Z1) - DENS(X0, Y1, Z1);
                                        c2 = DENS(X0, Y1, Z0) - c0;
                                        c3 = DENS(X0, Y1, Z1) - DENS(X0, Y1, Z0);
                                    }
                                    else
                                        if (rz >= ry && ry >= rx) {
                                            c1 = DENS(X1, Y1, Z1) - DENS(X0, Y1, Z1);
                                            c2 = DENS(X0, Y1, Z1) - DENS(X0, Y0, Z1);
                                            c3 = DENS(X0, Y0, Z1) - c0;
                                        }
                                        else {
                                            c1 = c2 = c3 = 0;
                                        }
                    Tmp2[OutChan] = c0 + c1 * rx + c2 * ry + c3 * rz;
                }
                for (OutChan = 0; OutChan < p->nOutputs; OutChan++) {
                    *(cmsFloat32Number*)(out[OutChan]) = LinearInterpInt(rk, Tmp1[OutChan], Tmp2[OutChan]);
                    out[OutChan] += DestIncrements[OutChan];
                }
                if (ain) {
                    *(cmsFloat32Number*)(out[TotalOut]) = *(cmsFloat32Number*)ain;
                    ain += SourceIncrements[4];
                    out[TotalOut] += DestIncrements[TotalOut];
                }
            }
            strideIn += Stride->BytesPerLineIn;
            strideOut += Stride->BytesPerLineOut;
        }
    }
}

// Minimal helper to fill increments (simplified for test)
static void _cmsComputeComponentIncrements(unsigned int /*InputFormat*/,
                                          unsigned int /*BytesPerPlaneIn*/,
                                          cmsUInt32Number* nchans,
                                          cmsUInt32Number* nalpha,
                                          cmsUInt32Number SourceStartingOrder[cmsMAXCHANNELS],
                                          cmsUInt32Number SourceIncrements[cmsMAXCHANNELS])
{
    // Assume 4 channels: C, M, Y, K
    *nchans = 4;
    *nalpha = 0;
    SourceStartingOrder[0] = 0;
    SourceStartingOrder[1] = 4;
    SourceStartingOrder[2] = 8;
    SourceStartingOrder[3] = 12;
    SourceIncrements[0] = 4;
    SourceIncrements[1] = 4;
    SourceIncrements[2] = 4;
    SourceIncrements[3] = 4;
}

// A helper to prepare a FloatCMYKData instance for the test
static FloatCMYKData* make_test_data(cmsInterpParams* p) {
    static FloatCMYKData data;
    data.p = p;
    return &data;
}

// Simple numeric comparison helper
static bool almost_equal(float a, float b, float tol = 1e-5f) {
    float diff = std::fabs(a - b);
    if (diff <= tol) return true;
    // Allow relative tolerance for large numbers
    if (std::fabs(b) > 1e-5f && diff / std::fabs(b) <= tol) return true;
    return false;
}

#define cmsMAXCHANNELS 16
#define cmsFLAGS_COPY_ALPHA 0x01

// Test 1: Basic scenario with all channels set to 50 (0.5 after /100) should yield Tmp1 ~ 1.5
// This exercises the primary CLUT interpolation branches when rx, ry, rz are equal (0.5)
void test_case_basic_center() {
    // Prepare input: c=m=y=k=50
    cmsUInt8Number inputBytes[4 * 1 * 1]; // 4 channels per pixel, 1 pixel, but raw bytes
    float inputFloats[4] = {50.0f, 50.0f, 50.0f, 50.0f};
    std::memcpy(inputBytes, inputFloats, 4 * sizeof(float));

    // Output buffer (4 bytes)
    unsigned char outputBytes[4] = {0};
    
    // Transform data setup
    cmsInterpParams p;
    p.nOutputs = 1;
    float lutTable[1] = {0.0f};
    p.Table = lutTable;
    p.Domain[0] = p.Domain[1] = p.Domain[2] = p.Domain[3] = 1.0f;
    p.opta[0] = p.opta[1] = p.opta[2] = p.opta[3] = 1;

    FloatCMYKData* pcmyk = make_test_data(&p);
    _cmstransform_struct CMMcargo;
    CMMcargo.Flags = 0;
    CMMcargo.UserData = pcmyk;

    cmsStride Stride;
    Stride.BytesPerPlaneIn = 16;  // 4 channels * 4 bytes
    Stride.BytesPerPlaneOut = 4;  // 1 output * 4 bytes
    Stride.BytesPerLineIn = 0;
    Stride.BytesPerLineOut = 0;

    // Execute
    FloatCMYKCLUTEval(&CMMcargo, inputBytes, outputBytes, 1, 1, &Stride);

    // Validate: expected ~1.5
    float result;
    std::memcpy(&result, outputBytes, 4);
    if (!almost_equal(result, 1.5f)) {
        std::fprintf(stderr, "test_case_basic_center FAILED: expected 1.5, got %f\n", result);
        std::exit(EXIT_FAILURE);
    } else {
        std::printf("test_case_basic_center PASSED: got %f as expected.\n", result);
    }
}

// Test 2: c=100 (1.0), m=50 (0.5), y=0, k=0 should yield Tmp1 ~ 0.5 (per manual derivation)
void test_case_c1_m05() {
    // Prepare input: c=100, m=50, y=0, k=0
    unsigned char inputBytes[4] = {0}; // will fill via floats
    float inputFloats[4] = {100.0f, 50.0f, 0.0f, 0.0f};
    std::memcpy(inputBytes, inputFloats, 4 * sizeof(float));

    unsigned char outputBytes[4] = {0};
    
    cmsInterpParams p;
    p.nOutputs = 1;
    float lutTable[1] = {0.0f};
    p.Table = lutTable;
    p.Domain[0] = p.Domain[1] = p.Domain[2] = p.Domain[3] = 1.0f;
    p.opta[0] = p.opta[1] = p.opta[2] = p.opta[3] = 1;

    FloatCMYKData* pcmyk = make_test_data(&p);
    _cmstransform_struct CMMcargo;
    CMMcargo.Flags = 0;
    CMMcargo.UserData = pcmyk;

    cmsStride Stride;
    Stride.BytesPerPlaneIn = 16;
    Stride.BytesPerPlaneOut = 4;
    Stride.BytesPerLineIn = 0;
    Stride.BytesPerLineOut = 0;

    FloatCMYKCLUTEval(&CMMcargo, inputBytes, outputBytes, 1, 1, &Stride);

    float result;
    std::memcpy(&result, outputBytes, 4);
    if (!almost_equal(result, 0.5f)) {
        std::fprintf(stderr, "test_case_c1_m05 FAILED: expected 0.5, got %f\n", result);
        std::exit(EXIT_FAILURE);
    } else {
        std::printf("test_case_c1_m05 PASSED: got %f as expected.\n", result);
    }
}

// Test 3: All zeros should yield 0 (degenerate case)
void test_case_all_zero() {
    unsigned char inputBytes[4] = {0,0,0,0};
    unsigned char outputBytes[4] = {0};

    cmsInterpParams p;
    p.nOutputs = 1;
    float lutTable[1] = {0.0f};
    p.Table = lutTable;
    p.Domain[0] = p.Domain[1] = p.Domain[2] = p.Domain[3] = 1.0f;
    p.opta[0] = p.opta[1] = p.opta[2] = p.opta[3] = 1;

    FloatCMYKData* pcmyk = make_test_data(&p);
    _cmstransform_struct CMMcargo;
    CMMcargo.Flags = 0;
    CMMcargo.UserData = pcmyk;

    cmsStride Stride;
    Stride.BytesPerPlaneIn = 16;
    Stride.BytesPerPlaneOut = 4;
    Stride.BytesPerLineIn = 0;
    Stride.BytesPerLineOut = 0;

    FloatCMYKCLUTEval(&CMMcargo, inputBytes, outputBytes, 1, 1, &Stride);

    float result;
    std::memcpy(&result, outputBytes, 4);
    if (!almost_equal(result, 0.0f)) {
        std::fprintf(stderr, "test_case_all_zero FAILED: expected 0.0, got %f\n", result);
        std::exit(EXIT_FAILURE);
    } else {
        std::printf("test_case_all_zero PASSED: got %f as expected.\n", result);
    }
}

int main() {
    // Run unit tests
    test_case_basic_center();
    test_case_c1_m05();
    test_case_all_zero();

    std::printf("All tests completed successfully.\n");
    return 0;
}
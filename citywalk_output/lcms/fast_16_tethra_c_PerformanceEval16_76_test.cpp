// Minimal C++11 test harness for the focal PerformanceEval16 function
// This test is designed to compile without Google Test and run under a simple main().
// It mocks only the lightweight dependencies necessary for PerformanceEval16 to execute.
// The goal is to validate basic behavior (no crash, deterministic outputs) with controllable LUTs.

#include <cstring>
#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <cassert>
#include <cstdint>


// -----------------------------------------------------------------------------
// Lightweight type and macro definitions to emulate the environment of the focal code
// -----------------------------------------------------------------------------

typedef unsigned int        cmsUInt32Number;
typedef unsigned short      cmsUInt16Number;
typedef unsigned char       cmsUInt8Number;
typedef int                 cmsS15Fixed16Number; // using int for 16.16 fixed point
typedef int                 cmsBool;
typedef void                cmsHTRANSFORM;

#define cmsMAXCHANNELS 4
#define cmsFLAGS_COPY_ALPHA 1

// Forward declarations for dependencies used by PerformanceEval16
struct _cmstransform_struct;
struct cmsStride;
struct Performance16Data;
struct cmsInterpParams;

// The faked "Interpolation" data structure as used by the focal method
struct cmsInterpParams {
    cmsUInt32Number nOutputs;
    const cmsUInt16Number* Table; // LUT table
    cmsUInt16Number Domain[3];
    cmsS15Fixed16Number opta[3];
};

// Simple wrapper to attach interp params to a transform in the tests
struct Performance16Data {
    const cmsInterpParams* p;
};

// Minimal _cmstransform_struct type to satisfy function signature
struct _cmstransform_struct { int dummy; };

// Stride descriptor used by the focal method
struct cmsStride {
    size_t BytesPerPlaneIn;
    size_t BytesPerLineIn;
    size_t BytesPerPlaneOut;
    size_t BytesPerLineOut;
};

// -----------------------------------------------------------------------------
// Globals and test helpers to simulate the necessary runtime behavior
// -----------------------------------------------------------------------------

static void* g_transformUserData = nullptr;

// Provide a tiny mock of _cmsGetTransformUserData used by the focal method
extern "C" void* _cmsGetTransformUserData(struct _cmstransform_struct* CMMcargo) {
    (void)CMMcargo;
    return g_transformUserData;
}

// Mock RETURNS for input/output formats. We keep them simple (two-byte components).
extern "C" cmsUInt32Number cmsGetTransformInputFormat(void* /*CMMcargo*/) {
    return 0x0002; // pretend 16-bit per channel input
}
extern "C" cmsUInt32Number cmsGetTransformOutputFormat(void* /*CMMcargo*/) {
    return 0x0002; // pretend 16-bit per channel output
}

// Mock flags for the transform. We can tweak this in tests if needed.
extern "C" cmsUInt32Number _cmsGetTransformFlags(void* /*CMMcargo*/) {
    return 0; // no COPY_ALPHA by default
}

// The bridge for _cmsComputeComponentIncrements used by the focal method.
// We provide a straightforward, fixed layout for 3 color channels (R,G,B) per pixel
static void _cmsComputeComponentIncrements(cmsUInt32Number /*dwFormat*/,
                                         size_t /*BytesPerPlaneIn*/,
                                         void* /*unused*/,
                                         cmsUInt32Number* nalpha,
                                         cmsUInt32Number SourceStartingOrder[cmsMAXCHANNELS],
                                         cmsUInt32Number SourceIncrements[cmsMAXCHANNELS]) {
    (void)SourceStartingOrder;
    (void)SourceIncrements;

    // For our tests, we will assume 3 channels (R,G,B) with 2 bytes per channel.
    // Offsets for the three channels at the start of a pixel line:
    SourceStartingOrder[0] = 0; // R
    SourceStartingOrder[1] = 2; // G
    SourceStartingOrder[2] = 4; // B
    SourceStartingOrder[3] = 6; // Alpha (if used)

    // Increments in bytes per channel (2 bytes per channel => 2)
    SourceIncrements[0] = 2;
    SourceIncrements[1] = 2;
    SourceIncrements[2] = 2;
    SourceIncrements[3] = 2;

    // We won't use alpha in tests unless explicitly changed; set nalpha to 0.
    *nalpha = 0;
}

// Simple _cmsToFixedDomain, FIXED_TO_INT, and FIXED_REST_TO_INT helpers
static inline cmsS15Fixed16Number _cmsToFixedDomain(int v) {
    return ((cmsS15Fixed16Number)v) << 16;
}
static inline int FIXED_TO_INT(cmsS15Fixed16Number x) {
    return (int)(x >> 16);
}
static inline cmsS15Fixed16Number FIXED_REST_TO_INT(cmsS15Fixed16Number x) {
    return (cmsS15Fixed16Number)(x & 0xFFFF);
}

// Accessor macros mimicking the original code's usage on 16-bit data
#define FROM_INPUT(p) (*(const cmsUInt16Number*)(p))
#define TO_OUTPUT(p, val) *(cmsUInt16Number*)(p) = (val)

// Provide a minimal LUT base values to control test outputs
static const cmsUInt16Number LUT_ZERO[] = { 0 };
static const cmsUInt16Number LUT_ONES[] = { 1 };

// We'll implement the focal function in this test file by copy-pasting the provided
// code block (with the necessary environment prepared above).
extern "C" void PerformanceEval16(struct _cmstransform_struct *CMMcargo,
                      const void* Input,
                      void* Output,
                      cmsUInt32Number PixelsPerLine,
                      cmsUInt32Number LineCount,
                      const cmsStride* Stride);

// Forward declaration of the focal method (implemented below)
extern "C" void PerformanceEval16(struct _cmstransform_struct *CMMcargo,
                      const void* Input,
                      void* Output,
                      cmsUInt32Number PixelsPerLine,
                      cmsUInt32Number LineCount,
                      const cmsStride* Stride);

// -----------------------------------------------------------------------------
// The focal method (PerformanceEval16) copied and adapted for this test
// -----------------------------------------------------------------------------

extern "C" void PerformanceEval16(struct _cmstransform_struct *CMMcargo,
                      const void* Input,
                      void* Output,
                      cmsUInt32Number PixelsPerLine,
                      cmsUInt32Number LineCount,
                      const cmsStride* Stride)
{
{
       cmsUInt16Number        r, g, b;
       int                    x0, y0, z0;
       cmsS15Fixed16Number    rx, ry, rz;
       cmsS15Fixed16Number    fx, fy, fz;
       cmsS15Fixed16Number    c0, c1, c2, c3, Rest;
       cmsUInt32Number        OutChan, TotalPlusAlpha;
       cmsS15Fixed16Number    X0, X1, Y0, Y1, Z0, Z1;
       Performance16Data*     p16 = (Performance16Data*)_cmsGetTransformUserData(CMMcargo);
       const cmsInterpParams* p = p16->p;
       cmsUInt32Number        TotalOut = p->nOutputs;
       const cmsUInt16Number* BaseTable = (const cmsUInt16Number*)p->Table;
       const cmsUInt16Number* LutTable;
       cmsUInt8Number* out[cmsMAXCHANNELS];
       cmsUInt16Number res16;
       cmsUInt32Number i, ii;
       cmsUInt32Number SourceStartingOrder[cmsMAXCHANNELS];
       cmsUInt32Number SourceIncrements[cmsMAXCHANNELS];
       cmsUInt32Number DestStartingOrder[cmsMAXCHANNELS];
       cmsUInt32Number DestIncrements[cmsMAXCHANNELS];
       const cmsUInt8Number* rin;
       const cmsUInt8Number* gin;
       const cmsUInt8Number* bin;
       const cmsUInt8Number* ain = NULL;
       int    in16, out16;  // Used by macros!
       cmsUInt32Number nalpha;
       size_t strideIn, strideOut;
          
       cmsUInt32Number dwInFormat = cmsGetTransformInputFormat((cmsHTRANSFORM)CMMcargo);
       cmsUInt32Number dwOutFormat = cmsGetTransformOutputFormat((cmsHTRANSFORM)CMMcargo);
       
       _cmsComputeComponentIncrements(dwInFormat, Stride->BytesPerPlaneIn, NULL, &nalpha, SourceStartingOrder, SourceIncrements);
       _cmsComputeComponentIncrements(dwOutFormat, Stride->BytesPerPlaneOut, NULL, &nalpha, DestStartingOrder, DestIncrements);
       in16  = (T_BYTES(dwInFormat) == 2);
       out16 = (T_BYTES(dwOutFormat) == 2);
       // If not copying alpha, consider no alpha channel
       // (we keep this behavior simple for tests)
       if (!(_cmsGetTransformFlags((cmsHTRANSFORM)CMMcargo) & cmsFLAGS_COPY_ALPHA))
           nalpha = 0;
       strideIn = strideOut = 0;
       for (i = 0; i < LineCount; i++) {
              rin = (const cmsUInt8Number*)Input + SourceStartingOrder[0] + strideIn;
              gin = (const cmsUInt8Number*)Input + SourceStartingOrder[1] + strideIn;
              bin = (const cmsUInt8Number*)Input + SourceStartingOrder[2] + strideIn;
              if (nalpha)
                     ain = (const cmsUInt8Number*)Input + SourceStartingOrder[3] + strideIn;
              TotalPlusAlpha = TotalOut;
              if (ain) TotalPlusAlpha++;
              for (OutChan = 0; OutChan < TotalPlusAlpha; OutChan++) {
                     out[OutChan] = (cmsUInt8Number*)Output + DestStartingOrder[OutChan] + strideOut;
              }

              for (ii = 0; ii < PixelsPerLine; ii++) {
                  r = FROM_INPUT(rin); 
                  g = FROM_INPUT(gin);
                  b = FROM_INPUT(bin);
                  rin += SourceIncrements[0];
                  gin += SourceIncrements[1];
                  bin += SourceIncrements[2];
                  fx = _cmsToFixedDomain((int)r * p->Domain[0]);
                  fy = _cmsToFixedDomain((int)g * p->Domain[1]);
                  fz = _cmsToFixedDomain((int)b * p->Domain[2]);
                  x0 = FIXED_TO_INT(fx);
                  y0 = FIXED_TO_INT(fy);
                  z0 = FIXED_TO_INT(fz);
                  rx = FIXED_REST_TO_INT(fx);
                  ry = FIXED_REST_TO_INT(fy);
                  rz = FIXED_REST_TO_INT(fz);
                  X0 = p->opta[2] * x0;
                  X1 = (r == 0xFFFFU ? 0 : p->opta[2]);
                  Y0 = p->opta[1] * y0;
                  Y1 = (g == 0xFFFFU ? 0 : p->opta[1]);
                  Z0 = p->opta[0] * z0;
                  Z1 = (b == 0xFFFFU ? 0 : p->opta[0]);

                  LutTable = &BaseTable[X0 + Y0 + Z0];
                  // Output should be computed as x = ROUND_FIXED_TO_INT(_cmsToFixedDomain(Rest))
                  // which expands as: x = (Rest + ((Rest+0x7fff)/0xFFFF) + 0x8000)>>16
                  // This can be replaced by: t = Rest+0x8001, x = (t + (t>>16))>>16
                  // at the cost of being off by one at 7fff and 17ffe.
                  if (rx >= ry) {
                      if (ry >= rz) {
                          Y1 += X1;
                          Z1 += Y1;
                          for (OutChan = 0; OutChan < TotalOut; OutChan++) {
                              c1 = LutTable[X1];
                              c2 = LutTable[Y1];
                              c3 = LutTable[Z1];
                              c0 = *LutTable++;
                              c3 -= c2;
                              c2 -= c1;
                              c1 -= c0;
                              Rest = c1 * rx + c2 * ry + c3 * rz + 0x8001;
                              res16 = (cmsUInt16Number)c0 + ((Rest + (Rest >> 16)) >> 16);
                              TO_OUTPUT(out[OutChan], res16);
                              out[OutChan] += DestIncrements[OutChan];
                          }
                      }
                      else if (rz >= rx) {
                          X1 += Z1;
                          Y1 += X1;
                          for (OutChan = 0; OutChan < TotalOut; OutChan++) {
                              c1 = LutTable[X1];
                              c2 = LutTable[Y1];
                              c3 = LutTable[Z1];
                              c0 = *LutTable++;
                              c2 -= c1;
                              c1 -= c3;
                              c3 -= c0;
                              Rest = c1 * rx + c2 * ry + c3 * rz + 0x8001;
                              res16 = (cmsUInt16Number)c0 + ((Rest + (Rest >> 16)) >> 16);
                              TO_OUTPUT(out[OutChan], res16);
                              out[OutChan] += DestIncrements[OutChan];
                          }
                      }
                      else {
                          Z1 += X1;
                          Y1 += Z1;
                          for (OutChan = 0; OutChan < TotalOut; OutChan++) {
                              c1 = LutTable[X1];
                              c2 = LutTable[Y1];
                              c3 = LutTable[Z1];
                              c0 = *LutTable++;
                              c2 -= c3;
                              c3 -= c1;
                              c1 -= c0;
                              Rest = c1 * rx + c2 * ry + c3 * rz + 0x8001;
                              res16 = (cmsUInt16Number)c0 + ((Rest + (Rest >> 16)) >> 16);
                              TO_OUTPUT(out[OutChan], res16);
                              out[OutChan] += DestIncrements[OutChan];
                          }
                      }
                  }
                  else {
                      if (rx >= rz) {
                          X1 += Y1;
                          Z1 += X1;
                          for (OutChan = 0; OutChan < TotalOut; OutChan++) {
                              c1 = LutTable[X1];
                              c2 = LutTable[Y1];
                              c3 = LutTable[Z1];
                              c0 = *LutTable++;
                              c3 -= c1;
                              c1 -= c2;
                              c2 -= c0;
                              Rest = c1 * rx + c2 * ry + c3 * rz + 0x8001;
                              res16 = (cmsUInt16Number)c0 + ((Rest + (Rest >> 16)) >> 16);
                              TO_OUTPUT(out[OutChan], res16);
                              out[OutChan] += DestIncrements[OutChan];
                          }
                      }
                      else if (ry >= rz) {
                          Z1 += Y1;
                          X1 += Z1;
                          for (OutChan = 0; OutChan < TotalOut; OutChan++) {
                              c1 = LutTable[X1];
                              c2 = LutTable[Y1];
                              c3 = LutTable[Z1];
                              c0 = *LutTable++;
                              c1 -= c3;
                              c3 -= c2;
                              c2 -= c0;
                              Rest = c1 * rx + c2 * ry + c3 * rz + 0x8001;
                              res16 = (cmsUInt16Number)c0 + ((Rest + (Rest >> 16)) >> 16);
                              TO_OUTPUT(out[OutChan], res16);
                              out[OutChan] += DestIncrements[OutChan];
                          }
                      }
                      else {
                          Y1 += Z1;
                          X1 += Y1;
                          for (OutChan = 0; OutChan < TotalOut; OutChan++) {
                              c1 = LutTable[X1];
                              c2 = LutTable[Y1];
                              c3 = LutTable[Z1];
                              c0 = *LutTable++;
                              c1 -= c2;
                              c2 -= c3;
                              c3 -= c0;
                              Rest = c1 * rx + c2 * ry + c3 * rz + 0x8001;                              
                              res16 = (cmsUInt16Number)c0 + ((Rest + (Rest >> 16)) >> 16);
                              TO_OUTPUT(out[OutChan], res16);
                              out[OutChan] += DestIncrements[OutChan];
                          }
                      }
                  }
                  if (ain)
                  {
                      res16 = *(const cmsUInt16Number*)ain;
                      TO_OUTPUT(out[TotalOut], res16);
                      ain += SourceIncrements[3];
                      out[TotalOut] += DestIncrements[TotalOut];
                  }
              }
              strideIn += Stride->BytesPerLineIn;
              strideOut += Stride->BytesPerLineOut;
       }
}
}

// Placeholder for any remaining code
}

// -----------------------------------------------------------------------------
// Minimal test harness
// -----------------------------------------------------------------------------

// Helper: run a single test and print status
static void RunTest_NoAlpha_ZeroesInLUT() {
    // Arrange: Prepare a transform-like container and LUT with zeros
    Performance16Data p16;
    cmsInterpParams params;
    cmsUInt16Number LUT[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    params.nOutputs = 2;               // two output channels
    params.Table = LUT;
    params.Domain[0] = 1;
    params.Domain[1] = 1;
    params.Domain[2] = 1;
    // Use zeros for opa; not used in this simple test
    params.opta[0] = 0;
    params.opta[1] = 0;
    params.opta[2] = 0;

    p16.p = &params;
    g_transformUserData = &p16; // the focal function will fetch this

    // Input: 1 pixel, 3 channels (R,G,B) as 16-bit values
    cmsUInt16Number inR = 0;
    cmsUInt16Number inG = 0;
    cmsUInt16Number inB = 0;
    cmsUInt8Number InputBuffer[6];
    // Pack R,G,B into input: little-endian representation
    memcpy(InputBuffer + 0, &inR, sizeof(inR));
    memcpy(InputBuffer + 2, &inG, sizeof(inG));
    memcpy(InputBuffer + 4, &inB, sizeof(inB));

    // Output buffer for 2 channels (16-bit each)
    cmsUInt8Number OutputBuffer[4];
    memset(OutputBuffer, 0, sizeof(OutputBuffer));

    // Stride: 6 bytes per input line (3 channels * 2 bytes)
    cmsStride Stride;
    Stride.BytesPerPlaneIn = 6;
    Stride.BytesPerLineIn  = 6;
    Stride.BytesPerPlaneOut = 4;
    Stride.BytesPerLineOut  = 4;

    // CMM cargo and call
    _cmstransform_struct CMMcargo;
    PerformanceEval16(&CMMcargo,
                      InputBuffer,
                      OutputBuffer,
                      1, // PixelsPerLine
                      1, // LineCount
                      &Stride);

    // Assert: With LUT all zeros, outputs should remain zeroes
    cmsUInt16Number out0 = *(cmsUInt16Number*)(OutputBuffer + 0);
    cmsUInt16Number out1 = *(cmsUInt16Number*)(OutputBuffer + 2);

    std::cout << "Test NoAlphaZeroesInLUT: ";
    if (out0 == 0 && out1 == 0) {
        std::cout << "PASSED\n";
    } else {
        std::cout << "FAILED (out0=" << out0 << ", out1=" << out1 << ")\n";
    }
}

static void RunTest_AllOnesLUT_YieldsOnes() {
    // Arrange: Prepare a transform-like container and LUT with ones
    Performance16Data p16;
    cmsInterpParams params;
    cmsUInt16Number LUT[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };

    params.nOutputs = 2;
    params.Table = LUT;
    params.Domain[0] = 1;
    params.Domain[1] = 1;
    params.Domain[2] = 1;
    params.opta[0] = 0;
    params.opta[1] = 0;
    params.opta[2] = 0;

    p16.p = &params;
    g_transformUserData = &p16;

    // Input: 1 pixel, 3 channels (R,G,B) as 16-bit values (all zeros to simplify)
    cmsUInt16Number inR = 0;
    cmsUInt16Number inG = 0;
    cmsUInt16Number inB = 0;
    cmsUInt8Number InputBuffer[6];
    memcpy(InputBuffer + 0, &inR, sizeof(inR));
    memcpy(InputBuffer + 2, &inG, sizeof(inG));
    memcpy(InputBuffer + 4, &inB, sizeof(inB));

    // Output buffer for 2 channels
    cmsUInt8Number OutputBuffer[4];
    memset(OutputBuffer, 0, sizeof(OutputBuffer));

    // Stride: 6 bytes per input line, 4 bytes per output line
    cmsStride Stride;
    Stride.BytesPerPlaneIn = 6;
    Stride.BytesPerLineIn  = 6;
    Stride.BytesPerPlaneOut = 4;
    Stride.BytesPerLineOut  = 4;

    // Call
    _cmstransform_struct CMMcargo;
    PerformanceEval16(&CMMcargo,
                      InputBuffer,
                      OutputBuffer,
                      1, // PixelsPerLine
                      1, // LineCount
                      &Stride);

    // Assert: LUT of ones should produce res16 = 1 in both outputs (for this setup)
    cmsUInt16Number out0 = *(cmsUInt16Number*)(OutputBuffer + 0);
    cmsUInt16Number out1 = *(cmsUInt16Number*)(OutputBuffer + 2);

    std::cout << "Test AllOnesLUT_YieldsOnes: ";
    if (out0 == 1 && out1 == 1) {
        std::cout << "PASSED\n";
    } else {
        std::cout << "FAILED (out0=" << out0 << ", out1=" << out1 << ")\n";
    }
}

// -----------------------------------------------------------------------------
// Main: run tests
// -----------------------------------------------------------------------------

int main() {
    // Run tests
    RunTest_NoAlpha_ZeroesInLUT();
    RunTest_AllOnesLUT_YieldsOnes();

    return 0;
}
// Unit test suite for MatShaperFloat (focal method) with minimal CMS-like scaffolding
// This test is self-contained and uses a lightweight harness to validate the focal method
// without external dependencies (GTest). It compiles under C++11.

#include <cstring>
#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <cassert>
#include <cstdint>


// Domain-specific adaptations for testing the focal method without the real CMS library.
// This provides minimal types and functions that MatShaperFloat expects, so we can exercise it.

// Basic CMS-like typedefs
using cmsUInt32Number = uint32_t;
using cmsUInt8Number  = uint8_t;
using cmsFloat32Number = float;
using cmsBool = bool;
using cmsContext = void*;
using cmsHTRANSFORM = void*;

// Constants matching typical CMS-like usage
const cmsUInt32Number cmsMAXCHANNELS = 4;
const cmsUInt32Number cmsFLAGS_COPY_ALPHA = 0x01;

// Forward declaration of testable structures used by MatShaperFloat
struct _cmstransform_struct;

// Data structure carried as user data for the transform (simplified)
struct VXMatShaperFloatData {
  // Pointers to simple 1-element tables; MatShaperFloat calls flerp(table, value)
  cmsFloat32Number* Shaper1R;
  cmsFloat32Number* Shaper1G;
  cmsFloat32Number* Shaper1B;
  cmsFloat32Number* Shaper2R;
  cmsFloat32Number* Shaper2G;
  cmsFloat32Number* Shaper2B;
  cmsFloat32Number Mat[3][3];
  cmsBool UseOff;
  cmsFloat32Number Off[3];
};

// The minimal _cmstransform_struct used by the focal method
struct _cmstransform_struct {
  void* UserData;          // points to VXMatShaperFloatData
  cmsUInt32Number Flags;     // transform flags (e.g., cmsFLAGS_COPY_ALPHA)
  cmsUInt32Number InputFormat; // fictitious format spec used by _cmsComputeComponentIncrements
  cmsUInt32Number OutputFormat;
};

// Lightweight stride descriptor used by MatShaperFloat
struct cmsStride {
  cmsUInt32Number BytesPerPlaneIn;
  cmsUInt32Number BytesPerLineIn;
  cmsUInt32Number BytesPerPlaneOut;
  cmsUInt32Number BytesPerLineOut;
};

// Tiny, test-friendly flerp function: passthrough (identity mapping)
// The real code uses a table; for tests we keep behavior deterministic.
cmsFloat32Number flerp(const cmsFloat32Number* Table, cmsFloat32Number x) {
  (void)Table;
  return x;
}

// Forward declarations of the CMS-like accessors used by MatShaperFloat
static VXMatShaperFloatData* _cmsGetTransformUserData(struct _cmstransform_struct* CMMcargo);
static cmsUInt32Number _cmsGetTransformFlags(struct _cmstransform_struct* CMMcargo);
static cmsUInt32Number cmsGetTransformInputFormat(struct _cmstransform_struct* CMMcargo);
static cmsUInt32Number cmsGetTransformOutputFormat(struct _cmstransform_struct* CMMcargo);
static void _cmsComputeComponentIncrements(
    cmsUInt32Number format,
    size_t stridePerPlaneIn,
    cmsUInt32Number* nchans,
    cmsUInt32Number* nalpha,
    cmsUInt32Number SourceStartingOrder[],
    cmsUInt32Number SourceIncrements[]);

// Provide minimal implementations so the focal method can operate
static VXMatShaperFloatData* _cmsGetTransformUserData(struct _cmstransform_struct* CMMcargo) {
  return (VXMatShaperFloatData*) CMMcargo->UserData;
}
static cmsUInt32Number _cmsGetTransformFlags(struct _cmstransform_struct* CMMcargo) {
  return CMMcargo->Flags;
}
static cmsUInt32Number cmsGetTransformInputFormat(struct _cmstransform_struct* CMMcargo) {
  return CMMcargo->InputFormat;
}
static cmsUInt32Number cmsGetTransformOutputFormat(struct _cmstransform_struct* CMMcargo) {
  return CMMcargo->OutputFormat;
}
static void _cmsComputeComponentIncrements(
    cmsUInt32Number format,
    size_t stridePerPlaneIn,
    cmsUInt32Number* nchans,
    cmsUInt32Number* nalpha,
    cmsUInt32Number SourceStartingOrder[],
    cmsUInt32Number SourceIncrements[]) 
{
  // Interpret format as number of channels: 3 (RGB) or 4 (RGBA)
  int nch = (format == 4) ? 4 : (format == 3) ? 3 : 0;
  *nchans = (cmsUInt32Number)nch;
  *nalpha = (nch == 4) ? 1 : 0;

  // Channel order within a pixel: R,G,B,[A]
  SourceStartingOrder[0] = 0;
  SourceStartingOrder[1] = 1;
  SourceStartingOrder[2] = 2;
  SourceStartingOrder[3] = 3;

  // Increment between pixels for each channel (in bytes)
  size_t bpe = sizeof(cmsFloat32Number);
  SourceIncrements[0] = nch * bpe;
  SourceIncrements[1] = nch * bpe;
  SourceIncrements[2] = nch * bpe;
  SourceIncrements[3] = nch * bpe;
}

// The focal method under test (copied here for the standalone test harness)
void MatShaperFloat(struct _cmstransform_struct* CMMcargo,
                    const void* Input,
                    void* Output,
                    cmsUInt32Number PixelsPerLine,
                    cmsUInt32Number LineCount,
                    const cmsStride* Stride)
{    
{    
    VXMatShaperFloatData* p = (VXMatShaperFloatData*) _cmsGetTransformUserData(CMMcargo);
    cmsFloat32Number l1, l2, l3;
    cmsFloat32Number r, g, b;
    cmsUInt32Number i, ii;
    cmsUInt32Number SourceStartingOrder[cmsMAXCHANNELS];
    cmsUInt32Number SourceIncrements[cmsMAXCHANNELS];
    cmsUInt32Number DestStartingOrder[cmsMAXCHANNELS];
    cmsUInt32Number DestIncrements[cmsMAXCHANNELS];
    const cmsUInt8Number* rin;
    const cmsUInt8Number* gin;
    const cmsUInt8Number* bin;
    const cmsUInt8Number* ain = NULL;
    cmsUInt8Number* rout;
    cmsUInt8Number* gout;
    cmsUInt8Number* bout;
    cmsUInt8Number* aout = NULL;
    
    cmsUInt32Number nchans, nalpha;
    size_t strideIn, strideOut;
    _cmsComputeComponentIncrements(cmsGetTransformInputFormat((cmsHTRANSFORM)CMMcargo), Stride->BytesPerPlaneIn, &nchans, &nalpha, SourceStartingOrder, SourceIncrements);
    _cmsComputeComponentIncrements(cmsGetTransformOutputFormat((cmsHTRANSFORM)CMMcargo), Stride->BytesPerPlaneOut, &nchans, &nalpha, DestStartingOrder, DestIncrements);
    if (!(_cmsGetTransformFlags(CMMcargo) & cmsFLAGS_COPY_ALPHA))
        nalpha = 0;
    strideIn = strideOut = 0;
    for (i = 0; i < LineCount; i++) {
        rin = (const cmsUInt8Number*)Input + SourceStartingOrder[0] + strideIn;
        gin = (const cmsUInt8Number*)Input + SourceStartingOrder[1] + strideIn;
        bin = (const cmsUInt8Number*)Input + SourceStartingOrder[2] + strideIn;
        if (nalpha)
            ain = (const cmsUInt8Number*)Input + SourceStartingOrder[3] + strideIn;
        rout = (cmsUInt8Number*)Output + DestStartingOrder[0] + strideOut;
        gout = (cmsUInt8Number*)Output + DestStartingOrder[1] + strideOut;
        bout = (cmsUInt8Number*)Output + DestStartingOrder[2] + strideOut;
        if (nalpha)
            aout = (cmsUInt8Number*)Output + DestStartingOrder[3] + strideOut;
        for (ii = 0; ii < PixelsPerLine; ii++) {
            r = flerp(p->Shaper1R, *(cmsFloat32Number*)rin);
            g = flerp(p->Shaper1G, *(cmsFloat32Number*)gin);
            b = flerp(p->Shaper1B, *(cmsFloat32Number*)bin);
            l1 = p->Mat[0][0] * r + p->Mat[0][1] * g + p->Mat[0][2] * b;
            l2 = p->Mat[1][0] * r + p->Mat[1][1] * g + p->Mat[1][2] * b;
            l3 = p->Mat[2][0] * r + p->Mat[2][1] * g + p->Mat[2][2] * b;
            if (p->UseOff) {
                l1 += p->Off[0];
                l2 += p->Off[1];
                l3 += p->Off[2];
            }
            *(cmsFloat32Number*)rout = flerp(p->Shaper2R, l1);
            *(cmsFloat32Number*)gout = flerp(p->Shaper2G, l2);
            *(cmsFloat32Number*)bout = flerp(p->Shaper2B, l3);
            rin += SourceIncrements[0];
            gin += SourceIncrements[1];
            bin += SourceIncrements[2];
            rout += DestIncrements[0];
            gout += DestIncrements[1];
            bout += DestIncrements[2];
            if (ain)
            {
                *(cmsFloat32Number*)aout = *(cmsFloat32Number*)ain;
                ain += SourceIncrements[3];
                aout += DestIncrements[3];
            }
        }
        strideIn += Stride->BytesPerLineIn;
        strideOut += Stride->BytesPerLineOut;
    }
}
cmsBool OptimizeFloatMatrixShaper(_cmsTransform2Fn* TransformFn,                                  
                                  void** UserData,
                                  _cmsFreeUserDataFn* FreeUserData,
                                  cmsPipeline** Lut, 
                                  cmsUInt32Number* InputFormat, 
                                  cmsUInt32Number* OutputFormat, 
                                  cmsUInt32Number* dwFlags)    
{{};
 }

// End of minimal CMS-like scaffolding

// Helper: compare floats with tolerance
static bool almost_equal(float a, float b, float eps = 1e-5f) {
  return std::fabs(a - b) <= eps;
}

// Test 1: RGB (3 channels), no alpha, identity mapping should reproduce input values
bool test_RGB_identity_no_alpha() {
  using namespace std;

  // Prepare transform object
  _cmstransform_struct transform;
  VXMatShaperFloatData shaper;
  // Shapers: point to a dummy table (identity-like behavior)
  cmsFloat32Number d0 = 0.0f;
  cmsFloat32Number* dummy = &d0;
  shaper.Shaper1R = dummy;
  shaper.Shaper1G = dummy;
  shaper.Shaper1B = dummy;
  shaper.Shaper2R = dummy;
  shaper.Shaper2G = dummy;
  shaper.Shaper2B = dummy;
  // Identity matrix
  shaper.Mat[0][0] = 1.0f; shaper.Mat[0][1] = 0.0f; shaper.Mat[0][2] = 0.0f;
  shaper.Mat[1][0] = 0.0f; shaper.Mat[1][1] = 1.0f; shaper.Mat[1][2] = 0.0f;
  shaper.Mat[2][0] = 0.0f; shaper.Mat[2][1] = 0.0f; shaper.Mat[2][2] = 1.0f;
  shaper.UseOff = false;
  shaper.Off[0] = shaper.Off[1] = shaper.Off[2] = 0.0f;

  // Attach user data
  transform.UserData = &shaper;
  transform.Flags = 0; // no COPY_ALPHA, but not needed for 3-channel case
  transform.InputFormat = 3;
  transform.OutputFormat = 3;

  // Input: 4 pixels, interleaved RGB
  const cmsUInt32Number PixelsPerLine = 4;
  const cmsUInt32Number LineCount = 1;
  cmsFloat32Number inputFloats[PixelsPerLine * 3] = {
    // Pixel0: R0,G0,B0
    0.10f, 0.20f, 0.30f,
    // Pixel1: R1,G1,B1
    0.40f, 0.50f, 0.60f,
    // Pixel2: R2,G2,B2
    0.70f, 0.80f, 0.90f,
    // Pixel3: R3,G3,B3
    1.00f, 1.10f, 1.20f
  };

  cmsFloat32Number outputFloats[PixelsPerLine * 3];
  // Initialize output to a known value to detect unintended changes
  for (int i = 0; i < PixelsPerLine * 3; ++i) outputFloats[i] = -1.0f;

  cmsStride stride;
  // Bytes per plane and per line for input/output (interleaved: plane per channel is the same as per line)
  // Each line has PixelsPerLine * nchans floats; nchans = 3
  stride.BytesPerPlaneIn  = (PixelsPerLine * 3) * sizeof(cmsFloat32Number);
  stride.BytesPerLineIn   = (PixelsPerLine * 3) * sizeof(cmsFloat32Number);
  stride.BytesPerPlaneOut = (PixelsPerLine * 3) * sizeof(cmsFloat32Number);
  stride.BytesPerLineOut  = (PixelsPerLine * 3) * sizeof(cmsFloat32Number);

  // Call focal method
  MatShaperFloat(&transform,
                inputFloats,
                outputFloats,
                PixelsPerLine,
                LineCount,
                &stride);

  // Validate: each output channel equals corresponding input value
  bool pass = true;
  for (cmsUInt32Number p = 0; p < PixelsPerLine; ++p) {
    for (cmsUInt32Number c = 0; c < 3; ++c) {
      float inVal = inputFloats[p*3 + c];
      float outVal = outputFloats[p*3 + c];
      if (!almost_equal(inVal, outVal)) {
        std::cerr << "Test RGB identity failed at pixel " << p << " channel " << c
                  << ": expected " << inVal << " got " << outVal << "\n";
        pass = false;
      }
    }
  }

  return pass;
}

// Test 2a: RGBA (4 channels) with COPY_ALPHA flag enabled; alpha should be copied
bool test_RGBA_identity_with_alpha_flag_on() {
  using namespace std;

  _cmstransform_struct transform;
  VXMatShaperFloatData shaper;
  cmsFloat32Number dummy = 0.0f;
  shaper.Shaper1R = &dummy;
  shaper.Shaper1G = &dummy;
  shaper.Shaper1B = &dummy;
  shaper.Shaper2R = &dummy;
  shaper.Shaper2G = &dummy;
  shaper.Shaper2B = &dummy;
  shaper.Mat[0][0] = 1.0f; shaper.Mat[0][1] = 0.0f; shaper.Mat[0][2] = 0.0f;
  shaper.Mat[1][0] = 0.0f; shaper.Mat[1][1] = 1.0f; shaper.Mat[1][2] = 0.0f;
  shaper.Mat[2][0] = 0.0f; shaper.Mat[2][1] = 0.0f; shaper.Mat[2][2] = 1.0f;
  shaper.UseOff = false;
  shaper.Off[0] = shaper.Off[1] = shaper.Off[2] = 0.0f;

  transform.UserData = &shaper;
  transform.Flags = cmsFLAGS_COPY_ALPHA; // enable COPY_ALPHA
  transform.InputFormat = 4;
  transform.OutputFormat = 4;

  const cmsUInt32Number PixelsPerLine = 4;
  const cmsUInt32Number LineCount = 1;
  cmsFloat32Number inputFloats[PixelsPerLine * 4] = {
    // Pixel0: R,G,B,A
    0.10f, 0.20f, 0.30f, 0.40f,
    // Pixel1
    0.50f, 0.60f, 0.70f, 0.80f,
    // Pixel2
    0.90f, 1.00f, 1.10f, 1.20f,
    // Pixel3
    1.30f, 1.40f, 1.50f, 1.60f
  };
  cmsFloat32Number outputFloats[PixelsPerLine * 4];
  for (int i = 0; i < PixelsPerLine * 4; ++i) outputFloats[i] = -1.0f;

  cmsStride stride;
  stride.BytesPerPlaneIn  = (PixelsPerLine * 4) * sizeof(cmsFloat32Number);
  stride.BytesPerLineIn   = (PixelsPerLine * 4) * sizeof(cmsFloat32Number);
  stride.BytesPerPlaneOut = (PixelsPerLine * 4) * sizeof(cmsFloat32Number);
  stride.BytesPerLineOut  = (PixelsPerLine * 4) * sizeof(cmsFloat32Number);

  MatShaperFloat(&transform,
                inputFloats,
                outputFloats,
                PixelsPerLine,
                LineCount,
                &stride);

  // Validate: RGB channels match input; Alpha should be copied
  bool pass = true;
  for (cmsUInt32Number p = 0; p < PixelsPerLine; ++p) {
    cmsUInt32Number baseIn = p*4;
    cmsUInt32Number baseOut = p*4;
    // R,G,B
    for (int c = 0; c < 3; ++c) {
      float inVal = inputFloats[baseIn + c];
      float outVal = outputFloats[baseOut + c];
      if (!almost_equal(inVal, outVal)) {
        std::cerr << "Test RGBA (flag on) failed at pixel " << p << " channel " << c
                  << ": expected " << inVal << " got " << outVal << "\n";
        pass = false;
      }
    }
    // Alpha should be copied
    float inA = inputFloats[baseIn + 3];
    float outA = outputFloats[baseOut + 3];
    if (!almost_equal(inA, outA)) {
      std::cerr << "Test RGBA (flag on) failed at pixel " << p
                << " alpha: expected " << inA << " got " << outA << "\n";
      pass = false;
    }
  }

  return pass;
}

// Test 2b: RGBA with COPY_ALPHA flag OFF; alpha channel should remain unchanged in output
bool test_RGBA_identity_with_alpha_flag_off() {
  using namespace std;

  _cmstransform_struct transform;
  VXMatShaperFloatData shaper;
  cmsFloat32Number dummy = 0.0f;
  shaper.Shaper1R = &dummy;
  shaper.Shaper1G = &dummy;
  shaper.Shaper1B = &dummy;
  shaper.Shaper2R = &dummy;
  shaper.Shaper2G = &dummy;
  shaper.Shaper2B = &dummy;
  shaper.Mat[0][0] = 1.0f; shaper.Mat[0][1] = 0.0f; shaper.Mat[0][2] = 0.0f;
  shaper.Mat[1][0] = 0.0f; shaper.Mat[1][1] = 1.0f; shaper.Mat[1][2] = 0.0f;
  shaper.Mat[2][0] = 0.0f; shaper.Mat[2][1] = 0.0f; shaper.Mat[2][2] = 1.0f;
  shaper.UseOff = false;
  shaper.Off[0] = shaper.Off[1] = shaper.Off[2] = 0.0f;

  transform.UserData = &shaper;
  transform.Flags = 0; // COPY_ALPHA not set
  transform.InputFormat = 4;
  transform.OutputFormat = 4;

  const cmsUInt32Number PixelsPerLine = 4;
  const cmsUInt32Number LineCount = 1;
  cmsFloat32Number inputFloats[PixelsPerLine * 4] = {
    // Pixel0: R,G,B,A
    0.10f, 0.20f, 0.30f, 0.40f,
    // Pixel1
    0.50f, 0.60f, 0.70f, 0.80f,
    // Pixel2
    0.90f, 1.00f, 1.10f, 1.20f,
    // Pixel3
    1.30f, 1.40f, 1.50f, 1.60f
  };
  cmsFloat32Number outputFloats[PixelsPerLine * 4];
  // Initialize output with a known value to detect no-change on alpha
  for (int i = 0; i < PixelsPerLine * 4; ++i) outputFloats[i] = 9.876f;

  cmsStride stride;
  stride.BytesPerPlaneIn  = (PixelsPerLine * 4) * sizeof(cmsFloat32Number);
  stride.BytesPerLineIn   = (PixelsPerLine * 4) * sizeof(cmsFloat32Number);
  stride.BytesPerPlaneOut = (PixelsPerLine * 4) * sizeof(cmsFloat32Number);
  stride.BytesPerLineOut  = (PixelsPerLine * 4) * sizeof(cmsFloat32Number);

  MatShaperFloat(&transform,
                inputFloats,
                outputFloats,
                PixelsPerLine,
                LineCount,
                &stride);

  // Validate: RGB channels copied; alpha remains unchanged (initial value)
  bool pass = true;
  for (cmsUInt32Number p = 0; p < PixelsPerLine; ++p) {
    cmsUInt32Number baseIn = p*4;
    cmsUInt32Number baseOut = p*4;
    // R,G,B
    for (int c = 0; c < 3; ++c) {
      float inVal = inputFloats[baseIn + c];
      float outVal = outputFloats[baseOut + c];
      if (!almost_equal(inVal, outVal)) {
        std::cerr << "Test RGBA (flag off) failed at pixel " << p << " channel " << c
                  << ": expected " << inVal << " got " << outVal << "\n";
        pass = false;
      }
    }
    // Alpha should remain as initially set (9.876)
    float outA = outputFloats[baseOut + 3];
    if (!almost_equal(outA, 9.876f)) {
      std::cerr << "Test RGBA (flag off) failed at pixel " << p
                << " alpha: expected unchanged " << 9.876f << " got " << outA << "\n";
      pass = false;
    }
  }

  return pass;
}

// Main runner: executes tests and reports results
int main() {
  int total = 0;
  int passed = 0;

  auto run = [&](const char* name, bool (*fn)()) {
    ++total;
    bool ok = false;
    try {
      ok = fn();
    } catch (...) {
      ok = false;
    }
    std::cout << (ok ? "[PASS] " : "[FAIL] ") << name << "\n";
    if (ok) ++passed;
  };

  run("RGB identity (3-channel, no alpha)", test_RGB_identity_no_alpha);
  run("RGBA identity with COPY_ALPHA flag ON (4-channel)", test_RGBA_identity_with_alpha_flag_on);
  run("RGBA identity with COPY_ALPHA flag OFF (4-channel) - alpha unchanged", test_RGBA_identity_with_alpha_flag_off);

  std::cout << "\nSummary: " << passed << " / " << total << " tests passed.\n";

  return (passed == total) ? 0 : 1;
}
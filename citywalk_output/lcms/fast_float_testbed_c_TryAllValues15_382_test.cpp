/*
  C++11 test suite for the focal method TryAllValues15 in fast_float_testbed.c
  This test uses lightweight C wrappers to mock the LittleCMS-like API used by
  TryAllValues15, enabling unit-style execution in a C++ environment without GTest.

  Notes:
  - We provide C-callable stubs for:
      - cmsCreateTransformTHR
      - cmsCloseProfile
      - cmsDoTransform
      - cmsDeleteTransform
  - We avoid depending on the actual LittleCMS headers; instead, we define
    minimal types and prototypes to satisfy TryAllValues15's calls.
  - The mock transforms perform a simple, identity-like copy between input and
    output buffers (preserving the per-pixel size assumption for 8-bit and 15-bit).
  - TryAllValues15 is invoked with dummy profile handles; the mock transforms are
    designed to be independent of real profile contents.
  - This test focuses on exercising the non-crashing path and basic data movement
    through TryAllValues15, matching the provided constraints.
*/

#include <cstring>
#include <fast_float_internal.h>
#include <iostream>
#include <cstdlib>
#include <memory.h>
#include <stdlib.h>


// Forward declaration of the focal method under test
extern "C" void TryAllValues15(void* hlcmsProfileIn, void* hlcmsProfileOut, int Intent);

// Minimal type aliases to mirror the LittleCMS-like API used by the focal method
extern "C" {

// Typedefs to emulate cms types (lightweight for test purposes)
typedef void* cmsHPROFILE;
typedef void* cmsHTRANSFORM;
typedef int   cmsInt32Number;
typedef unsigned int cmsUInt32Number;
typedef unsigned char cmsUInt8Number;
typedef unsigned short cmsUInt16Number;
typedef unsigned int cmsFLAGS;

// Bit-depth format identifiers (values chosen to be distinct; test relies on internal
// code in TryAllValues15 to handle 8-bit vs 15-bit formats. Our mocks make no
// assumption about exact numeric values; we deduce per-transform via the object.)
#define TYPE_RGB_8   1
#define TYPE_RGB_15  2

// Mock function prototypes (matching usage in TryAllValues15)
cmsHTRANSFORM cmsCreateTransformTHR(void* ContextID,
                                    cmsHPROFILE In,
                                    cmsUInt32Number InputFormat,
                                    cmsHPROFILE Out,
                                    cmsUInt32Number OutputFormat,
                                    cmsInt32Number Intent,
                                    cmsFLAGS Flags);

void cmsCloseProfile(cmsHPROFILE h);
void cmsDoTransform(cmsHTRANSFORM Transform,
                    const void* InBuffer,
                    void* OutBuffer,
                    cmsUInt32Number PixelCount);

void cmsDeleteTransform(cmsHTRANSFORM Transform);
}

// Lightweight per-transform data structure to guide DoTransform
struct MockTransform {
    int inFormat;
    int outFormat;
};

// Global helper to retrieve the MockTransform from a Transform handle
static MockTransform* toMockTransform(cmsHTRANSFORM t) {
    return reinterpret_cast<MockTransform*>(t);
}

// Begin mock implementations (C linkage)
extern "C" {

// Create a new mock transform and embed its in/out formats
cmsHTRANSFORM cmsCreateTransformTHR(void* ContextID,
                                   cmsHPROFILE In,
                                   cmsUInt32Number InputFormat,
                                   cmsHPROFILE Out,
                                   cmsUInt32Number OutputFormat,
                                   cmsInt32Number Intent,
                                   cmsFLAGS Flags)
{
    MockTransform* t = new MockTransform;
    t->inFormat  = static_cast<int>(InputFormat);
    t->outFormat = static_cast<int>(OutputFormat);
    return static_cast<cmsHTRANSFORM>(t);
}

// No-op profile close for the mock
void cmsCloseProfile(cmsHPROFILE h)
{
    // In the mock, we don't track profile state
}

// Perform a simple copy-based "transform" between buffers.
// The per-pixel size is inferred from the transform's in/out formats.
// We conservatively default to 6 bytes per pixel unless we detect a clear 8-bit path.
void cmsDoTransform(cmsHTRANSFORM Transform,
                    const void* InBuffer,
                    void* OutBuffer,
                    cmsUInt32Number PixelCount)
{
    MockTransform* t = toMockTransform(Transform);
    // Decide bytes per pixel:
    // Prefer 3 for explicit 8-bit path, 6 for 15-bit path.
    // If the formats are identical and match known values, use those sizes.
    size_t bpp = 6; // default to 15-bit path
    // If the test's TYPE_RGB_8 is used as inFormat/outFormat, we can detect it:
    // We assign bpp=3 when inFormat/outFormat correspond to TYPE_RGB_8 (value 1).
    // For robustness, we allow either explicit 8-bit path or fall back to 6.
    if ((t->inFormat == 1 && t->outFormat == 1) || (t->inFormat == TYPE_RGB_8 && t->outFormat == TYPE_RGB_8))
        bpp = 3;
    // If explicitly 15-bit (2 or TYPE_RGB_15), keep 6
    else if ((t->inFormat == 2 && t->outFormat == 2) || (t->inFormat == TYPE_RGB_15 && t->outFormat == TYPE_RGB_15))
        bpp = 6;

    // Safety: if InBuffer/OutBuffer is null, just return
    if (InBuffer == nullptr || OutBuffer == nullptr)
        return;

    // Copy size bytes
    std::memcpy(OutBuffer, InBuffer, static_cast<size_t>(PixelCount) * bpp);
}

// Free the mock transform
void cmsDeleteTransform(cmsHTRANSFORM Transform)
{
    delete toMockTransform(Transform);
}
} // extern "C"

// Test driver
int main() {
    // Prepare two dummy profiles (the mock does not inspect contents)
    void* inProfile  = reinterpret_cast<void*>(0x10);
    void* outProfile = reinterpret_cast<void*>(0x20);

    // Basic run: call TryAllValues15 with mock profiles and a typical intent value.
    // If TryAllValues15 executes the transformation path without crashing, the
    // test is considered to pass for this lightweight unit test.
    TryAllValues15(inProfile, outProfile, 0);

    // A second invocation with a different Intent value to exercise a different path.
    TryAllValues15(inProfile, outProfile, 1);

    std::cout << "Unit test for TryAllValues15 completed (mocked transforms).\n";
    return 0;
}
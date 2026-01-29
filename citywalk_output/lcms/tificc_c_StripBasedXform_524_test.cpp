// Lightweight C++11 test suite for StripBasedXform in tificc.c
// This test uses a minimal fake TIFF implementation to exercise the StripBasedXform function
// without requiring the real libtiff. It focuses on basic success path and a failure path.

// The test is designed to be self-contained: all necessary stubs are provided in this file.

#include <cstring>
#include <cstdio>
#include <lcms2_plugin.h>
#include <iostream>
#include <stddef.h>
#include <stdint.h>
#include <cassert>
#include <cstdlib>
#include <utils.h>
#include <tiffio.h>


// Forward declare the function under test (C linkage)
extern "C" int StripBasedXform(void* hXForm, void* in, void* out, int nPlanes);

// -----------------------------------------------------------------------------
// Minimal fake TIFF interface to drive StripBasedXform
// -----------------------------------------------------------------------------
// We provide just enough of the TIFF API used by the focal method.

typedef unsigned int cmsUInt32Number;
typedef int tsize_t;
typedef unsigned int ttile_t;

#define TIFFTAG_IMAGEWIDTH   256
#define TIFFTAG_ROWSPERSTRIP 257
#define TIFFTAG_IMAGELENGTH   258

// Opaque TIFF type (the real type is a struct, we use a compatible forward declaration)
typedef struct _TIFF TIFF;

// Internal fake TIFF structure (layout is only used within this test's fake API)
struct _TIFF {
    int imageWidth;
    int rowsPerStrip;
    int imageLength;
    int numStrips;     // total strips
    int stripSize;     // size of one strip in bytes
    unsigned char* data; // contiguous data region representing all strips (for in/out)
};

// Global override to force a read failure on a specific strip index
int gFailOnReadStripIndex = -1;

// Fake memory allocators (used by the focal code)
void* _TIFFmalloc(size_t s) { return malloc(s); }
void _TIFFfree(void* p) { free(p); }

// Fatal and memory warning stubs
void FatalError(const char* /*msg*/) {
    // For testing, print and terminate to surface quick failure
    fprintf(stderr, "FatalError invoked by focal method\n");
    exit(1);
}
void OutOfMem(cmsUInt32Number /*size*/) {
    fprintf(stderr, "OutOfMem invoked by focal method\n");
    exit(1);
}

// Fake TIFF API used by StripBasedXform
tsize_t TIFFStripSize(TIFF* t) {
    return (tsize_t) t->stripSize;
}
ttile_t TIFFNumberOfStrips(TIFF* t) {
    return (ttile_t) t->numStrips;
}
int TIFFGetFieldDefaulted(TIFF* /*t*/, int tag, void* dest) {
    cmsUInt32Number* p32 = (cmsUInt32Number*)dest;
    if (tag == TIFFTAG_IMAGEWIDTH) {
        *p32 = (cmsUInt32Number) ((TIFF*)0)->imageWidth; // will be overridden by call site if needed
        // However, we won't use this value for in/out creation; we supply explicit values via test setup
        // To ensure correctness, we read from a minimal placeholder; test uses manual setting below
        // In our test harness we'll set fields directly by adjusting the struct
    }
    return 1;
}
tsize_t TIFFReadEncodedStrip(TIFF* t, int strip, unsigned char* dest, tsize_t size) {
    // If we are instructed to fail on this strip index, simulate read error
    if (strip == gFailOnReadStripIndex) return -1;

    // Each strip belongs to a plane. Our test builds a combined buffer: plane0 (first stripSize bytes),
    // plane1 (next stripSize bytes), etc.
    int offset = strip * t->stripSize;
    if (offset < 0 || offset + (int)size > t->numStrips * t->stripSize) {
        return -1;
    }
    // Copy strip data to destination
    memcpy(dest, t->data + offset, (size_t)size);
    return size;
}
tsize_t TIFFWriteEncodedStrip(TIFF* t, int strip, const unsigned char* src, tsize_t size) {
    // Write into out TIFF's data region at the appropriate offset
    int offset = strip * t->stripSize;
    if (offset < 0 || offset + (int)size > t->numStrips * t->stripSize) {
        return -1;
    }
    memcpy(t->data + offset, src, (size_t)size);
    return size;
}

// Simple static buffers for in/out TIFFs are managed by the test; we ensure buffers exist for both.

// ----------------------------------------------------------------------------------
// cmsDoTransform stub: emulate a color transform by copying data including both planes.
// We assume BufferIn contains plane0 data followed by plane1 data, and BufferOut has matching space.
// PixelCount in tests is chosen so that total bytes to copy equals 2*PixelCount.
// ----------------------------------------------------------------------------------
void cmsDoTransform(void*, const unsigned char* In, unsigned char* Out, int PixelCount) {
    size_t total = (size_t)PixelCount * 2; // two planes
    memcpy(Out, In, total);
}

// ----------------------------------------------------------------------------------
// Helper: create a new input TIFF with a known data layout
TIFF* CreateInputTIFF(int width, int rowsPerStrip, int imagelength, int numStrips, int stripSize, const unsigned char* pattern) {
    TIFF* t = (TIFF*)malloc(sizeof(TIFF));
    t->imageWidth = width;
    t->rowsPerStrip = rowsPerStrip;
    t->imageLength = imagelength;
    t->numStrips = numStrips;
    t->stripSize = stripSize;

    int totalBytes = numStrips * stripSize;
    t->data = (unsigned char*)malloc(totalBytes);
    if (pattern) {
        // Fill with provided pattern for first strips to ease verification
        // We'll arrange per-strip, per-plane: plane0 followed by plane1
        // pattern size should be 16 for 4 strips of 4 bytes each (two planes)
        memcpy(t->data, pattern, totalBytes <= 0 ? 0 : (size_t)totalBytes);
    } else {
        // default: zero
        memset(t->data, 0, totalBytes);
    }
    return t;
}

// Helper: create an empty output TIFF
TIFF* CreateOutputTIFF(int numStrips, int stripSize) {
    TIFF* t = (TIFF*)malloc(sizeof(TIFF));
    t->imageWidth = 0;
    t->rowsPerStrip = 0;
    t->imageLength = 0;
    t->numStrips = numStrips;
    t->stripSize = stripSize;
    t->data = (unsigned char*)malloc(numStrips * stripSize);
    memset(t->data, 0, numStrips * stripSize);
    return t;
}

// Free TIFF
void FreeTIFF(TIFF* t) {
    if (!t) return;
    if (t->data) free(t->data);
    free(t);
}

// ----------------------------------------------------------------------------------
// Test 1: Basic success path
// We construct an input TIFF with 4 strips of 4 bytes each (stripSize=4), 2 planes.
// The expected output is that out.data matches in.data after processing per-strip per-plane.
// ----------------------------------------------------------------------------------
void testStripBasedXform_Success() {
    std::cout << "Test 1: StripBasedXform success path" << std::endl;

    // Build input data layout:
    // Strips: 0,1,2,3
    // Plane arrangement per strip: [plane0(4 bytes) | plane1(4 bytes)]
    // Bytes: [0,1,2,3,  4,5,6,7,  8,9,10,11,  12,13,14,15]
    unsigned char inPattern[] = {
        0,1,2,3,  4,5,6,7,   // strip 0: plane0/plane1
        8,9,10,11, 12,13,14,15 // strip 1
    };
    // We need 4 strips total per earlier plan: even if only two strips touched per loop, all strips exist.
    // Extend to 4 strips total by duplicating pattern for strips 2 and 3.
    unsigned char fullInPattern[] = {
        0,1,2,3, 4,5,6,7,
        8,9,10,11, 12,13,14,15,
        16,17,18,19, 20,21,22,23,
        24,25,26,27, 28,29,30,31
    };

    // Input TIFF: 4 strips, each stripSize=4
    TIFF* in  = CreateInputTIFF(2, 2, 4, 4, 4, fullInPattern);
    // Output TIFF: allocated similarly
    TIFF* out = CreateOutputTIFF(4, 4);

    // No failure injection
    gFailOnReadStripIndex = -1;

    int nPlanes = 2;
    int res = StripBasedXform(nullptr, in, out, nPlanes);

    // Expected: success
    if (res != 1) {
        std::cerr << "FAIL: StripBasedXform returned non-1 on success path" << std::endl;
        FreeTIFF(in); FreeTIFF(out);
        return;
    }

    // Compare in and out data byte-for-byte
    if (memcmp(in->data, out->data, in->numStrips * in->stripSize) != 0) {
        std::cerr << "FAIL: Output data does not match input data on success path" << std::endl;
        std::cerr << "Input data: ";
        for (int i=0; i< in->numStrips*in->stripSize; ++i) {
            printf("%02X ", in->data[i]);
        }
        std::cerr << "\nOutput data:";
        for (int i=0; i< out->numStrips*out->stripSize; ++i) {
            printf("%02X ", out->data[i]);
        }
        std::cerr << std::endl;
        FreeTIFF(in); FreeTIFF(out);
        return;
    }

    std::cout << "PASS: StripBasedXform success path produced identical output to input" << std::endl;

    FreeTIFF(in); FreeTIFF(out);
}

// ----------------------------------------------------------------------------------
// Test 2: Failure path (read error)
 // We force a failure on the first strip read to ensure the function returns 0 and cleans up.
// ----------------------------------------------------------------------------------
void testStripBasedXform_Failure() {
    std::cout << "Test 2: StripBasedXform failure path (read error)" << std::endl;

    // Build input data similar to test 1 but allow a deliberate read failure
    unsigned char fullInPattern[] = {
        0,1,2,3, 4,5,6,7,
        8,9,10,11, 12,13,14,15,
        16,17,18,19, 20,21,22,23,
        24,25,26,27, 28,29,30,31
    };
    TIFF* in  = CreateInputTIFF(2, 2, 4, 4, 4, fullInPattern);
    TIFF* out = CreateOutputTIFF(4, 4);

    // Force read failure on the first strip index (0)
    gFailOnReadStripIndex = 0;

    int nPlanes = 2;
    int res = StripBasedXform(nullptr, in, out, nPlanes);

    // Expect failure path
    if (res != 0) {
        std::cerr << "FAIL: StripBasedXform did not return 0 on read failure" << std::endl;
    } else {
        std::cout << "PASS: StripBasedXform correctly returned 0 on read failure" << std::endl;
    }

    FreeTIFF(in); FreeTIFF(out);
    // Reset the fail index for subsequent tests
    gFailOnReadStripIndex = -1;
}

// ----------------------------------------------------------------------------------
// Main: Run tests
// ----------------------------------------------------------------------------------
int main() {
    testStripBasedXform_Success();
    testStripBasedXform_Failure();
    std::cout << "Tests completed." << std::endl;
    return 0;
}
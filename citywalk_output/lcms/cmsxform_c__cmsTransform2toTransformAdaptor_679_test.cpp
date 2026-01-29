// cmsxform_test.cpp
// Minimal C++11 test suite for the internal CMS adaptor function:
// _cmsTransform2toTransformAdaptor
// This test uses a small, non-terminating assertion framework to
// verify basic behavior without depending on GoogleTest or GMock.

#include <cstring>
#include <iostream>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


//#include "lcms2_internal.h" // Optional: depends on build setup

// Forward declaration of the focal function (from cmsxform.c)
extern "C" void _cmsTransform2toTransformAdaptor(struct _cmstransform_struct *CMMcargo,
                                      const void* InputBuffer,
                                      void* OutputBuffer,
                                      cmsUInt32Number PixelsPerLine,
                                      cmsUInt32Number LineCount,
                                      const cmsStride* Stride);

// Minimal, non-terminating assertion helpers
static int gFailCount = 0;
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        ++gFailCount; \
        std::cerr << "EXPECT_TRUE failed: " #cond " at " << __FILE__ << ":" << __LINE__ << std::endl; \
    } \
} while(0)

#define EXPECT_EQ(a,b) do { \
    if(!((a) == (b))) { \
        ++gFailCount; \
        std::cerr << "EXPECT_EQ failed: " #a " = " << (a) << " != " #b " = " << (b) \
                  << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
    } \
} while(0)

#define EXPECT_NE(a,b) do { \
    if((a) == (b)) { \
        ++gFailCount; \
        std::cerr << "EXPECT_NE failed: " #a " == " << (a) << " but != " #b " == " << (b) \
                  << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
    } \
} while(0)

// Global fixtures for the test harness
static unsigned char g_InputBuffer[256];
static unsigned char g_OutputBuffer[256];
static const unsigned char* g_InputBufferStart = nullptr;
static cmsStride g_Stride;

// Counter to verify OldXform invocation sequence
static cmsUInt32Number g_LineCalled = 0;

// Fake OldXform implementation to observe adaptor behavior.
// This function will be injected via the CMM cargo's OldXform pointer.
static void FakeOldXform(struct _cmstransform_struct* CMMcargo,
                        const void* in,
                        void* out,
                        cmsUInt32Number PixelsPerLine,
                        cmsUInt32Number BytesPerPlaneIn)
{
    // Expect input pointer to be at the start of the current line: InputBuffer + LineCalled * BytesPerLineIn
    const unsigned char* expected = g_InputBufferStart + g_LineCalled * g_Stride.BytesPerLineIn;

    // Basic safety: ensure pointer arithmetic looks sane
    if (in != expected) {
        // Use a non-fatal failure: report mismatch but do not abort test
        ++gFailCount;
        std::cerr << "FakeOldXform: unexpected input pointer. Expected " 
                  << (void*)expected << " but got " << in << std::endl;
    }

    // Write a recognizable pattern to the output buffer for this line
    unsigned char* dst = (unsigned char*)out;
    for (cmsUInt32Number i = 0; i < PixelsPerLine; ++i) {
        dst[i] = static_cast<unsigned char>(g_LineCalled + 1); // line-indexed marker
    }

    // Move to next line for subsequent calls
    ++g_LineCalled;
}

// 1) Basic behavior test: two lines processed, OldXform invoked for each line with correct addresses.
//    The adaptor should loop LineCount times and advance input/output pointers by Stride
//    BytesPerLineIn/Out per line.
static void test_transform2toTransformAdaptor_basic_operation() {
    // Prepare a tiny input with two lines, 4 pixels per line
    const cmsUInt32Number PixelsPerLine = 4;
    const cmsUInt32Number LineCount = 2;

    // Line 0: 0,1,2,3 ; Line 1: 4,5,6,7
    for (cmsUInt32Number i = 0; i < PixelsPerLine * LineCount; ++i) {
        g_InputBuffer[i] = static_cast<unsigned char>(i);
    }
    std::memset(g_OutputBuffer, 0, sizeof(g_OutputBuffer));

    g_InputBufferStart = g_InputBuffer;
    g_Stride.BytesPerLineIn = PixelsPerLine;   // 4 bytes per line input
    g_Stride.BytesPerLineOut = PixelsPerLine;  // 4 bytes per line output
    g_Stride.BytesPerPlaneIn = PixelsPerLine;  // Per-line plane size used by OldXform

    g_LineCalled = 0;

    // Create a fake cargo with the OldXform hook
    struct _cmstransform_struct cargo;
    cargo.OldXform = FakeOldXform;

    // Call the focal function
    _cmsTransform2toTransformAdaptor(&cargo,
                                   g_InputBuffer,
                                   g_OutputBuffer,
                                   PixelsPerLine,
                                   LineCount,
                                   &g_Stride);

    // Verify: two lines processed
    EXPECT_EQ(g_LineCalled, LineCount);

    // Verify output pattern: line 0 filled with 1s, line 1 with 2s
    for (cmsUInt32Number line = 0; line < LineCount; ++line) {
        unsigned char expectedVal = static_cast<unsigned char>(line + 1);
        for (cmsUInt32Number p = 0; p < PixelsPerLine; ++p) {
            unsigned char actual = g_OutputBuffer[line * PixelsPerLine + p];
            EXPECT_EQ(actual, expectedVal);
        }
    }
}

// 2) Zero-line edge case: ensure no OldXform calls are made and buffers are untouched.
//    This exercises the loop boundary where LineCount == 0.
static void test_transform2toTransformAdaptor_zero_lines() {
    const cmsUInt32Number PixelsPerLine = 4;
    const cmsUInt32Number LineCount = 0;

    // Initialize input with some data; output should remain unchanged (or zeroed)
    for (cmsUInt32Number i = 0; i < 16; ++i) g_InputBuffer[i] = static_cast<unsigned char>(i);
    std::memset(g_OutputBuffer, 0xAA, sizeof(g_OutputBuffer)); // mark with a clear non-zero value

    g_InputBufferStart = g_InputBuffer;
    g_Stride.BytesPerLineIn = PixelsPerLine;
    g_Stride.BytesPerLineOut = PixelsPerLine;
    g_Stride.BytesPerPlaneIn = PixelsPerLine;

    g_LineCalled = 0;

    struct _cmstransform_struct cargo;
    cargo.OldXform = FakeOldXform;

    _cmsTransform2toTransformAdaptor(&cargo,
                                   g_InputBuffer,
                                   g_OutputBuffer,
                                   PixelsPerLine,
                                   LineCount,
                                   &g_Stride);

    // No lines processed; OldXform should not be called
    EXPECT_EQ(g_LineCalled, 0);

    // OutputBuffer should remain unchanged from its initial value
    for (size_t i = 0; i < 16; ++i) {
        EXPECT_EQ(g_OutputBuffer[i], 0xAA);
    }
}

// 3) Additional sanity check: ensure that multiple lines with varying strides still invoke OldXform properly.
//    This uses the same basic mechanism but a slightly different configuration.
static void test_transform2toTransformAdaptor_multiple_lines_with_different_stride() {
    const cmsUInt32Number PixelsPerLine = 3;
    const cmsUInt32Number LineCount = 3;

    // Prepare input: 3 lines, 3 pixels each
    for (cmsUInt32Number line = 0; line < LineCount; ++line) {
        for (cmsUInt32Number p = 0; p < PixelsPerLine; ++p) {
            g_InputBuffer[line * PixelsPerLine + p] = static_cast<unsigned char>(line * 10 + p);
        }
    }
    std::memset(g_OutputBuffer, 0, sizeof(g_OutputBuffer));

    g_InputBufferStart = g_InputBuffer;
    g_Stride.BytesPerLineIn = PixelsPerLine;    // 3 bytes per line input
    g_Stride.BytesPerLineOut = PixelsPerLine;   // 3 bytes per line output
    g_Stride.BytesPerPlaneIn = PixelsPerLine;   // 3 per plane in

    g_LineCalled = 0;

    struct _cmstransform_struct cargo;
    cargo.OldXform = FakeOldXform;

    _cmsTransform2toTransformAdaptor(&cargo,
                                   g_InputBuffer,
                                   g_OutputBuffer,
                                   PixelsPerLine,
                                   LineCount,
                                   &g_Stride);

    // Validate line invocation count
    EXPECT_EQ(g_LineCalled, LineCount);

    // Validate outputs per line: line index + 1 as before
    for (cmsUInt32Number line = 0; line < LineCount; ++line) {
        unsigned char expectedVal = static_cast<unsigned char>(line + 1);
        for (cmsUInt32Number p = 0; p < PixelsPerLine; ++p) {
            unsigned char actual = g_OutputBuffer[line * PixelsPerLine + p];
            EXPECT_EQ(actual, expectedVal);
        }
    }
}

// Entry point to run all tests
int main() {
    std::cout << "Starting cmsTransform adaptor unit tests (C++11 harness)..." << std::endl;

    test_transform2toTransformAdaptor_basic_operation();
    test_transform2toTransformAdaptor_zero_lines();
    test_transform2toTransformAdaptor_multiple_lines_with_different_stride();

    if (gFailCount == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << gFailCount << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}
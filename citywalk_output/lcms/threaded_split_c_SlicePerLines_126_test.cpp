// Unit test suite for the focal method SlicePerLines in threaded_split.c
// This test suite is designed for C++11, does not rely on GTest, and uses a
// small self-contained test harness with plain asserts and descriptive comments.
// The tests assume the existence of the following function exported from threaded_split.c:
// extern "C" void SlicePerLines(const _cmsWorkSlice* master, cmsInt32Number nslices,
//                               cmsInt32Number LinesPerSlice, _cmsWorkSlice slices[]);

// The test harness defines minimal type and struct definitions compatible with
// the function's expectations (as observed in the provided code).

#include <cstring>
#include <threaded_internal.h>
#include <cassert>
#include <iostream>


// Typedefs matching the CMS-like types used by the focal method
typedef int cmsInt32Number;
typedef unsigned char cmsUInt8Number;
typedef unsigned int cmsUInt32Number;

// Forward declare the function under test (C linkage)
extern "C" void SlicePerLines(const struct _cmsWorkSlice* master,
                              cmsInt32Number nslices,
                              cmsInt32Number LinesPerSlice,
                              struct _cmsWorkSlice slices[]);

// Minimal compatibility structs expected by SlicePerLines
struct _cmsStride {
    cmsUInt32Number BytesPerLineIn;
    cmsUInt32Number BytesPerLineOut;
};

struct _cmsWorkSlice {
    const cmsUInt8Number* InputBuffer;
    cmsUInt8Number* OutputBuffer;
    cmsInt32Number LineCount;
    _cmsStride* Stride;
};

// Simple test framework scaffolding
static int g_failures = 0;

// Helper to report a mismatch with a readable message
#define CHECK_EQ(actual, expected, msg) do { \
    if ((actual) != (expected)) { \
        std::cerr << "FAIL: " << (msg) \
                  << " | actual: " << (actual) \
                  << " != expected: " << (expected) << "\n"; \
        ++g_failures; \
    } \
} while (0)

#define CHECK_PTR_EQ(actual, expected, msg) do { \
    if ((actual) != (expected)) { \
        std::cerr << "FAIL: " << (msg) \
                  << " | actual: " << static_cast<const void*>(actual) \
                  << " != expected: " << static_cast<const void*>(expected) << "\n"; \
        ++g_failures; \
    } \
} while (0)

// Test 1: Basic distribution across multiple slices with exact distribution (no leftover)
void test_SlicePerLines_basic_distribution() {
    // Explanation:
    // master.LineCount = 7, LinesPerSlice = 3, nslices = 3
    // Expected per-slice lines: [3, 3, 1], with no leftover added because TotalLines becomes 0 after the loop.
    // Expected Input/Output pointers:
    //  - Slice 0:  InputBuffer + 0 * LinesPerSlice * BytesPerLineIn
    //  - Slice 1:  InputBuffer + 1 * LinesPerSlice * BytesPerLineIn
    //  - Slice 2:  InputBuffer + 2 * LinesPerSlice * BytesPerLineIn
    const cmsUInt32Number BytesPerLineIn  = 4;
    const cmsUInt32Number BytesPerLineOut = 5;

    _cmsStride stride;
    stride.BytesPerLineIn  = BytesPerLineIn;
    stride.BytesPerLineOut = BytesPerLineOut;

    // Allocate buffers for 7 lines
    const int TOTAL_LINES = 7;
    const int TOTAL_INPUT_BYTES  = TOTAL_LINES * BytesPerLineIn;
    const int TOTAL_OUTPUT_BYTES = TOTAL_LINES * BytesPerLineOut;

    cmsUInt8Number* inputBuf  = new cmsUInt8Number[TOTAL_INPUT_BYTES];
    cmsUInt8Number* outputBuf = new cmsUInt8Number[TOTAL_OUTPUT_BYTES];

    // Initialize buffers with deterministic values for traceability (line index is encoded)
    for (int line = 0; line < TOTAL_LINES; ++line) {
        for (int b = 0; b < (int)BytesPerLineIn; ++b) {
            inputBuf[line * BytesPerLineIn + b] = static_cast<cmsUInt8Number>((line + 1) * 10 + b);
        }
        for (int b = 0; b < (int)BytesPerLineOut; ++b) {
            outputBuf[line * BytesPerLineOut + b] = static_cast<cmsUInt8Number>((line + 1) * 20 + b);
        }
    }

    _cmsWorkSlice master;
    master.InputBuffer = inputBuf;
    master.OutputBuffer = outputBuf;
    master.LineCount    = TOTAL_LINES;
    master.Stride       = &stride;

    _cmsWorkSlice slices[3];
    // Call the function under test
    SlicePerLines(&master, 3, 3, slices);

    // Validate per-slice pointers and LineCount
    const cmsUInt8Number* baseIn  = master.InputBuffer;
    const cmsUInt8Number* baseOut = master.OutputBuffer;

    // Slice 0
    CHECK_PTR_EQ(slices[0].InputBuffer, baseIn + 0 * 3 * BytesPerLineIn, "Slice 0 InputBuffer offset");
    CHECK_PTR_EQ(slices[0].OutputBuffer, baseOut + 0 * 3 * BytesPerLineOut, "Slice 0 OutputBuffer offset");
    CHECK_EQ(slices[0].LineCount, 3, "Slice 0 LineCount");

    // Slice 1
    CHECK_PTR_EQ(slices[1].InputBuffer, baseIn + 1 * 3 * BytesPerLineIn, "Slice 1 InputBuffer offset");
    CHECK_PTR_EQ(slices[1].OutputBuffer, baseOut + 1 * 3 * BytesPerLineOut, "Slice 1 OutputBuffer offset");
    CHECK_EQ(slices[1].LineCount, 3, "Slice 1 LineCount");

    // Slice 2
    CHECK_PTR_EQ(slices[2].InputBuffer, baseIn + 2 * 3 * BytesPerLineIn, "Slice 2 InputBuffer offset");
    CHECK_PTR_EQ(slices[2].OutputBuffer, baseOut + 2 * 3 * BytesPerLineOut, "Slice 2 OutputBuffer offset");
    CHECK_EQ(slices[2].LineCount, 1, "Slice 2 LineCount (before leftover addition)");

    // After the loop, TotalLines should be 0, so last slice's LineCount remains 1
    // Additionally, Stride pointer should be preserved
    CHECK_PTR_EQ(slices[0].Stride, master.Stride, "Slice 0 Stride pointer equality");
    CHECK_PTR_EQ(slices[1].Stride, master.Stride, "Slice 1 Stride pointer equality");
    CHECK_PTR_EQ(slices[2].Stride, master.Stride, "Slice 2 Stride pointer equality");

    // Clean up
    delete[] inputBuf;
    delete[] outputBuf;
}

// Test 2: Case with leftover lines added to the last slice (nslices > 0, TotalLines not divisible)
void test_SlicePerLines_leftover_added_to_last_slice() {
    // Explanation:
    // master.LineCount = 10, LinesPerSlice = 3, nslices = 3
    // Distribution inside loop: [3, 3, 3], TotalLines left = 1
    // After loop: last slice LineCount += 1 => [3, 3, 4]
    const cmsUInt32Number BytesPerLineIn  = 4;
    const cmsUInt32Number BytesPerLineOut = 5;

    _cmsStride stride;
    stride.BytesPerLineIn  = BytesPerLineIn;
    stride.BytesPerLineOut = BytesPerLineOut;

    const int TOTAL_LINES = 10;

    cmsUInt8Number* inputBuf  = new cmsUInt8Number[TOTAL_LINES * BytesPerLineIn];
    cmsUInt8Number* outputBuf = new cmsUInt8Number[TOTAL_LINES * BytesPerLineOut];

    // Initialize buffers with deterministic values (not strictly validated here)
    for (int line = 0; line < TOTAL_LINES; ++line) {
        for (int b = 0; b < (int)BytesPerLineIn; ++b) {
            inputBuf[line * BytesPerLineIn + b] = static_cast<cmsUInt8Number>((line + 1) * 7 + b);
        }
        for (int b = 0; b < (int)BytesPerLineOut; ++b) {
            outputBuf[line * BytesPerLineOut + b] = static_cast<cmsUInt8Number>((line + 1) * 11 + b);
        }
    }

    _cmsWorkSlice master;
    master.InputBuffer = inputBuf;
    master.OutputBuffer = outputBuf;
    master.LineCount    = TOTAL_LINES;
    master.Stride       = &stride;

    _cmsWorkSlice slices[3];
    SlicePerLines(&master, 3, 3, slices);

    const cmsUInt8Number* baseIn  = master.InputBuffer;
    const cmsUInt8Number* baseOut = master.OutputBuffer;

    CHECK_PTR_EQ(slices[0].InputBuffer, baseIn + 0 * 3 * BytesPerLineIn, "Slice 0 InputBuffer offset (leftover test)");
    CHECK_PTR_EQ(slices[0].OutputBuffer, baseOut + 0 * 3 * BytesPerLineOut, "Slice 0 OutputBuffer offset (leftover test)");
    CHECK_EQ(slices[0].LineCount, 3, "Slice 0 LineCount (leftover test)");

    CHECK_PTR_EQ(slices[1].InputBuffer, baseIn + 1 * 3 * BytesPerLineIn, "Slice 1 InputBuffer offset (leftover test)");
    CHECK_PTR_EQ(slices[1].OutputBuffer, baseOut + 1 * 3 * BytesPerLineOut, "Slice 1 OutputBuffer offset (leftover test)");
    CHECK_EQ(slices[1].LineCount, 3, "Slice 1 LineCount (leftover test)");

    CHECK_PTR_EQ(slices[2].InputBuffer, baseIn + 2 * 3 * BytesPerLineIn, "Slice 2 InputBuffer offset (leftover test)");
    CHECK_PTR_EQ(slices[2].OutputBuffer, baseOut + 2 * 3 * BytesPerLineOut, "Slice 2 OutputBuffer offset (leftover test)");
    CHECK_EQ(slices[2].LineCount, 4, "Slice 2 LineCount after adding leftover");

    // Stride pointers should be preserved
    CHECK_PTR_EQ(slices[0].Stride, master.Stride, "Slice 0 Stride pointer equality (leftover test)");
    CHECK_PTR_EQ(slices[1].Stride, master.Stride, "Slice 1 Stride pointer equality (leftover test)");
    CHECK_PTR_EQ(slices[2].Stride, master.Stride, "Slice 2 Stride pointer equality (leftover test)");

    delete[] inputBuf;
    delete[] outputBuf;
}

// Test 3: nslices == 0 should exercise the false branch and leave slices untouched
void test_SlicePerLines_nslices_zero_no_changes() {
    const cmsUInt32Number BytesPerLineIn  = 4;
    const cmsUInt32Number BytesPerLineOut = 5;

    _cmsStride stride;
    stride.BytesPerLineIn  = BytesPerLineIn;
    stride.BytesPerLineOut = BytesPerLineOut;

    const int TOTAL_LINES = 6;
    cmsUInt8Number* inputBuf  = new cmsUInt8Number[TOTAL_LINES * BytesPerLineIn];
    cmsUInt8Number* outputBuf = new cmsUInt8Number[TOTAL_LINES * BytesPerLineOut];

    // Initialize with known pattern
    for (int i = 0; i < TOTAL_LINES * BytesPerLineIn; ++i) inputBuf[i] = static_cast<cmsUInt8Number>(i);
    for (int i = 0; i < TOTAL_LINES * BytesPerLineOut; ++i) outputBuf[i] = static_cast<cmsUInt8Number>(i + 100);

    _cmsWorkSlice master;
    master.InputBuffer = inputBuf;
    master.OutputBuffer = outputBuf;
    master.LineCount = TOTAL_LINES;
    master.Stride = &stride;

    _cmsWorkSlice slices[1];
    // Pre-fill slices[0] with sentinel values to detect any unintended modification
    slices[0].InputBuffer = nullptr;
    slices[0].OutputBuffer = nullptr;
    slices[0].LineCount = 1234;
    slices[0].Stride = &stride;

    SlicePerLines(&master, 0, 3, slices);

    // slices[0] should remain unchanged
    CHECK_EQ(slices[0].LineCount, 1234, "Slice 0 LineCount should be unchanged when nslices == 0");
    CHECK_PTR_EQ(slices[0].InputBuffer, (const cmsUInt8Number*)nullptr, "Slice 0 InputBuffer should remain unchanged (nullptr)");
    CHECK_PTR_EQ(slices[0].OutputBuffer, (cmsUInt8Number*)nullptr, "Slice 0 OutputBuffer should remain unchanged (nullptr)");
    CHECK_PTR_EQ(slices[0].Stride, master.Stride, "Slice 0 Stride pointer equality (nslices == 0)");

    delete[] inputBuf;
    delete[] outputBuf;
}

// Test 4: Sum of all slice LineCount equals master.LineCount (sanity check)
void test_SlicePerLines_linecount_sum_matches_master() {
    // Configuration chosen to exercise multiple slice counts
    const cmsUInt32Number BytesPerLineIn  = 2;
    const cmsUInt32Number BytesPerLineOut = 3;

    _cmsStride stride;
    stride.BytesPerLineIn  = BytesPerLineIn;
    stride.BytesPerLineOut = BytesPerLineOut;

    const int MASTER_LINES = 11;
    cmsUInt8Number* inputBuf  = new cmsUInt8Number[MASTER_LINES * BytesPerLineIn];
    cmsUInt8Number* outputBuf = new cmsUInt8Number[MASTER_LINES * BytesPerLineOut];

    for (int i = 0; i < MASTER_LINES * BytesPerLineIn; ++i) inputBuf[i] = static_cast<cmsUInt8Number>(i);
    for (int i = 0; i < MASTER_LINES * BytesPerLineOut; ++i) outputBuf[i] = static_cast<cmsUInt8Number>(i + 1);

    _cmsWorkSlice master;
    master.InputBuffer = inputBuf;
    master.OutputBuffer = outputBuf;
    master.LineCount = MASTER_LINES;
    master.Stride = &stride;

    _cmsWorkSlice slices[4];
    SlicePerLines(&master, 3, 4, slices); // 3 slices, 4 lines per slice

    // Sum of lines in slices should equal MASTER_LINES
    int sum = slices[0].LineCount + slices[1].LineCount + slices[2].LineCount;
    CHECK_EQ(sum, MASTER_LINES, "Sum of slice LineCount equals master LineCount");

    // Basic address checks for the first and last slice pointers
    const cmsUInt8Number* baseIn  = master.InputBuffer;
    const cmsUInt8Number* baseOut = master.OutputBuffer;

    CHECK_PTR_EQ(slices[0].InputBuffer, baseIn + 0 * 4 * BytesPerLineIn, "Slice 0 InputBuffer offset (sanity)");
    CHECK_PTR_EQ(slices[2].InputBuffer, baseIn + 2 * 4 * BytesPerLineIn, "Slice 2 InputBuffer offset (sanity)");

    delete[] inputBuf;
    delete[] outputBuf;
}

int main() {
    // Run all tests and report results
    test_SlicePerLines_basic_distribution();
    test_SlicePerLines_leftover_added_to_last_slice();
    test_SlicePerLines_nslices_zero_no_changes();
    test_SlicePerLines_linecount_sum_matches_master();

    if (g_failures == 0) {
        std::cout << "All SlicePerLines tests PASSED.\n";
        return 0;
    } else {
        std::cerr << g_failures << " SlicePerLines test(s) FAILED.\n";
        return 1;
    }
}
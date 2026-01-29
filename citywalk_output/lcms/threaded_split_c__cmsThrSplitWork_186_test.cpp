// High-quality C++11 unit test suite for cmsBool _cmsThrSplitWork in threaded_split.c
// This test suite is framework-free (no Google Test) and uses a lightweight
// in-house assertion mechanism that does not terminate on failure, to maximize coverage.
// It focuses on key branches inside _cmsThrSplitWork and relies on the project's
// existing dependencies for SlicePerPixels and SlicePerLines behavior.

#include <cstdio>
#include <vector>
#include <threaded_internal.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cmath>


// Include the project dependencies that define _cmsWorkSlice, cmsBool, cmsInt32Number, etc.
// The exact path depends on the build system; adjust if necessary.

// Ensure C linkage for the focal function to avoid name mangling when linking with C code
extern "C" {
    cmsBool _cmsThrSplitWork(const _cmsWorkSlice* master, cmsInt32Number nslices, _cmsWorkSlice slices[]);
}

// Lightweight test framework (non-terminating assertions)
static int g_total_tests = 0;
static int g_passed_tests = 0;

static void expectTrue(bool cond, const std::string& msg) {
    ++g_total_tests;
    if (cond) {
        ++g_passed_tests;
    } else {
        std::cerr << "TEST FAIL: " << msg << std::endl;
    }
}

static void expectFalse(bool cond, const std::string& msg) {
    ++g_total_tests;
    if (!cond) {
        ++g_passed_tests;
    } else {
        std::cerr << "TEST FAIL: " << msg << std::endl;
    }
}

// Helper to set up a master _cmsWorkSlice with a given configuration
static _cmsWorkSlice makeMasterSlice(cmsInt32Number pixelsPerLine,
                                     cmsInt32Number lineCount,
                                     cmsUInt32Number inBytesPerLine,
                                     cmsUInt32Number outBytesPerLine) {
    // Stride struct (BytesPerLineIn/BytesPerLineOut)
    // The lifetime of 'stride' must extend until _cmsThrSplitWork is called.
    static cmsStride strideStatic; // Use static here to ensure availability in short tests
    cmsStride* stride = &strideStatic;
    stride->BytesPerLineIn  = inBytesPerLine;
    stride->BytesPerLineOut = outBytesPerLine;

    _cmsWorkSlice master;
    master.PixelsPerLine = pixelsPerLine;
    master.LineCount      = lineCount;
    master.Stride         = stride;

    // Note: _cmsWorkSlice has other members in the real codebase; we only set what we need.
    return master;
}

// Test 1: Returns FALSE when PixelsPerLine == 0
static void test_PixelsPerLineZero_ReturnsFalse() {
    // Setup: zero PixelsPerLine, non-zero stride values
    _cmsWorkSlice master = makeMasterSlice(0, 2, 8, 8);
    _cmsWorkSlice slices[4] = {};
    cmsBool res = _cmsThrSplitWork(&master, 4, slices);
    expectFalse(res, "PixelsPerLine == 0 should cause FALSE return");
}

// Test 2: Returns FALSE when Stride.BytesPerLineIn == 0
static void test_BytesPerLineInZero_ReturnsFalse() {
    _cmsWorkSlice master = makeMasterSlice(8, 2, 0, 8);
    _cmsWorkSlice slices[4] = {};
    cmsBool res = _cmsThrSplitWork(&master, 4, slices);
    expectFalse(res, "BytesPerLineIn == 0 should cause FALSE return");
}

// Test 3: Returns FALSE when Stride.BytesPerLineOut == 0
static void test_BytesPerLineOutZero_ReturnsFalse() {
    _cmsWorkSlice master = makeMasterSlice(8, 2, 8, 0);
    _cmsWorkSlice slices[4] = {};
    cmsBool res = _cmsThrSplitWork(&master, 4, slices);
    expectFalse(res, "BytesPerLineOut == 0 should cause FALSE return");
}

// Test 4: LineCount <= 1 and PixelsPerLine sufficient to split (SlicePerPixels path)
static void test_LineCountOne_PixelsPerLine_Sufficient_ReturnsTrue() {
    // LineCount <= 1, e.g., 1 line; require PixelsPerLine/nslices > 0
    const cmsInt32Number nslices = 4;
    _cmsWorkSlice master = makeMasterSlice(8, 1, 8, 8); // 8 / 4 = 2 > 0
    _cmsWorkSlice slices[4] = {};
    cmsBool res = _cmsThrSplitWork(&master, nslices, slices);
    expectTrue(res, "LineCount <= 1 with sufficient PixelsPerLine should return TRUE");
}

// Test 5: LineCount <= 1 and PixelsPerLine insufficient (0 per worker)
static void test_LineCountOne_PixelsPerLine_Insufficient_ReturnsFalse() {
    const cmsInt32Number nslices = 4;
    _cmsWorkSlice master = makeMasterSlice(3, 1, 8, 8); // 3 / 4 = 0
    _cmsWorkSlice slices[4] = {};
    cmsBool res = _cmsThrSplitWork(&master, nslices, slices);
    expectFalse(res, "LineCount <= 1 with insufficient PixelsPerLine should return FALSE");
}

// Test 6: LineCount > 1 and LinesPerWorker sufficient (SlicePerLines path)
static void test_LineCountMore_LinesPerWorkerSufficient_ReturnsTrue() {
    const cmsInt32Number nslices = 4;
    _cmsWorkSlice master = makeMasterSlice(8, 4, 8, 8); // 4 / 4 = 1 > 0
    _cmsWorkSlice slices[4] = {};
    cmsBool res = _cmsThrSplitWork(&master, nslices, slices);
    expectTrue(res, "LineCount > 1 with LinesPerWorker >= 1 should return TRUE");
}

// Test 7: LineCount > 1 and LinesPerWorker zero (SlicePerLines path should fail)
static void test_LineCountMore_LinesPerWorkerZero_ReturnsFalse() {
    const cmsInt32Number nslices = 4;
    _cmsWorkSlice master = makeMasterSlice(8, 3, 8, 8); // 3 / 4 = 0
    _cmsWorkSlice slices[4] = {};
    cmsBool res = _cmsThrSplitWork(&master, nslices, slices);
    expectFalse(res, "LineCount > 1 with LinesPerWorker == 0 should return FALSE");
}

// Run all tests and report summary
static void runAllTests() {
    std::cout << "Starting cmsThrSplitWork test suite (no GTest)..." << std::endl;

    test_PixelsPerLineZero_ReturnsFalse();
    test_BytesPerLineInZero_ReturnsFalse();
    test_BytesPerLineOutZero_ReturnsFalse();
    test_LineCountOne_PixelsPerLine_Sufficient_ReturnsTrue();
    test_LineCountOne_PixelsPerLine_Insufficient_ReturnsFalse();
    test_LineCountMore_LinesPerWorkerSufficient_ReturnsTrue();
    test_LineCountMore_LinesPerWorkerZero_ReturnsFalse();

    std::cout << "Tests completed. Total: " << g_total_tests
              << ", Passed: " << g_passed_tests << std::endl;
}

int main() {
    // Execute tests
    runAllTests();
    // Return non-zero if there were failures
    return (g_total_tests == g_passed_tests) ? 0 : 1;
}
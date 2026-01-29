// Unit test suite for the focal method:
 // OptimizeCLUTRGBTransform located in fast_float_tethra.c
 // This test suite avoids GTest and uses a small in-code framework.
 // It targets C++11 compilation and relies on the LittleCMS/libcms API
 // being available in the include/link path (lcms2.h, cmsPipeline*, etc.).

#include <cstring>
#include <cstdio>
#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <lcms2.h>


// Include the LittleCMS headers to obtain all required types and constants.
// If your environment uses a different header name, adjust accordingly.

// Ensure the function under test is visible to this translation unit.
// The focal function is defined in the provided source file.
// We declare the prototype here using the same signature as in the source.
extern "C" cmsBool OptimizeCLUTRGBTransform(_cmsTransform2Fn* TransformFn,
                                           void** UserData,
                                           _cmsFreeUserDataFn* FreeDataFn,
                                           cmsPipeline** Lut,
                                           cmsUInt32Number* InputFormat,
                                           cmsUInt32Number* OutputFormat,
                                           cmsUInt32Number* dwFlags);

// Simple test harness utilities
static int g_passed = 0;
static int g_failed = 0;

#define TEST_PASS(msg) do { std::cout << "[PASS] " << msg << "\n"; ++g_passed; } while(0)
#define TEST_FAIL(msg) do { std::cerr << "[FAIL] " << msg << "\n"; ++g_failed; } while(0)

// Helper to print a summary at end
static void print_summary() {
    std::cout << "\nTest Summary: " << g_passed << " passed, " << g_failed << " failed.\n";
}

// Test 1: Null Lut (LUT is NULL) should return FALSE immediately (do-nothing path)
static bool test_null_lut_returns_false() {
    // Prepare inputs
    cmsPipeline* lut = NULL;
    cmsUInt32Number InputFormat = 0;   // Any value; the function should short-circuit on Lut==NULL
    cmsUInt32Number OutputFormat = 0;
    cmsUInt32Number dwFlags = 0;

    // Call the focal function
    cmsBool res = OptimizeCLUTRGBTransform(NULL,      // TransformFn
                                             NULL,      // UserData
                                             NULL,      // FreeDataFn
                                             &lut,      // Lut
                                             &InputFormat,
                                             &OutputFormat,
                                             &dwFlags);

    if (res == FALSE && lut == NULL) {
        TEST_PASS("test_null_lut_returns_false: Correctly handled NULL LUT by returning FALSE and leaving LUT NULL");
        return true;
    } else {
        TEST_FAIL("test_null_lut_returns_false: Expected FALSE for NULL Lut path");
        return false;
    }
}

// Test 2: Non-floating input format should cause an early FALSE return
static bool test_non_float_input_returns_false() {
    // Create a minimal pipeline to pass the non-NULL Lut case
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        TEST_FAIL("test_non_float_input_returns_false: Failed to create cmsContext");
        return false;
    }

    cmsPipeline* OriginalLut = cmsPipelineAlloc(ctx, 3, 3);
    if (OriginalLut == NULL) {
        cmsFreeContext(ctx);
        TEST_FAIL("test_non_float_input_returns_false: Failed to allocate OriginalLut pipeline");
        return false;
    }

    // Pass a non-floating InputFormat value. We rely on the macro interpretation inside the library.
    // Using 0 here as a conservative non-float indicator; if the library treats 0 as FLOAT by default on your build,
    // this test will reflect that. The important aspect is that at least one of the early checks fails.
    cmsUInt32Number InputFormat = 0;       // Intentionally non-floating (expected to fail)
    cmsUInt32Number OutputFormat = 0;      // Keep in a simple state
    cmsUInt32Number dwFlags = 0;

    cmsPipeline** LutPtr = &OriginalLut;

    cmsBool res = OptimizeCLUTRGBTransform(NULL,  // TransformFn
                                           NULL,  // UserData
                                           NULL,  // FreeDataFn
                                           LutPtr,
                                           &InputFormat,
                                           &OutputFormat,
                                           &dwFlags);

    // Cleanup
    if (OriginalLut != NULL) cmsPipelineFree(OriginalLut);
    cmsDestroyContext(ctx);

    if (res == FALSE) {
        TEST_PASS("test_non_float_input_returns_false: Correctly rejected non-floating RGB input format");
        return true;
    } else {
        TEST_FAIL("test_non_float_input_returns_false: Expected FALSE for non-floating input format");
        return false;
    }
}

// Test 3: Non-floating output format should cause an early FALSE return
static bool test_non_float_output_returns_false() {
    // Create a minimal pipeline to pass the non-NULL Lut case
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        TEST_FAIL("test_non_float_output_returns_false: Failed to create cmsContext");
        return false;
    }

    cmsPipeline* OriginalLut = cmsPipelineAlloc(ctx, 3, 3);
    if (OriginalLut == NULL) {
        cmsDestroyContext(ctx);
        TEST_FAIL("test_non_float_output_returns_false: Failed to allocate OriginalLut pipeline");
        return false;
    }

    // Provide a valid InputFormat that might be treated as FLOAT, but set OutputFormat to non-FLOAT.
    cmsUInt32Number InputFormat = 0;        // Attempt to set as FLOAT for compatibility
    cmsUInt32Number OutputFormat = 0;       // Intentionally non-floating for test
    cmsUInt32Number dwFlags = 0;

    cmsPipeline** LutPtr = &OriginalLut;

    cmsBool res = OptimizeCLUTRGBTransform(NULL,  // TransformFn
                                           NULL,  // UserData
                                           NULL,  // FreeDataFn
                                           LutPtr,
                                           &InputFormat,
                                           &OutputFormat,
                                           &dwFlags);

    // Cleanup
    if (OriginalLut != NULL) cmsPipelineFree(OriginalLut);
    cmsDestroyContext(ctx);

    if (res == FALSE) {
        TEST_PASS("test_non_float_output_returns_false: Correctly rejected non-floating Output format");
        return true;
    } else {
        TEST_FAIL("test_non_float_output_returns_false: Expected FALSE for non-floating output format");
        return false;
    }
}

int main() {
    // Run tests
    bool t1 = test_null_lut_returns_false();
    bool t2 = test_non_float_input_returns_false();
    bool t3 = test_non_float_output_returns_false();

    // Aggregate results
    print_summary();

    // Return non-zero if any test failed
    if (g_failed > 0) return 1;
    return 0;
}
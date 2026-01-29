/*
Unit test suite for the focal method EmitIntent in cmsps2.c

Overview:
- We test the exact string emitted by EmitIntent for each RenderingIntent value.
- Since the real project relies on the lcMS library, we provide a minimal C++11 test harness
  that mocks the necessary IO function (_cmsIOPrintf) and the cmsIOHANDLER type.
- We declare the EmitIntent function with C linkage so it can be linked against cmsps2.c.
- We cover:
  - INTENT_PERCEPTUAL -> "Perceptual"
  - INTENT_RELATIVE_COLORIMETRIC -> "RelativeColorimetric"
  - INTENT_ABSOLUTE_COLORIMETRIC -> "AbsoluteColorimetric"
  - INTENT_SATURATION -> "Saturation"
  - Undefined/other values -> "Undefined"
  - Optional: multiple sequential calls on the same handler to verify accumulation

Notes:
- This test does not use GTest. It implements simple, non-terminating assertions via helper macros
  and runs from main() as required by the task constraints.
- The test defines a minimal cmsIOHANDLER type and a tiny _cmsIOPrintf implementation that captures
  the emitted output per handler instance for assertion.
- We rely on the conventional integer values for the rendering intents (0..3) which are
  typically defined as:
  INTENT_PERCEPTUAL = 0, INTENT_RELATIVE_COLORIMETRIC = 1,
  INTENT_ABSOLUTE_COLORIMETRIC = 2, INTENT_SATURATION = 3.
  For the undefined case we pass a value like 9999.

Compilation guidance (not part of the code output):
- Build with g++ -std=c++11 -c test_emitintent.cpp
  and link with the cmsps2.c object file in the same executable.
- Ensure cmsps2.c is compiled with the proper include path to pull in the actual declarations
  for the function prototype EmitIntent (extern "C" linkage in test).

The code below should be placed in a single file, e.g., test_emitintent.cpp.
*/

#include <unordered_map>
#include <cstdio>
#include <iostream>
#include <string>
#include <cstdarg>
#include <lcms2_internal.h>


// Forward declarations to match the focal function signature in cmsps2.c
typedef unsigned int cmsUInt32Number;
typedef struct _cmsIOHANDLER cmsIOHANDLER;

// The focal function is defined in cmsps2.c and linked during build.
// We declare it with C linkage to avoid name mangling when linking from C++.
extern "C" void EmitIntent(cmsIOHANDLER* m, cmsUInt32Number RenderingIntent);

// Minimal mock of _cmsIOPrintf used by EmitIntent.
// It captures output per provided cmsIOHANDLER instance for later assertions.
extern "C" void _cmsIOPrintf(cmsIOHANDLER* m, const char* format, ...);

// Simple in-test rendering intent constants.
// We use concrete values to feed EmitIntent; the real library defines these similarly.
const cmsUInt32Number INTENT_PERCEPTUAL = 0;
const cmsUInt32Number INTENT_RELATIVE_COLORIMETRIC = 1;
const cmsUInt32Number INTENT_ABSOLUTE_COLORIMETRIC = 2;
const cmsUInt32Number INTENT_SATURATION = 3;

// Define a minimal cmsIOHANDLER type (contents are irrelevant for our test).
struct _cmsIOHANDLER {
    int dummy; // no real use; exists to satisfy the type
};

// Global map to capture emitted text per handler instance.
static std::unordered_map<void*, std::string> g_output;

// Implementation of the mocked _cmsIOPrintf.
// It formats the provided string and appends to the per-handler captured output.
extern "C" void _cmsIOPrintf(cmsIOHANDLER* m, const char* format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[4096];
    std::vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    g_output[(void*)m] += buffer;
}

// Simple test framework (non-terminating) to compare expected vs actual outputs.
static int g_failures = 0;

static void assert_eq(const std::string& expected, const std::string& actual, const std::string& testName) {
    if (expected == actual) {
        std::cout << "[PASS] " << testName << "\n";
    } else {
        std::cerr << "[FAIL] " << testName << "\n";
        std::cerr << "  Expected: \"" << expected << "\"\n";
        std::cerr << "  Actual:   \"" << actual << "\"\n";
        ++g_failures;
    }
}

// Utility to reset captured output for a given handler.
static void reset_output(cmsIOHANDLER* m) {
    g_output[(void*)m].clear();
}

// Test 1: INTENT_PERCEPTUAL should emit "Perceptual"
static void test_emitintent_perceptual() {
    // Arrange
    cmsIOHANDLER m;
    reset_output(&m);
    // Act
    EmitIntent(&m, INTENT_PERCEPTUAL);
    // Assert
    std::string got = g_output[(void*)&m];
    std::string expected = "/RenderingIntent (Perceptual)\n";
    assert_eq(expected, got, "EmitIntent -> Perceptual");
}

// Test 2: INTENT_RELATIVE_COLORIMETRIC should emit "RelativeColorimetric"
static void test_emitintent_relative_colorimetric() {
    cmsIOHANDLER m;
    reset_output(&m);
    EmitIntent(&m, INTENT_RELATIVE_COLORIMETRIC);
    std::string got = g_output[(void*)&m];
    std::string expected = "/RenderingIntent (RelativeColorimetric)\n";
    assert_eq(expected, got, "EmitIntent -> RelativeColorimetric");
}

// Test 3: INTENT_ABSOLUTE_COLORIMETRIC should emit "AbsoluteColorimetric"
static void test_emitintent_absolute_colorimetric() {
    cmsIOHANDLER m;
    reset_output(&m);
    EmitIntent(&m, INTENT_ABSOLUTE_COLORIMETRIC);
    std::string got = g_output[(void*)&m];
    std::string expected = "/RenderingIntent (AbsoluteColorimetric)\n";
    assert_eq(expected, got, "EmitIntent -> AbsoluteColorimetric");
}

// Test 4: INTENT_SATURATION should emit "Saturation"
static void test_emitintent_saturation() {
    cmsIOHANDLER m;
    reset_output(&m);
    EmitIntent(&m, INTENT_SATURATION);
    std::string got = g_output[(void*)&m];
    std::string expected = "/RenderingIntent (Saturation)\n";
    assert_eq(expected, got, "EmitIntent -> Saturation");
}

// Test 5: Undefined/other value should emit "Undefined"
static void test_emitintent_undefined() {
    cmsIOHANDLER m;
    reset_output(&m);
    // Use a value outside the known range to trigger the default case
    EmitIntent(&m, 9999);
    std::string got = g_output[(void*)&m];
    std::string expected = "/RenderingIntent (Undefined)\n";
    assert_eq(expected, got, "EmitIntent -> Undefined");
}

// Test 6: Multiple sequential calls on the same handler should accumulate outputs
static void test_emitintent_multiple_calls_accumulate() {
    cmsIOHANDLER m;
    reset_output(&m);
    EmitIntent(&m, INTENT_PERCEPTUAL);
    EmitIntent(&m, INTENT_SATURATION);
    std::string got = g_output[(void*)&m];
    std::string expected = "/RenderingIntent (Perceptual)\n/RenderingIntent (Saturation)\n";
    assert_eq(expected, got, "EmitIntent -> accumulation across calls");
}

// Main: run all tests
int main() {
    std::cout << "Running EmitIntent test suite...\n";

    test_emitintent_perceptual();
    test_emitintent_relative_colorimetric();
    test_emitintent_absolute_colorimetric();
    test_emitintent_saturation();
    test_emitintent_undefined();
    test_emitintent_multiple_calls_accumulate();

    if (g_failures == 0) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cerr << g_failures << " test(s) FAILED.\n";
        return 1;
    }
}
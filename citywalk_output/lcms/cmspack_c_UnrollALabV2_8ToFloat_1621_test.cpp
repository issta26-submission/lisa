// Lightweight unit test suite for UnrollALabV2_8ToFloat
// This test suite is designed to be used without GoogleTest.
// It relies on the project's existing cmspack.c implementation and its
// dependent types. The test harness uses non-terminating assertions to
// maximize code execution coverage.

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>
#include <cstdint>


// Domain-aligned minimal type aliases to enable compilation when linking
// with the actual CMS library types in the project environment.
typedef unsigned char cmsUInt8Number;
typedef unsigned int  cmsUInt32Number;
typedef unsigned short cmsUInt16Number;
typedef float cmsFloat32Number;
struct _cmsTRANSFORM; // forward declaration
typedef struct _cmsTRANSFORM _cmsTRANSFORM;

// Forward declaration of the focal method (C linkage)
extern "C" cmsUInt8Number* UnrollALabV2_8ToFloat(_cmsTRANSFORM* info,
                                               cmsFloat32Number wIn[],
                                               cmsUInt8Number* accum,
                                               cmsUInt32Number Stride);

// Simple non-terminating test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Helper to print test result and continue
static void print_result(const char* test_name, bool passed) {
    ++g_total_tests;
    if (passed) {
        std::cout << "[PASS] " << test_name << std::endl;
    } else {
        ++g_failed_tests;
        std::cout << "[FAIL] " << test_name << std::endl;
    }
}

// Helper: compare two pointers (for readability)
static bool ptrEq(const void* a, const void* b) {
    return a == b;
}

// Test 1: Pointer advancement test
// Rationale: UnrollALabV2_8ToFloat performs four accum increments
// (A, L, a, b) and returns the updated accum. This test ensures that
// the function advances the accumulator correctly (accum + 4).
static void test_pointer_advances_by_four() {
    // Domain note: We only verify pointer arithmetic, not internal lab conversions.
    cmsUInt8Number buffer[16];
    for (int i = 0; i < 16; ++i) buffer[i] = static_cast<cmsUInt8Number>(i);

    cmsUInt8Number* accum = buffer;
    cmsFloat32Number wIn[3] = {0.0f, 0.0f, 0.0f};
    cmsUInt32Number Stride = 0;

    cmsUInt8Number* ret = UnrollALabV2_8ToFloat(nullptr, wIn, accum, Stride);

    bool passed = (ret == buffer + 4);
    print_result("test_pointer_advances_by_four", passed);
}

// Test 2: wIn remains unchanged (Lab-to-float conversion is a no-op in this test harness slice)
// Rationale: In the provided focal code, lab4toFloat is called but (in this test context)
// its body appears to be a no-op. This test asserts that wIn is not modified by the function.
static void test_wIn_remains_unchanged() {
    cmsUInt8Number buffer[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    cmsUInt8Number* accum = buffer;
    cmsFloat32Number wIn[3] = {1.25f, -3.5f, 7.0f};
    cmsUInt32Number Stride = 0;

    cmsUInt8Number* ret = UnrollALabV2_8ToFloat(nullptr, wIn, accum, Stride);

    bool unchanged = (wIn[0] == 1.25f) && (wIn[1] == -3.5f) && (wIn[2] == 7.0f);
    bool passed = unchanged && (ret == buffer + 4);
    print_result("test_wIn_remains_unchanged", passed);
}

// Test 3: Sequential calls preserve correct accumulation and allow multiple invocations
// Rationale: After the first call, subsequent calls should continue consuming the next 4 bytes
// from the buffer. This test validates that behavior.
static void test_sequential_calls_consume_four_each_time() {
    cmsUInt8Number buffer[32];
    // Fill with a known pattern for easy traceability
    for (int i = 0; i < 32; ++i) buffer[i] = static_cast<cmsUInt8Number>(0x10 + i);

    cmsUInt8Number* accum = buffer;
    cmsFloat32Number wIn[3] = {0.0f, 0.0f, 0.0f};
    cmsUInt32Number Stride = 0;

    // First call
    cmsUInt8Number* ret1 = UnrollALabV2_8ToFloat(nullptr, wIn, accum, Stride);
    // Second call uses the updated accum pointer
    cmsUInt8Number* ret2 = UnrollALabV2_8ToFloat(nullptr, wIn, ret1, Stride);

    bool firstAdv = (ret1 == buffer + 4);
    bool secondAdv = (ret2 == buffer + 8);

    bool passed = firstAdv && secondAdv;
    print_result("test_sequential_calls_consume_four_each_time", passed);
}

// Main test runner
int main() {
    // Run tests
    test_pointer_advances_by_four();
    test_wIn_remains_unchanged();
    test_sequential_calls_consume_four_each_time();

    // Summary
    std::cout << "Total tests: " << g_total_tests
              << ", Failures: " << g_failed_tests
              << std::endl;

    // Return non-zero if any test failed
    return g_failed_tests != 0 ? 1 : 0;
}
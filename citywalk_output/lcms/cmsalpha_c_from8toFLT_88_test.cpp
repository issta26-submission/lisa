/*
Candidate Keywords extracted from the focal method and its surrounding context:
- from8toFLT: conversion routine
- dst, src: pointer arguments
- cmsFloat32Number, cmsUInt8Number: CMS-specific numeric types
- division by 255.0f: normalization from 8-bit to float
- memory reinterpretation via casts
This test-suite targets from8toFLT(void* dst, const void* src) ensuring correct 0..255 -> [0.0f..1.0f] mapping.
*/

#include <iostream>
#include <iomanip>
#include <cmath>
#include <lcms2_internal.h>
#include <cstdint>


// Include CMS internal header to pick up type definitions (cmsFloat32Number, cmsUInt8Number, etc.)

// Declare the focal function with C linkage for the C function defined in cmsalpha.c
extern "C" void from8toFLT(void* dst, const void* src);

/*
Domain knowledge notes applied:
- Tests should be executable without GoogleTest; a lightweight in-house harness is used.
- Use non-terminating checks; failures are logged but do not abort test execution.
- Validate both boundary cases (0 and 255) and a mid-range value (128).
- Test with a multi-byte source to ensure only the first byte is read.
- All tests compiled under C++11; rely only on standard library + CMS headers/types.
*/

static int g_failures = 0;

static inline bool almost_equal_float(float a, float b, float tol = 1e-6f) {
    return std::fabs(a - b) <= tol;
}

static void log_pass(const char* name) {
    std::cout << "[PASS] " << name << std::endl;
}

static void log_fail(const char* name, const char* reason) {
    std::cout << "[FAIL] " << name << " - " << reason << std::endl;
    ++g_failures;
}

/*
Test 1: From 0 -> 0.0f
Verifies that src = 0 maps to dst = 0.0f exactly.
*/
void test_from8toFLT_zero() {
    const char testName[] = "from8toFLT_zero";
    cmsUInt8Number src = 0;
    cmsFloat32Number dst = 0.0f;

    from8toFLT(&dst, &src);

    if (dst == 0.0f) {
        log_pass(testName);
    } else {
        log_fail(testName, "Expected dst == 0.0f for src = 0");
    }
}

/*
Test 2: From 255 -> 1.0f
Verifies that src = 255 maps to dst = 1.0f exactly.
*/
void test_from8toFLT_max() {
    const char testName[] = "from8toFLT_max";
    cmsUInt8Number src = 255;
    cmsFloat32Number dst = 0.0f;

    from8toFLT(&dst, &src);

    if (dst == 1.0f) {
        log_pass(testName);
    } else {
        log_fail(testName, "Expected dst == 1.0f for src = 255");
    }
}

/*
Test 3: From 128 -> 128/255.0f
Verifies that a mid-range value is correctly scaled to [0.0f, 1.0f].
Uses a tolerance since 128/255 is not exactly representable in binary float.
*/
void test_from8toFLT_mid() {
    const char testName[] = "from8toFLT_mid";
    cmsUInt8Number src = 128;
    cmsFloat32Number dst = 0.0f;
    const float expected = 128.0f / 255.0f;

    from8toFLT(&dst, &src);

    if (almost_equal_float(dst, expected)) {
        log_pass(testName);
    } else {
        std::ostringstream oss;
        oss << "Expected dst ~ " << std::setprecision(9) << expected
            << ", got " << dst;
        log_fail(testName, oss.str().c_str());
    }
}

/*
Test 4: Source with extra bytes should not affect first byte reading
Ensures that only the first byte is read even if a longer source buffer is provided.
*/
void test_from8toFLT_extra_bytes() {
    const char testName[] = "from8toFLT_extra_bytes";
    uint8_t srcBuffer[4] = { 10, 99, 0, 0 }; // First byte is 10; remaining bytes are arbitrary
    cmsFloat32Number dst = 0.0f;
    const float expected = 10.0f / 255.0f;

    from8toFLT(&dst, srcBuffer);

    if (almost_equal_float(dst, expected)) {
        log_pass(testName);
    } else {
        std::ostringstream oss;
        oss << "Expected dst ~ " << std::setprecision(9) << expected
            << ", got " << dst;
        log_fail(testName, oss.str().c_str());
    }
}

/*
Test 5: Boundary check with another value to ensure consistent behavior
*/
void test_from8toFLT_another_value() {
    const char testName[] = "from8toFLT_another_value";
    cmsUInt8Number src = 1;
    cmsFloat32Number dst = 0.0f;
    const float expected = 1.0f / 255.0f;

    from8toFLT(&dst, &src);

    if (almost_equal_float(dst, expected)) {
        log_pass(testName);
    } else {
        std::ostringstream oss;
        oss << "Expected dst ~ " << std::setprecision(9) << expected
            << ", got " << dst;
        log_fail(testName, oss.str().c_str());
    }
}

int main() {
    std::cout << "Starting tests for from8toFLT(void* dst, const void* src)" << std::endl;

    // Run all tests
    test_from8toFLT_zero();
    test_from8toFLT_max();
    test_from8toFLT_mid();
    test_from8toFLT_extra_bytes();
    test_from8toFLT_another_value();

    // Summary
    std::cout << "Test suite completed. Failures: " << g_failures << std::endl;
    // Return non-zero if any test failed
    return g_failures;
}
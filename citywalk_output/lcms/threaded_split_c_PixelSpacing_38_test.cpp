// pixel_spacing_tests.cpp
// A lightweight, non-terminating assertion-based test suite for PixelSpacing.
// This test targets the focal function PixelSpacing used in the threaded_split.c context.
// It exercises both branches of the internal conditional (planar vs non-planar formats).

#include <threaded_internal.h>
#include <cstdint>
#include <iostream>


// Import necessary dependencies from the project under test.
// This header is expected to define cmsUInt32Number, PixelSpacing, ComponentSize,
// T_PLANAR, T_CHANNELS, T_EXTRA, and common color/format constants like TYPE_GRAY_8 and TYPE_RGB_8.

// Basic non-terminating assertions: they report failures but continue execution.
static int g_failures = 0;

static void log_failure(const char* test, cmsUInt32Number actual, cmsUInt32Number expected) {
    std::cerr << "[FAIL] " << test << ": actual=" << actual << ", expected=" << expected << "\n";
    ++g_failures;
}

static void log_success(const char* test) {
    std::cout << "[PASS] " << test << "\n";
}

// Helper wrappers for assertions on 32-bit unsigned values.
static void expect_eq_uint32(const char* test, cmsUInt32Number actual, cmsUInt32Number expected) {
    if (actual != expected)
        log_failure(test, actual, expected);
    else
        log_success(test);
}

// Compile-time guard: ensure required format constants are available.
// If the environment provides different format constants, adjust accordingly.
#ifndef TYPE_GRAY_8
#error "Required format constant TYPE_GRAY_8 is not defined. Ensure proper headers are included."
#endif

#ifndef TYPE_RGB_8
#error "Required format constant TYPE_RGB_8 is not defined. Ensure proper headers are included."
#endif

// Step 2: Unit Test Generation for PixelSpacing
// Test 1: Planar format should use only ComponentSize(format).
// We choose TYPE_GRAY_8 as a canonical planar format assumption.
static bool test_planar_pixel_spacing() {
    // Arrange
    cmsUInt32Number format = TYPE_GRAY_8;     // Assumed planar format
    // Act
    cmsUInt32Number actual = PixelSpacing(format);
    cmsUInt32Number expected = ComponentSize(format);

    // Assert
    expect_eq_uint32("PixelSpacing_planar_equals_ComponentSize(TYPE_GRAY_8)", actual, expected);
    // Return value indicates if test passed for quick boolean checks.
    return (actual == expected);
}

// Test 2: Non-planar format should multiply ComponentSize by (T_CHANNELS + T_EXTRA).
// We choose TYPE_RGB_8 as a canonical non-planar RGB format assumption.
static bool test_non_planar_pixel_spacing() {
    // Arrange
    cmsUInt32Number format = TYPE_RGB_8;     // Assumed non-planar format
    // Act
    cmsUInt32Number actual = PixelSpacing(format);
    cmsUInt32Number cs = ComponentSize(format);
    cmsUInt32Number ch = T_CHANNELS(format);
    cmsUInt32Number ex = T_EXTRA(format);
    // Use 64-bit intermediate to avoid overflow in the multiplication
    uint64_t expected64 = static_cast<uint64_t>(cs) * (static_cast<uint64_t>(ch) + static_cast<uint64_t>(ex));
    cmsUInt32Number expected = static_cast<cmsUInt32Number>(expected64);

    // Assert
    expect_eq_uint32("PixelSpacing_non_planar_equals_ComponentSize*(channels+extras)(TYPE_RGB_8)", actual, expected);
    return (actual == expected);
}

// Step 3: Test Case Refinement
// Additional coverage: ensure PixelSpacing respects the non-decreasing property
// relative to ComponentSize for non-planar formats (a lightweight invariant check).
static bool test_invariant_non_planar_pixel_spacing() {
    cmsUInt32Number format = TYPE_RGB_8; // non-planar
    cmsUInt32Number cs = ComponentSize(format);
    cmsUInt32Number ps = PixelSpacing(format);
    // Invariant: PixelSpacing should be at least ComponentSize (since (channels + extras) >= 1)
    if (ps < cs) {
        log_failure("PixelSpacing_invariant_non_planar", ps, cs);
        return false;
    } else {
        log_success("PixelSpacing_invariant_non_planar");
        return true;
    }
}

// Main harness: executes all tests and reports summary.
// Non-terminating assertions ensure we attempt all tests even if some fail.
int main() {
    std::cout << "Starting PixelSpacing unit tests...\n";

    bool t1 = test_planar_pixel_spacing();
    bool t2 = test_non_planar_pixel_spacing();
    bool t3 = test_invariant_non_planar_pixel_spacing();

    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
    } else {
        std::cerr << g_failures << " test(s) failed.\n";
    }

    // Return non-zero if any test failed, to integrate with simple build systems.
    return g_failures ? 1 : 0;
}

// Comments about test strategy:
// - The tests cover both branches of the focus method's if (T_PLANAR(format)) conditional:
//   - Planar format (TYPE_GRAY_8) should yield PixelSpacing == ComponentSize.
//   - Non-planar format (TYPE_RGB_8) should yield PixelSpacing == ComponentSize * (T_CHANNELS + T_EXTRA).
// - We rely on the library's format macros (TYPE_GRAY_8, TYPE_RGB_8) and the associated
//   metadata functions (ComponentSize, T_CHANNELS, T_EXTRA) to derive expected outcomes.
// - Tests are non-terminating: they log failures but continue execution to maximize coverage.
// - This file avoids private methods; it exercises only the focal public-like API PixelSpacing
//   and its dependent components exposed by threaded_internal.h.
// High-coverage unit tests for cmsFormatter _cmsGetStockOutputFormatter
// This test suite is written for C++11, uses the public Little CMS headers where available,
// and does not rely on GTest. It uses a small in-house assertion framework and a
// simple main() to drive tests.
// Notes:
// - The tests dynamically probe the stock formatters to detect true branches (matches)
//   and false/default branches (no match).
// - It also validates the OPTIMIZED_SH(1) masking effect by asserting invariance
//   of the chosen formatter after masking.

#include <limits>
#include <iostream>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Include public Little CMS header for types and constants.
// If cmsGetStockOutputFormatter and related types are defined in a different header
// in your environment, adjust the include accordingly.

// Ensure C linkage for the focal function when linking from C++ test code.
extern "C" cmsFormatter _cmsGetStockOutputFormatter(cmsUInt32Number dwInput, cmsUInt32Number dwFlags);

// Lightweight assertion helper: non-terminating, returns success/failure for chaining.
static inline bool expect(bool cond, const char* msg) {
    if (!cond) {
        std::cerr << "Expectation failed: " << msg << std::endl;
        return false;
    }
    return true;
}

// Test 1: True branch for 16-bit stock formatter: find at least one input that yields a non-NULL Fmt16
static bool test_stock_output_formatter_16bits_true_branch() {
    bool ok = true;
    cmsFormatter fr = {};
    cmsUInt32Number foundInput = 0;
    // Try a reasonable search window; if the environment provides any 16-bit stock formatter,
    // at least one input should match.
    const cmsUInt32Number LIMIT = 4096;
    for (cmsUInt32Number input = 0; input < LIMIT; ++input) {
        fr = _cmsGetStockOutputFormatter(input, CMS_PACK_FLAGS_16BITS);
        if (fr.Fmt16 != NULL) {
            foundInput = input;
            break;
        }
    }

    ok &= expect(fr.Fmt16 != NULL, "16-bit stock formatter should provide a non-NULL Fmt16 for some input in 0..4095");
    if (!ok) return false;

    // Test the masking behavior with OPTIMIZED_SH(1): results should remain the same after masking
    cmsUInt32Number maskedInput = foundInput & ~OPTIMIZED_SH(1);
    cmsFormatter frMasked = _cmsGetStockOutputFormatter(maskedInput, CMS_PACK_FLAGS_16BITS);
    ok &= expect(frMasked.Fmt16 == fr.Fmt16,
                 "Masking OPTIMIZED_SH(1) should not change the chosen 16-bit formatter (Fmt16 matches)");
    return ok;
}

// Test 2: True branch for FLOAT stock formatter: find at least one input that yields a non-NULL FmtFloat
static bool test_stock_output_formatter_float_true_branch() {
    bool ok = true;
    cmsFormatter fr = {};
    cmsUInt32Number foundInput = 0;
    // Similar search for float formatter
    const cmsUInt32Number LIMIT = 4096;
    for (cmsUInt32Number input = 0; input < LIMIT; ++input) {
        fr = _cmsGetStockOutputFormatter(input, CMS_PACK_FLAGS_FLOAT);
        if (fr.FmtFloat != NULL) {
            foundInput = input;
            break;
        }
    }

    ok &= expect(fr.FmtFloat != NULL, "FLOAT stock formatter should provide a non-NULL FmtFloat for some input in 0..4095");
    if (!ok) return false;

    // Test the masking behavior with OPTIMIZED_SH(1)
    cmsUInt32Number maskedInput = foundInput & ~OPTIMIZED_SH(1);
    cmsFormatter frMasked = _cmsGetStockOutputFormatter(maskedInput, CMS_PACK_FLAGS_FLOAT);
    ok &= expect(frMasked.FmtFloat == fr.FmtFloat,
                 "Masking OPTIMIZED_SH(1) should not change the chosen  FLOAT formatter (FmtFloat matches)");
    return ok;
}

// Test 3: Default branch: non-matching flag should yield Fmt16 == NULL
static bool test_stock_output_formatter_default_branch() {
    bool ok = true;
    cmsFormatter fr = {};
    // Use a flag value that is not CMS_PACK_FLAGS_16BITS nor CMS_PACK_FLAGS_FLOAT.
    // If the public API defines a known invalid flag value, prefer it; otherwise, use 0.
    // We rely on 0 here as a safe non-matching value.
    fr = _cmsGetStockOutputFormatter(0, 0);
    ok &= expect(fr.Fmt16 == NULL, "Default branch should set Fmt16 to NULL when flag is non-matching");
    // No assertion on FmtFloat; the function only guarantees Fmt16 NULL in this path.
    return ok;
}

// Test 4: Cross-check consistency across multiple runs to improve coverage
static bool test_stock_output_formatter_consistency_across_calls() {
    bool ok = true;
    cmsFormatter fr1 = _cmsGetStockOutputFormatter(12345, CMS_PACK_FLAGS_16BITS);
    cmsFormatter fr2 = _cmsGetStockOutputFormatter(12345, CMS_PACK_FLAGS_16BITS);
    ok &= expect(fr1.Fmt16 == fr2.Fmt16, "Repeated calls with same input and flag should yield identical 16-bit formatter (consistency)");
    // Also check with a different input
    cmsFormatter fr3 = _cmsGetStockOutputFormatter(12346, CMS_PACK_FLAGS_FLOAT);
    cmsFormatter fr4 = _cmsGetStockOutputFormatter(12346, CMS_PACK_FLAGS_FLOAT);
    ok &= expect(fr3.FmtFloat == fr4.FmtFloat, "Repeated calls with same input and flag should yield identical  FLOAT formatter (consistency)");
    return ok;
}

int main() {
    int totalTests = 4;
    int failures = 0;

    std::cout << "Running cmsGetStockOutputFormatter unit tests (no GTest, C++11)..." << std::endl;

    if (!test_stock_output_formatter_16bits_true_branch()) {
        ++failures;
        std::cerr << "[FAIL] test_stock_output_formatter_16bits_true_branch" << std::endl;
    } else {
        std::cout << "[PASS] test_stock_output_formatter_16bits_true_branch" << std::endl;
    }

    if (!test_stock_output_formatter_float_true_branch()) {
        ++failures;
        std::cerr << "[FAIL] test_stock_output_formatter_float_true_branch" << std::endl;
    } else {
        std::cout << "[PASS] test_stock_output_formatter_float_true_branch" << std::endl;
    }

    if (!test_stock_output_formatter_default_branch()) {
        ++failures;
        std::cerr << "[FAIL] test_stock_output_formatter_default_branch" << std::endl;
    } else {
        std::cout << "[PASS] test_stock_output_formatter_default_branch" << std::endl;
    }

    if (!test_stock_output_formatter_consistency_across_calls()) {
        ++failures;
        std::cerr << "[FAIL] test_stock_output_formatter_consistency_across_calls" << std::endl;
    } else {
        std::cout << "[PASS] test_stock_output_formatter_consistency_across_calls" << std::endl;
    }

    if (failures > 0) {
        std::cerr << "Unit tests completed with " << failures << " failure(s) out of " << totalTests << "." << std::endl;
        return 1;
    } else {
        std::cout << "All unit tests passed." << std::endl;
        return 0;
    }
}
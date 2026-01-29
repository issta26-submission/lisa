// C++11 test suite for QuantizeToSector (cmssm.c)
// Note: This test uses a direct extern C declaration to the focal function.
// It assumes SECTORS is defined within the original cmssm.c (commonly 8 in this project).
// The test provides a minimal cmsSpherical structure mirroring the used fields.

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>


// Provide a C-compatible prototype for the focal function.
extern "C" {
    void QuantizeToSector(const struct cmsSpherical* sp, int* alpha, int* theta);
}

// Minimal replication of the cmsSpherical structure used by the function.
// This must be compatible with the actual definition in the library.
struct cmsSpherical {
    double alpha; // degrees
    double theta; // degrees
};

// Helper macro-like inline function for test assertions
static int g_totalTests = 0;
static int g_failedTests = 0;

static void check_eq_int(int got, int expected, const char* testDesc) {
    ++g_totalTests;
    if (got == expected) {
        std::cout << "[OK] " << testDesc << " (got " << got << ")" << std::endl;
    } else {
        std::cout << "[FAIL] " << testDesc << " (expected " << expected << ", got " << got << ")" << std::endl;
        ++g_failedTests;
    }
}

int main() {
    // Test 1: Zero angles should map to the first sector (alpha=0, theta=0)
    // This validates the lower-bound behavior of both quantization steps.
    {
        cmsSpherical sp = {0.0, 0.0};
        int a = 0, t = 0;
        QuantizeToSector(&sp, &a, &t);
        check_eq_int(a, 0, "QuantizeToSector: alpha == 0 at sp.alpha = 0");
        check_eq_int(t, 0, "QuantizeToSector: theta == 0 at sp.theta = 0");
    }

    // Test 2: Angles at the upper bound (360 for alpha, 0 for theta)
    // Expected: alpha should clamp to SECTORS-1; theta remains 0.
    // We assume SECTORS == 8 based on common usage in this project.
    {
        const int SECTORS = 8;
        cmsSpherical sp = {360.0, 0.0};
        int a = 0, t = 0;
        QuantizeToSector(&sp, &a, &t);
        check_eq_int(a, SECTORS - 1, "QuantizeToSector: alpha clamps to SECTORS-1 for sp.alpha = 360");
        check_eq_int(t, 0, "QuantizeToSector: theta remains 0 for sp.theta = 0");
    }

    // Test 3: Mid-range angles expecting a non-clamped, exact floor result
    // With SECTORS=8, alpha = floor(180 * 8 / 360) = floor(4) = 4
    // theta = floor(90 * 8 / 180) = floor(4) = 4
    {
        cmsSpherical sp = {180.0, 90.0};
        int a = 0, t = 0;
        QuantizeToSector(&sp, &a, &t);
        check_eq_int(a, 4, "QuantizeToSector: alpha == 4 for sp.alpha = 180");
        check_eq_int(t, 4, "QuantizeToSector: theta == 4 for sp.theta = 90");
    }

    // Test 4: Negative angles should yield negative quantized indices (no clamping on negative values)
    // With SECTORS=8: alpha = floor((-10) * 8 / 360) = floor(-0.222...) = -1
    // theta = floor((-90) * 8 / 180) = floor(-4) = -4
    {
        cmsSpherical sp = {-10.0, -90.0};
        int a = 0, t = 0;
        QuantizeToSector(&sp, &a, &t);
        check_eq_int(a, -1, "QuantizeToSector: alpha == -1 for sp.alpha = -10");
        check_eq_int(t, -4, "QuantizeToSector: theta == -4 for sp.theta = -90");
    }

    // Test 5: Near-boundary alpha just below 360 should map to SECTORS-1
    // For SECTORS=8: floor((359 * 8) / 360) = floor(7.977...) = 7
    {
        cmsSpherical sp = {359.0, 0.0};
        int a = 0, t = 0;
        QuantizeToSector(&sp, &a, &t);
        check_eq_int(a, 7, "QuantizeToSector: alpha == 7 for sp.alpha = 359");
        check_eq_int(t, 0, "QuantizeToSector: theta == 0 for sp.theta = 0");
    }

    // Test 6: Theta at upper bound 360 should clamp to SECTORS-1
    // For SECTORS=8: theta = floor((360 * 8) / 180) = floor(16) = 16 -> clamp to 7
    {
        const int SECTORS = 8;
        cmsSpherical sp = {0.0, 360.0};
        int a = 0, t = 0;
        QuantizeToSector(&sp, &a, &t);
        check_eq_int(t, SECTORS - 1, "QuantizeToSector: theta clamps to SECTORS-1 for sp.theta = 360");
        check_eq_int(a, 0, "QuantizeToSector: alpha remains 0 for sp.alpha = 0");
    }

    // Summary
    std::cout << "Total tests run: " << g_totalTests << std::endl;
    if (g_failedTests == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cout << "Tests FAILED: " << g_failedTests << std::endl;
        return 1;
    }
}
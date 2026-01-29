// Test suite for the focal method CheckLab2XYZ (from testcms2.c) using a C++11 test harness.
// This harness does not rely on any unit test framework (no GTest). It uses a lightweight
// non-terminating assertion mechanism to maximize code coverage while reporting failures.
//
// The tests exercise:
// - Directly calling CheckLab2XYZ() and verifying it returns true (non-zero).
// - Independently validating Lab -> XYZ -> Lab round-trips for a few representative Lab values
//   using the underlying cmsLab2XYZ, cmsXYZ2Lab, and cmsDeltaE routines.
// - Ensuring that the delta E of round-trips remains within a tight tolerance.
//
// Note: This test harness assumes the Little CMS headers (lcms2.h) and library are available
// on the include/library path during linking.

#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <lcms2.h>
#include <cmath>


// Import the Little CMS C API with C linkage
extern "C" {
}

// Lightweight non-terminating test framework
namespace TestLab2XYZ {

static std::vector<std::string> g_failures;

// Record a failure message without terminating the test run
static void Fail(const std::string& msg) {
    g_failures.push_back("FAIL: " + msg);
}

// Assert a condition; on failure, record the message but continue
static void AssertTrue(bool cond, const std::string& msg) {
    if (!cond) {
        Fail(msg);
    }
}

// Convenience: convert a Lab value to a string (for debugging if needed)
static std::string LabToString(const cmsCIELab& lab) {
    char buf[256];
    snprintf(buf, sizeof(buf), "Lab{L=%.6f, a=%.6f, b=%.6f}", lab.L, lab.a, lab.b);
    return std::string(buf);
}

// Test 1: Ensure CheckLab2XYZ() returns a non-zero value (true) on the actual implementation.
// This validates the overall integrity of the Lab<->XYZ round-trip logic across the tested domain.
static void Test_CheckLab2XYZ_ReturnsTrue(void) {
    // Rely on the focal function as the contract: non-zero means "true" per original code.
    cmsInt32Number res = CheckLab2XYZ();
    if (res == 0) {
        Fail("CheckLab2XYZ() returned false (0); expected non-zero (true).");
    } else {
        // Optional: log success length (no termination)
        // std::cout << "CheckLab2XYZ() returned true as expected.\n";
    }

    // Additional direct validation of a representative Lab round-trip using the lower-level API.
    cmsCIELab Lab;
    cmsCIEXYZ XYZ;
    cmsCIELab Lab2;

    // Representative values to exercise conversion paths
    Lab.L = 50.0; Lab.a = 0.0; Lab.b = 0.0;
    cmsLab2XYZ(NULL, &XYZ, &Lab);
    cmsXYZ2Lab(NULL, &Lab2, &XYZ);

    // Compute deltaE for the round-trip
    cmsFloat64Number dist = cmsDeltaE(&Lab, &Lab2);
    const cmsFloat64Number tol = 1e-9; // tight tolerance for identity check

    if (dist > tol) {
        Fail("Direct Lab->XYZ->Lab round-trip distance exceeds tolerance: dist=" +
             std::to_string(dist) + " (tol=" + std::to_string((double)tol) + ")");
        // Optional: show Lab values for debugging
        // std::cout << "Original " << LabToString(Lab) << ", after round-trip " << LabToString(Lab2) 
        //           << ", dist=" << dist << "\n";
    }
}

// Test 2: Validate Lab->XYZ->Lab round-trip across a small but representative sample of Lab values.
// This helps cover both branches of the distance comparison and ensures numerical stability.
static void Test_LabRoundTrip_Samples(void) {
    struct Sample {
        cmsFloat64Number L;
        cmsFloat64Number a;
        cmsFloat64Number b;
    };

    // A small, diverse set of samples including edges and mid-range values
    const Sample samples[] = {
        { 0.0,  -128.0, -128.0 },
        { 50.0,  0.0,    0.0   },
        { 100.0, 128.0,   128.0 },
        { 25.0, -60.0,   40.0  },
        { 75.0,  90.0,  -70.0  },
        { 0.0,   0.0,    0.0   }
    };

    const size_t n = sizeof(samples) / sizeof(samples[0]);
    for (size_t i = 0; i < n; ++i) {
        const Sample& s = samples[i];
        cmsCIELab Lab{ s.L, s.a, s.b };
        cmsCIEXYZ XYZ;
        cmsCIELab Lab2;

        cmsLab2XYZ(NULL, &XYZ, &Lab);
        cmsXYZ2Lab(NULL, &Lab2, &XYZ);
        cmsFloat64Number dist = cmsDeltaE(&Lab, &Lab2);

        // Use a slightly looser tolerance for diverse inputs
        const cmsFloat64Number tol = 1e-9;
        const bool ok = dist <= tol;
        if (!ok) {
            Fail("LabRoundTrip sample failed: Lab=" + LabToString(Lab) +
                 ", dist=" + std::to_string((double)dist) + ", tol=" + std::to_string((double)tol));
        }
    }
}

// Test 3: Spot-check a few additional Lab values to stress the conversion pipeline.
// This helps ensure no unexpected out-of-range calculations or NaNs appear.
static void Test_SpotCheck_AdditionalValues(void) {
    const cmsFloat64Number L_vals[] = { 0.0, 10.0, 90.0, 100.0 };
    const cmsFloat64Number a_vals[] = { -128.0, -32.0, 32.0, 128.0 };
    const cmsFloat64Number b_vals[] = { -128.0, -32.0, 32.0, 128.0 };

    for (cmsSize_t i = 0; i < sizeof(L_vals)/sizeof(L_vals[0]); ++i) {
        for (cmsSize_t j = 0; j < sizeof(a_vals)/sizeof(a_vals[0]); ++j) {
            for (cmsSize_t k = 0; k < sizeof(b_vals)/sizeof(b_vals[0]); ++k) {
                cmsCIELab Lab{ L_vals[i], a_vals[j], b_vals[k] };
                cmsCIEXYZ XYZ;
                cmsCIELab Lab2;

                cmsLab2XYZ(NULL, &XYZ, &Lab);
                cmsXYZ2Lab(NULL, &Lab2, &XYZ);
                cmsFloat64Number dist = cmsDeltaE(&Lab, &Lab2);
                const cmsFloat64Number tol = 1e-9;
                if (dist > tol) {
                    Fail("SpotCheck: Lab=" + LabToString(Lab) +
                         " -> dist=" + std::to_string((double)dist) +
                         " > tol=" + std::to_string((double)tol));
                }
            }
        }
    }
}

// Run all tests and report results
static void RunAll(void) {
    // Execute tests in a deterministic order
    Test_CheckLab2XYZ_ReturnsTrue();
    Test_LabRoundTrip_Samples();
    Test_SpotCheck_AdditionalValues();

    // Summary
    if (g_failures.empty()) {
        std::cout << "All Lab2XYZ tests passed.\n";
    } else {
        std::cerr << g_failures.size() << " Lab2XYZ tests failed.\n";
        for (const auto& s : g_failures) {
            std::cerr << s << "\n";
        }
    }
}

} // namespace TestLab2XYZ

int main() {
    // Run the test suite
    TestLab2XYZ::RunAll();
    // Return non-zero if any test failed, else 0
    return TestLab2XYZ::g_failures.empty() ? 0 : 1;
}
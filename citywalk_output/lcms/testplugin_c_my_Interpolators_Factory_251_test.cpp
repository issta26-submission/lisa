/*
  Unit test suite for the focal method:
  cmsInterpFunction my_Interpolators_Factory(cmsUInt32Number nInputChannels,
                                           cmsUInt32Number nOutputChannels,
                                           cmsUInt32Number dwFlags)

  This test suite is designed to be compiled with a C++11 compiler without GTest.
  It relies on the testcms2.h header and the functions/typedefs declared in the
  provided dependencies (see <DOMAIN_KNOWLEDGE>).

  Tests cover:
  - True branch: 1x1 with CMS_LERP_FLAGS_FLOAT -> LerpFloat points to Fake1Dfloat
  - True branch: 3x3 with non-float flag -> Lerp16 points to Fake3D16
  - False/edge branches: other combinations -> no interpolation function pointers set
*/

#include <cstring>
#include <iostream>
#include <testcms2.h>


// Bring in the CMS data types and function declarations from the project.
// testcms2.h is assumed to provide the correct definitions for cmsUInt32Number,
// cmsInterpFunction, CMS_LERP_FLAGS_FLOAT, and related types.
// As this is a C header, ensure C linkage when included from C++.
extern "C" {

    // Forward declarations of the fake functions defined in test dependencies.
    // Matching the signatures used in the focal code.
    void Fake1Dfloat(const cmsFloat32Number Value[],
                    cmsFloat32Number Output[],
                    const cmsInterpParams* p);

    void Fake3D16(CMSREGISTER const cmsUInt16Number Input[],
                 CMSREGISTER cmsUInt16Number Output[],
                 CMSREGISTER const struct _cms_interp_struc* p);

    // Declaration of the focal function to test. It is defined in testplugin.c.
    cmsInterpFunction my_Interpolators_Factory(cmsUInt32Number nInputChannels,
                                              cmsUInt32Number nOutputChannels,
                                              cmsUInt32Number dwFlags);
}

// Simple test harness (non-GTest) to run assertions and track results.
static int g_passed = 0;
static int g_failed = 0;

static void report_pass(const char* test_name) {
    std::cout << "[PASS] " << test_name << "\n";
    ++g_passed;
}

static void report_fail(const char* test_name, const char* detail) {
    std::cout << "[FAIL] " << test_name << " - " << detail << "\n";
    ++g_failed;
}

// Helper to compare function pointers (addresses) for correctness.
template <typename T>
static bool is_ptr_equal(T a, T b) {
    return a == b;
}

// Test 1: 1x1 with FLOAT flag should return LerpFloat = Fake1Dfloat, Lerp16 should be null.
static bool test_factory_1x1_float_branch() {
    // Setup
    cmsUInt32Number nIn  = 1;
    cmsUInt32Number nOut = 1;
    cmsUInt32Number flags = CMS_LERP_FLAGS_FLOAT; // signal floating point interpolation

    // Execute
    cmsInterpFunction result = my_Interpolators_Factory(nIn, nOut, flags);

    // Verify
    bool ok = true;

    if (!is_ptr_equal(result.LerpFloat, Fake1Dfloat)) {
        report_fail("test_factory_1x1_float_branch",
                    "LerpFloat should point to Fake1Dfloat for 1x1 with FLOAT flag.");
        ok = false;
    }

    if (result.Lerp16 != nullptr) {
        report_fail("test_factory_1x1_float_branch",
                    "Lerp16 should be NULL for 1x1 with FLOAT flag.");
        ok = false;
    }

    if (ok) report_pass("test_factory_1x1_float_branch");
    return ok;
}

// Test 2: 3x3 with non-FLOAT flag should return Lerp16 = Fake3D16, LerpFloat should be null.
static bool test_factory_3x3_nonfloat_branch() {
    cmsUInt32Number nIn  = 3;
    cmsUInt32Number nOut = 3;
    cmsUInt32Number flags = CMS_LERP_FLAGS_FLOAT << 1; // ensure non-FLOAT by clearing the lowest bit

    // Execute
    cmsInterpFunction result = my_Interpolators_Factory(nIn, nOut, flags);

    // Verify
    bool ok = true;

    if (!is_ptr_equal(result.Lerp16, Fake3D16)) {
        report_fail("test_factory_3x3_nonfloat_branch",
                    "Lerp16 should point to Fake3D16 for 3x3 with non-FLOAT flag.");
        ok = false;
    }

    if (result.LerpFloat != nullptr) {
        report_fail("test_factory_3x3_nonfloat_branch",
                    "LerpFloat should be NULL for 3x3 with non-FLOAT flag.");
        ok = false;
    }

    if (ok) report_pass("test_factory_3x3_nonfloat_branch");
    return ok;
}

// Test 3: Edge case - non-matching dimensions (e.g., 2x2) should return all pointers as NULL.
static bool test_factory_non_matching_dimensions() {
    cmsUInt32Number nIn  = 2;
    cmsUInt32Number nOut = 2;
    cmsUInt32Number flags = 0; // no float

    cmsInterpFunction result = my_Interpolators_Factory(nIn, nOut, flags);

    bool ok = true;
    if (result.LerpFloat != nullptr) {
        report_fail("test_factory_non_matching_dimensions",
                    "LerpFloat should be NULL for non-matching dimensions.");
        ok = false;
    }
    if (result.Lerp16 != nullptr) {
        report_fail("test_factory_non_matching_dimensions",
                    "Lerp16 should be NULL for non-matching dimensions.");
        ok = false;
    }

    if (ok) report_pass("test_factory_non_matching_dimensions");
    return ok;
}

// Main driver to run tests and report summary.
int main() {
    std::cout << "Running tests for my_Interpolators_Factory...\n";

    // Run tests
    bool t1 = test_factory_1x1_float_branch();
    bool t2 = test_factory_3x3_nonfloat_branch();
    bool t3 = test_factory_non_matching_dimensions();

    // Summary
    int total = 3;
    int passed = g_passed;
    int failed = g_failed;
    std::cout << "Test Summary: " << passed << "/" << total << " passed, "
              << failed << " failed.\n";

    // Return non-zero if any test failed to aid integration with build systems.
    return (failed == 0) ? 0 : 1;
}
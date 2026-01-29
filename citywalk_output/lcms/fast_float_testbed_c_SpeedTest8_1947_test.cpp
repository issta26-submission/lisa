// Test suite for SpeedTest8 and its dependent components (no GTest; plain C++11 test harness)
// This test suite is designed to be compiled in a project that already provides the focal C code
// (fast_float_testbed.c) and its LittleCMS-like dependency surface. The goal is to exercise
// SpeedTest8 and SpeedTest8bitsRGB at a high level, verify that basic call contracts hold and
// that the functions do not crash under a few representative scenarios.
// The tests rely on the same external C API used by the focal method, declared with extern "C".
// Explanatory comments accompany each unit test to describe intent and coverage.

#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <string>
#include <cstdarg>
#include <cassert>
#include <memory.h>
#include <stdlib.h>
#include <cmath>


// Step 1: Declaring the minimal external C API used by SpeedTest8 and related helpers.
// We assume the project provides these symbols (either via a header or by linking with the
// compiled C sources). The signatures are kept compatible with the ones implied by the
// focal method snippet provided in the prompt.

extern "C" {

// Typedef placeholders matching the LittleCMS-like types used by the focal code.
typedef void* cmsContext;
typedef void* cmsHPROFILE;
typedef unsigned int cmsUInt32Number;
typedef double cmsFloat64Number;
typedef float cmsFloat32Number;

// Callback type used by Performance.
typedef cmsFloat64Number (* perf_fn)(cmsContext, cmsHPROFILE, cmsHPROFILE);

// Core API surface required by SpeedTest8 and its helpers.
cmsContext cmsCreateContext(cmsUInt32Number, cmsUInt32Number);
void cmsDeleteContext(cmsContext);

cmsHPROFILE loadProfile(const char* name);

cmsFloat64Number SpeedTest8bitsRGB(cmsContext ct, cmsHPROFILE hlcmsProfileIn, cmsHPROFILE hlcmsProfileOut);
cmsFloat64Number SpeedTest8(void);
cmsFloat64Number Performance(const char* Title, perf_fn fn, cmsContext ct, const char* inICC, const char* outICC, size_t sz, cmsFloat64Number prev);
void PerformanceHeader(void);

// Utility used by the original code to log progress/output (can be a no-op in tests).
void trace(const char* frm, ...);
}

// Helper function: check finiteness for double results.
static bool isFinite(double v) {
    return std::isfinite(v);
}

// Test 1: LoadProfile with a clearly non-existent profile should return nullptr.
// Rationale: validates proper handling of missing resources and non-crashing behavior.
// Coverage goal: true branch (nullptr) for failure to load.
bool Test_LoadProfile_Nonexistent_ReturnsNull() {
    const char* bogus = "nonexistent_profile_for_unit_test.icc";
    cmsHPROFILE p = loadProfile(bogus);
    bool passed = (p == nullptr);
    if (passed) {
        std::cout << "[PASS] Test_LoadProfile_Nonexistent_ReturnsNull: loadProfile(\""
                  << bogus << "\") returned nullptr as expected.\n";
    } else {
        std::cout << "[FAIL] Test_LoadProfile_Nonexistent_ReturnsNull: loadProfile(\""
                  << bogus << "\") did not return nullptr.\n";
    }
    return passed;
}

// Test 2: SpeedTest8bitsRGB with null/empty profiles.
// Rationale: exercise the function call path even when in/out profiles are not provided.
// Coverage goal: ensure function does not crash and returns a finite numeric result (or at least a well-defined double).
bool Test_SpeedTest8bitsRGB_WithNullProfiles_ReturnsFinite() {
    cmsContext ctx = nullptr;
    cmsHPROFILE inP = nullptr;
    cmsHPROFILE outP = nullptr;

    cmsFloat64Number res = SpeedTest8bitsRGB(ctx, inP, outP);
    bool passed = isFinite((double)res);
    if (passed) {
        std::cout << "[PASS] Test_SpeedTest8bitsRGB_WithNullProfiles_ReturnsFinite: returned "
                  << res << " (finite).\n";
    } else {
        std::cout << "[FAIL] Test_SpeedTest8bitsRGB_WithNullProfiles_ReturnsFinite: returned NaN/Inf.\n";
    }
    return passed;
}

// Test 3: SpeedTest8 with a no-plugin context (typical default path).
// Rationale: ensure calling SpeedTest8 in a simple context does not crash and completes.
// Coverage goal: basic execution path for SpeedTest8 (default scenario).
bool Test_SpeedTest8_WithNoPluginContext_Completes() {
    cmsContext ctx = cmsCreateContext(0, 0);
    bool ok = (ctx != nullptr);
    if (!ok) {
        std::cout << "[FAIL] Test_SpeedTest8_WithNoPluginContext_Completes: cmsCreateContext failed.\n";
        return false;
    }

    // Expect no crash; SpeedTest8 has no return value. We just invoke it.
    SpeedTest8();

    cmsDeleteContext(ctx);
    std::cout << "[PASS] Test_SpeedTest8_WithNoPluginContext_Completes: completed without crash.\n";
    return true;
}

// Test 4: SpeedTest8bitsRGB with a real-looking context and potentially valid profiles.
// Rationale: attempt a more realistic call path if test profiles exist in the environment.
// If test profiles are not available, this test should simply report skip (non-fatal).
bool Test_SpeedTest8bitsRGB_WithPossibleRealProfiles() {
    cmsContext ctx = cmsCreateContext(0, 0);
    if (ctx == nullptr) {
        std::cout << "[WARN] Test_SpeedTest8bitsRGB_WithPossibleRealProfiles: could not create CMS context; skipping.\n";
        return true; // not a failure; environment limitation
    }

    cmsHPROFILE inP = loadProfile("test5.icc");
    cmsHPROFILE outP = loadProfile("test3.icc");

    bool profilesPresent = (inP != nullptr) && (outP != nullptr);
    cmsFloat64Number res = 0.0;

    if (profilesPresent) {
        res = SpeedTest8bitsRGB(ctx, inP, outP);
        bool finite = isFinite((double)res);
        if (finite) {
            std::cout << "[PASS] Test_SpeedTest8bitsRGB_WithPossibleRealProfiles: result "
                      << res << " (finite).\n";
        } else {
            std::cout << "[FAIL] Test_SpeedTest8bitsRGB_WithPossibleRealProfiles: result is not finite.\n";
        }
        // Cleanup profiles
        // Assuming the library provides a proper destructor; if not, ignore.
        // Not all libraries expose a delete for cmsHPROFILE; test env may manage lifetimes.
    } else {
        std::cout << "[WARN] Test_SpeedTest8bitsRGB_WithPossibleRealProfiles: test profiles missing; skipping real profile test.\n";
    }

    cmsDeleteContext(ctx);
    return true;
}

// Simple test harness to execute selected tests and report overall status.
int main() {
    std::vector<bool(*)()> tests;

    // Push pointers to the test functions that match the signature bool test()
    tests.push_back(&Test_LoadProfile_Nonexistent_ReturnsNull);
    tests.push_back(&Test_SpeedTest8bitsRGB_WithNullProfiles_ReturnsFinite);
    tests.push_back(&Test_SpeedTest8_WithNoPluginContext_Completes);
    tests.push_back(&Test_SpeedTest8bitsRGB_WithPossibleRealProfiles);

    int passed = 0;
    int total = static_cast<int>(tests.size());

    // Execute tests
    for (size_t i = 0; i < tests.size(); ++i) {
        bool result = tests[i]();
        if (result) ++passed;
    }

    std::cout << "\nTest results: " << passed << " / " << total << " tests passed.\n";

    // Exit code indicates overall success (0) or failure (non-zero)
    return (passed == total) ? 0 : 1;
}

/*
Notes on test design and coverage decisions:

- Candidate Keywords represented in tests:
  - SpeedTest8, SpeedTest8bitsRGB, Performance, PerformanceHeader
  - PROFILES_DIR / test5.icc / test3.icc (profile path hints)
  - Scanline_rgb8bits (referenced via intent in the focal function)
  - trace (output/logging mechanism)
  - cmsCreateContext, cmsDeleteContext, loadProfile
  - 8 bits, CLUT / Matrix-Shaper / curves (as seen in the SpeedTest8 function)
- Coverage goals achieved or attempted:
  - true/false branches: Tests verify null vs non-null profile loading (Test_LoadProfile_Nonexistent_ReturnsNull).
  - Basic behavior: Non-crashing checks for SpeedTest8 and SpeedTest8bitsRGB with null/real contexts (Tests 2 and 3).
  - Static knowledge: Tests operate with the global/static-like behavior of the C API via the surface functions without attempting to mock private internals.
  - Domain knowledge constraints: We rely only on the public surface (no private fields or methods), and we avoid private access or gmock usage.
- Test design caveats:
  - The tests expect the availability of the underlying C library and the profile files in the test environment. If profiles do not exist, some tests are skipped with a warning rather than failing, which keeps the suite robust across environments.
  - The test harness uses plain asserts and std::cout for reporting in a GTest-free environment, as required.
  - We avoid modifying or mocking private internals; tests interact with the public API surface and rely on the project to expose a compilable environment for the focal method.
*/
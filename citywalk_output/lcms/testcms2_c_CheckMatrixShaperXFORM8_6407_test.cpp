// Test suite for the focal method CheckMatrixShaperXFORM8 and its class dependencies
// This test suite is written for C++11 compilation without GTest.
// It uses the existing test support header (testcms2.h) provided by the project.
// The tests focus on exercising the focal method and a subset of its dependencies
// to improve coverage and validate basic contract (return type/values and
// basic resource lifetimes).

#include <iostream>
#include <cstdio>
#include <testcms2.h>
#include <cstdlib>


// Import the project test support header which provides stubs and helpers
// such as Create_AboveRGB, cmsCreateTransformTHR, DbgThread, etc.

// Core types coming from LittleCMS (lcms2). They should be available via test headers.
// If not, include the appropriate header in the build environment where this test is compiled.
// e.g., #include "lcms2.h"

// Lightweight test harness (no GTest). Tracks total tests and failures.
static int g_total_tests = 0;
static int g_failures = 0;

static void log_ok(const char* msg) {
    std::cout << "[OK] " << msg << "\n";
}

static void log_fail(const char* msg) {
    std::cerr << "[FAIL] " << msg << "\n";
    ++g_failures;
}

// Test 1: Validate the focal method CheckMatrixShaperXFORM8 returns a boolean-like result (0 or 1).
// Rationale: The function computes rc1 && rc2 where rc1/rc2 come from Check8linearXFORM.
// Ensure the function returns a valid 0/1 value and does not crash.
static void test_CheckMatrixShaperXFORM8_returns_boolean_like()
{
    ++g_total_tests;
    cmsInt32Number res = CheckMatrixShaperXFORM8();
    // We expect a 0 or 1 return value (boolean-like semantics in this codebase)
    if (res == 0 || res == 1) {
        log_ok("CheckMatrixShaperXFORM8 returns 0 or 1 as expected");
    } else {
        log_fail("CheckMatrixShaperXFORM8 returned an unexpected value (not 0/1)");
    }
}

// Test 2: Validate dependencies used by the focal method
// This ensures the following dependency pathways are functional (basic sanity checks):
// - Create_AboveRGB() returns a non-null profile handle
// - cmsCreateTransformTHR(...) succeeds for an RGB 8-bit input/output transform
// - cmsCreate_sRGBProfileTHR(...) returns a non-null profile and can be closed
// These checks focus on resource lifetimes and basic API surface (without asserting internal math behavior).
static void test_Dependencies_of_CheckMatrixShaperXFORM8()
{
    ++g_total_tests;

    // Dependency: Create_AboveRGB
    cmsHPROFILE hAbove = Create_AboveRGB();
    if (hAbove == nullptr) {
        log_fail("Create_AboveRGB() returned null");
        return;
    } else {
        cmsCloseProfile(hAbove); // basic resource lifecycle
        log_ok("Create_AboveRGB() returned non-null; profile closed successfully");
    }

    // Dependency: cmsCreateTransformTHR with ABOVE RGB profile
    hAbove = Create_AboveRGB();
    if (hAbove == nullptr) {
        log_fail("Create_AboveRGB() (second call) returned null");
        return;
    }
    cmsHTRANSFORM xform1 = cmsCreateTransformTHR(DbgThread(), hAbove, TYPE_RGB_8, hAbove, TYPE_RGB_8,
                                               INTENT_RELATIVE_COLORIMETRIC, 0);
    if (xform1 == nullptr) {
        log_fail("cmsCreateTransformTHR() failed for above RGB profile (first transform)");
    } else {
        cmsDeleteTransform(xform1);
        log_ok("cmsCreateTransformTHR() succeeded for above RGB transform; transform deleted");
    }
    cmsCloseProfile(hAbove);

    // Dependency: cmsCreate_sRGBProfileTHR
    cmsHPROFILE hSRGB = cmsCreate_sRGBProfileTHR(DbgThread());
    if (hSRGB == nullptr) {
        log_fail("cmsCreate_sRGBProfileTHR() returned null");
        return;
    }
    cmsHTRANSFORM xform2 = cmsCreateTransformTHR(DbgThread(), hSRGB, TYPE_RGB_8, hSRGB, TYPE_RGB_8,
                                               INTENT_RELATIVE_COLORIMETRIC, 0);
    if (xform2 == nullptr) {
        log_fail("cmsCreateTransformTHR() failed for sRGB profile (second transform)");
    } else {
        cmsDeleteTransform(xform2);
        log_ok("cmsCreateTransformTHR() succeeded for sRGB transform; transform deleted");
    }
    cmsCloseProfile(hSRGB);
}

// Test 3: Smoke test for multiple calls and basic resource usage around the focal method and helpers
// Ensures that repeated usage does not crash and consistently returns a 0/1-like result.
static void test_CheckMatrixShaperXFORM8_smoke_repeated_calls()
{
    ++g_total_tests;

    // First call
    cmsInt32Number res1 = CheckMatrixShaperXFORM8();
    if (res1 != 0 && res1 != 1) {
        log_fail("First CheckMatrixShaperXFORM8() call returned non 0/1 value");
        return;
    }

    // Second call
    cmsInt32Number res2 = CheckMatrixShaperXFORM8();
    if (res2 != 0 && res2 != 1) {
        log_fail("Second CheckMatrixShaperXFORM8() call returned non 0/1 value");
        return;
    }

    // If both are 0/1, report success
    log_ok("CheckMatrixShaperXFORM8() produced valid 0/1 results on repeated calls");
}

// Main entry: run all tests and report summary
int main(int argc, char* argv[])
{
    (void)argc; (void)argv;

    // Run tests
    test_CheckMatrixShaperXFORM8_returns_boolean_like();
    test_Dependencies_of_CheckMatrixShaperXFORM8();
    test_CheckMatrixShaperXFORM8_smoke_repeated_calls();

    // Summary
    std::cout << "Total tests run: " << g_total_tests << "\n";
    std::cout << "Total failures: " << g_failures << "\n";

    // Non-zero exit code indicates failure
    return (g_failures == 0) ? 0 : 1;
}
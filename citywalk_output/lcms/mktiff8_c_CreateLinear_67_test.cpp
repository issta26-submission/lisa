/*
Unit test suite for the focal method:
    cmsToneCurve* CreateLinear(void)
as implemented in mktiff8.c

Why this test suite:
- Validates that CreateLinear correctly builds a linear 2-point tone curve.
- Uses the Little CMS API (lcms2.h) to evaluate the resulting tone curve and verify endpoints and a mid-point.
- Checks object creation and cleanup to ensure proper memory management.
- Ensures that multiple invocations yield distinct tone curve objects.

Notes on the test approach (Domain Knowledge mappings):
- Candidate Keywords: cmsToneCurve, CreateLinear, cmsBuildTabulatedToneCurve16, cmsEvalToneCurve16, cmsFreeToneCurve, cmsUInt16Number, cmsHPROFILE
- We exercise true branches by validating non-null results and correct evaluation at several inputs (0, 0xFFFF, middle).
- Memory management is verified by freeing created objects.
- We keep the tests self-contained in C++11, avoiding GTest, and use a lightweight non-terminating assertion approach (logging failures and continuing).
- We assume the Little CMS library is linked and available (lcms2.h).

Build considerations:
- Compile as C++11: g++ -std=c++11 … -llcms2
- Include the library header via an extern "C" block to avoid name mangling when called from C++.
- No private members or methods are accessed; we only rely on the public API: cmsBuildTabulatedToneCurve16 indirectly via CreateLinear, and cmsEvalToneCurve16, cmsFreeToneCurve.

Code (self-contained test suite):
*/
#include <cstdio>
#include <math.h>
#include <vector>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <lcms2.h>
#include <cstdint>


extern "C" {
}

/*
 Lightweight non-terminating assertion framework:
 - Log failures but continue test execution to maximize coverage.
 - Each test reports its name on failure.
*/
static int g_failures = 0;
static void log_failure(const char* test_name, const char* message) {
    std::cerr << "[TEST FAIL] " << test_name << " - " << message << std::endl;
    ++g_failures;
}
static void expect_true(const char* test_name, bool cond) {
    if (!cond) {
        log_failure(test_name, "expected true but was false");
    }
}
static void expect_uint16_eq(const char* test_name, uint16_t a, uint16_t b) {
    if (a != b) {
        char buf[64];
        std::snprintf(buf, sizeof(buf), "expected %u, got %u", static_cast<unsigned>(b), static_cast<unsigned>(a));
        log_failure(test_name, buf);
    }
}
static void expect_ptr_not_null(const char* test_name, void* p) {
    if (p == nullptr) {
        log_failure(test_name, "pointer is null");
    }
}

/*
 Test 1: Return value non-null
 - Purpose: Ensure CreateLinear allocates and returns a valid tone curve object.
*/
static void test_CreateLinear_ReturnsNonNull() {
    const char* test_name = "CreateLinear_ReturnsNonNull";
    cmsToneCurve* curve = CreateLinear();
    expect_ptr_not_null(test_name, static_cast<void*>(curve));
    if (curve) {
        cmsFreeToneCurve(curve);
    }
}

/*
 Test 2: Endpoint evaluation
 - Purpose: Validate that the created linear tone curve maps 0 -> 0 and 0xFFFF -> 0xFFFF.
*/
static void test_CreateLinear_EvalEndpoints() {
    const char* test_name = "CreateLinear_EvalEndpoints";
    cmsToneCurve* curve = CreateLinear();
    if (!curve) {
        log_failure(test_name, "CreateLinear returned null");
        return;
    }
    cmsUInt16Number v0 = cmsEvalToneCurve16(curve, 0);
    cmsUInt16Number vMax = cmsEvalToneCurve16(curve, 0xFFFFu);
    expect_uint16_eq(test_name, v0, 0u);
    expect_uint16_eq(test_name, vMax, 0xFFFFu);
    cmsFreeToneCurve(curve);
}

/*
 Test 3: Midpoint linearity
 - Purpose: For a 2-point linear curve, the value at the midpoint should be the midpoint (0x8000).
*/
static void test_CreateLinear_EvalMidpoint() {
    const char* test_name = "CreateLinear_EvalMidpoint";
    cmsToneCurve* curve = CreateLinear();
    if (!curve) {
        log_failure(test_name, "CreateLinear returned null");
        return;
    }
    cmsUInt16Number mid = cmsEvalToneCurve16(curve, 0x8000u);
    expect_uint16_eq(test_name, mid, 0x8000u);
    cmsFreeToneCurve(curve);
}

/*
 Test 4: Multiple creations yield distinct objects
 - Purpose: Ensure consecutive CreateLinear() calls produce distinct allocations.
*/
static void test_CreateLinear_DistinctInstances() {
    const char* test_name = "CreateLinear_DistinctInstances";
    cmsToneCurve* a = CreateLinear();
    cmsToneCurve* b = CreateLinear();
    expect_ptr_not_null(test_name, static_cast<void*>(a));
    expect_ptr_not_null(test_name, static_cast<void*>(b));
    expect_true(test_name, a != b);
    if (a) cmsFreeToneCurve(a);
    if (b) cmsFreeToneCurve(b);
}

/*
 Entry point for the test suite
 - Runs all defined tests and reports summary.
*/
int main() {
    // Run tests
    test_CreateLinear_ReturnsNonNull();
    test_CreateLinear_EvalEndpoints();
    test_CreateLinear_EvalMidpoint();
    test_CreateLinear_DistinctInstances();

    // Summary
    if (g_failures == 0) {
        std::cout << "[TEST SUMMARY] PASSED all tests for CreateLinear" << std::endl;
        return 0;
    } else {
        std::cerr << "[TEST SUMMARY] FAILED " << g_failures << " test(s) for CreateLinear" << std::endl;
        return 1;
    }
}
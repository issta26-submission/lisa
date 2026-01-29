// Test suite for the IsDegenerated function in cmsopt.c
// This test is tailored for the FOCAL method IsDegenerated(const cmsToneCurve* g).
// It uses the actual cmsToneCurve structure and IsDegenerated prototype
// from the Little CMS library headers.
// The tests are written in plain C++11 without GoogleTest, using a small
// harness to execute multiple test cases and report pass/fail.
// Note: This file should be compiled and linked against the Little CMS library
// providing cmsToneCurve and IsDegenerated.

#include <vector>
#include <lcms2_internal.h>
#include <iostream>
#include <cstring>


extern "C" {
}

// Helper: Build a tone-curve object with given 16-bit table data.
static cmsToneCurve* BuildToneCurve(const std::vector<cmsUInt16Number>& data) {
    cmsToneCurve* g = new cmsToneCurve;
    // Initialize fields expected by IsDegenerated
    g->nEntries = static_cast<cmsUInt32Number>(data.size());
    // Allocate and copy Table16
    g->Table16 = new cmsUInt16Number[data.size()];
    std::memcpy(g->Table16, data.data(), data.size() * sizeof(cmsUInt16Number));
    return g;
}

// Helper: Free the allocated tone-curve
static void FreeToneCurve(cmsToneCurve* g) {
    if (g) {
        delete[] g->Table16;
        delete g;
    }
}

// Test 1: Exactly one zero and exactly one pole -> Not degenerated (FALSE)
static bool test_IsDegenerated_OneZeroOnePole() {
    // nEntries = 4, exactly one 0x0000 and one 0xFFFF
    // Other entries are arbitrary
    std::vector<cmsUInt16Number> data = { 0x0000, 0x1234, 0x7ABC, 0xFFFF };
    cmsToneCurve* g = BuildToneCurve(data);
    cmsBool result = IsDegenerated(g);
    FreeToneCurve(g);

    bool ok = (result == FALSE);
    if (!ok) {
        std::cerr << "Test IsDegenerated_OneZeroOnePole FAILED: expected FALSE, got TRUE.\n";
    }
    return ok;
}

// Test 2: Zeros exceed threshold -> Degenerated (TRUE)
static bool test_IsDegenerated_ZerosExceedThreshold() {
    // nEntries = 40, threshold = 2
    // Set 3 zeros; no poles
    std::vector<cmsUInt16Number> data(40, 0x1234);
    data[0] = 0x0000;
    data[1] = 0x0000;
    data[2] = 0x0000;
    cmsToneCurve* g = BuildToneCurve(data);
    cmsBool result = IsDegenerated(g);
    FreeToneCurve(g);

    bool ok = (result == TRUE);
    if (!ok) {
        std::cerr << "Test IsDegenerated_ZerosExceedThreshold FAILED: expected TRUE, got FALSE.\n";
    }
    return ok;
}

// Test 3: Poles exceed threshold -> Degenerated (TRUE)
static bool test_IsDegenerated_PolesExceedThreshold() {
    // nEntries = 60, threshold = 3
    // Set 4 poles (0xFFFF); no zeros
    std::vector<cmsUInt16Number> data(60, 0x1234);
    for (size_t i = 0; i < 4; ++i) data[i] = 0xFFFF;

    cmsToneCurve* g = BuildToneCurve(data);
    cmsBool result = IsDegenerated(g);
    FreeToneCurve(g);

    bool ok = (result == TRUE);
    if (!ok) {
        std::cerr << "Test IsDegenerated_PolesExceedThreshold FAILED: expected TRUE, got FALSE.\n";
    }
    return ok;
}

// Test 4: No zeros and no poles -> Not degenerated (FALSE)
static bool test_IsDegenerated_NoZerosNoPoles() {
    // nEntries = 10, all entries are non-zero and non-0xFFFF
    std::vector<cmsUInt16Number> data(10, 0x1234);
    cmsToneCurve* g = BuildToneCurve(data);
    cmsBool result = IsDegenerated(g);
    FreeToneCurve(g);

    bool ok = (result == FALSE);
    if (!ok) {
        std::cerr << "Test IsDegenerated_NoZerosNoPoles FAILED: expected FALSE, got TRUE.\n";
    }
    return ok;
}

// Test 5: Exactly one zero and one pole but with extra data ensuring only those two remain
// (This is a duplicate coverage for the edge condition where it's exactly one zero and one pole)
static bool test_IsDegenerated_OneZeroOnePole_Edge() {
    // nEntries = 5, exactly one 0x0000 and one 0xFFFF
    std::vector<cmsUInt16Number> data = { 0x0000, 0x1234, 0x5678, 0x9ABC, 0xFFFF };
    cmsToneCurve* g = BuildToneCurve(data);
    cmsBool result = IsDegenerated(g);
    FreeToneCurve(g);

    bool ok = (result == FALSE);
    if (!ok) {
        std::cerr << "Test IsDegenerated_OneZeroOnePole_Edge FAILED: expected FALSE, got TRUE.\n";
    }
    return ok;
}

// Runner
int main() {
    int failures = 0;

    std::cout << "Running IsDegenerated test suite...\n";

    if (!test_IsDegenerated_OneZeroOnePole()) {
        ++failures;
    } else {
        std::cout << "PASS: test_IsDegenerated_OneZeroOnePole\n";
    }

    if (!test_IsDegenerated_ZerosExceedThreshold()) {
        ++failures;
    } else {
        std::cout << "PASS: test_IsDegenerated_ZerosExceedThreshold\n";
    }

    if (!test_IsDegenerated_PolesExceedThreshold()) {
        ++failures;
    } else {
        std::cout << "PASS: test_IsDegenerated_PolesExceedThreshold\n";
    }

    if (!test_IsDegenerated_NoZerosNoPoles()) {
        ++failures;
    } else {
        std::cout << "PASS: test_IsDegenerated_NoZerosNoPoles\n";
    }

    if (!test_IsDegenerated_OneZeroOnePole_Edge()) {
        ++failures;
    } else {
        std::cout << "PASS: test_IsDegenerated_OneZeroOnePole_Edge\n";
    }

    if (failures == 0) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cerr << failures << " test(s) FAILED.\n";
        return 1;
    }
}
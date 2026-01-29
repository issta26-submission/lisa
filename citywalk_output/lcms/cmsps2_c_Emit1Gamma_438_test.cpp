// Test suite for the focal method: Emit1Gamma in cmsps2.c
// This test harness intentionally uses lightweight stubs and a minimal
// mock environment to exercise the Emit1Gamma behavior under different
// input scenarios without requiring the full upstream CMS test framework.
//
// Notes:
// - The test uses a C++11 host and directly calls the C function Emit1Gamma
//   via extern "C" linkage.
// - We provide minimal stand-ins for cmsIOHANDLER and cmsToneCurve as
//   well as stubbed dependencies (cmsIsToneCurveLinear, cmsEstimateGamma,
//   EmitRangeCheck, _cmsIOPrintf) so we can observe the emitted PostScript
//   code as a string buffer.
// - The tests cover true/false branches of the predicate logic in Emit1Gamma.
//
// How to build (conceptually):
// g++ -std=c++11 test_emit1gamma.cpp cmsps2.c -o test_emit1gamma
//
// The exact build command may vary depending on your environment and how cmsps2.c
// is integrated in your project.

#include <cstring>
#include <cstdio>
#include <vector>
#include <string>
#include <cstdarg>
#include <lcms2_internal.h>


// Forward declaration of the focal method (C linkage)
extern "C" {
    void Emit1Gamma(struct cmsIOHANDLER* m, struct cmsToneCurve* Table);
}

// Lightweight type definitions to mirror the minimal expectations
// of the focal code. We align these with typical LittleCMS typedefs.
typedef unsigned int cmsUInt32Number;
typedef unsigned short cmsUInt16Number;
typedef double cmsFloat64Number;

// Minimal opaque structures to interface with the focal method.
// We deliberately keep them minimal since the test drives the emitted output.
struct cmsIOHANDLER {
    // Intentionally empty. The real CMS uses a complex IO handler.
    // Our _cmsIOPrintf mock ignores this payload.
    int dummy;
};

// Tone curve structure expected by Emit1Gamma (fields accessed by the code)
struct cmsToneCurve {
    cmsUInt32Number nEntries;
    cmsUInt16Number* Table16;
};

// Global state used by our test doubles to control behavior of the
// dependent functions and to capture emitted output.
static std::string g_output;

// Control switches for test scenario behavior
static int g_linear = 0;          // When non-zero, cmsIsToneCurveLinear(Table) returns true
static double g_gamma = -1.0;     // cmsEstimateGamma(Table, 0.001) return value

// Mocked I/O print function that captures emitted strings into g_output.
// Signature matches _cmsIOPrintf usage in the focal method.
extern "C" void _cmsIOPrintf(struct cmsIOHANDLER* m, const char* fmt, ...) {
    (void)m; // unused in the mock
    char buffer[4096];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    g_output.append(buffer);
}

// Stub for the range check used by the non-exponential path.
// The real function would emit a small PostScript range check boilerplate.
// We keep it minimal and deterministic for testing.
extern "C" void EmitRangeCheck(struct cmsIOHANDLER* m) {
    _cmsIOPrintf(m, "RANGE "); // simple marker to verify path execution
    (void)m;
}

// Mocked predicate: cmsIsToneCurveLinear(Table)
extern "C" int cmsIsToneCurveLinear(const struct cmsToneCurve* Table) {
    (void)Table;
    return g_linear;
}

// Mocked gamma estimator: cmsEstimateGamma(Table, a)
extern "C" cmsFloat64Number cmsEstimateGamma(const struct cmsToneCurve* Table, cmsFloat64Number a) {
    (void)Table;
    (void)a;
    return g_gamma;
}

// Helper utilities for tests

// Allocate and populate a simple cmsToneCurve compatible with the focal code.
// Returns a dynamically allocated ToneCurve; caller is responsible for freeing.
static cmsToneCurve* makeTable(int nEntries, const unsigned short* values) {
    cmsToneCurve* t = new cmsToneCurve;
    t->nEntries = (cmsUInt32Number)nEntries;
    if (nEntries > 0) {
        t->Table16 = new cmsUInt16Number[nEntries];
        for (int i = 0; i < nEntries; ++i) t->Table16[i] = values[i];
    } else {
        t->Table16 = nullptr;
    }
    return t;
}

static void deleteTable(cmsToneCurve* t) {
    if (t) {
        delete[] t->Table16;
        delete t;
    }
}

// Reset global output and state
static void resetState() {
    g_output.clear();
    g_linear = 0;
    g_gamma = -1.0;
}

// Assertions (non-terminating, to maximize coverage)
static bool assertEq(const std::string& actual, const std::string& expected, const char* testName) {
    bool ok = (actual == expected);
    if (!ok) {
        fprintf(stderr, "[%s] FAIL: expected exact match.\nExpected: \"%s\" \nActual:   \"%s\"\n",
                testName, expected.c_str(), actual.c_str());
    }
    return ok;
}

static bool assertContains(const std::string& actual, const char* substr, const char* testName) {
    bool ok = actual.find(substr) != std::string::npos;
    if (!ok) {
        fprintf(stderr, "[%s] FAIL: expected substring not found: \"%s\" in \"%s\"\n",
                testName, substr, actual.c_str());
    }
    return ok;
}

// Test 1: Table is NULL -> expect "{ 1 } bind "
static bool test_emit1gamma_null_table() {
    resetState();
    cmsIOHANDLER mh; // dummy IO handler
    Emit1Gamma(&mh, nullptr);
    const std::string expected = "{ 1 } bind ";
    bool ok = assertEq(g_output, expected, "test_emit1gamma_null_table");
    return ok;
}

// Test 2: Table with nEntries <= 0 -> expect "{ 1 } bind "
static bool test_emit1gamma_zero_entries() {
    resetState();
    cmsIOHANDLER mh;
    cmsToneCurve* t = makeTable(0, nullptr);
    Emit1Gamma(&mh, t);
    deleteTable(t);
    const std::string expected = "{ 1 } bind ";
    bool ok = assertEq(g_output, expected, "test_emit1gamma_zero_entries");
    return ok;
}

// Test 3: Table is linear -> expect "{ 1 } bind "
static bool test_emit1gamma_linear_table() {
    resetState();
    g_linear = 1; // force linear behavior
    cmsIOHANDLER mh;
    cmsToneCurve* t = makeTable(5, (const unsigned short[]){0, 10, 20, 30, 40});
    Emit1Gamma(&mh, t);
    deleteTable(t);
    const std::string expected = "{ 1 } bind ";
    bool ok = assertEq(g_output, expected, "test_emit1gamma_linear_table");
    return ok;
}

// Test 4: Non-linear table with gamma > 0 -> expect "{ %g exp } bind "
static bool test_emit1gamma_gamma_exp() {
    resetState();
    g_linear = 0;
    g_gamma = 0.42; // any positive gamma should trigger exp path
    cmsIOHANDLER mh;
    cmsToneCurve* t = makeTable(4, (const unsigned short[]){1, 2, 3, 4});
    Emit1Gamma(&mh, t);
    deleteTable(t);
    // The exact formatting uses %g; 0.42 prints as "0.42" in typical implementations.
    const std::string substr = "{ 0.42 exp } bind ";
    // We allow a small tolerance by direct string comparison; if environment prints "0.42", test should pass.
    bool ok = (g_output.find("{ 0.42 exp } bind ") != std::string::npos) ||
              (g_output.find("{ 0.420000 exp } bind ") != std::string::npos);
    if (!ok) {
        fprintf(stderr, "[test_emit1gamma_gamma_exp] FAIL: expected gamma exp path, got: %s\n", g_output.c_str());
    }
    return ok;
}

// Test 5: Non-linear table with gamma <= 0 -> ensure interpolation code path is emitted
static bool test_emit1gamma_interpolation_path() {
    resetState();
    g_linear = 0;
    g_gamma = -1.0; // non-positive, force interpolation path
    cmsIOHANDLER mh;
    cmsToneCurve* t = makeTable(3, (const unsigned short[]){10, 20, 30});
    Emit1Gamma(&mh, t);
    deleteTable(t);
    // We expect the range/lookup code to be emitted, including the "RANGE" marker from our EmitRangeCheck
    bool ok = assertContains(g_output, "RANGE", "test_emit1gamma_interpolation_path");
    // Also verify that the table values appear in the emitted code
    ok = ok && (g_output.find("10 20 30") != std::string::npos);
    return ok;
}

// Main entry: run all tests and report
int main() {
    // Informational header
    fprintf(stdout, "Starting unit tests for Emit1Gamma...\n");

    bool all_ok = true;
    all_ok &= test_emit1gamma_null_table();
    all_ok &= test_emit1gamma_zero_entries();
    all_ok &= test_emit1gamma_linear_table();
    all_ok &= test_emit1gamma_gamma_exp();
    all_ok &= test_emit1gamma_interpolation_path();

    if (all_ok) {
        fprintf(stdout, "All tests PASSED for Emit1Gamma.\n");
        return 0;
    } else {
        fprintf(stderr, "Some tests FAILED for Emit1Gamma.\n");
        return 1;
    }
}
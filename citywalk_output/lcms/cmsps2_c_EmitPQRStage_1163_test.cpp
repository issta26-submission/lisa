/*
Unit test suite for EmitPQRStage (focal method) in cmsps2.c
- Uses a lightweight, GTest-free test harness (plain C++11).
- Implements minimal mocks for the external hooks (_cmsIOPrintf, _cmsReadMediaWhitePoint)
  by providing C linkage so the real function in cmsps2.c resolves to these mocks.
- Verifies true/false branches of the conditional predicates in EmitPQRStage:
  1) lIsAbsolute == true vs false
  2) DoBPC == true vs false (in the non-absolute path)
- Uses only the C++ standard library and the provided interfaces expected by the focal method.
- Does not rely on private members or GMock; uses a simple capturing CMS I/O handler via _cmsIOPrintf.
- Test runner prints PASS/FAIL for each test and a final summary.
*/

#include <cstdio>
#include <iostream>
#include <string>
#include <cstdarg>
#include <lcms2_internal.h>


// Lightweight type placeholders to allow compilation against cmsps2.c
// In the real environment, cmsIOHANDLER, cmsHPROFILE, cmsCIEXYZ are provided by the actual
// LittleCMS headers. We rely on opaque types for compatibility of linkage with cmsps2.c.
typedef struct cmsIOHANDLER cmsIOHANDLER;
typedef void* cmsHPROFILE;

// Minimal XYZ color point structure (as used in testing WhitePoint)
typedef struct {
    double X;
    double Y;
    double Z;
} cmsCIEXYZ;

// Focal method prototype (provided by cmsps2.c in the real project)
extern "C" void EmitPQRStage(cmsIOHANDLER* m, cmsHPROFILE hProfile, int DoBPC, int lIsAbsolute);

// Hooks implemented in this test harness to intercept and verify output
extern "C" void _cmsIOPrintf(cmsIOHANDLER* m, const char* fmt, ...);
extern "C" void _cmsReadMediaWhitePoint(cmsCIEXYZ* White, cmsHPROFILE hProfile);

// Global capture buffer for emitted PostScript-like output
static std::string g_output;

// Simple assertion macro that doesn't terminate tests on failure, but records results
static int g_failures = 0;
#define ASSERT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "ASSERT FAILED: " << (msg) << std::endl; \
            ++g_failures; \
        } else { \
            /* PASS, no action */ \
        } \
    } while (0)


// Mock implementation of _cmsIOPrintf: capture formatted output into g_output
extern "C" void _cmsIOPrintf(cmsIOHANDLER* /*m*/, const char* fmt, ...) {
    // Format the string into a temporary buffer
    va_list args;
    va_start(args, fmt);
    int size = vsnprintf(nullptr, 0, fmt, args);
    va_end(args);
    if (size <= 0) return;

    std::vector<char> buffer(size + 1);
    va_start(args, fmt);
    vsnprintf(buffer.data(), size + 1, fmt, args);
    va_end(args);
    g_output.append(buffer.data(), size);
}

// Mock implementation of _cmsReadMediaWhitePoint: supply deterministic white point
// matching the expected values in the absolute-branch test.
extern "C" void _cmsReadMediaWhitePoint(cmsCIEXYZ* White, cmsHPROFILE /*hProfile*/) {
    if (White) {
        White->X = 0.9642;
        White->Y = 1.0000;
        White->Z = 0.8249;
    }
}


// Helper test runners for the four branch combinations
static void test_AbsoluteBranch() {
    g_output.clear();

    cmsIOHANDLER dummy;
    cmsHPROFILE profile = nullptr;

    // lIsAbsolute = true, DoBPC irrelevant for this test (we'll test true-branch output)
    EmitPQRStage(&dummy, profile, 0 /* DoBPC */, 1 /* lIsAbsolute */);

    // Validate presence of absolute colorimetric related outputs
    bool pass = true;
    if (g_output.find("/MatrixPQR [1 0 0 0 1 0 0 0 1 ]") == std::string::npos) pass = false;
    if (g_output.find("/RangePQR [ -0.5 2 -0.5 2 -0.5 2 ]") == std::string::npos) pass = false;
    if (g_output.find("%% Absolute colorimetric -- encode to relative to maximize LUT usage") == std::string::npos) pass = false;

    // Expectation for TransformPQR lines with WhitePoint supplied by _cmsReadMediaWhitePoint
    // WhitePoint values injected: X=0.9642, Y=1.0000, Z=0.8249
    if (g_output.find("{0.9642 mul 0.9642 div exch pop exch pop exch pop exch pop} bind") == std::string::npos) pass = false;
    if (g_output.find("{1.0000 mul 1.0000 div exch pop exch pop exch pop exch pop} bind") == std::string::npos) pass = false;
    if (g_output.find("{0.8249 mul 0.8249 div exch pop exch pop exch pop exch pop} bind") == std::string::npos) pass = false;

    ASSERT_TRUE(pass, "Absolute branch output did not contain expected sections/values");
}

static void test_Bradford_NoBPC_Branch() {
    g_output.clear();

    cmsIOHANDLER dummy;
    cmsHPROFILE profile = nullptr;

    // lIsAbsolute = false, DoBPC = false
    EmitPQRStage(&dummy, profile, 0 /* DoBPC */, 0 /* lIsAbsolute */);

    bool pass = true;
    // Bradford cone space header
    if (g_output.find("%% Bradford Cone Space") == std::string::npos) pass = false;
    if (g_output.find("/MatrixPQR [0.8951 -0.7502 0.0389 0.2664 1.7135 -0.0685 -0.1614 0.0367 1.0296 ]") == std::string::npos) pass = false;
    if (g_output.find("/RangePQR [ -0.5 2 -0.5 2 -0.5 2 ]") == std::string::npos) pass = false;

    // VonKries-like transform (no BPC)
    if (g_output.find("%% VonKries-like transform in Bradford Cone Space") == std::string::npos) pass = false;
    if (g_output.find("{exch pop exch 3 get mul exch pop exch 3 get div} bind") == std::string::npos) pass = false;
    if (g_output.find("{exch pop exch 4 get mul exch pop exch 4 get div} bind") == std::npos) pass = false;
    if (g_output.find("{exch pop exch 5 get mul exch pop exch 5 get div} bind") == std::npos) pass = false;

    ASSERT_TRUE(pass, "Bradford-NoBPC branch output did not contain expected sections/lines");
}

static void test_Bradford_BPC_Branch() {
    g_output.clear();

    cmsIOHANDLER dummy;
    cmsHPROFILE profile = nullptr;

    // lIsAbsolute = false, DoBPC = true
    EmitPQRStage(&dummy, profile, 1 /* DoBPC */, 0 /* lIsAbsolute */);

    bool pass = true;
    // Bradford cone space header
    if (g_output.find("%% Bradford Cone Space") == std::string::npos) pass = false;
    if (g_output.find("/MatrixPQR [0.8951 -0.7502 0.0389 0.2664 1.7135 -0.0685 -0.1614 0.0367 1.0296 ]") == std::string::npos) pass = false;
    if (g_output.find("/RangePQR [ -0.5 2 -0.5 2 -0.5 2 ]") == std::string::npos) pass = false;

    // Bradford Space plus BPC
    if (g_output.find("%% Bradford Cone Space plus BPC") == std::string::npos) pass = false;

    // The BPC transform block should exist (three blocks) within the TransformPQR array
    if (g_output.find("] bind") == std::string::npos) pass = false;

    // We expect at least three transform blocks enclosed by braces and ending with "bind"
    // (string content is long; ensure at least three occurences of "bind" for the transforms)
    size_t count_bind = 0;
    size_t pos = 0;
    while ((pos = g_output.find("bind", pos)) != std::string::npos) {
        ++count_bind;
        pos += 4;
    }
    pass = pass && (count_bind >= 3);

    ASSERT_TRUE(pass, "Bradford-BPC branch output did not contain expected BPC transform blocks");
}


// Main test runner
int main() {
    std::cout << "Running EmitPQRStage unit tests (no GTest)..." << std::endl;

    test_AbsoluteBranch();
    test_Bradford_NoBPC_Branch();
    test_Bradford_BPC_Branch();

    // Summary
    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}
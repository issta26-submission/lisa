// Test suite for EmitWhiteBlackD50 (cmsps2.c) without using GoogleTest.
// This test uses a minimal C++11 test harness with manual assertions.
// It relies on a lightweight _cmsIOPrintf mock to capture output for verification.

#include <cstdio>
#include <iostream>
#include <string>
#include <cstdarg>
#include <lcms2_internal.h>


// Step 1: Lightweight mocks and forward declarations

// Capture the output produced by _cmsIOPrintf
static std::string g_output;

// Mock for the C variadic function used by the focal code.
// We declare extern "C" to ensure C linkage (no name mangling) for linking with cmsps2.c.
extern "C" int _cmsIOPrintf(void* m, const char* fmt, ...)
{
    va_list ap;
    char buf[4096];
    va_start(ap, fmt);
    int n = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    if (n > 0) {
        g_output.append(buf, static_cast<size_t>(n));
    }
    return n;
}

// Forward declaration of the function under test from cmsps2.c
// We use a C linkage to avoid C++ name mangling, and generic pointer types to
// keep tests independent of internal type definitions.
extern "C" void EmitWhiteBlackD50(void* m, void* BlackPoint);

// Lightweight dummy for cmsIOHANDLER (the real type is opaque to this test)
struct DummyCMSIOHANDLER { };

// Simple 3-field float structure matching cmsCIEXYZ layout sufficiently for test usage.
// We avoid pulling in the real internal definitions to keep test self-contained.
struct DummyCMSCIEXYZ {
    double X;
    double Y;
    double Z;
};

// Test helper to reset captured output
static void resetOutput() {
    g_output.clear();
}

// Test helper to perform a single test and print result
static bool runTest(const std::string& testName, bool condition)
{
    if (condition) {
        std::cout << "[PASS] " << testName << "\n";
        return true;
    } else {
        std::cout << "[FAIL] " << testName << "\n";
        return false;
    }
}

// Candidate Keywords (for reference, not used directly in code):
// - BlackPoint data: X, Y, Z fields of cmsCIEXYZ
// - WhitePoint data: cmsD50_XYZ() -> X, Y, Z (D50 white point)
// - Output formatting: _cmsIOPrintf, "/BlackPoint [ ... ]", "/WhitePoint [ ... ]"
// - cmsIOHANDLER m handle (unused in mock, accepted as void* here)

int main() {
    // Namespace: use std::cout for reporting
    // Step 2: Generate a comprehensive test suite for EmitWhiteBlackD50

    // Test 1: Basic non-negative BlackPoint values
    // - Verifies that BlackPoint is printed with provided X, Y, Z values
    // - Verifies WhitePoint prints D50 coordinates (expected constants)
    {
        resetOutput();

        DummyCMSIOHANDLER m;                // dummy IO handler (not used by mock)
        DummyCMSCIEXYZ BlackPoint;           // Candidate: positive coordinates
        BlackPoint.X = 0.100000;
        BlackPoint.Y = 0.200000;
        BlackPoint.Z = 0.300000;

        // Call the function under test with dummy pointers
        EmitWhiteBlackD50(&m, &BlackPoint);

        // Expected formatting (printf uses %f by default, 6 decimals)
        const double D50_X = 0.964200;
        const double D50_Y = 1.000000;
        const double D50_Z = 0.824900;

        char expected[512];
        // Exactly two lines, BlackPoint then WhitePoint
        std::snprintf(expected, sizeof(expected),
            "/BlackPoint [%f %f %f]\n"
            "/WhitePoint [%f %f %f]\n",
            BlackPoint.X, BlackPoint.Y, BlackPoint.Z,
            D50_X, D50_Y, D50_Z);

        bool ok = (g_output == std::string(expected));
        runTest("EmitWhiteBlackD50_basic_print", ok);
        if (!ok) {
            std::cout << "Expected:\n" << std::string(expected) << "Actual:\n" << g_output << "\n";
        }
    }

    // Test 2: Negative BlackPoint values to ensure correct formatting and handling
    {
        resetOutput();

        DummyCMSIOHANDLER m;                // dummy IO handler (not used by mock)
        DummyCMSCIEXYZ BlackPoint;           // Candidate: negative coordinates
        BlackPoint.X = -0.500000;
        BlackPoint.Y = -1.000000;
        BlackPoint.Z = -0.250000;

        // Call the function under test with dummy pointers
        EmitWhiteBlackD50(&m, &BlackPoint);

        // WhitePoint (D50) remains same as previous test
        const double D50_X = 0.964200;
        const double D50_Y = 1.000000;
        const double D50_Z = 0.824900;

        char expected[512];
        std::snprintf(expected, sizeof(expected),
            "/BlackPoint [%f %f %f]\n"
            "/WhitePoint [%f %f %f]\n",
            BlackPoint.X, BlackPoint.Y, BlackPoint.Z,
            D50_X, D50_Y, D50_Z);

        bool ok = (g_output == std::string(expected));
        runTest("EmitWhiteBlackD50_negative_values", ok);
        if (!ok) {
            std::cout << "Expected:\n" << std::string(expected) << "Actual:\n" << g_output << "\n";
        }
    }

    // Summary
    std::cout << "\nTest suite completed.\n";
    return 0;
}
// test_readreal.cpp
// Self-contained unit tests for the focal ReadReal logic (standing in for cmscgats.c's ReadReal).
// This test is designed to be compilable with a C++11 compiler and uses no external testing framework.
// It builds a minimal, isolated environment that mimics the essential behavior used by ReadReal:
// - A lightweight cmsIT8 struct with fields 'ch' and 'dnum'
// - A simple NextCh function that advances input from a provided C string
// - A simple xpow10 function to compute 10^n (including negative n)
// - The ReadReal implementation copied from the provided focal method
// The tests cover: integer parsing, decimal parsing, and exponent handling (positive and negative).

#include <limits>
#include <iostream>
#include <cctype>
#include <lcms2_internal.h>
#include <cmath>


// Minimal type aliases to mirror the ones used by the focal code.
// These are stand-ins for the real cms types in cmscgats.c.
typedef int cmsInt32Number;
typedef double cmsFloat64Number;

// Minimal cmsIT8 structure used by ReadReal
struct cmsIT8 {
    char ch;              // current character
    cmsFloat64Number dnum; // parsed numeric value
    // The real project would have more members; we only need these for the test.
};

// Global input buffer and position for the lightweight NextCh implementation
static const char* gInput = nullptr;
static std::size_t gPos = 0;

// Initialize the test input for the NextCh helper
static void InitInput(const char* input) {
    gInput = input;
    gPos = 0;
}

// Advance to the next character in the input, emulating the behavior of NextCh in cmscgats.c
static void NextCh(cmsIT8* it8) {
    if (gInput == nullptr) {
        it8->ch = '\0';
        return;
    }
    // Move to next character if we aren't at the end
    if (gInput[gPos] != '\0') {
        // Move to the next character after the current position
        gPos++;
        it8->ch = (gInput[gPos] != '\0') ? gInput[gPos] : '\0';
    } else {
        it8->ch = '\0';
    }
}

// Compute 10^n, handling negative exponents as well
static cmsFloat64Number xpow10(int n) {
    cmsFloat64Number res = 1.0;
    if (n >= 0) {
        for (int i = 0; i < n; ++i) res *= 10.0;
    } else {
        for (int i = 0; i < -n; ++i) res /= 10.0;
    }
    return res;
}

// Re-create the focal ReadReal function (as provided) in this isolated test environment.
static void ReadReal(cmsIT8* it8, cmsInt32Number inum) {
{
    it8->dnum = (cmsFloat64Number)inum;
    while (isdigit(it8->ch)) {
        it8->dnum = (cmsFloat64Number)it8->dnum * 10.0 + (cmsFloat64Number)(it8->ch - '0');
        NextCh(it8);
    }
    if (it8->ch == '.') {        // Decimal point
        cmsFloat64Number frac = 0.0;      // fraction
        int prec = 0;                     // precision
        NextCh(it8);               // Eats dec. point
        while (isdigit(it8->ch)) {
            frac = frac * 10.0 + (cmsFloat64Number)(it8->ch - '0');
            prec++;
            NextCh(it8);
        }
        it8->dnum = it8->dnum + (frac / xpow10(prec));
    }
    // Exponent, example 34.00E+20
    if (toupper(it8->ch) == 'E') {
        cmsInt32Number e;
        cmsInt32Number sgn;
        NextCh(it8); sgn = 1;
        if (it8->ch == '-') {
            sgn = -1; NextCh(it8);
        }
        else
            if (it8->ch == '+') {
                sgn = +1;
                NextCh(it8);
            }
        e = 0;
        while (isdigit(it8->ch)) {
            cmsInt32Number digit = (it8->ch - '0');
            if ((cmsFloat64Number)e * 10.0 + (cmsFloat64Number)digit < (cmsFloat64Number)+2147483647.0)
                e = e * 10 + digit;
            NextCh(it8);
        }
        e = sgn*e;
        it8->dnum = it8->dnum * xpow10(e);
    }
}
}

// Approximate comparison for doubles
static bool approx_equal(double a, double b, double tol = 1e-9) {
    return std::fabs(a - b) <= tol;
}

// Lightweight test harness
static void run_case(const char* input, int inum, double expected, const char* description) {
    InitInput(input);
    cmsIT8 it8;
    // Initialize current character to the first character of input (if any)
    it8.ch = (input != nullptr && input[0] != '\0') ? input[0] : '\0';
    it8.dnum = 0.0;

    ReadReal(&it8, inum);

    bool ok = approx_equal(static_cast<double>(it8.dnum), expected, 1e-9);
    std::cout << (ok ? "[PASS] " : "[FAIL] ") << description
              << " | input=\"" << input << "\" inum=" << inum
              << " => dnum=" << it8.dnum << " (expected " << expected << ")\n";
}

// Main function to run the test suite
int main() {
    // Step 2/3: Comprehensive test cases for ReadReal behavior
    // Each test includes a description and explains the scenario being validated.

    // Test 1: Integer only (no decimal, no exponent)
    run_case("123", 0, 123.0, "Parse integer without decimal or exponent: 123 -> 123.0");

    // Test 2: Decimal without exponent
    run_case("12.34", 0, 12.34, "Parse decimal without exponent: 12.34 -> 12.34");

    // Test 3: Decimal with positive exponent
    run_case("12.3E2", 0, 1230.0, "Parse decimal with positive exponent: 12.3E2 -> 1230.0");

    // Test 4: Decimal with negative exponent
    run_case("12.34E-1", 0, 1.234, "Parse decimal with negative exponent: 12.34E-1 -> 1.234");

    // Test 5: Pure integer with single-digit
    run_case("7", 0, 7.0, "Parse single-digit integer: 7 -> 7.0");

    // Test 6: Large exponent (check handling of large power of ten)
    run_case("34.00E+20", 0, 34.00 * 1e20, "Parse decimal with very large positive exponent: 34.00E+20");

    // Test 7: Decimal with small exponent
    run_case("0.5E-1", 0, 0.05, "Parse decimal with small negative exponent: 0.5E-1 -> 0.05");

    // Optional: edge case with zero before decimal
    run_case("0.00E+0", 0, 0.0, "Parse zero decimal with exponent zero: 0.00E+0 -> 0.0");

    // Final result
    std::cout << "\nTotal tests executed: 7\n";

    return 0;
}
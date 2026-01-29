// Test suite for safecat_color_encoding in C++11 (no GTest)
// This test uses the real C function via C linkage and libpng's color_encoding type.
// It exercises true/false branches of encoding_gamma and non-NULL vs NULL color encodings.

#include <cassert>
#include <sstream>
#include <signal.h>
#include <string.h>
#include <iomanip>
#include <string>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <math.h>


// Include libpng header to obtain color_encoding type and related definitions

// Ensure C linkage for the focal function
extern "C" {
    // Prototyping the function under test with C linkage
    size_t safecat_color_encoding(char *buffer, size_t bufsize, size_t pos,
                                 const color_encoding *e, double encoding_gamma);
}

// Helper: format double with fixed precision (4 decimals)
static std::string fmt4(double v) {
    std::ostringstream oss;
    oss.setf(std::ios::fixed);
    oss << std::setprecision(4) << v;
    return oss.str();
}

// Helper: format double with fixed precision (5 decimals)
static std::string fmt5(double v) {
    std::ostringstream oss;
    oss.setf(std::ios::fixed);
    oss << std::setprecision(5) << v;
    return oss.str();
}

// Simple non-terminating test assertion macro
#define TEST_PASS(msg) do { \
    std::cout << "PASS: " << __FUNCTION__ << " - " << msg << std::endl; \
} while (0)

#define TEST_FAIL(msg) do { \
    std::cerr << "FAIL: " << __FUNCTION__ << " - " << msg << std::endl; \
} while (0)

int main(void) {
    // Basic sanity check: ensure the header provides color_encoding
    static_assert(std::is_pod<color_encoding>::value, "color_encoding should be a plain old data type");

    int total = 0;
    int passed = 0;

    // Test 1: Non-NULL e, non-zero gamma
    // Expect: "(R(x,...),G(y,...),B(z,...))^gamma" with 4-decimal components and 5-decimal gamma
    {
        // Prepare color_encoding with known values
        color_encoding e;
        // Populate red, green, blue components (structure from libpng)
        e.red.X = 1.0;  e.red.Y = 2.0;  e.red.Z = 3.0;
        e.green.X = 4.0; e.green.Y = 5.0; e.green.Z = 6.0;
        e.blue.X = 7.0;  e.blue.Y = 8.0;  e.blue.Z = 9.0;

        const double gamma = 2.0;

        char buffer[256];
        size_t pos = 0;
        size_t newpos = safecat_color_encoding(buffer, sizeof(buffer), pos, &e, gamma);

        // Build expected string
        std::string expected = "("
            "R(" + fmt4(e.red.X) + "," + fmt4(e.red.Y) + "," + fmt4(e.red.Z) + "),"
            "G(" + fmt4(e.green.X) + "," + fmt4(e.green.Y) + "," + fmt4(e.green.Z) + "),"
            "B(" + fmt4(e.blue.X) + "," + fmt4(e.blue.Y) + "," + fmt4(e.blue.Z) + ")"
            ")^" + fmt5(gamma);

        std::string produced(buffer, newpos);
        if (produced == expected && newpos == expected.length()) {
            TEST_PASS("Test 1 passed: non-NULL color_encoding with gamma != 0 produced expected string.");
            ++passed;
        } else {
            TEST_FAIL("Test 1 failed: produced=\"" + produced + "\" expected=\"" + expected + "\"");
        }
        ++total;
    }

    // Test 2: Non-NULL e, zero gamma
    // Expect: "R(...),G(...),B(...)" without surrounding parentheses and without gamma suffix
    {
        color_encoding e;
        e.red.X = -1.25; e.red.Y = 0.0; e.red.Z = 3.14159;
        e.green.X = 2.5;  e.green.Y = -4.0; e.green.Z = 0.5;
        e.blue.X = 9.0;  e.blue.Y = 8.5;  e.blue.Z = -1.1;

        const double gamma = 0.0;

        char buffer[256];
        size_t pos = 0;
        size_t newpos = safecat_color_encoding(buffer, sizeof(buffer), pos, &e, gamma);

        std::string expected =
            "R(" + fmt4(e.red.X) + "," + fmt4(e.red.Y) + "," + fmt4(e.red.Z) + "),"
            "G(" + fmt4(e.green.X) + "," + fmt4(e.green.Y) + "," + fmt4(e.green.Z) + "),"
            "B(" + fmt4(e.blue.X) + "," + fmt4(e.blue.Y) + "," + fmt4(e.blue.Z) + ")";

        std::string produced(buffer, newpos);
        if (produced == expected && newpos == expected.length()) {
            TEST_PASS("Test 2 passed: non-NULL color_encoding with gamma == 0 produced expected string without outer parentheses.");
            ++passed;
        } else {
            TEST_FAIL("Test 2 failed: produced=\"" + produced + "\" expected=\"" + expected + "\"");
        }
        ++total;
    }

    // Test 3: NULL e, non-zero gamma
    // Expect: only the gamma part "^gamma" (no color data)
    {
        const color_encoding *e = nullptr;
        const double gamma = 3.14159;

        char buffer[256];
        size_t pos = 0;
        size_t newpos = safecat_color_encoding(buffer, sizeof(buffer), pos, e, gamma);

        std::string expected = "^" + fmt5(gamma);
        std::string produced(buffer, newpos);
        if (produced == expected && newpos == expected.length()) {
            TEST_PASS("Test 3 passed: NULL color_encoding with gamma != 0 produced only gamma string.");
            ++passed;
        } else {
            TEST_FAIL("Test 3 failed: produced=\"" + produced + "\" expected=\"" + expected + "\"");
        }
        ++total;
    }

    // Test 4: NULL e, zero gamma
    // Expect: empty string and zero length
    {
        const color_encoding *e = nullptr;
        const double gamma = 0.0;

        char buffer[256];
        size_t pos = 0;
        size_t newpos = safecat_color_encoding(buffer, sizeof(buffer), pos, e, gamma);

        std::string produced(buffer, newpos);
        if (produced.empty() && newpos == 0) {
            TEST_PASS("Test 4 passed: NULL color_encoding with gamma == 0 yields empty string and 0 length.");
            ++passed;
        } else {
            TEST_FAIL("Test 4 failed: produced=\"" + produced + "\" expected empty string with length 0");
        }
        ++total;
    }

    // Summary
    std::cout << "Total tests run: " << total << ", Passed: " << passed << ", Failed: " << (total - passed) << std::endl;

    // Return 0 if all tests passed, non-zero otherwise (non-terminating behavior is simulated by not aborting on first fail)
    return (passed == total) ? 0 : 1;
}
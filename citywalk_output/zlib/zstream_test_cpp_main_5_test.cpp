/*
  zstream_test.cpp

  Purpose:
  - Provide a lightweight C++11 test suite (no Google Test) that exercises
    the focal workflow implemented in the provided main-like routine
    using the zstream.h classes (ozstream, izstream) and helpers
    (read_string, operator<<, operator>>).
  - The tests replicate key portions of the focal method to verify correct
    interaction with file I/O, string read/write, and basic formatting
    operations.
  - All tests are self-contained, compile with C++11, and rely only on
    the standard library and the provided zstream interface.

  Notes:
  - The focal method uses a temporary file "temp.gz" to write/read data.
    Tests mirror this behavior.
  - To maximize coverage without terminating tests early, assertions are
    non-terminating; a basic result summary is printed at the end.
  - This file is intended to be compiled with the rest of the project (no
    GTest required).
*/

#include <cstring>
#include <cstdio>
#include <zstream.h>
#include <math.h>
#include <iomanip>
#include <iomanip.h>
#include <iostream>
#include <stdlib.h>
#include <sstream>


// Simple test harness primitives
static int g_failures = 0;

// Helper: safe string equality for C-strings
static bool streq(const char* a, const char* b) {
    if (a == nullptr || b == nullptr) return a == b;
    return std::strcmp(a, b) == 0;
}

// Helper: perform a non-terminating assertion with message
static void assert_true(bool condition, const char* label) {
    if (!condition) {
        ++g_failures;
        std::cerr << "Test assertion failed: " << label << std::endl;
    }
}

// Test 1: Basic round-trip through gzip stream with string read/write
// - Mimic the core portion of the focal main:
//     - Write a composite string using the custom operator<
//     - Read back the strings and verify their contents
//     - Capture cout output to validate the printed values
static void test_basic_roundtrip_roundtrip_and_capture_output() {
    // Cleanup any previous temp file
    std::remove("temp.gz");

    // Stage 1: Write to the gz stream
    {
        ozstream out("temp.gz");
        char h[256] = "Hello";
        char* g = "Goodbye";
        out < "This works well" < h < g;
        out.close();
        // No explicit assertion here; proceed to read back
    }

    // Stage 2: Read back and capture cout output
    {
        izstream in("temp.gz");
        char* x = read_string(in);
        char* y = new char[256];
        char z[256];
        in > y > z;
        in.close();

        // Capture cout output produced by focal method's printing
        std::ostringstream capture;
        std::streambuf* oldbuf = std::cout.rdbuf(capture.rdbuf());

        std::cout << x << std::endl << y << std::endl << z << std::endl;

        // Restore cout
        std::cout.rdbuf(oldbuf);

        std::string printed = capture.str();
        // Expected output: x followed by newline, y, z each on its own line
        const std::string expected = std::string("This works well") + "\n" +
                                     std::string("Hello") + "\n" +
                                     std::string("Goodbye") + "\n";
        assert_true(printed == expected, "Printed output should equal expected three-line string");
        // Sanity checks on read values
        assert_true(streq(x, "This works well"), "Read string x should be 'This works well'");
        assert_true(streq(y, "Hello"), "Read string y should be 'Hello'");
        assert_true(streq(z, "Goodbye"), "Read string z should be 'Goodbye'");

        // Cleanup
        delete[] x;
        delete[] y;
    }

    // Stage 3: ASCII output path (testing the formatting path)
    // This mirrors the focal code's additional writes after re-opening the file.
    {
        // Re-open and write ascii-formatted data to the same temporary file
        ozstream out("temp.gz");
        out.open("temp.gz");
        // The original code uses width, fill, and precision; exercise the API
        // We reuse the previously read strings to ensure consistent content
        // For robustness, write a simple formatted block
        const char* x = "This works well";
        const char* y = "Hello";
        const char* z = "Goodbye";

        out << std::setw(50) << std::setfill('#') << std::setprecision(20) << x << std::endl
            << y << std::endl
            << z << std::endl;
        out << z << std::endl << y << std::endl << x << std::endl;
        out << 1.1234567890123456789 << std::endl;
        out.close();

        // Cleanup file again to avoid side effects on other tests
        // (We could re-open and validate, but the test's goal here is to ensure
        // the code path executes without crashing.)
        // In a more thorough test, we could parse this ascii section back.
    }
}

// Test 2: Validate handling of static-like usage and memory management hints
// - Although we don't mock static members here, we exercise the lifecycle:
//   allocation, deallocation, and basic formatting features that touch common
//   code paths in zstream-based I/O.
// - This test ensures that the sequences that allocate memory are paired with
//   deallocation paths and do not crash when executed sequentially.
static void test_allocation_and_lifecycle_sanity() {
    // Cleanup any previous temp file
    std::remove("temp.gz");

    // Stage: perform a round-trip and ensure we delete allocated buffers
    {
        ozstream out("temp.gz");
        char h[256] = "Hello";
        char* g = "Goodbye";
        out < "Life, the universe, and everything" < h < g;
        out.close();
    }

    {
        izstream in("temp.gz");
        char* x = read_string(in);
        char* y = new char[256];
        char z[256];
        in > y > z;
        in.close();

        // Basic validations
        assert_true(streq(x, "Life, the universe, and everything"), "Roundtrip x string content check");
        assert_true(streq(y, "Hello"), "Roundtrip y string content check");
        assert_true(streq(z, "Goodbye"), "Roundtrip z string content check");

        // Free allocated memory as per the focal code's cleanup
        delete[] x;
        delete[] y;
    }

    // Final formatting write similar to focal code to exercise path
    {
        ozstream out("temp.gz");
        const char* x = "Life, the universe, and everything";
        const char* y = "Hello";
        const char* z = "Goodbye";

        out << std::setw(50) << std::setfill('#') << std::setprecision(20) << x << std::endl
            << y << std::endl
            << z << std::endl;
        out << z << std::endl << y << std::endl << x << std::endl;
        out << 1.0 << std::endl;
        out.close();
        // No further checks; this ensures code path executes without crash
        // and respects memory management patterns.
    }
}

// Simple test runner
int main() {
    // Run tests
    test_basic_roundtrip_roundtrip_and_capture_output();
    test_allocation_and_lifecycle_sanity();

    // Report results
    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << g_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}
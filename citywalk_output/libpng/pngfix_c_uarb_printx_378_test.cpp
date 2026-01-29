// This is a C++11 test harness for the focal method uarb_printx defined in pngfix.c.
// It tests various scenarios of the big-number hex printing behavior.
// The tests are designed to run without GoogleTest, using a lightweight custom harness.

#include <cassert>
#include <setjmp.h>
#include <vector>
#include <string.h>
#include <string>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <cstdlib>
#include <stdlib.h>
#include <functional>
#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <ctype.h>


// Step 1 & 2: Declare the focal function and its dependent type.
// We assume the typical project defines `uarb` as a pointer to unsigned int (array of limbs).
extern "C" {
typedef unsigned int *uarb;
 // Focal function under test
 void uarb_printx(uarb num, int digits, FILE *out);
}

// Helper: capture the output of uarb_printx into a std::string using a temporary file.
std::string capture_output(uarb num, int digits) {
    // Use a temporary file to capture the output of C code that writes to FILE*
    FILE *f = tmpfile();
    if (!f) {
        throw std::runtime_error("Failed to create temporary file for test output");
    }
    uarb_printx(num, digits, f);
    fflush(f);
    rewind(f);

    // Read all content
    std::string out;
    char buffer[1024];
    while (true) {
        size_t r = fread(buffer, 1, sizeof(buffer), f);
        if (r == 0) break;
        out.append(buffer, r);
    }
    fclose(f);
    return out;
}

// Lightweight test framework (non-terminating assertions)
struct TestCase {
    std::string name;
    std::function<bool()> run;
    std::string note;
};

// Utility to run all tests and print results
int main() {
    std::vector<TestCase> tests;

    // Test 1: Single limb non-zero should print exactly "0x<val>" (lowercase hex)
    tests.push_back({
        "single_limb_nonzero_prints_msb_only",
        []() -> bool {
            unsigned int limb[1] = { 0xABCD };
            uarb arr = limb;
            std::string out = capture_output(arr, 1);
            std::string expected = "0xabcd";
            if (out != expected) {
                std::cerr << "Expected: " << expected << ", Got: " << out << "\n";
                return false;
            }
            return true;
        },
        ""
    });

    // Test 2: Two limbs where MSB != 0 -> "0x<msb>" then lower limbs with 4-digit padding
    tests.push_back({
        "two_limbs_msb_nonzero_prints_msb_then_lsb_with_padding",
        []() -> bool {
            // arr[0] is least significant limb, arr[1] is most significant
            unsigned int limbs[2] = { 0x2345, 0x1 };
            uarb arr = limbs;
            std::string out = capture_output(arr, 2);
            std::string expected = "0x12345";
            if (out != expected) {
                std::cerr << "Expected: " << expected << ", Got: " << out << "\n";
                return false;
            }
            return true;
        },
        ""
    });

    // Test 3: Two limbs with MSB zero, LSB non-zero -> should print only the LSB (as per logic)
    tests.push_back({
        "two_limbs_msb_zero_prints_only_lsb",
        []() -> bool {
            unsigned int limbs[2] = { 0x0005, 0x0 };
            uarb arr = limbs;
            std::string out = capture_output(arr, 2);
            std::string expected = "0x5";
            if (out != expected) {
                std::cerr << "Expected: " << expected << ", Got: " << out << "\n";
                return false;
            }
            return true;
        },
        ""
    });

    // Test 4: All limbs zero -> should print "0x0" when the loop reaches digits == 0
    tests.push_back({
        "all_limbs_zero_prints_0x0",
        []() -> bool {
            unsigned int limbs[3] = { 0x0, 0x0, 0x0 };
            uarb arr = limbs;
            std::string out = capture_output(arr, 3);
            std::string expected = "0x0";
            if (out != expected) {
                std::cerr << "Expected: " << expected << ", Got: " << out << "\n";
                return false;
            }
            return true;
        },
        ""
    });

    // Test 5: Complex mix: MSB non-zero, mid zero, LSB zero -> "0x<msb>" + padded mid/lsb
    tests.push_back({
        "msb_nonzero_with_zero_mid_and_lsb_padding",
        []() -> bool {
            unsigned int limbs[3] = { 0x0, 0x5, 0x0 };
            uarb arr = limbs;
            std::string out = capture_output(arr, 3);
            std::string expected = "0x50000"; // "0x5" + "0000" from LSB
            if (out != expected) {
                std::cerr << "Expected: " << expected << ", Got: " << out << "\n";
                return false;
            }
            return true;
        },
        ""
    });

    // Test 6: MSB non-zero with all lower limbs non-zero to verify full multi-limb printing order
    tests.push_back({
        "full_three_limb_print_with_padding",
        []() -> bool {
            // arr[2] = 1, arr[1] = 0, arr[0] = 0x00FF
            unsigned int limbs[3] = { 0x00FF, 0x0, 0x1 };
            uarb arr = limbs;
            std::string out = capture_output(arr, 3);
            // Expect 0x1 (msb) then 0000 for middle, then 00ff for least significant
            std::string expected = "0x100000ff";
            if (out != expected) {
                std::cerr << "Expected: " << expected << ", Got: " << out << "\n";
                return false;
            }
            return true;
        },
        ""
    });

    // Run all tests
    int passed = 0;
    for (const auto &tc : tests) {
        bool ok = false;
        try {
            ok = tc.run();
        } catch (const std::exception &e) {
            std::cerr << "Test '" << tc.name << "' threw exception: " << e.what() << "\n";
            ok = false;
        } catch (...) {
            std::cerr << "Test '" << tc.name << "' threw an unknown exception.\n";
            ok = false;
        }
        std::cout << "[ " << (ok ? "PASS" : "FAIL") << " ] " << tc.name << "\n";
        if (ok) ++passed;
    }

    std::cout << "Summary: " << passed << "/" << tests.size() << " tests passed.\n";

    // Return non-zero if any test failed to indicate failure to build system scripts
    return (passed == (int)tests.size()) ? 0 : 1;
}
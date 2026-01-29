// Unit test suite for the focal method: standard_name (from pngvalid.c)
// This test suite is written in C++11 without GoogleTest.
// It uses a lightweight internal test harness that records failures and reports a summary.
// The tests target the behavior of standard_name, focusing on true/false branches
// of its conditional predicates as described in the focal method.
//
// How to run (conceptual):
//  - Build this test alongside the project sources, ensuring the C file containing
//    standard_name (pngvalid.c) is compiled and linked.
//  - The test harness calls the C function via C linkage (extern "C") and validates
//    the produced buffer content.
//
// Notes:
//  - We do not rely on private/internal details beyond the public signature of standard_name.
//  - The colour type names come from colour_types[colour_type] in the original file; the tests
//    validate the existence/structure of strings appended by standard_name without depending on
//    the exact colour name (except for specific branches like palette brackets and tRNS).
//  - The tests assume libpng-like macros (e.g., PNG_INTERLACE_NONE) are visible via <png.h>.

#include <algorithm>
#include <vector>
#include <signal.h>
#include <string.h>
#include <string>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <math.h>



// The focal function under test is declared with C linkage.
// It resides in the C source (pngvalid.c) and is linked together with this test.
extern "C" size_t standard_name(char *buffer, size_t bufsize, size_t pos,
                                png_byte colour_type, int bit_depth,
                                unsigned int npalette, int interlace_type,
                                png_uint_32 w, png_uint_32 h, int do_interlace);

// Lightweight test harness
namespace TestHarness {
    static std::vector<std::string> g_failures;

    static void log_failure(const std::string &msg) {
        g_failures.push_back(msg);
    }

    static void assert_contains(const std::string &buf, const std::string &substr, const std::string &desc) {
        if (buf.find(substr) == std::string::npos) {
            log_failure("ASSERT FAILED: " + desc + " - expected to find substring '" + substr +
                        "' in buffer: \"" + buf + "\"");
        }
    }

    static void assert_not_contains(const std::string &buf, const std::string &substr, const std::string &desc) {
        if (buf.find(substr) != std::string::npos) {
            log_failure("ASSERT FAILED: " + desc + " - did not expect substring '" +
                        substr + "' in buffer: \"" + buf + "\"");
        }
    }

    static void assert_true(bool cond, const std::string &desc) {
        if (!cond) {
            log_failure("ASSERT FAILED: " + desc);
        }
    }

    static void reset() {
        g_failures.clear();
    }

    static int summary() {
        if (g_failures.empty()) {
            std::cerr << "[TEST] All tests passed.\n";
            return 0;
        } else {
            std::cerr << "[TEST] " << g_failures.size() << " failure(s) detected:\n";
            for (const auto &f : g_failures) {
                std::cerr << "  - " << f << "\n";
            }
            return 1;
        }
    }

    // Helper to run standard_name with a fresh buffer and return its string output
    static std::string run_standard_name_with_params(
        png_byte colour_type, int bit_depth,
        unsigned int npalette, int interlace_type,
        png_uint_32 w, png_uint_32 h, int do_interlace)
    {
        const size_t BUF = 1024;
        char buffer[BUF];
        std::memset(buffer, 0, BUF);
        size_t pos = 0;

        // The function returns the updated position (length of the produced string)
        pos = standard_name(buffer, BUF, pos, colour_type, bit_depth,
                            npalette, interlace_type, w, h, do_interlace);

        // Ensure buffer is null-terminated in case safecat did not append a terminator
        buffer[BUF-1] = '\0';
        return std::string(buffer);
    }
}

// Tests

// 1) Palette case: colour_type == 3 must produce "[<npalette>]" after the colour name.
//    Also verify the " <bit> bit" segment exists and there is no interlace text when interlace_type is NONE.
void test_palette_brackets_presence() {
    using namespace TestHarness;
    reset();

    // colour_type 3 (palette), npalette 5, no interlace, width/height not requested
    std::string out = run_standard_name_with_params(/*colour_type=*/3, /*bit_depth=*/8,
                                                    /*npalette=*/5, /*interlace_type=*/PNG_INTERLACE_NONE,
                                                    /*w=*/0, /*h=*/0, /*do_interlace=*/0);

    // Expect a bracketed number "[5]" after the colour name
    bool has_bracket = (out.find("[") != std::string::npos) && (out.find("]") != std::string::npos);
    assert_true(has_bracket, "Palette path should include bracketed palette count [npalette]");

    // Ensure the inside of brackets is exactly "5"
    size_t l = out.find("[");
    size_t r = out.find("]", l);
    std::string inside = (l != std::string::npos && r != std::string::npos && r > l) ? out.substr(l+1, r-l-1) : "";
    assert_true(inside == "5", "Bracket content should be the palette count '5'");

    // Verify the " bit" segment exists with the correct bit depth (8)
    assert_contains(out, " 8 bit", "Bit depth segment should be '8 bit'");

    // Ensure no interlace text when interlace_type == NONE
    assert_not_contains(out, " interlaced", "There should be no interlace text when interlace_type is NONE");
}

// 2) Non-palette path with npalette != 0 should append "+tRNS" in the output.
void test_tRNS_appendion() {
    using namespace TestHarness;
    reset();

    // colour_type != 3, e.g., 0 (gray), with npalette != 0
    std::string out = run_standard_name_with_params(/*colour_type=*/0, /*bit_depth=*/8,
                                                    /*npalette=*/2, /*interlace_type=*/PNG_INTERLACE_NONE,
                                                    /*w=*/0, /*h=*/0, /*do_interlace=*/0);

    assert_contains(out, "+tRNS", "When npalette != 0 and colour_type != 3, output should contain '+tRNS'.");
}

// 3) Interlaced path with do_interlace = 1 should include "(pngvalid)" after "(interlaced)".
void test_interlaced_pngvalid_branch() {
    using namespace TestHarness;
    reset();

    // Interlaced with do_interlace true
    std::string out = run_standard_name_with_params(/*colour_type=*/0, /*bit_depth=*/8,
                                                    /*npalette=*/0, /*interlace_type=*/PNG_INTERLACE_ADAM7,
                                                    /*w=*/10, /*h=*/20, /*do_interlace=*/1);

    assert_contains(out, " interlaced", "Interlaced path should contain ' interlaced' text.");
    assert_contains(out, "(pngvalid)", "When do_interlace is true, should append '(pngvalid)'.");
}

// 4) Interlaced path with do_interlace = 0 should include "(libpng)".
void test_interlaced_libpng_branch() {
    using namespace TestHarness;
    reset();

    // Interlaced with do_interlace false
    std::string out = run_standard_name_with_params(/*colour_type=*/0, /*bit_depth=*/8,
                                                    /*npalette=*/0, /*interlace_type=*/PNG_INTERLACE_ADAM7,
                                                    /*w=*/12, /*h=*/34, /*do_interlace=*/0);

    assert_contains(out, " interlaced", "Interlaced path should contain ' interlaced' text.");
    assert_contains(out, "(libpng)", "When do_interlace is false, should append '(libpng)'.");
}

// 5) Width/Height formatting: verify " <width>x<height>" is appended when w>0 || h>0.
void test_width_height_formatting() {
    using namespace TestHarness;
    reset();

    std::string out = run_standard_name_with_params(/*colour_type=*/0, /*bit_depth=*/8,
                                                   /*npalette=*/0, /*interlace_type=*/PNG_INTERLACE_NONE,
                                                   /*w=*/800, /*h=*/600, /*do_interlace=*/0);

    assert_contains(out, " 800x600", "Width/Height formatting should appear as ' 800x600'.");
}

// 6) No palette brackets when colour_type == 3 but npalette == 0.
void test_no_palette_brackets_when_npalette_zero() {
    using namespace TestHarness;
    reset();

    std::string out = run_standard_name_with_params(/*colour_type=*/3, /*bit_depth=*/8,
                                                   /*npalette=*/0, /*interlace_type=*/PNG_INTERLACE_NONE,
                                                   /*w=*/0, /*h=*/0, /*do_interlace=*/0);

    // There should be no '[' or ']' since npalette == 0 for a palette colour type
    assert_not_contains(out, "[", "When npalette == 0 for colour_type == 3, there should be no bracketed palette count.");
    assert_not_contains(out, "]", "There should be no closing bracket when npalette == 0.");
}

// 7) Buffer must be non-empty to demonstrate basic content produced.
//    This also ensures the function does not crash with minimal inputs.
void test_basic_buffer_nonempty() {
    using namespace TestHarness;
    reset();

    std::string out = run_standard_name_with_params(/*colour_type=*/0, /*bit_depth=*/8,
                                                   /*npalette=*/0, /*interlace_type=*/PNG_INTERLACE_NONE,
                                                   /*w=*/0, /*h=*/0, /*do_interlace=*/0);

    assert_true(!out.empty(), "Output buffer should be non-empty for basic call.");
}

// Runner
int main() {
    // Execute individual tests
    test_palette_brackets_presence();
    test_tRNS_appendion();
    test_interlaced_pngvalid_branch();
    test_interlaced_libpng_branch();
    test_width_height_formatting();
    test_no_palette_brackets_when_npalette_zero();
    test_basic_buffer_nonempty();

    // Print and return results
    int rc = TestHarness::summary();
    return rc;
}
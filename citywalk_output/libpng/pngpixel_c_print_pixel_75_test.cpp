// C++11 based lightweight unit-test harness for the focal function print_pixel
// Note: This test suite uses a small, self-contained mock of the libpng-like API
// to drive the print_pixel function without requiring the real libpng library.
// The goal is to exercise the various color-type branches and related code paths
// in print_pixel, while keeping tests self-contained and executable with
// standard library facilities only.

// This file should be compiled together with the original pngpixel.c (the focal
// method under test). We provide minimal extern "C" declarations to link with
// print_pixel and the mocked PNG helpers implemented below.

#include <unistd.h>
#include <../../png.h>
#include <setjmp.h>
#include <vector>
#include <string>
#include <functional>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <stdexcept>
#include <stdio.h>
#include <cstring>


//////////////////////////////////////////
// Mocked PNG API (minimal subset)

using png_uint_32 = unsigned int;
typedef void* png_structp;
typedef void* png_infop;
typedef const unsigned char* png_const_bytep;
typedef unsigned char* png_bytep;
typedef struct { unsigned char red, green, blue; } png_color;
typedef png_color* png_colorp;
#define PNG_INFO_PLTE 1
#define PNG_INFO_tRNS 2

// Extern C bridge for the focal function and helpers
extern "C" {
    // Focal function under test
    void print_pixel(png_structp png_ptr, png_infop info_ptr, png_const_bytep row,
                     png_uint_32 x);

    // PNG API stubs used by the focal method
    unsigned int png_get_bit_depth(png_structp png_ptr, png_infop info_ptr);
    int  png_get_color_type(png_structp png_ptr, png_infop info_ptr);
    int  png_get_PLTE(png_structp png_ptr, png_infop info_ptr,
                      png_colorp* palette, int* num_palette);
    int  png_get_tRNS(png_structp png_ptr, png_infop info_ptr,
                      png_bytep* trans_alpha, int* num_trans,
                      void**); // donor parameter unused in tests
    void png_error(png_ptr, const char* err);
}

// Global test context to drive mock behavior
struct TestContext {
    unsigned int bit_depth = 8;
    int color_type = 0; // will be set to one of PNG_COLOR_TYPE_*
    bool has_plte = false;
    int  num_palette = 0;
    png_color palette[256] = {};
    bool has_tRNS = false;
    int  num_trans = 0;
    unsigned char trans[256] = {};
};

// Provide a single global test context accessible by mocks
static TestContext g_tc;

// Simple palette accessor pointer management for png_get_PLTE
static png_colorp g_plte_ptr = nullptr;

// Helper: reset context for a clean test
static void reset_context() {
    g_tc = TestContext();
    g_plte_ptr = nullptr;
}

// Mocks
unsigned int png_get_bit_depth(png_structp /*png_ptr*/, png_infop /*info_ptr*/) {
    return g_tc.bit_depth;
}

int png_get_color_type(png_structp /*png_ptr*/, png_infop /*info_ptr*/) {
    return g_tc.color_type;
}

int png_get_PLTE(png_structp /*png_ptr*/, png_infop /*info_ptr*/,
                 png_colorp* palette, int* num_palette) {
    if (g_tc.has_plte && g_tc.num_palette > 0) {
        *palette = g_tc.palette;      // array of colors
        *num_palette = g_tc.num_palette;
        g_plte_ptr = *palette;
        return PNG_INFO_PLTE;
    } else {
        *palette = nullptr;
        *num_palette = 0;
        g_plte_ptr = nullptr;
        return 0;
    }
}

int png_get_tRNS(png_structp /*png_ptr*/, png_infop /*info_ptr*/,
                 png_bytep* trans_alpha, int* num_trans,
                 void** /*ignore*/) {
    if (g_tc.has_tRNS && g_tc.num_trans > 0) {
        *trans_alpha = g_tc.trans;
        *num_trans = g_tc.num_trans;
        return PNG_INFO_tRNS;
    } else {
        *trans_alpha = nullptr;
        *num_trans = 0;
        return 0;
    }
}

void png_error(png_structp /*png_ptr*/, const char* /*err*/) {
    // Emulate libpng error by throwing to allow test to catch.
    throw std::runtime_error("png_error invoked");
}

// We deliberately do not provide a real main here; tests will call print_pixel
// via a small harness that includes this file.

//////////////////////////////////////////
// Test harness: capture stdout for a given test invocation

// Helper to capture stdout produced by a callable
static std::string capture_stdout(const std::function<void()> &fn) {
    // POSIX pipe-based capture
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        throw std::runtime_error("Failed to create pipe for stdout redirection");
    }

    // Flush and duplicate stdout
    fflush(stdout);
    int saved_stdout = dup(fileno(stdout));
    if (saved_stdout == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        throw std::runtime_error("Failed to dup stdout");
    }

    // Redirect stdout to pipe's write end
    if (dup2(pipefd[1], fileno(stdout)) == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        close(saved_stdout);
        throw std::runtime_error("Failed to redirect stdout");
    }
    // Now close the original write end; stdout points to pipe
    close(pipefd[1]);

    // Execute the function that prints
    fn();

    // Ensure all output is flushed
    fflush(stdout);

    // Restore original stdout
    if (dup2(saved_stdout, fileno(stdout)) == -1) {
        close(pipefd[0]);
        close(saved_stdout);
        throw std::runtime_error("Failed to restore stdout");
    }
    close(saved_stdout);

    // Read from the pipe
    std::string output;
    const size_t BUFSZ = 4096;
    char buf[BUFSZ];
    ssize_t n;
    // The read end may need non-blocking wait for the writer; since we closed
    // writer, there should be data available immediately.
    while ((n = read(pipefd[0], buf, BUFSZ)) > 0) {
        output.append(buf, static_cast<size_t>(n));
    }
    close(pipefd[0]);
    return output;
}

//////////////////////////////////////////
// Test helpers and utilities

static int g_failures = 0;

// Simple non-terminating assertion macro
#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << (msg) << " (at " << __FILE__ << ":" << __LINE__ << ")\n"; \
        ++g_failures; \
    } \
} while (0)

#define EXPECT_SUBSTRING(haystack, needle, msg) do { \
    if ((haystack).find((needle)) == std::string::npos) { \
        std::cerr << "EXPECT_SUBSTRING failed: " << (msg) \
                  << "  - couldn't find '" << (needle) << "' in '" << (haystack) \
                  << "' (at " << __FILE__ << ":" << __LINE__ << ")\n"; \
        ++g_failures; \
    } \
} while (0)

static unsigned char dummy_row[8] = {0x00}; // arbitrary content; component() logic resides in PNG lib

// Helpers to run individual tests with descriptive comments
static void test_gray_branch() {
    // Test that a GRAY color type prints the Gray prefix
    reset_context();
    g_tc.bit_depth = 8;
    g_tc.color_type = PNG_COLOR_TYPE_GRAY;
    g_tc.has_plte = false;
    g_tc.has_tRNS = false;

    std::string out = capture_stdout([&]() {
        // Call the focal function with dummy pointers
        print_pixel(nullptr, nullptr, dummy_row, 0);
    });

    // Expect the Gray branch: "GRAY <value>\n"
    // We don't rely on the numeric component value, only the prefix
    EXPECT_TRUE(out.size() > 0, "Gray branch should print something");
    EXPECT_TRUE(out.rfind("GRAY ", 0) == 0, "Gray branch should start with 'GRAY '");
}

static void test_palette_no_plte() {
    // Test when color type is PALETTE but there is no PLTE data
    reset_context();
    g_tc.bit_depth = 8;
    g_tc.color_type = PNG_COLOR_TYPE_PALETTE;
    g_tc.has_plte = false;

    std::string out = capture_stdout([&]() {
        print_pixel(nullptr, nullptr, dummy_row, 0);
    });

    EXPECT_TRUE(out.size() > 0, "Palette (no PLTE) should print something");
    EXPECT_TRUE(out.rfind("INDEXED ", 0) == 0, "Palette path should print an INDEXED line");
    EXPECT_SUBSTRING(out, "invalid index", "Palette with no PLTE should indicate invalid index");
}

static void test_palette_with_plte_no_tRNS() {
    // Palette present, but no transparency data
    reset_context();
    g_tc.bit_depth = 8;
    g_tc.color_type = PNG_COLOR_TYPE_PALETTE;
    g_tc.has_plte = true;
    g_tc.has_tRNS = false;
    g_tc.num_palette = 1;
    g_tc.palette[0] = { 10, 20, 30 };

    std::string out = capture_stdout([&]() {
        print_pixel(nullptr, nullptr, dummy_row, 0);
    });

    EXPECT_TRUE(out.size() > 0, "Palette with PLTE should print something");
    EXPECT_TRUE(out.rfind("INDEXED ", 0) == 0, "Expected line to start with INDEXED");
    // We can't rely on exact numeric values due to row/component behavior;
    // ensure that the palette color 10 20 30 appears in the output.
    EXPECT_SUBSTRING(out, "10", "Output should include palette red component 10");
    EXPECT_SUBSTRING(out, "20", "Output should include palette green component 20");
    EXPECT_SUBSTRING(out, "30", "Output should include palette blue component 30");
}

static void test_palette_with_plte_and_tRNS() {
    // Palette present with tRNS (transparency) values
    reset_context();
    g_tc.bit_depth = 8;
    g_tc.color_type = PNG_COLOR_TYPE_PALETTE;
    g_tc.has_plte = true;
    g_tc.has_tRNS = true;
    g_tc.num_palette = 1;
    g_tc.palette[0] = { 15, 25, 35 };
    g_tc.num_trans = 1;
    g_tc.trans[0] = 128;

    std::string out = capture_stdout([&]() {
        print_pixel(nullptr, nullptr, dummy_row, 0);
    });

    EXPECT_TRUE(out.size() > 0, "Palette with PLTE and tRNS should print something");
    EXPECT_TRUE(out.rfind("INDEXED ", 0) == 0, "Expected line to start with INDEXED");
    EXPECT_SUBSTRING(out, "15", "Output should include palette red component 15");
    EXPECT_SUBSTRING(out, "25", "Output should include palette green component 25");
    EXPECT_SUBSTRING(out, "35", "Output should include palette blue component 35");
    EXPECT_SUBSTRING(out, "128", "Output should include tRNS alpha value 128");
}

static void test_rgb_branch() {
    // Test that a RGB color type prints the RGB prefix
    reset_context();
    g_tc.bit_depth = 8;
    g_tc.color_type = PNG_COLOR_TYPE_RGB;
    g_tc.has_plte = false;

    std::string out = capture_stdout([&]() {
        print_pixel(nullptr, nullptr, dummy_row, 0);
    });

    EXPECT_TRUE(out.size() > 0, "RGB branch should print something");
    EXPECT_TRUE(out.rfind("RGB ", 0) == 0, "RGB branch should start with 'RGB '");
}

static void test_gray_alpha_branch() {
    // Test that a Gray+Alpha color type prints the Gray+Alpha prefix
    reset_context();
    g_tc.bit_depth = 8;
    g_tc.color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
    g_tc.has_plte = false;

    std::string out = capture_stdout([&]() {
        print_pixel(nullptr, nullptr, dummy_row, 0);
    });

    EXPECT_TRUE(out.size() > 0, "Gray+Alpha branch should print something");
    EXPECT_TRUE(out.rfind("GRAY+ALPHA ", 0) == 0, "GRAY+ALPHA branch should print correct prefix");
}

static void test_rgb_alpha_branch() {
    // Test that an RGB+Alpha color type prints the RGBA prefix
    reset_context();
    g_tc.bit_depth = 8;
    g_tc.color_type = PNG_COLOR_TYPE_RGB_ALPHA;
    g_tc.has_plte = false;

    std::string out = capture_stdout([&]() {
        print_pixel(nullptr, nullptr, dummy_row, 0);
    });

    EXPECT_TRUE(out.size() > 0, "RGB+Alpha branch should print something");
    EXPECT_TRUE(out.rfind("RGBA ", 0) == 0, "RGBA branch should start with 'RGBA '");
}

static void test_default_branch_trigger() {
    // This test aims to hit the default branch which triggers png_error.
    // We expect an exception to be thrown; we catch it to mark test as passed.
    reset_context();
    g_tc.bit_depth = 8;
    g_tc.color_type = 999; // invalid color type to force default case

    try {
        capture_stdout([&]() {
            print_pixel(nullptr, nullptr, dummy_row, 0);
        });
        // If no exception, that's a failure for this test
        std::cerr << "Default branch did not trigger error as expected\n";
        ++g_failures;
    } catch (const std::exception &ex) {
        // Expected path: png_error should throw
        (void)ex;
    }
}

//////////////////////////////////////////
// Test runner entry point

static void run_all_tests() {
    test_gray_branch();
    test_palette_no_plte();
    test_palette_with_plte_no_tRNS();
    test_palette_with_plte_and_tRNS();
    test_rgb_branch();
    test_gray_alpha_branch();
    test_rgb_alpha_branch();
    test_default_branch_trigger();

    // Summary
    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
    } else {
        std::cout << g_failures << " test(s) failed.\n";
    }
}

//////////////////////////////////////////
// Main
int main() {
    // Run all tests
    run_all_tests();
    return g_failures == 0 ? 0 : 1;
}

/*
Explanation of test strategy and coverage goals:

- Gray branch test verifies that when color type is PNG_COLOR_TYPE_GRAY, the
  output begins with the expected prefix "GRAY ". It does not rely on the exact
  numeric value produced by component(), which is implementation-defined.

- Palette tests:
  - test_palette_no_plte ensures the code handles a PALETTE color type without a PLTE
    entry by printing an "INDEXED 0 = invalid index" path (robust to the index value).
  - test_palette_with_plte_no_tRNS verifies the "INDEXED ..." line with palette RGB values
    when a PLTE is present but no tRNS data.
  - test_palette_with_plte_and_tRNS covers the path where tRNS exists and the alpha column
    is printed, validating both color components and the transparency value.

- RGB, Gray+Alpha, and RGB+Alpha tests assert the correct prefix is printed for those
  color types (e.g., RGB, GRAY+ALPHA, RGBA) without depending on precise component values.

- Default branch test attempts to trigger the invalid color type path which should lead to
  png_error being invoked. The test expects an exception to be thrown and marks failure
  if not.

- All tests are non-terminating (they do not exit on failures) and use a lightweight
  EXPECT_TRUE/EXPECT_SUBSTRING mechanism to accumulate failures and print a final summary.

- The tests rely on a mock of the PNG API to drive print_pixel's behavior without a full
  libpng dependency. They are designed to be portable to a C++11 environment and
  to be executed from main as required when GTest is not available.

- Important: This harness assumes that the project under test provides a compatible
  implementation of print_pixel in pngpixel.c and that it links with the mocks defined
  here. The exact numeric values printed by component() are not asserted; only the
  branch prefixes and the presence of key substrings are validated to maximize
  coverage across color types.
*/
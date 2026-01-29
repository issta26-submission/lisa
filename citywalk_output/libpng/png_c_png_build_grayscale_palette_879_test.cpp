/*
   Unit test suite for the focal method:
   png_build_grayscale_palette(int bit_depth, png_colorp palette)

   Notes:
   - This test suite is designed to be compiled as C++11 code and linked
     against the C implementation that provides png_build_grayscale_palette
     (as declared below).
   - We avoid GTest and implement a lightweight test harness that continues
     execution even if a test fails, allowing multiple tests to run in one pass.
   - We replicate minimal necessary PNG-related types locally to avoid
     depending on external headers. The tests exercise true/false branches of
     the function's internal predicates and verify correct memory writes.
   - The tests cover:
     - bit_depth in {1,2,4,8} and expected grayscale palettes
     - default/invalid bit_depth branches (no palette modification)
     - NULL palette handling (early return)
     - boundary behavior (only first num_palette entries are written)
*/

#include <cstring>
#include <iostream>
#include <pngpriv.h>
#include <vector>


// Minimal local definitions to match the expected public API used by png_build_grayscale_palette
typedef unsigned char png_byte;
typedef struct png_color_s {
    png_byte red;
    png_byte green;
    png_byte blue;
} png_color, *png_colorp;

// Declaration of the focal C function with C linkage for compatibility in C++
extern "C" void png_build_grayscale_palette(int bit_depth, png_colorp palette);

// Simple test harness
static int g_failures = 0;
static void log_fail(const std::string& msg) {
    ++g_failures;
    std::cerr << "FAIL: " << msg << std::endl;
}
static void log_pass(const std::string& msg) {
    std::cout << "PASS: " << msg << std::endl;
}

// Helpers to map bit_depth to expected palette parameters
static void get_params_for_depth(int bit_depth, int &num_palette, int &color_inc) {
    switch (bit_depth) {
        case 1:
            num_palette = 2;
            color_inc = 0xff; // 255
            break;
        case 2:
            num_palette = 4;
            color_inc = 0x55; // 85
            break;
        case 4:
            num_palette = 16;
            color_inc = 0x11; // 17
            break;
        case 8:
            num_palette = 256;
            color_inc = 1;    // 1
            break;
        default:
            num_palette = 0;
            color_inc = 0;
            break;
    }
}

// Test 1: bit_depth = 1 should fill exactly two grayscale entries: 0 and 255
// The first palette entry should be (0,0,0); second should be (255,255,255)
static void test_bit_depth_1() {
    const int bit_depth = 1;
    int num_palette, color_inc;
    get_params_for_depth(bit_depth, num_palette, color_inc);
    std::vector<png_color> pal(256); // allocate enough space for safety
    // Initialize with a sentinel value to detect unintended modifications
    for (size_t i = 0; i < pal.size(); ++i) {
        pal[i].red = pal[i].green = pal[i].blue = 0xAA;
    }

    // Call the focal function
    png_build_grayscale_palette(bit_depth, pal.data());

    // Expected: pal[0] = (0,0,0); pal[1] = (255,255,255)
    if (pal[0].red == 0 && pal[0].green == 0 && pal[0].blue == 0 &&
        pal[1].red == 255 && pal[1].green == 255 && pal[1].blue == 255) {
        log_pass("test_bit_depth_1: correct grayscale palette for depth 1");
    } else {
        log_fail("test_bit_depth_1: incorrect palette values or count");
        // Provide detailed diagnostics
        std::cerr << "  pal[0]="
                  << int(pal[0].red) << "," << int(pal[0].green) << "," << int(pal[0].blue)
                  << " pal[1]="
                  << int(pal[1].red) << "," << int(pal[1].green) << "," << int(pal[1].blue)
                  << std::endl;
    }
}

// Test 2: bit_depth = 2 should fill four entries: 0, 85, 170, 255
// All channels should be equal and follow the (i * 85) & 0xff pattern
static void test_bit_depth_2() {
    const int bit_depth = 2;
    int num_palette, color_inc;
    get_params_for_depth(bit_depth, num_palette, color_inc);
    std::vector<png_color> pal(256);
    for (size_t i = 0; i < pal.size(); ++i) {
        pal[i].red = pal[i].green = pal[i].blue = 0xAA;
    }

    png_build_grayscale_palette(bit_depth, pal.data());

    bool ok = true;
    for (int i = 0; i < num_palette; ++i) {
        int expected = (i * color_inc) & 0xff;
        if (!(pal[i].red == expected && pal[i].green == expected && pal[i].blue == expected)) {
            ok = false;
            std::cerr << "  Mismatch at i=" << i
                      << " got=(" << int(pal[i].red) << "," << int(pal[i].green) << "," << int(pal[i].blue)
                      << ") expected=(" << expected << "," << expected << "," << expected << ")\n";
            break;
        }
    }
    if (ok) log_pass("test_bit_depth_2: correct grayscale palette for depth 2");
    else log_fail("test_bit_depth_2: incorrect values detected");
}

// Test 3: bit_depth = 4 should fill 16 entries with increments of 17
// Expected sequence: 0,17,34,51,68,85,102,119,136,153,170,187,204,221,238,255
static void test_bit_depth_4() {
    const int bit_depth = 4;
    int num_palette, color_inc;
    get_params_for_depth(bit_depth, num_palette, color_inc);
    std::vector<png_color> pal(256);
    for (size_t i = 0; i < pal.size(); ++i) {
        pal[i].red = pal[i].green = pal[i].blue = 0xAA;
    }

    png_build_grayscale_palette(bit_depth, pal.data());

    bool ok = true;
    for (int i = 0; i < num_palette; ++i) {
        int expected = (i * color_inc) & 0xff;
        if (!(pal[i].red == expected && pal[i].green == expected && pal[i].blue == expected)) {
            ok = false;
            std::cerr << "  Mismatch at i=" << i
                      << " got=(" << int(pal[i].red) << "," << int(pal[i].green) << "," << int(pal[i].blue)
                      << ") expected=(" << expected << "," << expected << "," << expected << ")\n";
            break;
        }
    }
    if (ok) log_pass("test_bit_depth_4: correct grayscale palette for depth 4");
    else log_fail("test_bit_depth_4: incorrect values detected");
}

// Test 4: bit_depth = 8 should fill 256 entries with values 0..255
// Verify a handful of representative indices to ensure proper behavior
static void test_bit_depth_8() {
    const int bit_depth = 8;
    int num_palette, color_inc;
    get_params_for_depth(bit_depth, num_palette, color_inc); // 256 and 1
    std::vector<png_color> pal(256);
    for (size_t i = 0; i < pal.size(); ++i) {
        pal[i].red = pal[i].green = pal[i].blue = 0xAA;
    }

    png_build_grayscale_palette(bit_depth, pal.data());

    // Representative checks
    struct Check {
        int idx;
        int expected;
    } checks[] = {
        {0, 0},
        {1, 1},
        {2, 2},
        {10, 10},
        {128, 128},
        {255, 255}
    };
    bool ok = true;
    for (const auto &c : checks) {
        if (!(pal[c.idx].red == c.expected && pal[c.idx].green == c.expected && pal[c.idx].blue == c.expected)) {
            ok = false;
            std::cerr << "  Mismatch at i=" << c.idx
                      << " got=(" << int(pal[c.idx].red) << "," << int(pal[c.idx].green) << "," << int(pal[c.idx].blue)
                      << ") expected=(" << c.expected << "," << c.expected << "," << c.expected << ")\n";
            break;
        }
    }
    if (ok) log_pass("test_bit_depth_8: correct grayscale palette for depth 8");
    else log_fail("test_bit_depth_8: incorrect values detected");
}

// Test 5: Invalid depth (default branch) should not modify the palette
static void test_invalid_depth_no_modification() {
    const int bit_depth = 3; // not a supported depth
    int num_palette, color_inc;
    get_params_for_depth(bit_depth, num_palette, color_inc); // 0, 0
    std::vector<png_color> pal(16);
    for (size_t i = 0; i < pal.size(); ++i) {
        pal[i].red = pal[i].green = pal[i].blue = static_cast<png_byte>(0x55);
    }

    // Copy original for comparison
    std::vector<png_color> pal_before = pal;

    png_build_grayscale_palette(bit_depth, pal.data());

    // Expect no changes
    bool ok = (pal == pal_before);
    if (ok) log_pass("test_invalid_depth_no_modification: no palette changes for invalid depth");
    else {
        log_fail("test_invalid_depth_no_modification: palette was modified for invalid depth");
        // Optional diagnostic
        for (size_t i = 0; i < pal.size(); ++i) {
            if (pal[i].red != pal_before[i].red ||
                pal[i].green != pal_before[i].green ||
                pal[i].blue != pal_before[i].blue) {
                std::cerr << "  Difference at index " << i
                          << " after call: (" << int(pal[i].red) << "," << int(pal[i].green) << "," << int(pal[i].blue)
                          << ") vs before: (" << int(pal_before[i].red) << "," << int(pal_before[i].green) << "," << int(pal_before[i].blue) << ")\n";
                break;
            }
        }
    }
}

// Test 6: NULL palette should cause early return without crash
static void test_null_palette_returns_without_crash() {
    // This simply ensures the function handles NULL gracefully
    png_build_grayscale_palette(8, nullptr);
    log_pass("test_null_palette_returns_without_crash: function safely handles NULL palette");
}

// Boundary test: ensure only first num_palette entries are touched (sanity check)
static void test_boundary_writes_only_expected_entries() {
    const int bit_depth = 2; // num_palette = 4
    int num_palette, color_inc;
    get_params_for_depth(bit_depth, num_palette, color_inc);

    // Prepare a palette with sufficient size and sentinel values
    std::vector<png_color> pal(8);
    for (size_t i = 0; i < pal.size(); ++i) {
        pal[i].red = pal[i].green = pal[i].blue = 0xAA;
    }

    png_build_grayscale_palette(bit_depth, pal.data());

    // Check that only indices 0..3 were modified
    bool ok = true;
    for (int i = 0; i < num_palette; ++i) {
        int expected = (i * color_inc) & 0xff;
        if (!(pal[i].red == expected && pal[i].green == expected && pal[i].blue == expected)) {
            ok = false;
            std::cerr << "  Boundary fail at i=" << i
                      << " got=(" << int(pal[i].red) << "," << int(pal[i].green) << "," << int(pal[i].blue)
                      << ") expected=(" << expected << "," << expected << "," << expected << ")\n";
            break;
        }
    }
    // Ensure indices beyond num_palette were not modified
    for (size_t i = (size_t)num_palette; i < pal.size(); ++i) {
        if (pal[i].red != 0xAA || pal[i].green != 0xAA || pal[i].blue != 0xAA) {
            ok = false;
            std::cerr << "  Boundary tail modified at i=" << i
                      << " value=(" << int(pal[i].red) << "," << int(pal[i].green) << "," << int(pal[i].blue) << ")\n";
            break;
        }
    }

    if (ok) log_pass("test_boundary_writes_only_expected_entries: boundary behavior correct");
    else log_fail("test_boundary_writes_only_expected_entries: boundary behavior failed");
}

// Main runner
int main() {
    // Step 3: Execute tests (ordered for clarity)
    test_bit_depth_1();
    test_bit_depth_2();
    test_bit_depth_4();
    test_bit_depth_8();
    test_invalid_depth_no_modification();
    test_null_palette_returns_without_crash();
    test_boundary_writes_only_expected_entries();

    // Summary
    if (g_failures == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) FAILED." << std::endl;
        return 1;
    }
}
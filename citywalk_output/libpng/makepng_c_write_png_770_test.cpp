// C++11 test suite for the focal function write_png in makepng.c
// This test harness invokes the C function directly from C++,
// exercising different code paths by varying color_type, bit_depth,
// small, tRNS, gamma values, and colors array.
// It uses a lightweight, non-terminating assertion approach (no GTest/GMock).
// The tests verify that a valid PNG file is produced (signature check)
// and that the function returns success (0).

#include <stdio.h>
#include <math.h>
#include <cstdint>
#include <vector>
#include <stddef.h>
#include <fstream>
#include <assert.h>
#include <cstdlib>
#include <cstdio>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <png.h>
#include <string.h>
#include <string>
#include <zlib.h>
#include <iostream>
#include <cstring>
#include <ctype.h>


// Forward declaration of the C function under test.
// The actual implementation resides in makepng.c, which should be linked.
// We declare a compatible prototype here for linking.
extern "C" {
    // Opaque struct for chunk_insert (definition is in makepng.c).
    typedef struct chunk_insert chunk_insert;

    // The focal function under test.
    int write_png(const char **name, FILE *fp, int color_type, int bit_depth,
                  volatile int gamma, chunk_insert * volatile insert,
                  unsigned int filters, unsigned int *colors, int small, int tRNS);
}

// Forward declaration for libpng types to ensure prototype compatibility.
// We use the libpng types that the function expects (as defined by include in makepng.c).
// We rely on the actual libpng headers being available during compilation/linking.

// Simple helper: delete a file if it exists
static void remove_file_if_exists(const std::string &path) {
    if (!path.empty()) {
        std::remove(path.c_str());
    }
}

// Simple helper: create a unique temporary file path for output
static std::string make_temp_png_path() {
    // Use a simple approach with tmpnam for portability in test context
    char tmpl[] = "makepng_test_XXXXXX.png";
#if defined(_WIN32)
    int fd = _mktemp(tmpl);
    (void)fd; // ignore - _mktemp already creates unique name in-place
#else
    // mkstemp creates and opens a unique file; but we need a path only.
    // We'll use tmpnam as a fallback to obtain a unique path and append ".png".
    char tmp[L_tmpnam];
    if (tmpnam(tmp) == nullptr) {
        // Fallback string
        return "makepng_test.png";
    }
    snprintf(tmpl, sizeof(tmpl), "%s", tmp);
    // Ensure PNG extension
    std::string p = std::string(tmpl) + std::string("_makepng.png");
    return p;
#endif
    // On some platforms tmpnam is not thread-safe; fallback:
    return std::string(tmpl);
}

// Helper: verify that the produced file starts with PNG signature
static bool has_png_signature(const std::string &path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return false;
    unsigned char sig[8];
    in.read(reinterpret_cast<char*>(sig), 8);
    if (in.gcount() != 8) return false;
    const unsigned char expected[8] = {0x89,'P','N','G',0x0D,0x0A,0x1A,0x0A};
    return std::memcmp(sig, expected, 8) == 0;
}

// Test helper macro: non-terminating checks
#define TCHECK(cond, msg) do { if(!(cond)) { \
    std::cerr << "[FAIL] " << __func__ << ": " << msg << "\n"; \
    g_failed++; \
} else { \
    /* Optional verbose success could be here */ \
} } while(0)

static int g_failed = 0; // per-test failure counter
static int g_total = 0;  // per-test total checks (optional)

// Test 1: Basic RGB PNG write (color_type=PNG_COLOR_TYPE_RGB, bit_depth=8, small=0, tRNS=0)
static int test_basic_rgb_png_write() {
    g_failed = 0; g_total = 0;
    const char *name_arr[] = { nullptr };
    // Create a temporary file path
    std::string path = make_temp_png_path();
    FILE *fp = std::fopen(path.c_str(), "wb");
    TCHECK(fp != nullptr, "Failed to open output file for basic RGB test");
    if (!fp) { remove_file_if_exists(path); return 1; }
    // Colors array (dummy values, 0-initialized)
    unsigned int colors[256] = {0};

    int ret = write_png(name_arr, fp,
                        PNG_COLOR_TYPE_RGB, 8,
                        0, NULL, 0,
                        colors, 0, 0);
    std::fclose(fp);

    TCHECK(ret == 0, "write_png returned non-zero for RGB basic test");
    // Check that file was created and contains PNG signature
    TCHECK(ret == 0 && has_png_signature(path),
           "Output file is not a valid PNG for RGB basic test");
    // Clean up
    remove_file_if_exists(path);
    // Update totals
    g_total++;
    return (g_failed == 0) ? 0 : 1;
}

// Test 2: Palette-based PNG write (color_type=PNG_COLOR_TYPE_PALETTE, bit_depth=8)
static int test_palette_png_write() {
    g_failed = 0; g_total = 0;
    const char *name_arr[] = { nullptr };
    std::string path = make_temp_png_path();
    FILE *fp = std::fopen(path.c_str(), "wb");
    TCHECK(fp != nullptr, "Failed to open output file for palette test");
    if (!fp) { remove_file_if_exists(path); return 1; }
    // 256 palette indices (colors array length)
    unsigned int colors[256];
    for (int i = 0; i < 256; ++i) colors[i] = 0;

    int ret = write_png(name_arr, fp,
                        PNG_COLOR_TYPE_PALETTE, 8,
                        0, NULL, 0,
                        colors, 0, 0);
    std::fclose(fp);

    TCHECK(ret == 0, "write_png returned non-zero for palette test");
    TCHECK(ret == 0 && has_png_signature(path),
           "Output file is not a valid PNG for palette test");
    remove_file_if_exists(path);
    g_total++;
    return (g_failed == 0) ? 0 : 1;
}

// Test 3: tRNS path for non-paletted image (alpha/transparent color)
static int test_trns_path() {
    g_failed = 0; g_total = 0;
    const char *name_arr[] = { nullptr };
    std::string path = make_temp_png_path();
    FILE *fp = std::fopen(path.c_str(), "wb");
    TCHECK(fp != nullptr, "Failed to open output file for tRNS test");
    if (!fp) { remove_file_if_exists(path); return 1; }
    unsigned int colors[256] = {0};

    int ret = write_png(name_arr, fp,
                        PNG_COLOR_TYPE_RGB, 8,
                        0, NULL, 0,
                        colors, 0, 1); // tRNS set
    std::fclose(fp);

    TCHECK(ret == 0, "write_png returned non-zero for tRNS test");
    TCHECK(ret == 0 && has_png_signature(path),
           "Output file is not a valid PNG for tRNS test");
    remove_file_if_exists(path);
    g_total++;
    return (g_failed == 0) ? 0 : 1;
}

// Test 4: Small mode RGB with gamma MAC18 to exercise gamma conversion path
static int test_small_rgb_gamma_mac18() {
    g_failed = 0; g_total = 0;
    const char *name_arr[] = { nullptr };
    std::string path = make_temp_png_path();
    FILE *fp = std::fopen(path.c_str(), "wb");
    TCHECK(fp != nullptr, "Failed to open output file for small RGB gamma MAC18 test");
    if (!fp) { remove_file_if_exists(path); return 1; }
    unsigned int colors[256] = {0};

    // gamma MAC 18
    int ret = write_png(name_arr, fp,
                        PNG_COLOR_TYPE_RGB, 8,
                        PNG_GAMMA_MAC_18, NULL, 0,
                        colors, 1, 0);
    std::fclose(fp);

    TCHECK(ret == 0, "write_png returned non-zero for small RGB gamma MAC18 test");
    TCHECK(ret == 0 && has_png_signature(path),
           "Output file is not a valid PNG for small RGB gamma MAC18 test");
    remove_file_if_exists(path);
    g_total++;
    return (g_failed == 0) ? 0 : 1;
}

// Test 5: Small mode with gamma FP conversion path (gamma between 1 and 999)
static int test_small_rgb_gamma_fp() {
    g_failed = 0; g_total = 0;
    const char *name_arr[] = { nullptr };
    std::string path = make_temp_png_path();
    FILE *fp = std::fopen(path.c_str(), "wb");
    TCHECK(fp != nullptr, "Failed to open output file for small RGB gamma FP test");
    if (!fp) { remove_file_if_exists(path); return 1; }
    unsigned int colors[256] = {0};

    // gamma in (1,999) should be mapped to PNG_FP_1 internally
    int ret = write_png(name_arr, fp,
                        PNG_COLOR_TYPE_RGB, 8,
                        500, NULL, 0,
                        colors, 1, 0);
    std::fclose(fp);

    TCHECK(ret == 0, "write_png returned non-zero for small RGB gamma FP test");
    TCHECK(ret == 0 && has_png_signature(path),
           "Output file is not a valid PNG for small RGB gamma FP test");
    remove_file_if_exists(path);
    g_total++;
    return (g_failed == 0) ? 0 : 1;
}

// Helper: run all tests and report summary
static void run_all_tests() {
    int failures = 0;
    int total = 0;

    auto accumulate = [&failures, &total](int r) { total++; if (r != 0) failures++; };

    accumulate(test_basic_rgb_png_write());
    accumulate(test_palette_png_write());
    accumulate(test_trns_path());
    accumulate(test_small_rgb_gamma_mac18());
    accumulate(test_small_rgb_gamma_fp());

    // Summary
    if (failures == 0) {
        std::cout << "[OK] All tests passed (" << total << " tests).\n";
    } else {
        std::cout << "[WARN] Tests finished with " << failures << " failure(s) out of "
                  << total << " tests.\n";
    }
}

// Main: run tests
int main() {
    // Note: The tests assume the project compiles with C linkage for write_png
    // and libpng is available at link time.

    // Run the test suite
    run_all_tests();

    return 0;
}
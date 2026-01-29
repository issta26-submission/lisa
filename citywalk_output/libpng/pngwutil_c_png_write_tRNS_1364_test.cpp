// Note: This is a standalone, self-contained C++11 unit-test-like harness
// that mimics the essential behavior of the focal function png_write_tRNS
// from pngwutil.c in order to enable deterministic unit testing without
// depending on the full libpng build environment.
// The test suite covers all branches of the focal method: PALETTE, GRAY,
// RGB, and the "alpha channel" fallback, including boundary/invalid cases.
// The implementation intentionally keeps the environment minimal while
// preserving the logic paths exercised by png_write_tRNS.

#include <cstdint>
#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Domain: Basic types and constants that mirror a tiny subset of libpng types.
// This is a self-contained mock to exercise the focal logic without external deps.

typedef unsigned char png_byte;
typedef png_byte* png_bytep;
typedef const png_byte* png_const_bytep;
typedef uint32_t png_uint_32;
typedef uint16_t png_uint_16;
typedef struct png_color_16 {
    png_uint_16 gray;
    png_uint_16 red;
    png_uint_16 green;
    png_uint_16 blue;
} png_color_16, *png_color_16p;
typedef const png_color_16* png_const_color_16p;
typedef struct png_struct_def* png_structrp;

// Color type constants (subset from libpng)
#define PNG_COLOR_TYPE_GRAY    0
#define PNG_COLOR_TYPE_RGB     2
#define PNG_COLOR_TYPE_PALETTE 3

// Simulate 16-bit write support macro (as seen in the original code)
#define PNG_WRITE_16BIT_SUPPORTED 1

// Chunk name for tRNS (stand-in value; tests compare against this symbol)
static const png_uint_32 png_tRNS = 0x74524E53; // ASCII 't' 'R' 'N' 'S'

// Minimal, test-only state to emulate libpng behavior inside tests
struct TestPngState {
    int num_palette;
    int bit_depth;
};

// Logging helpers to verify behavior without aborting on failures
static std::vector<std::vector<unsigned char>> g_written_chunks; // each entry is raw chunk data
static std::vector<uint32_t> g_written_chunk_names;
static std::vector<std::string> g_warnings;
static void g_clear_logs() {
    g_written_chunks.clear();
    g_written_chunk_names.clear();
    g_warnings.clear();
}
static void g_png_debug(int /*level*/, const char* /*msg*/) {
    // no-op for tests
}
static void g_png_app_warning(png_structrp /*png_ptr*/, const char* msg) {
    g_warnings.push_back(std::string(msg));
}
static void g_png_write_complete_chunk(png_structrp /*png_ptr*/, png_uint_32 chunk_name,
                                       png_const_bytep data, size_t length) {
    // copy data to log
    std::vector<unsigned char> v;
    if (data && length > 0) {
        v.assign(data, data + length);
    }
    g_written_chunk_names.push_back(chunk_name);
    g_written_chunks.push_back(std::move(v));
}
static void g_png_save_uint_16(png_bytep buf, unsigned int i) {
    buf[0] = (png_byte)((i >> 8) & 0xFF);
    buf[1] = (png_byte)(i & 0xFF);
}
static void g_png_priv_dummy() {}

// Forward declaration of the focal-like function under test.
// In this self-contained test, we implement a faithful copy of the original
// logic using the simplified types above.
static void png_write_tRNS(png_structrp png_ptr,
                          png_const_bytep trans_alpha,
                          png_const_color_16p tran,
                          int num_trans,
                          int color_type);

// Implementation of the focal-like function (self-contained)
static void png_write_tRNS(png_structrp png_ptr,
                          png_const_bytep trans_alpha,
                          png_const_color_16p tran,
                          int num_trans,
                          int color_type)
{
    (void)trans_alpha;
    (void)tran;
    {
        png_byte buf[6];
        g_png_debug(1, "in png_write_tRNS");
        if (color_type == PNG_COLOR_TYPE_PALETTE)
        {
            if (num_trans <= 0 || num_trans > (int)png_ptr->num_palette)
            {
                g_png_app_warning(png_ptr,
                    "Invalid number of transparent colors specified");
                return;
            }
            /* Write the chunk out as it is */
            g_png_write_complete_chunk(png_ptr, png_tRNS, trans_alpha,
                (size_t)num_trans);
        }
        else if (color_type == PNG_COLOR_TYPE_GRAY)
        {
            /* One 16-bit value */
            if (tran->gray >= (1 << png_ptr->bit_depth))
            {
                g_png_app_warning(png_ptr,
                    "Ignoring attempt to write tRNS chunk out-of-range for bit_depth");
                return;
            }
            g_png_save_uint_16(buf, tran->gray);
            g_png_write_complete_chunk(png_ptr, png_tRNS, buf, 2);
        }
        else if (color_type == PNG_COLOR_TYPE_RGB)
        {
            /* Three 16-bit values */
            g_png_save_uint_16(buf, tran->red);
            g_png_save_uint_16(buf + 2, tran->green);
            g_png_save_uint_16(buf + 4, tran->blue);
#if defined(PNG_WRITE_16BIT_SUPPORTED)
            if (png_ptr->bit_depth == 8 && (buf[0] | buf[2] | buf[4]) != 0)
#else
            if ((buf[0] | buf[2] | buf[4]) != 0)
#endif
            {
                g_png_app_warning(png_ptr,
                    "Ignoring attempt to write 16-bit tRNS chunk when bit_depth is 8");
                return;
            }
            g_png_write_complete_chunk(png_ptr, png_tRNS, buf, 6);
        }
        else
        {
            g_png_app_warning(png_ptr, "Can't write tRNS with an alpha channel");
        }
    }
}

// Simple test harness helpers
static void assert_true(bool cond, const std::string& msg) {
    if (!cond) {
        std::cerr << "ASSERT FAILED: " << msg << "\n";
    }
}

// Test 1: Palette path with valid num_trans writes correct data and no warnings
static void test_palette_valid() {
    g_clear_logs();
    TestPngState st;
    st.num_palette = 5;
    st.bit_depth = 8;
    png_structrp ptr = &st;

    unsigned char trans_alpha[3] = { 0x01, 0x02, 0x03 };
    png_write_tRNS(ptr,
                  trans_alpha,
                  nullptr, // tran not used for palette path
                  3,
                  PNG_COLOR_TYPE_PALETTE);

    assert_true(g_warnings.empty(), "Palette valid: no warnings expected");
    assert_true(g_written_chunks.size() == 1, "Palette valid: one chunk expected");
    if (!g_warnings.empty()) {
        std::cerr << "Warnings: ";
        for (auto &w : g_warnings) std::cerr << w << " | ";
        std::cerr << "\n";
    }
    if (g_written_chunk_names.size() == 1) {
        assert_true(g_written_chunk_names[0] == png_tRNS, "Palette valid: chunk name matches png_tRNS");
        assert_true(g_written_chunks[0].size() == 3, "Palette valid: chunk length == 3");
        if (g_written_chunks[0].size() == 3) {
            if (std::memcmp(g_written_chunks[0].data(), trans_alpha, 3) != 0) {
                std::cerr << "Palette valid: chunk data mismatch\n";
            }
        }
    } else {
        std::cerr << "Palette valid: unexpected number of chunks: " << g_written_chunk_names.size() << "\n";
    }
}

// Test 2: Palette path with invalid num_trans (<1) should warn and not write chunk
static void test_palette_invalid_num_trans() {
    g_clear_logs();
    TestPngState st;
    st.num_palette = 5;
    st.bit_depth = 8;
    png_structrp ptr = &st;

    unsigned char trans_alpha[3] = { 0xAA, 0xBB, 0xCC };
    png_write_tRNS(ptr,
                  trans_alpha,
                  nullptr,
                  0, // invalid
                  PNG_COLOR_TYPE_PALETTE);

    assert_true(!g_warnings.empty(), "Palette invalid: at least one warning expected");
    bool found = false;
    for (const auto& w : g_warnings) {
        if (w.find("Invalid number of transparent colors specified") != std::string::npos) {
            found = true;
            break;
        }
    }
    assert_true(found, "Palette invalid: expected invalid number warning present");
    assert_true(g_written_chunks.empty(), "Palette invalid: no chunk should be written");
}

// Test 3: Gray path within range writes two-byte value
static void test_gray_in_range() {
    g_clear_logs();
    TestPngState st;
    st.num_palette = 0;
    st.bit_depth = 8;
    png_color_16 tran;
    tran.gray = 100;
    png_structrp ptr = &st;

    png_write_tRNS(ptr,
                  nullptr,
                  &tran,
                  1,
                  PNG_COLOR_TYPE_GRAY);

    assert_true(g_warnings.empty(), "Gray in-range: no warnings expected");
    assert_true(g_written_chunks.size() == 1, "Gray in-range: one chunk expected");
    if (g_written_chunks.size() == 1) {
        // Data should be [0x00, 0x64]
        unsigned char expect[2] = { 0x00, 0x64 };
        assert_true(g_written_chunks[0].size() == 2, "Gray in-range: chunk length == 2");
        if (g_written_chunks[0].size() == 2) {
            if (std::memcmp(g_written_chunks[0].data(), expect, 2) != 0) {
                std::cerr << "Gray in-range: chunk data mismatch\n";
            }
        }
    }
}

// Test 4: Gray path out of range triggers a warning and no chunk
static void test_gray_out_of_range() {
    g_clear_logs();
    TestPngState st;
    st.num_palette = 0;
    st.bit_depth = 8;
    png_color_16 tran;
    tran.gray = 300; // >= 256
    png_structrp ptr = &st;

    png_write_tRNS(ptr,
                  nullptr,
                  &tran,
                  1,
                  PNG_COLOR_TYPE_GRAY);

    assert_true(!g_warnings.empty(), "Gray out-of-range: at least one warning expected");
    bool found = false;
    for (const auto& w : g_warnings) {
        if (w.find("Ignoring attempt to write tRNS chunk out-of-range for bit_depth") != std::string::npos) {
            found = true;
            break;
        }
    }
    assert_true(found, "Gray out-of-range: expected range warning present");
    assert_true(g_written_chunks.empty(), "Gray out-of-range: no chunk should be written");
}

// Test 5: RGB path with nonzero MSB in any component causes a warning (when bit_depth == 8 and PNG_WRITE_16BIT_SUPPORTED)
static void test_rgb_nonzero_msb_triggers_warning() {
    g_clear_logs();
    TestPngState st;
    st.num_palette = 0;
    st.bit_depth = 8;
    png_color_16 tran;
    tran.red = 256;   // 0x0100 -> MSB non-zero
    tran.green = 0;
    tran.blue = 0;
    png_structrp ptr = &st;

    png_write_tRNS(ptr,
                  nullptr,
                  &tran,
                  1,
                  PNG_COLOR_TYPE_RGB);

    assert_true(!g_warnings.empty(), "RGB nonzero MSB: at least one warning expected");
    bool found = false;
    for (const auto& w : g_warnings) {
        if (w.find("Ignoring attempt to write 16-bit tRNS chunk when bit_depth is 8") != std::string::npos) {
            found = true;
            break;
        }
    }
    assert_true(found, "RGB nonzero MSB: expected 16-bit write warning");
    assert_true(g_written_chunks.empty(), "RGB nonzero MSB: no chunk should be written");
}

// Test 6: RGB path with all zero components writes a 6-byte chunk
static void test_rgb_all_zero_writes_chunk() {
    g_clear_logs();
    TestPngState st;
    st.num_palette = 0;
    st.bit_depth = 8;
    png_color_16 tran;
    tran.red = 0;
    tran.green = 0;
    tran.blue = 0;
    png_structrp ptr = &st;

    png_write_tRNS(ptr,
                  nullptr,
                  &tran,
                  1,
                  PNG_COLOR_TYPE_RGB);

    assert_true(g_warnings.empty(), "RGB all zero: no warnings expected");
    assert_true(g_written_chunks.size() == 1, "RGB all zero: one chunk expected");
    if (g_written_chunks.size() == 1) {
        assert_true(g_written_chunks[0].size() == 6, "RGB all zero: chunk length == 6");
        unsigned char expect[6] = {0,0, 0,0, 0,0};
        if (std::memcmp(g_written_chunks[0].data(), expect, 6) != 0) {
            std::cerr << "RGB all zero: chunk data mismatch\n";
        }
    }
}

// Test 7: Other color type (alpha channel) path should warn and not write chunk
static void test_other_color_type_warns_for_alpha() {
    g_clear_logs();
    TestPngState st;
    st.num_palette = 0;
    st.bit_depth = 8;
    png_color_16 tran;
    tran.gray = 0;
    png_structrp ptr = &st;

    // Use a color_type that represents "with alpha" in this subset, e.g., 4
    png_write_tRNS(ptr,
                  nullptr,
                  &tran,
                  1,
                  4); // not Gray/RGB/Palette in our subset

    assert_true(!g_warnings.empty(), "Other color type: at least one warning expected");
    bool found = false;
    for (const auto& w : g_warnings) {
        if (w.find("Can't write tRNS with an alpha channel") != std::string::npos) {
            found = true;
            break;
        }
    }
    assert_true(found, "Other color type: expected can't write tRNS warning");
    assert_true(g_written_chunks.empty(), "Other color type: no chunk should be written");
}

int main() {
    // Run the test scenarios
    test_palette_valid();
    test_palette_invalid_num_trans();
    test_gray_in_range();
    test_gray_out_of_range();
    test_rgb_nonzero_msb_triggers_warning();
    test_rgb_all_zero_writes_chunk();
    test_other_color_type_warns_for_alpha();

    // Simple summary
    // Note: We do not rely on a testing framework; we report failures to std::cerr.
    // If you want more structured output, you can redirect stdout/stderr accordingly.

    std::cout << "Finished png_write_tRNS tests (standalone harness).\n";
    return 0;
}
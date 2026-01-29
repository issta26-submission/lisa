// Test suite for png_write_info (libpng-inspired) using a lightweight C++11 test harness
// This test suite does not rely on GTest/GMock and uses manual EXPECT-like macros.
// It provides minimal stubs to exercise specific branches of png_write_info as given in the focal code.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <stdexcept>
#include <cstring>


// Domain-specific macros to enable/disable specific code paths inside the focal method.
// We enable the most relevant paths for unit testing (PLTE and tRNS/invert alpha).
#define PNG_INFO_PLTE 0x01
#define PNG_INFO_tRNS 0x02
#define PNG_COLOR_TYPE_PALETTE 3
#define PNG_MAX_PALETTE_LENGTH 256
#define PNG_INVERT_ALPHA 0x01

#define PNG_WRITE_tRNS_SUPPORTED 1
#define PNG_WRITE_INVERT_ALPHA_SUPPORTED 1
// Note: We don't enable every possible PNG_WRITE_* macro to keep tests focused.

typedef unsigned char png_byte;
typedef unsigned int png_uint_32;
typedef unsigned int png_uint_16;

// Forward declarations for helper stubs (the real library would provide these)
struct png_struct;
struct png_info;
typedef struct png_struct* png_structrp;
typedef struct png_info* png_inforp;
typedef const struct png_info* png_const_inforp;
typedef png_byte* png_bytep;

// Mock state to observe behavior of the focal function
struct MockState {
    bool plt_written;
    void* plt_palette;
    png_uint_32 plt_num;

    bool trns_written;
    png_bytep trans_alpha_ptr;
    unsigned int num_trans;
    int color_type_seen;

    bool error_raised;
    std::string error_msg;

    bool before_PLTE_seen;
    MockState() { reset(); }
    void reset() {
        plt_written = false; plt_palette = nullptr; plt_num = 0;
        trns_written = false; trans_alpha_ptr = nullptr; num_trans = 0;
        color_type_seen = -1;
        error_raised = false; error_msg.clear();
        before_PLTE_seen = false;
    }
} g_mock;

// Forward declarations for the focal function and its dependencies (stubbed)
void png_debug(int /*level*/, const char* /*message*/) { /* no-op in tests */ }

void png_error(png_structrp /*png_ptr*/, const char* msg) {
    // Simulate libpng's error handling by throwing to indicate a test-failure path
    throw std::runtime_error(msg ? msg : "png_error invoked");
}

void png_write_info_before_PLTE(png_structrp /*png_ptr*/, png_const_inforp /*info_ptr*/) {
    // Mark that the before_PLTE hook ran (for potential assertions)
    g_mock.before_PLTE_seen = true;
}

void png_write_PLTE(png_structrp /*png_ptr*/, void* palette, png_uint_32 num_palette) {
    g_mock.plt_written = true;
    g_mock.plt_palette = palette;
    g_mock.plt_num = num_palette;
}

void png_write_tRNS(png_structrp /*png_ptr*/, png_bytep trans_alpha,
                  png_const_inforp /*trans_color_ptr*/, unsigned int num_trans,
                  int color_type) {
    g_mock.trns_written = true;
    g_mock.trans_alpha_ptr = trans_alpha;
    g_mock.num_trans = num_trans;
    g_mock.color_type_seen = color_type;
}

// The following stubs exist to satisfy conditional compilation blocks in the focal method.
// They won't be invoked in our targeted tests but are provided for completeness.
void png_write_bKGD(png_structrp /*png_ptr*/, void* /*background*/, int /*color_type*/) {}
void png_write_eXIf(png_structrp /*png_ptr*/, void* /*exif*/, unsigned int /*num_exif*/) {}
void png_write_hIST(png_structrp /*png_ptr*/, void* /*hist*/, unsigned int /*num_palette*/) {}
void png_write_oFFs(png_structrp /*png_ptr*/, int /*x_offset*/, int /*y_offset*/, int /*offset_unit_type*/) {}
void png_write_pCAL(png_structrp /*png_ptr*/, int /*pcal_purpose*/, double /*pcal_X0*/, double /*pcal_X1*/,
                    int /*pcal_type*/, int /*pcal_nparams*/, void* /*pcal_units*/, void* /*pcal_params*/) {}
void png_write_sCAL_s(png_structrp /*png_ptr*/, int /*scal_unit*/, double /*scal_s_width*/, double /*scal_s_height*/) {}
void png_write_pHYs(png_structrp /*png_ptr*/, int /*x*/, int /*y*/, int /*unit*/) {}
void png_write_tIME(png_structrp /*png_ptr*/, void* /*mod_time*/) {}

void write_unknown_chunks(png_structrp /*png_ptr*/, png_const_inforp /*info_ptr*/, unsigned int /*where*/) {}

// Minimal structure definitions to support png_write_info's logic
struct png_struct {
    unsigned transformations;
    unsigned mode;
};

struct png_info {
    unsigned valid;
    int color_type;
    void* palette;
    unsigned num_palette;
    unsigned num_trans;
    png_bytep trans_alpha;
    void* trans_color;
#ifdef PNG_WRITE_tRNS_SUPPORTED
    unsigned int dummy_tRNS;
#endif
};

// Typedefs to match code usage
typedef struct png_struct png_struct;
typedef struct png_info png_info;
typedef png_info* png_inforp;
typedef const png_info* png_const_inforp;

// The focal function under test (adapted to compile in this isolated test harness)
void png_write_info(png_structrp png_ptr, png_const_inforp info_ptr)
{
    // Step 1: Diagnostics and null checks
    png_debug(1, "in png_write_info");
    if (png_ptr == nullptr || info_ptr == nullptr)
        return;

    // Step 2: PLTE handling
    png_write_info_before_PLTE(png_ptr, info_ptr);
    if ((info_ptr->valid & PNG_INFO_PLTE) != 0)
        png_write_PLTE(png_ptr, info_ptr->palette, (png_uint_32)info_ptr->num_palette);
    else if (info_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
        png_error(png_ptr, "Valid palette required for paletted images");

    // Step 3: tRNS handling
#ifdef PNG_WRITE_tRNS_SUPPORTED
    if ((info_ptr->valid & PNG_INFO_tRNS) != 0)
    {
#ifdef PNG_WRITE_INVERT_ALPHA_SUPPORTED
        /* Invert the alpha channel (in tRNS) */
        if ((png_ptr->transformations & PNG_INVERT_ALPHA) != 0 &&
            info_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
        {
            int j, jend;
            jend = info_ptr->num_trans;
            if (jend > PNG_MAX_PALETTE_LENGTH)
                jend = PNG_MAX_PALETTE_LENGTH;
            for (j = 0; j < jend; ++j)
                info_ptr->trans_alpha[j] = (png_byte)(255 - info_ptr->trans_alpha[j]);
        }
#endif
        png_write_tRNS(png_ptr, info_ptr->trans_alpha, &(info_ptr->trans_color),
            info_ptr->num_trans, info_ptr->color_type);
    }
#endif

#ifdef PNG_WRITE_bKGD_SUPPORTED
    if ((info_ptr->valid & PNG_INFO_bKGD) != 0)
        png_write_bKGD(png_ptr, &(info_ptr->background), info_ptr->color_type);
#endif

#ifdef PNG_WRITE_eXIf_SUPPORTED
    if ((info_ptr->valid & PNG_INFO_eXIf) != 0)
    {
        png_write_eXIf(png_ptr, info_ptr->exif, info_ptr->num_exif);
        png_ptr->mode |= 0; // PNG_WROTE_eXIf placeholder
    }
#endif

#ifdef PNG_WRITE_hIST_SUPPORTED
    if ((info_ptr->valid & PNG_INFO_hIST) != 0)
        png_write_hIST(png_ptr, info_ptr->hist, info_ptr->num_palette);
#endif

#ifdef PNG_WRITE_oFFs_SUPPORTED
    if ((info_ptr->valid & PNG_INFO_oFFs) != 0)
        png_write_oFFs(png_ptr, info_ptr->x_offset, info_ptr->y_offset,
            info_ptr->offset_unit_type);
#endif

#ifdef PNG_WRITE_pHYs_SUPPORTED
    if ((info_ptr->valid & PNG_INFO_pHYs) != 0)
        png_write_pHYs(png_ptr, info_ptr->x_pixels_per_unit,
            info_ptr->y_pixels_per_unit, info_ptr->phys_unit_type);
#endif

#ifdef PNG_WRITE_tIME_SUPPORTED
    if ((info_ptr->valid & PNG_INFO_tIME) != 0)
    {
        png_write_tIME(png_ptr, &(info_ptr->mod_time));
        png_ptr->mode |= 0; // PNG_WROTE_tIME placeholder
    }
#endif

#ifdef PNG_WRITE_sPLT_SUPPORTED
    if ((info_ptr->valid & PNG_INFO_sPLT) != 0)
        for (int i = 0; i < (int)info_ptr->splt_palettes_num; i++)
            png_write_sPLT(png_ptr, info_ptr->splt_palettes + i);
#endif

#ifdef PNG_WRITE_TEXT_SUPPORTED
    /* We won't write header text chunks in this focused test */
#endif

#ifdef PNG_WRITE_UNKNOWN_CHUNKS_SUPPORTED
    write_unknown_chunks(png_ptr, info_ptr, 0);
#endif
}

// Simple assertion helpers (non-terminating) to allow multiple tests in a single run
static int g_failures = 0;
#define EXPECT_TRUE(cond) do { if(!(cond)) { std::cerr << "EXPECT_TRUE failed: " << #cond << " at " << __FILE__ << ":" << __LINE__ << std::endl; ++g_failures; } } while(0)
#define EXPECT_EQ(a, b) do { if((a) != (b)) { std::cerr << "EXPECT_EQ failed: " << #a << " != " << #b << " (" << (a) << " != " << (b) << ") at " << __FILE__ << ":" << __LINE__ << std::endl; ++g_failures; } } while(0)
#define EXPECT_THROW(func) \
    try { func; EXPECT_TRUE(false && "Expected exception not thrown"); } \
    catch (...) { EXPECT_TRUE(true && "Caught expected exception"); }

// Test 1: Palette path writes PLTE when PLTE flag is set
void test_plte_path() {
    g_mock.reset();
    // Prepare png_ptr and info_ptr
    png_structpng: ; // no effect, placeholder to avoid compiler warning in some toolchains

    png_structrp png_ptr = new png_struct{0,0};
    png_inforp info_ptr = new png_info{0, 0, nullptr, 0, 0, nullptr, nullptr};

    // Setup PLTE scenario
    info_ptr->valid = PNG_INFO_PLTE;
    info_ptr->color_type = 0;
    unsigned char palette[4] = { 1, 2, 3, 4 };
    info_ptr->palette = palette;
    info_ptr->num_palette = 2;

    // Call png_write_info
    try {
        png_write_info(png_ptr, info_ptr);
    } catch (...) {
        std::cerr << "Unexpected exception in test_plte_path" << std::endl;
        g_failures++;
    }

    // Validate that PLTE path was invoked
    EXPECT_TRUE(g_mock.plt_written);
    EXPECT_EQ(g_mock.plt_num, (png_uint_32)2);

    delete png_ptr;
    delete info_ptr;
}

// Test 2: Paletted color type with no PLTE should raise an error
void test_missing_palette_error() {
    g_mock.reset();
    png_structrp png_ptr = new png_struct{0,0};
    png_inforp info_ptr = new png_info{0, 0, nullptr, 0, 0, nullptr, nullptr};

    // No PLTE flag, but color_type indicates PALETTE => should error
    info_ptr->valid = 0; // PLTE not present
    info_ptr->color_type = PNG_COLOR_TYPE_PALETTE;
    info_ptr->palette = nullptr;

    bool caught = false;
    try {
        png_write_info(png_ptr, info_ptr);
    } catch (const std::exception& e) {
        caught = true;
        // Expected
    }
    EXPECT_TRUE(caught);

    delete png_ptr;
    delete info_ptr;
}

// Test 3: tRNS with invert alpha should invert trans_alpha array
void test_invert_alpha_path() {
    g_mock.reset();
    // Prepare structures
    png_structrp png_ptr = new png_struct{0,0};
    png_inforp info_ptr = new png_info{0, 0, nullptr, 0, 0, nullptr, nullptr};

    // Initialize tRNS scenario
    info_ptr->valid = PNG_INFO_tRNS;
    info_ptr->color_type = PNG_COLOR_TYPE_PALETTE;
    info_ptr->num_trans = 3;
    unsigned char tRNS_alpha[3] = { 10, 20, 30 };
    info_ptr->trans_alpha = tRNS_alpha;
    info_ptr->trans_color = nullptr;

    // Set up palette (not used for tRNS but required for function path)
    info_ptr->palette = nullptr;
    info_ptr->num_palette = 0;

    // Enable invert alpha path via png_ptr->transformations
    png_ptr->transformations = PNG_INVERT_ALPHA;

    // Call function
    try {
        png_write_info(png_ptr, info_ptr);
    } catch (...) {
        std::cerr << "Unexpected exception in test_invert_alpha_path" << std::endl;
        g_failures++;
    }

    // Expect inversion on first 3 entries
    EXPECT_EQ(info_ptr->trans_alpha[0], 245);
    EXPECT_EQ(info_ptr->trans_alpha[1], 235);
    EXPECT_EQ(info_ptr->trans_alpha[2], 225);

    // Also ensure tRNS was written
    EXPECT_TRUE(g_mock.trns_written);
    EXPECT_EQ((int)g_mock.color_type_seen, info_ptr->color_type);

    delete png_ptr;
    delete info_ptr;
}

int main() {
    // Run tests
    test_plte_path();
    test_missing_palette_error();
    test_invert_alpha_path();

    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}

// Note: This test harness focuses on key logical branches of png_write_info:
// - PLTE path when PNG_INFO_PLTE is set
// - Paletted image without valid PLTE triggers png_error
// - tRNS path with PERMITTED INVERT_ALPHA transformation inverts alpha values
// It uses minimal stubs for library functions and simple non-terminating assertions for coverage.
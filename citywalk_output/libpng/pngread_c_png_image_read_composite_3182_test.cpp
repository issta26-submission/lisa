// Comprehensive C++11 unit test suite for the focal method png_image_read_composite
// This test suite mocks the necessary libpng-like structures and functions
// to exercise the core logic of png_image_read_composite without requiring
// the real libpng dependencies. The tests cover multiple code paths as described.

#include <cstdint>
#include <cassert>
#include <vector>
#include <iostream>
#include <pngpriv.h>
#include <stdexcept>
#include <cstring>
#include <cstddef>


// Domain-specific notes adapted for this test harness:
// - We implement minimal mocks for types/macros used by png_image_read_composite.
// - We exercise NONE interlace path, grayscale/color modes, alpha handling (255, <255, and 0).
// - We test the unknown interlace default branch (error path).
// - We avoid dependencies on external libraries; only standard C++ is used.

// ---------------- Mocked libpng-like types and helpers ----------------

// Basic types
typedef void* png_voidp;
typedef unsigned char png_byte;
typedef png_byte* png_bytep;
typedef const png_byte* png_const_bytep;
typedef std::ptrdiff_t ptrdiff_t;
typedef uint32_t png_uint_32;
typedef uint16_t png_uint_16;

// Forward declarations for lightweight mocks
struct png_struct;
struct png_image;
struct png_image_opaque;
struct png_image_read_control;

// Pointer/type aliases used by the focal method
typedef png_struct* png_structrp;   // simplified pointer alias
typedef png_image*  png_imagep;

// Macro-like helpers (simple stand-ins)
#define png_voidcast(type, value) (static_cast<type>(value))

// Interlace types (simplified)
#define PNG_INTERLACE_NONE 0
#define PNG_INTERLACE_ADAM7 1

// Format flag (color vs grayscale)
#define PNG_FORMAT_FLAG_COLOR 0x1

// Alpha optimization flag
#define PNG_FLAG_OPTIMIZE_ALPHA 0x1

// Adam7 pass helpers (not needed for our NONE path; defined to satisfy usage)
#define PNG_PASS_COLS(width, pass) ((width) > 0 ? 1 : 0)
#define PNG_PASS_START_COL(pass) 0
#define PNG_PASS_COL_OFFSET(pass) 0
#define PNG_PASS_START_ROW(pass) 0
#define PNG_PASS_ROW_OFFSET(pass) 0

// Error signaling (mock)
void png_error(png_structrp png_ptr, const char* msg)
{
    (void)png_ptr;
    throw std::runtime_error(msg ? msg : "png_error");
}

// A lightweight sRGB conversion placeholder (not exercised heavily in tests)
static const png_uint_16 png_sRGB_table[256] = {0};
static inline png_uint_32 PNG_sRGB_FROM_LINEAR(png_uint_32 v)
{
    // Placeholder: real code uses a sophisticated conversion; we keep it simple.
    return (v & 0xFF);
}

// ---------------- Data structures corresponding to the focal method ----------------

struct png_image_opaque {
    png_structrp png_ptr;
};

struct png_image {
    int height;
    int width;
    unsigned int format;
    png_image_opaque* opaque;
};

// png_image_read_control structure used by the focal method
struct png_image_read_control {
    png_imagep image;
    ptrdiff_t row_bytes;
    void* local_row;
    void* first_row;
};

// ---------------- Test harness global context and PNG-like reader ----------------

// Global test context to feed per-row data into png_read_row
struct TestContext {
    int width;
    int height;
    int channels;              // 1 (grayscale) or 3 (color)
    unsigned int format;         // PNG_FORMAT_FLAG_COLOR or 0
    // Per-row input data: for each pixel, [channels] bytes + 1 alpha byte
    std::vector<uint8_t> per_row_in;  // length = width * (channels + 1)

    // Background/output buffer (one row) length = width * channels
    std::vector<uint8_t> first_row_buf;
    // Next input row to supply (for this test suite, single-row height is used)
};

// Static context pointer for the mock png_read_row
static TestContext* g_ctx = nullptr;

// Mocked per-row reader: fills the input buffer with preconfigured per-row data
void png_read_row(png_structrp /*png_ptr*/, png_bytep row, png_bytep /*dsp_row*/)
{
    if (!g_ctx) return;
    // Copy the per-row input into the provided row buffer
    size_t need = static_cast<size_t>(g_ctx->width) * static_cast<size_t>(g_ctx->channels + 1);
    if (need > 0) {
        std::memcpy(row, g_ctx->per_row_in.data(), need);
    }
}

// Helper to initialize test environment for a given test case
static void init_test_environment(png_structrp png_ptr,
                                png_image_read_control& display,
                                png_image& image,
                                int width, int height, int channels, unsigned int format,
                                uint8_t const* per_row_in_bytes, size_t per_row_len,
                                uint8_t* first_row_out, size_t first_row_len)
{
    // Setup image struct
    image.height = height;
    image.width  = width;
    image.format = format;
    image.opaque = new png_image_opaque;
    static png_struct local_png_ptr;
    // The focal method reads image->opaque->png_ptr
    image.opaque->png_ptr = &local_png_ptr;
    // Setup display/control struct
    display.image = &image;
    display.row_bytes = static_cast<ptrdiff_t>(width * channels);
    display.local_row = nullptr; // will be set below
    display.first_row = first_row_out;
    // Setup context used by png_read_row
    TestContext ctx;
    ctx.width = width;
    ctx.height = height;
    ctx.channels = channels;
    ctx.format = format;
    ctx.per_row_in.assign(per_row_len ? per_row_in_bytes : nullptr, per_row_in_bytes ? per_row_in_bytes + per_row_len : nullptr);
    // If per_row_in is provided, total length should be width*(channels+1)
    if (per_row_len && per_row_len < static_cast<size_t>(width * (channels + 1))) {
        // pad with zeros if needed
        ctx.per_row_in.resize(width * (channels + 1), 0);
    }
    // Build first_row background/output
    ctx.first_row_buf.assign(first_row_len, 0);
    // Link global context for the mocks
    // Note: We pass the address of ctx to g_ctx and use its data in mocks
    // Since png_image_read_composite will reside in the same translation unit, we can set a static pointer
    // For simplicity, copy ctx to a static storage
    static TestContext* s_ctx = nullptr;
    if (s_ctx) delete s_ctx;
    s_ctx = new TestContext(std::move(ctx));
    g_ctx = s_ctx;
    // Provide the row buffer to the display
    display.local_row = (void*)g_ctx->per_row_in.data();
    // The first_row pointer (output buffer) uses g_ctx data
    // Note: After test, we will free s_ctx
    // Finally, assign a temporary cast for png_ptr to satisfy the type usage
    (void)png_ptr;
}

// ---------------- FOCAL METHOD (PNG image composite) ----------------

// We reproduce the focal method under test here, adapted to our mocks
png_byte png_image_read_composite(png_voidp argument)
{
    {
        png_image_read_control *display = png_voidcast(png_image_read_control*,
            argument);
        png_imagep image = display->image;
        png_structrp png_ptr = image->opaque->png_ptr;
        int passes;
        switch (png_ptr->interlaced)
        {
            case PNG_INTERLACE_NONE:
                passes = 1;
                break;
            case PNG_INTERLACE_ADAM7:
                passes = PNG_INTERLACE_ADAM7_PASSES;
                break;
            default:
                png_error(png_ptr, "unknown interlace type");
        }
        {
            png_uint_32 height = image->height;
            png_uint_32 width = image->width;
            ptrdiff_t step_row = display->row_bytes;
            unsigned int channels =
                (image->format & PNG_FORMAT_FLAG_COLOR) != 0 ? 3 : 1;
            int optimize_alpha = (png_ptr->flags & PNG_FLAG_OPTIMIZE_ALPHA) != 0;
            int pass;
            for (pass = 0; pass < passes; ++pass)
            {
                unsigned int startx, stepx, stepy;
                png_uint_32 y;
                if (png_ptr->interlaced == PNG_INTERLACE_ADAM7)
                {
                    /* The row may be empty for a short image: */
                    if (PNG_PASS_COLS(width, pass) == 0)
                        continue;
                    startx = PNG_PASS_START_COL(pass) * channels;
                    stepx = PNG_PASS_COL_OFFSET(pass) * channels;
                    y = PNG_PASS_START_ROW(pass);
                    stepy = PNG_PASS_ROW_OFFSET(pass);
                }
                else
                {
                    y = 0;
                    startx = 0;
                    stepx = channels;
                    stepy = 1;
                }
                for (; y<height; y += stepy)
                {
                    png_bytep inrow = png_voidcast(png_bytep, display->local_row);
                    png_bytep outrow;
                    png_const_bytep end_row;
                    /* Read the row, which is packed: */
                    png_read_row(png_ptr, inrow, NULL);
                    outrow = png_voidcast(png_bytep, display->first_row);
                    outrow += y * step_row;
                    end_row = outrow + width * channels;
                    /* Now do the composition on each pixel in this row. */
                    outrow += startx;
                    for (; outrow < end_row; outrow += stepx)
                    {
                        png_byte alpha = inrow[channels];
                        if (alpha > 0) /* else no change to the output */
                        {
                            unsigned int c;
                            for (c=0; c<channels; ++c)
                            {
                                png_uint_32 component = inrow[c];
                                if (alpha < 255) /* else just use component */
                                {
                                    if (optimize_alpha != 0)
                                    {
                                        /* This is PNG_OPTIMIZED_ALPHA, the component value
                                         * is a linear 8-bit value.  Combine this with the
                                         * current outrow[c] value which is sRGB encoded.
                                         * Arithmetic here is 16-bits to preserve the output
                                         * values correctly.
                                         */
                                        component *= 257*255; /* =65535 */
                                        component += (255-alpha)*png_sRGB_table[outrow[c]];
                                        /* Clamp to the valid range to defend against
                                         * unforeseen cases where the data might be sRGB
                                         * instead of linear premultiplied.
                                         * (Belt-and-suspenders for CVE-2025-66293.)
                                         */
                                        if (component > 255*65535)
                                            component = 255*65535;
                                        /* So 'component' is scaled by 255*65535 and is
                                         * therefore appropriate for the sRGB-to-linear
                                         * conversion table.
                                         */
                                        component = PNG_sRGB_FROM_LINEAR(component);
                                    }
                                    else
                                    {
                                        /* Compositing was already done on the palette
                                         * entries.  The data is sRGB premultiplied on black.
                                         * Composite with the background in sRGB space.
                                         * This is not gamma-correct, but matches what was
                                         * done to the palette.
                                         */
                                        png_uint_32 background = outrow[c];
                                        component += ((255-alpha) * background + 127) / 255;
                                        if (component > 255)
                                            component = 255;
                                    }
                                }
                                outrow[c] = (png_byte)component;
                            }
                        }
                        inrow += channels+1; /* components and alpha channel */
                    }
                }
            }
        }
    }
    return 1;
}

// ---------------- Unit Tests (simple harness without GTest) ----------------

// Test: NONE interlace, color (3 channels), alpha = 255 (no compositing changes)
bool test_none_color_alpha255()
{
    // Prepare test data
    TestContext* ctx = new TestContext();
    g_ctx = ctx; // used by png_read_row

    const int width = 2;
    const int height = 1;
    const int channels = 3;
    const unsigned int format = PNG_FORMAT_FLAG_COLOR; // color

    // Per-row input: two pixels, each [R,G,B,Alpha]
    uint8_t per_row_in[] = {
        10, 20, 30, 255,   // Pixel 0
        40, 50, 60, 255    // Pixel 1
    };

    // Background (first_row) buffer: set to zeros initially
    uint8_t first_row[] = {
        0, 0, 0, 0, 0, 0
    };

    // Create the structures
    png_struct local_png_ptr;
    local_png_ptr.interlaced = PNG_INTERLACE_NONE;
    local_png_ptr.flags = 0;
    png_image img;
    png_image_read_control display;

    // Ensure image->opaque exists to provide png_ptr
    img.opaque = new png_image_opaque;
    img.opaque->png_ptr = &local_png_ptr;

    img.height = height;
    img.width  = width;
    img.format = format;

    // Link mocks
    display.image = &img;
    display.row_bytes = width * channels; // 2 * 3 = 6
    display.local_row = (void*)ctx->per_row_in.data();
    display.first_row = first_row;

    // Setup test context data
    ctx->width = width;
    ctx->height = height;
    ctx->channels = channels;
    ctx->format = format;
    ctx->per_row_in.assign(per_row_in, per_row_in + sizeof(per_row_in));
    ctx->first_row_buf.assign(first_row, first_row + sizeof(first_row));

    // Simulate per-row input
    g_ctx = ctx;

    // Run the focal method
    png_image_read_composite((png_voidp)&display);

    // Expected: first_row unchanged (since alpha == 255)
    uint8_t expected[] = {10,20,30,40,50,60};
    bool ok = (0 == std::memcmp(display.first_row, expected, sizeof(expected)));

    // Cleanup
    delete img.opaque;
    delete ctx;
    g_ctx = nullptr;

    return ok;
}

// Test: NONE interlace, color (3 channels), alpha = 128 with background => mixed output
// background: [100,100,100, 150,150,150]
bool test_none_color_alpha128_background_mix()
{
    TestContext* ctx = new TestContext();
    g_ctx = ctx;

    const int width = 2;
    const int height = 1;
    const int channels = 3;
    const unsigned int format = PNG_FORMAT_FLAG_COLOR;

    // Per-row input: two pixels, each [R,G,B,Alpha=128]
    uint8_t per_row_in[] = {
        20, 30, 40, 128,   // Pixel 0
        60, 70, 80, 128    // Pixel 1
    };

    // Backgrounds
    uint8_t first_row[] = {
        100, 100, 100,   150, 150, 150
    };

    png_struct local_png_ptr;
    local_png_ptr.interlaced = PNG_INTERLACE_NONE;
    local_png_ptr.flags = 0;
    png_image img;
    png_image_read_control display;
    img.opaque = new png_image_opaque;
    img.opaque->png_ptr = &local_png_ptr;
    img.height = height;
    img.width  = width;
    img.format = format;

    display.image = &img;
    display.row_bytes = width * channels;
    display.local_row = (void*)ctx->per_row_in.data();
    display.first_row = first_row;

    // Setup test context
    ctx->width = width;
    ctx->height = height;
    ctx->channels = channels;
    ctx->format = format;
    ctx->per_row_in.assign(per_row_in, per_row_in + sizeof(per_row_in));
    ctx->first_row_buf.assign(first_row, first_row + sizeof(first_row));

    // Run
    png_image_read_composite((png_voidp)&display);

    // Expected:
    // Pixel 0: background 100,100,100 with alpha 128 and input (20,30,40)
    // component = inrow[c] + ((255-128)*background + 127) / 255
    // = inrow[c] + (127*100 + 127)/255 = inrow[c] + floor(12827/255) = inrow[c] + 50
    // -> Pixel0: 20+50=70, 30+50=80, 40+50=90
    // Pixel 1: background 150, alpha 128, inrow 60,70,80
    // -> 60+ floor((127*150+127)/255) = 60+75 = 135
    // -> 70+75 = 145, 80+75 = 155
    uint8_t expected[] = {70,80,90, 135,145,155};
    bool ok = (0 == std::memcmp(display.first_row, expected, sizeof(expected)));

    delete img.opaque;
    delete ctx;
    g_ctx = nullptr;
    return ok;
}

// Test: NONE interlace, grayscale (1 channel), alpha = 0 (no change on output)
bool test_none_grayscale_alpha_zero_no_change()
{
    TestContext* ctx = new TestContext();
    g_ctx = ctx;

    const int width = 2;
    const int height = 1;
    const int channels = 1;
    const unsigned int format = 0; // grayscale

    // Per-row input: two pixels with alpha 0
    uint8_t per_row_in[] = {
        25, 0,    // Pixel 0: value 25, alpha 0
        200, 0    // Pixel 1: value 200, alpha 0
    };

    uint8_t first_row[] = {
        7, 8 // backgrounds for 2 pixels (one channel per pixel)
    };

    png_struct local_png_ptr;
    local_png_ptr.interlaced = PNG_INTERLACE_NONE;
    local_png_ptr.flags = 0;
    png_image img;
    png_image_read_control display;
    img.opaque = new png_image_opaque;
    img.opaque->png_ptr = &local_png_ptr;
    img.height = height;
    img.width  = width;
    img.format = format;

    display.image = &img;
    display.row_bytes = width * channels;
    display.local_row = (void*)ctx->per_row_in.data();
    display.first_row = first_row;

    ctx->width = width;
    ctx->height = height;
    ctx->channels = channels;
    ctx->format = format;
    ctx->per_row_in.assign(per_row_in, per_row_in + sizeof(per_row_in));
    ctx->first_row_buf.assign(first_row, first_row + sizeof(first_row));

    png_image_read_composite((png_voidp)&display);

    // Expect no changes to first_row due to alpha == 0
    uint8_t expected[] = {7, 8};
    bool ok = (0 == std::memcmp(display.first_row, expected, sizeof(expected)));

    delete img.opaque;
    delete ctx;
    g_ctx = nullptr;
    return ok;
}

// Test: Unknown interlace leads to error
bool test_unknown_interlace_throws()
{
    TestContext* ctx = new TestContext();
    g_ctx = ctx;

    const int width = 1; const int height = 1; const int channels = 3;
    const unsigned int format = PNG_FORMAT_FLAG_COLOR;

    uint8_t per_row_in[] = { 5, 6, 7, 255 };
    uint8_t first_row[] = { 0, 0, 0 };

    png_struct local_png_ptr;
    local_png_ptr.interlaced = 999; // unknown
    local_png_ptr.flags = 0;
    png_image img;
    png_image_read_control display;
    img.opaque = new png_image_opaque;
    img.opaque->png_ptr = &local_png_ptr;
    img.height = height;
    img.width = width;
    img.format = format;

    display.image = &img;
    display.row_bytes = width * channels;
    display.local_row = (void*)ctx->per_row_in.data();
    display.first_row = first_row;

    ctx->width = width;
    ctx->height = height;
    ctx->channels = channels;
    ctx->format = format;
    ctx->per_row_in.assign(per_row_in, per_row_in + sizeof(per_row_in));
    ctx->first_row_buf.assign(first_row, first_row + sizeof(first_row));

    bool caught = false;
    try {
        png_image_read_composite((png_voidp)&display);
    } catch (const std::exception&) {
        caught = true;
    }

    delete img.opaque;
    delete ctx;
    g_ctx = nullptr;
    return caught;
}

// Main driver: run all tests and report
int main()
{
    int passed = 0;
    int total = 0;

    auto run = [&](const char* name, bool ok){
        ++total;
        if (ok){
            std::cout << "[PASS] " << name << "\n";
            ++passed;
        } else {
            std::cout << "[FAIL] " << name << "\n";
        }
    };

    run("test_none_color_alpha255", test_none_color_alpha255());
    run("test_none_color_alpha128_background_mix", test_none_color_alpha128_background_mix());
    run("test_none_grayscale_alpha_zero_no_change", test_none_grayscale_alpha_zero_no_change());
    run("test_unknown_interlace_throws", test_unknown_interlace_throws());

    std::cout << "Summary: " << passed << " / " << total << " tests passed.\n";

    // Cleanup any remaining allocated memory from tests
    // Note: Individual tests clean up; nothing else to do here.

    return (total == passed) ? 0 : 1;
}
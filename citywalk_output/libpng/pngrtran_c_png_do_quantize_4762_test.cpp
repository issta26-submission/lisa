// Unit test suite for the focal method: png_do_quantize
// Targeted for C++11 without GoogleTest. Uses libpng types/macros.
// The tests are designed to exercise the main decision branches inside
// png_do_quantize as described in the provided source snippet.
// Explanatory comments are included for clarity and maintenance.

#include <vector>
#include <cstdlib>
#include <iostream>
#include <pngpriv.h>
#include <cstring>
#include <png.h>


// Include libpng public API for type and macro definitions.
// The test assumes the project under test provides these headers.
extern "C" {
}

// Simple non-terminating test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_ASSERT(cond, desc) do { \
    ++g_total_tests; \
    if (!(cond)) { \
        ++g_failed_tests; \
        std::cerr << "[FAIL] " << desc << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
    } else { \
        std::cout << "[PASS] " << desc << "\n"; \
    } \
} while (0)

namespace test_png_do_quantize {

// Helper: compute the quantization index p for a given (r,g,b) using libpng macros
static unsigned int compute_p(unsigned int r, unsigned int g, unsigned int b)
{
    // r contributes to the top RED_BITS bits
    unsigned int rbits = (r >> (8 - PNG_QUANTIZE_RED_BITS)) &
        ((1u << PNG_QUANTIZE_RED_BITS) - 1);
    // g contributes to the middle GREEN_BITS bits
    unsigned int gbits = (g >> (8 - PNG_QUANTIZE_GREEN_BITS)) &
        ((1u << PNG_QUANTIZE_GREEN_BITS) - 1);
    // b contributes to the low BLUE_BITS bits
    unsigned int bbits = (b >> (8 - PNG_QUANTIZE_BLUE_BITS)) &
        ((1u << PNG_QUANTIZE_BLUE_BITS) - 1);

    return (rbits << (PNG_QUANTIZE_GREEN_BITS + PNG_QUANTIZE_BLUE_BITS)) |
           (gbits << (PNG_QUANTIZE_BLUE_BITS)) |
           (bbits);
}

// Test 1: PNG_COLOR_TYPE_RGB with a non-null palette_lookup should quantize 3-channel RGB
//          -> convert to PALETTE color type, 1 channel, and update rowbytes accordingly.
static void test_quantize_rgb_palette_lookup()
{
    // Prepare a small row of 4 pixels (RGB)
    const png_uint_32 row_width = 4;
    std::vector<unsigned char> row_buf(row_width * 3); // 3 bytes per pixel (RGB)
    // Pixel colors: (0,0,0), (255,0,0), (0,255,0), (0,0,255)
    row_buf[0] = 0;   row_buf[1] = 0;   row_buf[2] = 0;
    row_buf[3] = 255; row_buf[4] = 0;   row_buf[5] = 0;
    row_buf[6] = 0;   row_buf[7] = 255; row_buf[8] = 0;
    row_buf[9] = 0;   row_buf[10]= 0;  row_buf[11]= 255;

    // Allocate and init a libpng row_info structure
    png_row_infop row_info = (png_row_infop)std::malloc(sizeof(png_row_info));
    if (row_info == nullptr) {
        std::cerr << "[ERROR] Failed to allocate row_info\n";
        return;
    }
    row_info->width = row_width;
    row_info->bit_depth = 8;
    row_info->color_type = PNG_COLOR_TYPE_RGB;
    // Value placeholders; the function will override some fields later
    row_info->channels = 3;
    row_info->pixel_depth = 8;
    row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth, row_width);

    // Prepare a large enough palette_lookup as required by quantization index space
    int quant_bits = PNG_QUANTIZE_RED_BITS + PNG_QUANTIZE_GREEN_BITS + PNG_QUANTIZE_BLUE_BITS;
    int palette_size = (1 << quant_bits);
    std::vector<unsigned char> palette_lookup(palette_size);
    // Simple deterministic mapping: palette_lookup[p] == (p & 0xFF)
    for (int i = 0; i < palette_size; ++i) {
        palette_lookup[i] = static_cast<unsigned char>(i & 0xFF);
    }

    // Call the function under test
    png_do_quantize(row_info, row_buf.data(), palette_lookup.data(), nullptr);

    // Verify: after quantization, the row is a single palette-index per pixel
    for (unsigned int i = 0; i < row_width; ++i) {
        unsigned int r = (i == 0) ? 0 : 0; // from the test data
    }
    // Re-derive expected values per pixel from the input data
    unsigned int r0 = 0, g0 = 0, b0 = 0;
    unsigned int r1 = 255, g1 = 0, b1 = 0;
    unsigned int r2 = 0, g2 = 255, b2 = 0;
    unsigned int r3 = 0, g3 = 0, b3 = 255;

    unsigned int p0 = compute_p(r0, g0, b0);
    unsigned int p1 = compute_p(r1, g1, b1);
    unsigned int p2 = compute_p(r2, g2, b2);
    unsigned int p3 = compute_p(r3, g3, b3);

    TEST_ASSERT(row_buf[0] == palette_lookup[p0],
        "RGB->palette quantization pixel 0 value matches palette_lookup[p0]");
    TEST_ASSERT(row_buf[1] == palette_lookup[p1],
        "RGB->palette quantization pixel 1 value matches palette_lookup[p1]");
    TEST_ASSERT(row_buf[2] == palette_lookup[p2],
        "RGB->palette quantization pixel 2 value matches palette_lookup[p2]");
    TEST_ASSERT(row_buf[3] == palette_lookup[p3],
        "RGB->palette quantization pixel 3 value matches palette_lookup[p3]");

    // Verify row_info metadata updates
    TEST_ASSERT(row_info->color_type == PNG_COLOR_TYPE_PALETTE,
                "After quantization, color_type updated to PALETTE");
    TEST_ASSERT(row_info->channels == 1,
                "After quantization, channels updated to 1");
    TEST_ASSERT(row_info->pixel_depth == 8,
                "After quantization, pixel_depth remains equal to bit_depth (8)");
    TEST_ASSERT(row_info->rowbytes == PNG_ROWBYTES(row_info->pixel_depth, row_width),
                "rowbytes updated to correct value after quantization");

    std::free(row_info);
}

// Test 2: PNG_COLOR_TYPE_RGB_ALPHA with a non-null palette_lookup should quantize 4-byte pixels
static void test_quantize_rgbalpha_palette_lookup()
{
    // Prepare a small row of 2 pixels (RGB with Alpha)
    const png_uint_32 row_width = 2;
    std::vector<unsigned char> row_buf(row_width * 4); // 4 bytes per pixel (RGBA)
    // Pixel 0: (255,0,0,0)
    row_buf[0] = 255; row_buf[1] = 0; row_buf[2] = 0; row_buf[3] = 0;
    // Pixel 1: (0,255,0,0)
    row_buf[4] = 0; row_buf[5] = 255; row_buf[6] = 0; row_buf[7] = 0;

    // Prepare row_info
    png_row_infop row_info = (png_row_infop)std::malloc(sizeof(png_row_info));
    if (row_info == nullptr) {
        std::cerr << "[ERROR] Failed to allocate row_info\n";
        return;
    }
    row_info->width = row_width;
    row_info->bit_depth = 8;
    row_info->color_type = PNG_COLOR_TYPE_RGB_ALPHA;
    row_info->channels = 4;
    row_info->pixel_depth = 8;
    row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth, row_width);

    // Palette lookup as in Test 1
    int quant_bits = PNG_QUANTIZE_RED_BITS + PNG_QUANTIZE_GREEN_BITS + PNG_QUANTIZE_BLUE_BITS;
    int palette_size = (1 << quant_bits);
    std::vector<unsigned char> palette_lookup(palette_size);
    for (int i = 0; i < palette_size; ++i) {
        palette_lookup[i] = static_cast<unsigned char>(i & 0xFF);
    }

    // Execute
    png_do_quantize(row_info, row_buf.data(), palette_lookup.data(), nullptr);

    // Expected p values
    unsigned int p0 = compute_p(255, 0, 0);
    unsigned int p1 = compute_p(0, 255, 0);

    TEST_ASSERT(row_buf[0] == palette_lookup[p0],
                "RGB_ALPHA->palette quantization pixel 0 value matches palette_lookup[p0]");
    TEST_ASSERT(row_buf[1] == palette_lookup[p1],
                "RGB_ALPHA->palette quantization pixel 1 value matches palette_lookup[p1]");

    // Metadata checks
    TEST_ASSERT(row_info->color_type == PNG_COLOR_TYPE_PALETTE,
                "After quantization (RGBA), color_type updated to PALETTE");
    TEST_ASSERT(row_info->channels == 1,
                "After quantization (RGBA), channels updated to 1");
    TEST_ASSERT(row_info->pixel_depth == 8,
                "After quantization (RGBA), pixel_depth remains 8");
    TEST_ASSERT(row_info->rowbytes == PNG_ROWBYTES(row_info->pixel_depth, row_width),
                "rowbytes updated correctly for PALETTE after RGBA quantization");

    std::free(row_info);
}

// Test 3: PNG_COLOR_TYPE_PALETTE with quantize_lookup should map each palette index
static void test_quantize_palette_with_quantize_lookup()
{
    // Row of 4 palette indices
    const png_uint_32 row_width = 4;
    std::vector<unsigned char> row_buf(row_width); // 1 byte per pixel for PALETTE
    row_buf[0] = 0;
    row_buf[1] = 1;
    row_buf[2] = 200;
    row_buf[3] = 255;

    // row_info in PALETTE mode
    png_row_infop row_info = (png_row_infop)std::malloc(sizeof(png_row_info));
    if (row_info == nullptr) {
        std::cerr << "[ERROR] Failed to allocate row_info\n";
        return;
    }
    row_info->width = row_width;
    row_info->bit_depth = 8;
    row_info->color_type = PNG_COLOR_TYPE_PALETTE;
    row_info->channels = 1;
    row_info->pixel_depth = 8;
    row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth, row_width);

    // quantize_lookup (index map)
    std::vector<unsigned char> quantize_lookup(256);
    for (int i = 0; i < 256; ++i) {
        quantize_lookup[i] = static_cast<unsigned char>(255 - i);
    }

    // palettes are not used in this path; set palette_lookup to nullptr
    png_do_quantize(row_info, row_buf.data(), nullptr, quantize_lookup.data());

    // Expected: row_buf[i] becomes quantize_lookup[original_value]
    TEST_ASSERT(row_buf[0] == quantize_lookup[0],
                "PALETTE->quantize: index 0 maps correctly");
    TEST_ASSERT(row_buf[1] == quantize_lookup[1],
                "PALETTE->quantize: index 1 maps correctly");
    TEST_ASSERT(row_buf[2] == quantize_lookup[200],
                "PALETTE->quantize: index 200 maps correctly");
    TEST_ASSERT(row_buf[3] == quantize_lookup[255],
                "PALETTE->quantize: index 255 maps correctly");

    // Metadata checks
    TEST_ASSERT(row_info->color_type == PNG_COLOR_TYPE_PALETTE,
                "PALETTE quantize path preserves color_type as PALETTE");
    TEST_ASSERT(row_info->channels == 1,
                "PALETTE quantize path preserves channels as 1");
    TEST_ASSERT(row_info->pixel_depth == 8,
                "PALETTE quantize path preserves pixel_depth as 8");
    TEST_ASSERT(row_info->rowbytes == PNG_ROWBYTES(row_info->pixel_depth, row_width),
                "PALETTE quantize path rowbytes updated correctly");

    std::free(row_info);
}

// Test 4: Bit depth != 8 should bypass processing (no changes)
static void test_no_change_when_bit_depth_not_8()
{
    // 2 pixels of RGB, bit_depth set to 4 (non-8)
    const png_uint_32 row_width = 2;
    std::vector<unsigned char> row_buf(row_width * 3);
    // Pixel 1: (10,20,30)
    row_buf[0] = 10; row_buf[1] = 20; row_buf[2] = 30;
    // Pixel 2: (40,50,60)
    row_buf[3] = 40; row_buf[4] = 50; row_buf[5] = 60;

    png_row_infop row_info = (png_row_infop)std::malloc(sizeof(png_row_info));
    if (row_info == nullptr) {
        std::cerr << "[ERROR] Failed to allocate row_info\n";
        return;
    }
    row_info->width = row_width;
    row_info->bit_depth = 4; // non-8
    row_info->color_type = PNG_COLOR_TYPE_RGB;
    row_info->channels = 3;
    row_info->pixel_depth = 4;
    row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth, row_width);

    // palette_lookup non-null to ensure code would enter first branch if bit_depth were 8
    std::vector<unsigned char> palette_lookup(1 << (PNG_QUANTIZE_RED_BITS +
                                                   PNG_QUANTIZE_GREEN_BITS +
                                                   PNG_QUANTIZE_BLUE_BITS));
    for (size_t i = 0; i < palette_lookup.size(); ++i) palette_lookup[i] = static_cast<unsigned char>(i & 0xFF);

    // Copy original to compare after call
    std::vector<unsigned char> original = row_buf;

    png_do_quantize(row_info, row_buf.data(), palette_lookup.data(), nullptr);

    // Expect no changes to row data and metadata since bit_depth != 8
    TEST_ASSERT(std::memcmp(original.data(), row_buf.data(), row_buf.size()) == 0,
                "No changes to row data when bit_depth != 8");

    TEST_ASSERT(row_info->color_type == PNG_COLOR_TYPE_RGB,
                "color_type remains unchanged when bit_depth != 8");
    TEST_ASSERT(row_info->rowbytes == PNG_ROWBYTES(row_info->pixel_depth, row_width),
                "rowbytes remains unchanged when bit_depth != 8");

    std::free(row_info);
}

// Main entry: execute all tests
void run_all_tests()
{
    std::cout << "Starting png_do_quantize unit tests (no GTest)..." << std::endl;
    test_quantize_rgb_palette_lookup();
    test_quantize_rgbalpha_palette_lookup();
    test_quantize_palette_with_quantize_lookup();
    test_no_change_when_bit_depth_not_8();
    std::cout << "Testing finished. Total: " << g_total_tests
              << ", Failed: " << g_failed_tests << std::endl;
}

} // namespace test_png_do_quantize

// Entry point for the test executable
int main()
{
    test_png_do_quantize::run_all_tests();
    return (test_png_do_quantize::g_failed_tests == 0) ? 0 : 1;
}
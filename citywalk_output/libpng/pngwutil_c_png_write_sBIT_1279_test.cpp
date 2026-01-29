// Lightweight C++11 test suite for libpng's png_write_sBIT function (pngwutil.c)
// This test uses libpng to drive a controlled in-memory write and validates
// the behavior of png_write_sBIT across various color types and sBIT values.
// It avoids GTest and uses simple EXPECT-style macros that do not terminate on failure.

#include <cstdint>
#include <vector>
#include <setjmp.h>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>
#include <png.h>


// Lightweight test harness macros (non-terminating assertions)
#define TEST_EXIT_IF(cond, msg) do { if (cond) { /* no-op */ } else { /* keep running */ } } while(0)
#define EXPECT_TRUE(cond) do { if (!(cond)) { std::cerr << "EXPECT_TRUE failed: " #cond " at " << __FILE__ << ":" << __LINE__ << "\n"; failures++; } } while(0)
#define EXPECT_FALSE(cond) do { if (cond) { std::cerr << "EXPECT_FALSE failed: " #cond " at " << __FILE__ << ":" << __LINE__ << "\n"; failures++; } } while(0)
#define EXPECT_EQ(a,b) do { if ((a)!=(b)) { std::cerr << "EXPECT_EQ failed: " #a " != " #b " (" << (a) << " != " << (b) << ") at " << __FILE__ << ":" << __LINE__ << "\n"; failures++; } } while(0)
#define LOG(msg) std::cout << "[LOG] " << msg << "\n";

// Global failure counter
static int failures = 0;

// Memory sink to capture PNG bytes written by libpng
static std::vector<uint8_t> g_output;

// Memory write function for libpng
static void png_mem_write(png_structp png_ptr, png_bytep data, png_size_t length)
{
    (void)png_ptr; // unused
    g_output.insert(g_output.end(), data, data + length);
}

// Error handler for libpng that uses longjmp for control flow
static void png_test_error(png_structp png_ptr, png_const_charp /*msg*/)
{
    // Propagate error back to test harness
    longjmp(png_jmpbuf(png_ptr), 1);
}

// Helper to check if an sBIT chunk exists in the produced PNG and return its raw data
static bool extract_sBIT_chunk(const std::vector<uint8_t>& buf, std::vector<uint8_t>& data_out)
{
    // PNG file layout: signature (8 bytes) + chunks
    // Each chunk: length (4 bytes, big-endian) + type (4 bytes) + data (length) + CRC (4 bytes)
    // We'll scan for a chunk with type "sBIT" after the signature.
    if (buf.size() < 8) return false;
    for (size_t i = 8; i + 8 <= buf.size(); ++i)
    {
        // Read length (big-endian)
        uint32_t len = (static_cast<uint32_t>(buf[i]) << 24) |
                       (static_cast<uint32_t>(buf[i+1]) << 16) |
                       (static_cast<uint32_t>(buf[i+2]) << 8)  |
                       (static_cast<uint32_t>(buf[i+3]));
        // Chunk type
        const char chunk_type[5] = { static_cast<char>(buf[i+4]),
                                     static_cast<char>(buf[i+5]),
                                     static_cast<char>(buf[i+6]),
                                     static_cast<char>(buf[i+7]),
                                     '\0' };
        if (std::strncmp(chunk_type, "sBIT", 4) == 0) {
            if (i + 8 + len <= buf.size()) {
                data_out.assign(buf.begin() + i + 8, buf.begin() + i + 8 + len);
                return true;
            } else {
                return false;
            }
        }
        // Move to next chunk: i advances by 8 + len (we'll adjust in next loop naturally)
        i += 8 + len - 1; // -1 because for loop will increment
    }
    return false;
}

// Test helper: initialize a PNG writer and attempt to write an sBIT chunk for given color type
static bool run_png_write_sBIT_test(png_uint_32 width, png_uint_32 height, int color_type,
                                  const png_color_8& sbit)
{
    // Clear previous output
    g_output.clear();

    // Create write struct and info struct
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, png_test_error, NULL);
    if (!png_ptr) {
        std::cerr << "Failed to create write struct\n";
        return false;
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        std::cerr << "Failed to create info struct\n";
        return false;
    }

    // Set memory sink for output
    png_set_write_fn(png_ptr, NULL, png_mem_write, NULL);

    // Setup error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return false;
    }

    // Write signature and IHDR to establish a minimal valid PNG
    png_write_sig(png_ptr);
    png_write_IHDR(png_ptr, width, height, 8, color_type,
                   PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE, PNG_INTERLACE_NONE);

    // Write sBIT chunk (the focal method under test)
    png_write_sBIT(png_ptr, &sbit, color_type);

    // Finish
    png_write_end(png_ptr, NULL);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);

    // Analyze output for sBIT chunk
    std::vector<uint8_t> sBIT_data;
    bool found = extract_sBIT_chunk(g_output, sBIT_data);
    // Return true only if sBIT chunk found
    return found;
}

// Test 1: RGB truecolor with valid sBIT values -> expect 3 data bytes [red, green, blue]
static void test_rgb_sBIT_valid()
{
    LOG("Test RGB truecolor: valid sBIT (3 bytes expected)");
    png_color_8 sbit;
    sbit.red = 4;
    sbit.green = 4;
    sbit.blue = 4;
    sbit.gray = 0;
    sbit.alpha = 0;

    bool ok = run_png_write_sBIT_test(1, 1, PNG_COLOR_TYPE_RGB, sbit);

    // Inspect the produced PNG for sBIT data
    std::vector<uint8_t> data;
    bool found = extract_sBIT_chunk(g_output, data);
    EXPECT_TRUE(ok);
    EXPECT_TRUE(found);
    EXPECT_EQ(data.size(), 3);
    EXPECT_EQ(static_cast<int>(data[0]), 4);
    EXPECT_EQ(static_cast<int>(data[1]), 4);
    EXPECT_EQ(static_cast<int>(data[2]), 4);
}

// Test 2: RGB truecolor with invalid sBIT (red == 0) -> expect no sBIT chunk
static void test_rgb_sBIT_invalid_zero_red()
{
    LOG("Test RGB truecolor: invalid sBIT (red == 0) should not emit sBIT chunk");
    png_color_8 sbit;
    sbit.red = 0;      // invalid
    sbit.green = 4;
    sbit.blue = 4;
    sbit.gray = 0;
    sbit.alpha = 0;

    bool ok = run_png_write_sBIT_test(1, 1, PNG_COLOR_TYPE_RGB, sbit);
    std::vector<uint8_t> data;
    bool found = extract_sBIT_chunk(g_output, data);
    // Expect no sBIT chunk on invalid input
    EXPECT_TRUE(ok); // function should not crash
    EXPECT_FALSE(found);
}

// Test 3: Grayscale without alpha with valid sBIT (gray depth)
static void test_gray_sBIT_valid()
{
    LOG("Test grayscale: valid sBIT (single gray value)");
    png_color_8 sbit;
    sbit.red = 0;
    sbit.green = 0;
    sbit.blue = 0;
    sbit.gray = 7;      // valid
    sbit.alpha = 0;

    bool ok = run_png_write_sBIT_test(1, 1, PNG_COLOR_TYPE_GRAY, sbit);
    std::vector<uint8_t> data;
    bool found = extract_sBIT_chunk(g_output, data);
    EXPECT_TRUE(ok);
    EXPECT_TRUE(found);
    EXPECT_EQ(data.size(), 1);
    EXPECT_EQ(static_cast<int>(data[0]), 7);
}

// Test 4: Grayscale with alpha valid (GRAY_ALPHA)
static void test_gray_alpha_sBIT_valid()
{
    LOG("Test grayscale with alpha: valid sBIT (gray, alpha)");
    png_color_8 sbit;
    sbit.red = 0;
    sbit.green = 0;
    sbit.blue = 0;
    sbit.gray = 5;
    sbit.alpha = 2;       // alpha depth valid under usr_bit_depth (8)

    bool ok = run_png_write_sBIT_test(1, 1, PNG_COLOR_TYPE_GRAY_ALPHA, sbit);
    std::vector<uint8_t> data;
    bool found = extract_sBIT_chunk(g_output, data);
    EXPECT_TRUE(ok);
    EXPECT_TRUE(found);
    EXPECT_EQ(data.size(), 2);
    EXPECT_EQ(static_cast<int>(data[0]), 5);
    EXPECT_EQ(static_cast<int>(data[1]), 2);
}

// Test 5: Grayscale invalid (gray depth 0) -> no sBIT
static void test_gray_sBIT_invalid_zero_gray()
{
    LOG("Test grayscale: invalid sBIT (gray depth 0) should not emit sBIT chunk");
    png_color_8 sbit;
    sbit.gray = 0;        // invalid
    sbit.red = 0; sbit.green = 0; sbit.blue = 0; sbit.alpha = 0;

    bool ok = run_png_write_sBIT_test(1, 1, PNG_COLOR_TYPE_GRAY, sbit);
    std::vector<uint8_t> data;
    bool found = extract_sBIT_chunk(g_output, data);
    EXPECT_TRUE(ok);
    EXPECT_FALSE(found);
}

// Main bootstrap: run all tests
int main()
{
    failures = 0;

    test_rgb_sBIT_valid();
    test_rgb_sBIT_invalid_zero_red();
    test_gray_sBIT_valid();
    test_gray_alpha_sBIT_valid();
    test_gray_sBIT_invalid_zero_gray();

    if (failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << "Tests completed with " << failures << " failure(s).\n";
        return 1;
    }
}
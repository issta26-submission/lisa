// C++11 test suite for png_handle_zTXt (via end-to-end PNG write/read using libpng).
// This test does not use GTest; it uses a small custom test harness with
// non-terminating assertions to maximize coverage across tests.
// The goal is to exercise the zTXt path by writing a PNG with a zTXt text chunk
// and reading it back to verify correct handling.

#include <cstdint>
#include <cassert>
#include <vector>
#include <setjmp.h>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>
#include <png.h>



// Dominant dependencies and their keywords (for Step 1 candidacy):
// - buffer, read_buffer, write buffer: png_read_buffer, png_write_fn, png_set_write_fn
// - keyword_length, keyword, compression type (PNG_COMPRESSION_TYPE_BASE, PNG_TEXT_COMPRESSION_zTXt)
// - uncompressed_length, zTXt, zstream, png_decompress_chunk, PNG_SIZE_MAX
// - errmsg, "bad keyword", "truncated", "unknown compression type", "out of memory", "Read failure in png_handle_zTXt"
// - PNG_USER_LIMITS_SUPPORTED, user_chunk_cache_max
// - png_set_text_2, png_text, PNG_TEXT_COMPRESSION_zTXt
// - png_ptr, info_ptr, length, buffer, read_buffer, zstream
// - png_crc_finish, png_crc_read, png_crc_finish_critical
// - png_handle_zTXt, PNG_COMPRESSION_TYPE_BASE
// These keywords guide understanding and coverage for the tests.


// Test harness: a tiny non-terminating assertion facility.
struct TestResult {
    int passed = 0;
    int failed = 0;
    void report() const {
        std::cerr << "Test results: " << passed << " passed, " << failed << " failed.\n";
    }
};
#define ASSERT_TRUE(cond, msg, res) do { if (cond) { ++(res).passed; } else { ++(res).failed; std::cerr << "FAIL: " << (msg) << "\n"; } } while(0)
#define ASSERT_EQ(a, b, res, msg) do { if ((a) == (b)) { ++(res).passed; } else { ++(res).failed; std::cerr << "FAIL: " << (msg) << " (expected " << (b) << ", got " << (a) << ")\n"; } } while(0)


// Helpers to write/read PNG data to/from memory using libpng.
struct PngMemoryWriteCtx {
    std::vector<unsigned char> data;
};

static void png_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    PngMemoryWriteCtx* ctx = static_cast<PngMemoryWriteCtx*>(png_get_io_ptr(png_ptr));
    if (!ctx) return;
    ctx->data.insert(ctx->data.end(), data, data + length);
}

struct PngMemoryReadCtx {
    const unsigned char* data;
    size_t size;
    size_t offset;
};

static void png_read_fn(png_structp png_ptr, png_bytep out, png_size_t length) {
    PngMemoryReadCtx* ctx = static_cast<PngMemoryReadCtx*>(png_get_io_ptr(png_ptr));
    if (!ctx) return;
    if (ctx->offset >= ctx->size) { // No more data
        length = 0;
        return;
    }
    if (ctx->offset + length > ctx->size) {
        length = ctx->size - ctx->offset;
    }
    if (length > 0) {
        std::memcpy(out, ctx->data + ctx->offset, length);
        ctx->offset += length;
    }
}

// Create a tiny 1x1 RGB PNG image in memory with a zTXt text chunk.
// keyword: PK, text: content string to be compressed by libpng on write.
// Returns true on success, fills out_png with produced PNG bytes.
static bool write_png_with_zTXt(std::vector<unsigned char>& out_png,
                               const std::string& keyword,
                               const std::string& text) {
    // Initialize writer
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return false;
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return false;
    }
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return false;
    }

    PngMemoryWriteCtx writer_ctx;
    png_set_write_fn(png_ptr, &writer_ctx, png_write_fn, NULL);

    // IHDR: 1x1 image, 8-bit RGB
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8,
                 PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);

    // Prepare text chunk with zTXt compression
    png_text text_chunk;
    text_chunk.compression = PNG_TEXT_COMPRESSION_zTXt;
    text_chunk.key = (png_charp)keyword.c_str();
    text_chunk.text = (png_charp)text.c_str();
    text_chunk.text_length = (png_size_t)text.size();

    png_set_text(png_ptr, info_ptr, &text_chunk, 1);

    // Write a single row (red pixel)
    unsigned char row[3] = { 0x7F, 0x00, 0x00 }; // nice red
    png_bytep row_ptr = row;
    // Write header
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return false;
    }
    png_write_info(png_ptr, info_ptr);
    // Write image data
    png_write_image(png_ptr, &row_ptr);
    png_write_end(png_ptr, info_ptr);

    png_destroy_write_struct(&png_ptr, &info_ptr);

    out_png.swap(writer_ctx.data);
    return true;
}

// Read a PNG from memory and extract text entries via libpng's public API.
static bool read_png_and_get_text(const std::vector<unsigned char>& in_png,
                                  const std::string& keyword,
                                  std::string& out_text) {
    // Setup reader
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return false;
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return false;
    }
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return false;
    }

    PngMemoryReadCtx reader_ctx;
    reader_ctx.data = in_png.data();
    reader_ctx.size = in_png.size();
    reader_ctx.offset = 0;

    png_set_read_fn(png_ptr, &reader_ctx, png_read_fn);

    // Initialize reading
    png_read_info(png_ptr, info_ptr);

    // Fetch text entries
    int num_text = 0;
    png_textp text_ptrs = NULL;
    png_get_text(png_ptr, info_ptr, &text_ptrs, &num_text);
    bool found = false;
    for (int i = 0; i < num_text; ++i) {
        if (text_ptrs[i].key && keyword == text_ptrs[i].key) {
            // text_ptrs[i].text is uncompressed text for tEXt/png_text with compression
            if (text_ptrs[i].text) {
                out_text = std::string(text_ptrs[i].text, text_ptrs[i].text_length);
            } else {
                out_text.clear();
            }
            found = true;
            break;
        }
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return found;
}


// Test 1: End-to-end zTXt encoding/decoding.
// - Write a 1x1 PNG with a zTXt chunk containing keyword "Comment" and text "Hello world".
// - Read it back and verify the retrieved text matches.
static void test_end_to_end_zTXt(TestResult& res) {
    std::vector<unsigned char> png_bytes;
    const std::string keyword = "Comment";
    const std::string text = "Hello world";

    bool wrote = write_png_with_zTXt(png_bytes, keyword, text);
    ASSERT_TRUE(wrote, "Failed to write PNG with zTXt text chunk", res);
    if (!wrote) return;

    std::string extracted;
    bool found = read_png_and_get_text(png_bytes, keyword, extracted);
    ASSERT_TRUE(found, "Expected to find zTXt text blob with keyword 'Comment'", res);
    if (!found) return;

    ASSERT_EQ(extracted, text, res, "Text content mismatch for zTXt payload");
}

// Test 2: End-to-end zTXt with empty text content.
// - Write a 1x1 PNG with keyword "Empty" and empty text.
// - Read it back and verify the retrieved text is empty.
static void test_zTXt_empty_text(TestResult& res) {
    std::vector<unsigned char> png_bytes;
    const std::string keyword = "Empty";
    const std::string text = ""; // empty payload

    bool wrote = write_png_with_zTXt(png_bytes, keyword, text);
    ASSERT_TRUE(wrote, "Failed to write PNG with zTXt empty text chunk", res);
    if (!wrote) return;

    std::string extracted;
    bool found = read_png_and_get_text(png_bytes, keyword, extracted);
    ASSERT_TRUE(found, "Expected to find zTXt text blob with keyword 'Empty'", res);
    if (!found) return;

    ASSERT_EQ(extracted.length(), (size_t)0, res, "Expected empty text content for zTXt payload");
}


// Main: run all tests and report.
int main() {
    TestResult res;

    // Step 2: Generate test suite for png_handle_zTXt by going through the public API path.
    test_end_to_end_zTXt(res);
    test_zTXt_empty_text(res);

    res.report();
    return res.failed;
}
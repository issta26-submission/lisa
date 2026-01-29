// C++11 test suite for the focal method: png_write_tEXt from pngwutil.c
// This test suite uses libpng public APIs to exercise the tEXt chunk writing
// behavior, including happy-path (valid keyword and text), empty text, and
// error handling when the keyword is invalid. It is designed to be run with
// a C++11 compiler and linked against libpng (no GTest; a small in-file test
// harness is provided).

#include <cstdint>
#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>
#include <png.h>


// Include libpng headers (C API). Use extern "C" to avoid name mangling issues in C++.
extern "C" {
}

// Helper: memory-backed I/O for libpng writes
namespace png_test {

// Storage for all bytes written by libpng
static std::vector<uint8_t> g_stream;

// libpng write callback: append to g_stream
static void memory_write(png_structp png_ptr, png_bytep data, png_size_t length)
{
    (void)png_ptr; // Unused except for callback signature
    const uint8_t* begin = data;
    const uint8_t* end = data + length;
    g_stream.insert(g_stream.end(), begin, end);
}

// libpng flush callback (no-op for memory sink)
static void memory_flush(png_structp png_ptr)
{
    (void)png_ptr;
}

static bool parse_tEXt_chunk(const std::vector<uint8_t>& data,
                             std::string& out_key,
                             std::string& out_text)
{
    // PNG signature is 8 bytes; chunks follow
    const size_t sig_len = 8;
    if (data.size() < sig_len) return false;

    // Scan for tEXt chunk
    for (size_t i = sig_len; i + 8 <= data.size(); /*i grows inside*/) {
        // Read length (big-endian)
        uint32_t len = (static_cast<uint32_t>(data[i]) << 24) |
                       (static_cast<uint32_t>(data[i + 1]) << 16) |
                       (static_cast<uint32_t>(data[i + 2]) << 8) |
                       (static_cast<uint32_t>(data[i + 3]));
        // Chunk type
        const char* type = reinterpret_cast<const char*>(&data[i + 4]);
        if (std::strncmp(type, "tEXt", 4) == 0) {
            // Ensure payload is within stream
            if (i + 8 + len > data.size()) return false;
            const uint8_t* payload = &data[i + 8];
            // Find NUL terminator between key and text
            size_t pos = 0;
            while (pos < len && payload[pos] != 0) ++pos;
            if (pos == len) return false; // No NUL found; invalid
            out_key.assign(reinterpret_cast<const char*>(payload), pos);
            if (pos + 1 <= len) {
                size_t text_len = len - (pos + 1);
                if (text_len > 0) {
                    out_text.assign(reinterpret_cast<const char*>(payload + pos + 1),
                                    text_len);
                } else {
                    out_text.clear();
                }
            } else {
                out_text.clear();
            }
            return true;
        }
        // Move to next chunk: i += 8 + len + 4 (len+CRC)
        // But we will just advance safely by len + 12 (header + CRC)
        i += static_cast<size_t>(12) + len;
    }
    return false;
}

// Test helper: reset stream
static void reset_stream() { g_stream.clear(); }

// Test 1: Valid tEXt write with non-empty key and text
static bool test_valid_tEXt_writes_chunk()
{
    reset_stream();

    // Create a write struct
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        std::cerr << "[Test1] Failed to create png write struct.\n";
        return false;
    }

    // Bind memory-backed IO
    png_set_write_fn(png_ptr, &g_stream, memory_write, memory_flush);

    // Set up a small image so that writing a tEXt chunk is valid
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred unexpectedly
        std::cerr << "[Test1] Unexpected error during write.\n";
        png_destroy_write_struct(&png_ptr, NULL);
        return false;
    }

    // Write signature and IHDR
    png_write_sig(png_ptr);
    png_write_IHDR(png_ptr, 1, 1, 8,
                   PNG_COLOR_TYPE_RGB,
                   PNG_COMPRESSION_TYPE_BASE,
                   PNG_FILTER_TYPE_BASE,
                   PNG_INTERLACE_NONE);

    // Prepare key and text
    const char* key = "title";
    const char* text = "Hello PNG";

    // Call focal method
    png_write_tEXt(png_ptr, key, text, strlen(text)); // text_len parameter ignored by function

    // Finish PNG
    png_write_end(png_ptr, NULL);
    png_destroy_write_struct(&png_ptr, NULL);

    // Parse the generated stream to locate tEXt chunk
    std::string parsed_key, parsed_text;
    bool found = parse_tEXt_chunk(g_stream, parsed_key, parsed_text);
    if (!found) {
        std::cerr << "[Test1] tEXt chunk not found in output.\n";
        return false;
    }

    // Validate content
    bool ok = (parsed_key == key) && (parsed_text == text);
    if (!ok) {
        std::cerr << "[Test1] tEXt content mismatch. Expected key='" << key
                  << "' text='" << text << "' but got key='" << parsed_key
                  << "' text='" << parsed_text << "'.\n";
    }
    return ok;
}

// Test 2: Valid tEXt write with empty text (text == "" -> text_len == 0)
static bool test_valid_tEXt_with_empty_text()
{
    reset_stream();

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        std::cerr << "[Test2] Failed to create png write struct.\n";
        return false;
    }

    png_set_write_fn(png_ptr, &g_stream, memory_write, memory_flush);

    if (setjmp(png_jmpbuf(png_ptr))) {
        std::cerr << "[Test2] Unexpected error during write.\n";
        png_destroy_write_struct(&png_ptr, NULL);
        return false;
    }

    png_write_sig(png_ptr);
    png_write_IHDR(png_ptr, 1, 1, 8,
                   PNG_COLOR_TYPE_RGB,
                   PNG_COMPRESSION_TYPE_BASE,
                   PNG_FILTER_TYPE_BASE,
                   PNG_INTERLACE_NONE);

    const char* key = "author";
    const char* text = ""; // empty text
    png_write_tEXt(png_ptr, key, text, strlen(text)); // length param ignored

    png_write_end(png_ptr, NULL);
    png_destroy_write_struct(&png_ptr, NULL);

    std::string parsed_key, parsed_text;
    bool found = parse_tEXt_chunk(g_stream, parsed_key, parsed_text);
    if (!found) {
        std::cerr << "[Test2] tEXt chunk not found in output.\n";
        return false;
    }

    // The data should be key + NUL only (no extra text)
    bool ok = (parsed_key == key) && (parsed_text == "");
    if (!ok) {
        std::cerr << "[Test2] tEXt content mismatch for empty text. key='"
                  << parsed_key << "' text='" << parsed_text << "'.\n";
    }
    return ok;
}

// Test 3: Invalid keyword triggers error path (png_error) and longjmp
static bool test_invalid_keyword_triggers_error()
{
    reset_stream();

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        std::cerr << "[Test3] Failed to create png write struct.\n";
        return false;
    }

    png_set_write_fn(png_ptr, &g_stream, memory_write, memory_flush);

    bool error_occurred = false;
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error path taken as expected
        error_occurred = true;
        png_destroy_write_struct(&png_ptr, NULL);
        // success of this test
        return error_occurred;
    }

    // Trigger error by providing invalid keyword (empty key)
    png_write_sig(png_ptr);
    png_write_IHDR(png_ptr, 1, 1, 8,
                   PNG_COLOR_TYPE_RGB,
                   PNG_COMPRESSION_TYPE_BASE,
                   PNG_FILTER_TYPE_BASE,
                   PNG_INTERLACE_NONE);

    // Invalid keyword: empty string should trigger "tEXt: invalid keyword"
    const char* invalid_key = "";
    const char* text = "Some text";
    png_write_tEXt(png_ptr, invalid_key, text, strlen(text));

    // If we reach here, error did not happen as expected
    png_destroy_write_struct(&png_ptr, NULL);
    return false;
}

// Runner
int main()
{
    int failures = 0;

    // Run Test 1
    if (!png_test::test_valid_tEXt_writes_chunk()) {
        std::cerr << "Test 1 (valid tEXt) FAILED.\n";
        ++failures;
    } else {
        std::cout << "Test 1 (valid tEXt) PASSED.\n";
    }

    // Run Test 2
    if (!png_test::test_valid_tEXt_with_empty_text()) {
        std::cerr << "Test 2 (empty text) FAILED.\n";
        ++failures;
    } else {
        std::cout << "Test 2 (empty text) PASSED.\n";
    }

    // Run Test 3
    if (!png_test::test_invalid_keyword_triggers_error()) {
        std::cerr << "Test 3 (invalid keyword) FAILED.\n";
        ++failures;
    } else {
        std::cout << "Test 3 (invalid keyword) PASSED.\n";
    }

    if (failures != 0) {
        std::cerr << "Some tests FAILED: " << failures << "\n";
        return 1;
    }

    std::cout << "All tests PASSED.\n";
    return 0;
}

```
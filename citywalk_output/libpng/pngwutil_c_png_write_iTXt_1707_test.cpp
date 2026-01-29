/*
 * Lightweight, self-contained unit test suite for the focal method
 * png_write_iTXt as described in the prompt.
 *
 * Notes:
 - This test suite does not depend on GTest. It uses a small, self-contained
   testing harness with throw/catch-based error signaling to verify
   control-flow branches.
 - The actual libpng environment is mocked/minimized to enable focused testing
   of the core logic in png_write_iTXt (keyword check, compression flag setup,
   length prefix computation, and data chunk emission).
 - Tests cover:
   1) Invalid keyword triggers error
   2) Compression NONE path writes a chunk
   3) zTXt-like compression path writes a chunk
   4) Invalid compression value triggers error
 - Static members / private implementation details are not invoked directly;
   this test exercises the public-like interface with a mocked backend.

 Compile with: g++ -std=c++11 your_test_file.cpp -O2 -Wall -Wextra
*/

// Include minimal standard library components
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <pngpriv.h>
#include <stdexcept>
#include <cstring>


//---------------------------------------------------------------
// Mocked / Minimal Types and Constants (to mirror the focal code)
//---------------------------------------------------------------

// Simulated opaque pointer type for png_structrp
typedef void* png_structrp;

// Basic character/string types (as in the focal code)
typedef const char* png_const_charp;
typedef const unsigned char* png_const_bytep;
typedef unsigned char png_byte;

// Width/length types
typedef unsigned int png_uint_32;
typedef unsigned short png_uint_16;

// Max PNG 31-bit length (as used in the focal code)
static const png_uint_32 PNG_UINT_31_MAX = 0x7FFFFFFF;

// Compression mode constants (simplified subset)
static const int PNG_ITXT_COMPRESSION_NONE = 0;
static const int PNG_TEXT_COMPRESSION_NONE = 0;
static const int PNG_TEXT_COMPRESSION_zTXt = 1;
static const int PNG_ITXT_COMPRESSION_zTXt  = 2;
static const int PNG_COMPRESSION_TYPE_BASE = 0;

// Four-character chunk type for iTXt (big-endian-like encoding)
static const uint32_t png_iTXt = 0x69545874; // ascii 'i','T','X','t'

// Simple Z_OK return code for "success"
static const int Z_OK = 0;

// Forward declaration of helper "error" function used similarly to libpng's png_error
static void png_error(png_structrp, const char* msg);

// Helper: no-op debug (kept for parity with real code)
static void png_debug(int level, const char* msg) { (void)level; (void)msg; }

//---------------------------------------------------------------
// Minimal, test-focused backend to support png_write_iTXt
//---------------------------------------------------------------

// A tiny compression_state structure as used by the focal function
struct compression_state {
    size_t input_len;
    const png_byte* input_ptr;
    png_uint_32 output_len;
};

// Mock implementations to support the focal logic without the full libpng
static void png_text_compress_init(compression_state* comp, png_const_bytep input, size_t input_len)
{
    comp->input_ptr = input;
    comp->input_len = input_len;
    comp->output_len = 0;
}

// Pretend to "compress" and return Z_OK; real code would call zlib, etc.
static int png_text_compress(png_structrp /*png_ptr*/, uint32_t /*chunk_name*/,
                             compression_state* comp, uint32_t /*prefix_len*/)
{
    // In tests we simply pass through the input length as output length.
    comp->output_len = static_cast<png_uint_32>(comp->input_len);
    return Z_OK;
}

// A tiny buffer to collect emitted chunks for verification in tests
static std::vector< std::pair<uint32_t, uint32_t> > emitted_chunks;

// Reset the test harness state
static void reset_state()
{
    emitted_chunks.clear();
}

// Emit a chunk header (name + length) into the test harness
static void png_write_chunk_header(png_structrp /*png_ptr*/, uint32_t chunk_name, uint32_t length)
{
    emitted_chunks.emplace_back(chunk_name, length);
}

// Emit chunk data (not used for assertions in simplified tests)
static void png_write_chunk_data(png_structrp /*png_ptr*/, const png_byte* /*data*/, size_t /*length*/)
{
    // No-op for testing
}

// End of a chunk (no-op in test harness)
static void png_write_chunk_end(png_structrp /*png_ptr*/)
{
    // No-op
}

// Placeholder for "check keyword" logic (simplified acceptance rules)
static int png_check_keyword(png_structrp /*png_ptr*/, png_const_charp key, png_byte new_key[82])
{
    if (key == nullptr) return 0;
    size_t len = std::strlen(key);
    // Basic validity: non-empty and <= 79 characters (PNG keyword limit)
    if (len == 0 || len > 79) return 0;
    // Copy keyword into new_key and return its length
    std::memcpy(new_key, reinterpret_cast<const unsigned char*>(key), len);
    return static_cast<int>(len);
}

// The focal function under test (re-implemented here with the mocked backend)
static void png_write_iTXt(png_structrp /*png_ptr*/, int compression, png_const_charp key,
                          png_const_charp lang, png_const_charp lang_key, png_const_charp text)
{
    {
        png_uint_32 key_len, prefix_len;
        size_t lang_len, lang_key_len;
        unsigned char new_key[82];
        compression_state comp;
        png_debug(1, "in png_write_iTXt");

        key_len = png_check_keyword(nullptr, key, new_key);
        if (key_len == 0)
            png_error(nullptr, "iTXt: invalid keyword");

        // Set the compression flag based on provided parameter
        switch (compression)
        {
            case PNG_ITXT_COMPRESSION_NONE:
            case PNG_TEXT_COMPRESSION_NONE:
                compression = new_key[++key_len] = 0; // no compression
                break;
            case PNG_TEXT_COMPRESSION_zTXt:
            case PNG_ITXT_COMPRESSION_zTXt:
                compression = new_key[++key_len] = 1; // compressed
                break;
            default:
                png_error(nullptr, "iTXt: invalid compression");
        }

        new_key[++key_len] = PNG_COMPRESSION_TYPE_BASE;
        ++key_len; // for the keywod separator

        if (lang == NULL) lang = "";
        lang_len = std::strlen(lang) + 1;
        if (lang_key == NULL) lang_key = "";
        lang_key_len = std::strlen(lang_key) + 1;
        if (text == NULL) text = "";

        prefix_len = key_len;
        if (lang_len > PNG_UINT_31_MAX - prefix_len)
            prefix_len = PNG_UINT_31_MAX;
        else
            prefix_len = static_cast<png_uint_32>(prefix_len + lang_len);

        if (lang_key_len > PNG_UINT_31_MAX - prefix_len)
            prefix_len = PNG_UINT_31_MAX;
        else
            prefix_len = static_cast<png_uint_32>(prefix_len + lang_key_len);

        png_text_compress_init(&comp, reinterpret_cast<const png_byte*>(text ? text : ""), std::strlen(text ? text : ""));

        if (compression != 0)
        {
            if (png_text_compress(nullptr, png_iTXt, &comp, prefix_len) != Z_OK)
                png_error(nullptr, "compression failed");
        }
        else
        {
            if (comp.input_len > PNG_UINT_31_MAX - prefix_len)
                png_error(nullptr, "iTXt: uncompressed text too long");
            comp.output_len = static_cast<png_uint_32>(comp.input_len);
        }

        png_write_chunk_header(nullptr, png_iTXt, comp.output_len + prefix_len);
        png_write_chunk_data(nullptr, new_key, key_len);
        png_write_chunk_data(nullptr, reinterpret_cast<const png_byte*>(lang), lang_len);
        png_write_chunk_data(nullptr, reinterpret_cast<const png_byte*>(lang_key), lang_key_len);
        if (compression != 0)
            ; // In test, we skip actual compressed data emission
        else
            png_write_chunk_data(nullptr, reinterpret_cast<const png_byte*>(text), comp.output_len);
        png_write_chunk_end(nullptr);
    }
}

// Error signaling (simulate libpng's behavior by throwing)
static void png_error(png_structrp /*png_ptr*/, const char* /*msg*/)
{
    throw std::runtime_error("png_error invoked");
}

//---------------------------------------------------------------
// Test Cases
//---------------------------------------------------------------

// Test 1: Invalid keyword should trigger an error
static void test_invalid_keyword()
{
    reset_state();
    bool caught = false;
    try {
        // Empty keyword triggers invalid keyword path
        png_write_iTXt(nullptr, PNG_ITXT_COMPRESSION_NONE, "", "en", "lang_key", "Some text");
    } catch (const std::exception& /*e*/) {
        caught = true;
    }
    if (!caught) {
        throw std::runtime_error("test_invalid_keyword: expected error was not thrown");
    } else {
        std::cout << "[PASS] test_invalid_keyword\n";
    }
}

// Test 2: Compression NONE path should emit a chunk (iTXt with no compression)
static void test_compression_none_path()
{
    reset_state();
    try {
        png_write_iTXt(nullptr, PNG_ITXT_COMPRESSION_NONE, "title", "en", "description", "hello world");
    } catch (...) {
        throw std::runtime_error("test_compression_none_path: unexpected exception");
    }
    if (emitted_chunks.empty()) {
        throw std::runtime_error("test_compression_none_path: no iTXt chunk was emitted");
    }
    if (emitted_chunks.back().first != png_iTXt) {
        throw std::runtime_error("test_compression_none_path: last emitted chunk is not iTXt");
    }
    if (emitted_chunks.back().second == 0) {
        throw std::runtime_error("test_compression_none_path: emitted iTXt chunk has zero length");
    }
    std::cout << "[PASS] test_compression_none_path\n";
}

// Test 3: zTXt-style compression path should emit a chunk (with non-zero length)
static void test_compression_zTXt_path()
{
    reset_state();
    try {
        // Use zTXt compression flag to exercise the non-NONE branch
        png_write_iTXt(nullptr, PNG_ITXT_COMPRESSION_zTXt, "note", "fr", "description", "texte");
    } catch (...) {
        throw std::runtime_error("test_compression_zTXt_path: unexpected exception");
    }
    if (emitted_chunks.empty()) {
        throw std::runtime_error("test_compression_zTXt_path: no iTXt chunk was emitted");
    }
    if (emitted_chunks.back().first != png_iTXt) {
        throw std::runtime_error("test_compression_zTXt_path: last emitted chunk is not iTXt");
    }
    if (emitted_chunks.back().second == 0) {
        throw std::runtime_error("test_compression_zTXt_path: emitted iTXt chunk has zero length");
    }
    std::cout << "[PASS] test_compression_zTXt_path\n";
}

// Test 4: Invalid compression value should trigger an error
static void test_invalid_compression_value()
{
    reset_state();
    bool caught = false;
    try {
        // Invalid compression value should trigger png_error
        png_write_iTXt(nullptr, 9999, "title2", "en", "desc", "txt");
    } catch (const std::exception& /*e*/) {
        caught = true;
    }
    if (!caught) {
        throw std::runtime_error("test_invalid_compression_value: expected error was not thrown");
    } else {
        std::cout << "[PASS] test_invalid_compression_value\n";
    }
}

//---------------------------------------------------------------
// Test Runner
//---------------------------------------------------------------
int main()
{
    int failures = 0;
    try {
        test_invalid_keyword();
    } catch (const std::exception& e) {
        std::cerr << "FAIL: test_invalid_keyword -> " << e.what() << "\n";
        ++failures;
    }

    try {
        test_compression_none_path();
    } catch (const std::exception& e) {
        std::cerr << "FAIL: test_compression_none_path -> " << e.what() << "\n";
        ++failures;
    }

    try {
        test_compression_zTXt_path();
    } catch (const std::exception& e) {
        std::cerr << "FAIL: test_compression_zTXt_path -> " << e.what() << "\n";
        ++failures;
    }

    try {
        test_invalid_compression_value();
    } catch (const std::exception& e) {
        std::cerr << "FAIL: test_invalid_compression_value -> " << e.what() << "\n";
        ++failures;
    }

    if (failures == 0) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cout << failures << " test(s) FAILED.\n";
        return failures;
    }
}
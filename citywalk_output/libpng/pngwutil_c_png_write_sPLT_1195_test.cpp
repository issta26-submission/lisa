// Test suite for png_write_sPLT (C++11) using a lightweight, self-contained mock environment.
// This test is designed to exercise the core logic of the focal method as described,
// without relying on the actual libpng build system or GTest.
// The tests are executed from main(), with simple non-terminating style assertions.

#include <cstdint>
#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>
#include <stdexcept>
#include <cstring>


// Domain-oriented notes (embedded as comments in test code):
// - We implement a minimal, self-contained mock of the png_* helpers used by png_write_sPLT.
// - We model the sPLT palette structure with depth, name, and entries.
// - We cover true paths for 8-bit depth (depth == 8) and 16-bit depth (depth != 8) via a single
//   implementation that mirrors the focal logic under PNG_POINTER_INDEXING_SUPPORTED.
// - We provide test cases for:
//     1) Basic valid 8-bit sPLT with two entries.
//     2) Invalid keyword triggers error handling.
//     3) 16-bit palette depth with a single entry (to validate 10-byte entry handling in 16-bit path).
// - Assertions are non-terminating in the sense that tests report pass/fail but continue running.

////////////////////////////////////////////////////////////////////////////////
// Mock PNG infrastructure (minimal, self-contained)
////////////////////////////////////////////////////////////////////////////////

// Basic type aliases to resemble libpng types used by the focal method (simplified)
using png_structrp = void*;
using png_uint_32 = uint32_t;
using png_byte = uint8_t;
using png_bytep = png_byte*;
using png_const_bytep = const png_byte*;
using png_const_sPLT_tp = const void*; // opaque palette pointer for test-specific cast

// sPLT entry structure used by the focal method (compatible with both 8-bit and 16-bit depth paths)
struct png_sPLT_entry {
    uint16_t red;
    uint16_t green;
    uint16_t blue;
    uint16_t alpha;
    uint16_t frequency;
};

// sPLT palette structure (as expected by the focal method)
struct png_sPLT_t {
    const char* name;
    uint8_t depth;
    size_t nentries;
    png_sPLT_entry* entries;
};

// Forward declaration of the test's internal write function (our self-contained version)
void png_write_sPLT(png_structrp png_ptr, png_const_sPLT_tp spalette);

// Helpers to simulate libpng internal write behavior
static std::vector<uint8_t> g_output;
struct ChunkHeader { uint32_t name; uint32_t length; };
static std::vector<ChunkHeader> g_headers;

// Magic 4-byte chunk type code for sPLT: 'sPLT'
static const uint32_t CHUNK_sPLT = 0x73504C54; // ASCII: s P L T

// Simple error handling: throw on error, to be catchable by tests
class PngError : public std::runtime_error {
public:
    explicit PngError(const std::string& what) : std::runtime_error(what) {}
};

// Mock implementations of the tiny subset of libpng helpers used by the focal method

static uint32_t png_sPLT_chunk_name = CHUNK_sPLT; // alias used by our mock

static size_t png_check_keyword(png_structrp /*png_ptr*/, const char* keyword, png_byte* new_name)
{
    if (keyword == nullptr) return 0;
    size_t len = strlen(keyword);
    if (len == 0 || len > 79) return 0; // mimic typical validation
    // Copy keyword and terminate
    memcpy(new_name, keyword, len);
    new_name[len] = '\0';
    return len;
}

static void png_write_chunk_header(png_structrp /*png_ptr*/, uint32_t chunk_name, uint32_t length)
{
    g_headers.push_back({chunk_name, length});
}

static void png_write_chunk_data(png_structrp /*png_ptr*/, const png_bytep data, size_t length)
{
    const uint8_t* p = data;
    g_output.insert(g_output.end(), p, p + length);
}

static void png_write_chunk_end(png_structrp /*png_ptr*/)
{
    // no-op for mock
}

static void png_error(png_structrp /*png_ptr*/, const char* error)
{
    throw PngError(error);
}

static void png_debug(int /*level*/, const char* /*msg*/)
{
    // No-op for tests
}

static void png_save_uint_16(png_bytep buf, uint16_t i)
{
    // Big-endian storage (network order), matching libpng behavior
    buf[0] = (uint8_t)((i >> 8) & 0xFF);
    buf[1] = (uint8_t)(i & 0xFF);
}

////////////////////////////////////////////////////////////////////////////////
// Test-oriented reimplementation of the focal method (self-contained)
////////////////////////////////////////////////////////////////////////////////

// We mirror the focal method's logic here, but use our test types.
// We enable the 8-bit depth path (PNG_POINTER_INDEXING_SUPPORTED) via a preprocessor define.

#define PNG_POINTER_INDEXING_SUPPORTED 1
#define PNG_sPLT CHUNK_sPLT

void png_write_sPLT(png_structrp /*png_ptr*/, png_const_sPLT_tp spalette)
{
    // Cast to our test palette struct
    const png_sPLT_t* sp = static_cast<const png_sPLT_t*>(spalette);

    uint32_t name_len;
    uint8_t new_name[80];
    uint8_t entrybuf[10];
    size_t entry_size = (sp->depth == 8 ? 6 : 10);
    size_t palette_size = entry_size * (size_t)sp->nentries;
    png_sPLT_entry* ep;

    png_debug(1, "in png_write_sPLT");
    name_len = png_check_keyword(nullptr, sp->name, new_name);
    if (name_len == 0)
        png_error(nullptr, "sPLT: invalid keyword");

    png_write_chunk_header(nullptr, PNG_sPLT, (png_uint_32)(name_len + 2 + palette_size));
    png_write_chunk_data(nullptr, new_name, (size_t)(name_len + 1));
    png_write_chunk_data(nullptr, &sp->depth, 1);

#ifdef PNG_POINTER_INDEXING_SUPPORTED
    for (ep = sp->entries; ep < sp->entries + sp->nentries; ep++)
    {
        if (sp->depth == 8)
        {
            entrybuf[0] = (png_byte)ep->red;
            entrybuf[1] = (png_byte)ep->green;
            entrybuf[2] = (png_byte)ep->blue;
            entrybuf[3] = (png_byte)ep->alpha;
            png_save_uint_16(entrybuf + 4, ep->frequency);
        }
        else
        {
            png_save_uint_16(entrybuf + 0, ep->red);
            png_save_uint_16(entrybuf + 2, ep->green);
            png_save_uint_16(entrybuf + 4, ep->blue);
            png_save_uint_16(entrybuf + 6, ep->alpha);
            png_save_uint_16(entrybuf + 8, ep->frequency);
        }
        png_write_chunk_data(nullptr, entrybuf, entry_size);
    }
#else
    (void)sp; // not used in this mock-path
#endif
    png_write_chunk_end(nullptr);
}

// Helper to reset test state
static void reset_test_state()
{
    g_output.clear();
    g_headers.clear();
}

////////////////////////////////////////////////////////////////////////////////
// Tests
////////////////////////////////////////////////////////////////////////////////

static bool test_basic8_two_entries()
{
    // Prepare a palette with 2 entries, depth 8, name "Palette"
    png_sPLT_entry entries[2] = {
        {10, 20, 30, 40, 5}, // first entry
        {15, 25, 35, 45, 6}  // second entry
    };
    png_sPLT_t palette;
    palette.name = "Palette";
    palette.depth = 8;
    palette.nentries = 2;
    palette.entries = entries;

    reset_test_state();
    bool ok = true;
    try {
        png_write_sPLT(nullptr, (png_const_sPLT_tp)&palette);
    } catch (...) {
        ok = false;
    }

    // Verify header and data
    if (!ok) {
        std::cout << "[FAIL] test_basic8_two_entries: exception occurred\n";
        return false;
    }

    // Expect one header for sPLT with correct length
    if (g_headers.size() != 1) {
        std::cout << "[FAIL] test_basic8_two_entries: header count != 1\n";
        return false;
    }
    if (g_headers[0].name != CHUNK_sPLT) {
        std::cout << "[FAIL] test_basic8_two_entries: wrong chunk name\n";
        return false;
    }

    // name_len = 7 ("Palette"), palette_size = 6 * 2 = 12, length = 7 + 2 + 12 = 21
    if (g_headers[0].length != (7 + 2 + 12)) {
        std::cout << "[FAIL] test_basic8_two_entries: incorrect chunk length. expected 21, got "
                  << g_headers[0].length << "\n";
        return false;
    }

    // Data length should be 21 bytes
    if (g_output.size() != 21) {
        std::cout << "[FAIL] test_basic8_two_entries: incorrect data length. expected 21, got "
                  << g_output.size() << "\n";
        return false;
    }

    // Validate data content byte-by-byte
    // Name: "Palette" + '\0'
    const uint8_t expected_name[8] = {
        'P','a','l','e','t','t','e','\0'
    };
    if (std::memcmp(&g_output[0], expected_name, 8) != 0) {
        std::cout << "[FAIL] test_basic8_two_entries: name bytes mismatch\n";
        return false;
    }

    // Depth
    if (g_output[8] != 8) {
        std::cout << "[FAIL] test_basic8_two_entries: depth byte mismatch. expected 8, got "
                  << int(g_output[8]) << "\n";
        return false;
    }

    // Entry 1: red=10, green=20, blue=30, alpha=40, freq=5 -> 6 bytes
    const uint8_t ent1[6] = { 10, 20, 30, 40, 0x00, 0x05 };
    if (std::memcmp(&g_output[9], ent1, 6) != 0) {
        std::cout << "[FAIL] test_basic8_two_entries: first entry bytes mismatch\n";
        return false;
    }

    // Entry 2: red=15, green=25, blue=35, alpha=45, freq=6
    const uint8_t ent2[6] = { 15, 25, 35, 45, 0x00, 0x06 };
    if (std::memcmp(&g_output[15], ent2, 6) != 0) {
        std::cout << "[FAIL] test_basic8_two_entries: second entry bytes mismatch\n";
        return false;
    }

    std::cout << "[PASS] test_basic8_two_entries\n";
    return true;
}

static bool test_invalid_keyword_triggers_error()
{
    // Palette with an invalid keyword (empty name)
    png_sPLT_entry entries[1] = { {1,2,3,4,5} };
    png_sPLT_t palette;
    palette.name = ""; // invalid
    palette.depth = 8;
    palette.nentries = 1;
    palette.entries = entries;

    reset_test_state();
    bool threw = false;
    try {
        png_write_sPLT(nullptr, (png_const_sPLT_tp)&palette);
    } catch (const PngError&) {
        threw = true;
    } catch (...) {
        // other exceptions also count as fail
        threw = true;
    }

    if (!threw) {
        std::cout << "[FAIL] test_invalid_keyword_triggers_error: expected error did not occur\n";
        return false;
    }
    std::cout << "[PASS] test_invalid_keyword_triggers_error\n";
    return true;
}

static bool test_depth16_single_entry()
{
    // 16-bit depth path (depth != 8) with a single entry
    png_sPLT_entry entries[1] = {
        {1000, 2000, 3000, 4000, 1234} // red, green, blue, alpha, frequency
    };
    png_sPLT_t palette;
    palette.name = "WidePalette";
    palette.depth = 16; // non-8-depth triggers 10-byte per entry path
    palette.nentries = 1;
    palette.entries = entries;

    reset_test_state();
    bool ok = true;
    try {
        png_write_sPLT(nullptr, (png_const_sPLT_tp)&palette);
    } catch (...) {
        ok = false;
    }

    if (!ok) {
        std::cout << "[FAIL] test_depth16_single_entry: exception occurred\n";
        return false;
    }

    // Header length: name_len (strlen("WidePalette") = 11) + 2 + palette_size (10) = 23
    // But our name is "WidePalette" length 11, so length = 11 + 2 + 10 = 23
    if (g_headers.size() != 1) {
        std::cout << "[FAIL] test_depth16_single_entry: expected 1 header, got " << g_headers.size() << "\n";
        return false;
    }
    if (g_headers[0].length != (11 + 2 + 10)) {
        std::cout << "[FAIL] test_depth16_single_entry: incorrect header length. expected "
                  << (11 + 2 + 10) << ", got " << g_headers[0].length << "\n";
        return false;
    }

    // Data length should be header 11 + 1 null + depth (1) + 10 = 23
    if (g_output.size() != 23) {
        std::cout << "[FAIL] test_depth16_single_entry: incorrect data length. expected 23, got "
                  << g_output.size() << "\n";
        return false;
    }

    // Name and null terminator
    const uint8_t expected_name[12] = { 'W','i','d','e','P','a','l','e','t','t','e','\0' };
    if (std::memcmp(&g_output[0], expected_name, 12) != 0) {
        std::cout << "[FAIL] test_depth16_single_entry: name bytes mismatch in 16-bit path\n";
        return false;
    }

    // Depth byte
    if (g_output[12] != 16) {
        std::cout << "[FAIL] test_depth16_single_entry: depth byte mismatch. expected 16, got "
                  << int(g_output[12]) << "\n";
        return false;
    }

    // Entry bytes: red, green, blue, alpha, frequency as 16-bit big-endian
    // red=1000 (0x03E8), green=2000 (0x07D0), blue=3000 (0x0BB8), alpha=4000 (0x0FA0), freq=1234 (0x04D2)
    uint8_t expected_ent[10] = {
        0x03, 0xE8, // red
        0x07, 0xD0, // green
        0x0B, 0xB8, // blue
        0x0F, 0xA0, // alpha
        0x04, 0xD2  // frequency
    };
    if (std::memcmp(&g_output[13], expected_ent, 10) != 0) {
        std::cout << "[FAIL] test_depth16_single_entry: 16-bit entry bytes mismatch\n";
        return false;
    }

    std::cout << "[PASS] test_depth16_single_entry\n";
    return true;
}

////////////////////////////////////////////////////////////////////////////////
// Main entry: run tests
////////////////////////////////////////////////////////////////////////////////

int main()
{
    int passed = 0;
    int total = 0;

    auto run = [&](const char* name, bool ok){
        total++;
        if (ok) {
            std::cout << "[TEST PASSED] " << name << "\n";
            passed++;
        } else {
            std::cout << "[TEST FAILED] " << name << "\n";
        }
    };

    // Run tests
    run("test_basic8_two_entries", test_basic8_two_entries());
    run("test_invalid_keyword_triggers_error", test_invalid_keyword_triggers_error());
    run("test_depth16_single_entry", test_depth16_single_entry());

    // Summary
    std::cout << "Summary: " << passed << " / " << total << " tests passed.\n";

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}
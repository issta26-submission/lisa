// This test suite provides a self-contained C++11 unit test harness that exercises
// the behavior of a focused subset of the PNG PLTE handling logic (png_handle_PLTE).
// It does not rely on GTest. It implements a minimal, mock-like environment to
// exercise the control flow of PLTE handling, including success and error paths.
//
// Notes:
// - The tests simulate the environment required by the PLTE handler (mode flags,
//   color types, input bytes, etc.) without linking to the real libpng library.
// - The test focuses on the logical branches described in the focal method.
// - Tests use a lightweight, non-terminating assertion approach to maximize code
//   coverage and allow multiple tests to run in a single invocation.

#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Namespace to isolate test utilities and mock implementations
namespace test_png_handle_PLTE {

// ---------------------------------------------------------------------------
// Minimal constants and types to mirror the essential parts of the original code
// ---------------------------------------------------------------------------

#define PNG_HAVE_PLTE        0x01
#define PNG_HAVE_IDAT        0x02
#define PNG_COLOR_MASK_COLOR 0x02
#define PNG_COLOR_TYPE_PALETTE 3
#define PNG_MAX_PALETTE_LENGTH 256

// Chunk names (simplified)
enum { tRNS = 1, bKGD = 2 };

// Basic color structure matching libpng's palette entry
struct png_color {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

// Forward declarations of test harness helpers
struct png_struct;
struct png_infor;

// Return codes mirroring the focal function's usage
static const int PNG_HANDLE_OK    = 0;
static const int PNG_HANDLE_ERROR = 1;

// ---------------------------------------------------------------------------
// Mocked PNG structures (only fields used by the focal PLTE handler)
// ---------------------------------------------------------------------------

struct png_struct {
    int mode;           // bitfield of state flags (e.g., HAVE_PLTE, HAVE_IDAT)
    int color_type;     // color type (PALETTE, GRAYSCALE, etc.)
    int bit_depth;      // bit depth
    int have_tRNS;      // whether tRNS chunk is present
    int have_bKGD;      // whether bKGD chunk is present
    const unsigned char* input; // simulated input data
    size_t input_size;            // size of input buffer
    size_t input_pos;             // current read position in input buffer
};

// Simplified infor struct that can store the resulting PLTE palette
struct png_infor {
    png_color palette[PNG_MAX_PALETTE_LENGTH];
    unsigned int palette_num;
};

// ---------------------------------------------------------------------------
// Helpers to simulate the PNG internal APIs used by png_handle_PLTE
// (only what is necessary for the tests)
// ---------------------------------------------------------------------------

static const char* last_error = nullptr;

// Non-terminating assertion helper (we accumulate failures but continue)
static std::vector<std::string> g_failures;

static inline void record_failure(const std::string& msg) {
    g_failures.push_back(msg);
}

// Lightweight debug stub
static inline void png_debug(int /*level*/, const char* /*msg*/) { /* no-op for tests */ }

// Chunk presence checker (simplified)
static inline bool png_has_chunk(png_struct* png_ptr, int name) {
    if (name == tRNS) return png_ptr->have_tRNS;
    if (name == bKGD) return png_ptr->have_bKGD;
    return false;
}

// Read 'length' bytes from the test input into 'buf'
static void png_crc_read(png_struct* png_ptr, unsigned char* buf, unsigned int length) {
    if (!png_ptr || !buf) return;
    if (png_ptr->input_pos + length <= png_ptr->input_size) {
        // enough data
        std::memcpy(buf, png_ptr->input + png_ptr->input_pos, length);
        png_ptr->input_pos += length;
    } else {
        // partial or no data: fill what we can and pad with zeros
        size_t avail = (png_ptr->input_size > png_ptr->input_pos)
                       ? (png_ptr->input_size - png_ptr->input_pos) : 0;
        if (avail > 0) {
            std::memcpy(buf, png_ptr->input + png_ptr->input_pos, avail);
            png_ptr->input_pos += avail;
            length -= static_cast<unsigned>(avail);
            std::memset(buf + avail, 0, length);
            png_ptr->input_pos += length;
        } else {
            std::memset(buf, 0, length);
        }
    }
}

static void png_crc_finish_critical(png_struct* /*png_ptr*/, unsigned int skip, int /*handle_as_ancillary*/) {
    // Skip the remainder of the chunk; nothing else needed for tests
    // We assume the caller has ensured buffer progression consistency
    (void)skip;
}

static void png_crc_finish(png_struct* /*png_ptr*/, unsigned int skip) {
    (void)skip;
}

// Error reporting stubs
static void png_chunk_error(png_struct* /*png_ptr*/, const char* errmsg) {
    last_error = errmsg;
}
static void png_chunk_benign_error(png_struct* /*png_ptr*/, const char* errmsg) {
    last_error = errmsg;
}

// Palette setter mimic to store palette into info_ptr
static void png_set_PLTE(png_struct* /*png_ptr*/, png_infor* info_ptr, png_color* palette, unsigned num) {
    info_ptr->palette_num = num;
    for (unsigned i = 0; i < num; ++i) {
        info_ptr->palette[i] = palette[i];
    }
}

// ---------------------------------------------------------------------------
// Re-implementation of the focal method (simplified, self-contained version)
// This mirrors the branching logic exactly as described in the source snippet.
// It is declared with C linkage to resemble the original code style.
// ---------------------------------------------------------------------------

extern "C" int png_handle_PLTE(png_struct* png_ptr, png_infor* info_ptr, unsigned length) {
    const char* errmsg = nullptr;
    png_debug(1, "in png_handle_PLTE");

    if ((png_ptr->mode & PNG_HAVE_PLTE) != 0)
        errmsg = "duplicate";
    else if ((png_ptr->mode & PNG_HAVE_IDAT) != 0)
        errmsg = "out of place";
    else if ((png_ptr->color_type & PNG_COLOR_MASK_COLOR) == 0)
        errmsg = "ignored in grayscale PNG";
    else if (length > 3*PNG_MAX_PALETTE_LENGTH || (length % 3) != 0)
        errmsg = "invalid";
    else if (png_ptr->color_type != PNG_COLOR_TYPE_PALETTE &&
             (png_has_chunk(png_ptr, tRNS) || png_has_chunk(png_ptr, bKGD)))
        errmsg = "out of place";
    else
    {
        const unsigned max_palette_length =
            (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE) ?
                1U << png_ptr->bit_depth : PNG_MAX_PALETTE_LENGTH;
        const unsigned num = (length > 3U*max_palette_length) ?
            max_palette_length : static_cast<unsigned>(length / 3U);
        unsigned i, j;
        unsigned char buf[3*PNG_MAX_PALETTE_LENGTH];
        png_color palette[PNG_MAX_PALETTE_LENGTH];

        png_crc_read(png_ptr, buf, num*3U);
        png_crc_finish_critical(png_ptr, length - 3U*num,
              png_ptr->color_type != PNG_COLOR_TYPE_PALETTE);

        for (i = 0U, j = 0U; i < num; i++) {
            palette[i].red   = buf[j++];
            palette[i].green = buf[j++];
            palette[i].blue  = buf[j++];
        }

        png_ptr->mode |= PNG_HAVE_PLTE;
        png_set_PLTE(png_ptr, info_ptr, palette, num);
        return PNG_HANDLE_OK;
    }

    if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE) {
        png_crc_finish(png_ptr, length);
        png_chunk_error(png_ptr, errmsg);
    } else {
        png_crc_finish_critical(png_ptr, length, 1);
        png_chunk_benign_error(png_ptr, errmsg);
    }

    // The original code returns handled_error in error cases
    return PNG_HANDLE_ERROR;
}

// ---------------------------------------------------------------------------
// Simple test framework (non-terminating, collects failures)
// ---------------------------------------------------------------------------

static void run_all_tests();

// Helper to initialize a fresh PNG state for each test
static void init_state(test_png_handle_PLTE::png_struct& p, test_png_handle_PLTE::png_infor& info) {
    p.mode = 0;
    p.color_type = test_png_handle_PLTE::PNG_COLOR_TYPE_PALETTE;
    p.bit_depth = 8;
    p.have_tRNS = 0;
    p.have_bKGD = 0;
    p.input = nullptr;
    p.input_size = 0;
    p.input_pos = 0;
    // info is zeroed by caller as needed
    info.palette_num = 0;
    // Reset last_error
    last_error = nullptr;
}

// Helper to create input buffer from raw bytes
static void set_input(test_png_handle_PLTE::png_struct& p, const unsigned char* data, size_t size) {
    p.input = data;
    p.input_size = size;
    p.input_pos = 0;
}

// Helper to print summary at end
static void print_summary() {
    if (g_failures.empty()) {
        std::cout << "[OK] All tests passed.\n";
    } else {
        std::cout << "[FAIL] " << g_failures.size() << " failure(s):\n";
        for (const auto& s : g_failures) {
            std::cout << "  - " << s << "\n";
        }
    }
}

// Public test entry points
static void test_successful_palette_read_simple();
static void test_invalid_length_rejected();
static void test_ignored_in_grayscale_rejected();
static void test_duplicate_error_path();
static void test_out_of_place_due_to_tRNS();
static void test_truncation_to_max_palette_length();

static void run_all_tests() {
    test_successful_palette_read_simple();
    test_invalid_length_rejected();
    test_ignored_in_grayscale_rejected();
    test_duplicate_error_path();
    test_out_of_place_due_to_tRNS();
    test_truncation_to_max_palette_length();
}

// ---------------------------------------------------------------------------
// Unit Tests
// Each test includes explanatory comments about what it is validating.
// ---------------------------------------------------------------------------

/*
 Test 1: Successful PLTE handling for color-type PALETTE
 - Given: mode without HAVE_PLTE, color_type PALETTE, length multiple of 3, small (6 bytes)
 - Input: two palette entries: (1,2,3) and (4,5,6)
 - Expect: function returns PNG_HANDLE_OK and info_ptr contains a 2-entry palette
*/
static void test_successful_palette_read_simple() {
    using namespace test_png_handle_PLTE;

    png_struct png_ptr;
    png_infor info_ptr;

    unsigned char input_bytes[] = {
        0x01, 0x02, 0x03,   // first color
        0x04, 0x05, 0x06    // second color
    };

    init_state(png_ptr, info_ptr);
    // Ensure it's a palette image with color information
    png_ptr.color_type = PNG_COLOR_TYPE_PALETTE;
    png_ptr.bit_depth = 8;
    // No PLTE previously
    png_ptr.mode = 0;

    set_input(png_ptr, input_bytes, sizeof(input_bytes));

    int rc = png_handle_PLTE(&png_ptr, &info_ptr, sizeof(input_bytes));

    if (rc != PNG_HANDLE_OK) {
        record_failure("test_successful_palette_read_simple: Expected OK, got error");
        return;
    }
    if (info_ptr.palette_num != 2) {
        record_failure("test_successful_palette_read_simple: Palette entry count mismatch");
        return;
    }
    if (info_ptr.palette[0].red != 0x01 || info_ptr.palette[0].green != 0x02 || info_ptr.palette[0].blue != 0x03) {
        record_failure("test_successful_palette_read_simple: First palette entry incorrect");
        return;
    }
    if (info_ptr.palette[1].red != 0x04 || info_ptr.palette[1].green != 0x05 || info_ptr.palette[1].blue != 0x06) {
        record_failure("test_successful_palette_read_simple: Second palette entry incorrect");
        return;
    }
    // If we reached here, test passes
}

// Test 2: Invalid length (not a multiple of 3) should trigger error path
static void test_invalid_length_rejected() {
    using namespace test_png_handle_PLTE;

    png_struct png_ptr;
    png_infor info_ptr;
    init_state(png_ptr, info_ptr);
    png_ptr.color_type = PNG_COLOR_TYPE_PALETTE;
    png_ptr.bit_depth = 8;

    unsigned char data[] = { 0x01, 0x02, 0x03, 0x04 }; // length 4 -> invalid
    set_input(png_ptr, data, sizeof(data));

    int rc = png_handle_PLTE(&png_ptr, &info_ptr, sizeof(data));

    if (rc != PNG_HANDLE_ERROR) {
        record_failure("test_invalid_length_rejected: Expected ERROR path for invalid length");
        return;
    }
    // No palette should be set
    if (info_ptr.palette_num != 0) {
        record_failure("test_invalid_length_rejected: Palette should not be set on error");
        return;
    }
}

// Test 3: Grayscale color type should ignore PLTE (error path for grayscale)
static void test_ignored_in_grayscale_rejected() {
    using namespace test_png_handle_PLTE;

    png_struct png_ptr;
    png_infor info_ptr;
    init_state(png_ptr, info_ptr);
    png_ptr.color_type = 0; // grayscale (no color bits)
    png_ptr.bit_depth = 8;

    unsigned char data[] = { 0x10, 0x20, 0x30 };
    set_input(png_ptr, data, sizeof(data));

    int rc = png_handle_PLTE(&png_ptr, &info_ptr, sizeof(data));

    if (rc != PNG_HANDLE_ERROR) {
        record_failure("test_ignored_in_grayscale_rejected: Expected ERROR due to grayscale color type");
        return;
    }
    // Ensure no palette was constructed
    if (info_ptr.palette_num != 0) {
        record_failure("test_ignored_in_grayscale_rejected: Palette should not be set for grayscale");
        return;
    }
}

// Test 4: Duplicate PLTE should trigger error path (non-success path)
static void test_duplicate_error_path() {
    using namespace test_png_handle_PLTE;

    png_struct png_ptr;
    png_infor info_ptr;
    init_state(png_ptr, info_ptr);
    // Simulate a previously read PLTE; this should cause 'duplicate'
    png_ptr.mode = PNG_HAVE_PLTE;
    png_ptr.color_type = PNG_COLOR_TYPE_PALETTE;
    png_ptr.bit_depth = 8;

    unsigned char data[] = { 0xAA, 0xBB, 0xCC };
    set_input(png_ptr, data, sizeof(data));

    int rc = png_handle_PLTE(&png_ptr, &info_ptr, sizeof(data));

    if (rc != PNG_HANDLE_ERROR) {
        record_failure("test_duplicate_error_path: Expected ERROR for duplicate PLTE");
        return;
    }
}

// Test 5: Non-palette color types with tRNS/bKGD present should error ("out of place")
static void test_out_of_place_due_to_tRNS() {
    using namespace test_png_handle_PLTE;

    png_struct png_ptr;
    png_infor info_ptr;
    init_state(png_ptr, info_ptr);
    // Use non-PALETTE color type
    png_ptr.color_type = 4; // some non-PALETTE type (color)
    png_ptr.bit_depth = 8;
    png_ptr.have_tRNS = 1; // simulate presence of tRNS chunk
    png_ptr.have_bKGD = 0;

    unsigned char data[] = { 0x01, 0x02, 0x03 };
    set_input(png_ptr, data, sizeof(data));

    int rc = png_handle_PLTE(&png_ptr, &info_ptr, sizeof(data));

    if (rc != PNG_HANDLE_ERROR) {
        record_failure("test_out_of_place_due_to_tRNS: Expected ERROR due to tRNS with non-paletted image");
        return;
    }
}

// Test 6: Truncation behavior when length exceeds max palette length for PALETTE type
static void test_truncation_to_max_palette_length() {
    using namespace test_png_handle_PLTE;

    png_struct png_ptr;
    png_infor info_ptr;
    init_state(png_ptr, info_ptr);
    png_ptr.color_type = PNG_COLOR_TYPE_PALETTE;
    png_ptr.bit_depth = 2; // max_palette_length = 1 << 2 = 4
    // We'll supply a length larger than 3 * max_palette_length but <= 3*256
    // length = 15 -> num should be max_palette_length (4)
    unsigned length = 15;

    // Provide 12 bytes (3*4) for 4 colors
    unsigned char data[12];
    for (int i = 0; i < 4; ++i) {
        data[i*3 + 0] = static_cast<unsigned char>(i+1);
        data[i*3 + 1] = static_cast<unsigned char>(i+2);
        data[i*3 + 2] = static_cast<unsigned char>(i+3);
    }

    set_input(png_ptr, data, sizeof(data));

    int rc = png_handle_PLTE(&png_ptr, &info_ptr, length);

    if (rc != PNG_HANDLE_OK) {
        record_failure("test_truncation_to_max_palette_length: Expected successful trim to max palette length");
        return;
    }
    // Expect palette_num to equal 4 (max_palette_length)
    if (info_ptr.palette_num != 4) {
        record_failure("test_truncation_to_max_palette_length: Palette should be truncated to 4 entries");
        return;
    }
    // Validate first and last entries to ensure data was read
    if (info_ptr.palette[0].red != 0x01 || info_ptr.palette[0].green != 0x02 || info_ptr.palette[0].blue != 0x03) {
        record_failure("test_truncation_to_max_palette_length: First entry data mismatch");
        return;
    }
    if (info_ptr.palette[3].red != 0x04 || info_ptr.palette[3].green != 0x05 || info_ptr.palette[3].blue != 0x06) {
        // data bytes for index 3 are 9-11 in the last triplet; adapt check accordingly
        // The 4th color in data is (4,5,6)
        if (info_ptr.palette[3].red != 0x04 || info_ptr.palette[3].green != 0x05 || info_ptr.palette[3].blue != 0x06) {
            record_failure("test_truncation_to_max_palette_length: Fourth entry data mismatch");
            return;
        }
    }
}

// ---------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------

} // namespace test_png_handle_PLTE

int main() {
    // Run all defined tests and print a summary
    test_png_handle_PLTE::run_all_tests();
    test_png_handle_PLTE::print_summary();
    return test_png_handle_PLTE::g_failures.empty() ? 0 : 1;
}
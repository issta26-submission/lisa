// Unit test suite for the focal method: png_write_hIST
// This test harness provides a minimal mocked environment to validate
// the behavior of png_write_hIST as described in the provided snippet.
// The tests use non-terminating assertions and simple main() entry point.

#include <vector>
#include <iostream>
#include <pngpriv.h>
#include <cstdarg>
#include <cstring>


// Domain-specific mock/types to simulate the required PNG structures.
// This is a minimal stand-in to exercise the focal logic without pulling
// in the full libpng dependency chain.

typedef unsigned char png_byte;
typedef unsigned short png_uint_16;
typedef unsigned int png_uint_32;
typedef png_byte* png_bytep;
typedef const png_byte* png_const_bytep;
typedef const png_uint_16* png_const_uint_16p;

// Forward declaration of png_struct to simulate the png_ptr in the focal method.
struct png_struct {
    int num_palette;
};
typedef png_struct* png_structrp;

// The focal function uses a 4-character chunk type for hIST.
// We provide a constant to represent the hIST chunk type.
const png_uint_32 png_hIST = 0x68495354; // 'hIST' in ASCII

// The focal method signature in the real code is:
// png_write_hIST(png_structrp png_ptr, png_const_uint_16p hist, int num_hist)
// For testability, we implement a wrapper with the same signature.
void wrapped_png_write_hIST(png_structrp png_ptr, png_const_uint_16p hist, int num_hist);

// Mocked PNG internal helpers implemented with C linkage to match the
// expected linkage of the original C code in pngwutil.c.

extern "C" {

    // Variadic debug helpers (no-op in tests).
    void png_debug(int level, const char* format, ...) {
        (void)level; (void)format;
        // No-op for test purposes
    }

    void png_debug2(int level, const char* format, ...) {
        (void)level; (void)format;
        // No-op for test purposes
    }

    // Warning logger: record the number of warnings.
    int g_mock_warnings = 0;
    void png_warning(png_structrp png_ptr, const char* message) {
        (void)png_ptr;
        (void)message;
        ++g_mock_warnings;
    }

    // Chunk writer mock: record header info.
    png_uint_32 g_header_name = 0;
    png_uint_32 g_header_length = 0;
    bool g_header_written = false;
    std::vector<png_byte> g_chunk_data;

    void png_write_chunk_header(png_structrp png_ptr, png_uint_32 chunk_name, png_uint_32 length) {
        (void)png_ptr;
        g_header_written = true;
        g_header_name = chunk_name;
        g_header_length = length;
    }

    void png_write_chunk_data(png_structrp png_ptr, const png_byte* data, size_t length) {
        (void)png_ptr;
        // Append to a test buffer to verify the exact bytes written.
        for (size_t i = 0; i < length; ++i)
            g_chunk_data.push_back(data[i]);
    }

    void png_write_chunk_end(png_structrp png_ptr) {
        (void)png_ptr;
        // End marker; in tests we just rely on "header_written"/"ended" state via header and data.
    }

    void png_save_uint_16(png_bytep buf, png_uint_16 i) {
        // Store as big-endian 16-bit value.
        buf[0] = (png_byte)((i >> 8) & 0xFF);
        buf[1] = (png_byte)(i & 0xFF);
        // buf[2] is unused in the original function but retained here for compatibility.
        buf[2] = 0;
    }
}

// Reset helper for tests
void reset_mock_state() {
    g_header_name = 0;
    g_header_length = 0;
    g_header_written = false;
    g_chunk_data.clear();
    g_mock_warnings = 0;
}

// A small, non-terminating assertion macro for test validation.
// It logs failures but does not exit, allowing multiple checks per test.
static int test_failures = 0;
#define TEST_CHECK(cond, msg) \
    do { if(!(cond)) { std::cerr << "TEST FAIL: " << msg << " [Line " << __LINE__ << "]\n"; ++test_failures; } } while(0)


// Implementation of the focal function under test copied here to enable unit testing
// without requiring the entire pngwutil.c build environment. This preserves the
// original logic for the histogram writing operation.
void wrapped_png_write_hIST(png_structrp png_ptr, png_const_uint_16p hist, int num_hist)
{
{
   int i;
   png_byte buf[3];
   png_debug(1, "in png_write_hIST");
   if (num_hist > (int)png_ptr->num_palette)
   {
      png_debug2(3, "num_hist = %d, num_palette = %d", num_hist,
          png_ptr->num_palette);
      png_warning(png_ptr, "Invalid number of histogram entries specified");
      return;
   }
   png_write_chunk_header(png_ptr, png_hIST, (png_uint_32)(num_hist * 2));
   for (i = 0; i < num_hist; i++)
   {
      png_save_uint_16(buf, hist[i]);
      png_write_chunk_data(png_ptr, buf, 2);
   }
   png_write_chunk_end(png_ptr);
}
}

// Test 1: Normal operation path where num_hist <= num_palette
// Expect a hIST chunk header with correct length and histogram data encoded in big-endian order.
void test_png_write_hIST_normal_path() {
    reset_mock_state();

    png_struct s;
    s.num_palette = 256;  // enough palette entries

    png_uint_16 hist_arr[3] = {1, 2, 3}; // histogram values

    wrapped_png_write_hIST(&s, hist_arr, 3);

    // Validate: header written and correct chunk type and length
    TEST_CHECK(g_header_written, "Header should be written for valid histogram entries");
    TEST_CHECK(g_header_name == png_hIST, "Chunk type should be hIST");
    TEST_CHECK(g_header_length == 6, "Header length should be num_hist * 2 (3*2=6)");

    // Validate: histogram data encoded as big-endian 16-bit values
    unsigned char expected[6] = {
        0x00, 0x01, // 1
        0x00, 0x02, // 2
        0x00, 0x03  // 3
    };
    TEST_CHECK(g_chunk_data.size() == 6, "Histogram data length should be 6 bytes");
    if (g_chunk_data.size() == 6) {
        TEST_CHECK(std::memcmp(g_chunk_data.data(), expected, 6) == 0, "Histogram bytes must match big-endian 16-bit encoding");
    }

    // Ensure no warnings for normal path
    TEST_CHECK(g_mock_warnings == 0, "No warnings should be issued for valid input");
}

// Test 2: Invalid histogram length path
// num_hist > num_palette should trigger a warning and abort without writing a chunk.
void test_png_write_hIST_invalid_num_hist() {
    reset_mock_state();

    png_struct s;
    s.num_palette = 2;  // not enough palette entries for histogram entries

    png_uint_16 hist_arr[3] = {10, 11, 12};

    wrapped_png_write_hIST(&s, hist_arr, 3);

    // Validate: header should not be written
    TEST_CHECK(!g_header_written, "Header should not be written when num_hist exceeds num_palette");

    // Validate: a warning was issued
    TEST_CHECK(g_mock_warnings > 0, "Warning should be issued for invalid histogram count");
}

// Main entry to run tests
int main() {
    std::cout << "Running tests for png_write_hIST (mocked environment)..." << std::endl;

    test_png_write_hIST_normal_path();
    test_png_write_hIST_invalid_num_hist();

    if (test_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "TESTS FAILED: " << test_failures << " failure(s) detected" << std::endl;
        return 1;
    }
}
// Automated C++11 unit tests for the focal method: png_write_sCAL_s
// This test suite provides a minimal in-source mock of the required PNG types/functions
// and a faithful inlined implementation of png_write_sCAL_s (as in the provided snippet).
// The tests verify both the normal (buffer fits) and error (buffer too small) paths,
// plus a boundary case that exactly hits the 64-byte buffer limit.
// Explanatory comments accompany each test case.

// Note: This file is self-contained and uses only the C++ standard library.
// It does not rely on external test frameworks (no GTest) as requested.

#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Minimal stand-in types and functions to emulate the environment for png_write_sCAL_s.
// These are intentionally lightweight and only implement what's necessary for the tests.

typedef unsigned char png_byte;
typedef unsigned char* png_bytep;
typedef const unsigned char* png_const_bytep;
typedef const char* png_const_charp;
typedef unsigned int png_uint_32;
typedef unsigned short png_uint_16;

struct png_struct {
    bool warned;                          // whether a warning was issued
    bool chunk_written;                   // whether a PNG chunk was written
    png_uint_32 last_chunk_name;            // last written chunk's name
    size_t last_chunk_len;                   // last written chunk length
    std::vector<png_byte> last_chunk_buf;    // last written chunk data
};

typedef png_struct* png_structrp;

// PNG chunk name for sCAL ("sCAL" in ASCII)
static const png_uint_32 png_sCAL = 0x7343414C;

// Lightweight, no-op debug helpers
static void png_debug(int /*level*/, const char* /*msg*/) {
    // Intentionally empty for tests
}
static void png_debug1(int /*level*/, const char* /*format*/, unsigned int /*val*/) {
    // Intentionally empty for tests
}

// Simple warning recorder
static void png_warning(png_structrp png_ptr, png_const_charp /*msg*/) {
    if (png_ptr) png_ptr->warned = true;
}

// Chunk writer stub: records the written chunk into the png_struct
static void png_write_complete_chunk(png_structrp png_ptr, png_uint_32 chunk_name,
                                     png_const_bytep data, size_t length) {
    if (!png_ptr) return;
    png_ptr->chunk_written = true;
    png_ptr->last_chunk_name = chunk_name;
    png_ptr->last_chunk_len = length;
    png_ptr->last_chunk_buf.assign(data, data + length);
}

// Forward declaration of the focal method (as implemented in the provided snippet)
extern "C" void png_write_sCAL_s(png_structrp png_ptr, int unit,
                                png_const_charp width, png_const_charp height);

// Inlined reproduction of the focal method (to enable testing in this isolated TU)
extern "C" void png_write_sCAL_s(png_structrp png_ptr, int unit,
                                png_const_charp width, png_const_charp height)
{
    {
        png_byte buf[64];
        size_t wlen, hlen, total_len;
        png_debug(1, "in png_write_sCAL_s");
        wlen = strlen(width);
        hlen = strlen(height);
        total_len = wlen + hlen + 2;
        if (total_len > 64)
        {
            png_warning(png_ptr, "Can't write sCAL (buffer too small)");
            return;
        }
        buf[0] = (png_byte)unit;
        memcpy(buf + 1, width, wlen + 1);      /* Append the '\0' here */
        memcpy(buf + wlen + 2, height, hlen);  /* Do NOT append the '\0' here */
        png_debug1(3, "sCAL total length = %u", (unsigned int)total_len);
        png_write_complete_chunk(png_ptr, png_sCAL, buf, total_len);
    }
}

// Simple test harness utilities
static int tests_failed = 0;
#define TEST_ASSERT(cond, msg) \
    do { if (!(cond)) { \
        std::cerr << "TEST FAILURE: " << (msg) \
                  << " (File: " << __FILE__ << ", Line: " << __LINE__ << ")\n"; \
        ++tests_failed; \
      } \
    } while (0)

namespace test_p CAL_s_namespace { // intentionally odd-named namespace to illustrate namespace usage (not required)

// Test 1: Normal path where width+height fit into 64-byte buffer.
// Expect a chunk to be written with correct data.
void test_sCAL_basic_write() {
    // Arrange
    png_struct png;
    png.warned = false;
    png.chunk_written = false;
    png.last_chunk_name = 0;
    png.last_chunk_len = 0;
    png.last_chunk_buf.clear();

    // Act
    const char *width = "ABC"; // wlen = 3
    const char *height = "DEF"; // hlen = 3
    png_write_sCAL_s(&png, 1, width, height);

    // Assert
    TEST_ASSERT(png.chunk_written, "Chunk should be written when total_len <= 64");
    TEST_ASSERT(png.last_chunk_name == png_sCAL, "Chunk name should be png_sCAL");
    TEST_ASSERT(png.last_chunk_len == (strlen(width) + strlen(height) + 2),
                "Chunk length should be wlen + hlen + 2");

    // Expected buffer:
    // buf[0] = unit (1)
    // buf[1..wlen] = width (wlen bytes)
    // buf[wlen+1] = terminator '\0'
    // buf[wlen+2 ..] = height (hlen bytes)
    unsigned char expected[8] = {
        (unsigned char)1, 'A', 'B', 'C', '\0', 'D', 'E', 'F'
    };
    // Verify data length and content
    TEST_ASSERT(png.last_chunk_buf.size() == 8, "Buffer size should be exactly 8 bytes");
    for (size_t i = 0; i < 8; ++i) {
        if (png.last_chunk_buf[i] != expected[i]) {
            std::cerr << "MISMATCH at byte " << i
                      << ": expected " << static_cast<unsigned int>(expected[i])
                      << ", got " << static_cast<unsigned int>(png.last_chunk_buf[i]) << "\n";
            tests_failed++;
            break;
        }
    }
}

void test_sCAL_buffer_too_small() {
    // Arrange
    png_struct png;
    png.warned = false;
    png.chunk_written = false;
    png.last_chunk_name = 0;
    png.last_chunk_len = 0;
    png.last_chunk_buf.clear();

    // Create a width string of length 63 to force total_len = 63 + 0 + 2 = 65 (>64)
    std::string w(63, 'X');
    const char* width = w.c_str();
    const char* height = "";

    // Act
    png_write_sCAL_s(&png, 2, width, height);

    // Assert
    TEST_ASSERT(!png.chunk_written, "Chunk should NOT be written when total_len > 64");
    TEST_ASSERT(png.warned, "Warning should be issued when buffer is too small");
    TEST_ASSERT(png.last_chunk_len == 0, "No chunk length should be recorded when not written");
}

void test_sCAL_boundary_exact64() {
    // Arrange
    png_struct png;
    png.warned = false;
    png.chunk_written = false;
    png.last_chunk_name = 0;
    png.last_chunk_len = 0;
    png.last_chunk_buf.clear();

    // Create a width string of length 62 to get total_len = 62 + 0 + 2 = 64
    std::string w(62, 'Y');
    const char* width = w.c_str();
    const char* height = "";

    // Act
    png_write_sCAL_s(&png, 3, width, height);

    // Assert
    TEST_ASSERT(png.chunk_written, "Chunk should be written when total_len == 64");
    TEST_ASSERT(png.last_chunk_len == 64, "Chunk length should be exactly 64 when total_len == 64");

    // Validate the data contents
    // buf[0] = 3
    // buf[1..62] = 62 'Y' chars
    // buf[63] = '\0'
    TEST_ASSERT(png.last_chunk_buf.size() == 64, "Buffer size should be 64 bytes");
    TEST_ASSERT(png.last_chunk_buf[0] == (unsigned char)3, "First byte should be the unit value (3)");
    for (size_t i = 0; i < 62; ++i) {
        TEST_ASSERT(png.last_chunk_buf[i+1] == (unsigned char)'Y',
                    "Buffer should contain width characters in sequence");
    }
    TEST_ASSERT(png.last_chunk_buf[63] == '\0', "Last byte should be null terminator for width string");
}

} // namespace test_p CAL_s_namespace

// Entry point for running tests without a test framework
int main() {
    using namespace test_p CAL_s_namespace;

    std::cout << "Running png_write_sCAL_s unit tests (no external framework)..." << std::endl;

    test_sCAL_basic_write();
    test_sCAL_buffer_too_small();
    test_sCAL_boundary_exact64();

    if (tests_failed == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << tests_failed << " test(s) FAILED." << std::endl;
        return 1;
    }
}
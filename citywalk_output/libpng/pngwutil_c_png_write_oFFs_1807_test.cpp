#include <cstdint>
#include <vector>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>


// Minimal mock/types to simulate the libpng environment for testing png_write_oFFs

typedef unsigned char png_byte;
typedef unsigned char* png_bytep;
typedef const unsigned char* png_const_bytep;
typedef int32_t png_int_32;
typedef uint32_t png_uint_32;
typedef unsigned int png_uint_16;

// Opaque pointer type to simulate png_struct* (png_structrp in libpng)
typedef struct png_struct* png_structrp;

// Forward declarations of dependencies (mocks)
extern "C" void png_debug(int level, const char* msg);
extern "C" void png_warning(png_structrp png_ptr, const char* msg);
extern "C" void png_save_int_32(png_bytep buf, png_int_32 i);
extern "C" void png_write_complete_chunk(png_structrp png_ptr, png_uint_32 chunk_name,
                                         png_const_bytep data, size_t length);

// Focal constants (mimicking libpng values)
const int PNG_OFFSET_LAST = 8;
const png_uint_32 png_oFFs = 0x6f464673; // ASCII "oFFs" in big-endian form

// Global test state to verify behavior
static bool g_warning_seen = false;
static std::vector<unsigned char> g_last_chunk_buf;
static png_uint_32 g_last_chunk_name = 0;
static size_t g_last_chunk_length = 0;

// A simple PNG_struct mock (contents are irrelevant for the test)
struct png_struct { };

// Test PNG struct pointer alias
static struct png_struct test_png_struct;
static png_structrp test_png = &test_png_struct;

// Utils to reset test state
static void reset_test_state() {
    g_warning_seen = false;
    g_last_chunk_buf.clear();
    g_last_chunk_name = 0;
    g_last_chunk_length = 0;
}

// Mock implementations of dependencies (C linkage)
extern "C" void png_debug(int /*level*/, const char* /*msg*/) {
    // no-op for tests
}

extern "C" void png_warning(png_structrp /*png_ptr*/, const char* /*msg*/) {
    g_warning_seen = true;
}

extern "C" void png_save_int_32(png_bytep buf, png_int_32 i) {
    // Write 32-bit integer in big-endian order
    buf[0] = static_cast<png_byte>((i >> 24) & 0xFF);
    buf[1] = static_cast<png_byte>((i >> 16) & 0xFF);
    buf[2] = static_cast<png_byte>((i >> 8) & 0xFF);
    buf[3] = static_cast<png_byte>(i & 0xFF);
}

extern "C" void png_write_complete_chunk(png_structrp /*png_ptr*/, png_uint_32 chunk_name,
                                         png_const_bytep data, size_t length) {
    g_last_chunk_name = chunk_name;
    g_last_chunk_length = length;
    g_last_chunk_buf.assign(data, data + length);
}

// The focal method under test (reproduced inline for standalone testing)
extern "C" void png_write_oFFs(png_structrp png_ptr, png_int_32 x_offset, png_int_32 y_offset,
    int unit_type)
{
{
   png_byte buf[9];
   png_debug(1, "in png_write_oFFs");
   if (unit_type >= PNG_OFFSET_LAST)
      png_warning(png_ptr, "Unrecognized unit type for oFFs chunk");
   png_save_int_32(buf, x_offset);
   png_save_int_32(buf + 4, y_offset);
   buf[8] = (png_byte)unit_type;
   png_write_complete_chunk(png_ptr, png_oFFs, buf, 9);
}
}

// Lightweight test harness (non-terminating assertions)
static int g_failures = 0;
static void report_failure(const char* msg, const char* file, int line) {
    std::fprintf(stderr, "Test Failure: %s (%s:%d)\n", msg, file, line);
    ++g_failures;
}
#define TEST_ASSERT(cond, msg) do { if(!(cond)) report_failure(msg, __FILE__, __LINE__); } while(0)

// Unit Tests

// Test 1: Basic encoding path with valid unit_type and coordinates
// Expected: no warning, 9-byte oFFs data with proper big-endian encoding.
void test_basic_write_oFFs() {
    reset_test_state();

    // Given
    png_int_32 x = 1;
    png_int_32 y = 2;
    int unit_type = 0; // below PNG_OFFSET_LAST

    // When
    png_write_oFFs(test_png, x, y, unit_type);

    // Then
    TEST_ASSERT(!g_warning_seen, "No warning should be issued for valid unit_type");
    TEST_ASSERT(g_last_chunk_name == png_oFFs, "Chunk name should be oFFs");
    TEST_ASSERT(g_last_chunk_length == 9, "Chunk length should be 9");
    const unsigned char* buf = g_last_chunk_buf.data();
    TEST_ASSERT(buf != nullptr, "Buffer should not be null");

    // x_offset = 1 -> 0x00 00 00 01
    TEST_ASSERT(buf[0] == 0x00 && buf[1] == 0x00 && buf[2] == 0x00 && buf[3] == 0x01,
                "x_offset encoding incorrect");
    // y_offset = 2 -> 0x00 00 00 02
    TEST_ASSERT(buf[4] == 0x00 && buf[5] == 0x00 && buf[6] == 0x00 && buf[7] == 0x02,
                "y_offset encoding incorrect");
    // unit_type = 0
    TEST_ASSERT(buf[8] == 0x00, "unit_type encoding incorrect");
}

// Test 2: Unrecognized unit_type triggers a warning but still writes chunk
void test_warning_on_unrecognized_unit_type() {
    reset_test_state();

    // Given
    png_int_32 x = 10;
    png_int_32 y = -20;
    int unit_type = PNG_OFFSET_LAST; // equals threshold

    // When
    png_write_oFFs(test_png, x, y, unit_type);

    // Then
    TEST_ASSERT(g_warning_seen, "Warning should be issued for unit_type >= PNG_OFFSET_LAST");
    TEST_ASSERT(g_last_chunk_length == 9, "Chunk length should still be 9");
    // Basic sanity check on data encoding
    const unsigned char* buf = g_last_chunk_buf.data();
    // x_offset = 10
    TEST_ASSERT(buf[0] == 0x00 && buf[1] == 0x00 && buf[2] == 0x00 && buf[3] == 0x0A,
                "x_offset encoding incorrect for positive value");
    // y_offset = -20 -> 0xFF FF FF EC
    TEST_ASSERT(buf[4] == 0xFF && buf[5] == 0xFF && buf[6] == 0xFF && buf[7] == 0xEC,
                "y_offset encoding incorrect for negative value");
    TEST_ASSERT(buf[8] == 0x00, "unit_type encoding should be 0");
}

// Test 3: Negative coordinates are encoded as two's complement (edge case)
void test_negative_coordinates_encoding() {
    reset_test_state();

    // Given
    png_int_32 x = -1;
    png_int_32 y = -2;
    int unit_type = 0;

    // When
    png_write_oFFs(test_png, x, y, unit_type);

    // Then
    TEST_ASSERT(g_last_chunk_length == 9, "Chunk length should be 9 for negative values");
    const unsigned char* buf = g_last_chunk_buf.data();
    // x_offset = -1 -> 0xFF FF FF FF
    TEST_ASSERT(buf[0] == 0xFF && buf[1] == 0xFF && buf[2] == 0xFF && buf[3] == 0xFF,
                "x_offset encoding incorrect for -1");
    // y_offset = -2 -> 0xFF FF FF FE
    TEST_ASSERT(buf[4] == 0xFF && buf[5] == 0xFF && buf[6] == 0xFF && buf[7] == 0xFE,
                "y_offset encoding incorrect for -2");
    TEST_ASSERT(buf[8] == 0x00, "unit_type encoding should be 0");
}

// Test runner
int main() {
    // Run tests
    test_basic_write_oFFs();
    test_warning_on_unrecognized_unit_type();
    test_negative_coordinates_encoding();

    // Summary
    if (g_failures == 0) {
        std::fprintf(stdout, "All tests passed.\n");
        return 0;
    } else {
        std::fprintf(stderr, "%d test(s) failed.\n", g_failures);
        return 1;
    }
}
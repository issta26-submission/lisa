// Self-contained C++11 test suite for the focal function write_vpAg_chunk
// This test recreates the minimal environment needed to compile and run the
// focal method from pngtest.c without external dependencies.
// It uses non-terminating assertions style (collects failures and reports at the end).

#include <cstdint>
#include <string.h>
#include <time.h>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <png.h>


// Minimal type aliases to simulate the C environment used by the focal method.
typedef void* png_structp;
typedef unsigned char png_byte;
typedef unsigned char* png_bytep;
typedef uint32_t png_uint_32;

// Minimal user_chunk_info structure with the fields used by write_vpAg_chunk.
typedef struct {
    unsigned long vpAg_width;
    unsigned long vpAg_height;
    unsigned int vpAg_units;
} user_chunk_info;

// Global verbose flag used by the focal method for debug printing.
int verbose = 0;

// Capture buffers to verify the output of write_vpAg_chunk.
static unsigned char captured_vpAg_name[5];
static unsigned char captured_vpag_data[9];
static size_t captured_len = 0;

// Utility to reset capture buffers between tests.
static void reset_capture() {
    memset(captured_vpAg_name, 0, sizeof(captured_vpAg_name));
    memset(captured_vpag_data, 0, sizeof(captured_vpag_data));
    captured_len = 0;
}

// Mocked implementation of the external helper used by the focal method.
// It writes a 5-byte chunk name and a 9-byte payload to internal capture buffers.
void png_write_chunk(png_structp /*write_ptr*/, png_byte *name, png_bytep data, png_uint_32 length) {
    // The chunk name is always 5 bytes for vpAg in the provided code.
    memcpy(captured_vpAg_name, name, 5);

    // Track length (the focal method passes 9).
    captured_len = static_cast<size_t>(length);

    // Copy the payload bytes (up to the available buffer).
    size_t to_copy = (length < 9) ? static_cast<size_t>(length) : 9;
    memcpy(captured_vpag_data, data, to_copy);
}

// Mocked 32-bit save routine used by the focal method to encode width/height into big-endian form.
void png_save_uint_32(unsigned char *p, png_uint_32 v) {
    p[0] = (unsigned char)((v >> 24) & 0xFF);
    p[1] = (unsigned char)((v >> 16) & 0xFF);
    p[2] = (unsigned char)((v >> 8) & 0xFF);
    p[3] = (unsigned char)(v & 0xFF);
}

// The focal method under test copied here from the provided snippet.
// It writes a vpAg chunk with width, height and units into a chunk payload.
void write_vpAg_chunk(png_structp write_ptr, user_chunk_info *data) {
{
   png_byte vpAg[5] = {118, 112,  65, 103, '\0'};
   png_byte vpag_chunk_data[9];
   if (verbose != 0)
      fprintf(stdout, " vpAg = %lu x %lu, units = %d\n",
          (unsigned long)data->vpAg_width,
          (unsigned long)data->vpAg_height,
          data->vpAg_units);
   png_save_uint_32(vpag_chunk_data, data->vpAg_width);
   png_save_uint_32(vpag_chunk_data + 4, data->vpAg_height);
   vpag_chunk_data[8] = data->vpAg_units;
   png_write_chunk(write_ptr, vpAg, vpag_chunk_data, 9);
}
}

// Simple non-terminating test assertion helper.
// Prints a message but does not abort the test execution.
static int g_fail_count = 0;
static void test_assert(bool condition, const char *msg) {
    if (!condition) {
        std::fprintf(stderr, "TEST FAILURE: %s\n", msg);
        g_fail_count++;
    }
}

// Compute big-endian bytes for a 32-bit value for assertion convenience.
static void be_write_uint32(unsigned char *out, uint32_t v) {
    png_save_uint_32(out, v);
}

// Test 1: Verify normal behavior writes correct chunk name, length and payload.
static void test_case_normal_write() {
    reset_capture();

    user_chunk_info data;
    data.vpAg_width  = 1234;  // 0x000004D2
    data.vpAg_height = 5678;  // 0x0000168E
    data.vpAg_units  = 1;

    verbose = 0;
    write_vpAg_chunk(nullptr, &data);

    // Validate chunk name: should be "vpAg" followed by a null terminator (5 bytes)
    unsigned char expected_name[5] = {118, 112, 65, 103, 0};
    test_assert(std::memcmp(captured_vpAg_name, expected_name, 5) == 0,
                "Chunk name must be 'vpAg' (with terminal 0)");

    // Validate payload length
    test_assert(captured_len == 9, "Payload length must be 9 bytes");

    // Validate width (big-endian)
    unsigned char w_be[4];
    be_write_uint32(w_be, data.vpAg_width);
    test_assert(std::memcmp(captured_vpag_data, w_be, 4) == 0,
                "Payload width bytes mismatch (big-endian)");

    // Validate height (big-endian)
    unsigned char h_be[4];
    be_write_uint32(h_be, data.vpAg_height);
    test_assert(std::memcmp(captured_vpag_data + 4, h_be, 4) == 0,
                "Payload height bytes mismatch (big-endian)");

    // Validate units byte
    test_assert(captured_vpag_data[8] == (unsigned char)data.vpAg_units,
                "Payload units byte mismatch");
}

// Test 2: Verify behavior with zero values (edge case).
static void test_case_zero_values() {
    reset_capture();

    user_chunk_info data;
    data.vpAg_width  = 0;
    data.vpAg_height = 0;
    data.vpAg_units  = 0;

    verbose = 0;
    write_vpAg_chunk(nullptr, &data);

    // Name check (same as above)
    unsigned char expected_name[5] = {118, 112, 65, 103, 0};
    test_assert(std::memcmp(captured_vpAg_name, expected_name, 5) == 0,
                "Chunk name must be 'vpAg' for zero-values case");

    // Payload length
    test_assert(captured_len == 9, "Payload length must be 9 bytes for zero-values case");

    // Width 0
    unsigned char w_be[4] = {0,0,0,0};
    test_assert(std::memcmp(captured_vpag_data, w_be, 4) == 0,
                "Zero width should encode as four zero bytes");

    // Height 0
    unsigned char h_be[4] = {0,0,0,0};
    test_assert(std::memcmp(captured_vpag_data + 4, h_be, 4) == 0,
                "Zero height should encode as four zero bytes");

    // Units 0
    test_assert(captured_vpag_data[8] == 0,
                "Zero units byte should be zero");
}

// Test 3: Exercise both branches of the verbose predicate (true and false).
// We cannot reliably capture stdout in this minimal harness without additional
// plumbing, but we ensure both branches are executed by calling with verbose = 0
// and verbose = 1. This still exercises the conditional path in the code.
static void test_case_verbose_branch_execution() {
    reset_capture();

    user_chunk_info data;
    data.vpAg_width  = 42;
    data.vpAg_height = 24;
    data.vpAg_units  = 2;

    // False branch: verbose = 0
    verbose = 0;
    write_vpAg_chunk(nullptr, &data);
    test_assert(captured_len == 9, "Length should be 9 even when verbose is 0");

    // Clear capture and exercise true branch
    reset_capture();
    verbose = 1;
    write_vpAg_chunk(nullptr, &data);
    test_assert(captured_len == 9, "Length should be 9 when verbose is 1");

    // Note: stdout content is not asserted here due to harness simplicity.
}

// Helper to describe test suite
static void run_all_tests() {
    test_case_normal_write();
    test_case_zero_values();
    test_case_verbose_branch_execution();
}

// Entry point
int main() {
    g_fail_count = 0;
    run_all_tests();

    if (g_fail_count > 0) {
        std::fprintf(stderr, "Total test failures: %d\n", g_fail_count);
        return EXIT_FAILURE;
    } else {
        std::printf("All tests passed.\n");
        return EXIT_SUCCESS;
    }
}
/*
  Lightweight C++11 unit tests for the focal function write_sTER_chunk
  (adapted in a self-contained manner to avoid dependencies on libpng).
  The tests mock the PNG writing routine and verify that the focal method
  constructs the sTER chunk name correctly and passes the address of
  data->sTER_mode as the payload with the correct length.

  Notes:
  - This test suite does not rely on GTest. It uses a minimal main-based test runner.
  - We provide a fake png.h / types and a mock png_write_chunk to isolate the
    logic of write_sTER_chunk from the real libpng.
  - We test both verbose=0 and verbose=1 paths to cover true/false branches.
*/

#include <string.h>
#include <time.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <png.h>


// ---------------------------------------------------------------------------
// Fake PNG API surface (to allow unit testing in isolation)
// ---------------------------------------------------------------------------

// Forward-declare types used by the focal function
typedef void* png_structp;
typedef unsigned char png_byte;
typedef png_byte* png_bytep;
typedef unsigned char* png_bytep;
typedef unsigned int png_uint_32;

// Minimal user_chunk_info structure used by the focal function
typedef struct user_chunk_info {
    int sTER_mode;
} user_chunk_info;

// Emulate STDERR macro (as in the original file)
#define STDERR stdout

// Global verbosity flag used by the focal method
static int verbose = 0;

// ---------------------------------------------------------------------------
// Mock PNG writer to capture how write_sTER_chunk would call the API
// ---------------------------------------------------------------------------

static bool g_png_write_chunk_called = false;
static std::string g_last_name;
static unsigned int g_last_len = 0;
static int g_last_sTER_mode = 0;

// Mock implementation of png_write_chunk. It records inputs for assertions.
extern "C" void png_write_chunk(png_structp /*write_ptr*/, png_bytep name, png_bytep data, png_uint_32 length)
{
    g_png_write_chunk_called = true;
    // Name is expected to be "sTER" with a terminating null
    g_last_name = std::string(reinterpret_cast<char*>(name));
    g_last_len = length;
    // Data points to the sTER_mode field of the provided struct
    g_last_sTER_mode = *reinterpret_cast<int*>(data);
}

// ---------------------------------------------------------------------------
// Focal method under test (reproduced in this test to enable isolated unit tests)
// ---------------------------------------------------------------------------

void write_sTER_chunk(png_structp write_ptr, user_chunk_info *data)
{
{
   png_byte sTER[5] = {115,  84,  69,  82, '\0'}; // "sTER" + null terminator
   if (verbose != 0)
      fprintf(STDERR, "\n stereo mode = %d\n", data->sTER_mode);
   png_write_chunk(write_ptr, sTER, &data->sTER_mode, 1);
}
}

// ---------------------------------------------------------------------------
// Simple test harness
// ---------------------------------------------------------------------------

static int g_pass_count = 0;
static int g_fail_count = 0;

#define CHECK(cond, msg)                                  \
    do {                                                  \
        if (cond) {                                       \
            ++g_pass_count;                               \
        } else {                                          \
            ++g_fail_count;                               \
            std::cerr << "TEST FAIL: " << msg << "\n";   \
        }                                                 \
    } while (0)


// Test 1: verbose = 0 should still call png_write_chunk with correct parameters
void test_write_sTER_chunk_verbose_off() {
    // Reset mock state
    g_png_write_chunk_called = false;
    g_last_name.clear();
    g_last_len = 0;
    g_last_sTER_mode = 0;
    verbose = 0;

    user_chunk_info data;
    data.sTER_mode = 12345;

    write_sTER_chunk(nullptr, &data);

    bool passed = g_png_write_chunk_called
                  && g_last_name == "sTER"
                  && g_last_len == 1
                  && g_last_sTER_mode == 12345;

    CHECK(passed, "test_write_sTER_chunk_verbose_off: png_write_chunk should be called with name 'sTER', length 1, and correct data pointer value when verbose=0");
}

// Test 2: verbose = 1 should also call png_write_chunk and exercise the true-branch print path
void test_write_sTER_chunk_verbose_on() {
    // Reset mock state
    g_png_write_chunk_called = false;
    g_last_name.clear();
    g_last_len = 0;
    g_last_sTER_mode = 0;
    verbose = 1;

    user_chunk_info data;
    data.sTER_mode = -7; // arbitrary value to ensure non-trivial payload
    // No assertion on printed output; focus on correct PNG write invocation
    write_sTER_chunk(nullptr, &data);

    bool passed = g_png_write_chunk_called
                  && g_last_name == "sTER"
                  && g_last_len == 1
                  && g_last_sTER_mode == -7;

    CHECK(passed, "test_write_sTER_chunk_verbose_on: png_write_chunk should be called with name 'sTER', length 1, and correct data pointer value when verbose!=0");
}

// Test runner
void run_all_tests() {
    test_write_sTER_chunk_verbose_off();
    test_write_sTER_chunk_verbose_on();

    std::cout << "Tests run: " << (g_pass_count + g_fail_count)
              << ", Passed: " << g_pass_count
              << ", Failed: " << g_fail_count << std::endl;
}

// ---------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------

int main() {
    run_all_tests();
    return (g_fail_count == 0) ? 0 : 1;
}
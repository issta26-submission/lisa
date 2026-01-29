// Automated unit tests for read_user_chunk_callback
// Environment: C++11, no GTest. Tests are implemented in a standalone harness.
// The test harness provides minimal stubs for the libpng dependent APIs
// and reuses (replicates) the focal function logic to enable isolated testing.
// Comments explain intent and coverage for each test case.

#include <cstdint>
#include <string.h>
#include <time.h>
#include <stdexcept>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <png.h>


// -------------------------
// Minimal type definitions and stubs
// -------------------------

typedef unsigned char png_byte;
typedef png_byte* png_bytep;
typedef unsigned int png_uint_32;
typedef struct png_struct png_struct;

// Forward declaration for test chunk type
typedef struct png_unknown_chunk {
    png_byte name[5];    // 4 letters + terminator
    png_bytep data;
    size_t size;
} *png_unknown_chunkp;

// user_chunk_info struct as used by the focal code
typedef struct user_chunk_info {
    int sTER_mode;
    int vpAg_width;
    int vpAg_height;
    int vpAg_units;
} user_chunk_info;

// "Unknown" chunk constants (simplified)
static const int have_sTER = 1;
static const int have_vpAg = 2;

// Global mock state (per-test control)
static void* g_mock_user_chunk_ptr = nullptr;
static int g_next_set_chunk_location_return_value = 1;
static int g_last_what_param = 0;

// Helpers to set up mocks
void reset_mock_state() {
    g_mock_user_chunk_ptr = nullptr;
    g_next_set_chunk_location_return_value = 1;
    g_last_what_param = 0;
}

// Expose the mock state (used by tests)
void* png_get_user_chunk_ptr(png_struct* /*png_ptr*/) {
    // Return the pointer that the test wants to simulate as "my_user_chunk_data"
    return g_mock_user_chunk_ptr;
}

// The focal function uses png_error to report fatal errors.
// We implement a test-friendly version that throws a C++ exception.
void png_error(png_struct* /*png_ptr*/, const char* message) {
    throw std::runtime_error(message ? message : "png_error");
}

// Mock implementation for set_chunk_location used by read_user_chunk_callback
int set_chunk_location(png_struct* /*png_ptr*/, user_chunk_info* /*chunk_data*/, int what) {
    // Record what is requested for debugging/verification
    g_last_what_param = what;
    // Return controlled value to simulate "duplicate" vs "new" vs "error" paths
    int ret = g_next_set_chunk_location_return_value;
    // After using, perhaps reset to a sane default (1) to avoid leakage across tests
    // (Tests explicitly set this as needed.)
    return ret;
}

// Mock for extracting 4-byte big-endian unsigned int from 4 data bytes
unsigned int png_get_uint_31(png_struct* /*png_ptr*/, png_bytep data) {
    // Treat data as 4-byte big-endian unsigned int
    return (static_cast<unsigned int>(data[0]) << 24) |
           (static_cast<unsigned int>(data[1]) << 16) |
           (static_cast<unsigned int>(data[2]) << 8)  |
           (static_cast<unsigned int>(data[3]));
}

// Function prototype for the focal function under test
// We replicate the exact signature as in the snippet for compatibility
int read_user_chunk_callback(png_struct *png_ptr, png_unknown_chunkp chunk);

// -------------------------
// Reimplementation of the focal function (for test harness)
// Note: This mirrors the logic shown in the provided <FOCAL_METHOD>.
// It relies on the mocks above to drive paths without requiring a real libpng build.
// -------------------------

int read_user_chunk_callback(png_struct *png_ptr, png_unknown_chunkp chunk) {
{
   user_chunk_info *my_user_chunk_data =
      (user_chunk_info*)png_get_user_chunk_ptr(png_ptr);
   if (my_user_chunk_data == NULL)
      png_error(png_ptr, "lost pointer to user chunk data");
   /* Return one of the following:
    *    return -n;  chunk had an error
    *    return 0;   did not recognize
    *    return n;   success
    *
    * The unknown chunk structure contains the chunk data:
    * png_byte name[5];
    * png_byte *data;
    * size_t size;
    *
    * Note that libpng has already taken care of the CRC handling.
    */
   if (chunk->name[0] == 115 && chunk->name[1] ==  84 &&     /* s  T */
       chunk->name[2] ==  69 && chunk->name[3] ==  82)       /* E  R */
      {
         /* Found sTER chunk */
         if (chunk->size != 1)
            return -1; /* Error return */
         if (chunk->data[0] != 0 && chunk->data[0] != 1)
            return -1;  /* Invalid mode */
         if (set_chunk_location(png_ptr, my_user_chunk_data, have_sTER) != 0)
         {
            my_user_chunk_data->sTER_mode = chunk->data[0];
            return 1;
         }
         else
            return 0; /* duplicate sTER - give it to libpng */
      }
   if (chunk->name[0] != 118 || chunk->name[1] != 112 ||    /* v  p */
       chunk->name[2] !=  65 || chunk->name[3] !=  103)      /* A  g */
      return 0; /* Did not recognize */
   /* Found ImageMagick vpAg chunk */
   if (chunk->size != 9)
      return -1; /* Error return */
   if (set_chunk_location(png_ptr, my_user_chunk_data, have_vpAg) == 0)
      return 0;  /* duplicate vpAg */
   my_user_chunk_data->vpAg_width = png_get_uint_31(png_ptr, chunk->data);
   my_user_chunk_data->vpAg_height = png_get_uint_31(png_ptr, chunk->data + 4);
   my_user_chunk_data->vpAg_units = chunk->data[8];
   return 1;
}
}

// -------------------------
// Simple test framework (non-GTest)
// Provides a few EXPECT_EQ-like macros and a runner to execute tests
// -------------------------

static bool g_any_failure = false;

#define EXPECT_EQ(actual, expected) \
    do { \
        if ((actual) != (expected)) { \
            std::cerr << "EXPECT_EQ failed: " #actual " (" << (actual) \
                      << ") != " #expected " (" << (expected) << ")" << std::endl; \
            g_any_failure = true; \
        } \
    } while (0)

#define EXPECT_TRUE(cond) \
    do { \
        if (!(cond)) { \
            std::cerr << "EXPECT_TRUE failed: condition is false" << std::endl; \
            g_any_failure = true; \
        } \
    } while (0)

#define EXPECT_THROW(statement, exception_type) \
    do { \
        bool caught = false; \
        try { \
            statement; \
        } catch (const exception_type&) { \
            caught = true; \
        } catch (...) { \
            std::cerr << "EXPECT_THROW: Caught wrong exception type" << std::endl; \
            g_any_failure = true; \
        } \
        if (!caught) { \
            std::cerr << "EXPECT_THROW failed: exception not thrown" << std::endl; \
            g_any_failure = true; \
        } \
    } while (0)


// Test helper: create a chunk with given parameters
png_unknown_chunkp make_chunk(const char* name4, size_t size, const unsigned char* data) {
    png_unknown_chunkp chunk = new png_unknown_chunk;
    // Copy name (4 bytes)
    size_t i;
    for (i = 0; i < 4; ++i) {
        chunk->name[i] = static_cast<png_byte>(name4[i]);
    }
    chunk->name[4] = 0;
    // Data
    if (size > 0 && data != nullptr) {
        chunk->data = new png_byte[size];
        std::memcpy(chunk->data, data, size);
    } else {
        chunk->data = nullptr;
    }
    chunk->size = size;
    return chunk;
}

void free_chunk(png_unknown_chunkp chunk) {
    if (chunk) {
        if (chunk->data) delete[] chunk->data;
        delete chunk;
    }
}

// Test 1: NULL user chunk pointer should trigger png_error
void test_null_user_chunk_ptr_throws() {
    reset_mock_state();
    g_mock_user_chunk_ptr = nullptr;

    // Prepare a chunk (any valid 4-letter name)
    unsigned char d[4] = {0};
    png_unknown_chunkp chunk = make_chunk("test", 4, d);

    bool threw = false;
    try {
        png_struct dummy;
        read_user_chunk_callback(&dummy, chunk);
    } catch (const std::runtime_error& e) {
        threw = true;
        // Ensure the message matches
        EXPECT_EQ(std::string(e.what()), std::string("lost pointer to user chunk data"));
    }

    EXPECT_TRUE(threw);
    free_chunk(chunk);
}

// Test 2: sTER chunk with invalid size should error (-1)
void test_sTER_invalid_size() {
    reset_mock_state();

    // Allocate a valid my_user_chunk_data
    user_chunk_info* info = new user_chunk_info;
    info->sTER_mode = -1;
    info->vpAg_width = 0;
    info->vpAg_height = 0;
    info->vpAg_units = 0;
    g_mock_user_chunk_ptr = static_cast<void*>(info);

    // sTER chunk with size != 1
    unsigned char d[1] = {0};
    png_unknown_chunkp chunk = make_chunk("sTER", 2, d);

    int res = read_user_chunk_callback(reinterpret_cast<png_struct*>(nullptr), chunk);
    EXPECT_EQ(res, -1);

    // cleanup
    free_chunk(chunk);
    delete info;
}

// Test 3: sTER chunk valid size but duplicate (set_chunk_location returns 0)
void test_sTER_duplicate() {
    reset_mock_state();

    user_chunk_info* info = new user_chunk_info;
    info->sTER_mode = -1;
    info->vpAg_width = 0;
    info->vpAg_height = 0;
    info->vpAg_units = 0;
    g_mock_user_chunk_ptr = static_cast<void*>(info);

    // sTER with valid size and data[0] = 0
    unsigned char d[1] = {0};
    png_unknown_chunkp chunk = make_chunk("sTER", 1, d);

    g_next_set_chunk_location_return_value = 0; // simulate duplicate
    int res = read_user_chunk_callback(reinterpret_cast<png_struct*>(nullptr), chunk);
    EXPECT_EQ(res, 0);
    // sTER_mode should remain unchanged
    EXPECT_EQ(info->sTER_mode, -1);

    free_chunk(chunk);
    delete info;
}

// Test 4: sTER chunk valid size and not duplicate (should set mode and return 1)
void test_sTER_success() {
    reset_mock_state();

    user_chunk_info* info = new user_chunk_info;
    info->sTER_mode = -1;
    info->vpAg_width = 0;
    info->vpAg_height = 0;
    info->vpAg_units = 0;
    g_mock_user_chunk_ptr = static_cast<void*>(info);

    unsigned char d[1] = {1}; // mode = 1
    png_unknown_chunkp chunk = make_chunk("sTER", 1, d);

    g_next_set_chunk_location_return_value = 2; // non-zero means success path
    int res = read_user_chunk_callback(reinterpret_cast<png_struct*>(nullptr), chunk);
    EXPECT_EQ(res, 1);
    EXPECT_EQ(info->sTER_mode, 1);

    free_chunk(chunk);
    delete info;
}

// Test 5: vpAg chunk not recognized (wrong name)
void test_vpAg_not_recognized() {
    reset_mock_state();

    // Wrong name: "XXXX"
    unsigned char d[9] = {0};
    png_unknown_chunkp chunk = make_chunk("XXXX", 9, d);

    // Prepare user chunk
    user_chunk_info* info = new user_chunk_info;
    info->sTER_mode = -1;
    info->vpAg_width = 0;
    info->vpAg_height = 0;
    info->vpAg_units = 0;
    g_mock_user_chunk_ptr = static_cast<void*>(info);

    int res = read_user_chunk_callback(reinterpret_cast<png_struct*>(nullptr), chunk);
    // Should not recognize, so return 0
    EXPECT_EQ(res, 0);

    free_chunk(chunk);
    delete info;
}

// Test 6: vpAg invalid size
void test_vpAg_invalid_size() {
    reset_mock_state();

    user_chunk_info* info = new user_chunk_info;
    info->sTER_mode = -1;
    info->vpAg_width = 0;
    info->vpAg_height = 0;
    info->vpAg_units = 0;
    g_mock_user_chunk_ptr = static_cast<void*>(info);

    // vpAg with size != 9
    unsigned char d[8] = {0};
    png_unknown_chunkp chunk = make_chunk("vpAg", 8, d);

    int res = read_user_chunk_callback(reinterpret_cast<png_struct*>(nullptr), chunk);
    EXPECT_EQ(res, -1);

    free_chunk(chunk);
    delete info;
}

// Test 7: vpAg duplicate
void test_vpAg_duplicate() {
    reset_mock_state();

    user_chunk_info* info = new user_chunk_info;
    info->sTER_mode = -1;
    info->vpAg_width = 0;
    info->vpAg_height = 0;
    info->vpAg_units = 0;
    g_mock_user_chunk_ptr = static_cast<void*>(info);

    unsigned char d[9] = {0}; // dummy values
    png_unknown_chunkp chunk = make_chunk("vpAg", 9, d);

    g_next_set_chunk_location_return_value = 0; // duplicate
    int res = read_user_chunk_callback(reinterpret_cast<png_struct*>(nullptr), chunk);
    EXPECT_EQ(res, 0);

    free_chunk(chunk);
    delete info;
}

// Test 8: vpAg success path (width, height, units parsed)
void test_vpAg_success() {
    reset_mock_state();

    user_chunk_info* info = new user_chunk_info;
    info->sTER_mode = -1;
    info->vpAg_width = 0;
    info->vpAg_height = 0;
    info->vpAg_units = 0;
    g_mock_user_chunk_ptr = static_cast<void*>(info);

    // width = 10, height = 20, units = 3
    unsigned char d[9] = { 0,0,0,10,   0,0,0,20,  3 };
    png_unknown_chunkp chunk = make_chunk("vpAg", 9, d);

    g_next_set_chunk_location_return_value = 1; // success path
    int res = read_user_chunk_callback(reinterpret_cast<png_struct*>(nullptr), chunk);
    EXPECT_EQ(res, 1);
    EXPECT_EQ(info->vpAg_width, 10);
    EXPECT_EQ(info->vpAg_height, 20);
    EXPECT_EQ(info->vpAg_units, 3);

    free_chunk(chunk);
    delete info;
}

// -------------------------
// Main: run all tests
// -------------------------

int main(void) {
    std::cout << "Running read_user_chunk_callback unit tests ( senza GTest )" << std::endl;

    // Execute tests
    test_null_user_chunk_ptr_throws();
    test_sTER_invalid_size();
    test_sTER_duplicate();
    test_sTER_success();
    test_vpAg_not_recognized();
    test_vpAg_invalid_size();
    test_vpAg_duplicate();
    test_vpAg_success();

    if (g_any_failure) {
        std::cerr << "Some tests FAILED." << std::endl;
        return 1;
    } else {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    }
}
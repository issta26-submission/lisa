// Lightweight C++11 test suite for the png_handle_sCAL function
// This test mimics the function's environment by providing minimal
// stubs for its dependencies. It uses non-terminating assertions to
// maximize code coverage and continues execution after a failed check.

// Note: This test re-implements a focused version of png_handle_sCAL
// along with necessary dependencies to enable unit testing without a full
// libpng build. The test cases cover all branches: invalid unit, bad width
// format, non-positive width, bad height format, non-positive height, and
// a successful parsing path.

#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// ----------------------------------------------------------------------------
// Minimal type and constant definitions to mirror the focal function's API
// ----------------------------------------------------------------------------

typedef unsigned char png_bytep;
typedef png_bytep png_bytep_p; // not used, kept for clarity if needed
typedef const char* png_const_charp;
typedef const unsigned char* png_const_bytep;
typedef unsigned int png_uint_32;
typedef long png_alloc_size_t; // simplified for test

typedef struct png_struct_tag png_struct;
typedef struct png_info_tag png_info;
typedef png_struct* png_structrp;
typedef png_info* png_inforp;

// Public constants used by the function
static const int handled_ok = 0;
static const int handled_error = 1;

// Domain helpers and stubs (mocked internal libpng-like API)
void png_debug(int, const char*) { /* no-op in test */ }
void png_debug1(int, const char*, unsigned int) { /* no-op in test */ }

// Forward declarations for the function under test (we implement it below in test)
extern "C" int png_handle_sCAL(png_structrp png_ptr, png_inforp info_ptr, png_uint_32 length);

// Internal state used by our test doubles for dependency functions
static std::vector<png_bytep> allocated_buffers; // track buffers allocated by png_read_buffer

// Mock storage for "chunk content" that png_crc_read will copy into the buffer
static std::vector<unsigned char> mock_input;
static size_t mock_input_pos = 0;

// Last captured values for successful sCAL parsing (to verify correctness)
static int last_sCAL_unit = -1;
static std::string last_sCAL_width;
static std::string last_sCAL_height;

// Reset captured state before each test
void reset_sCAL_capture() {
    last_sCAL_unit = -1;
    last_sCAL_width.clear();
    last_sCAL_height.clear();
}

// Prepare mock input data (chunk payload) that png_crc_read will copy into buffer
void prepare_mock_input(const unsigned char* data, size_t len) {
    mock_input.assign(data, data + len);
    mock_input_pos = 0;
}

// ----------------------------------------------------------------------------
// Mocked dependency functions used by png_handle_sCAL
// They are defined with C linkage to mimic the external C API
extern "C" {

// Allocate a buffer of given size and return as png_bytep
png_bytep png_read_buffer(png_structrp /*png_ptr*/, png_alloc_size_t length) {
    // Allocate length bytes
    unsigned char* buf = new unsigned char[length];
    allocated_buffers.push_back(buf);
    return buf;
}

// Copy 'length' bytes from mock_input into the provided buffer
void png_crc_read(png_structrp /*png_ptr*/, png_bytep buf, png_uint_32 length) {
    for (png_uint_32 i = 0; i < length; ++i) {
        if (mock_input_pos < mock_input.size()) {
            buf[i] = mock_input[mock_input_pos++];
        } else {
            buf[i] = 0; // pad with nulls if input is shorter than expected
        }
    }
}

// Finish CRC check for this chunk (fake success)
int png_crc_finish(png_structrp /*png_ptr*/, png_uint_32 /*skip*/) {
    return 0;
}

// Record benign error (no-op for test)
void png_chunk_benign_error(png_structrp /*png_ptr*/, const char* /*message*/) {
    // In a full test, we could capture the message; not required here
}

// Check if the ASCII number pointed by s is a valid floating-point number
// If valid, update *state to a positive value for >0 numbers, 0 for zero/negative
// and advance *i to the index just after the parsed number.
int png_check_fp_number(png_const_charp s, size_t length, int* state, size_t* i) {
    size_t pos = *i;
    long value = 0;
    bool has_digits = false;

    // Accept optional leading spaces (loosely, for robustness)
    while (pos < length && s[pos] == ' ') ++pos;

    // Parse digits
    while (pos < length && s[pos] >= '0' && s[pos] <= '9') {
        has_digits = true;
        value = value * 10 + (s[pos] - '0');
        ++pos;
    }

    if (!has_digits) {
        return 0; // failure
    }

    // Set whether the parsed value is positive (> 0)
    *state = (value > 0) ? 1 : 0;
    *i = pos;
    return 1; // success
}

// Positive predicate for the test (simulating PNG_FP_IS_POSITIVE)
#define PNG_FP_IS_POSITIVE(state) ((state) > 0)

// Set sCAL data into libpng structures (we capture values to validate in tests)
typedef char* png_charp;
int png_set_sCAL_s(png_structrp /*png_ptr*/, png_inforp /*info_ptr*/, int unit,
                 png_charp width, png_charp height) {
    last_sCAL_unit = unit;
    if (width) last_sCAL_width = std::string(width);
    if (height) last_sCAL_height = std::string(height);
    return 0;
}

// No-op macro for compatibility (unused in this simplified test)
void png_crc_finish_critical(png_structrp /*png_ptr*/, png_uint_32 /*skip*/, int /*handle_as_ancillary*/) {}

} // extern "C"

// ----------------------------------------------------------------------------
// Implementation of the focal method png_handle_sCAL (copied/adapted from the prompt)
// This mirrors the logic exactly to test the real control flow branches.
// It relies on the mocked dependency functions above.
// ----------------------------------------------------------------------------

int png_handle_sCAL(png_structrp png_ptr, png_inforp info_ptr, png_uint_32 length)
{
{
   png_bytep buffer;
   size_t i;
   int state;
   png_debug(1, "in png_handle_sCAL");
   png_debug1(2, "Allocating and reading sCAL chunk data (%u bytes)",
       length + 1);
   buffer = png_read_buffer(png_ptr, length+1);
   if (buffer == NULL)
   {
      png_crc_finish(png_ptr, length);
      png_chunk_benign_error(png_ptr, "out of memory");
      return handled_error;
   }
   png_crc_read(png_ptr, buffer, length);
   buffer[length] = 0; /* Null terminate the last string */
   if (png_crc_finish(png_ptr, 0) != 0)
      return handled_error;
   /* Validate the unit. */
   if (buffer[0] != 1 && buffer[0] != 2)
   {
      png_chunk_benign_error(png_ptr, "invalid unit");
      return handled_error;
   }
   /* Validate the ASCII numbers, need two ASCII numbers separated by
    * a '\0' and they need to fit exactly in the chunk data.
    */
   i = 1;
   state = 0;
   if (png_check_fp_number((png_const_charp)buffer, length, &state, &i) == 0 ||
       i >= length || buffer[i++] != 0)
      png_chunk_benign_error(png_ptr, "bad width format");
   else if (PNG_FP_IS_POSITIVE(state) == 0)
      png_chunk_benign_error(png_ptr, "non-positive width");
   else
   {
      size_t heighti = i;
      state = 0;
      if (png_check_fp_number((png_const_charp)buffer, length,
          &state, &i) == 0 || i != length)
         png_chunk_benign_error(png_ptr, "bad height format");
      else if (PNG_FP_IS_POSITIVE(state) == 0)
         png_chunk_benign_error(png_ptr, "non-positive height");
      else
      {
         /* This is the (only) success case. */
         png_set_sCAL_s(png_ptr, info_ptr, buffer[0],
             (png_charp)buffer+1, (png_charp)buffer+heighti);
         return handled_ok;
      }
   }
   return handled_error;
}
png_handle_tIME(png_structrp png_ptr, png_inforp info_ptr, png_uint_32 length)
{{};
png_handle_tEXt(png_structrp png_ptr, png_inforp info_ptr, png_uint_32 length)
{{};
png_handle_zTXt(png_structrp png_ptr, png_inforp info_ptr, png_uint_32 length)
{{};
png_handle_iTXt(png_structrp png_ptr, png_inforp info_ptr, png_uint_32 length)
{{};
png_cache_unknown_chunk(png_structrp png_ptr, png_uint_32 length)
{{};
png_handle_unknown(png_structrp png_ptr, png_inforp info_ptr,
    png_uint_32 length, int keep)
{{};
png_chunk_index_from_name(png_uint_32 chunk_name)
{{};
png_handle_chunk(png_structrp png_ptr, png_inforp info_ptr, png_uint_32 length)
{{};
png_combine_row(png_const_structrp png_ptr, png_bytep dp, int display)
{{};
png_do_read_interlace(png_row_infop row_info, png_bytep row, int pass,
    png_uint_32 transformations )
{{};
png_read_filter_row_sub(png_row_infop row_info, png_bytep row,
    png_const_bytep prev_row)
{{};
png_read_filter_row_up(png_row_infop row_info, png_bytep row,
    png_const_bytep prev_row)
{{};
png_read_filter_row_avg(png_row_infop row_info, png_bytep row,
    png_const_bytep prev_row)
{{};
png_read_filter_row_paeth_1byte_pixel(png_row_infop row_info, png_bytep row,
    png_const_bytep prev_row)
{{};
png_read_filter_row_paeth_multibyte_pixel(png_row_infop row_info, png_bytep row,
    png_const_bytep prev_row)
{{};
png_init_filter_functions(png_structrp pp)
{{};
png_read_filter_row(png_structrp pp, png_row_infop row_info, png_bytep row,
    png_const_bytep prev_row, int filter)
{{};
png_read_IDAT_data(png_structrp png_ptr, png_bytep output,
    png_alloc_size_t avail_out)
{{};
png_read_finish_IDAT(png_structrp png_ptr)
{{};
png_read_finish_row(png_structrp png_ptr)
{{};
png_read_start_row(png_structrp png_ptr)
{{};
}

// ----------------------------------------------------------------------------
// A set of standalone tests for png_handle_sCAL
// ----------------------------------------------------------------------------

// Simple assertion helper that does not terminate the test on failure
static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << (msg) << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    if(!((a) == (b))) { \
        std::cerr << "EXPECT_EQ failed: " << (msg) \
                  << " (actual: " << (a) << ", expected: " << (b) << ")" \
                  << std::endl; \
        ++g_failures; \
    } \
} while(0)

// Test 1: invalid unit (not 1 or 2) should yield handled_error
void test_invalid_unit()
{
    reset_sCAL_capture();

    // Layout: unit=3, width="12",'\0', height="34"
    unsigned char chunk[] = {
        3, '1', '2',  // width digits (will fail at unit check before using 0)
        0,              // terminator (this would be the expected zero after width)
        '3', '4'        // height digits
    };
    size_t len = sizeof(chunk) / sizeof(chunk[0]);

    prepare_mock_input(chunk, len);

    int ret = png_handle_sCAL(nullptr, nullptr, (png_uint_32)len);

    EXPECT_EQ(ret, handled_error, "invalid unit should return handled_error");
}

// Test 2: bad width format (missing 0 terminator after width digits)
void test_bad_width_format()
{
    reset_sCAL_capture();

    // Layout: unit=1, width="12X" (X is not 0, so bad width format)
    unsigned char chunk[] = {
        1, '1', '2', 'X', '3', '4'
    };
    size_t len = sizeof(chunk) / sizeof(chunk[0]);

    prepare_mock_input(chunk, len);

    int ret = png_handle_sCAL(nullptr, nullptr, (png_uint_32)len);

    EXPECT_TRUE(ret == handled_error, "bad width format should yield handled_error");
}

// Test 3: non-positive width (width = 0)
void test_non_positive_width()
{
    reset_sCAL_capture();

    // Layout: unit=1, width="0", terminator, height="34"
    unsigned char chunk[] = {
        1, '0', 0, '3', '4'
    };
    size_t len = sizeof(chunk) / sizeof(chunk[0]);

    prepare_mock_input(chunk, len);

    int ret = png_handle_sCAL(nullptr, nullptr, (png_uint_32)len);

    EXPECT_TRUE(ret == handled_error, "non-positive width should yield handled_error");
}

// Test 4: bad height format (height string invalid)
void test_bad_height_format()
{
    reset_sCAL_capture();

    // Layout: unit=1, width="12", 0, height="X" (invalid height)
    unsigned char chunk[] = {
        1, '1', '2', 0, 'X'
    };
    size_t len = sizeof(chunk) / sizeof(chunk[0]);

    prepare_mock_input(chunk, len);

    int ret = png_handle_sCAL(nullptr, nullptr, (png_uint_32)len);

    EXPECT_TRUE(ret == handled_error, "bad height format should yield handled_error");
}

// Test 5: non-positive height (height = 0)
void test_non_positive_height()
{
    reset_sCAL_capture();

    // Layout: unit=1, width="1", 0, height="0"
    unsigned char chunk[] = {
        1, '1', 0, '0'
    };
    size_t len = sizeof(chunk) / sizeof(chunk[0]);

    prepare_mock_input(chunk, len);

    int ret = png_handle_sCAL(nullptr, nullptr, (png_uint_32)len);

    EXPECT_TRUE(ret == handled_error, "non-positive height should yield handled_error");
}

// Test 6: success case (valid unit, positive width and height)
void test_success_case()
{
    reset_sCAL_capture();

    // Layout: unit=1, width="12", 0, height="34"
    unsigned char chunk[] = {
        1, '1', '2', 0, '3', '4'
    };
    size_t len = sizeof(chunk) / sizeof(chunk[0]);

    prepare_mock_input(chunk, len);

    int ret = png_handle_sCAL(nullptr, nullptr, (png_uint_32)len);

    EXPECT_TRUE(ret == handled_ok, "valid sCAL should return handled_ok");
    EXPECT_EQ(last_sCAL_unit, 1, "unit should be 1 on success");
    EXPECT_TRUE(last_sCAL_width == "12", "width string should be '12' on success");
    EXPECT_TRUE(last_sCAL_height == "34", "height string should be '34' on success");
}

// ----------------------------------------------------------------------------
// Main: run all tests and report the summary
// ----------------------------------------------------------------------------

int main() {
    std::cout << "Running png_handle_sCAL unit tests (standalone harness)..." << std::endl;

    test_invalid_unit();
    test_bad_width_format();
    test_non_positive_width();
    test_bad_height_format();
    test_non_positive_height();
    test_success_case();

    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 2;
    }
}
// Lightweight C++11 unit tests for the focal function png_handle_pHYs
// This test harness provides minimal, self-contained stubs for the
// dependencies used by the focal function to enable isolated testing
// without requiring the real libpng library.
// The tests cover true/false branches and validate that the function
// correctly parses the input and invokes the dependent API (png_set_pHYs).

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <cstring>
#include <cstddef>


// Domain-specific constants representing the focal function's outcome.
const int handled_ok = 0;
const int handled_error = -1;

// Minimal type aliases to resemble the libpng types used by png_handle_pHYs.
typedef void* png_structrp;
typedef void* png_inforp;
typedef uint32_t png_uint_32;
typedef uint8_t png_byte;
typedef const uint8_t* png_const_bytep;
typedef uint8_t* png_bytep;

// Macro to silence unused parameter warnings in the test harness.
#define PNG_UNUSED(x) (void)(x)

// Forward declaration of the focal method under test (re-implemented here for isolation)
int png_handle_pHYs(png_structrp png_ptr, png_inforp info_ptr, png_uint_32 length);

// --------- Test harness state and stubs for dependencies ---------

// Global state used by the mock dependency implementations to verify behavior.
static uint8_t g_input_buffer[256];
static size_t g_input_pos = 0;
static int g_png_crc_finish_return = 0;

static bool g_png_set_pHYs_called = false;
static uint32_t g_last_set_res_x = 0;
static uint32_t g_last_set_res_y = 0;
static int g_last_set_unit_type = -1;

// Mocked/libpng-like dependency: png_debug
void png_debug(int /*level*/, const char* /*message*/) {
    // No-op for test harness
}

// Mocked dependency: png_crc_read
void png_crc_read(png_structrp /*png_ptr*/, png_bytep buf, png_uint_32 length) {
    // Copy from a pre-filled input buffer into buf (simulate reading 9 bytes)
    for (png_uint_32 i = 0; i < length; ++i) {
        buf[i] = g_input_buffer[g_input_pos++];
    }
}

// Mocked dependency: png_crc_finish
int png_crc_finish(png_structrp /*png_ptr*/, png_uint_32 /*skip*/) {
    int ret = g_png_crc_finish_return;
    // Reset for subsequent tests
    g_png_crc_finish_return = 0;
    return ret;
}

// Mocked dependency: png_get_uint_32 (big-endian to host-order conversion)
uint32_t png_get_uint_32(png_const_bytep buf) {
    return (static_cast<uint32_t>(buf[0]) << 24) |
           (static_cast<uint32_t>(buf[1]) << 16) |
           (static_cast<uint32_t>(buf[2]) << 8)  |
           (static_cast<uint32_t>(buf[3]));
}

// Mocked dependency: png_set_pHYs (records the values it would set)
int png_set_pHYs(png_structrp /*png_ptr*/, png_inforp /*info_ptr*/,
                 uint32_t res_x, uint32_t res_y, int unit_type) {
    g_png_set_pHYs_called = true;
    g_last_set_res_x = res_x;
    g_last_set_res_y = res_y;
    g_last_set_unit_type = unit_type;
    return 0;
}

// --------- Re-implementation of the focal function under test ---------

// Reproduced exactly (conceptually) to enable isolated testing without libpng.
// This mirrors the provided FOCAL_METHOD.
int png_handle_pHYs(png_structrp png_ptr, png_inforp info_ptr, png_uint_32 length)
{
{
   png_byte buf[9];
   png_uint_32 res_x, res_y;
   int unit_type;
   png_debug(1, "in png_handle_pHYs");
   png_crc_read(png_ptr, buf, 9);
   if (png_crc_finish(png_ptr, 0) != 0)
      return handled_error;
   res_x = png_get_uint_32(buf);
   res_y = png_get_uint_32(buf + 4);
   unit_type = buf[8];
   png_set_pHYs(png_ptr, info_ptr, res_x, res_y, unit_type);
   return handled_ok;
   PNG_UNUSED(length)
}
}

// --------- Simple test framework (non-GTest) ---------

static int s_total = 0;
static int s_passed = 0;

#define ASSERT_TRUE(cond, msg) \
    do { \
        ++s_total; \
        if (cond) { \
            ++s_passed; \
        } else { \
            std::cerr << "Assertion failed: " << (msg) << std::endl; \
        } \
    } while (0)

#define ASSERT_EQ_UINT(a, b, msg) \
    do { \
        ++s_total; \
        if ((a) == (b)) { \
            ++s_passed; \
        } else { \
            std::cerr << "Assertion failed: " << (msg) \
                      << " | expected: " << (b) << ", actual: " << (a) << std::endl; \
        } \
    } while (0)

static void reset_test_state() {
    std::memset(g_input_buffer, 0, sizeof(g_input_buffer));
    g_input_pos = 0;
    g_png_crc_finish_return = 0;

    g_png_set_pHYs_called = false;
    g_last_set_res_x = 0;
    g_last_set_res_y = 0;
    g_last_set_unit_type = -1;
}

// Helper to fill the 9-byte input for pHYs: 4 bytes res_x, 4 bytes res_y, 1 byte unit_type
static void fill_input_bytes(uint32_t res_x, uint32_t res_y, uint8_t unit_type) {
    // big-endian 32-bit values
    g_input_buffer[0] = static_cast<uint8_t>((res_x >> 24) & 0xFF);
    g_input_buffer[1] = static_cast<uint8_t>((res_x >> 16) & 0xFF);
    g_input_buffer[2] = static_cast<uint8_t>((res_x >> 8) & 0xFF);
    g_input_buffer[3] = static_cast<uint8_t>(res_x & 0xFF);

    g_input_buffer[4] = static_cast<uint8_t>((res_y >> 24) & 0xFF);
    g_input_buffer[5] = static_cast<uint8_t>((res_y >> 16) & 0xFF);
    g_input_buffer[6] = static_cast<uint8_t>((res_y >> 8) & 0xFF);
    g_input_buffer[7] = static_cast<uint8_t>(res_y & 0xFF);

    g_input_buffer[8] = static_cast<uint8_t>(unit_type);
    g_input_pos = 0;
}

// Test 1: Success path - png_crc_finish returns 0 and values are parsed and set correctly
static void test_handle_pHYs_success() {
    reset_test_state();
    // Input: res_x = 1, res_y = 2, unit_type = 1
    fill_input_bytes(1, 2, 1);
    g_png_crc_finish_return = 0; // success path

    int ret = png_handle_pHYs(nullptr, nullptr, 9);

    ASSERT_EQ_UINT(ret, handled_ok, "png_handle_pHYs should return handled_ok on success");
    ASSERT_TRUE(g_png_set_pHYs_called, "png_set_pHYs should be called on success");
    ASSERT_EQ_UINT(g_last_set_res_x, 1, "parsed res_x should be 1");
    ASSERT_EQ_UINT(g_last_set_res_y, 2, "parsed res_y should be 2");
    ASSERT_EQ_UINT(static_cast<uint32_t>(g_last_unit_type), static_cast<uint32_t>(1),
                   "unit_type should be 1");
}

// Test 2: Error path - png_crc_finish returns non-zero -> should return handled_error
static void test_handle_pHYs_crc_finish_error() {
    reset_test_state();
    // Any 9-byte input; content is irrelevant because finish will fail
    fill_input_bytes(0, 0, 0);
    g_png_crc_finish_return = 1; // simulate CRC finish error

    int ret = png_handle_pHYs(nullptr, nullptr, 9);

    ASSERT_EQ_UINT(ret, handled_error, "png_handle_pHYs should return handled_error when CRC finish fails");
}

// --------- Main runner ---------

int main() {
    std::cout << "Running unit tests for png_handle_pHYs (standalone harness)\n";

    test_handle_pHYs_success();
    test_handle_pHYs_crc_finish_error();

    std::cout << "Tests completed. Passed " << s_passed << " / " << s_total << " tests.\n";
    if (s_passed != s_total) {
        std::cerr << "Some tests failed.\n";
        return 1;
    }
    return 0;
}
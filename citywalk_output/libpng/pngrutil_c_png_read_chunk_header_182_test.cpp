// Lightweight C++11 test suite for png_read_chunk_header (no GTest, no external deps)

// This test harness provides a minimal mock of the libpng-like environment
// required by the focal function. It purposely implements only what is
// necessary to exercise png_read_chunk_header and its basic error paths.

#include <cstdint>
#include <cassert>
#include <iostream>
#include <pngpriv.h>
#include <cmath>
#include <cstdarg>
#include <stdexcept>
#include <cstring>
#include <cstddef>


// Domain knowledge: Use only standard library facilities and provided methods.
// Namespace: keep tests in global namespace for simplicity per domain guidance.

// --------------------------- Mocked PNG Interfaces ---------------------------

// Basic PNG style typedefs (subset)
using png_byte = uint8_t;
using png_uint_32 = uint32_t;
using png_uint_16 = uint16_t;
using png_bytep = png_byte*;
using png_const_bytep = const png_byte*;
using png_structrp = struct png_struct_def*;
using png_const_structrp = const struct png_struct_def*;

// Minimal png_struct to hold test state
struct png_struct_def {
    unsigned int io_state;           // For IO state testing
    png_uint_32 chunk_name;            // Stores last parsed chunk name
    unsigned char read_buffer[16];     // Source data for png_read_data
    size_t read_pos;                   // Current read position in buffer
    size_t read_len;                   // Total length of valid data in buffer
    char last_error[256];              // Capture error messages (if needed)
    unsigned int error_flag;           // Simple error flag (not used in tests)
};

// Forward declarations of functions used by the focal method
// Implementations provided below in this test file.
static void png_read_data(png_structrp png_ptr, png_bytep data, size_t length);
static png_uint_32 png_get_uint_31(png_const_structrp png_ptr, png_const_bytep buf);
static png_uint_32 PNG_CHUNK_FROM_STRING(png_const_bytep str);
static void png_reset_crc(png_structrp png_ptr);
static void png_calculate_crc(png_structrp png_ptr, png_const_bytep ptr, size_t length);
static void png_debug2(int level, const char* format, ...);
static void png_chunk_error(png_structrp png_ptr, const char* msg);
static bool check_chunk_name(png_uint_32 name);

// PNG IO state macro control (to enable/disable IO state tests)
#define PNG_IO_STATE_SUPPORTED 1

// IO state component constants
static const unsigned int PNG_IO_READING     = 0x01;
static const unsigned int PNG_IO_CHUNK_HDR   = 0x02;
static const unsigned int PNG_IO_CHUNK_DATA  = 0x04;

// --------------------------- Focal Method (adapted) ---------------------------
// The function under test, adapted to use the mock environment above.

static png_uint_32 png_read_chunk_header(png_structrp png_ptr)
{
    {
       png_byte buf[8];
       png_uint_32 chunk_name, length;
#ifdef PNG_IO_STATE_SUPPORTED
       png_ptr->io_state = PNG_IO_READING | PNG_IO_CHUNK_HDR;
#endif
       // Read 8 bytes: [length(4 bytes), chunk_type(4 bytes)]
       png_read_data(png_ptr, buf, 8);
       length = png_get_uint_31(png_ptr, buf);
       png_ptr->chunk_name = chunk_name = PNG_CHUNK_FROM_STRING(buf + 4);
       /* Reset the crc and run it over the chunk name. */
       png_reset_crc(png_ptr);
       png_calculate_crc(png_ptr, buf + 4, 4);
       png_debug2(0, "Reading chunk typeid = 0x%lx, length = %lu",
           (unsigned long)png_ptr->chunk_name, (unsigned long)length);
       /* Sanity check the length (first by <= 0x80) and the chunk name.  An error
        * here indicates a broken stream and libpng has no recovery from this.
        */
       if (buf[0] >= 0x80U)
          png_chunk_error(png_ptr, "bad header (invalid length)");
       /* Check to see if chunk name is valid. */
       if (!check_chunk_name(chunk_name))
          png_chunk_error(png_ptr, "bad header (invalid type)");
#ifdef PNG_IO_STATE_SUPPORTED
       png_ptr->io_state = PNG_IO_READING | PNG_IO_CHUNK_DATA;
#endif
       return length;
    }
}

// --------------------------- Implementations ---------------------------

// Copy 8 bytes from the test buffer to the destination
static void png_read_data(png_structrp png_ptr, png_bytep data, size_t length)
{
    if (png_ptr == nullptr || data == nullptr) return;
    // Clamp length to available data
    if (png_ptr->read_pos + length > png_ptr->read_len) {
        if (png_ptr->read_pos >= png_ptr->read_len) {
            length = 0;
        } else {
            length = png_ptr->read_len - png_ptr->read_pos;
        }
    }
    if (length > 0) {
        std::memcpy(data, png_ptr->read_buffer + png_ptr->read_pos, length);
        png_ptr->read_pos += length;
    }
}

// 31-bit big-endian integer from 4-byte buffer
static png_uint_32 png_get_uint_31(png_const_structrp /*png_ptr*/, png_const_bytep buf)
{
    uint32_t v = ((uint32_t)buf[0] << 24) |
                 ((uint32_t)buf[1] << 16) |
                 ((uint32_t)buf[2] << 8)  |
                 ((uint32_t)buf[3]);
    // Keep only 31 bits
    return v & 0x7fffffff;
}

// Build a 32-bit name from 4 ASCII characters
static png_uint_32 PNG_CHUNK_FROM_STRING(png_const_bytep str)
{
    return ((uint32_t)str[0] << 24) |
           ((uint32_t)str[1] << 16) |
           ((uint32_t)str[2] << 8)  |
           ((uint32_t)str[3]);
}

// CRC helpers (no-op for test harness)
static void png_reset_crc(png_structrp /*png_ptr*/)
{
    // No-op in test harness
}
static void png_calculate_crc(png_structrp /*png_ptr*/, png_const_bytep /*ptr*/, size_t /*length*/)
{
    // No-op in test harness
}
static void png_debug2(int /*level*/, const char* /*format*/, ...)
{
    // Lightweight noop for test harness
}

// Error handling in tests: throw to simulate libpng error behavior

static void png_chunk_error(png_structrp /*png_ptr*/, const char* msg)
{
    // Propagate as an exception to allow test to detect error paths
    throw std::runtime_error(msg);
}

// Chunk name validity: all four bytes must be ASCII letters A-Z or a-z
static bool check_chunk_name(png_uint_32 name)
{
    unsigned char b0 = (name >> 24) & 0xFF;
    unsigned char b1 = (name >> 16) & 0xFF;
    unsigned char b2 = (name >> 8)  & 0xFF;
    unsigned char b3 =  name        & 0xFF;
    auto isalpha = [](unsigned char c) -> bool {
        return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
    };
    return isalpha(b0) && isalpha(b1) && isalpha(b2) && isalpha(b3);
}

// --------------------------- Lightweight Test Framework ---------------------------

static int g_total = 0;
static int g_failed = 0;

// Simple non-terminating assertion helper
#define ASSERT_TRUE(cond, msg) do { \
    ++g_total; \
    if (!(cond)) { \
        ++g_failed; \
        std::cerr << "ASSERTION FAILED: " << (msg) << " in " << __func__ << "()" << std::endl; \
    } \
} while(0)

#define ASSERT_EQ(a, b, msg) do { \
    ++g_total; \
    if (!((a) == (b))) { \
        ++g_failed; \
        std::cerr << "ASSERTION FAILED: " << (msg) \
                  << " (expected: " << (a) << ", actual: " << (b) \
                  << ") in " << __func__ << "()" << std::endl; \
    } \
} while(0)

// Per-test descriptive functions

// Test 1: Valid header path
// - Reads a standard 4-byte length and 4-byte chunk type (e.g., "IHDR")
// - Verifies length, chunk_name, and final IO state
static void test_valid_header()
{
    // Initialize a fake png_ptr
    static png_struct_def ctx;
    std::memset(&ctx, 0, sizeof(ctx));
    ctx.io_state = 0;
    ctx.read_pos = 0;
    // Prepare 8-byte stream: length = 4, chunk name = "IHDR"
    const unsigned char stream[8] = {
        0x00, 0x00, 0x00, 0x04, // length = 4
        'I', 'H', 'D', 'R'       // chunk type
    };
    std::memcpy(ctx.read_buffer, stream, 8);
    ctx.read_len = 8;
    ctx.read_pos = 0;

    try {
        png_uint_32 length = png_read_chunk_header(&ctx);
        // Verify outcomes
        unsigned long expected_name = ((unsigned long)('I') << 24) |
                                    ((unsigned long)('H') << 16) |
                                    ((unsigned long)('D') << 8)  |
                                    ((unsigned long)('R'));
        ASSERT_EQ(length, 4u, "length should be 4 for IHDR header");
        ASSERT_EQ((unsigned long)ctx.chunk_name, expected_name, "chunk_name should reflect 'IHDR'");
        ASSERT_TRUE( (ctx.io_state & (PNG_IO_READING | PNG_IO_CHUNK_DATA)) == (PNG_IO_READING | PNG_IO_CHUNK_DATA),
                     "io_state should be READING|CHUNK_DATA after header");
    } catch (const std::exception& ex) {
        std::cerr << "EXCEPTION in test_valid_header: " << ex.what() << std::endl;
        ++g_failed;
    }
}

// Test 2: Invalid length in header triggers error path (buf[0] >= 0x80)
// - Expect an exception to be thrown
static void test_invalid_length()
{
    static png_struct_def ctx;
    std::memset(&ctx, 0, sizeof(ctx));
    ctx.io_state = 0;
    ctx.read_pos = 0;
    // 8-byte stream where first byte has high bit set
    const unsigned char stream[8] = {
        0x81, 0x00, 0x00, 0x00, // length's high bit set
        'I', 'H', 'D', 'R'        // chunk type
    };
    std::memcpy(ctx.read_buffer, stream, 8);
    ctx.read_len = 8;
    ctx.read_pos = 0;

    bool caught = false;
    try {
        png_read_chunk_header(&ctx);
    } catch (const std::exception&) {
        caught = true;
    }
    ASSERT_TRUE(caught, "invalid length should trigger png_chunk_error (exception)");
}

// Test 3: Invalid chunk type (non-letter) triggers error path
// - Expect an exception to be thrown
static void test_invalid_type()
{
    static png_struct_def ctx;
    std::memset(&ctx, 0, sizeof(ctx));
    ctx.io_state = 0;
    ctx.read_pos = 0;
    // 8-byte stream with invalid chunk name (digits)
    const unsigned char stream[8] = {
        0x00, 0x00, 0x00, 0x04, // length = 4
        '1', '2', '3', '4'        // chunk type invalid (digits)
    };
    std::memcpy(ctx.read_buffer, stream, 8);
    ctx.read_len = 8;
    ctx.read_pos = 0;

    bool caught = false;
    try {
        png_read_chunk_header(&ctx);
    } catch (const std::exception&) {
        caught = true;
    }
    ASSERT_TRUE(caught, "invalid type should trigger png_chunk_error (exception)");
}

// --------------------------- Test Runner ---------------------------

static void run_all_tests()
{
    std::cout << "Running png_read_chunk_header test suite (no GTest)..." << std::endl;

    test_valid_header();
    test_invalid_length();
    test_invalid_type();

    std::cout << std::endl;
    std::cout << "Tests run: " << g_total << ", Failures: " << g_failed << std::endl;
}

int main()
{
    run_all_tests();

    if (g_failed > 0) {
        std::cerr << "Some tests FAILED." << std::endl;
        return 1;
    } else {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    }
}
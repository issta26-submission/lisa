// High-quality, self-contained C++11 unit tests for the focal method
// png_push_read_IDAT, re-implemented in this test harness to run without GTest.
// The goal is to exercise true/false branches and key internal state transitions
// of the method, using a minimal, controlled subset of the png_struct API.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Domain knowledge driven helpers and minimal stubs to run the focal method
// without depending on the real libpng library.

using uint32 = uint32_t;
using uint8  = uint8_t;
using size_t_ref = size_t;

// Forward declaration of the focal function (adapted to C++ with explicit types)
struct png_struct;
typedef png_struct* png_structrp;
typedef unsigned char png_byte;
typedef unsigned char* png_bytep;
typedef const unsigned char* png_const_bytep;

// Minimal, test-oriented definitions to mimic necessary libpng internals

// Type alias for the internal append/ptr used by the function
struct png_struct {
    // internal state used by png_push_read_IDAT
    unsigned int mode;
    unsigned int process_mode;
    unsigned int push_length;
    unsigned int chunk_name;
    unsigned int idat_size;
    size_t save_buffer_size;
    png_bytep save_buffer_ptr;
    size_t current_buffer_size;
    png_bytep current_buffer_ptr;
    size_t buffer_size;
    unsigned int flags;
    unsigned int zowner;

    // error reporting used by tests
    bool error;
    const char* error_msg;

    // Input stream simulation for tests
    const png_bytep input;
    size_t input_len;
    size_t input_pos;

    // Test accounting
    size_t processed_bytes;

    // Constructor initializer for convenience
    png_struct(const png_bytep in = nullptr, size_t ilen = 0)
        : mode(0), process_mode(0), push_length(0), chunk_name(0),
          idat_size(0), save_buffer_size(0), save_buffer_ptr(nullptr),
          current_buffer_size(0), current_buffer_ptr(nullptr),
          buffer_size(0), flags(0), zowner(0),
          error(false), error_msg(nullptr),
          input(in), input_len(ilen), input_pos(0),
          processed_bytes(0)
    {}
};

// Helper types
typedef png_struct png_struct;

// Helper constants mirroring libpng internal flags and chunk IDs
#define PNG_HAVE_CHUNK_HEADER   0x01
#define PNG_AFTER_IDAT           0x02
#define PNG_READ_CHUNK_MODE      0x01 // distinct value for test clarity
#define PNG_FLAG_ZSTREAM_ENDED     (1u << 0)

#define PNG_IDAT 0x49444154 // 'IDAT'

// Macros no-ops for test environment
#define PNG_PUSH_SAVE_BUFFER_IF_LT(n) do { } while(0)

// Forward declarations for functions used by the focal method
static void png_error(png_structrp png_ptr, const char* msg);
static void png_reset_crc(png_structrp png_ptr);
static void png_crc_read(png_structrp png_ptr, png_bytep buffer, size_t length);
static uint32 png_get_uint_31(png_structrp png_ptr, png_bytep p);
static void png_push_fill_buffer(png_structrp png_ptr, png_bytep buffer, size_t length);
static void png_calculate_crc(png_structrp png_ptr, png_bytep buffer, size_t length);
static void png_process_IDAT_data(png_structrp png_ptr, png_bytep buffer, size_t length);
static void png_crc_finish(png_structrp png_ptr, int dummy);
static inline uint32 PNG_CHUNK_FROM_STRING(const png_bytep tag);

// The focal method adapted to C++ (signature changed to explicit void return)
static void png_push_read_IDAT(png_structrp png_ptr)
{
    {
       if ((png_ptr->mode & PNG_HAVE_CHUNK_HEADER) == 0)
       {
          png_byte chunk_length[4];
          png_byte chunk_tag[4];
          PNG_PUSH_SAVE_BUFFER_IF_LT(8)
          png_push_fill_buffer(png_ptr, chunk_length, 4);
          png_ptr->push_length = png_get_uint_31(png_ptr, chunk_length);
          png_reset_crc(png_ptr);
          png_crc_read(png_ptr, chunk_tag, 4);
          png_ptr->chunk_name = PNG_CHUNK_FROM_STRING(chunk_tag);
          png_ptr->mode |= PNG_HAVE_CHUNK_HEADER;
          if (png_ptr->chunk_name != PNG_IDAT)
          {
             png_ptr->process_mode = PNG_READ_CHUNK_MODE;
             if ((png_ptr->flags & PNG_FLAG_ZSTREAM_ENDED) == 0)
                png_error(png_ptr, "Not enough compressed data");
             return;
          }
          png_ptr->idat_size = png_ptr->push_length;
       }
       if (png_ptr->idat_size != 0 && png_ptr->save_buffer_size != 0)
       {
          size_t save_size = png_ptr->save_buffer_size;
          uint32 idat_size = png_ptr->idat_size;
          if (idat_size < save_size)
             save_size = (size_t)idat_size;
          else
             idat_size = (uint32)save_size;
          png_calculate_crc(png_ptr, png_ptr->save_buffer_ptr, save_size);
          png_process_IDAT_data(png_ptr, png_ptr->save_buffer_ptr, save_size);
          png_ptr->idat_size -= idat_size;
          png_ptr->buffer_size -= save_size;
          png_ptr->save_buffer_size -= save_size;
          png_ptr->save_buffer_ptr += save_size;
       }
       if (png_ptr->idat_size != 0 && png_ptr->current_buffer_size != 0)
       {
          size_t save_size = png_ptr->current_buffer_size;
          uint32 idat_size = png_ptr->idat_size;
          if (idat_size < save_size)
             save_size = (size_t)idat_size;
          else
             idat_size = (uint32)save_size;
          png_calculate_crc(png_ptr, png_ptr->current_buffer_ptr, save_size);
          png_process_IDAT_data(png_ptr, png_ptr->current_buffer_ptr, save_size);
          png_ptr->idat_size -= idat_size;
          png_ptr->buffer_size -= save_size;
          png_ptr->current_buffer_size -= save_size;
          png_ptr->current_buffer_ptr += save_size;
       }
       if (png_ptr->idat_size == 0)
       {
          PNG_PUSH_SAVE_BUFFER_IF_LT(4)
          png_crc_finish(png_ptr, 0);
          png_ptr->mode &= ~PNG_HAVE_CHUNK_HEADER;
          png_ptr->mode |= PNG_AFTER_IDAT;
          png_ptr->zowner = 0;
       }
    }
}

//////////////////////////////////////////////////////////////
// Minimal, test-focused implementations of supporting routines
//////////////////////////////////////////////////////////////

static void png_error(png_structrp png_ptr, const char* msg)
{
    if (png_ptr) {
        png_ptr->error = true;
        png_ptr->error_msg = msg;
    }
}

static void png_reset_crc(png_structrp /*png_ptr*/)
{
    // no-op for test harness
}

static void png_crc_read(png_structrp png_ptr, png_bytep buffer, size_t length)
{
    // Read 'length' bytes from the simulated input into 'buffer'
    if (!png_ptr) return;
    if (png_ptr->input_pos + length <= png_ptr->input_len) {
        // Copy from test input into buffer
        const unsigned char* src = png_ptr->input + png_ptr->input_pos;
        std::memcpy(buffer, src, length);
        png_ptr->input_pos += length;
    } else {
        // Not enough input; fill with zeroes
        size_t avail = (png_ptr->input_len > png_ptr->input_pos) ? (png_ptr->input_len - png_ptr->input_pos) : 0;
        if (avail > 0) {
            std::memcpy(buffer, png_ptr->input + png_ptr->input_pos, avail);
            std::memset(buffer + avail, 0, length - avail);
            png_ptr->input_pos += avail;
        } else {
            std::memset(buffer, 0, length);
        }
    }
}

static uint32 png_get_uint_31(png_structrp /*png_ptr*/, png_bytep p)
{
    // Treat chunk_length as 4-byte big-endian
    return ( (uint32)p[0] << 24) | ((uint32)p[1] << 16) | ((uint32)p[2] << 8) | ((uint32)p[3]);
}

static void png_push_fill_buffer(png_structrp png_ptr, png_bytep buffer, size_t length)
{
    // Fill '(buffer)' by taking bytes from the test input
    if (!png_ptr) return;
    if (png_ptr->input_pos + length <= png_ptr->input_len) {
        std::memcpy(buffer, png_ptr->input + png_ptr->input_pos, length);
        png_ptr->input_pos += length;
    } else {
        size_t avail = (png_ptr->input_len > png_ptr->input_pos) ? (png_ptr->input_len - png_ptr->input_pos) : 0;
        if (avail > 0) {
            std::memcpy(buffer, png_ptr->input + png_ptr->input_pos, avail);
            std::memset(buffer + avail, 0, length - avail);
            png_ptr->input_pos += avail;
        } else {
            std::memset(buffer, 0, length);
        }
    }
}

static void png_calculate_crc(png_structrp /*png_ptr*/, png_bytep /*buffer*/, size_t /*length*/)
{
    // No-CRC calculation in test harness; marker only
}

static void png_process_IDAT_data(png_structrp png_ptr, png_bytep /*buffer*/, size_t length)
{
    if (png_ptr) {
        png_ptr->processed_bytes += length;
    }
}

static void png_crc_finish(png_structrp /*png_ptr*/, int /*dummy*/)
{
    // No CRC finishing in test harness
}

static inline uint32 PNG_CHUNK_FROM_STRING(const png_bytep tag)
{
    // Convert 4-byte tag to 32-bit code
    return ((uint32)tag[0] << 24) | ((uint32)tag[1] << 16) | ((uint32)tag[2] << 8) | ((uint32)tag[3]);
}

//////////////////////////////////////////
// Lightweight, self-contained unit tests
//////////////////////////////////////////

// Simple test framework (non-terminating assertions)
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << (msg) << " [" << __FILE__ << ":" << __LINE__ << "]\n"; \
        ++g_tests_failed; \
    } else { ++g_tests_passed; } \
} while(0)

#define EXPECT_EQ(a,b, msg) do { \
    if (!((a) == (b))) { \
        std::cerr << "EXPECT_EQ failed: " << (msg) << " ((" << (a) << ") != (" << (b) << ")) [" << __FILE__ << ":" << __LINE__ << "]\n"; \
        ++g_tests_failed; \
    } else { ++g_tests_passed; } \
} while(0)

// Helper to run and print summary
static void print_summary() {
    int total = g_tests_passed + g_tests_failed;
    std::cout << "Tests run: " << total << ", Passed: " << g_tests_passed << ", Failed: " << g_tests_failed << "\n";
}

// Test 1: Not enough data when non-IDAT chunk encountered with ZSTREAM_ENDED not set
static void test_NotEnoughCompressedData()
{
    png_byte input_bytes[8] = {
        0x00, 0x00, 0x00, 0x04, // chunk_length = 4
        'I', 'H', 'D', 'R'        // chunk_tag = 'IHDR' (non-IDAT)
    };

    png_struct png;
    std::memset(&png, 0, sizeof(png));
    png_structrp ptr = &png;

    // Prepare test state
    ptr->mode = 0;
    ptr->flags = 0;     // ZSTREAM_ENDED not set
    ptr->input = input_bytes;
    ptr->input_len = sizeof(input_bytes);
    ptr->input_pos = 0;
    ptr->save_buffer_size = 0;
    ptr->save_buffer_ptr = nullptr;
    ptr->current_buffer_size = 0;
    ptr->current_buffer_ptr = nullptr;

    // Push length handling requires push_length to be filled by png_get_uint_31
    // The function will fill via png_push_fill_buffer and then call png_crc_read
    ptr->chunk_name = 0;
    ptr->error = false;
    ptr->error_msg = nullptr;

    png_push_read_IDAT(ptr);

    // Expect an error due to not enough compressed data
    EXPECT_TRUE(ptr->error, "png_push_read_IDAT should set error for non-IDAT with ZSTREAM_ENDED unset");
    EXPECT_TRUE(ptr->error_msg != nullptr && std::strcmp(ptr->error_msg, "Not enough compressed data") == 0,
                "Error message should indicate Not enough compressed data");
}

// Test 2: Non-IDAT chunk with ZSTREAM_ENDED set should not error and set header flag
static void test_HeaderEnded()
{
    png_byte input_bytes[8] = {
        0x00, 0x00, 0x00, 0x04, // chunk_length = 4
        'I', 'H', 'D', 'R'        // chunk_tag = 'IHDR'
    };

    png_struct png;
    std::memset(&png, 0, sizeof(png));
    png_structrp ptr = &png;

    ptr->mode = 0;
    ptr->flags = PNG_FLAG_ZSTREAM_ENDED; // ZSTREAM_ENDED set
    ptr->input = input_bytes;
    ptr->input_len = sizeof(input_bytes);
    ptr->input_pos = 0;
    ptr->save_buffer_size = 0;
    ptr->save_buffer_ptr = nullptr;
    ptr->current_buffer_size = 0;
    ptr->current_buffer_ptr = nullptr;

    ptr->error = false;
    ptr->error_msg = nullptr;

    png_push_read_IDAT(ptr);

    // No error expected
    EXPECT_TRUE(ptr->error == false, "No error should be reported when ZSTREAM_ENDED is set");
    // mode should have the header bit set
    EXPECT_TRUE((ptr->mode & PNG_HAVE_CHUNK_HEADER) != 0, "PNG_HAVE_CHUNK_HEADER should be set after header read");
}

// Test 3: IDAT path with idat_size > 0 and save_buffer_size > 0 (full processing in one step)
static void test_IDAT_fullProcess()
{
    // Build a single IDAT chunk header: length=4, tag=IDAT
    png_byte input_bytes[8] = {
        0x00, 0x00, 0x00, 0x04, // chunk_length = 4
        'I', 'D', 'A', 'T'        // chunk_tag = 'IDAT'
    };

    // Test buffers to simulate IDAT data
    unsigned char save_buf[8] = { 0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,0,0 };
    unsigned char current_buf[4] = { 0,0,0,0 };

    png_struct png;
    std::memset(&png, 0, sizeof(png));
    png_structrp ptr = &png;

    ptr->mode = 0;
    ptr->flags = 0;
    ptr->input = input_bytes;
    ptr->input_len = sizeof(input_bytes);
    ptr->input_pos = 0;

    // Prepare IDAT data environment
    ptr->save_buffer_size = 8;
    ptr->save_buffer_ptr = save_buf;
    ptr->current_buffer_size = 0;
    ptr->current_buffer_ptr = nullptr;
    ptr->buffer_size = 100; // arbitrary large
    ptr->idat_size = 0; // will be set by function, push_length
    ptr->error = false;
    ptr->error_msg = nullptr;

    // We want to ensure that push_length is treated as 4
    // For that, we let png_get_uint_31 read 4 bytes from input (chunk_length)
    // Now, call the function
    // Note: After reading chunk header, idat_size = push_length = 4
    // Then due to idat_size != 0 and save_buffer_size != 0, first branch executes
    // It will process 4 bytes from save_buffer
    png_push_read_IDAT(ptr);

    // Assertions
    EXPECT_TRUE(!ptr->error, "No error expected on IDAT full-process path");
    EXPECT_TRUE((ptr->mode & PNG_HAVE_CHUNK_HEADER) == 0, "Have header flag should be cleared after IDAT");
    EXPECT_TRUE((ptr->mode & PNG_AFTER_IDAT) != 0, "Mode should be set to PNG_AFTER_IDAT after IDAT completes");
    EXPECT_EQ(ptr->idat_size, 0, "idat_size should be zero after full processing");
    EXPECT_EQ(ptr->save_buffer_size, 4, "save_buffer_size should be reduced by processed length (4 bytes)");
    EXPECT_EQ(ptr->save_buffer_ptr - save_buf, 4u, "save_buffer_ptr should advance by 4 bytes");
    EXPECT_EQ(ptr->processed_bytes, 4u, "Processed bytes should be 4 after processing IDAT data");
}

// Test 4: IDAT partial processing when idat_size > save_buffer_size
static void test_IDAT_partialProcess()
{
    // Header: length = 6, tag = IDAT
    png_byte input_bytes[8] = {
        0x00, 0x00, 0x00, 0x06, // chunk_length = 6
        'I', 'D', 'A', 'T'        // chunk_tag = 'IDAT'
    };

    unsigned char save_buf[4] = { 0x11,0x22,0x33,0x44 };
    unsigned char current_buf[4] = { 0 };

    png_struct png;
    std::memset(&png, 0, sizeof(png));
    png_structrp ptr = &png;

    ptr->mode = 0;
    ptr->flags = 0;
    ptr->input = input_bytes;
    ptr->input_len = sizeof(input_bytes);
    ptr->input_pos = 0;

    // Set up save buffer smaller than idat_size
    ptr->save_buffer_size = 4;
    ptr->save_buffer_ptr = save_buf;
    ptr->current_buffer_size = 0;
    ptr->current_buffer_ptr = nullptr;
    ptr->buffer_size = 100;
    ptr->idat_size = 0; // will be set by function
    ptr->error = false;
    ptr->error_msg = nullptr;

    // Call
    png_push_read_IDAT(ptr);

    // Assertions
    EXPECT_TRUE(!ptr->error, "No error expected in partial IDAT processing");
    // idat_size should be reduced from 6 by 4 (the min of idat_size and save_buffer_size)
    EXPECT_EQ(ptr->idat_size, 2u, "idat_size should be reduced to 2 after partial processing");
    EXPECT_EQ(ptr->save_buffer_size, 0u, "save_buffer_size should be exhausted (0)");
    EXPECT_EQ(ptr->save_buffer_ptr - save_buf, 4u, "save_buffer_ptr should advance by 4 bytes");
    EXPECT_EQ(ptr->processed_bytes, 4u, "Processed bytes should be 4 after partial IDAT data processing");
    // Mode should still have the header bit set (not finished yet)
    EXPECT_TRUE((ptr->mode & PNG_HAVE_CHUNK_HEADER) != 0, "PNG_HAVE_CHUNK_HEADER should still be set after partial processing");
}

int main()
{
    test_NotEnoughCompressedData();
    test_HeaderEnded();
    test_IDAT_fullProcess();
    test_IDAT_partialProcess();

    print_summary();
    return (g_tests_failed == 0) ? 0 : 1;
}
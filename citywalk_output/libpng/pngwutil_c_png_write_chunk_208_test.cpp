// Generated C++11 unit tests for png_write_chunk in pngwutil.c
// This test suite uses libpng to exercise the focal method via a real write path.
// It avoids GTest and uses a lightweight, non-terminating assertion framework.

#include <cstdint>
#include <vector>
#include <setjmp.h>
#include <iostream>
#include <pngpriv.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <png.h>



// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_START   do { ++g_total_tests; } while (0)
#define TEST_ASSERT(cond, msg)                                            \
    do { if (!(cond)) {                                         \
            std::cerr << "Test failed: " << (msg) << std::endl;  \
            ++g_failed_tests;                                    \
        } else {                                                  \
            /* pass, do nothing */                               \
        }                                                         \
    } while (0)

// Simple CRC32 implementation (IEEE 802.3) for validating last CRC field.
// This matches the common CRC used by libpng for chunk CRCs.
class Crc32 {
public:
    static uint32_t compute(const unsigned char* data, size_t len) {
        ensure_table();
        uint32_t crc = 0xFFFFFFFFu;
        for (size_t i = 0; i < len; ++i) {
            unsigned char byte = data[i];
            crc = table[(crc ^ byte) & 0xFF] ^ (crc >> 8);
        }
        return crc ^ 0xFFFFFFFFu;
    }

private:
    static uint32_t table[256];
    static bool initialized;

    static void ensure_table() {
        if (initialized) return;
        for (uint32_t i = 0; i < 256; ++i) {
            uint32_t c = i;
            for (int j = 0; j < 8; ++j) {
                if (c & 1) c = 0xEDB88320u ^ (c >> 1);
                else       c = c >> 1;
            }
            table[i] = c;
        }
        initialized = true;
    }
};

uint32_t Crc32::table[256];
bool Crc32::initialized = false;

// Memory-backed writer to capture libpng output
struct MemoryBuffer {
    std::vector<unsigned char> data;

    // libpng call signature: void write_fn(png_structp, png_bytep, png_size_t)
    static void write_data(png_structp png_ptr, png_bytep in_data, png_size_t length) {
        MemoryBuffer* mb = static_cast<MemoryBuffer*>(png_get_io_ptr(png_ptr));
        if (mb == nullptr) return;
        mb->data.insert(mb->data.end(), in_data, in_data + length);
    }

    // Optional flush callback (not used here)
    static void flush(png_structp, png_voidp) {
        // no-op
    }
};

// Helper to run a single test and print a summary
static bool run_all_tests();

// Test 1: Write a non-empty tEXt chunk and verify header, data, and CRC
static bool test_png_write_chunk_with_data() {
    TEST_START;
    // Prepare chunk data
    const char chunk_str[4] = { 't', 'E', 'X', 't' }; // 4-char chunk name
    const unsigned char text_data[] = "Hello";

    // Libpng structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    TEST_ASSERT(png_ptr != NULL, "png_create_write_struct failed");

    png_infop info_ptr = png_create_info_struct(png_ptr);
    TEST_ASSERT(info_ptr != NULL, "png_create_info_struct failed");

    // Set up non-terminating error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        TEST_ASSERT(false, "libpng longjmp during test_png_write_chunk_with_data");
        return false;
    }

    // Memory writer
    MemoryBuffer writer;
    // Install the memory writer; pass pointer to our MemoryBuffer instance
    png_set_write_fn(png_ptr, &writer, MemoryBuffer::write_data, MemoryBuffer::flush);

    // Optionally write the signature (not strictly necessary for chunk write but safe)
    png_write_sig(png_ptr);

    // Call the focal method (public API in libpng)
    png_const_bytep chunk_string = reinterpret_cast<const png_bytep>(chunk_str);
    const png_bytep data_ptr = text_data;
    size_t length = sizeof(text_data) - 1; // "Hello" without null terminator

    png_write_chunk(png_ptr, chunk_string, data_ptr, length);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);

    // Validate buffer content
    const std::vector<unsigned char>& buf = writer.data;
    // Expected: 4 bytes length + 4 bytes chunk type + 5 bytes data + 4 bytes CRC = 17 bytes
    const size_t expected_len = 4 + 4 + length + 4;
    TEST_ASSERT(buf.size() == expected_len, "Buffer length mismatch for data chunk");

    // 1) Length field (big-endian)
    if (buf.size() >= 4) {
        uint32_t be_len = (static_cast<uint32_t>(buf[0]) << 24) |
                          (static_cast<uint32_t>(buf[1]) << 16) |
                          (static_cast<uint32_t>(buf[2]) << 8)  |
                          (static_cast<uint32_t>(buf[3]));
        TEST_ASSERT(be_len == length, "Chunk length field mismatch");
    }

    // 2) Chunk type
    if (buf.size() >= 8) {
        TEST_ASSERT(buf[4] == static_cast<unsigned char>('t') &&
                    buf[5] == static_cast<unsigned char>('E') &&
                    buf[6] == static_cast<unsigned char>('X') &&
                    buf[7] == static_cast<unsigned char>('t'),
                    "Chunk type bytes mismatch");
    }

    // 3) Chunk data
    if (buf.size() >= 13) { // 4 + 4 + 4? Actually 8 + length -> 13 if length==5
        TEST_ASSERT(std::memcmp(&buf[8], text_data, length) == 0, "Chunk data bytes mismatch");
    }

    // 4) CRC (last 4 bytes) - verify CRC over type + data
    if (buf.size() >= 17) {
        // Prepare input for CRC: type + data
        unsigned char crc_input[4 + 5];
        std::memcpy(crc_input, &buf[4], 4);
        std::memcpy(crc_input + 4, text_data, length);
        uint32_t expected_crc = Crc32::compute(crc_input, 4 + length);

        // Convert to big-endian 4 bytes
        unsigned char crc_bytes[4];
        crc_bytes[0] = (expected_crc >> 24) & 0xFF;
        crc_bytes[1] = (expected_crc >> 16) & 0xFF;
        crc_bytes[2] = (expected_crc >> 8) & 0xFF;
        crc_bytes[3] = (expected_crc) & 0xFF;

        TEST_ASSERT(std::memcmp(&buf[13], crc_bytes, 4) == 0, "CRC bytes mismatch");
    }

    // Accumulate result
    return g_failed_tests == 0;
}

// Test 2: Write a zero-length chunk to ensure no data path is mishandled
static bool test_png_write_chunk_with_no_data() {
    TEST_START;
    // Prepare chunk data
    const char chunk_str[4] = { 't', 'E', 'X', 't' }; // 4-char chunk name
    const unsigned char* data_ptr = NULL;
    size_t length = 0;

    // Libpng structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    TEST_ASSERT(png_ptr != NULL, "png_create_write_struct failed (no data test)");

    png_infop info_ptr = png_create_info_struct(png_ptr);
    TEST_ASSERT(info_ptr != NULL, "png_create_info_struct failed (no data test)");

    // Error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        TEST_ASSERT(false, "libpng longjmp during test_png_write_chunk_with_no_data");
        return false;
    }

    // Memory writer
    MemoryBuffer writer;
    png_set_write_fn(png_ptr, &writer, MemoryBuffer::write_data, MemoryBuffer::flush);

    // Write signature
    png_write_sig(png_ptr);

    // Call focal method with zero-length data
    const png_bytep chunk_string = reinterpret_cast<const png_bytep>(chunk_str);
    png_write_chunk(png_ptr, chunk_string, data_ptr, length);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);

    // Validate buffer content
    const std::vector<unsigned char>& buf = writer.data;
    // Expected: 4 + 4 + 0 + 4 = 12 bytes
    const size_t expected_len = 4 + 4 + 0 + 4;
    TEST_ASSERT(buf.size() == expected_len, "Buffer length mismatch for zero-length data chunk");

    // Length field should be 0
    if (buf.size() >= 4) {
        uint32_t be_len = (static_cast<uint32_t>(buf[0]) << 24) |
                          (static_cast<uint32_t>(buf[1]) << 16) |
                          (static_cast<uint32_t>(buf[2]) << 8)  |
                          (static_cast<uint32_t>(buf[3]));
        TEST_ASSERT(be_len == 0, "Chunk length field should be 0 for zero-length data");
    }

    // Chunk type should be tEXt
    if (buf.size() >= 8) {
        TEST_ASSERT(buf[4] == static_cast<unsigned char>('t') &&
                    buf[5] == static_cast<unsigned char>('E') &&
                    buf[6] == static_cast<unsigned char>('X') &&
                    buf[7] == static_cast<unsigned char>('t'),
                    "Chunk type bytes mismatch for zero-length data");
    }

    // CRC should be computed over only the chunk type
    if (buf.size() >= 12) {
        unsigned char crc_input[4];
        std::memcpy(crc_input, &buf[4], 4);
        uint32_t expected_crc = Crc32::compute(crc_input, 4);

        unsigned char crc_bytes[4];
        crc_bytes[0] = (expected_crc >> 24) & 0xFF;
        crc_bytes[1] = (expected_crc >> 16) & 0xFF;
        crc_bytes[2] = (expected_crc >> 8) & 0xFF;
        crc_bytes[3] = (expected_crc) & 0xFF;

        TEST_ASSERT(std::memcmp(&buf[8], crc_bytes, 4) == 0, "CRC bytes mismatch for zero-length data");
    }

    return g_failed_tests == 0;
}

// Entry point to run tests
static bool run_all_tests() {
    bool ok = true;
    ok &= test_png_write_chunk_with_data();
    ok &= test_png_write_chunk_with_no_data();
    return ok && (g_failed_tests == 0);
}

int main() {
    bool all_ok = run_all_tests();

    std::cout << "Total tests run: " << g_total_tests << std::endl;
    std::cout << "Tests failed: " << g_failed_tests << std::endl;

    if (all_ok) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << "Some tests failed." << std::endl;
        return 1;
    }
}
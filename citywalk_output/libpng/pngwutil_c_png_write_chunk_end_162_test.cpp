#include <cstdint>
#include <vector>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Minimal mockery of the libpng types and functions required by the focal method.
// This test harness re-implements just enough to exercise png_write_chunk_end.

typedef unsigned char png_byte;
typedef unsigned int  png_uint_32;
typedef png_byte*       png_bytep;
typedef png_byte*       png_const_bytep;

// Forward declaration of the focal struct used by png_write_chunk_end.
struct png_struct_def {
    unsigned int io_state;
    unsigned int crc;
};

// In the real library, png_structrp is a pointer to the png_struct_def.
// We mirror that here for compatibility with the focal method signature.
typedef struct png_struct_def* png_structrp;

// Core constants used by the I/O state logic (when enabled in real libpng).
static const unsigned int PNG_IO_WRITING    = 0x01;
static const unsigned int PNG_IO_CHUNK_CRC  = 0x02;

// Global buffer to capture data written by the mocked png_write_data.
static std::vector<png_byte> g_written;

// Mocked implementation of the helper that writes a 32-bit unsigned int to a 4-byte buffer.
// This mirrors the big-endian behavior of png_save_uint_32 in libpng.
void png_save_uint_32(png_bytep buf, png_uint_32 i) {
    buf[0] = static_cast<png_byte>((i >> 24) & 0xFF);
    buf[1] = static_cast<png_byte>((i >> 16) & 0xFF);
    buf[2] = static_cast<png_byte>((i >> 8)  & 0xFF);
    buf[3] = static_cast<png_byte>( i        & 0xFF);
}

// Mocked implementation of the data writing function used by png_write_chunk_end.
// It appends the data to a global buffer so tests can verify the exact bytes written.
void png_write_data(png_structrp /*png_ptr*/, const png_byte* data, size_t length) {
    g_written.insert(g_written.end(), data, data + length);
}

// The focal method under test (re-implemented here for unit testing in isolation).
// It follows the exact logic shown in the prompt.
void png_write_chunk_end(png_structrp png_ptr)
{
{
   png_byte buf[4];
   if (png_ptr == NULL) return;
#ifdef PNG_IO_STATE_SUPPORTED
   /* Inform the I/O callback that the chunk CRC is being written.
    * PNG_IO_CHUNK_CRC requires a single I/O function call.
    */
   png_ptr->io_state = PNG_IO_WRITING | PNG_IO_CHUNK_CRC;
#endif
   /* Write the crc in a single operation */
   png_save_uint_32(buf, png_ptr->crc);
   png_write_data(png_ptr, buf, 4);
}
}

// Simple test harness (no external test framework) to validate behavior.
// This test suite focuses on the runtime predicate: png_ptr == NULL and the
// correctness of the CRC being written to output (big-endian 4-byte sequence).

// Assertion helper for readability and consistent error messaging.
static void assert_true(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "ASSERTION FAILED: " << message << std::endl;
        throw std::runtime_error(message);
    }
}

// Test 1: When png_ptr is NULL, the function should return immediately without writing anything.
void test_png_write_chunk_end_null_ptr_noop() {
    // Arrange
    g_written.clear();
    // Act
    png_write_chunk_end(nullptr);
    // Assert
    assert_true(g_written.empty(), "png_write_chunk_end should not write any data when png_ptr is NULL.");
}

// Test 2: When png_ptr is non-NULL, the function should write exactly 4 bytes equal to png_ptr->crc
// using big-endian representation.
void test_png_write_chunk_end_writes_crc_correctly() {
    // Arrange
    g_written.clear();
    png_struct_def s;
    s.io_state = 0;     // Initial IO state; not observed in test since PNG_IO_STATE_SUPPORTED is not defined here.
    s.crc = 0x01020304; // Known CRC value to validate endianness and bytes written.

    // Act
    png_write_chunk_end(&s);

    // Assert: exactly 4 bytes were written
    assert_true(g_written.size() == 4, "png_write_chunk_end should write exactly 4 bytes for the CRC.");

    // Assert: bytes correspond to 0x01, 0x02, 0x03, 0x04 (big-endian)
    unsigned char expected[4] = { 0x01, 0x02, 0x03, 0x04 };
    for (size_t i = 0; i < 4; ++i) {
        std::ostringstream oss;
        oss << "Byte " << i << " should be " << static_cast<int>(expected[i])
            << " but was " << static_cast<int>(g_written[i]);
        assert_true(g_written[i] == expected[i], oss.str().c_str());
    }
}

// Entry point to run the tests.
int main() {
    try {
        std::cout << "Running tests for png_write_chunk_end (minimal libpng-like environment)..." << std::endl;

        // Run tests that cover the key predicates described in Step 3.
        test_png_write_chunk_end_null_ptr_noop();
        std::cout << "[PASS] test_png_write_chunk_end_null_ptr_noop" << std::endl;

        test_png_write_chunk_end_writes_crc_correctly();
        std::cout << "[PASS] test_png_write_chunk_end_writes_crc_correctly" << std::endl;

        std::cout << "All tests passed." << std::endl;
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "TEST FAILURE: " << ex.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "TEST FAILURE: Unknown exception." << std::endl;
        return 1;
    }
}
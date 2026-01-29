// Automated unit tests for the focal PNG push-read signature function
// This test suite is designed to exercise png_push_read_sig in a self-contained manner
// by providing minimal fake PNG types and a mocked PNG I/O surface.
// Note: This test is written with C++11 and does not rely on GTest.
// It uses exceptions to model fatal errors from png_error(...) to enable precise assertions.

#include <vector>
#include <iostream>
#include <pngpriv.h>
#include <stdexcept>
#include <cstring>


// Faked minimal PNG-like interface to mirror the dependencies of png_push_read_sig.
// These definitions are intentionally small and self-contained for unit testing.

typedef unsigned char png_byte;
typedef png_byte* png_bytep;
typedef struct png_struct_def* png_structrp;
typedef struct png_info_def* png_inforp;

struct png_struct_def {
    png_byte sig_bytes;       // number of bytes of the signature already checked/read
    size_t buffer_size;       // available bytes in the input buffer
    int process_mode;           // processing mode (e.g., PNG_READ_CHUNK_MODE)
};

struct png_info_def {
    png_byte signature[8];      // signature buffer where bytes are accumulated
};

#define PNG_READ_CHUNK_MODE 1

// Forward declarations of functions used by png_push_read_sig.
// They are provided by our test harness (fake implementations here) to enable
// standalone unit testing without depending on the actual libpng.
extern "C" void png_push_fill_buffer(png_structrp png_ptr, png_bytep buffer, size_t length);
extern "C" int  png_sig_cmp(png_bytep signature, size_t start, size_t num_to_check);
extern "C" void png_error(png_structrp png_ptr, const char *error);
extern "C" void png_push_read_sig(png_structrp png_ptr, png_inforp info_ptr);

// Global test input stream that mimics the data source PNG would read from.
static std::vector<unsigned char> g_test_input_stream;
static size_t g_test_input_pos = 0;

// Helpers to manage test input
static void reset_input_stream() {
    g_test_input_stream.clear();
    g_test_input_pos = 0;
}
static void set_input_stream(const std::vector<unsigned char>& data) {
    g_test_input_stream = data;
    g_test_input_pos = 0;
}

// Fake implementations of the PNG helper routines used by png_push_read_sig

// Fill 'buffer' with next 'length' bytes from the fake input stream.
// If fewer bytes are available than requested, fill what remains and pad with zeros.
// Update the png_ptr->buffer_size to reflect remaining bytes.
extern "C" void png_push_fill_buffer(png_structrp png_ptr, png_bytep buffer, size_t length) {
    size_t remaining = (g_test_input_stream.size() > g_test_input_pos)
                           ? (g_test_input_stream.size() - g_test_input_pos)
                           : 0;
    size_t to_copy = length;
    if (to_copy > remaining) to_copy = remaining;

    for (size_t i = 0; i < to_copy; ++i) {
        buffer[i] = g_test_input_stream[g_test_input_pos + i];
    }
    // Advance the input stream position
    g_test_input_pos += to_copy;

    // If we couldn't fill the requested length fully, pad the rest with zeros
    for (size_t i = to_copy; i < length; ++i) {
        buffer[i] = 0;
    }

    // Update the simulated buffer_size to reflect the remaining bytes after this fill
    size_t new_remaining = (g_test_input_stream.size() > g_test_input_pos)
                           ? (g_test_input_stream.size() - g_test_input_pos)
                           : 0;
    png_ptr->buffer_size = new_remaining;
}

// Simple signature comparison helper mimicking the behavior used by png_push_read_sig.
// Returns 0 if the specified portion matches the PNG signature, non-zero otherwise.
// If num_to_check == 0, return non-zero to simplify triggering the ASCII-conversion path in tests.
extern "C" int png_sig_cmp(png_bytep signature, size_t start, size_t num_to_check) {
    static const unsigned char PNG_SIGNATURE[8] = {
        0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A
    };
    if (start >= 8) return 1;
    if (start + num_to_check > 8) {
        // Clamp to signature boundary
        num_to_check = (8 - start);
    }
    for (size_t i = 0; i < num_to_check; ++i) {
        if (signature[start + i] != PNG_SIGNATURE[start + i]) return 1;
    }
    // Special-case: zero-length checks should be treated as non-match to exercise specific branches
    if (num_to_check == 0) return 1;
    return 0;
}

// Fake error handler that uses C++ exceptions to emulate fatal errors.
// Each call to png_error will throw a runtime_error with the provided message.
extern "C" void png_error(png_structrp png_ptr, const char *error) {
    (void)png_ptr; // silence unused parameter in tests that don't inspect it
    throw std::runtime_error(error ? error : "png_error");
}

// Declare the focal function with C linkage so tests can call it directly.
// The actual implementation exists in the provided pngpread.c during build/link.
// We declare it here to allow test code to reference it.
extern "C" void png_push_read_sig(png_structrp png_ptr, png_inforp info_ptr);

// ------------------------------------------------------------------------------------------------
// Test harness for png_push_read_sig
// ------------------------------------------------------------------------------------------------

// Helper for test assertions without a full testing framework
static void assert_true(bool cond, const char* msg) {
    if (!cond) throw std::runtime_error(msg ? msg : "Assertion failed");
}

// Typedefs for local test objects matching the fake API
static struct png_struct_def g_test_ptr;
static struct png_info_def   g_test_info;

// Utility to check memory equality of 8-byte signature buffers
static bool mem8_equal(const unsigned char a[8], const unsigned char b[8]) {
    return std::memcmp(a, b, 8) == 0;
}

// Test Case 1: Valid PNG signature provided in a single read; process_mode should be set
static void test_valid_signature_sets_mode() {
    reset_input_stream();
    // The 8-byte PNG signature
    unsigned char sig8[8] = {0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A};
    set_input_stream(std::vector<unsigned char>(sig8, sig8 + 8));

    // Initialize fake PNG objects
    g_test_ptr.sig_bytes = 0;
    g_test_ptr.buffer_size = 8;
    g_test_ptr.process_mode = 0;

    std::memset(g_test_info.signature, 0, 8);

    // Call the focal function
    try {
        png_push_read_sig(&g_test_ptr, &g_test_info);
    } catch (const std::exception& ex) {
        throw; // bubble up as test failure
    }

    // Assertions
    assert_true(g_test_ptr.sig_bytes == 8, "sig_bytes should be 8 after full signature read");
    assert_true(g_test_ptr.buffer_size == 0, "buffer_size should be 0 after full read");
    assert_true(g_test_ptr.process_mode == PNG_READ_CHUNK_MODE, "process_mode should be set to PNG_READ_CHUNK_MODE after complete signature");
    assert_true(mem8_equal(g_test_info.signature, sig8), "Signature bytes should match the PNG signature after read");
}

// Test Case 2: Partial reads across two calls; ensure proper accumulation and no errors yet
static void test_partial_then_complete_signature() {
    reset_input_stream();
    // First partial input: 3 bytes of signature
    unsigned char first[3] = {0x89, 'P', 'N'};
    set_input_stream(std::vector<unsigned char>(first, first + 3));

    g_test_ptr.sig_bytes = 0;
    g_test_ptr.buffer_size = 3;
    g_test_ptr.process_mode = 0;
    std::memset(g_test_info.signature, 0, 8);

    // First call (partial)
    try {
        png_push_read_sig(&g_test_ptr, &g_test_info);
    } catch (...) {
        throw; // should not error here
    }
    // Validate partial state
    assert_true(g_test_ptr.sig_bytes == 3, "After first partial read, sig_bytes should be 3");
    // The first three bytes should be filled
    unsigned char partial_expected[8] = {0x89, 'P', 'N', 0, 0, 0, 0, 0};
    if (g_test_info.signature[0] != partial_expected[0] ||
        g_test_info.signature[1] != partial_expected[1] ||
        g_test_info.signature[2] != partial_expected[2]) {
        throw std::runtime_error("First 3 signature bytes not stored correctly after partial read");
    }

    // Second partial: remaining 5 bytes to complete signature
    unsigned char rest[5] = { 'G', 0x0D, 0x0A, 0x1A, 0x0A };
    set_input_stream(std::vector<unsigned char>(rest, rest + 5));

    g_test_ptr.buffer_size = 5;
    // Second call (complete)
    try {
        png_push_read_sig(&g_test_ptr, &g_test_info);
    } catch (...) {
        throw;
    }

    // Assertions after complete
    assert_true(g_test_ptr.sig_bytes == 8, "sig_bytes should be 8 after completing the signature");
    assert_true(g_test_ptr.process_mode == PNG_READ_CHUNK_MODE, "process_mode should be set after complete signature");
    unsigned char expected[8] = {0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A};
    if (!mem8_equal(g_test_info.signature, expected)) {
        throw std::runtime_error("Signature bytes do not match expected PNG signature after completion");
    }
}

// Test Case 3: Not a PNG path using first-byte mismatch triggers "Not a PNG file"
static void test_not_png_not_a_png_path() {
    reset_input_stream();
    // 4 bytes that do not match the PNG signature
    unsigned char bad4[4] = {0x00, 0x00, 0x00, 0x01};
    set_input_stream(std::vector<unsigned char>(bad4, bad4 + 4));

    g_test_ptr.sig_bytes = 0;
    g_test_ptr.buffer_size = 4;
    g_test_ptr.process_mode = 0;
    std::memset(g_test_info.signature, 0, 8);

    // Expect an exception with "Not a PNG file"
    try {
        png_push_read_sig(&g_test_ptr, &g_test_info);
        throw std::runtime_error("Expected png_error to throw Not a PNG file, but it did not");
    } catch (const std::runtime_error& ex) {
        std::string msg = ex.what();
        if (msg.find("Not a PNG file") == std::string::npos) {
            throw std::runtime_error("png_error did not propagate expected Not a PNG file message");
        }
        // success
    }
}

// Test Case 4: ASCII conversion error path (signature matches first 4 bytes, rest differ)
static void test_ascii_conversion_error_path() {
    reset_input_stream();
    // 4 bytes match PNG signature, next 4 do not
    unsigned char mixed[8] = {0x89, 'P', 'N', 'G', 0x00, 0x00, 0x00, 0x00};
    set_input_stream(std::vector<unsigned char>(mixed, mixed + 8));

    g_test_ptr.sig_bytes = 0;
    g_test_ptr.buffer_size = 8;
    g_test_ptr.process_mode = 0;
    std::memset(g_test_info.signature, 0, 8);

    // Expect an exception with "PNG file corrupted by ASCII conversion"
    try {
        png_push_read_sig(&g_test_ptr, &g_test_info);
        throw std::runtime_error("Expected ascii-conversion error, but no exception was thrown");
    } catch (const std::runtime_error& ex) {
        std::string msg = ex.what();
        if (msg.find("PNG file corrupted by ASCII conversion") == std::string::npos) {
            throw std::runtime_error("png_error did not propagate expected ASCII-conversion message");
        }
        // success
    }
}

// Test Case 5: Buffer size smaller than needed (partial read with limited buffer)
static void test_buffer_size_limited_read() {
    reset_input_stream();
    // 8-byte signature, but input buffer reports only 2 bytes available
    unsigned char sig8[8] = {0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A};
    set_input_stream(std::vector<unsigned char>(sig8, sig8 + 2)); // only first 2 bytes available now

    g_test_ptr.sig_bytes = 0;
    g_test_ptr.buffer_size = 2; // limited buffer
    g_test_ptr.process_mode = 0;
    std::memset(g_test_info.signature, 0, 8);

    // First call should read only 2 bytes
    try {
        png_push_read_sig(&g_test_ptr, &g_test_info);
    } catch (...) {
        throw;
    }

    // After first call, 2 bytes should be filled
    assert_true(g_test_ptr.sig_bytes == 2, "sig_bytes should be 2 after limited read");
    assert_true(g_test_ptr.buffer_size == 6, "buffer_size should reflect remaining 6 bytes");

    // The first two bytes should match the signature's first two bytes
    if (g_test_info.signature[0] != 0x89 || g_test_info.signature[1] != 'P') {
        throw std::runtime_error("First two signature bytes not stored correctly in limited read");
    }
}

// Runner that executes all tests and reports results
int main() {
    int passed = 0;
    int failed = 0;

    auto run = [&](const char* name, void(*fn)()) {
        try {
            fn();
            std::cout << "[PASS] " << name << "\n";
            ++passed;
        } catch (const std::exception& ex) {
            ++failed;
            std::cerr << "[FAIL] " << name << " - " << ex.what() << "\n";
        } catch (...) {
            ++failed;
            std::cerr << "[FAIL] " << name << " - unknown exception\n";
        }
    };

    // Execute all tests with descriptive comments in code
    run("test_valid_signature_sets_mode", test_valid_signature_sets_mode);
    run("test_partial_then_complete_signature", test_partial_then_complete_signature);
    run("test_not_png_not_a_png_path", test_not_png_not_a_png_path);
    run("test_ascii_conversion_error_path", test_ascii_conversion_error_path);
    run("test_buffer_size_limited_read", test_buffer_size_limited_read);

    std::cout << "\nSummary: " << passed << " passed, " << failed << " failed.\n";
    return (failed == 0) ? 0 : 1;
}

// End of unit tests for png_push_read_sig
// The tests rely on a minimal fake PNG API implemented above to enable deterministic, self-contained verification
// of both true/false branches and error-paths without depending on the full libpng codebase.
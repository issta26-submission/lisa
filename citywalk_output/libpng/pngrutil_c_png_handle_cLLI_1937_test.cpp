// Minimal, self-contained unit test suite for the focal method png_handle_cLLI
// This test re-implements just enough of the PNG handling environment to exercise the logic
// of png_handle_cLLI without requiring the real libpng sources or libraries.
// The focus is on the two branches: successful handling and error when CRC finish != 0.

#include <cstdint>
#include <cstdlib>
#include <pngpriv.h>
#include <iostream>
#include <cstdio>
#include <cstring>


namespace PNGTest { // Encapsulate mock types and functions to resemble libpng style

    // Forward-declare the dependent types used by the focal method
    struct png_struct_def;
    struct png_info_def;

    typedef png_struct_def* png_structrp;
    typedef png_info_def* png_inforp;
    typedef uint32_t png_uint_32;
    typedef unsigned char png_byte;
    typedef unsigned char* png_bytep;
    typedef const unsigned char* png_const_bytep;

    // Return codes used by the focal method
    // assumed by the original code: handled_ok on success, handled_error on failure
    static const int handled_ok = 1;
    static const int handled_error = 0;

    // Minimal internal object shapes (only what we touch in tests)
    struct png_struct_def {
        int crc_finish_ret;        // value to return from png_crc_finish()
        unsigned char crc_input_buf[8]; // 8-byte buffer that png_crc_read will fill
        int crc_read_pos;            // not strictly needed, but helpful for clarity
        uint32_t cLLI_A;              // captured first 4 bytes (big-endian)
        uint32_t cLLI_B;              // captured second 4 bytes (big-endian)
    };

    struct png_info_def {
        // Empty for test; in real libpng this holds image metadata
    };

    // Stubs for dependencies used by the focal function

    // Silently log debug messages (no-op in tests)
    void png_debug(int /*level*/, const char* /*msg*/) { }

    // Simulate reading 8 bytes from the PNG data stream into buf
    void png_crc_read(png_structrp png_ptr, png_bytep buf, png_uint_32 length) {
        (void)length; // length is always 8 in the focal function
        for (int i = 0; i < 8; ++i)
            buf[i] = png_ptr->crc_input_buf[i];
        png_ptr->crc_read_pos = 8;
    }

    // Simulate final CRC check; return value dictates success/failure path
    int png_crc_finish(png_structrp png_ptr, png_uint_32 /*skip*/) {
        return png_ptr->crc_finish_ret;
    }

    // Extract a 32-bit unsigned integer from 4 bytes (big-endian)
    png_uint_32 png_get_uint_32(png_const_bytep buf) {
        return (static_cast<png_uint_32>(buf[0]) << 24) |
               (static_cast<png_uint_32>(buf[1]) << 16) |
               (static_cast<png_uint_32>(buf[2]) << 8)  |
               (static_cast<png_uint_32>(buf[3]));
    }

    // Record the cLLI values into the png_ptr (simulating effect of libpng setter)
    void png_set_cLLI_fixed(png_structrp png_ptr, png_inforp /*info_ptr*/,
                            png_uint_32 a, png_uint_32 b) {
        png_ptr->cLLI_A = a;
        png_ptr->cLLI_B = b;
        (void)info_ptr; // unused in test
    }

    // The focal method under test (adapted to the testing environment)
    int png_handle_cLLI(png_structrp png_ptr, png_inforp info_ptr, png_uint_32 length) {
        (void)length; // length is unused in the function body (besides PNG_UNUSED)
        png_byte buf[8];
        png_debug(1, "in png_handle_cLLI");
        png_crc_read(png_ptr, buf, 8);
        if (png_crc_finish(png_ptr, 0) != 0)
            return handled_error;

        // Parse the two 32-bit values and set them in the png struct
        png_set_cLLI_fixed(png_ptr, info_ptr,
                          png_get_uint_32(buf),
                          png_get_uint_32(buf + 4));
        return handled_ok;
    }

} // namespace PNGTest


// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_passed_tests = 0;

static void log_pass(const char* test_name) {
    std::cout << "[PASS] " << test_name << "\n";
    ++g_passed_tests;
}

static void log_fail(const char* test_name, const char* reason) {
    std::cerr << "[FAIL] " << test_name << " - " << reason << "\n";
}

static void expect_eq_uint32(const char* test_name, uint32_t a, uint32_t b) {
    ++g_total_tests;
    if (a == b) {
        log_pass(test_name);
    } else {
        char buf[256];
        std::snprintf(buf, sizeof(buf), "expected %u, got %u", static_cast<unsigned>(b), static_cast<unsigned>(a));
        log_fail(test_name, buf);
    }
}

static void expect_int(const char* test_name, int a, int b) {
    ++g_total_tests;
    if (a == b) {
        log_pass(test_name);
    } else {
        char buf[256];
        std::snprintf(buf, sizeof(buf), "expected %d, got %d", b, a);
        log_fail(test_name, buf);
    }
}

// Public API surface used by tests (bring in the mock implementations)
using namespace PNGTest;

int main() {
    // Expose local types for readability
    typedef png_struct_def* png_structrp;
    typedef png_info_def* png_inforp;

    // Test 1: Success path - CRC finish returns 0 and two 32-bit values are parsed correctly
    {
        // Setup: create fake PNG structures and prepare 8-byte payload representing A=1, B=2
        png_structrp png_ptr = new png_struct_def();
        png_inforp info_ptr = new png_info_def();

        png_ptr->crc_finish_ret = 0; // success
        unsigned char payload[8] = { 0, 0, 0, 1,   0, 0, 0, 2 }; // A=1, B=2 in big-endian
        std::memcpy(png_ptr->crc_input_buf, payload, 8);

        int result = png_handle_cLLI(png_ptr, info_ptr, 0);

        // Validate: returns handled_ok, and A/B fields were set correctly
        expect_int("png_handle_cLLI_success_return", result, handled_ok);
        expect_eq_uint32("png_handle_cLLI_success_A", png_ptr->cLLI_A, 1);
        expect_eq_uint32("png_handle_cLLI_success_B", png_ptr->cLLI_B, 2);

        delete png_ptr;
        delete info_ptr;
    }

    // Test 2: Failure path - CRC finish returns non-zero should yield handled_error
    {
        png_structrp png_ptr = new png_struct_def();
        png_inforp info_ptr = new png_info_def();

        png_ptr->crc_finish_ret = 1; // non-zero triggers error
        unsigned char payload[8] = { 0, 0, 0, 3,   0, 0, 0, 4 }; // A=3, B=4 (won't be used due to error)
        std::memcpy(png_ptr->crc_input_buf, payload, 8);

        // Pre-fill A/B to check they are not overwritten on error
        png_ptr->cLLI_A = 0xAAAAAAAA;
        png_ptr->cLLI_B = 0xBBBBBBBB;

        int result = png_handle_cLLI(png_ptr, info_ptr, 0);

        // Validate: returns handled_error and A/B untouched in error path
        expect_int("png_handle_cLLI_failure_return", result, handled_error);
        expect_eq_uint32("png_handle_cLLI_failure_A_unchanged", png_ptr->cLLI_A, 0xAAAAAAAA);
        expect_eq_uint32("png_handle_cLLI_failure_B_unchanged", png_ptr->cLLI_B, 0xBBBBBBBB);

        delete png_ptr;
        delete info_ptr;
    }

    // Report overall results
    std::cout << "Total tests: " << g_total_tests << ", Passed: " << g_passed_tests << "\n";

    // Return non-zero if any test failed to ensure visibility to CI tools if desired
    return (g_total_tests == g_passed_tests) ? 0 : 1;
}
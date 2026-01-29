// Comprehensive C++11 test suite for the focal method png_read_sig
// NOTE: This test harness mocks the minimal subset of the libpng environment
// needed to exercise png_read_sig in isolation (no external GTest required).

#include <cstdint>
#include <functional>
#include <iostream>
#include <pngpriv.h>
#include <stdexcept>
#include <cstring>


// Domain-aligned definitions (mocked minimal subset of libpng types)
typedef unsigned char png_bytep;
typedef const unsigned char* png_const_bytep;

typedef unsigned int png_uint_32;
typedef void* png_structrp;
typedef void* png_inforp;

// Forward declarations of the focal method and its helpers (mocked)
#define PNG_HAVE_PNG_SIGNATURE 0x01
#define PNG_IO_READING        0x01
#define PNG_IO_SIGNATURE      0x02
#define PNG_IO_STATE_SUPPORTED 1

// Toggle the io_state handling as per domain knowledge
#define PNG_IO_STATE_SUPPORTED 1

// Minimal png_struct and png_info suitable for tests
struct mock_png_struct {
    unsigned sig_bytes;
    unsigned mode;
#ifdef PNG_IO_STATE_SUPPORTED
    unsigned io_state;
#endif
};

struct mock_png_info {
    unsigned char signature[8];
};

// Typedefs matching the expected API signatures
typedef mock_png_struct* png_structrp;
typedef mock_png_info* png_inforp;

// Global constants representing the PNG signature
static const unsigned char PNG_SIGNATURE[8] = {
    0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A
};

// --- Mocked I/O and utility state for tests ---

// Input buffer that png_read_data will "read" from (simulated I/O)
static const unsigned char* test_input_buffer = nullptr;
static size_t test_input_size = 0;
static size_t test_input_pos = 0;

// Set the simulated input bytes for the next read
static void test_set_input_bytes(const unsigned char* buf, size_t size) {
    test_input_buffer = buf;
    test_input_size = size;
    test_input_pos = 0;
}

// Simulated png_read_data: copies 'length' bytes from test_input_buffer to dest
static void png_read_data(png_structrp /*png_ptr*/, png_bytep data, size_t length) {
    if (length == 0) return;
    if (test_input_pos >= test_input_size) {
        // No more data; in real libpng this would likely error, but tests can fail explicitly
        throw std::runtime_error("png_read_data: unexpected end of input");
    }
    size_t to_copy = (test_input_pos + length <= test_input_size) ? length : (test_input_size - test_input_pos);
    std::memcpy(data, test_input_buffer + test_input_pos, to_copy);
    test_input_pos += to_copy;
    if (to_copy != length) {
        throw std::runtime_error("png_read_data: insufficient input data for requested length");
    }
}

// Compare signature segment to the canonical PNG signature
static int png_sig_cmp(png_const_bytep signature, png_uint_32 start, png_uint_32 num_to_check) {
    // Compare signature[start .. start+num_to_check-1] with PNG_SIGNATURE
    for (png_uint_32 i = 0; i < num_to_check; ++i) {
        unsigned idx = static_cast<unsigned>(start + i);
        unsigned sig_idx = static_cast<unsigned>(start + i);
        if (signature[sig_idx] != PNG_SIGNATURE[sig_idx]) {
            return 1; // non-zero means not equal
        }
    }
    return 0; // equal
}

// Helper to signal libpng errors: throw an exception so tests can catch and inspect
static void png_error(png_structrp /*png_ptr*/, const char* msg) {
    throw std::runtime_error(msg ? msg : "png_error");
}

// --- Focal method under test (re-implemented here for isolated unit testing) ---

void png_read_sig(png_structrp png_ptr, png_inforp info_ptr) {
    // The implementation mirrors the provided focal method exactly
    size_t num_checked, num_to_check;
    if (png_ptr->sig_bytes >= 8)
        return;
    num_checked = png_ptr->sig_bytes;
    num_to_check = 8 - num_checked;
#ifdef PNG_IO_STATE_SUPPORTED
    png_ptr->io_state = PNG_IO_READING | PNG_IO_SIGNATURE;
#endif
    png_read_data(png_ptr, &(info_ptr->signature[num_checked]), num_to_check);
    png_ptr->sig_bytes = 8;
    if (png_sig_cmp(info_ptr->signature, num_checked, num_to_check) != 0) {
        if (num_checked < 4 &&
            png_sig_cmp(info_ptr->signature, num_checked, num_to_check - 4) != 0)
            png_error(png_ptr, "Not a PNG file");
        else
            png_error(png_ptr, "PNG file corrupted by ASCII conversion");
    }
    if (num_checked < 3)
        png_ptr->mode |= PNG_HAVE_PNG_SIGNATURE;
}

// --- Test Harness Infrastructure ---

static int g_tests_run = 0;
static int g_tests_failed = 0;

#define RUN_TEST(name) do { \
    std::cout << "Running " #name "...\n"; \
    (name)(); \
} while (0)

static void report_test_result(const char* test_name, bool passed, const char* note = nullptr) {
    ++g_tests_run;
    if (passed) {
        std::cout << "PASS: " << test_name << "\n";
    } else {
        ++g_tests_failed;
        std::cout << "FAIL: " << test_name;
        if (note && note[0] != '\\0') {
            std::cout << " - " << note;
        }
        std::cout << "\n";
    }
}

// Convenience macro for small assertions that do not terminate tests
#define ASSERT(cond, test_name, message) do { \
    if (!(cond)) { \
        report_test_result(test_name, false, message); \
        return; \
    } \
} while (0)

// Utility: create a fresh png_ptr / info_ptr for each test
static mock_png_struct g_png_ptr;
static mock_png_info g_info_ptr;

// Candidate Keywords (Step 1): Core dependent components for png_read_sig
// - Signature reading logic (8-byte PNG signature)
// - Signature verification via png_sig_cmp
// - I/O behavior (png_read_data, PNG_IO_STATE... macro)
// - Error handling via png_error
// - Sig_bytes tracking and signature readiness (png_ptr->sig_bytes, PNG_HAVE_PNG_SIGNATURE)
// - io_state manipulation when PNG_IO_STATE_SUPPORTED is defined

// Test 1: No-Op when sig_bytes already >= 8
// Expectation: function returns immediately; info_ptr.signature remains unchanged.
static void test_png_read_sig_no_op_when_already_signed() {
    // Prepare
    g_png_ptr.sig_bytes = 8;
    g_png_ptr.mode = 0;
#ifdef PNG_IO_STATE_SUPPORTED
    g_png_ptr.io_state = 0;
#endif
    // Set a marker in info.signature to ensure no writes happen
    std::memset(g_info_ptr.signature, 0xAA, sizeof(g_info_ptr.signature));

    // Execute
    try {
        png_read_sig(reinterpret_cast<png_structrp>(&g_png_ptr), reinterpret_cast<png_inforp>(&g_info_ptr));
        // Verify
        bool signature_unchanged = true;
        for (int i = 0; i < 8; ++i) {
            if (g_info_ptr.signature[i] != 0xAA) { signature_unchanged = false; break; }
        }
        report_test_result("test_png_read_sig_no_op_when_already_signed",
                           signature_unchanged,
                           signature_unchanged ? nullptr : "signature bytes were modified unexpectedly");
        // Also verify that io_state was not modified unless macro path is compiled (we compile with it defined)
#ifdef PNG_IO_STATE_SUPPORTED
        // In this code path, since we returned early, we would not set io_state
        if (g_png_ptr.io_state != 0) {
            report_test_result("test_png_read_sig_no_op_when_already_signed_io_state",
                                   false, "io_state changed on early return");
            return;
        } else {
            report_test_result("test_png_read_sig_no_op_when_already_signed_io_state",
                               true);
        }
#endif
    } catch (const std::exception& e) {
        report_test_result("test_png_read_sig_no_op_when_already_signed", false, e.what());
    }
}

// Test 2: Not a PNG file path when initial bytes completely mismatch
// Expectation: png_error with "Not a PNG file" is thrown.
static void test_png_read_sig_not_a_png_file() {
    // Prepare: ensure no bytes yet read
    g_png_ptr.sig_bytes = 0;
    g_png_ptr.mode = 0;
#ifdef PNG_IO_STATE_SUPPORTED
    g_png_ptr.io_state = 0;
#endif
    // Input: first 8 bytes not matching PNG_SIGNATURE at all
    unsigned char input8[8] = {0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00};
    test_set_input_bytes(input8, 8);

    // Execute
    bool caught = false;
    try {
        png_read_sig(reinterpret_cast<png_structrp>(&g_png_ptr), reinterpret_cast<png_inforp>(&g_info_ptr));
    } catch (const std::exception& e) {
        caught = true;
        // Expect specific message
        if (std::strcmp(e.what(), "Not a PNG file") == 0) {
            report_test_result("test_png_read_sig_not_a_png_file", true);
        } else {
            report_test_result("test_png_read_sig_not_a_png_file", false, e.what());
        }
    }
    if (!caught) {
        report_test_result("test_png_read_sig_not_a_png_file", false, "expected Not a PNG file exception not thrown");
    }
}

// Test 3: PNG ASCII conversion path (first 4 bytes match, last 4 mismatch)
// Expectation: png_error with "PNG file corrupted by ASCII conversion" is thrown.
static void test_png_read_sig_ascii_conversion_error() {
    // Prepare
    g_png_ptr.sig_bytes = 0;
    g_png_ptr.mode = 0;
#ifdef PNG_IO_STATE_SUPPORTED
    g_png_ptr.io_state = 0;
#endif
    // Build input: first 8 bytes with first 4 bytes equal to PNG_SIGNATURE and last 4 different
    unsigned char input8[8];
    std::memcpy(input8, PNG_SIGNATURE, 4); // first 4 match
    input8[4] = 0x00;
    input8[5] = 0x00;
    input8[6] = 0x00;
    input8[7] = 0x00; // last 4 do not match
    test_set_input_bytes(input8, 8);

    bool caught = false;
    try {
        png_read_sig(reinterpret_cast<png_structrp>(&g_png_ptr), reinterpret_cast<png_inforp>(&g_info_ptr));
    } catch (const std::exception& e) {
        caught = true;
        if (std::strcmp(e.what(), "PNG file corrupted by ASCII conversion") == 0) {
            report_test_result("test_png_read_sig_ascii_conversion_error", true);
        } else {
            report_test_result("test_png_read_sig_ascii_conversion_error", false, e.what());
        }
    }
    if (!caught) {
        report_test_result("test_png_read_sig_ascii_conversion_error", false, "expected ASCII conversion error not thrown");
    }
}

// Test 4: Valid signature leads to signature recognized and mode bit set
// Expectation: No error; if initial num_checked < 3, PNG_HAVE_PNG_SIGNATURE is set in mode
static void test_png_read_sig_valid_signature_sets_mode() {
    // Prepare
    g_png_ptr.sig_bytes = 0;
    g_png_ptr.mode = 0;
#ifdef PNG_IO_STATE_SUPPORTED
    g_png_ptr.io_state = 0;
#endif
    // Input: exactly the PNG signature
    test_set_input_bytes(const_cast<unsigned char*>(PNG_SIGNATURE), 8);

    try {
        png_read_sig(reinterpret_cast<png_structrp>(&g_png_ptr), reinterpret_cast<png_inforp>(&g_info_ptr));
        // Verify no error and mode flag is set
        bool no_error = (g_png_ptr.sig_bytes == 8);
        bool mode_set = (g_png_ptr.mode & PNG_HAVE_PNG_SIGNATURE) != 0;
        if (no_error && mode_set) {
            report_test_result("test_png_read_sig_valid_signature_sets_mode", true);
        } else {
            report_test_result("test_png_read_sig_valid_signature_sets_mode", false,
                               no_error ? "mode not set" : "signature bytes not 8");
        }
    } catch (const std::exception& e) {
        report_test_result("test_png_read_sig_valid_signature_sets_mode", false, e.what());
    }
}

// Test 5: IO state is updated when PNG_IO_STATE_SUPPORTED is defined
// Expectation: png_ptr->io_state equals PNG_IO_READING | PNG_IO_SIGNATURE
static void test_png_read_sig_io_state_update() {
    // Prepare
    g_png_ptr.sig_bytes = 0;
    g_png_ptr.mode = 0;
#ifdef PNG_IO_STATE_SUPPORTED
    g_png_ptr.io_state = 0;
#endif
    // Input: valid signature
    test_set_input_bytes(const_cast<unsigned char*>(PNG_SIGNATURE), 8);

    try {
        png_read_sig(reinterpret_cast<png_structrp>(&g_png_ptr), reinterpret_cast<png_inforp>(&g_info_ptr));
#ifdef PNG_IO_STATE_SUPPORTED
        unsigned expected = PNG_IO_READING | PNG_IO_SIGNATURE;
        if (g_png_ptr.io_state == expected) {
            report_test_result("test_png_read_sig_io_state_update", true);
        } else {
            report_test_result("test_png_read_sig_io_state_update", false,
                               "io_state mismatch");
        }
#else
        report_test_result("test_png_read_sig_io_state_update", true,
                           "PNG_IO_STATE_SUPPORTED not defined (test skipped)");
#endif
    } catch (const std::exception& e) {
        report_test_result("test_png_read_sig_io_state_update", false, e.what());
    }
}

// Main: Run all tests
int main() {
    std::cout << "Starting png_read_sig unit tests (C++11, no GTest)\n";

    // Run tests
    RUN_TEST(test_png_read_sig_no_op_when_already_signed);
    RUN_TEST(test_png_read_sig_not_a_png_file);
    RUN_TEST(test_png_read_sig_ascii_conversion_error);
    RUN_TEST(test_png_read_sig_valid_signature_sets_mode);
    RUN_TEST(test_png_read_sig_io_state_update);

    // Summary
    std::cout << "\nTest Summary: Ran " << g_tests_run << " tests, "
              << g_tests_failed << " failed.\n";

    return g_tests_failed == 0 ? 0 : 1;
}
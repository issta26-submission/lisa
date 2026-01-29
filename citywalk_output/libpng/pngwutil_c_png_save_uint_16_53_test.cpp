// Minimal C++11 test harness for the C function: png_save_uint_16
// This test suite exercises the behavior of encoding the lower 16 bits of an unsigned int
// into a 2-byte big-endian buffer: buf[0] = high byte of lower 16 bits, buf[1] = low byte.

#include <cstdint>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <pngpriv.h>


// Minimal compatible types to interface with the C function without requiring libpng headers.
// The real implementation uses libpng typedefs; for testing purposes we map them to standard types.
typedef unsigned char png_byte;
typedef png_byte*     png_bytep;

// Forward declaration of the C function under test.
// It is assumed to be compiled along with this test file and linked.
// Use extern "C" to avoid C++ name mangling issues when linking with the C object.
extern "C" {
void png_save_uint_16(png_bytep buf, unsigned int i);
}

// Simple test harness state
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Helper: print a simple hex pair for a byte
static std::string hex_pair(uint8_t b) {
    std::ostringstream os;
    os << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << (int)b;
    return os.str();
}

// Helper: report a failed test with details
static void report_failure(const char* test_name, const std::string& details) {
    std::cerr << "Test FAILED: " << test_name << " — " << details << std::endl;
    ++g_failed_tests;
}

// Test 1
// Description: When i is 0, the buffer should be [0x00, 0x00].
static bool test_png_save_uint_16_zero() {
    const char* test_name = "test_png_save_uint_16_zero";
    uint8_t buf[2] = {0xAA, 0xBB}; // pre-fill to ensure only first two bytes are overwritten

    png_save_uint_16(buf, 0);

    bool ok = (buf[0] == 0x00) && (buf[1] == 0x00);
    if (!ok) {
        std::ostringstream details;
        details << "buf after call = [" << hex_pair(buf[0]) << ", " << hex_pair(buf[1]) << "], "
                << "expected [00, 00]";
        report_failure(test_name, details.str());
    }
    return ok;
}

// Test 2
// Description: When i is small (0x00001234), buffer should be [0x12, 0x34].
static bool test_png_save_uint_16_small() {
    const char* test_name = "test_png_save_uint_16_small";
    uint8_t buf[2] = {0x11, 0x22};

    png_save_uint_16(buf, 0x1234);

    bool ok = (buf[0] == 0x12) && (buf[1] == 0x34);
    if (!ok) {
        std::ostringstream details;
        details << "buf after call = [" << hex_pair(buf[0]) << ", " << hex_pair(buf[1]) << "], "
                << "expected [12, 34]";
        report_failure(test_name, details.str());
    }
    return ok;
}

// Test 3
// Description: When i equals 0xFFFF, lower 16 bits are 0xFFFF -> buffer [0xFF, 0xFF].
static bool test_png_save_uint_16_max16() {
    const char* test_name = "test_png_save_uint_16_max16";
    uint8_t buf[2] = {0x00, 0x00};

    png_save_uint_16(buf, 0xFFFF);

    bool ok = (buf[0] == 0xFF) && (buf[1] == 0xFF);
    if (!ok) {
        std::ostringstream details;
        details << "buf after call = [" << hex_pair(buf[0]) << ", " << hex_pair(buf[1]) << "], "
                << "expected [FF, FF]";
        report_failure(test_name, details.str());
    }
    return ok;
}

// Test 4
// Description: When i is large (0x12345678), lower 16 bits are 0x5678 -> buffer [0x56, 0x78].
static bool test_png_save_uint_16_large() {
    const char* test_name = "test_png_save_uint_16_large";
    uint8_t buf[2] = {0x00, 0x00};

    png_save_uint_16(buf, 0x12345678);

    bool ok = (buf[0] == 0x56) && (buf[1] == 0x78);
    if (!ok) {
        std::ostringstream details;
        details << "buf after call = [" << hex_pair(buf[0]) << ", " << hex_pair(buf[1]) << "], "
                << "expected [56, 78]";
        report_failure(test_name, details.str());
    }
    return ok;
}

// Test 5
// Description: Repeated calls overwrite the buffer correctly without leaving remnants.
// i1 = 0x01 -> [0x00, 0x01], then i2 = 0xABCD -> [0xAB, 0xCD].
static bool test_png_save_uint_16_overwrite() {
    const char* test_name = "test_png_save_uint_16_overwrite";
    uint8_t buf[2] = {0x00, 0x00};

    png_save_uint_16(buf, 0x0001);
    bool ok1 = (buf[0] == 0x00) && (buf[1] == 0x01);

    png_save_uint_16(buf, 0xABCD);
    bool ok2 = (buf[0] == 0xAB) && (buf[1] == 0xCD);

    if (!(ok1 && ok2)) {
        std::ostringstream details;
        details << "After first call: [" << hex_pair( buf[0] ) << ", " << hex_pair(buf[1])
                << "], expected [00, 01]; "
                   "After second call: [" << hex_pair(buf[0]) << ", " << hex_pair(buf[1])
                << "], expected [AB, CD]";
        report_failure(test_name, details.str());
        return false;
    }
    return true;
}

// Test 6
// Description: Buffer content outside the first two bytes remains unchanged.
static bool test_png_save_uint_16_buffer_unchanged_outside() {
    const char* test_name = "test_png_save_uint_16_buffer_unchanged_outside";
    uint8_t buf[4] = {0xAA, 0xBB, 0xCC, 0xDD};

    png_save_uint_16(buf, 0x1234);

    bool ok = (buf[2] == 0xCC) && (buf[3] == 0xDD);
    if (!ok) {
        std::ostringstream details;
        details << "buf[2..3] = [" << hex_pair(buf[2]) << ", " << hex_pair(buf[3])
                << "], expected [CC, DD]";
        report_failure(test_name, details.str());
    }
    return ok;
}

// Test 7
// Description: i with all bits set (0xFFFFFFFF) should still yield lower 16 bits [0xFF, 0xFF].
static bool test_png_save_uint_16_all_bits_set() {
    const char* test_name = "test_png_save_uint_16_all_bits_set";
    uint8_t buf[2] = {0x00, 0x00};

    png_save_uint_16(buf, 0xFFFFFFFF);

    bool ok = (buf[0] == 0xFF) && (buf[1] == 0xFF);
    if (!ok) {
        std::ostringstream details;
        details << "buf after call = [" << hex_pair(buf[0]) << ", " << hex_pair(buf[1])
                << "], expected [FF, FF]";
        report_failure(test_name, details.str());
    }
    return ok;
}

// Entry point
int main() {
    std::cout << "Starting png_save_uint_16 test suite (C function)..." << std::endl;

    // Increment a test counter and run each test, reporting failures individually.
    ++g_total_tests; if (!test_png_save_uint_16_zero()) { /* already reported */ }
    ++g_total_tests; if (!test_png_save_uint_16_small()) { /* already reported */ }
    ++g_total_tests; if (!test_png_save_uint_16_max16()) { /* already reported */ }
    ++g_total_tests; if (!test_png_save_uint_16_large()) { /* already reported */ }
    ++g_total_tests; if (!test_png_save_uint_16_overwrite()) { /* already reported */ }
    ++g_total_tests; if (!test_png_save_uint_16_buffer_unchanged_outside()) { /* already reported */ }
    ++g_total_tests; if (!test_png_save_uint_16_all_bits_set()) { /* already reported */ }

    // Summary
    std::cout << "Tests run: " << g_total_tests
              << ", Failures: " << g_failed_tests
              << ", Passed: " << (g_total_tests - g_failed_tests) << std::endl;

    // Return non-zero if any test failed
    return (g_failed_tests == 0) ? 0 : 1;
}
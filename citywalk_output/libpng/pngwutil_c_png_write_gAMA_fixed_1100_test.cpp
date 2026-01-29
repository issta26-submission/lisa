// Minimal C++11 test harness for the focal method: png_write_gAMA_fixed
// This test suite provides a self-contained mock environment to verify
// that png_write_gAMA_fixed writes the expected 4-byte gamma payload.
// No external test framework is used (GTest is intentionally avoided as per instructions).

#include <cstdint>
#include <vector>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Domain-specific notes (mapped to Candidate Keywords from Step 1):
// - Focal method: png_write_gAMA_fixed
// - Dependencies: png_structrp (png_ptr), png_fixed_point (file_gamma), png_byte buf[4],
//                 png_debug, png_save_uint_32, png_write_complete_chunk, png_gAMA (chunk type).
// - Data handling: the 4-byte payload encodes file_gamma in big-endian order.

//////////////////////////////////////////////////////////////////////
// Mocked/Minimal PNG infrastructure and dependencies (Step 2)
//////////////////////////////////////////////////////////////////////

// Basic type aliases to simulate libpng types in a self-contained way
using png_uint_32 = uint32_t;
using png_bytep = unsigned char*;
using png_const_bytep = const unsigned char*;
using png_byte = unsigned char;
using png_structrp = struct png_struct_s*; // pointer type to mimic libpng style
using png_fixed_point = int;               // signed 32-bit fixed-point

// Forward declarations of dependencies (as used by the focal method)
extern "C" void png_debug(int, const char*);
extern "C" void png_save_uint_32(png_bytep, png_uint_32);
extern "C" void png_write_complete_chunk(png_structrp, png_uint_32, png_const_bytep, size_t);

// PNG chunk type constant (gAMA)
extern "C" const png_uint_32 png_gAMA;

// Simple internal PNG structure to collect written data for test assertions
struct png_struct_s {
    std::vector<uint8_t> out; // collected payload written by chunks (simplified)
};

// Helper utility to compare buffers
static bool buffers_equal(const std::vector<uint8_t>& a, const uint8_t* b, size_t len) {
    if (a.size() != len) return false;
    return std::memcmp(a.data(), b, len) == 0;
}

// Implementation of dependencies (stubs for testing)

// No-op debug message collector (we don't assert on log output in unit tests)
extern "C" void png_debug(int /*level*/, const char* /*message*/) {
    (void)level; (void)message;
}

// Write a 4-byte big-endian unsigned integer into buf
extern "C" void png_save_uint_32(png_bytep buf, png_uint_32 i) {
    buf[0] = static_cast<png_byte>((i >> 24) & 0xFF);
    buf[1] = static_cast<png_byte>((i >> 16) & 0xFF);
    buf[2] = static_cast<png_byte>((i >> 8) & 0xFF);
    buf[3] = static_cast<png_byte>(i & 0xFF);
}

// Simplified chunk writer: append the payload data to png_ptr->out
extern "C" void png_write_complete_chunk(png_structrp png_ptr, png_uint_32 /*chunk_name*/,
                                         png_const_bytep data, size_t length) {
    // In a full libpng, this would emit length, type, data, and CRC.
    // For testing the focal method, we only collect the payload data.
    png_ptr->out.insert(png_ptr->out.end(), data, data + length);
}

//////////////////////////////////////////////////////////////////////
// Focal method under test (as provided in the prompt)
//////////////////////////////////////////////////////////////////////

extern "C" void png_write_gAMA_fixed(png_structrp png_ptr, png_fixed_point file_gamma)
{
{
   png_byte buf[4];
   png_debug(1, "in png_write_gAMA");
   /* file_gamma is saved in 1/100,000ths */
   png_save_uint_32(buf, (png_uint_32)file_gamma);
   png_write_complete_chunk(png_ptr, png_gAMA, buf, 4);
}
}

//////////////////////////////////////////////////////////////////////
// Test harness (Step 3) - self-contained tests without GTest
//////////////////////////////////////////////////////////////////////

// Global test state
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Simple non-terminating assertion helpers
#define TEST_ASSERT(cond, msg) do { \
    ++g_total_tests; \
    if(!(cond)) { \
        std::cerr << "UNIT TEST FAILED: " << msg << std::endl; \
        ++g_failed_tests; \
    } \
} while(0)

#define TEST_CHECK_VECTOR_EQ(png_ptr, expected_vec) do { \
    TEST_ASSERT((png_ptr)->out.size() == (expected_vec).size(), "payload length mismatch"); \
    if((png_ptr)->out.size() == (expected_vec).size()) { \
        if(std::memcmp((png_ptr)->out.data(), (expected_vec).data(), (expected_vec).size()) != 0) { \
            TEST_ASSERT(false, "payload contents do not match expected"); \
        } \
    } \
} while(0)

// Helper to build expected 4-byte big-endian payload from gamma value
static std::vector<uint8_t> expected_gamma_payload(uint32_t gamma) {
    uint8_t bytes[4];
    bytes[0] = static_cast<uint8_t>((gamma >> 24) & 0xFF);
    bytes[1] = static_cast<uint8_t>((gamma >> 16) & 0xFF);
    bytes[2] = static_cast<uint8_t>((gamma >> 8) & 0xFF);
    bytes[3] = static_cast<uint8_t>(gamma & 0xFF);
    return std::vector<uint8_t>(bytes, bytes + 4);
}

// Test 1: gamma = 0 => payload 00 00 00 00
static void test_gAMA_fixed_gamma_zero() {
    png_struct_s png;
    png.out.clear();

    png_write_gAMA_fixed(&png, 0);

    std::vector<uint8_t> exp = expected_gamma_payload(0);
    TEST_CHECK_VECTOR_EQ(&png, exp);
}

// Test 2: gamma = 1 => payload 00 00 00 01
static void test_gAMA_fixed_gamma_one() {
    png_struct_s png;
    png.out.clear();

    png_write_gAMA_fixed(&png, 1);

    std::vector<uint8_t> exp = expected_gamma_payload(1);
    TEST_CHECK_VECTOR_EQ(&png, exp);
}

// Test 3: gamma = 100000 (0x000186A0) => payload 00 01 86 A0
static void test_gAMA_fixed_gamma_100000() {
    png_struct_s png;
    png.out.clear();

    png_write_gAMA_fixed(&png, 100000);

    std::vector<uint8_t> exp = expected_gamma_payload(100000);
    TEST_CHECK_VECTOR_EQ(&png, exp);
}

// Test 4: gamma = -1 (0xFFFFFFFF when cast) => payload FF FF FF FF
static void test_gAMA_fixed_gamma_negative_one() {
    png_struct_s png;
    png.out.clear();

    png_write_gAMA_fixed(&png, static_cast<png_fixed_point>(-1));

    std::vector<uint8_t> exp = expected_gamma_payload(0xFFFFFFFF);
    TEST_CHECK_VECTOR_EQ(&png, exp);
}

// Test 5: Multiple sequential writes accumulate payloads correctly
static void test_gAMA_fixed_gamma_multiple_writes() {
    png_struct_s png;
    png.out.clear();

    // First gamma
    png_write_gAMA_fixed(&png, 5);
    // Second gamma
    png_write_gAMA_fixed(&png, 0x01020304);

    // Expected payload is concatenation of two 4-byte chunks
    std::vector<uint8_t> exp;
    std::vector<uint8_t> a = expected_gamma_payload(5);
    std::vector<uint8_t> b = expected_gamma_payload(0x01020304);
    exp.insert(exp.end(), a.begin(), a.end());
    exp.insert(exp.end(), b.begin(), b.end());

    TEST_ASSERT(png.out.size() == exp.size(), "multiple writes: payload length mismatch");
    if(png.out.size() == exp.size()) {
        if(std::memcmp(png.out.data(), exp.data(), exp.size()) != 0) {
            TEST_ASSERT(false, "multiple writes: payload contents do not match expected concatenation");
        }
    }
}

// Run all tests
int main() {
    std::cout << "Running png_write_gAMA_fixed unit tests (self-contained)..." << std::endl;

    test_gAMA_fixed_gamma_zero();
    test_gAMA_fixed_gamma_one();
    test_gAMA_fixed_gamma_100000();
    test_gAMA_fixed_gamma_negative_one();
    test_gAMA_fixed_gamma_multiple_writes();

    std::cout << "Tests completed. Total: " << g_total_tests
              << ", Failures: " << g_failed_tests << std::endl;

    // Return non-zero if any test failed
    return g_failed_tests ? 1 : 0;
}

// Definitions to satisfy external linkage for the constant used by the focal function
extern "C" const png_uint_32 png_gAMA = 0x67414D41; // ASCII 'gAMA' in hex
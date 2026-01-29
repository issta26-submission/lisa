/*
Step 1: Program Understanding (Candidate Keywords)
- Candidate Keywords extracted from png_write_eXIf:
  png_ptr, png_bytep exif, int num_exif
  int i, png_byte buf[1]
  png_debug, png_write_chunk_header, png_write_chunk_data, png_write_chunk_end
  png_eXIf (chunk type constant)
- Core behavior: write a PNG tEXt-like eXIf chunk by emitting one 1-byte data unit per exif byte.
- Loop i from 0 to num_exif-1, write a single byte from exif[i], then end chunk.
- This implies verification should check header emission, per-byte data emission, and chunk end signals, for both non-zero and zero lengths.

Step 2: Unit Test Generation (png_write_eXIf in pngwutil.c)
- We create a minimal, isolated test harness that provides mock implementations for:
  - png_write_chunk_header(png_structrp, png_uint_32, png_uint_32)
  - png_write_chunk_data(png_structrp, png_bytep, size_t)
  - png_write_chunk_end(png_structrp)
  - png_debug(int, const char*)
  - and a symbol for png_eXIf
- We declare the focal function with C linkage to link from the C source:
  extern "C" void png_write_eXIf(png_structrp png_ptr, png_bytep exif, int num_exif);
- The tests validate:
  - When num_exif > 0, a single header is written with chunk_name == png_eXIf and length == num_exif.
  - For each exif byte, a 1-byte data chunk is written with the correct value sequence.
  - After finishing, a single png_write_chunk_end call is issued.
  - For num_exif == 0, header length is 0 and no data chunks are emitted, but end is still invoked.
- Static handling: We expose only the necessary fixtures; we don’t touch private members of the production code.
- Test approach uses non-terminating assertions (custom EXPECT-like checks) to maximize code coverage without aborting on failure.

Step 3: Test Case Refinement (Domain Knowledge)
- Test cases cover positive length (eXIf with 3, 5 bytes) and zero length (0 bytes) paths.
- Verify correctness of the emitted sequence (header, per-byte data blocks, end).
- Use direct comparisons of emitted data to ensure exact byte-by-byte correctness.
- Tests are executable from main() (as required when not using GTest).

Notes on dependencies
- The test provides lightweight stubs for the PNG writing helpers and a mock PNG struct type to compile against the focal function.
- The test is designed to compile under C++11 without GTest, using only the C runtime and C++ standard library.

Code (unit test suite for png_write_eXIf)
*/

#include <cstdint>
#include <vector>
#include <utility>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>
#include <cstddef>


// Candidate Keywords (for reference)
// png_ptr, png_bytep, exif, num_exif, png_write_chunk_header, png_write_chunk_data, png_write_chunk_end, png_eXIf, png_debug

// We declare C linkage for the production-like API to link with the focal method.
extern "C" {

    // Minimal type aliases to mirror the production API used by pngwutil.c
    typedef struct png_struct_def png_struct;
    typedef png_struct* png_structrp;
    typedef unsigned char png_bytep;
    typedef unsigned int png_uint_32;

    // Expose the eXIf chunk type symbol as the test's value.
    extern png_uint_32 png_eXIf;

    // Prototypes for the operations used by png_write_eXIf
    void png_write_chunk_header(png_structrp png_ptr, png_uint_32 chunk_name,
                                png_uint_32 length);
    void png_write_chunk_data(png_structrp png_ptr, png_bytep data, size_t length);
    void png_write_chunk_end(png_structrp png_ptr);

    // Debug stub used by png_write_eXIf (no-op)
    void png_debug(int level, const char* message);

    // The focal function under test (C linkage)
    void png_write_eXIf(png_structrp png_ptr, png_bytep exif, int num_exif);
}

// Global fixtures to capture write operations performed by the focal method
static std::vector<std::pair<uint32_t, uint32_t>> g_headers;                 // (chunk_name, length)
static std::vector<std::vector<unsigned char>> g_datas;                      // per-call data payloads
static int g_end_calls = 0;

// Stub implementations to capture the behavior of the PNG writer.
// They mimic the external C API used by png_write_eXIf without performing real IO.

extern "C" {

// Provide a numeric value for the eXIf chunk type (ASCII: "eXIf")
// Using the macro-like construction: ('e'<<24) | ('X'<<16) | ('I'<<8) | 'f'
png_uint_32 png_eXIf = ('e' << 24) | ('X' << 16) | ('I' << 8) | 'f';

void png_write_chunk_header(png_structrp /*png_ptr*/, png_uint_32 chunk_name,
                            png_uint_32 length)
{
    g_headers.emplace_back(chunk_name, length);
}

void png_write_chunk_data(png_structrp /*png_ptr*/, png_bytep data, size_t length)
{
    std::vector<unsigned char> v(length);
    for (size_t i = 0; i < length; ++i) {
        v[i] = data[i];
    }
    g_datas.emplace_back(std::move(v));
}

void png_write_chunk_end(png_structrp /*png_ptr*/)
{
    ++g_end_calls;
}

// No-op for the debug function to avoid noisy output during tests.
void png_debug(int /*level*/, const char* /*message*/)
{
    // Intentionally empty
}

} // extern "C"

// Simple non-terminating assertion helpers to maximize coverage without aborting.
static int g_test_failures = 0;

static void test_expect_true(bool cond, const char* msg)
{
    if (!cond) {
        std::cerr << "TEST FAIL: " << msg << std::endl;
        ++g_test_failures;
    }
}

static void test_expect_uint32_eq(uint32_t a, uint32_t b, const char* msg)
{
    if (a != b) {
        std::cerr << "TEST FAIL: " << msg
                  << " | expected " << b << " but got " << a << std::endl;
        ++g_test_failures;
    }
}

static void test_expect_size_t_eq(size_t a, size_t b, const char* msg)
{
    if (a != b) {
        std::cerr << "TEST FAIL: " << msg
                  << " | expected " << b << " but got " << a << std::endl;
        ++g_test_failures;
    }
}

// Test 1: Exif has 3 bytes. Validate header, per-byte data calls, and end.
static void test_png_write_eXIf_three_bytes()
{
    // Reset fixtures
    g_headers.clear();
    g_datas.clear();
    g_end_calls = 0;

    unsigned char exif[3] = { 0xAA, 0xBB, 0xCC };
    int num_exif = 3;

    // Call the focal method
    png_write_eXIf(nullptr, exif, num_exif);

    // Assertions
    test_expect_size_t_eq(g_headers.size(), 1, "header count should be 1");
    test_expect_uint32_eq(g_headers[0].first, png_eXIf, "header chunk_name should be png_eXIf");
    test_expect_uint32_eq(g_headers[0].second, (uint32_t)num_exif, "header length should equal num_exif");

    test_expect_true(g_end_calls == 1, "end should be called exactly once");

    test_expect_size_t_eq(g_datas.size(), (size_t)num_exif, "data calls count should equal num_exif");
    // Validate per-byte payloads
    if (g_datas.size() == (size_t)num_exif) {
        test_expect_true(g_datas[0].size() == 1 && g_datas[0][0] == 0xAA, "first data byte should be 0xAA");
        test_expect_true(g_datas[1].size() == 1 && g_datas[1][0] == 0xBB, "second data byte should be 0xBB");
        test_expect_true(g_datas[2].size() == 1 && g_datas[2][0] == 0xCC, "third data byte should be 0xCC");
    }
}

// Test 2: Exif length is zero. Validate no data payloads but header and end are emitted.
static void test_png_write_eXIf_zero_bytes()
{
    // Reset fixtures
    g_headers.clear();
    g_datas.clear();
    g_end_calls = 0;

    unsigned char* exif = nullptr;
    int num_exif = 0;

    png_write_eXIf(nullptr, exif, num_exif);

    // Assertions
    test_expect_size_t_eq(g_headers.size(), 1, "header count should be 1 for zero-length exif");
    test_expect_uint32_eq(g_headers[0].second, 0, "header length should be 0 when num_exif is 0");

    test_expect_true(g_end_calls == 1, "end should be called exactly once even for zero exif");

    test_expect_size_t_eq(g_datas.size(), 0, "no data payloads should be emitted for zero exif");
}

// Test 3: Exif length 5. Validate sequence correctness for multiple bytes.
static void test_png_write_eXIf_five_bytes()
{
    // Reset fixtures
    g_headers.clear();
    g_datas.clear();
    g_end_calls = 0;

    unsigned char exif[5] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
    int num_exif = 5;

    png_write_eXIf(nullptr, exif, num_exif);

    // Assertions
    test_expect_size_t_eq(g_headers.size(), 1, "header count should be 1 for five-byte exif");
    test_expect_uint32_eq(g_headers[0].first, png_eXIf, "header chunk_name should be png_eXIf");
    test_expect_uint32_eq(g_headers[0].second, (uint32_t)num_exif, "header length should equal num_exif");

    test_expect_true(g_end_calls == 1, "end should be called exactly once");

    test_expect_size_t_eq(g_datas.size(), (size_t)num_exif, "data calls count should equal num_exif");

    if (g_datas.size() == (size_t)num_exif) {
        for (int i = 0; i < num_exif; ++i) {
            if (g_datas[i].size() != 1) {
                std::cerr << "TEST FAIL: data block " << i << " size != 1\n";
                ++g_test_failures;
                break;
            }
            if (g_datas[i][0] != static_cast<unsigned char>(i + 1)) {
                std::cerr << "TEST FAIL: data block " << i
                          << " expected " << static_cast<unsigned char>(i + 1)
                          << " got " << static_cast<unsigned int>(g_datas[i][0]) << "\n";
                ++g_test_failures;
            }
        }
    }
}

// Runner
int main()
{
    // Run tests (called from main as required)
    test_png_write_eXIf_three_bytes();
    test_png_write_eXIf_zero_bytes();
    test_png_write_eXIf_five_bytes();

    // Summary
    if (g_test_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << g_test_failures << " test(s) failed.\n";
        return 1;
    }
}
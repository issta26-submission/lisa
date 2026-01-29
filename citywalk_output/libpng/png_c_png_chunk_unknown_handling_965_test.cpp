// Unit test suite for png_chunk_unknown_handling (C++11, no GTest)

/*
This test suite mocks the libpng interaction to verify that:
- png_chunk_unknown_handling correctly converts a 4-character chunk code
  into a null-terminated string via PNG_CSTRING_FROM_CHUNK and passes it
  to png_handle_as_unknown.
- The returned value from png_chunk_unknown_handling matches the value
  produced by the mocked png_handle_as_unknown based on the chunk string.

Test strategy:
- Provide C linkage bridges for the focal C function (png_chunk_unknown_handling)
  and the dependency it calls (png_handle_as_unknown) so we can intercept inputs.
- Implement a simple fake png_handle_as_unknown that records the passed chunk
  string and returns a known value for a specific chunk ("IHDR"), and 0 otherwise.
- Call png_chunk_unknown_handling with known 4-byte codes corresponding to
  "IHDR", "tEXt", and "IDAT" and verify both the chunk string captured and
  the return value.

Notes:
- This test avoids GTest and uses standard C++ assertions.
- The test targets the logic inside png_chunk_unknown_handling, not the
  external libpng implementation.
*/

// Necessary includes
#include <cassert>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Domain-specific typedefs to enable compiling the focal C code in a C++ test.
// These are minimal shims to allow linking with the png.c translation unit provided.
typedef const void* png_const_structrp;
typedef unsigned char* png_bytep;
typedef const unsigned char* png_const_bytep;
typedef unsigned int png_uint_32;

// Forward declarations of the focal function under test and its dependency.
// We assume C linkage to match the original C code.
extern "C" int png_chunk_unknown_handling(png_const_structrp png_ptr, png_uint_32 chunk_name);

// Mocked dependency: we provide a C-linkage implementation to intercept arguments.
static std::string g_last_chunk;

// Mocked implementation of the dependency that png_chunk_unknown_handling calls.
// This allows us to verify that the correct chunk string is passed.
extern "C" int png_handle_as_unknown(png_const_structrp png_ptr, png_const_bytep chunk_name)
{
    // Capture the chunk string (null-terminated) passed to the dependency.
    g_last_chunk = reinterpret_cast<const char*>(chunk_name);

    // For test purposes, return a distinct value for the "IHDR" chunk string,
    // and a default value for all other chunks.
    if (g_last_chunk == "IHDR")
        return 123;   // Arbitrary non-zero value to verify propagation
    return 0;         // Default for all other chunk strings
}

// Helper: provide access to the last chunk string for assertions in tests
extern "C" const char* get_last_chunk_string()
{
    return g_last_chunk.c_str();
}

// Test 1: Ensure that IHDR is correctly marshaled to "IHDR" and the return value is propagated.
void test_png_chunk_unknown_handling_ihdr()
{
    g_last_chunk.clear();

    // "IHDR" chunk code: 0x49 0x48 0x44 0x52 -> 0x49484452 in big-endian
    const png_uint_32 ihdr_code = 0x49484452;

    int ret = png_chunk_unknown_handling(nullptr, ihdr_code);

    // Validate that the return value comes from the mocked png_handle_as_unknown
    assert(ret == 123 && "IHDR path should return 123 as per mock implementation");

    // Validate that the chunk string passed to the dependency was "IHDR"
    const char* passed = get_last_chunk_string();
    assert(passed != nullptr);
    assert(std::string(passed) == "IHDR" && "Chunk string should be 'IHDR' for IHDR code");

    std::cout << "Test 1 (IHDR) passed: correct string marshaling and return value.\n";
}

// Test 2: Ensure that tEXt chunk code is marshaled to "tEXt" and the default return is 0.
void test_png_chunk_unknown_handling_text()
{
    g_last_chunk.clear();

    // "tEXt" chunk code: 0x74 0x45 0x58 0x74 -> 0x74455874
    const png_uint_32 text_code = 0x74455874;

    int ret = png_chunk_unknown_handling(nullptr, text_code);

    // Expect default return value from mock (0)
    assert(ret == 0 && "tEXt path should return 0 (default) in mock");

    // Chunk string should be "tEXt"
    const char* passed = get_last_chunk_string();
    assert(passed != nullptr);
    assert(std::string(passed) == "tEXt" && "Chunk string should be 'tEXt' for tEXt code");

    std::cout << "Test 2 (tEXt) passed: correct string marshaling and default return value.\n";
}

// Test 3: Ensure that IDAT chunk code is marshaled to "IDAT" and the default return is 0.
void test_png_chunk_unknown_handling_idat()
{
    g_last_chunk.clear();

    // "IDAT" chunk code: 0x49 0x44 0x41 0x54 -> 0x49444154
    const png_uint_32 idat_code = 0x49444154;

    int ret = png_chunk_unknown_handling(nullptr, idat_code);

    // Expect default return value from mock (0)
    assert(ret == 0 && "IDAT path should return 0 (default) in mock");

    // Chunk string should be "IDAT"
    const char* passed = get_last_chunk_string();
    assert(passed != nullptr);
    assert(std::string(passed) == "IDAT" && "Chunk string should be 'IDAT' for IDAT code");

    std::cout << "Test 3 (IDAT) passed: correct string marshaling and default return value.\n";
}

// Main test harness: execute all tests and report results.
int main()
{
    // Run tests; any assertion failure will terminate the program.
    test_png_chunk_unknown_handling_ihdr();
    test_png_chunk_unknown_handling_text();
    test_png_chunk_unknown_handling_idat();

    std::cout << "All tests passed successfully.\n";
    return 0;
}
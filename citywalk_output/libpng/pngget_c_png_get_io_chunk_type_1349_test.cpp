// C++11 unit test suite for libpng's png_get_io_chunk_type
// This test suite targets the focal method:
//    png_bytep png_get_io_chunk_type(png_const_structrp png_ptr)
// which returns a pointer to the 4-byte chunk name stored inside the png_struct.
// The test uses libpng internal structures (pngpriv.h) to set the chunk_name field
// and then calls the function to verify it returns the expected chunk name.
//
// Notes:
// - We avoid GTest and use a lightweight, non-terminating assertion style.
// - We cover multiple common chunk types to exercise the data-path (no branches in code,
//   but varying inputs increases test coverage of the data access path).
// - The tests rely on the project exposing internal structures via pngpriv.h.

#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>
#include <png.h>


// Include libpng internal headers to access the png_struct layout.
// The tests assume the environment provides these headers in the include path.

// Simple non-terminating test harness
static int g_total_tests = 0;
static int g_passed_tests = 0;

// Helper to log test results without stopping execution
static void log_test_result(const std::string& test_name, bool passed, const std::string& details = "")
{
    ++g_total_tests;
    if (passed) {
        ++g_passed_tests;
        std::cout << "[OK]   " << test_name << "\n";
    } else {
        std::cout << "[FAILED] " << test_name;
        if (!details.empty()) {
            std::cout << " - " << details;
        }
        std::cout << "\n";
    }
}

// Core test helper:
// - Creates a minimal png_struct instance on the stack.
// - Sets the 4-byte chunk_name to the provided expected value.
// - Invokes png_get_io_chunk_type and compares the returned pointer's 4 bytes with expected.
static bool test_chunk_type(const char* expected4)
{
    // Ensure the expected string is exactly 4 characters
    if (expected4 == nullptr) return false;
    if (std::strlen(expected4) != 4) {
        // If it's not exactly 4 chars, this is a misusage of the helper
        return false;
    }

    // Create a minimal png_struct. We only rely on the public layout for the needed field.
    png_struct png;
    // Zero initialize to avoid accidental leftover data
    std::memset(&png, 0, sizeof(png));

    // Copy the 4-byte chunk name into the internal field.
    // The internal structure (from pngpriv.h) defines chunk_name as a 4-byte array.
    std::memcpy(png.chunk_name, expected4, 4);

    // Call the focal function.
    // Cast to the expected opaque type as libpng exposes it via png_const_structrp.
    png_bytep result = png_get_io_chunk_type(reinterpret_cast<png_const_structrp>(&png));

    // Validate result is non-null
    if (result == nullptr) {
        log_test_result("test_chunk_type(" + std::string(expected4) + ")", false,
                        "png_get_io_chunk_type returned null");
        return false;
    }

    // Compare the first 4 bytes of the returned pointer with the expected 4 characters.
    char actual[5];
    std::memcpy(actual, result, 4);
    actual[4] = '\0';

    bool ok = (std::strncmp(actual, expected4, 4) == 0);
    if (!ok) {
        log_test_result("test_chunk_type(" + std::string(expected4) + ")", false,
                        "expected \"" + std::string(expected4, 4) + "\", got \"" + std::string(actual) + "\"");
    }

    return ok;
}

// Test 1: IHDR chunk type
// IHDR is the critical header chunk for PNG images. Verifies that the function
// correctly returns the pointer to the internal chunk_name when set to "IHDR".
static bool test_png_get_io_chunk_type_IHDR()
{
    return test_chunk_type("IHDR");
}

// Test 2: IDAT chunk type
// IDAT contains the image data. This ensures the function works for a common data chunk.
static bool test_png_get_io_chunk_type_IDAT()
{
    return test_chunk_type("IDAT");
}

// Test 3: PLTE chunk type
// PLTE contains the palette and exercises an alternative normal chunk type.
static bool test_png_get_io_chunk_type_PLTE()
{
    return test_chunk_type("PLTE");
}

// Test 4: tEXt chunk type
// tEXt chunks store textual data; ensures non-ASCII or mixed chunks are handled similarly.
static bool test_png_get_io_chunk_type_tEXt()
{
    return test_chunk_type("tEXt");
}

// Entry point for the test suite
int main()
{
    // Run tests. Each test uses non-terminating assertions (logs failures but continues).
    log_test_result("test_png_get_io_chunk_type_IHDR", test_png_get_io_chunk_type_IHDR());
    log_test_result("test_png_get_io_chunk_type_IDAT", test_png_get_io_chunk_type_IDAT());
    log_test_result("test_png_get_io_chunk_type_PLTE", test_png_get_io_chunk_type_PLTE());
    log_test_result("test_png_get_io_chunk_type_tEXt", test_png_get_io_chunk_type_tEXt());

    // Summary
    std::cout << "\nTest Summary: " << g_passed_tests << " passed, "
              << (g_total_tests - g_passed_tests) << " failed, "
              << g_total_tests << " total.\n";

    // Return non-zero if any test failed
    return (g_total_tests == g_passed_tests) ? 0 : 1;
}
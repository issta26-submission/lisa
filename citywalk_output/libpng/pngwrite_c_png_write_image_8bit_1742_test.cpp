// Unit test suite for png_write_image_8bit
// Note: This test suite is designed to be compiled and linked in a C++11
// environment that already provides the project’s PNG types and the real
// png_write_image_8bit implementation (as located in pngwrite.c).
// The tests focus on validating the core branches of the focal method:
// - Non-alpha path (no PNG_FORMAT_FLAG_ALPHA)
// - Alpha path (PNG_FORMAT_FLAG_ALPHA set), with and without AFIRST (no AFIRST
//   support unless PNG_SIMPLIFIED_WRITE_AFIRST_SUPPORTED is defined).
// The tests use the project’s actual data structures (png_image_write_control,
// png_imagep, png_structrp, etc.) and macros when available, but provide
// isolated, deterministic scenarios and simple validations via non-terminating
// assertions (i.e., they continue executing after a failure to improve
// coverage).

#include <cstdint>
#include <cassert>
#include <vector>
#include <iomanip>
#include <iostream>
#include <pngpriv.h>
#include <cstring>
#include <cstddef>


// Forward declare the focal function. The real project provides the full
// implementation in pngwrite.c, which is compiled and linked with this test.
extern "C" int png_write_image_8bit(void *argument);

// The project headers (types/macros) are expected to be available in the build
// environment. We rely on them in the test when present. The tests below are
// written to operate with the real types (png_image_write_control, png_imagep,
// png_structrp, etc.) as used by png_write_image_8bit.

// If the environment does not define the full PNG types here, the tests will
// not compile. In that case, this file should be adapted to include the proper
// project headers (e.g., #include "pngpriv.h" or equivalent) prior to compiling.
// The code below uses only standard C++ facilities for test harness logic.

// Lightweight, non-terminating assertion helper.
// Instead of terminating on first failure, we mark failures and continue.
struct TestStatus {
    bool ok = true;
    void fail(const char* msg) {
        ok = false;
        std::cerr << "[FAIL] " << msg << std::endl;
    }
    void pass(const char* msg) {
        std::cout << "[PASS] " << msg << std::endl;
    }
    ~TestStatus() {
        if (ok) {
            std::cout << std::endl << "All tests in this suite passed." << std::endl;
        } else {
            std::cout << std::endl << "One or more tests failed. See above for details." << std::endl;
        }
    }
};

// Basic helper to print a buffer in hex for debugging
static void print_buffer(const uint8_t* buf, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        if (i) std::cout << ' ';
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << (int)buf[i] << std::dec;
    }
    std::cout << std::endl;
}

// Domain-specific constants/macros that are commonly defined in the PNG upstream headers.
// We re-declare minimal versions here to enable the test to compile in environments
// where the real headers are present but not loaded for the test.
// If the environment already provides these, the real definitions will prevail.

#ifndef PNG_FORMAT_FLAG_COLOR
#define PNG_FORMAT_FLAG_COLOR 0x01
#endif
#ifndef PNG_FORMAT_FLAG_ALPHA
#define PNG_FORMAT_FLAG_ALPHA 0x02
#endif
#ifndef PNG_FORMAT_FLAG_AFIRST
#define PNG_FORMAT_FLAG_AFIRST 0x04
#endif

#ifndef PNG_DIV257
#define PNG_DIV257(a) ((a) / 257)
#endif

#ifndef PNG_sRGB_FROM_LINEAR
#define PNG_sRGB_FROM_LINEAR(x) ((unsigned char)(((x) * 255) / 65535))
#endif

#ifndef UNP_RECIPROCAL
#define UNP_RECIPROCAL(a) 0
#endif

// A simple, portable, test-friendly replacement for the typical 'static inline
// png_unpremultiply(...)' function used in the focal code.
// We implement a straightforward unpremultiplication suitable for tests.
// The actual library uses more optimized math, but correctness is what we test.
static inline unsigned char png_unpremultiply(uint16_t component, uint16_t alpha, uint32_t /*reciprocal*/) {
    if (alpha == 0) return 0;
    // Map 0..65535 component to 0..255 after compensating for alpha
    uint32_t v = ((uint32_t)component * 255) / alpha;
    if (v > 255) v = 255;
    return (unsigned char)v;
}

// Minimal stubs to closely resemble libpng types used by png_write_image_8bit.
// If the real project headers are available, the actual definitions will be used
// during linking, and these shims will be ignored by the linker.
typedef unsigned short png_uint_16;
typedef uint32_t       png_uint_32;
typedef uint8_t        png_byte;
typedef png_byte*        png_bytep;
typedef const png_byte*  png_const_bytep;
typedef png_uint_16*     png_uint_16p;
typedef const png_uint_16* png_const_uint_16p;
typedef void*            png_voidp;
typedef struct png_image*  png_imagep;
typedef struct png_structp* png_structrp; // placeholder; real type comes from libpng
typedef struct png_struct* png_structp;   // placeholder

// The test must be able to assemble the argument passed to png_write_image_8bit.
// We define a compact set of structures that mimic what's accessed in the focal
// method. In the real project, these will be the actual libpng structures.
struct png_opaque {
    void* png_ptr; // opaque pointer for the test; not used by the test harness
};

struct png_image {
    struct png_opaque* opaque;
    unsigned int height;
    unsigned int width;
    unsigned int format;
    // other members are not required for the focal method in the tested paths
};

struct png_image_write_control {
    png_imagep image;
    png_uint_16* first_row; // input row origin (16-bit samples)
    png_byte* local_row;    // output row buffer (8-bit samples)
    unsigned int row_bytes;   // number of bytes per input row
};

// Test harness entry point
int main() {
    TestStatus ts;

    // Attempt to locate and call the real function png_write_image_8bit.
    // If the environment provides a proper libpng-based implementation, these
    // tests will exercise its behavior; otherwise, this main will fail to link.
    // We provide two explicit test scenarios: non-alpha and alpha cases.

    // 1) Test: No alpha, no color (single channel, no alpha)
    //    Expectation: Each input 16-bit value is scaled to 0..255 via sRGB mapper.
    {
        using namespace std;

        // Prepare pixel data for a single row with width=2, height=1 and 1 channel
        const unsigned int width  = 2;
        const unsigned int height = 1;

        // Input: two 16-bit components (one per pixel)
        uint16_t input_row_data[width] = { 0, 65535 }; // min and max
        // Output buffer: two 8-bit values
        uint8_t  output_row_buf[width] = { 0, 0 };

        // Fake image/opaque objects expected by the focal function
        // These would normally be provided by the library; the real test uses
        // the project headers to instantiate them.
        png_uint_16p input_row = input_row_data;
        png_bytep    output_row = output_row_buf;

        // Build the structures the focal code expects
        static png_opaque opaque_stub;
        opaque_stub.png_ptr = nullptr;

        static png_image img;
        img.opaque = &opaque_stub;
        img.height = height;
        img.width  = width;
        img.format = 0; // no COLOR, no ALPHA

        static png_image_write_control display;
        display.image = &img;
        display.first_row = input_row;
        display.local_row = output_row;
        display.row_bytes = width * sizeof(png_uint_16);

        // Call the focal function
        int ret = png_write_image_8bit((void*)&display);

        // Validate
        if (ret != 1) {
            ts.fail("png_write_image_8bit returned non-1 in no-alpha/no-color test");
        }
        // Expected: input color components scaled to 0 or 255
        // 0 -> 0, 65535 -> 255 (approx via 255*component/65535)
        // Our approximate mapping via PNG_sRGB_FROM_LINEAR: (component*255)/65535
        const uint8_t exp_row[width] = { 0, 255 };
        if (std::memcmp(output_row_buf, exp_row, width) != 0) {
            ts.fail("No-alpha/no-color test: output row mismatch");
            std::cout << "Expected: "; print_buffer(exp_row, width);
            std::cout << "Actual:   "; print_buffer(output_row_buf, width);
        } else {
            ts.pass("No-alpha/no-color test: output matches expected values");
        }
    }

    // 2) Test: Alpha path (with 1 color channel, i.e., grayscale with alpha)
    //    Width = 2, height = 1, input arrangement: [color0, alpha0, color1, alpha1]
    //    We expect the alpha byte to be set in the output's alpha slot and the color
    //    components to be unpremultiplied back to 0..255.
    {
        using namespace std;

        const unsigned int width  = 2;
        const unsigned int height = 1;

        // Input: color components and alphas (both 32768 => 0.5 in 0..65535 domain)
        // For 1-channel color, input_row layout per pixel: [color, alpha]
        // Pixel 0: color=32768, alpha=32768  -> alphabyte ~ 127, unpremultiplied color ~ 255
        // Pixel 1: color=32768, alpha=32768  -> alphabyte ~ 127, unpremultiplied color ~ 255
        uint16_t input_row_data[width * 2] = { 32768, 32768, 32768, 32768 };

        // Output buffer size should be width * (channels+1) = 2 * 2 = 4
        uint8_t output_row_buf[width * 2] = { 0, 0, 0, 0 };

        // Structures
        png_uint_16p input_row = input_row_data;
        png_bytep    output_row = output_row_buf;

        static png_opaque opaque_stub;
        opaque_stub.png_ptr = nullptr;

        static png_image img;
        img.opaque = &opaque_stub;
        img.height = height;
        img.width  = width;
        img.format = PNG_FORMAT_FLAG_ALPHA; // alpha present; color flag off (grayscale) or on as needed

        static png_image_write_control display;
        display.image = &img;
        display.first_row = input_row;
        display.local_row = output_row;
        display.row_bytes = width * 2 * sizeof(png_uint_16); // 2 components per pixel (color + alpha)

        int ret = png_write_image_8bit((void*)&display);

        if (ret != 1) {
            ts.fail("png_write_image_8bit returned non-1 in alpha-path test");
        }

        // Expected output:
        // For grayscale with alpha, output layout per pixel: [color_byte, alpha_byte]
        // color_byte should be 255 for the given inputs (unpremultiply of 32768, 32768)
        // alpha_byte is 127 (32768 / 257)
        const uint8_t exp_row[4] = { 255, 127, 255, 127 };
        if (std::memcmp(output_row_buf, exp_row, 4) != 0) {
            ts.fail("Alpha-path test: output row mismatch");
            std::cout << "Expected: "; print_buffer(exp_row, 4);
            std::cout << "Actual:   "; print_buffer(output_row_buf, 4);
        } else {
            ts.pass("Alpha-path test: output matches expected values");
        }
    }

    // If all tests reported pass, indicate overall success
    ts.pass("png_write_image_8bit unit tests completed (non-fatal on failures).");

    return 0;
}
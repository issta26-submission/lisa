// Test suite for readpng_get_bgcolor in readpng.c
// This test uses a lightweight mock environment to drive the logic
// paths inside readpng_get_bgcolor without depending on a real libpng file.
// It is designed to be compiled with C++11 (no GoogleTest), using a simple
// in-file test harness.

// Note: This code assumes that readpng.c is compiled along with this test.
// The test provides minimal mock implementations of the libpng entry points
// (png_get_valid, png_get_bKGD, png_destroy_read_struct) and exposes the
// needed globals (bit_depth, color_type) as externs so readpng_get_bgcolor
// can read them. The macro/typedefs below are provided to simulate the libpng
// environment sufficiently for unit testing of the core logic.

#include <cstdint>
#include <zlib.h>
#include <iostream>
#include <stdlib.h>
#include <csetjmp>
#include <readpng.h>
#include <stdio.h>
#include <cstring>
#include <png.h>


// Types and macros to simulate libpng environment
using uch = unsigned char;
using ulong = unsigned long;
using ulg = unsigned long;

// Simulated PNG color structure (matches usage in readpng_get_bgcolor)
struct png_color_16_s {
    uint16_t red;
    uint16_t green;
    uint16_t blue;
    uint16_t gray;
};
typedef struct png_color_16_s png_color_16;
typedef png_color_16* png_color_16p;

// Public-ish constants (values are not relied upon by this mock for logic flow)
#define PNG_INFO_bKGD 0x01
#define PNG_COLOR_TYPE_GRAY 0

// Forward declaration of the function under test (to be linked with readpng.c)
extern "C" int readpng_get_bgcolor(uch *red, uch *green, uch *blue);

// Global mocks to drive the behavior of the function under test
static png_color_16 g_bg;        // Background color used by png_get_bKGD
static int g_bKGD_present = 0;    // Whether PNG has a valid bKGD chunk (mock)
extern "C" int bit_depth = 8;      // Extern defined in readpng.c; will be linked
extern "C" int color_type = 0;     // Extern defined in readpng.c; will be linked

// Mock implementations of libpng entry points (to be resolved by the linker)
extern "C" int png_get_valid(void* /*png_ptr*/, void* /*info_ptr*/, int /*option*/) {
    // Always return based on whether bKGD is present in our test harness.
    // readpng_get_bgcolor calls this with PNG_INFO_bKGD; we ignore option value here.
    return g_bKGD_present;
}

extern "C" void png_get_bKGD(void* /*png_ptr*/, void* /*info_ptr*/, png_color_16p* pBackground) {
    // Provide the address of our mock background color
    *pBackground = &g_bg;
}

extern "C" void png_destroy_read_struct(void** /*png_ptr*/, void** /*info_ptr*/, void** /*NULL_ptr*/) {
    // No resources to free in this mock
}

// In libpng usage, readpng.c would call setjmp(png_jmpbuf(png_ptr)).
static jmp_buf g_jmp_buf;
#define png_jmpbuf(p) g_jmp_buf

// Helper: reset mock state before each test
static void reset_mock_state() {
    std::memset(&g_bg, 0, sizeof(g_bg));
    g_bKGD_present = 0;
    bit_depth = 8;
    color_type = PNG_COLOR_TYPE_GRAY; // Treat as gray by default
    // Initialize jmp_buf to a known state (not strictly necessary for this mock)
    // setjmp will return 0 on first invocation
    // (we do not actually call longjmp in tests)
}

// Test 1: When there is no bKGD data, readpng_get_bgcolor should return 1
static bool test_no_bKGD_returns_error() {
    reset_mock_state();
    g_bKGD_present = 0; // simulate no bKGD chunk present

    uch r = 0, g = 0, b = 0;
    int ret = readpng_get_bgcolor(&r, &g, &b);

    bool pass = (ret == 1);
    if (!pass) {
        std::cerr << "[FAIL] test_no_bKGD_returns_error: expected return 1, got " << ret << "\n";
    } else {
        std::cout << "[PASS] test_no_bKGD_returns_error\n";
    }
    return pass;
}

// Test 2: 16-bit depth path: color channels are MSB-truncated (>>8)
static bool test_bit_depth_16_path() {
    reset_mock_state();
    g_bKGD_present = 1;
    bit_depth = 16;
    color_type = 2; // non-gray

    // Set background values
    g_bg.red = 0x1234;
    g_bg.green = 0xABCD;
    g_bg.blue = 0x0F0F;

    uch r=0, g=0, b=0;
    int ret = readpng_get_bgcolor(&r, &g, &b);

    bool pass = (ret == 0) && (r == 0x12) && (g == 0xAB) && (b == 0x0F);
    if (!pass) {
        std::cerr << "[FAIL] test_bit_depth_16_path: got ret=" << ret
                  << ", rgb=(" << (int)r << "," << (int)g << "," << (int)b << ")\n";
    } else {
        std::cout << "[PASS] test_bit_depth_16_path\n";
    }
    return pass;
}

// Test 3: Gray color type with bit_depth 1: grayscale bit-packed
static bool test_gray_bit_depth_1_path() {
    reset_mock_state();
    g_bKGD_present = 1;
    bit_depth = 1;
    color_type = PNG_COLOR_TYPE_GRAY;

    // Gray component is either 0 or 1; resulting color should be 0 or 255 for all channels
    g_bg.gray = 1; // should map to 255
    uch r=0, g=0, b=0;
    int ret = readpng_get_bgcolor(&r, &g, &b);
    bool pass = (ret == 0) && (r == 255) && (g == 255) && (b == 255);

    if (!pass) {
        std::cerr << "[FAIL] test_gray_bit_depth_1_path (gray=1): ret=" << ret
                  << ", rgb=(" << (int)r << "," << (int)g << "," << (int)b << ")\n";
        return false;
    }

    // Change gray to 0, expect 0 for all channels
    reset_mock_state();
    g_bKGD_present = 1;
    bit_depth = 1;
    color_type = PNG_COLOR_TYPE_GRAY;
    g_bg.gray = 0;
    r = g = b = 0;
    ret = readpng_get_bgcolor(&r, &g, &b);
    pass = (ret == 0) && (r == 0) && (g == 0) && (b == 0);
    if (!pass) {
        std::cerr << "[FAIL] test_gray_bit_depth_1_path (gray=0): ret=" << ret
                  << ", rgb=(" << (int)r << "," << (int)g << "," << (int)b << ")\n";
    } else {
        std::cout << "[PASS] test_gray_bit_depth_1_path (gray=0)\n";
    }
    return pass;
}

// Test 4: Gray color type with bit_depth 2: (255/3) * gray
static bool test_gray_bit_depth_2_path() {
    reset_mock_state();
    g_bKGD_present = 1;
    bit_depth = 2;
    color_type = PNG_COLOR_TYPE_GRAY;

    // Example: gray = 1 => (85) since 255/3 = 85
    g_bg.gray = 1;
    uch r=0,g=0,b=0;
    int ret = readpng_get_bgcolor(&r, &g, &b);
    bool pass = (ret == 0) && (r == 85) && (g == 85) && (b == 85);
    if (!pass) {
        std::cerr << "[FAIL] test_gray_bit_depth_2_path (gray=1): ret=" << ret
                  << ", rgb=(" << (int)r << "," << (int)g << "," << (int)b << ")\n";
    } else {
        std::cout << "[PASS] test_gray_bit_depth_2_path (gray=1)\n";
    }

    return pass;
}

// Test 5: Gray color type with bit_depth 4: (255/15) * gray
static bool test_gray_bit_depth_4_path() {
    reset_mock_state();
    g_bKGD_present = 1;
    bit_depth = 4;
    color_type = PNG_COLOR_TYPE_GRAY;

    // 255/15 = 17
    g_bg.gray = 8; // 17*8 = 136
    uch r=0,g=0,b=0;
    int ret = readpng_get_bgcolor(&r, &g, &b);
    bool pass = (ret == 0) && (r == 136) && (g == 136) && (b == 136);
    if (!pass) {
        std::cerr << "[FAIL] test_gray_bit_depth_4_path (gray=8): ret=" << ret
                  << ", rgb=(" << (int)r << "," << (int)g << "," << (int)b << ")\n";
    } else {
        std::cout << "[PASS] test_gray_bit_depth_4_path (gray=8)\n";
    }
    return pass;
}

// Test 6: RGB path (non-gray) should copy raw values without modification
static bool test_rgb_path_copy() {
    reset_mock_state();
    g_bKGD_present = 1;
    bit_depth = 8;
    color_type = 2; // not gray

    g_bg.red = 0x11;
    g_bg.green = 0x22;
    g_bg.blue = 0x33;

    uch r=0, g=0, b=0;
    int ret = readpng_get_bgcolor(&r, &g, &b);
    bool pass = (ret == 0) && (r == 0x11) && (g == 0x22) && (b == 0x33);
    if (!pass) {
        std::cerr << "[FAIL] test_rgb_path_copy: ret=" << ret
                  << ", rgb=(" << (int)r << "," << (int)g << "," << (int)b << ")\n";
    } else {
        std::cout << "[PASS] test_rgb_path_copy\n";
    }
    return pass;
}

// Simple test runner
int main() {
    // Run all tests and report results
    int total = 0;
    int failures = 0;

    auto run = [&](const char* name, bool (*fn)()) {
        ++total;
        bool ok = fn();
        if (!ok) ++failures;
        // Each test prints its own PASS/FAIL lines
    };

    // Execute tests with explanatory commentary in code
    run("test_no_bKGD_returns_error", test_no_bKGD_returns_error);
    run("test_bit_depth_16_path", test_bit_depth_16_path);
    run("test_gray_bit_depth_1_path", test_gray_bit_depth_1_path);
    run("test_gray_bit_depth_2_path", test_gray_bit_depth_2_path);
    run("test_gray_bit_depth_4_path", test_gray_bit_depth_4_path);
    run("test_rgb_path_copy", test_rgb_path_copy);

    std::cout << "\nSummary: " << (total - failures) << "/" << total << " tests PASSED.";
    if (failures > 0) {
        std::cout << " (" << failures << " failed)";
    }
    std::cout << std::endl;
    return failures;
}
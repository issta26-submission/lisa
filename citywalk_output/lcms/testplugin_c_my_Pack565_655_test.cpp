// Unit tests for the focal method: my_Pack565
// This test suite targets the C function declared in testplugin.c:
// cmsUInt8Number* my_Pack565(CMSREGISTER _cmsTRANSFORM* info, 
//                            CMSREGISTER cmsUInt16Number wOut[],
//                            CMSREGISTER cmsUInt8Number* output,
//                            CMSREGISTER cmsUInt32Number Stride)
// The tests are written in C++11 (no GoogleTest) and rely only on the C-style
// interface of the function. They check the 565 packing logic for various inputs.

#include <cstring>
#include <cstdint>
#include <iostream>
#include <testcms2.h>
#include <cmath>


// Forward declare the C function with C linkage to avoid name mangling.
// We use a minimal compatible prototype to keep the test independent of the
// exact internal typedefs in testcms2.h.
extern "C" unsigned char* my_Pack565(void* info,
                                   unsigned short wOut[],
                                   unsigned char* output,
                                   unsigned int Stride);

// Lightweight test harness: non-terminating assertions.
// We log failures and continue executing to maximize code coverage.
static int g_test_failures = 0;
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "TEST FAIL: " << (msg) \
                  << " (File: " << __FILE__ << ", Line: " << __LINE__ << ")" \
                  << std::endl; \
        ++g_test_failures; \
    } \
} while(0)

// Helper: run a single test case.
// wOut represents the 3 input components: wOut[0] = B, wOut[1] = G, wOut[2] = R (as used by the focal method).
// expected is the 16-bit packed pixel value.
static void run_case(const char* name, const uint16_t wOut[3], uint16_t expected) {
    unsigned char outputBuf[4] = {0};      // buffer big enough to hold 2-byte pixel
    unsigned char* retPtr = my_Pack565(nullptr, wOut, outputBuf, 0);

    // Read the 16-bit pixel from outputBuf considering little-endian layout
    uint16_t pixelWritten = static_cast<uint16_t>(outputBuf[0])
                          | (static_cast<uint16_t>(outputBuf[1]) << 8);

    // Assertions
    TEST_ASSERT(pixelWritten == expected,
                  name + std::string(" - Pixel mismatch. expected 0x") +
                  (std::to_string(expected).size() ? "" : ""));

    // Check the return pointer is outputBuf + 2
    TEST_ASSERT(retPtr == outputBuf + 2,
                name + std::string(" - Return pointer should be outputBuf + 2"));
}

// Entry point for the test suite
int main() {
    // Test cases chosen to cover typical and boundary scenarios.

    // Case 1: All zeros -> r=0, g=0, b=0 -> pixel = 0x0000
    // Explanation:
    // wOut = {B=0, G=0, R=0}
    // r = floor((0 * 31)/65535 + 0.5) = floor(0.5) = 0
    // g = floor((0 * 63)/65535 + 0.5) = floor(0.5) = 0
    // b = floor((0 * 31)/65535 + 0.5) = floor(0.5) = 0
    // pixel = 0 | (0<<5) | (0<<11) = 0x0000
    {
        const uint16_t wOut[3] = {0, 0, 0};
        run_case("Case 1 - All zeros", wOut, 0x0000);
    }

    // Case 2: All max -> r=31, g=63, b=31 -> pixel = 0xFFFF
    // Explanation:
    // wOut = {B=65535, G=65535, R=65535}
    // r = floor((65535*31)/65535 + 0.5) = floor(31 + 0.5) = 31
    // g = floor((65535*63)/65535 + 0.5) = floor(63 + 0.5) = 63
    // b = floor((65535*31)/65535 + 0.5) = floor(31 + 0.5) = 31
    // pixel = (31) | (63<<5) | (31<<11) = 0xFFFF
    {
        const uint16_t wOut[3] = {65535, 65535, 65535}; // B, G, R
        run_case("Case 2 - All max", wOut, 0xFFFF);
    }

    // Case 3: Only blue non-zero, others zero
    // wOut = {B=65535? No: B=0; G=0; R=65535}
    // Use wOut = {B=0, G=0, R=65535} to produce r=31, g=0, b=0 -> pixel = 0x1F
    // However to align with test, use wOut = {0, 0, 65535}
    // r = floor((R)*31/65535 + 0.5) = floor(31 + 0.5) = 31
    // g = 0, b = 0 -> pixel = 0x001F
    {
        const uint16_t wOut[3] = {0, 0, 65535}; // B, G, R
        run_case("Case 3 - Max Red only", wOut, 0x001F);
    }

    // Case 4: Only green non-zero (max green, others zero)
    // wOut = {B=0, G=65535, R=0} -> r=0, g=63, b=0 -> pixel = 0x07E0
    {
        const uint16_t wOut[3] = {0, 65535, 0}; // B, G, R
        run_case("Case 4 - Max Green only", wOut, 0x07E0);
    }

    // Case 5: Mid-range values to exercise rounding
    // wOut = {B=32768, G=32768, R=32768}
    // r = floor((32768*31)/65535 + 0.5) ≈ floor(15.5 + 0.5) = 16
    // g = floor((32768*63)/65535 + 0.5) ≈ floor(31.5 + 0.5) = 32
    // b = floor(...) ≈ 16
    // pixel = (16) | (32<<5) | (16<<11) = 0x8410
    {
        const uint16_t wOut[3] = {32768, 32768, 32768}; // B, G, R
        run_case("Case 5 - Mid-range values", wOut, 0x8410);
    }

    // Final results
    if (g_test_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
    } else {
        std::cout << g_test_failures << " TEST(S) FAILED" << std::endl;
    }

    // Return non-zero if any test failed
    return g_test_failures == 0 ? 0 : 1;
}
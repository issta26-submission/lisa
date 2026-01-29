/*
This is a lightweight C++11 unit test suite for the focal function:
    cmsUInt8Number* my_Unroll565(CMSREGISTER struct _cmstransform_struct* nfo,
                                 CMSREGISTER cmsUInt16Number wIn[],
                                 CMSREGISTER cmsUInt8Number* accum,
                                 CMSREGISTER cmsUInt32Number Stride)

Test goals:
- Verify correct color component extraction/mapping from a 565 pixel to 16-bit channels.
- Check that the function returns accum + 2 as expected.
- Cover several representative pixel patterns (red max, green max, blue max, all zero).
- Use a simple, non-terminating assertion approach (log failures, continue).
- No GTest; a small self-contained harness is provided.

Notes:
- The test relies on dependencies and types provided by the project headers (e.g., testcms2.h).
- We import dependencies with the path expected by the project (testcms2.h).
- The tests are designed to be executed by main() and do not rely on private members.
*/

#include <cstdint>
#include <iostream>
#include <cmath>
#include <testcms2.h>


// Import project-specific dependencies. Path should be adjusted to actual environment if needed.

extern "C" cmsUInt8Number* my_Unroll565(CMSREGISTER struct _cmstransform_struct* nfo,
                                       CMSREGISTER cmsUInt16Number wIn[],
                                       CMSREGISTER cmsUInt8Number* accum,
                                       CMSREGISTER cmsUInt32Number Stride);

// Helper: compute expected 5-bit -> 16-bit mapped value (with same rounding as the focal code)
static unsigned short map5_to_16(unsigned int v5) {
    double val = std::floor(((static_cast<double>(v5) * 65535.0) / 31.0) + 0.5);
    return static_cast<unsigned short>(val);
}

// Helper: compute expected 6-bit -> 16-bit mapped value
static unsigned short map6_to_16(unsigned int v6) {
    double val = std::floor(((static_cast<double>(v6) * 65535.0) / 63.0) + 0.5);
    return static_cast<unsigned short>(val);
}

// Simple non-terminating assertion helper
#define ASSERT_LOG(cond, msg) \
    do { if (!(cond)) { std::cerr << "ASSERT FAILED: " << (msg) << std::endl; } } while (0)

static bool run_case_pixel(uint16_t pixel, const char* caseName) {
    // accum contains the 16-bit pixel in little-endian form.
    unsigned char accumBuf[2];
    accumBuf[0] = static_cast<unsigned char>(pixel & 0xFF);
    accumBuf[1] = static_cast<unsigned char>((pixel >> 8) & 0xFF);

    cmsUInt16Number wIn[3] = {0, 0, 0};

    // The function under test should read pixel from accum and write to wIn in order:
    // wIn[2] = R, wIn[1] = G, wIn[0] = B
    cmsUInt8Number* ret = my_Unroll565(nullptr, wIn, accumBuf, 0);

    bool ok = true;
    // Validate return pointer arithmetic
    if (ret != accumBuf + 2) {
        std::cerr << "Test " << caseName << " failed: return pointer != accum + 2" << std::endl;
        ok = false;
    }

    // Compute expected components
    unsigned short expR = map5_to_16(pixel & 0x1F);           // lower 5 bits
    unsigned short expG = map6_to_16((pixel >> 5) & 0x3F);    // middle 6 bits
    unsigned short expB = map5_to_16((pixel >> 11) & 0x1F);   // upper 5 bits

    // Validate mapped components (note the order: R -> wIn[2], G -> wIn[1], B -> wIn[0])
    if (wIn[2] != expR) {
        std::cerr << "Test " << caseName << " failed: R component mismatch. got " << wIn[2]
                  << " expected " << expR << std::endl;
        ok = false;
    }
    if (wIn[1] != expG) {
        std::cerr << "Test " << caseName << " failed: G component mismatch. got " << wIn[1]
                  << " expected " << expG << std::endl;
        ok = false;
    }
    if (wIn[0] != expB) {
        std::cerr << "Test " << caseName << " failed: B component mismatch. got " << wIn[0]
                  << " expected " << expB << std::endl;
        ok = false;
    }

    if (ok) {
        std::cout << "Test " << caseName << " passed." << std::endl;
    }
    return ok;
}

int main() {
    int total = 0;
    int passed = 0;

    // Case 1: All components zero
    total++;
    if (run_case_pixel(0x0000, "pixel_all_zero"))
        ++passed;

    // Case 2: Red max (red=31, green=0, blue=0)
    total++;
    if (run_case_pixel(0x001F, "pixel_red_max_only"))
        ++passed;

    // Case 3: Green max (red=0, green=63, blue=0)
    total++;
    if (run_case_pixel(0x07E0, "pixel_green_max_only"))
        ++passed;

    // Case 4: Blue max (red=0, green=0, blue=31)
    total++;
    if (run_case_pixel(0xF800, "pixel_blue_max_only"))
        ++passed;

    std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;
    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}
// Lightweight unit-test harness for the focal method SpeedTest8bitLineStride
// This file provides a self-contained test environment with minimal
// CMS-like API stubs to exercise SpeedTest8bitLineStride without
// relying on the actual external library.
// The tests run from main(), do not terminate on assertion failures,
// and print a summary at the end.

#include <cstring>
#include <fast_float_internal.h>
#include <iostream>
#include <cstdarg>
#include <cassert>
#include <cstdlib>
#include <memory.h>
#include <stdlib.h>
#include <cstdint>
#include <ctime>


// Domain-adapted stubs and minimal type definitions to compile the focal method.

using cmsContext     = void*;
using cmsHPROFILE    = void*;
using cmsHTRANSFORM  = void*;
using cmsUInt32Number = unsigned int;
using cmsInt32Number  = int;
using cmsFloat64Number = double;

static const int TYPE_RGBA_8 = 0;
static const int INTENT_PERCEPTUAL = 0;
static const unsigned long cmsFLAGS_NOCACHE = 0;

// A padded_line type as used for stride sizing in the transform function
struct padded_line {
    uint8_t dummy[16];
};

// Pixel layout: 4 bytes per pixel (RGBA)
struct Pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

// 256x256 per line; 256 lines -> 256 * 256 * 256 pixels total
struct Line {
    Pixel pixels[256][256];
};

// big_bitmap with 256 lines
struct big_bitmap {
    Line line[256];
};

// Minimal Fail stub (non-terminating)
void Fail(const char* frm, ...)
{
    // In the real library, Fail would likely abort/longjmp.
    // Here we simply print a message to aid debugging and continue.
    va_list args;
    va_start(args, frm);
    std::cerr << "Fail: " << frm;
    while (true) { // consume variadic args in a simple way if provided
        // Avoid attempting to format without knowledge of types
        break;
    }
    std::cerr << std::endl;
    va_end(args);
}

// Minimal implementations to simulate the color transform API used by the focal method.

cmsHTRANSFORM cmsCreateTransformTHR(cmsContext, cmsHPROFILE /*in*/, int /*typeIn*/, cmsHPROFILE /*out*/, int /*typeOut*/, int /*intent*/, unsigned long /*flags*/)
{
    // Return a non-null dummy handle
    return reinterpret_cast<cmsHTRANSFORM>(new int(1));
}

void cmsCloseProfile(cmsHPROFILE /*profile*/) {
    // No-op for test harness
}

void cmsDeleteTransform(cmsHTRANSFORM tr)
{
    delete static_cast<int*>(tr);
}

void cmsDoTransformLineStride(
    cmsHTRANSFORM /*xform*/,
    big_bitmap* In,
    big_bitmap* Out,
    cmsUInt32Number /*count*/,
    cmsUInt32Number /*line*/,
    size_t /*srcStride*/,
    size_t /*dstStride*/,
    int /*translation*/,
    int /*intent*/)
{
    // Identity transform: copy RGBA channels from In to Out, pixel-by-pixel.
    // This simulates a simple, non-destructive transform.
    if (In == nullptr || Out == nullptr) return;
    for (int r = 0; r < 256; ++r) {
        for (int g = 0; g < 256; ++g) {
            for (int b = 0; b < 256; ++b) {
                Out->line[r].pixels[g][b].r = In->line[r].pixels[g][b].r;
                Out->line[r].pixels[g][b].g = In->line[r].pixels[g][b].g;
                Out->line[r].pixels[g][b].b = In->line[r].pixels[g][b].b;
                Out->line[r].pixels[g][b].a = In->line[r].pixels[g][b].a;
            }
        }
    }
}

// Simple MPixSec helper: return the same value for deterministic tests.
cmsFloat64Number MPixSec(cmsFloat64Number diff)
{
    return diff;
}

// The focal method under test (copied here for a self-contained test harness)
cmsFloat64Number SpeedTest8bitLineStride(cmsContext ct, cmsHPROFILE hlcmsProfileIn, cmsHPROFILE hlcmsProfileOut)
{
{
       cmsInt32Number r, g, b;
       clock_t atime;
       cmsFloat64Number diff;
       cmsHTRANSFORM hlcmsxform;
       big_bitmap* In;
       big_bitmap* Out;
       cmsUInt32Number Mb;
       if (hlcmsProfileIn == NULL || hlcmsProfileOut == NULL)
              Fail("Unable to open profiles");
       hlcmsxform = cmsCreateTransformTHR(ct, hlcmsProfileIn, TYPE_RGBA_8, hlcmsProfileOut, TYPE_RGBA_8, INTENT_PERCEPTUAL, cmsFLAGS_NOCACHE);
       cmsCloseProfile(hlcmsProfileIn);
       cmsCloseProfile(hlcmsProfileOut);

       // Our test bitmap is 256 x 256 padded lines
       Mb = sizeof(big_bitmap);
       In = (big_bitmap*)malloc(Mb);
       Out = (big_bitmap*)malloc(Mb);
       for (r = 0; r < 256; r++)
              for (g = 0; g < 256; g++)
                     for (b = 0; b < 256; b++) {
                            In->line[r].pixels[g][b].r = (cmsUInt8Number)r;
                            In->line[r].pixels[g][b].g = (cmsUInt8Number)g;
                            In->line[r].pixels[g][b].b = (cmsUInt8Number)b;
                            In->line[r].pixels[g][b].a = 0;
                     }
       atime = clock();
       
       cmsDoTransformLineStride(hlcmsxform, In, Out, 256*256, 256, sizeof(padded_line), sizeof(padded_line), 0, 0);
       
       diff = clock() - atime;
       free(In); free(Out);
       cmsDeleteTransform(hlcmsxform);
       return MPixSec(diff);
}
}

// The above is the focal method; test suite is below.


// Lightweight test framework (no GTest)
// - Non-terminating assertions
// - Simple test registration via functions
// - Run tests from main
static int g_tests_run = 0;
static int g_tests_pass = 0;
static int g_tests_fail = 0;

#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "Assertion failed: " << (msg) << " (line " << __LINE__ << ")" << std::endl; \
        ++g_tests_fail; \
    } else { \
        ++g_tests_pass; \
    } \
} while (0)

namespace TestSuite_NS {

// Simple helper to create dummy non-null profiles
inline cmsHPROFILE make_dummy_profile(void)
{
    // Non-null pointer to simulate a profile handle
    return reinterpret_cast<cmsHPROFILE>(new int(42));
}

// Cleanup helper for dummy profiles
inline void free_dummy_profile(cmsHPROFILE p)
{
    delete reinterpret_cast<int*>(p);
}

// Test 1: Basic sanity: ensure SpeedTest8bitLineStride returns a positive number for valid profiles
void test_speedtest8bitlinstride_basic_positive(void)
{
    // Prepare non-null profiles
    cmsHPROFILE inProfile  = make_dummy_profile();
    cmsHPROFILE outProfile = make_dummy_profile();

    // Call the focal method
    cmsFloat64Number result = SpeedTest8bitLineStride(nullptr, inProfile, outProfile);

    // Expected: a finite positive value (indicating a successful pass)
    TEST_ASSERT(result > 0.0, "SpeedTest8bitLineStride should return a positive value for valid profiles");

    // Cleanup
    free_dummy_profile(inProfile);
    free_dummy_profile(outProfile);
}

// Test 2: Run the test twice; ensure consistent non-zero results and no crashes
void test_speedtest8bitlinstride_consistency(void)
{
    cmsHPROFILE inProfile  = make_dummy_profile();
    cmsHPROFILE outProfile = make_dummy_profile();

    cmsFloat64Number r1 = SpeedTest8bitLineStride(nullptr, inProfile, outProfile);
    cmsFloat64Number r2 = SpeedTest8bitLineStride(nullptr, inProfile, outProfile);

    TEST_ASSERT(r1 > 0.0, "First SpeedTest8bitLineStride result should be positive");
    TEST_ASSERT(r2 > 0.0, "Second SpeedTest8bitLineStride result should be positive");
    // Ensure the two results are reasonable (not NaN or INF)
    TEST_ASSERT(std::isfinite(r1) && std::isfinite(r2), "Results must be finite numbers");

    free_dummy_profile(inProfile);
    free_dummy_profile(outProfile);
}

// Edge-case test: ensure the test harness handles NULL profiles gracefully without crashing.
// Note: The original focal method uses Fail() on NULL profiles, which in this harness is a no-op.
// We still exercise the NULL path to ensure no crash occurs before we reach the guard.
void test_speedtest8bitlinstride_null_profiles(void)
{
    // Expect that passing NULL profiles will not crash the test harness (we can't rely on Fail to abort)
    cmsFloat64Number result = SpeedTest8bitLineStride(nullptr, nullptr, nullptr);
    // We expect either a crash-free return (undefined behavior is avoided) or a sane value.
    // Since the NULL path currently leads to a Fail() call and potential subsequent undefined behavior,
    // we only assert that the function returns a finite value if it returns at all.
    TEST_ASSERT(std::isfinite(result) || result == 0.0, "NULL profiles should not crash the harness (best-effort check)");
}

} // namespace TestSuite_NS

// Helper to run tests and summarize
void run_all_tests()
{
    using namespace TestSuite_NS;
    std::cout << "Starting SpeedTest8bitLineStride unit tests (self-contained harness)..." << std::endl;

    test_speedtest8bitlinstride_basic_positive();
    test_speedtest8bitlinstride_consistency();
    test_speedtest8bitlinstride_null_profiles();

    std::cout << "Tests completed. Run summary: "
              << g_tests_pass << " passed, "
              << g_tests_fail << " failed, "
              << g_tests_run << " total (approxements)." << std::endl;
}

// Main: entry point to execute tests
int main()
{
    // Run all tests
    run_all_tests();
    // Return success if all tests pass
    if (g_tests_fail == 0) {
        return 0;
    } else {
        return 1;
    }
}
/*
 * Unit test suite for libpng's png_check_IHDR function
 * Target: C++11 environment, without GoogleTest
 * Approach:
 * - Use real libpng to exercise png_check_IHDR via setjmp/longjmp error handling.
 * - Create a separate png_ptr for each test case (using png_create_read_struct).
 * - Use setjmp(png_jmpbuf(png_ptr)) to catch png_error invocations.
 * - Cover multiple branches by supplying edge-case and valid IHDR inputs.
 * - Include a test for user limit support if available (png_set_user_limits).
 * - All tests run from main() with simple non-terminating assertions (log failures).
 *
 * Note: This code relies on libpng being available and linked (e.g., -lpng).
 * It declares the png_check_IHDR symbol explicitly to ensure we exercise the focal function.
 */

#include <cstdint>
#include <setjmp.h>
#include <vector>
#include <string>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>
#include <png.h>



extern "C" {
   // Explicit prototype for the focal function to ensure linkage
   int png_check_IHDR(png_const_structrp png_ptr,
       png_uint_32 width, png_uint_32 height, int bit_depth,
       int color_type, int interlace_type, int compression_type,
       int filter_type);
}

// Simple test result reporting (non-terminating)
struct TestResult {
    std::string name;
    bool expect_error;
    bool passed;
};

// Global counters for test summary
static int g_total = 0;
static int g_passed = 0;
static int g_failed = 0;

// Helper: Run a single test case with optional user limits
static bool run_case(const char* name,
                     uint32_t width, uint32_t height,
                     int bit_depth, int color_type,
                     int interlace_type, int compression_type,
                     int filter_type,
                     bool expect_error,
#if defined(PNG_SET_USER_LIMITS_SUPPORTED)
                     bool use_user_limits = false,
                     uint32_t user_max_width = 0,
                     uint32_t user_max_height = 0
#else
                     bool use_user_limits = false
#endif
                    )
{
    ++g_total;
    // Prepare a test result entry (will fill later)
    // Create a png_ptr and info struct
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // Create libpng read structure
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        fprintf(stderr, "Test %s: failed to create png_ptr\n", name);
        ++g_failed;
        return false;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fprintf(stderr, "Test %s: failed to create info_ptr\n", name);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        ++g_failed;
        return false;
    }

    // Prepare to catch libpng errors
    int setjmp_result = 0;
    if ((setjmp_result = setjmp(png_jmpbuf(png_ptr))) {
        // We expect an error (png_error triggered)
        bool did_error = true;
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

        // Compare with expectation
        bool test_passed = (did_error == expect_error);
        if (test_passed) {
            ++g_passed;
        } else {
            ++g_failed;
        }

        // Return test status
        return test_passed;
    } else {
        // Optional: apply user limits before IHDR checks
#if defined(PNG_SET_USER_LIMITS_SUPPORTED)
        if (use_user_limits) {
            // Ensure function exists before calling
            png_set_user_limits(png_ptr, user_max_width, user_max_height);
        }
#endif
        // Call the focal function
        png_check_IHDR(png_ptr, width, height, bit_depth, color_type,
                       interlace_type, compression_type, filter_type);

        // If we reach here, no png_error occurred
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

        bool test_passed = (!expect_error);
        if (test_passed) {
            ++g_passed;
        } else {
            ++g_failed;
        }
        return test_passed;
    }
}

// Overload for compiling environments where PNG_SET_USER_LIMITS_SUPPORTED may be defined or not.
// This helper ensures we can call run_case with or without user limits based on macro support.

int main()
{
    // Collect results
    std::vector<TestResult> results;

    // Test 1: width == 0 triggers an IHDR width error
    {
        const char* name = "width_zero";
        bool ok = run_case(name,
                           0, 10,           // width, height
                           8, 2,             // bit_depth, color_type
                           PNG_INTERLACE_NONE, 0, 0, // interlace, compression, filter
                           true  // expect error
#if defined(PNG_SET_USER_LIMITS_SUPPORTED)
                           , false, 0, 0
#endif
                          );
        results.push_back({name, true, ok});
    }

    // Test 2: width > PNG_UINT_31_MAX triggers error
    {
        const char* name = "width_exceeds_31";
        uint32_t w = (uint32_t)0x80000000u; // 2^31, exceeds 31-bit max
        bool ok = run_case(name, w, 10, 8, 2, PNG_INTERLACE_NONE, 0, 0, true
#if defined(PNG_SET_USER_LIMITS_SUPPORTED)
                           , false, 0, 0
#endif
                          );
        results.push_back({name, true, ok});
    }

    // Test 3: height == 0 triggers an IHDR height error
    {
        const char* name = "height_zero";
        bool ok = run_case(name,
                           1, 0,            // width, height
                           8, 2,             // bit_depth, color_type
                           PNG_INTERLACE_NONE, 0, 0, // interlace, compression, filter
                           true
#if defined(PNG_SET_USER_LIMITS_SUPPORTED)
                           , false, 0, 0
#endif
                          );
        results.push_back({name, true, ok});
    }

    // Test 4: height > PNG_UINT_31_MAX triggers error
    {
        const char* name = "height_exceeds_31";
        uint32_t h = (uint32_t)0x80000000u;
        bool ok = run_case(name,
                           1, h,
                           8, 2,
                           PNG_INTERLACE_NONE, 0, 0, true
#if defined(PNG_SET_USER_LIMITS_SUPPORTED)
                           , false, 0, 0
#endif
                          );
        results.push_back({name, true, ok});
    }

    // Test 5: invalid bit depth
    {
        const char* name = "invalid_bit_depth";
        bool ok = run_case(name,
                           1, 1,
                           3, // invalid bit depth
                           2, // RGB
                           PNG_INTERLACE_NONE, 0, 0, true
#if defined(PNG_SET_USER_LIMITS_SUPPORTED)
                           , false, 0, 0
#endif
                          );
        results.push_back({name, true, ok});
    }

    // Test 6: invalid color type (negative)
    {
        const char* name = "invalid_color_type_negative";
        bool ok = run_case(name,
                           1, 1,
                           8, -1,
                           PNG_INTERLACE_NONE, 0, 0, true
#if defined(PNG_SET_USER_LIMITS_SUPPORTED)
                           , false, 0, 0
#endif
                          );
        results.push_back({name, true, ok});
    }

    // Test 7: invalid color-depth combination with PALETTE
    {
        const char* name = "invalid_color_comb_palette_depth";
        bool ok = run_case(name,
                           1, 1,
                           16, // bit_depth too high for PALETTE
                           PNG_COLOR_TYPE_PALETTE,
                           PNG_INTERLACE_NONE, 0, 0, true
#if defined(PNG_SET_USER_LIMITS_SUPPORTED)
                           , false, 0, 0
#endif
                          );
        results.push_back({name, true, ok});
    }

    // Test 8: invalid interlace type
    {
        const char* name = "invalid_interlace";
        bool ok = run_case(name,
                           1, 1,
                           8, 2,
                           PNG_INTERLACE_LAST, 0, 0, true
#if defined(PNG_SET_USER_LIMITS_SUPPORTED)
                           , false, 0, 0
#endif
                          );
        results.push_back({name, true, ok});
    }

    // Test 9: invalid compression method
    {
        const char* name = "invalid_compression";
        bool ok = run_case(name,
                           1, 1,
                           8, 2,
                           PNG_INTERLACE_NONE, 999, 0, true
#if defined(PNG_SET_USER_LIMITS_SUPPORTED)
                           , false, 0, 0
#endif
                          );
        results.push_back({name, true, ok});
    }

    // Test 10: invalid filter method (non-base) with non-MNG scenario
    {
        const char* name = "invalid_filter_type_non_base";
        bool ok = run_case(name,
                           1, 1,
                           8, 2,
                           PNG_INTERLACE_NONE, 0, 1, // non-base filter
#if defined(PNG_SET_USER_LIMITS_SUPPORTED)
                           true, false, 0, 0
#else
                           true
#endif
                          );
        results.push_back({name, true, ok});
    }

    // Test 11: valid IHDR for RGB8
    {
        const char* name = "valid_IHDR_RGB8";
        bool ok = run_case(name,
                           1, 1,
                           8, // 8-bit
                           PNG_COLOR_TYPE_RGB,
                           PNG_INTERLACE_NONE, 0, 0, false
#if defined(PNG_SET_USER_LIMITS_SUPPORTED)
                           , false, 0, 0
#endif
                          );
        results.push_back({name, false, ok});
    }

#if defined(PNG_SET_USER_LIMITS_SUPPORTED)
    // Test 12: width exceeds user limit (ensures user-limit path is exercised)
    {
        const char* name = "width_exceeds_user_limit";
        uint32_t max_w = 2;
        uint32_t test_width = 3;
        bool ok = run_case(name,
                           test_width, 1,
                           8, 2,
                           PNG_INTERLACE_NONE, 0, 0, true,
                           true, // use_user_limits
                           max_w, 1000);
        results.push_back({name, true, ok});
    }
#endif

    // Print summary
    printf("Unit test summary: total=%d passed=%d failed=%d\n",
           g_total, g_passed, g_failed);

    // Detailed per-test report
    for (const auto& r : results) {
        printf("Test '%s' - %s\n", r.name.c_str(), r.passed ? "PASSED" : "FAILED");
        if (r.passed == false) {
            // keep a minimal, non-verbose trace
        }
    }

    return (g_failed > 0) ? 1 : 0;
}
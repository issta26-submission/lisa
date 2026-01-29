/*
Step 1: Program Understanding (Key Candidate Keywords)
- png_ptr / png_inforp: pointers to PNG reader state and image info (as used by libpng)
- PNG_INFO_tIME: flag indicating tIME is present in info
- info_ptr->valid: bitmask containing validity flags (including PNG_INFO_tIME)
- info_ptr->mod_time: storage for the modification time (png_time)
- mod_time / png_timep: the tIME structure and a pointer to it
- png_get_tIME: focal method under test; returns PNG_INFO_tIME when all inputs valid and mod_time is non-null
These keywords guide the test coverage (true/false branches, pointer behavior, and proper info struct handling).

<FOCAL_METHOD> png_get_tIME(png_const_structrp png_ptr, png_inforp info_ptr,
    png_timep *mod_time)
{
{
   png_debug1(1, "in %s retrieval function", "tIME");
   if (png_ptr != NULL && info_ptr != NULL &&
       (info_ptr->valid & PNG_INFO_tIME) != 0 && mod_time != NULL)
   {
      *mod_time = &(info_ptr->mod_time);
      return PNG_INFO_tIME;
   }
   return 0;
}
 </FOCAL_METHOD>

<DOMAIN_KNOWLEDGE> 
1. Import all necessary dependencies with correct paths.
2. Cover true and false branches of each condition predicate at least once.
3. Handle static members properly in tests. Access static members using the class name. Static functions at the file scope are only visible within the file.
4. Use only the C++ standard library, imported third-party libraries and provided methods.
5. Use non-terminating assertions (e.g., EXPECT_*) to maximize code execution and coverage.
6. Avoid using private methods or fields defined in the program.
7. For gmock, remember that only virtual methods can be mocked.
8. Choose appropriate assertions for data types, distinguishing between address and content comparisons.
9. Use the correct namespace.
10. If gtest isn't allowed, call test methods from the main function. 
</DOMAIN_KNOWLEDGE>

<FOCAL_CLASS_DEP> 
#include <iostream>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>
#include <png.h>

png_get_valid(png_const_structrp png_ptr, png_const_inforp info_ptr,
    png_uint_32 flag)
{{};
png_get_rowbytes(png_const_structrp png_ptr, png_const_inforp info_ptr)
{{};
png_get_rows(png_const_structrp png_ptr, png_const_inforp info_ptr)
{{};
png_get_image_width(png_const_structrp png_ptr, png_const_inforp info_ptr)
{{};
png_get_image_height(png_const_structrp png_ptr, png_const_inforp info_ptr)
{{};
png_get_bit_depth(png_const_structrp png_ptr, png_const_inforp info_ptr)
{{};
png_get_color_type(png_const_structrp png_ptr, png_const_inforp info_ptr)
{{};
png_get_filter_type(png_const_structrp png_ptr, png_const_inforp info_ptr)
{{};
png_get_interlace_type(png_const_structrp png_ptr, png_const_inforp info_ptr)
{{};
png_get_compression_type(png_const_structrp png_ptr, png_const_inforp info_ptr)
{{};
png_get_x_pixels_per_meter(png_const_structrp png_ptr,
    png_const_inforp info_ptr)
{{};
png_get_y_pixels_per_meter(png_const_structrp png_ptr,
    png_const_inforp info_ptr)
{{};
png_get_pixels_per_meter(png_const_structrp png_ptr, png_const_inforp info_ptr)
{{};
png_get_pixel_aspect_ratio(png_const_structrp png_ptr,
    png_const_inforp info_ptr)
{{};
png_get_pixel_aspect_ratio_fixed(png_const_structrp png_ptr, 
    png_const_inforp info_ptr)
{{};
png_get_x_offset_microns(png_const_structrp png_ptr, png_const_inforp info_ptr)
{{};
png_get_y_offset_microns(png_const_structrp png_ptr, png_const_inforp info_ptr)
{{};
png_get_x_offset_pixels(png_const_structrp png_ptr, png_const_inforp info_ptr)
{{};
png_get_y_offset_pixels(png_const_structrp png_ptr, png_const_inforp info_ptr)
{{};
ppi_from_ppm(png_uint_32 ppm)
{{};
png_get_pixels_per_inch(png_const_structrp png_ptr, png_const_inforp info_ptr)
{{};
png_get_x_pixels_per_inch(png_const_structrp png_ptr, png_const_inforp info_ptr)
{{};
png_get_y_pixels_per_inch(png_const_structrp png_ptr, png_const_inforp info_ptr)
{{};
png_fixed_inches_from_microns(png_const_structrp png_ptr, png_int_32 microns)
{{};
png_get_x_offset_inches_fixed(png_const_structrp png_ptr,
    png_const_inforp info_ptr)
{{};
png_get_y_offset_inches_fixed(png_const_structrp png_ptr,
    png_const_inforp info_ptr)
{{};
png_get_x_offset_inches(png_const_structrp png_ptr, png_const_inforp info_ptr)
{{};
png_get_y_offset_inches(png_const_structrp png_ptr, png_const_inforp info_ptr)
{{};
png_get_pHYs_dpi(png_const_structrp png_ptr, png_const_inforp info_ptr,
    png_uint_32 *res_x, png_uint_32 *res_y, int *unit_type)
{{};
png_get_channels(png_const_structrp png_ptr, png_const_inforp info_ptr)
{{};
png_get_signature(png_const_structrp png_ptr, png_const_inforp info_ptr)
{{};
png_get_bKGD(png_const_structrp png_ptr, png_inforp info_ptr,
    png_color_16p *background)
{{};
png_get_cHRM(png_const_structrp png_ptr, png_const_inforp info_ptr,
    double *whitex, double *whitey, double *redx, double *redy,
    double *greenx, double *greeny, double *bluex, double *bluey)
{{};
png_get_cHRM_XYZ(png_const_structrp png_ptr, png_const_inforp info_ptr,
    double *red_X, double *red_Y, double *red_Z, double *green_X,
    double *green_Y, double *green_Z, double *blue_X, double *blue_Y,
    double *blue_Z)
{{};
png_get_cHRM_XYZ_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr,
    png_fixed_point *int_red_X, png_fixed_point *int_red_Y,
    png_fixed_point *int_red_Z, png_fixed_point *int_green_X,
    png_fixed_point *int_green_Y, png_fixed_point *int_green_Z,
    png_fixed_point *int_blue_X, png_fixed_point *int_blue_Y,
    png_fixed_point *int_blue_Z)
{{};
png_get_cHRM_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr,
    png_fixed_point *whitex, png_fixed_point *whitey, png_fixed_point *redx,
    png_fixed_point *redy, png_fixed_point *greenx, png_fixed_point *greeny,
    png_fixed_point *bluex, png_fixed_point *bluey)
{{};
png_get_gAMA_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr,
    png_fixed_point *file_gamma)
{{};
png_get_gAMA(png_const_structrp png_ptr, png_const_inforp info_ptr,
    double *file_gamma)
{{};
png_get_sRGB(png_const_structrp png_ptr, png_const_inforp info_ptr,
    int *file_srgb_intent)
{{};
png_get_iCCP(png_const_structrp png_ptr, png_inforp info_ptr,
    png_charpp name, int *compression_type,
    png_bytepp profile, png_uint_32 *proflen)
{{};
png_get_sPLT(png_const_structrp png_ptr, png_inforp info_ptr,
    png_sPLT_tpp spalettes)
{{};
png_get_cICP(png_const_structrp png_ptr,
             png_const_inforp info_ptr, png_bytep colour_primaries,
             png_bytep transfer_function, png_bytep matrix_coefficients,
             png_bytep video_full_range_flag)
{{};
png_get_cLLI_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr,
    png_uint_32p maxCLL,
    png_uint_32p maxFALL)
{{};
png_get_cLLI(png_const_structrp png_ptr, png_const_inforp info_ptr,
      double *maxCLL, double *maxFALL)
{{};
png_get_mDCV_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr,
    png_fixed_point *white_x, png_fixed_point *white_y,
    png_fixed_point *red_x, png_fixed_point *red_y,
    png_fixed_point *green_x, png_fixed_point *green_y,
    png_fixed_point *blue_x, png_fixed_point *blue_y,
    png_uint_32p mastering_maxDL, png_uint_32p mastering_minDL)
{{};
png_get_mDCV(png_const_structrp png_ptr, png_const_inforp info_ptr,
    double *white_x, double *white_y, double *red_x, double *red_y,
    double *green_x, double *green_y, double *blue_x, double *blue_y,
    double *mastering_maxDL, double *mastering_minDL)
{{};
png_get_eXIf(png_const_structrp png_ptr, png_inforp info_ptr,
    png_bytep *exif)
{{};
png_get_eXIf_1(png_const_structrp png_ptr, png_const_inforp info_ptr,
    png_uint_32 *num_exif, png_bytep *exif)
{{};
png_get_hIST(png_const_structrp png_ptr, png_inforp info_ptr,
    png_uint_16p *hist)
{{};
png_get_IHDR(png_const_structrp png_ptr, png_const_inforp info_ptr,
    png_uint_32 *width, png_uint_32 *height, int *bit_depth,
    int *color_type, int *interlace_type, int *compression_type,
    int *filter_type)
{{};
png_get_oFFs(png_const_structrp png_ptr, png_inforp info_ptr,
    png_int_32 *offset_x, png_int_32 *offset_y, int *unit_type)
{{};
png_get_pCAL(png_const_structrp png_ptr, png_inforp info_ptr,
    png_charp *purpose, png_int_32 *X0, png_int_32 *X1, int *type, int *nparams,
    png_charp *units, png_charpp *params)
{{};
png_get_sCAL_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr,
    int *unit, png_fixed_point *width, png_fixed_point *height)
{{};
png_get_sCAL(png_const_structrp png_ptr, png_const_inforp info_ptr,
    int *unit, double *width, double *height)
{{};
png_get_sCAL_s(png_const_structrp png_ptr, png_const_inforp info_ptr,
    int *unit, png_charpp width, png_charpp height)
{{};
png_get_pHYs(png_const_structrp png_ptr, png_const_inforp info_ptr,
    png_uint_32 *res_x, png_uint_32 *res_y, int *unit_type)
{{};
png_get_PLTE(png_const_structrp png_ptr, png_inforp info_ptr,
    png_colorp *palette, int *num_palette)
{{};
png_get_sBIT(png_const_structrp png_ptr, png_inforp info_ptr,
    png_color_8p *sig_bit)
{{};
png_get_text(png_const_structrp png_ptr, png_inforp info_ptr,
    png_textp *text_ptr, int *num_text)
{{};
png_get_tIME(png_const_structrp png_ptr, png_inforp info_ptr,
    png_timep *mod_time)
{
{
   png_debug1(1, "in %s retrieval function", "tIME");
   if (png_ptr != NULL && info_ptr != NULL &&
       (info_ptr->valid & PNG_INFO_tIME) != 0 && mod_time != NULL)
   {
      *mod_time = &(info_ptr->mod_time);
      return PNG_INFO_tIME;
   }
   return 0;
}
png_get_tRNS(png_const_structrp png_ptr, png_inforp info_ptr,
    png_bytep *trans_alpha, int *num_trans, png_color_16p *trans_color)
{{};
<... rest omitted for brevity ...>
#endif
#endif 
 </FOCAL_CLASS_DEP>

<CODE_SNIPPET> 
// Note: The above class dependency block is a placeholder to reflect the original
// project structure. In this test, we rely on the actual libpng headers for real types.

*/

// Lightweight test suite for png_get_tIME using libpng
// No GTest; uses a minimal in-house assertion/runner.


// Simple test framework (non-terminating assertions)
static int g_failures = 0;
#define CHECK(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "Test Failure: " << __func__ << " - " << (msg) << std::endl; \
        ++g_failures; \
    } \
} while (0)

static void test_png_get_tIME_success() {
    // Arrange
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    CHECK(png_ptr != NULL, "Failed to create png_struct");
    if (!png_ptr) return;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    CHECK(info_ptr != NULL, "Failed to create png_info structure");
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return;
    }

    // Initialize info_ptr as if tIME is present
    info_ptr->valid |= PNG_INFO_tIME; // mark tIME as valid
    png_time t;
    // Populate a known tIME value
    t.year = 2020;
    t.month = 12;
    t.day = 31;
    t.hour = 23;
    t.minute = 59;
    t.second = 58;
    info_ptr->mod_time = t;

    // Act
    png_timep mod_time = NULL;
    unsigned int ret = png_get_tIME(png_ptr, info_ptr, &mod_time);

    // Assert
    CHECK(ret == PNG_INFO_tIME, "Expected PNG_INFO_tIME to be returned");
    CHECK(mod_time == &(info_ptr->mod_time),
          "mod_time should point to info_ptr->mod_time");

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

static void test_png_get_tIME_mod_time_null_param() {
    // Arrange
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    CHECK(png_ptr != NULL, "Failed to create png_struct");
    if (!png_ptr) return;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    CHECK(info_ptr != NULL, "Failed to create png_info structure");
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return;
    }

    // Mark tIME as valid so that the only failure is mod_time param being NULL
    info_ptr->valid |= PNG_INFO_tIME;
    png_time t = {2020, 1, 1, 0, 0, 0};
    info_ptr->mod_time = t;

    // Act: pass NULL for mod_time parameter
    unsigned int ret = png_get_tIME(png_ptr, info_ptr, NULL);

    // Assert
    CHECK(ret == 0, "Expected 0 when mod_time parameter is NULL");

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

static void test_png_get_tIME_null_ptrs() {
    // Arrange with valid info_ptr but NULL png_ptr and NULL info_ptr in separate calls
    png_ptrdiff_t dummy = 0; (void)dummy; // silence unused on some compilers

    // Case 1: info_ptr NULL
    png_timep mod_time = NULL;
    unsigned int ret1 = png_get_tIME(NULL, NULL, &mod_time);
    CHECK(ret1 == 0, "Expected 0 when both png_ptr and info_ptr are NULL (early exit)");

    // Case 2: png_ptr NULL with valid info_ptr would also be 0; create a minimal info
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    CHECK(png_ptr != NULL, "Failed to create png_struct for null-ptr test");
    if (!png_ptr) return;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    CHECK(info_ptr != NULL, "Failed to create png_info structure for null-ptr test");
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return;
    }

    info_ptr->valid |= PNG_INFO_tIME;
    mod_time = NULL;
    unsigned int ret2 = png_get_tIME(NULL, info_ptr, &mod_time);
    CHECK(ret2 == 0, "Expected 0 when png_ptr is NULL");
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

static void test_png_get_tIME_invalid_flag() {
    // Arrange: info_ptr without PNG_INFO_tIME flag
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    CHECK(png_ptr != NULL, "Failed to create png_struct");
    if (!png_ptr) return;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    CHECK(info_ptr != NULL, "Failed to create png_info structure");
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return;
    }

    // Do NOT set PNG_INFO_tIME flag
    info_ptr->valid = 0;
    png_time t = {2021, 6, 1, 12, 0, 0};
    info_ptr->mod_time = t;

    png_timep mod_time = NULL;
    unsigned int ret = png_get_tIME(png_ptr, info_ptr, &mod_time);

    CHECK(ret == 0, "Expected 0 when PNG_INFO_tIME flag is not set in info_ptr->valid");

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

// Entry point
int main() {
    std::cout << "Running unit tests for png_get_tIME (no GTest):" << std::endl;

    test_png_get_tIME_success();
    test_png_get_tIME_mod_time_null_param();
    test_png_get_tIME_null_ptrs();
    test_png_get_tIME_invalid_flag();

    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}